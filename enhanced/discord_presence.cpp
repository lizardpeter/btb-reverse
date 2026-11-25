#include "discord_presence.hpp"
#include "presentation_policy.hpp"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <array>
#include <chrono>
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>
namespace btb::enhanced {
namespace {
using namespace std::chrono_literals;
struct Handle{
    HANDLE value{INVALID_HANDLE_VALUE};
    ~Handle(){if(value&&value!=INVALID_HANDLE_VALUE)CloseHandle(value);}
    explicit operator bool()const{return value&&value!=INVALID_HANDLE_VALUE;}
};
bool transfer(HANDLE pipe,void* data,DWORD bytes,bool write){
    auto* cursor=static_cast<char*>(data);
    while(bytes){
        Handle event{CreateEventW(nullptr,TRUE,FALSE,nullptr)};if(!event)return false;
        OVERLAPPED op{};op.hEvent=event.value;DWORD n=0;
        const BOOL ok=write?WriteFile(pipe,cursor,bytes,&n,&op):ReadFile(pipe,cursor,bytes,&n,&op);
        if(!ok){
            if(GetLastError()!=ERROR_IO_PENDING)return false;
            if(WaitForSingleObject(event.value,250)!=WAIT_OBJECT_0){
                CancelIoEx(pipe,&op);GetOverlappedResult(pipe,&op,&n,TRUE);return false;
            }
            if(!GetOverlappedResult(pipe,&op,&n,FALSE))return false;
        }
        if(!n||n>bytes)return false;
        cursor+=n;bytes-=n;
    }
    return true;
}
bool send(HANDLE pipe,uint32_t opcode,const std::string& json){
    if(json.size()>65536)return false;
    std::vector<char> data(8+json.size());
    uint32_t length=static_cast<uint32_t>(json.size());
    for(int i=0;i<4;++i){data[i]=char(opcode>>(8*i));data[4+i]=char(length>>(8*i));}
    std::copy(json.begin(),json.end(),data.begin()+8);
    return transfer(pipe,data.data(),static_cast<DWORD>(data.size()),true);
}
bool receive(HANDLE pipe,uint32_t& opcode,std::string& body){
    std::array<unsigned char,8> header{};
    if(!transfer(pipe,header.data(),8,false))return false;
    opcode=0;uint32_t size=0;
    for(int i=0;i<4;++i){opcode|=uint32_t(header[i])<<(8*i);size|=uint32_t(header[i+4])<<(8*i);}
    if(size>65536)return false;
    body.resize(size);
    return !size||transfer(pipe,body.data(),size,false);
}
HANDLE connect(){
    for(int i=0;i<10;++i){
        auto name=std::wstring(L"\\\\.\\pipe\\discord-ipc-")+std::to_wstring(i);
        HANDLE h=CreateFileW(name.c_str(),GENERIC_READ|GENERIC_WRITE,0,nullptr,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,nullptr);
        if(h!=INVALID_HANDLE_VALUE)return h;
    }
    return INVALID_HANDLE_VALUE;
}
bool delay(std::stop_token stop,int count){
    for(int i=0;i<count&&!stop.stop_requested();++i)std::this_thread::sleep_for(100ms);
    return !stop.stop_requested();
}
std::string activity_json(std::string_view text,uint64_t start,uint64_t nonce,bool clear=false){
    const auto details=clear?std::string("null"):
        "{\"type\":0,\"details\":\"Bob Builds a Park Enhanced\",\"state\":\""+std::string(text)+
        "\",\"timestamps\":{\"start\":"+std::to_string(start)+"}}";
    return "{\"cmd\":\"SET_ACTIVITY\",\"args\":{\"pid\":"+std::to_string(GetCurrentProcessId())+
        ",\"activity\":"+details+"},\"nonce\":\""+std::to_string(nonce)+"\"}";
}
}
void DiscordPresence::start(){
    char id[32]{};const auto len=GetEnvironmentVariableA("BTB_DISCORD_APP_ID",id,32);
    if(!len||len>=32||!valid_discord_id({id,len}))return;
    if(worker_.joinable())return;
    worker_=std::jthread([this,app=std::string(id,len)](std::stop_token stop){
        try{
            const auto started=static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()).count());
            uint64_t nonce=0;
            while(!stop.stop_requested()){
                Handle pipe{connect()};
                if(!pipe){delay(stop,50);continue;}
                DWORD mode=PIPE_READMODE_BYTE;
                if(!SetNamedPipeHandleState(pipe.value,&mode,nullptr,nullptr)){delay(stop,50);continue;}
                uint32_t opcode=0;std::string body;
                if(!send(pipe.value,0,"{\"v\":1,\"client_id\":\""+app+"\"}")||
                   !receive(pipe.value,opcode,body)||opcode!=1||body.find("\"READY\"")==std::string::npos){
                    delay(stop,50);continue;
                }
                std::string_view last;
                auto next=std::chrono::steady_clock::now();
                bool alive=true;
                while(alive&&!stop.stop_requested()){
                    DWORD available=0;
                    if(!PeekNamedPipe(pipe.value,nullptr,0,nullptr,&available,nullptr)){alive=false;break;}
                    if(available>=8){
                        if(!receive(pipe.value,opcode,body)||opcode==2){alive=false;break;}
                        if(opcode==3&&!send(pipe.value,4,body)){alive=false;break;}
                    }
                    auto text=presence_for_state(state_.load(std::memory_order_relaxed));
                    const auto now=std::chrono::steady_clock::now();
                    if(text!=last&&now>=next){
                        if(!send(pipe.value,1,activity_json(text,started,++nonce))){alive=false;break;}
                        last=text;next=now+15s;
                    }
                    delay(stop,2);
                }
                if(alive&&stop.stop_requested())(void)send(pipe.value,1,activity_json({},started,++nonce,true));
                if(!stop.stop_requested())delay(stop,50);
            }
        }catch(...){/* Discord is never a gameplay failure. */}
    });
}
}
