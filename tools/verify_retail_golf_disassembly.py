#!/usr/bin/env python3
"""Compare production Golf Round to translated original PE32 instructions.

Requires the matching original executable (not distributed), GNU objdump and
an x86-64 GNU-style C++ compiler with 80-bit long double. This is an isolated
instruction translation with modeled host calls, not native PE execution.
"""
import argparse
import hashlib
import pathlib
import subprocess
import tempfile

from verify_retail_squirrel_disassembly import SHA, translate


def golf_instruction(mn, args, op):
    if mn == 'call':
        calls = {
            '0x4304d0': 'eax=uint32_t(int32_t(fp_pop()));',
            '0x414a30': 'reference_vector(esp);',
            '0x415d30': 'reference_distance(esp);',
            '0x402cf0': 'capture_sound(esp);esp+=12;',
        }
        return calls.get(args[0])
    if mn == 'neg':
        return f'{op(args[0])}=0u-{op(args[0])};comparison=int32_t({op(args[0])});'
    if mn == 'setl' and args[0] == 'dl':
        return 'edx=(edx&0xffffff00u)|(comparison<0);'
    if mn == 'mov' and args[0] == 'cl':
        if args[1] != 'BYTE PTR ds:0x4fbe5c':
            raise ValueError(args)
        return 'ecx=(ecx&0xffffff00u)|(mem(0x4fbe5c)&255u);'
    if mn == 'test' and args[0] in ('cl','ah'):
        lhs = 'ecx' if args[0] == 'cl' else '(eax>>8)'
        return f'comparison=({lhs}&255u)&{op(args[1])};'
    if mn == 'fnstsw' and args[0] == 'ax':
        return 'eax=(eax&0xffff0000u)|fp_status;'
    if mn == 'fild':
        return f'fp_push(int32_t({op(args[0])}));'
    if mn == 'fld':
        return f'fp_push(std::bit_cast<float>({op(args[0])}));'
    if mn in ('fdiv','fadd','fsub','fmul'):
        operator = {'fdiv':'/','fadd':'+','fsub':'-','fmul':'*'}[mn]
        rhs = 'decay' if args[0]=='QWORD PTR ds:0x43b3b0' else f'std::bit_cast<float>({op(args[0])})'
        return f'fp_top(){operator}={rhs};'
    if mn == 'fstp':
        return '(void)fp_pop();' if args[0]=='st(0)' else f'{op(args[0])}=std::bit_cast<uint32_t>(float(fp_pop()));'
    if mn == 'fcomp':
        return f'{{long double a=fp_pop(),b=std::bit_cast<float>({op(args[0])});fp_status=a<b?0x100:a==b?0x4000:0;}}'
    return None


PREAMBLE = r'''
#include "golf_retail_trace_fixture.hpp"
#include <array>
#include <bit>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <stdexcept>
static_assert(std::numeric_limits<long double>::digits==64,
              "reference requires x87 extended precision");
using std::array;
static array<uint32_t,256> stack{};
static array<uint32_t,512> golf{};
static uint32_t rng,mode,manager,mouse_x,confirm,click,keys,difficulty,matched,threshold_a=24,threshold_b=50,near_dist=0x42480000,hit_dist=0x41700000;
static constexpr double decay=0x1.89374bc6a7efap-7;
uint32_t& mem(uint32_t a){
 if(a>=0x7ffffc10u&&a<0x80000010u&&a%4==0)return stack[(a-0x7ffffc10u)/4];
 if(a>=0x50ab00u&&a<0x50b300u&&a%4==0)return golf[(a-0x50ab00u)/4];
 switch(a){case 0x447558:return rng;case 0x44de0c:return mode;case 0x44ddd8:return manager;
 case 0x4fbd24:return mouse_x;case 0x4fbfb4:return confirm;case 0x4fbe54:return click;case 0x4fbe5c:return keys;
 case 0x51c284:return difficulty;case 0x4437c8:return matched;case 0x4437d8:return threshold_a;case 0x4437dc:return threshold_b;
 case 0x43b3a4:return near_dist;case 0x43b3a8:return hit_dist;}
 throw std::runtime_error("unmapped address "+std::to_string(a));
}
static array<long double,8> fpu{};static int fp_count;static uint32_t fp_status;
long double& fp_top(){if(!fp_count)throw std::runtime_error("empty x87 stack");return fpu[fp_count-1];}
void fp_push(long double value){if(fp_count>=8)throw std::runtime_error("x87 overflow");fpu[fp_count++]=value;}
long double fp_pop(){auto value=fp_top();--fp_count;return value;}
struct Vec{float x,y;};
Vec reference_vector_raw(int deg,int mag){Vec v;constexpr double radians=0x1.1df5691cb5231p-6;
 asm volatile("fildl %[deg]\n\tfmull %[constant]\n\tfildl %[mag]\n\tfld %%st(1)\n\tfcos\n\tfmul %%st(1),%%st\n\tfstps %[x]\n\tfxch %%st(1)\n\tfsin\n\tfmul %%st(1),%%st\n\tfxch %%st(1)\n\tfstp %%st(0)\n\tfstps %[y]" : [x]"=m"(v.x),[y]"=m"(v.y):[deg]"m"(deg),[mag]"m"(mag),[constant]"m"(radians):"st","st(1)","st(2)");return v;}
void reference_vector(uint32_t sp){auto v=reference_vector_raw(int32_t(mem(sp+8)),int32_t(mem(sp+12)));mem(mem(sp))=std::bit_cast<uint32_t>(v.x);mem(mem(sp+4))=std::bit_cast<uint32_t>(v.y);}
void reference_distance(uint32_t sp){long double dx=int32_t(mem(sp))-int32_t(mem(sp+8)),dy=int32_t(mem(sp+4))-int32_t(mem(sp+12));fp_push(std::sqrt(dx*dx+dy*dy));}
static array<int,4> audio{};
void capture_sound(uint32_t sp){++audio[0];audio[1]=mem(sp);audio[2]=mem(sp+4);audio[3]=mem(sp+8);}
'''

HARNESS = r'''
int main(){
 using namespace btb::golf::trace;
 constexpr array<uint32_t,11> addresses{0x50ad9c,0x50ac08,0x50ac0c,0x50ae14,0x50ab94,0x50ae50,0x50af08,0x50af04,0x50abb0,0x4437c8,0x50abf4};
 auto result=run([](const Shot& shot){
    golf={};mode=shot.windowed;matched=uint32_t(-1);rng=shot.seed();fp_count=0;
    mem(0x50ad9c)=2;mem(0x50ada8)=shot.aim;mem(0x50abf4)=shot.power;mem(0x50abb0)=5;mem(0x50adcc)=16;mem(0x50ac30)=0x40000000;
    mem(0x50ac08)=std::bit_cast<uint32_t>(187.f);mem(0x50ac0c)=std::bit_cast<uint32_t>(330.f);mem(0x50ad80)=187;mem(0x50ad7c)=330;mem(0x50af08)=shot.counter();
    for(int i=0;i<3;++i){mem(0x50abb4+i*8)=centers[i].x;mem(0x50abb8+i*8)=centers[i].y;}
 },[&](const Shot& shot,int tick,const Words& actual){
    audio={};retail_golf();
    if(fp_count)throw std::runtime_error("unbalanced x87 stack");
    auto fail=[&](size_t i,uint32_t expected){
        std::cerr<<"mismatch "<<shot.windowed<<' '<<shot.aim<<' '<<shot.power<<' '<<tick<<" word "<<i<<" actual "<<actual[i]<<" expected "<<expected<<'\n';
        throw std::runtime_error("original frame mismatch");
    };
    for(size_t i=0;i<addresses.size();++i)
        if(actual[i]!=mem(addresses[i]))fail(i,mem(addresses[i]));
    if(actual[11]!=rng)fail(11,rng);
    const auto voice=int32_t(actual[12]);
    if(audio!=(voice<0?array<int,4>{}:array<int,4>{1,voice,50,1}))
        throw std::runtime_error("original sound request mismatch");
 });
 std::cout<<result.shots<<" trajectories, "<<result.frames<<" frame comparisons, hash "<<std::hex<<result.hash<<'\n';
 return result.matches()?0:1;
}
'''


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('pe', type=pathlib.Path)
    parser.add_argument('--repo', type=pathlib.Path, default=pathlib.Path(__file__).resolve().parents[1])
    parser.add_argument('--compiler', default='g++')
    parser.add_argument('--std', default='c++2c')
    args = parser.parse_args()
    if hashlib.sha256(args.pe.read_bytes()).hexdigest() != SHA:
        raise ValueError("original executable SHA-256 mismatch")
    code = PREAMBLE + translate(args.pe, 'retail_rand', 0x42ffc4, 0x42ffe2)
    code += '\n' + translate(args.pe, 'retail_golf', 0x415080, 0x415821, golf_instruction)
    code += '\n' + HARNESS
    with tempfile.TemporaryDirectory(prefix='btb-golf-reference-') as directory:
        path = pathlib.Path(directory)
        source = path / 'compare.cpp'
        source.write_text(code)
        exe = path / 'compare'
        subprocess.run([args.compiler, '-std='+args.std, '-O2',
                        '-I', str(args.repo/'include'), '-I', str(args.repo/'tests'),
                        str(source), str(args.repo/'src/golf_activity.cpp'),
                        str(args.repo/'src/golf_runtime.cpp'), '-o', str(exe)], check=True)
        subprocess.run([str(exe)], check=True)


if __name__ == '__main__':
    main()
