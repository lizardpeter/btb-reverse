#pragma once
#include <bit>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>
namespace batch20 {
inline std::uint32_t word(std::istream& in){std::uint32_t value{};in>>std::hex>>value;return value;}
inline std::int32_t integer(std::istream& in){return std::bit_cast<std::int32_t>(word(in));}
inline std::string bytes(std::istream& in){std::string hex,out;in>>hex;if(hex=="-")return out;for(std::size_t i=0;i<hex.size();i+=2)out.push_back(static_cast<char>(std::stoul(hex.substr(i,2),nullptr,16)));return out;}
inline std::vector<std::uint32_t> trace(std::istream& in){std::vector<std::uint32_t> out;const auto n=word(in);for(unsigned i=0;i<n;++i)out.push_back(word(in));return out;}
inline std::uint32_t get32(const std::byte* p){std::uint32_t v{};for(unsigned i=0;i<4;++i)v|=std::to_integer<std::uint32_t>(p[i])<<(8*i);return v;}
}
