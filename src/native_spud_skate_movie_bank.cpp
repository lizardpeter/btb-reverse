#include "btb/native_spud_skate_movie_bank.hpp"
#include <fstream>
#include <utility>
#include <vector>

namespace btb::full_game {
using namespace bink32_recovered;
bool NativeSpudSkateMovieBank::open(const std::array<std::filesystem::path,5>& paths,std::string& error) {
    if(active_){error="native Spud Skate bank already open";return false;}
    decltype(streams_) candidate{};
    for(std::size_t i=0;i<paths.size();++i) {
        std::ifstream file(paths[i],std::ios::binary|std::ios::ate);
        const auto n=file?file.tellg():std::streampos(-1);
        if(!spud_skate::original_skate_movie_name_matches(i,paths[i].filename().string())||n<=0||n>7000000) {
            error="missing or oversized original Spud Skate Bink: "+paths[i].string();return false;
        }
        std::vector<std::uint8_t> bytes(static_cast<std::size_t>(n));file.seekg(0);
        if(!file.read(reinterpret_cast<char*>(bytes.data()),static_cast<std::streamsize>(bytes.size()))) {
            error="cannot read Spud Skate Bink bytes";return false;
        }
        const auto index=parse_container_index(bytes);
        if(!index||index->header.magic!=Magic::SJPi||index->header.width!=600||index->header.height!=380||
           !index->header.frame_count||(i<4&&index->header.frame_count<660)||!index->tracks.empty()) {
            error="native Spud Skate requires valid silent 600x380 SJPi streams";return false;
        }
        candidate[i]=BinkNativeVideoStream::open(std::move(bytes));
        if(!candidate[i]){error="cannot open native Spud Skate Bink stream";return false;}
        if(i>0&&i<4&&(candidate[i]->frame_count()!=candidate[0]->frame_count()||
           candidate[i]->frame_rate_numerator()!=candidate[0]->frame_rate_numerator()||
           candidate[i]->frame_rate_denominator()!=candidate[0]->frame_rate_denominator())) {
            error="original Spud Skate quality stream lengths or rates differ";return false;
        }
    }
    streams_=std::move(candidate);active_=true;end_movie_=faulted_=false;selected_=0;displayed_.reset();
    error.clear();return true;
}
bool NativeSpudSkateMovieBank::fail(std::string& error,const char* fallback) {
    faulted_=true;if(error.empty())error=fallback;return false;
}
bool NativeSpudSkateMovieBank::pace(std::size_t i,std::uint32_t frame,std::string& error) {
    const auto time=streams_[i]->frame_timestamp_us(frame);
    return time&&(!pace_||pace_(*time,error))?true:fail(error,"invalid native Spud Skate frame clock");
}
bool NativeSpudSkateMovieBank::publish(std::size_t i,std::string& error) {
    const auto* frame=streams_[i]->current_frame();
    return frame&&(!present_||present_(i,*frame,error))?true:fail(error,"native Spud Skate frame upload failed");
}
bool NativeSpudSkateMovieBank::decode(std::size_t i,std::string& error) {
    return streams_[i]->decode_next()&&publish(i,error)?true:fail(error,"native Spud Skate frame decode failed");
}
bool NativeSpudSkateMovieBank::observe(SpudSkateMovieFrame& result,std::string& error) {
    result={};error.clear();
    if(!active_||faulted_)return fail(error,"native Spud Skate bank not usable");
    if(end_movie_) {
        const auto next=streams_[4]->frame_number()+1;
        // 00424690 checks Frames <= FrameNum BEFORE decoding.
        if(next>=streams_[4]->frame_count()){result.end_movie_finished=true;return true;}
        if(!pace(4,next,error)||!decode(4,error))return false;
        displayed_=4;return true;
    }
    const auto next=streams_[0]->frame_number()+1;
    for(std::size_t i=0;i<4;++i)
        if(streams_[i]->frame_number()+1!=next||next>streams_[i]->frame_count())
            return fail(error,"native Spud Skate quality streams desynchronized");
    result.primary_decoded_frame=static_cast<int>(next);
    result.four_quality_streams_synchronized=true;
    if(next==streams_[0]->frame_count()) {
        result.quality_stream_reached_end=true;return true;
    }
    if(!pace(selected_,next,error))return false;
    for(std::size_t i=0;i<4;++i)if(!decode(i,error))return false;
    displayed_=selected_;return true;
}
bool NativeSpudSkateMovieBank::select_quality(spud_skate::StuntQuality quality,std::string& error) {
    const int id=static_cast<int>(quality);
    if(!active_||faulted_||end_movie_||id<0||id>=4){error="invalid native Spud Skate quality selection";return false;}
    selected_=static_cast<std::size_t>(id);
    if(displayed_)displayed_=selected_;
    error.clear();return true;
}
bool NativeSpudSkateMovieBank::resynchronize_all(int frame,std::string& error) {
    if(!active_||faulted_||end_movie_||frame<1||frame>660){error="invalid native Spud Skate synchronization frame";return false;}
    error.clear();
    // 00424690 seeks, decodes and uploads frame 44 on ALL streams,
    // advances each cursor, then draws the Good surface and returns.
    for(std::size_t i=0;i<4;++i)
        if(!streams_[i]->seek(static_cast<std::uint32_t>(frame))||!publish(i,error))
            return fail(error,"native Spud Skate exact-frame seek failed");
    displayed_=3;return true;
}
bool NativeSpudSkateMovieBank::begin_end_movie(std::string& error) {
    if(!active_||faulted_||end_movie_){error="native Spud Skate end movie not available";return false;}
    error.clear();
    // Second frame-660 marker presents end.bik frame 1 immediately.
    if(!pace(4,1,error)||!decode(4,error))return false;
    end_movie_=true;displayed_=4;return true;
}
bool NativeSpudSkateMovieBank::close(std::string& error) {
    for(auto& stream:streams_)stream.reset();
    active_=end_movie_=faulted_=false;selected_=0;displayed_.reset();error.clear();return true;
}
}
