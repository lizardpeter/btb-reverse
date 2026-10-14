#include "btb/original_runtime16.hpp"
#include "btb/original_string8.hpp"
#include <bit>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
namespace {
std::vector<char> bytes(const std::string& hex) {
    std::vector<char> out;
    if(hex.size()%2)throw std::runtime_error("bad original vector hex");
    for(std::size_t i=0;i<hex.size();i+=2)
        out.push_back(static_cast<char>(std::stoul(hex.substr(i,2),nullptr,16)));
    return out;
}
}
int main(int argc,char** argv) {
    if(argc!=2)return 1;
    std::ifstream in(argv[1]);if(!in)return 2;
    std::string line;unsigned rem=0,find=0,move=0;
    while(std::getline(in,line)) {
        std::istringstream row(line);std::string kind;row>>kind;
        if(kind=="REM") {
            std::uint64_t a{},b{},expected{};row>>std::hex>>a>>b>>expected;
            if(!row || b==0 || btb::original_runtime16::unsigned_remainder(a,b)!=expected)return 3;
            ++rem;
        } else if(kind=="FIND") {
            std::uint32_t value{};std::string payload;unsigned offset{};int expected{};
            row>>std::hex>>value>>std::dec>>offset>>payload>>expected;
            auto text=bytes(payload);if(!row || offset>=text.size() || text.back()!=0)return 4;
            const auto* start=text.data()+offset;
            const auto* result=btb::original_runtime16::find_character(text.data()+offset,std::bit_cast<std::int32_t>(value));
            const auto position=result?result-start:-1;
            if(position!=expected)return 5;
            ++find;
        } else if(kind=="MOVE") {
            unsigned src{},dst{},count{};std::string before,after;
            row>>src>>dst>>count>>before>>after;
            auto text=bytes(before);auto expected=bytes(after);
            if(!row || src+count>text.size() || dst+count>text.size())return 6;
            if(btb::original_string8::move_bytes(text.data()+dst,text.data()+src,count)!=text.data()+dst || text!=expected)return 7;
            ++move;
        } else return 8;
    }
    if(rem!=1030 || find!=144 || move!=704)return 9;
    std::cout<<"Matched original x86: "<<rem<<" remainder, "<<find<<" strchr, "<<move<<" memmove vectors\n";
}
