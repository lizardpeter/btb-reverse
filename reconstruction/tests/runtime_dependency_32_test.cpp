#include "btb/runtime_dependency_32.hpp"
#include <cassert>
#include <cstring>
#include <array>
#include <vector>
#include <string>
#include <span>
using namespace btb::runtime_batch_32;
int main(){
    // 00430196; 004331CB
    std::string events;
    bool cpu=false;
    initialize_crt_runtime([&]{events+='I';},[&]{events+='Q';return true;},
        cpu,[&]{events+='P';});
    assert(cpu&&events=="IQP");
    events.clear();
    assert(check_processor_feature(
        [&]()->std::optional<bool>{events+='K';return std::nullopt;},
        [&]{events+='F';return true;}));
    assert(events=="KF");
    events.clear();
    assert(!check_processor_feature(
        [&]()->std::optional<bool>{events+='K';return false;},
        [&]{events+='F';return true;}));
    assert(events=="K");

    // 00436010, 004310E0, 00431105
    std::vector<I32> notices;
    runtime_processor_warning(1,0,[&](I32 v){notices.push_back(v);},
                              [&]{notices.push_back(77);});
    assert((notices==std::vector<I32>{252,77,255}));
    notices.clear();runtime_processor_warning(0,0,
        [&](I32 v){notices.push_back(v);},{});
    assert(notices.empty());
    runtime_amessage_exit(1,12,[&]{notices.push_back(91);},
        [&](I32 v){notices.push_back(v);},
        [&](I32 v){notices.push_back(v);});
    assert((notices==std::vector<I32>{91,12,255}));
    notices.clear();
    runtime_fatal_process(0,42,{},[&](I32 v){notices.push_back(v);},
        [&](I32 v){notices.push_back(v);});
    assert((notices==std::vector<I32>{42,255}));

    // 00433596 and 00436EE9
    U32 guard=99;char buffer[32]{};
    scoped_float_conversion(guard,0x44,buffer,sizeof(buffer),
      [&](U32 fmt,char* output,std::size_t n){
        assert(guard==1&&fmt==0x44&&n==32);
        std::strcpy(output,"1.25");
      });
    assert(guard==0&&std::strcmp(buffer,"1.25")==0);
    LocaleState locale{};
    locale.classification.fill(0xff);
    locale.c6b8=3;locale.c6cc=4;locale.c8e4=5;locale.c6c0=6;
    locale.c6c4=7;locale.c6c8=8;
    reset_locale_state(locale);
    for(auto b:locale.classification)assert(b==0);
    assert(locale.c6b8==0&&locale.c6cc==0&&locale.c8e4==0);
    assert(locale.c6c0==0&&locale.c6c4==0&&locale.c6c8==0);

    // 00436E6C system selection
    U32 system_cp=5;
    assert(choose_codepage(-2,1252,system_cp,[]{return 437;},[]{return 1252;})==437);
    assert(system_cp==1);
    assert(choose_codepage(-3,1252,system_cp,[]{return 437;},[]{return 1252;})==1252);
    assert(system_cp==1);
    assert(choose_codepage(-4,932,system_cp,{},{})==932&&system_cp==1);
    assert(choose_codepage(65001,932,system_cp,{},{})==65001&&system_cp==0);

    // 004311DC original FILE buffer free mask; 0042FB62 fclose.
    File f{5,0x8b,9,123,456,777};
    std::vector<U32> freed;
    assert(free_crt_buffer(f,[&](U32 p){freed.push_back(p);}));
    assert(f.pointer==0); // portable buffer pointer zero
    assert(f.base==0&&f.count==0&&(f.flags&8)==0);
    assert((freed==std::vector<U32>{456}));
    File skip{1,0x42,7,8,9,10};
    assert(!free_crt_buffer(skip,[&](U32){assert(false);}));
    File closable{12,0x8b,7,1,456,777};
    I32 ret=close_crt_file(closable,
      [&](File&){events+='F';return 1;},
      [&](U32 p){freed.push_back(p);},
      [&](I32 fd){events+='C';assert(fd==7);return 0;});
    assert(ret==1&&closable.flags==0&&closable.tmpname==0);
    assert(freed.back()==777&&events.ends_with("FC"));
    File invalid{0,0x40,0,0,0,0};
    assert(close_crt_file(invalid,{},{},{})==-1&&invalid.flags==0);
    File close_fail{0,0x83,10,0,0,0};
    assert(close_crt_file(close_fail,
        [](File&){return 0;},{},[](I32){return -1;})==-1);

    // 00431207 flush_one, 004312A7 flsall
    File writer{0,0x83,10,0,0,0};
    assert(flush_one(&writer,{},[](File&){return 1;},{})==-1);
    writer.flags|=0x40;
    assert(flush_one(&writer,{},[](File&){return 0;},[](I32){return 0;})==0);
    assert(flush_one(nullptr,[]{return 5;},{},{})==5);
    File one{0,0x83,0,0,0,0},two{0,0x82,0,0,0,0},three{0,0,0,0,0,0};
    std::array<File*,4> streams{&one,&two,&three,nullptr};
    int flush_count=0;
    assert(flush_all(streams,1,[&](File*){++flush_count;return 0;})==2);
    assert(flush_count==2);
    flush_count=0;
    assert(flush_all(streams,0,[&](File*){++flush_count;return -1;})==-1);
    assert(flush_count==1);

    // 00430FB8 stream scan lock/unlock
    events.clear();
    I32 scanned=formatted_scan_wrapper(one,"%d",
      [&](File&){events+='L';return 11;},
      [&](File&,const char* fmt){events+='S';assert(std::strcmp(fmt,"%d")==0);return 3;},
      [&](I32 guard,File&){assert(guard==11);events+='U';});
    assert(scanned==3&&events=="LSU");

    // 00430CB0 selection sort with original maximum-last semantics.
    std::array<int,6> list{8,2,9,1,5,5};
    selection_sort_original<int>(list,[](int a,int b){return (a>b)-(a<b);});
    assert((list==std::array<int,6>{1,2,5,5,8,9}));
    // 004392DE CompareStringA errors and flag-zero special case
    assert(mbsnbicoll(0,{})==0);
    assert(mbsnbicoll(5,[](std::size_t n){assert(n==5);return 3;})==1);
    assert(mbsnbicoll(10,[](std::size_t){return 0;})==0x7fffffff);

    // 00439ACC environment name matching with exact '=' terminator gate
    const std::vector<std::string> env{"TEMP=foo","TEMPERATURE=hot","PATH=/bin"};
    const auto compare=[](std::string_view a,std::string_view b){
        return a==b?0:a<b?-1:1;};
    assert(environment_find(env,"TEMP",compare)==0);
    assert(environment_find(env,"PATH",compare)==2);
    assert(environment_find(env,"PAT",compare)==-3);
    assert(environment_find(env,"NOTFOUND",compare)==-3);

    // 004343FC and 0043439F runtime heap allocation and algorithm dispatch
    HeapGlobals heap{};
    assert(!initialize_heap_slab(heap,12,[](std::size_t){return 0U;}));
    assert(initialize_heap_slab(heap,12,[](std::size_t n){assert(n==320);return 0xf00U;}));
    assert(heap.root==0xf00&&heap.current==0xf00&&heap.step==16);
    assert(heap.control==12);
    I32 initialized=0;bool destroyed=false;
    assert(initialize_runtime_heap(heap,true,
      [](bool isolated,std::size_t size){assert(isolated&&size==4096);return 666U;},
      []{return 3;},[&](I32 mode){initialized=mode;return true;},
      [&](U32){destroyed=true;}));
    assert(initialized==3&&!destroyed&&heap.handle==666);
    assert(!initialize_runtime_heap(heap,false,
      [](bool,std::size_t){return 45U;},[]{return 2;},
      [](I32){return false;},[&](U32 ptr){assert(ptr==45);destroyed=true;}));
    assert(destroyed);

    // 00437371 original CRT 4KiB stream allocation and embedded fallback
    StreamBuffer sb{};U32 alloc_calls=0;
    sb.inline_buffer=0x10;
    allocate_crt_stream_buffer(sb,alloc_calls,
      [](std::size_t size){assert(size==4096);return 77U;});
    assert(alloc_calls==1&&sb.base==77&&sb.ptr==77);
    assert(sb.capacity==4096&&(sb.flags&8)!=0&&sb.count==0);
    StreamBuffer fallback{};fallback.inline_buffer=88;
    allocate_crt_stream_buffer(fallback,alloc_calls,
      [](std::size_t){return 0U;});
    assert(alloc_calls==2&&fallback.base==88&&fallback.capacity==2);
    assert((fallback.flags&4)!=0);

    // 004351F6 free-slot table bit accumulation, page release threshold.
    U32 available=239,full=31;U8 freed_byte=1,indicator=0;
    notices.clear();
    release_heap_slot(available,freed_byte,indicator,full,
      [&](I32 v){notices.push_back(v);});
    assert(available==240&&freed_byte==0&&indicator==0xf1);
    assert(full==32&&notices.size()==1&&notices[0]==16);
    freed_byte=2;release_heap_slot(available,freed_byte,indicator,full,{});
    assert(available==242&&full==32);

    // 004358F3 commandline quoted and unquoted path scanning
    assert(next_command_token_end("\"my exe\"   -flag",0,{})==11);
    assert(next_command_token_end("hello world",0,{})==6);
    assert(next_command_token_end("hello world",6,{})==11);
    // 004325C0: two distinct notify/cleanup phases
    events.clear();
    call_setting_frame(256,[&](I32 v){assert(v==256);events+='A';},
      [&]{events+='C';},[&](I32 v){assert(v==2);events+='B';});
    assert(events=="ACB");

    // 004332B4 two modes of CRT floating-result assignment
    const auto two_word_result=assign_float_result(true,[]{return std::array<U32,2>{2,3};},{});
    assert(two_word_result[0]==2&&two_word_result[1]==3);
    const auto one_word=assign_float_result(false,{},[]{return 19U;});
    assert(one_word[0]==19&&one_word[1]==0);

    // 00430F5C exactly lowercase digit mapping 'W'+digit, negative.
    assert(write_radix(255,16,false).value()=="ff");
    assert(write_radix(0,10,false).value()=="0");
    assert(write_radix(12,10,true).value()=="-4294967284");
    assert(!write_radix(1,1,false).has_value());
    // 00433720, 004337A0, 004381B0
    assert(retail_strlen("abc")==3&&retail_strlen("")==0);
    assert(retail_strcmp("ab","ab")==0);
    assert(retail_strcmp("a","b")<0&&retail_strcmp("b","a")>0);
    assert(retail_strcmp("a","ab")<0);
    const char* sentence="ABCDEFGABC";
    assert(retail_strstr(sentence,"ABC")==sentence);
    assert(retail_strstr(sentence,"GAB")==sentence+6);
    assert(retail_strstr(sentence,"")==sentence);
    assert(retail_strstr(sentence,"no")==nullptr);

    // 00439E2B multibyte-aware strchr, lead 0x81 triggers two-byte match.
    const U8 chars[]{'a',0x81,0x43,'z',0};
    std::array<U8,256> flags{};flags[0x81]=4;
    assert(multibyte_strchr(chars,'a',true,flags)==chars);
    assert(multibyte_strchr(chars,0x8143,true,flags)==chars+1);
    assert(multibyte_strchr(chars,'z',true,flags)==chars+3);
    assert(multibyte_strchr(chars,0,true,flags)==chars+4);
    assert(multibyte_strchr(chars,0x43,true,flags)==nullptr);

    // 0043277A source Win32 error range fallback, mapped entries exact
    constexpr std::array<std::pair<I32,I32>,2> mapped{{{2,2},{5,13}}};
    assert(translate_winerror(5,mapped)==13);
    assert(translate_winerror(20,mapped)==13);
    assert(translate_winerror(193,mapped)==8);
    assert(translate_winerror(1,mapped)==22);

    // 00439B24 duplicate environment with original copying callback
    std::vector<std::string> names{"KEY=VALUE","TEMP=YES"};
    auto copied=duplicate_environment(&names,[](const std::string& x)
      ->std::optional<std::string>{return x;});
    assert(copied&&*copied==names);
    assert(!duplicate_environment(nullptr,{}));
    assert(!duplicate_environment(&names,[](const std::string&)
      ->std::optional<std::string>{return std::nullopt;}));

    // 004373DB WC to UTF8 native callback, and failure sets errno=42
    char mb[8]{};I32 err=0;
    assert(convert_wide_character(mb,'A',false,err,{})==1&&mb[0]=='A');
    assert(convert_wide_character(mb,0x20ac,true,err,[](U16)
      ->std::optional<std::string>{return std::string("\xe2\x82\xac");})==3);
    assert(convert_wide_character(mb,0x1234,true,err,[](U16)
      ->std::optional<std::string>{return std::nullopt;})==-1&&err==42);
    assert(convert_wide_character(nullptr,33,false,err,{})==0);

    // 0043748E all major x87 mask source branches
    assert(translate_fpu_control(0)==0x20000);
    assert(translate_fpu_control(1|2|4|8|16|32)==(0x80000U|0x1fU|0x20000U));
    assert((translate_fpu_control(0x400)&0x300)==0x100);
    assert((translate_fpu_control(0x800)&0x300)==0x200);
    assert((translate_fpu_control(0xc00)&0x300)==0x300);
    assert((translate_fpu_control(0x200)&0x10000)!=0);
    assert((translate_fpu_control(0x1000)&0x40000)!=0);
}
