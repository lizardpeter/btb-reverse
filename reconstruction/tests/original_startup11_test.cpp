#include "btb/original_startup11.hpp"
#include <bit>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
using namespace btb::original_startup11;
std::string bytes(const std::string& hex){
    if(hex=="-")return {};
    std::string out;
    for(std::size_t i=0;i<hex.size();i+=2)out.push_back(static_cast<char>(std::stoul(hex.substr(i,2),nullptr,16)));
    return out;
}
struct Memory final:GuestMemory {
    std::unordered_map<U32,U32> words;
    U32 read32(U32 at) override{return words.at(at);}
};
int main(int argc,char** argv){
    if(!verify_11_original_startup_functions()){std::cerr<<"Startup/I/O reconstruction failed\n";return 1;}
    if(argc!=2)return 2;
    std::ifstream file(argv[1]);if(!file)return 3;
    std::string line,kind;unsigned checked=0;
    while(std::getline(file,line)) {
        std::istringstream in(line);in>>kind>>std::hex;bool okay=false;
        if(kind=="ARGV") {
            std::string command;U32 count,size;in>>command>>count>>size;
            std::vector<std::string> expected;std::string word;
            for(U32 i=1;i<count;++i){in>>word;expected.push_back(bytes(word));}
            std::array<std::uint8_t,257> flags{};
            const auto actual=parse_command_line(bytes(command).c_str(),flags);
            okay=actual.values==expected && actual.pointer_count==count && actual.byte_count==size;
        } else if(kind=="COUNT") {
            std::string command;U32 count,size;in>>command>>count>>size;
            std::array<std::uint8_t,257> flags{};flags[0x81+1]=4;
            const auto actual=parse_command_line(bytes(command).c_str(),flags,true);
            okay=actual.pointer_count==count && actual.byte_count==size;
        } else if(kind=="CAT") {
            std::string left,right,expected;in>>left>>right>>expected;
            const auto a=bytes(left),b=bytes(right);std::array<char,256> buffer{};
            std::copy(a.begin(),a.end(),buffer.begin());
            okay=concatenate(buffer.data(),b.c_str())==buffer.data() && buffer.data()==bytes(expected);
        } else if(kind=="PMD") {
            U32 a,b,c,d,expected;in>>a>>b>>c>>d>>expected;Memory mem;
            if(std::bit_cast<I32>(b)>=0) {mem.words[0x800000+b]=0x801000;mem.words[0x801000+c]=d;}
            okay=adjust_pointer(0x800000,{std::bit_cast<I32>(a),std::bit_cast<I32>(b),std::bit_cast<I32>(c)},mem)-0x800000==expected;
        }
        if(!in || !okay){std::cerr<<"Mismatch at original startup vector "<<checked<<": "<<line<<'\n';return 4;}
        ++checked;
    }
    if(checked!=3272)return 5;
    std::cout<<checked<<" original x86 startup comparisons pass\n";
}
