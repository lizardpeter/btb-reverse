#include "btb/bink32_ac_command_pass.hpp"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <span>
using namespace btb::bink32_recovered;
#define CHECK(c) do{if(!(c)){std::fprintf(stderr,"Bink AC pass line %d: %s\n",__LINE__,#c);std::abort();}}while(0)
struct Writer {
    std::array<std::uint8_t,32> data{};
    unsigned count{};
    void append(unsigned value,unsigned bits) {
        for(unsigned i=0;i<bits;++i) {
            if((value>>i)&1U)data[count/8]|=std::uint8_t(1U<<(count%8));
            ++count;
        }
    }
    LsbBitReader reader()const{
        return LsbBitReader(std::span<const std::uint8_t>(data).first((count+7)/8));
    }
};
int main() {
    {
        const auto init=initial_bink_ac_pending();
        constexpr std::array<std::uint8_t,6> initial{
            0x10,0x60,0xb0,0x07,0x0b,0x0f
        };
        CHECK(init.pending().size()==6);
        for(std::size_t i=0;i<6;++i)CHECK(init.pending()[i]==initial[i]);
        const std::array<std::uint8_t,1> inactive{0};
        LsbBitReader bits(inactive);
        auto controls=init;
        std::array<std::int16_t,64> values{};values.fill(99);
        const auto run=run_bink_ac_command_pass(bits,controls,values,7,8);
        CHECK(run&&run->activation_reads==6&&run->bit_count==6);
        CHECK(run->remaining_commands==6&&controls.pending().size()==6);
        CHECK(bits.bit_position()==6);
        for(auto x:values)CHECK(x==99);
    }
    {
        // Command 3, active, 7-bit low=0x52, negative sign.
        // This is the original mode-4 EAX=7/class-bit=8 combination.
        constexpr std::array<std::uint8_t,1> code{(5U<<2)|3U};
        auto controls=*make_bink_ac_pending(code);
        Writer writer;
        writer.append(1,1);writer.append(0x52,7);writer.append(1,1);
        auto bits=writer.reader();
        std::array<std::int16_t,64> values{};
        const auto run=run_bink_ac_command_pass(bits,controls,values,7,8);
        CHECK(run&&run->activation_reads==1&&run->written_coefficients==1);
        CHECK(run->remaining_commands==0&&run->bit_count==9);
        CHECK(values[5]==-90&&controls.pending()[0]==0);
        CHECK(bits.bit_position()==9);
    }
    {
        // Opcode 1 rewrites current command and APPENDS 3 commands.
        // Four subsequent zero activation bits leave all 4 commands
        // to be handled in the next original magnitude-class pass.
        constexpr std::array<std::uint8_t,1> code{0x25};
        auto controls=*make_bink_ac_pending(code);
        Writer writer;writer.append(1,1);writer.append(0,4);
        auto bits=writer.reader();
        std::array<std::int16_t,64> values{};
        const auto run=run_bink_ac_command_pass(bits,controls,values,3,4);
        CHECK(run&&run->expanded_commands==3);
        CHECK(run->activation_reads==5&&run->bit_count==5);
        CHECK(run->remaining_commands==4&&controls.pending().size()==4);
        constexpr std::array<std::uint8_t,4> expected{
            0x26,0x36,0x46,0x56
        };
        for(unsigned i=0;i<4;++i)CHECK(controls.pending()[i]==expected[i]);
    }
    {
        // Opcode 0: activation triggers four deferred controls,
        // prepended for NEXT magnitude class, retains rewritten
        // source command at old position.
        constexpr std::array<std::uint8_t,1> code{20U<<2};
        auto controls=*make_bink_ac_pending(code);
        Writer writer;writer.append(1,1);writer.append(15,4);
        writer.append(0,1); // do not activate the rewritten code 0x61
        auto bits=writer.reader();
        std::array<std::int16_t,64> values{};values.fill(7);
        const auto run=run_bink_ac_command_pass(bits,controls,values,0,1);
        CHECK(run&&run->activation_reads==2&&run->deferred_commands==4);
        CHECK(run->remaining_commands==5&&run->bit_count==6);
        CHECK(controls.pending().size()==5);
        CHECK(controls.pending()[0]==((23U<<2)|3U));
        CHECK(controls.pending()[1]==((22U<<2)|3U));
        CHECK(controls.pending()[2]==((21U<<2)|3U));
        CHECK(controls.pending()[3]==((20U<<2)|3U));
        CHECK(controls.pending()[4]==((20U<<2)+0x11U));
        for(auto x:values)CHECK(x==7);
    }
    {
        // If a later command overruns the source, the ENTIRE pass
        // must roll back cursor, controls and coefficient writes.
        constexpr std::array<std::uint8_t,2> code{
            std::uint8_t((3U<<2)|3U),std::uint8_t((4U<<2)|3U)
        };
        auto controls=*make_bink_ac_pending(code);
        const auto before=controls;
        const std::array<std::uint8_t,1> short_stream{0x01};
        LsbBitReader bits(short_stream);
        std::array<std::int16_t,64> values{};values.fill(44);
        CHECK(!run_bink_ac_command_pass(bits,controls,values,7,8));
        CHECK(bits.bit_position()==0&&controls.head==before.head);
        CHECK(controls.end==before.end&&controls.data==before.data);
        for(auto x:values)CHECK(x==44);
    }
    {
        const std::array<std::uint8_t,1> zero{0};
        LsbBitReader bits(zero);
        std::array<std::int16_t,64> values{};
        auto malformed=initial_bink_ac_pending();
        malformed.head=600;
        CHECK(!run_bink_ac_command_pass(bits,malformed,values,7,8));
        CHECK(bits.bit_position()==0);
        auto valid=initial_bink_ac_pending();
        CHECK(!run_bink_ac_command_pass(bits,valid,values,32,8));
        // Large requested seed is rejected rather than overflowing.
        std::array<std::uint8_t,257> huge{};
        CHECK(!make_bink_ac_pending(huge));
    }
    std::puts("Bink 2002 coefficient mutable head/current/end pass: PASS");
}
