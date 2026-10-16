#include "btb/bink32_ac_unit_pass.hpp"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <span>
using namespace btb::bink32_recovered;
#define CHECK(x) do {if(!(x)){std::fprintf(stderr,"Bink unit pass line %d: %s\n",__LINE__,#x);std::abort();}}while(0)
struct Writer {
    std::array<std::uint8_t,16> data{};unsigned count{};
    void put(unsigned value,unsigned n){
        for(unsigned i=0;i<n;++i){
            if((value>>i)&1U)data[count/8]|=std::uint8_t(1U<<(count%8));
            ++count;
        }
    }
    LsbBitReader reader()const {
        return LsbBitReader(std::span<const std::uint8_t>(data).first((count+7)/8));
    }
};
int main(){
    {
        constexpr std::array<std::uint8_t,1> cmds{
            std::uint8_t((3U<<2)|3U)
        };
        auto state=*make_bink_ac_pending(cmds);
        Writer w;w.put(1,1);w.put(1,1); // active, negative
        auto bits=w.reader();
        std::array<std::int16_t,64> coeff{};
        auto result=run_bink_ac_unit_pass(bits,state,coeff);
        CHECK(result&&result->written_coefficients==1);
        CHECK(result->consumed_bits==2&&bits.bit_position()==2);
        CHECK(coeff[3]==-1&&state.pending().size()==1);
        CHECK(state.pending()[0]==0);
    }
    {
        // Four positions: signed +1, deferred, signed -1, deferred.
        constexpr std::array<std::uint8_t,1> cmds{
            std::uint8_t((8U<<2)|2U)
        };
        auto state=*make_bink_ac_pending(cmds);
        Writer w;
        w.put(1,1); // activate group
        w.put(0,1);w.put(0,1); // coeff[8] +1
        w.put(1,1); // coeff[9] deferred
        w.put(0,1);w.put(1,1); // coeff[10] -1
        w.put(1,1); // coeff[11] deferred
        auto bits=w.reader();
        std::array<std::int16_t,64> coeff{};coeff.fill(12);
        const auto result=run_bink_ac_unit_pass(bits,state,coeff);
        CHECK(result&&result->written_coefficients==2&&result->deferred_commands==2);
        CHECK(result->consumed_bits==7&&bits.bit_position()==7);
        CHECK(coeff[8]==1&&coeff[9]==12&&coeff[10]==-1&&coeff[11]==12);
        CHECK(state.pending().size()==3);
        CHECK(state.pending()[0]==((11U<<2)|3U));
        CHECK(state.pending()[1]==((9U<<2)|3U));
        CHECK(state.pending()[2]==0);
    }
    {
        // Case 0 rewrites its original control, and the same loop
        // can encounter it AGAIN before exiting current pass.
        constexpr std::array<std::uint8_t,1> cmds{20U<<2};
        auto state=*make_bink_ac_pending(cmds);
        Writer w;w.put(1,1);w.put(15,4);w.put(0,1);
        auto bits=w.reader();
        std::array<std::int16_t,64> coeff{};
        const auto result=run_bink_ac_unit_pass(bits,state,coeff);
        CHECK(result&&result->deferred_commands==4&&result->activations==2);
        CHECK(result->consumed_bits==6&&result->remaining_commands==5);
        CHECK(state.pending()[0]==((23U<<2)|3U));
        CHECK(state.pending()[4]==((20U<<2)+0x11U));
    }
    {
        // Case 1 expands to current group2 and three additional
        // controls, then an inactive bit for each skips without erasure.
        constexpr std::array<std::uint8_t,1> cmds{0x25};
        auto state=*make_bink_ac_pending(cmds);
        Writer w;w.put(1,1);w.put(0,4);
        auto bits=w.reader();
        std::array<std::int16_t,64> coeff{};
        const auto result=run_bink_ac_unit_pass(bits,state,coeff);
        CHECK(result&&result->expanded_commands==3);
        CHECK(result->activations==5&&result->consumed_bits==5);
        CHECK(result->remaining_commands==4);
        CHECK(state.pending()[0]==0x26&&state.pending()[3]==0x56);
    }
    {
        // Two signed commands and only one byte: first succeeds,
        // second cannot complete. Entire output state must roll back.
        constexpr std::array<std::uint8_t,2> cmds{3,7};
        auto state=*make_bink_ac_pending(cmds);
        const auto backup=state;
        const std::array<std::uint8_t,1> stream{1};
        LsbBitReader bits(stream);
        std::array<std::int16_t,64> coeff{};coeff.fill(47);
        CHECK(!run_bink_ac_unit_pass(bits,state,coeff));
        CHECK(bits.bit_position()==0&&state.head==backup.head);
        CHECK(state.end==backup.end&&state.data==backup.data);
        for(auto v:coeff)CHECK(v==47);
    }
    {
        // Reuse the source-derived command state between two passes:
        // activation bit zero leaves command pending; the following
        // unit pass activation writes +/-1 and clears it.
        constexpr std::array<std::uint8_t,1> cmds{3};
        auto state=*make_bink_ac_pending(cmds);
        const std::array<std::uint8_t,1> bits_bytes{0x02};
        LsbBitReader bits(bits_bytes);
        std::array<std::int16_t,64> coeff{};
        const auto first=run_bink_ac_command_pass(bits,state,coeff,7,8);
        CHECK(first&&first->remaining_commands==1&&bits.bit_position()==1);
        const auto unit=run_bink_ac_unit_pass(bits,state,coeff);
        CHECK(unit&&unit->consumed_bits==2&&bits.bit_position()==3);
        CHECK(coeff[0]==1&&unit->remaining_commands==0);
    }
    std::puts("Bink 2002 final +/-1 AC command-list pass: PASS");
}
