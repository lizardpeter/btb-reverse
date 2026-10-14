#include "btb/original_runtime15.hpp"
#include "btb/original_msvc30.hpp"
#include <bit>
#include <stdexcept>
#include <vector>
namespace btb::original_runtime15 {
namespace {
constexpr std::array<I32,13> normal{-1,30,58,89,119,150,180,211,242,272,303,333,364};
constexpr std::array<I32,13> leap{-1,30,59,90,120,151,181,212,243,273,304,334,365};
I32 days_before(I32 year,I32 month) {
    if(month<1 || month>12)throw std::out_of_range("original month index");
    return (year&3?normal:leap)[static_cast<std::size_t>(month-1)];
}
I32 wrap(std::int64_t value){return std::bit_cast<I32>(static_cast<U32>(value));}
void initialize_time(I32,void* context) {
    auto** pointers=static_cast<void**>(context);
    static_cast<TimeServices*>(pointers[1])->initialize_timezone(*static_cast<TimeState*>(pointers[0]));
}
void clear_floating_actions(SignalState& s) {
    if(s.floating_count<=0)return;
    if(s.floating_first<0 || static_cast<std::size_t>(s.floating_first)>s.actions.size() ||
       static_cast<std::size_t>(s.floating_count)>s.actions.size()-static_cast<std::size_t>(s.floating_first))
        throw std::out_of_range("original floating signal table range");
    for(I32 i=0;i<s.floating_count;++i)s.actions[static_cast<std::size_t>(s.floating_first+i)].action=0;
}
void* allocate_bytes(std::size_t n,void* ctx){return static_cast<InputServices*>(ctx)->allocate(n);}
}
void compute_transition(TimeState& s,bool start,bool relative,I32 year,I32 month,I32 week,I32 weekday,I32 day,I32 hour,I32 minute,I32 second,I32 milliseconds) {
    I32 result=days_before(year,month);
    if(relative) {
        const I32 first=wrap(static_cast<std::int64_t>(year)*365-0x63db+result+1+((year-1)>>2))%7;
        result=wrap(static_cast<std::int64_t>(result)+(weekday<first?1:-6)+static_cast<std::int64_t>(week)*7-first+weekday);
        if(week==5 && (year&3?normal:leap)[static_cast<std::size_t>(month)]<result)result-=7;
    } else result=wrap(static_cast<std::int64_t>(result)+day);
    I32 time=wrap(((static_cast<std::int64_t>(hour)*60+minute)*60+second+(start?0:s.daylight_bias))*1000+milliseconds);
    if(!start) {
        if(time<0){time=wrap(static_cast<std::int64_t>(time)+86400000);--result;}
        else if(time>86399999){time=wrap(static_cast<std::int64_t>(time)-86400000);++result;}
    }
    (start?s.start:s.end)={year,result,time};
}
bool is_daylight_time(TimeState& s,const std::array<I32,9>& tm) {
    if(!s.daylight)return false;
    if(tm[5]!=s.start.year || tm[5]!=s.end.year) {
        if(!s.system_rules) {
            compute_transition(s,true,true,tm[5],4,1,0,0,2,0,0,0);
            compute_transition(s,false,true,tm[5],10,5,0,0,2,0,0,0);
        } else {
            const auto compute=[&](bool start,const SystemTime& rule){
                compute_transition(s,start,rule.year==0,tm[5],rule.month,rule.year?0:rule.day,
                    rule.year?0:rule.weekday,rule.year?rule.day:0,rule.hour,rule.minute,rule.second,rule.milliseconds);
            };
            compute(true,s.daylight_date);compute(false,s.standard_date);
        }
    }
    const I32 day=tm[7];
    if(s.start.day<s.end.day) {
        if(day<s.start.day || day>s.end.day)return false;
        if(day>s.start.day && day<s.end.day)return true;
    } else {
        if(day<s.end.day || day>s.start.day)return true;
        if(day>s.end.day && day<s.start.day)return false;
    }
    const I32 time=wrap(((static_cast<std::int64_t>(tm[2])*60+tm[1])*60+tm[0])*1000);
    return day==s.start.day?time>=s.start.milliseconds:time<s.end.milliseconds;
}
I32 calendar_to_time(TimeState& s,I32 year,I32 month,I32 day,I32 hour,I32 minute,I32 second,I32 dst,TimeServices& services,std::array<I32,2> stack_prefix) {
    const I32 years=wrap(static_cast<std::int64_t>(year)-1900);
    if(years<70 || years>138)return -1;
    if(month<1 || month>12)throw std::out_of_range("original calendar month index");
    I32 yday=normal[static_cast<std::size_t>(month-1)]+day;
    if(!(years&3) && month>2)++yday;
    void* context[2]{&s,&services};
    static_cast<void>(original_msvc30::initialize_zero(s.initialized,initialize_time,context));
    const std::int64_t days=static_cast<std::int64_t>(years)*365+yday+((year-1901)>>2);
    I32 result=wrap(((hour+days*24)*60+minute)*60+s.timezone+0x7c558180LL+second);
    // Original leaves tm_sec/tm_min uninitialized. Explicit incoming stack
    // words retain that observable dependency for boundary comparisons.
    std::array<I32,9> tm{stack_prefix[0],stack_prefix[1],hour,day,month-1,years,0,yday,dst};
    if(dst==1 || (dst==-1 && s.daylight && is_daylight_time(s,tm)))result=wrap(static_cast<std::int64_t>(result)+s.daylight_bias);
    return result;
}
I32 raise_signal(SignalState& s,I32 signal,SignalServices& services) {
    U32* slot=nullptr;
    if(signal==2)slot=&s.handlers[0];
    else if(signal==15)slot=&s.handlers[1];
    else if(signal==21)slot=&s.handlers[2];
    else if(signal==22)slot=&s.handlers[3];
    else if(signal==4 || signal==8 || signal==11) {
        const auto* found=original_runtime16::find_action_record(s.actions,signal);
        if(!found)throw std::out_of_range("original signal table lacks required signal");
        slot=&s.actions[static_cast<std::size_t>(found-s.actions.data())].action;
    } else return -1;
    const U32 handler=*slot,saved=s.exception_pointers;const I32 fpe=s.floating_point_code;
    if(handler==1)return 0;
    if(!handler)services.exit_process(3);
    if(signal==4 || signal==8 || signal==11)s.exception_pointers=0;
    if(signal==8) {
        s.floating_point_code=140;clear_floating_actions(s);
        services.invoke_floating(handler,8,s.floating_point_code);
        s.floating_point_code=fpe;s.exception_pointers=saved;
    } else {
        *slot=0;services.invoke(handler,signal);
        if(signal==4 || signal==11)s.exception_pointers=saved;
    }
    return 0;
}
I32 exception_to_signal(SignalState& s,U32 code,U32 pointers,SignalServices& services) {
    // Use the same recovered three-word lookup algorithm without aliasing unlike types.
    std::vector<original_msvc30::Triple> table;table.reserve(s.actions.size());
    for(const auto& action:s.actions)table.push_back({std::bit_cast<I32>(action.code),action.signal,std::bit_cast<I32>(action.action)});
    const auto* found=original_msvc30::find_triple(table,std::bit_cast<I32>(code));
    if(!found)return services.unhandled_exception(pointers);
    auto& action=s.actions[static_cast<std::size_t>(found-table.data())];
    const U32 handler=action.action,saved=s.exception_pointers;
    if(!handler)return services.unhandled_exception(pointers);
    if(handler==5){action.action=0;return 1;}
    if(handler!=1) {
        s.exception_pointers=pointers;
        if(action.signal==8) {
            clear_floating_actions(s);
            const I32 saved_fpe=s.floating_point_code;
            switch(action.code) { // Action column is cleared; exception code survives.
                case 0xc000008e:s.floating_point_code=131;break;
                case 0xc0000090:s.floating_point_code=129;break;
                case 0xc0000091:s.floating_point_code=132;break;
                case 0xc0000093:s.floating_point_code=133;break;
                case 0xc000008d:s.floating_point_code=130;break;
                case 0xc000008f:s.floating_point_code=134;break;
                case 0xc0000092:s.floating_point_code=138;break;
                default:break;
            }
            services.invoke_floating(handler,8,s.floating_point_code);s.floating_point_code=saved_fpe;
        } else {action.action=0;services.invoke(handler,action.signal);}
    }
    s.exception_pointers=saved;return -1;
}
I32 cpp_unhandled_filter(SignalState& s,const ExceptionRecord& record,U32 pointers,SignalServices& services) {
    if(record.code==0xe06d7363 && record.parameter_count==3 && record.first_parameter==0x19930520)
        services.terminate_process();
    if(s.unhandled_hook && services.valid_handler(s.unhandled_hook))return services.invoke_unhandled(s.unhandled_hook,pointers);
    return 0;
}
U32 push_back_byte(U32 value,original_crt10::Stream& stream,U32& allocations,InputServices& services) {
    if(value==0xffffffff || (!(stream.flags&1) && (!(stream.flags&0x80) || (stream.flags&2))))return 0xffffffff;
    if(!stream.base)original_crt10::allocate_file_buffer(stream,allocations,allocate_bytes,&services);
    if(stream.pointer==stream.base) {
        if(stream.count)return 0xffffffff;
        ++stream.pointer;
    }
    --stream.pointer;
    const auto byte=std::byte{static_cast<std::uint8_t>(value)};
    if(stream.flags&0x40){if(*stream.pointer!=byte){++stream.pointer;return 0xffffffff;}}
    else *stream.pointer=byte;
    ++stream.count;stream.flags=(stream.flags&~0x10U)|1;
    return value&255;
}
U32 refill_input(original_crt10::Stream& stream,U32 descriptor,U32& allocations,InputServices& services) {
    const U32 before=stream.flags;
    if(!(before&0x83) || (before&0x40))return 0xffffffff;
    if(before&2){stream.flags|=0x20;return 0xffffffff;}
    stream.flags|=1;
    if(!(before&0x10c))original_crt10::allocate_file_buffer(stream,allocations,allocate_bytes,&services);
    else stream.pointer=stream.base;
    const I32 read=services.read_descriptor(descriptor,stream.base,stream.capacity);
    stream.count=std::bit_cast<U32>(read);
    if(!read || read==-1){stream.flags|=read?0x20:0x10;stream.count=0;return 0xffffffff;}
    if(!(stream.flags&0x82) && (services.descriptor_flags(descriptor)&0x82)==0x82)stream.flags|=0x2000;
    if(stream.capacity==512 && (stream.flags&8) && !(stream.flags&0x400))stream.capacity=4096;
    stream.count=static_cast<U32>(read)-1;
    return std::to_integer<U32>(*stream.pointer++);
}
U32 read_buffered_byte(original_crt10::Stream& stream,U32 descriptor,U32& allocations,InputServices& services) {
    struct Context {original_crt10::Stream& stream;U32 descriptor;U32& allocations;InputServices& services;original_msvc30::InputCursor* cursor{};};
    Context ctx{stream,descriptor,allocations,services};
    original_msvc30::InputCursor cursor{reinterpret_cast<const std::uint8_t*>(stream.pointer),std::bit_cast<I32>(stream.count),&ctx};
    ctx.cursor=&cursor;
    const auto refill=[](void* pointer)->U32 {
        auto& c=*static_cast<Context*>(pointer);
        c.stream.count=std::bit_cast<U32>(c.cursor->remaining);
        c.stream.pointer=const_cast<std::byte*>(reinterpret_cast<const std::byte*>(c.cursor->current));
        return refill_input(c.stream,c.descriptor,c.allocations,c.services);
    };
    const bool cached=cursor.remaining>0;
    const U32 result=original_msvc30::read_input_byte(cursor,refill);
    if(cached){stream.count=std::bit_cast<U32>(cursor.remaining);stream.pointer=const_cast<std::byte*>(reinterpret_cast<const std::byte*>(cursor.current));}
    return result;
}
}
