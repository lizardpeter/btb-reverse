#include "btb/bmp_decoder.hpp"
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <system_error>
#include <vector>

namespace fs=std::filesystem;
int main(int argc,char** argv) {
    if(argc!=2) {
        std::cerr<<"Usage: btb_enhanced_bmp_asset_audit <retail-game-root-or-Data-folder>\n";
        return 2;
    }
    const fs::path root=fs::u8path(argv[1]);
    std::error_code ec;
    if(!fs::is_directory(root,ec)||ec) {
        std::cerr<<"Not a readable directory: "<<root.string()<<"\n";
        return 2;
    }
    std::uint64_t files=0,decoded=0,decoded_bytes=0;
    std::map<unsigned,std::uint64_t> depths;
    std::map<unsigned,std::uint64_t> compression;
    std::vector<std::string> failures;
    fs::recursive_directory_iterator it(root,fs::directory_options::skip_permission_denied,ec),end;
    if(ec) {
        std::cerr<<"Cannot enumerate directory: "<<ec.message()<<"\n";
        return 2;
    }
    for(;it!=end;it.increment(ec)) {
        if(ec) { ec.clear();continue; }
        std::error_code status_error;
        if(!it->is_regular_file(status_error)||status_error)continue;
        auto extension=it->path().extension().string();
        std::transform(extension.begin(),extension.end(),extension.begin(),
            [](unsigned char c){return static_cast<char>(std::tolower(c));});
        if(extension!=".bmp")continue;
        ++files;
        btb::display::DecodedBitmap image;
        std::string error;
        if(!btb::display::decode_bmp_file(it->path(),image,error)) {
            failures.push_back(it->path().string()+": "+error);
            continue;
        }
        ++decoded;
        decoded_bytes+=image.bgra.size();
        ++depths[image.bits_per_pixel];
        ++compression[image.compression];
    }
    std::cout<<"BMP asset audit\n"
             <<"Root: "<<root.string()<<"\n"
             <<"Files discovered: "<<files<<"\n"
             <<"Successfully decoded: "<<decoded<<"\n"
             <<"Decoded pixel memory: "<<decoded_bytes<<" bytes\n"
             <<"Source bit depths:";
    for(const auto& [bits,count]:depths)std::cout<<" "<<bits<<"-bit="<<count;
    std::cout<<"\nCompression:";
    for(const auto& [kind,count]:compression)std::cout<<" BI_"<<kind<<"="<<count;
    std::cout<<"\nFailures: "<<failures.size()<<"\n";
    for(const auto& failure:failures)std::cerr<<failure<<"\n";
    return failures.empty()?0:1;
}
