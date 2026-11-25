#!/usr/bin/env python3
"""Translate selected retail PE instructions into a bounded native comparison harness.

Requires the user's original matching PE, GNU objdump and a C++ compiler supporting the repository headers.
No retail machine code is executed or written to the repository.
"""
import argparse
import hashlib
import pathlib
import re
import struct
import subprocess
import tempfile

SHA = 'c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05'
REGS = set('eax ebx ecx edx esi edi ebp esp'.split())


def translate(pe, name, start, end):
    asm = subprocess.check_output(['objdump', '-d', '-Mintel', '--no-show-raw-insn',
        f'--start-address={start:#x}', f'--stop-address={end:#x}', str(pe)], text=True)

    def op(s, addr=False):
        s = s.strip().removeprefix('DWORD PTR ')
        if s.startswith('ds:') and '[' not in s:
            return f'mem({s[3:]})'
        s = s.removeprefix('ds:')
        if s.startswith('[') and s.endswith(']'):
            expr = s[1:-1]
            if not re.fullmatch(r'[a-z0-9x+*\-]+', expr):
                raise ValueError(s)
            return expr if addr else f'mem({expr})'
        if s in REGS or re.fullmatch(r'0x[0-9a-f]+|[0-9]+',s):
            return s
        raise ValueError(s)

    lines = [f'uint32_t {name}() {{',
        'uint32_t eax=0,ebx=0,ecx=0,edx=0,esi=0,edi=0,ebp=0,esp=0x80000000; int64_t comparison=0;']
    if name=='selector':
        lines[1]=lines[1].replace('esp=0x80000000','esp=0x7ffff800')
    # Selector has a separate stack window when called by assembly.
    conds={'je':'==0','jne':'!=0','jl':'<0','jge':'>=0','jle':'<=0','jg':'>0','jns':'>=0','js':'<0'}
    for line in asm.splitlines():
        match=re.match(r'^\s*([0-9a-f]+):\s+([a-z]+)\s*(.*)$',line)
        if not match:
            continue
        pc,mn,ops=match.groups()
        args=ops.split(',') if ops.strip() else []
        if mn=='nop': body=';'
        elif mn=='ret': body='return eax;'
        elif mn=='jmp': body=f'goto L{int(args[0],16):x};'
        elif mn in conds: body=f'if(comparison{conds[mn]}) goto L{int(args[0],16):x};'
        elif mn=='cdq': body='edx=(int32_t(eax)<0)?0xffffffffu:0;'
        elif mn=='push': body=f'esp-=4;mem(esp)={op(args[0])};'
        elif mn=='pop': body=f'{op(args[0])}=mem(esp);esp+=4;'
        elif mn=='call' and args[0]=='0x42ffc4': body='eax=retail_rand();'
        elif mn=='call' and args[0]=='0x426f30': body='++retail_choice_calls;generate_choices();'
        elif mn=='call' and args[0]=='0x4259d0': body='selector();'
        elif mn=='call' and args[0]=='0x402af0': body=';' # verified RET
        elif mn=='call' and args[0]=='DWORD PTR [ecx+0x1c]': body='capture_draw(esp);esp+=24;'
        elif mn=='setge' and args[0]=='dl': body='edx=(edx&0xffffff00u)|(comparison>=0);'
        elif mn=='imul' and len(args)==2: body=f'{op(args[0])}*=uint32_t({op(args[1])});'
        elif mn=='imul' and len(args)==3: body=f'{op(args[0])}=uint32_t({op(args[1])})*uint32_t({op(args[2])});'
        elif mn=='imul': body=f'{{int64_t v=int64_t(int32_t(eax))*int32_t({op(args[0])});eax=uint32_t(v);edx=uint32_t(uint64_t(v)>>32);}}'
        elif mn=='idiv': body=f'{{int64_t v=int64_t(int32_t(edx))*4294967296LL+eax;int32_t d=int32_t({op(args[0])});eax=uint32_t(v/d);edx=uint32_t(v%d);}}'
        elif mn in {'dec','inc'}: body=f'{op(args[0])}{"--" if mn=="dec" else "++"};comparison=int32_t({op(args[0])});'
        elif mn=='test': body=f'comparison=int32_t({op(args[0])}&{op(args[1])});'
        elif mn=='cmp': body=f'comparison=int64_t(int32_t({op(args[0])}))-int32_t({op(args[1])});'
        elif mn=='lea': body=f'{op(args[0])}={op(args[1],True)};'
        elif mn=='sar': body=f'{op(args[0])}=uint32_t(int32_t({op(args[0])})>>{op(args[1])});'
        elif mn in {'mov','add','sub','xor','shl','shr','and','or'}:
            oper={'mov':'=','add':'+=','sub':'-=','xor':'^=','shl':'<<=','shr':'>>=','and':'&=','or':'|='}[mn]
            body=f'{op(args[0])}{oper}{op(args[1])};'
            if mn!='mov':
                body+=f'comparison=int32_t({op(args[0])});'
        else:
            raise ValueError(f'Unsupported instruction {mn} at {pc}')
        lines.append(f'L{pc}: {body}')
    return '\n'.join(lines+['throw std::runtime_error("fell through retail routine");','}'])


PREAMBLE=r'''
#include "btb/squirrel_runtime.hpp"
#include "btb/retail_activity_closure.hpp"
#include <iostream>
#include <stdexcept>
#include <array>
#include <cstdint>
using namespace btb::squirrel;
static std::array<uint32_t,1024> stack{};
static std::array<uint32_t,2048> globals{};
static std::array<uint32_t,512> image_data{IMAGE_WORDS};
static uint32_t rng_state;
static uint32_t window_mode;
static uint32_t retail_choice_calls, native_choice_calls;
void note_choice() noexcept {++native_choice_calls;}
uint32_t& mem(uint32_t a) {
    if(a>=0x7ffff010u&&a<0x80000010u&&a%4==0)return stack[(a-0x7ffff010u)/4];
    if(a>=0x514000u&&a<0x516000u&&a%4==0)return globals[(a-0x514000u)/4];
    if(a>=0x446700u&&a<0x446f00u&&a%4==0)return image_data[(a-0x446700u)/4];
    if(a==0x447558u)return rng_state;
    if(a==0x44de0cu)return window_mode;
    // Only the one observed DirectDraw receiver and BltFast call are
    // modeled. No original COM object or Windows device is executed.
    static uint32_t device=0x10000,surface=0x20000,vtable=0x30000;
    if(a==0x44de08)return device;
    if(a==0x1000c)return surface;
    if(a==0x20000)return vtable;
    throw std::runtime_error("unmapped read/write "+std::to_string(a));
}
static std::array<int,5> draw{};
void capture_draw(uint32_t sp) {
    ++draw[0];draw[1]=mem(sp+4);draw[2]=mem(sp+8);
    draw[3]=mem(mem(sp+16));draw[4]=mem(mem(sp+16)+8);
}
'''
HARNESS=r'''
int main(){
    uint64_t count=0;
    RunPlan c{};PlacedRunPieces p{};
    for(int level=0;level<3;++level)for(int tuple=0;tuple<81;++tuple)for(int id=0;id<36;++id){
        int t=tuple;
        for(int i=0;i<4;++i){c[level][i]=t%3;t/=3;p[level*4+i]=(id+i*11)%36;
          mem(0x514f0c+level*16+i*4)=c[level][i];mem(0x514fbc+level*16+i*4)=p[level*4+i];}
        mem(0x5150d0)=level;
        for(int section=0;section<8;++section){
            mem(0x514fb4)=section;selector();
            auto k=select_run_assembly_motion_keys(section,level,c,p);
            if(!k||k->a!=int(mem(0x514ee0))||k->b!=int(mem(0x514ee4))||k->c!=int(mem(0x514ee8))||k->d!=int(mem(0x514eec))){
                std::cerr<<"selector mismatch "<<level<<' '<<tuple<<' '<<id<<' '<<section<<'\n';return 1;}
            ++count;
        }
    }
    std::cout<<"Squirrel selector comparisons: "<<count<<'\n'; count=0;
    for(int x=-40;x<=640;x+=17)for(int y=-20;y<=480;y+=20)
    for(int dx=-8;dx<=8;++dx)for(int dy=-8;dy<=8;++dy){
        mem(0x44699c)=x;mem(0x4469a0)=y;mem(0x80000004)=x+dx;mem(0x80000008)=y+dy;
        auto expected=step_toward_target({x,y},{x+dx,y+dy});auto got=move_actor();
        if(expected.position.x!=int(mem(0x44699c))||expected.position.y!=int(mem(0x4469a0))||expected.reached!=bool(got)){
          std::cerr<<"actor mismatch "<<x<<' '<<y<<' '<<dx<<' '<<dy<<'\n';return 2;}
        ++count;
    }
    std::cout<<"Squirrel actor comparisons: "<<count<<'\n'; count=0;
    for(int difficulty=0;difficulty<3;++difficulty)for(int level=0;level<3;++level)
    for(int slot=0;slot<3;++slot)for(int from=0;from<3;++from)for(int to=-1;to<3;++to)
    for(uint32_t seed=0;seed<256;++seed){
        uint32_t initial=seed*0x9e3779b9u+0x1234u;
        mem(0x447558)=initial;mem(0x5150cc)=difficulty;mem(0x5150d0)=level;
        mem(0x515078+level*4)=slot;mem(0x514f0c+level*16+slot*4)=from;
        mem(0x514f0c+level*16+(slot+1)*4)=to;mem(0x514f54)=99;
        generate_choices();btb::retail::OriginalRetailRandom rng(initial);
        auto result=generate_conveyor_choices(static_cast<Difficulty>(difficulty),{from,to},rng);
        if(!result||result->count!=(difficulty==0?3:4)||rng.state()!=mem(0x447558)||
            result->variant_index!=int(mem(0x515098))||result->landing_connector.value_or(99)!=int(mem(0x514f54))){
            std::cerr<<"conveyor header mismatch "<<difficulty<<' '<<level<<' '<<slot<<' '<<from<<' '<<to<<' '<<seed<<'\n';return 3;}
        for(int i=0;i<result->count;++i){const auto& c=result->choices[i];
            if(!c.active||c.piece_id!=int(mem(0x515088+i*4))||c.correct!=bool(mem(0x514ff0+i*4))||
                c.feedback_comparison!=int(mem(0x514eb8+i*8))||c.feedback_to!=int(mem(0x514ebc+i*8))){
                std::cerr<<"conveyor choice mismatch "<<difficulty<<' '<<level<<' '<<slot<<' '<<from<<' '<<to<<' '<<seed<<' '<<i<<'\n';return 4;}}
        ++count;
    }
    std::cout<<"Squirrel conveyor comparisons: "<<count<<'\n'; count=0;
    using btb::retail_activity::SquirrelConveyor;
    using btb::retail_activity::update_squirrel_conveyor;
    for(bool windowed:{false,true})for(int phase=0;phase<3;++phase)
    for(int pos=-470;pos<=550;pos+=5)for(int countdown=0;countdown<4;++countdown)
    for(int frame=0;frame<3;++frame)for(uint32_t counter: {0u,1u,2u,0x7fffffffu,0xffffffffu}){
        SquirrelConveyor s{phase,pos,countdown,frame,counter};
        mem(0x44de0c)=windowed;mem(0x51511c)=counter;mem(0x5150e8)=phase;
        mem(0x5150ec)=pos;mem(0x5150f8)=countdown;mem(0x5150f4)=frame;
        retail_choice_calls=native_choice_calls=0;
        mem(0x5150cc)=0;mem(0x5150d0)=0;mem(0x515078)=0;
        mem(0x514f0c)=0;mem(0x514f10)=1;mem(0x447558)=42;
        slide_cycle();update_squirrel_conveyor(s,windowed,note_choice);
        if(s.phase!=int(mem(0x5150e8))||s.position!=int(mem(0x5150ec))||
           s.animation_countdown!=int(mem(0x5150f8))||s.animation_frame!=int(mem(0x5150f4))||
           s.window_counter!=mem(0x51511c)||retail_choice_calls!=native_choice_calls){
           std::cerr<<"slide mismatch "<<windowed<<' '<<phase<<' '<<pos<<' '<<countdown<<' '<<frame<<' '<<counter<<'\n';return 5;}
        ++count;
    }
    std::cout<<"Squirrel conveyor slide comparisons: "<<count<<'\n';
    count=0;globals={};
    Data fixture{};
    fixture.horizontal_motion_deltas={0,3,10,27,8,-4,-4};
    fixture.vertical_motion_deltas={{{0,0,1,-1,0,0,0},{0,0,0,0,0,0,0},
        {0,0,1,-17,0,0,0},{0,0,1,-33,0,0,0},{0,0,-1,17,0,0,0},
        {0,0,19,13,0,0,0},{0,0,0,-32,0,0,0},{0,0,0,32,0,0,0}}};
    for(int i=0;i<7;++i)mem(0x446958+i*4)=fixture.horizontal_motion_deltas[i];
    for(int i=0;i<56;++i)mem(0x446878+i*4)=fixture.vertical_motion_deltas[i/7][i%7];
    uint64_t hash=14695981039346656037ULL;
    auto mix=[&](uint32_t word){for(int byte=0;byte<4;++byte){
        hash^=word&255u;hash*=1099511628211ULL;word>>=8;}};
    const std::array<int,3> targets{237,205,173};
    constexpr std::array<uint32_t,10> addresses{0x514fb8,0x514fb4,0x514f94,0x514f98,
        0x514fa0,0x514f9c,0x514fa4,0x514fa8,0x515110,0x447558};
    for(int tuple=0;tuple<81;++tuple)for(int id=0;id<36;++id)
    for(int section=0;section<7;++section)for(int delay=1;delay<=7;delay+=6) {
        RunPlan plan{};PlacedRunPieces pieces{};int value=tuple;
        for(int i=0;i<4;++i) {
            plan[0][i]=value%3;value/=3;pieces[i]=(id+i*7)%36;
            mem(0x514f0c+i*4)=plan[0][i];mem(0x514fbc+i*4)=pieces[i];
        }
        mem(0x5150d0)=0;mem(0x515004)=section;mem(0x514fb4)=section;
        mem(0x514fb8)=0;mem(0x514f94)=-34;mem(0x514f98)=205;
        mem(0x514fa0)=0;mem(0x514f9c)=0;mem(0x514fa4)=0;mem(0x514fa8)=0;
        mem(0x515110)=id&1;mem(0x515114)=0;mem(0x514f54)=id%3;
        mem(0x5150d8)=0;mem(0x446778)=delay;mem(0x447558)=1234+id;mem(0x446a4c)=0;
        fixture.animation_step_delay=delay;
        RunAssemblyAnimationRuntime native{};
        native.run_section=section;native.x=-34;native.y=205;
        native.high_sprite=(id&1)!=0;native.landing_y=targets[id%3];
        btb::retail::OriginalRetailRandom rng(1234+id);
        for(int tick=0;tick<128;++tick) {
            draw={};assemble();
            bool visible;int frame;
            if(native.state==RunAssemblyAnimationState::Idle && native.run_section>section) {
                frame=tick_assembly_idle_sprite(native,delay,[&]{return rng.next_rand();});
                visible=true; // raw intent, before native BMP bounds checks
            } else {
                if(native.state==RunAssemblyAnimationState::Idle)
                    begin_run_assembly_animation(native,fixture,
                        *select_run_assembly_motion_keys(native.run_section,0,plan,pieces));
                (void)tick_run_assembly_animation(native,fixture);
                frame=active_assembly_sprite_frame(native);
                visible=assembly_sprite_visible(native,frame,0,pieces);
            }
            std::array<uint32_t,10> actual{
                uint32_t(native.state),uint32_t(native.run_section),uint32_t(native.x),
                uint32_t(native.y),uint32_t(native.delay_remaining),uint32_t(native.substep),
                uint32_t(native.sprite_mode),uint32_t(native.sprite_frame),
                uint32_t(native.high_sprite),rng.state()};
            for(size_t i=0;i<addresses.size();++i) {
                const auto reference=mem(addresses[i]);
                if(actual[i]!=reference) {
                    std::cerr<<"assembly state mismatch "<<tuple<<' '<<id<<' '<<section
                        <<' '<<delay<<' '<<tick<<" word "<<i<<'\n';return 6;
                }
                mix(reference);
            }
            const std::array<int,5> native_draw{int(visible),visible?native.x:0,
                visible?native.y:0,visible?frame*102:0,visible?(frame+1)*102:0};
            if(native_draw!=draw) {
                std::cerr<<"assembly draw mismatch "<<tuple<<' '<<id<<' '<<section
                    <<' '<<delay<<' '<<tick<<'\n';return 7;
            }
            for(auto word:draw)mix(uint32_t(word));
            ++count;
        }
    }
    std::cout<<"Squirrel assembly frame comparisons: "<<count<<", trace "<<std::hex<<hash<<'\n';
    if(count!=5225472 || hash!=0x9f2c9d55e0505c56ULL)return 8;
}
'''


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('pe',type=pathlib.Path)
    p.add_argument('--repo',type=pathlib.Path,default=pathlib.Path('.'))
    p.add_argument('--cxx',default='g++')
    p.add_argument('--std',default='c++2c',help='compiler C++ standard flag value')
    a=p.parse_args()
    image=a.pe.read_bytes()
    if hashlib.sha256(image).hexdigest()!=SHA:
        raise SystemExit('Retail PE SHA-256 mismatch')
    code=PREAMBLE.replace('IMAGE_WORDS',','.join(map(str,struct.unpack_from('<512I',image,0x46700))))
    for name,start,end in [('retail_rand',0x42ffc4,0x42ffe2),
        ('selector',0x4259d0,0x425f33),('move_actor',0x427270,0x427312),
        ('generate_choices',0x426f30,0x427263),
        ('slide_cycle',0x427320,0x4273eb),('assemble',0x425f40,0x4267ac)]:
        code+=translate(a.pe,name,start,end)+'\n'
    code+=HARNESS
    with tempfile.TemporaryDirectory() as d:
        cpp=pathlib.Path(d)/'diff.cpp'
        exe=pathlib.Path(d)/'diff'
        cpp.write_text(code)
        subprocess.run([a.cxx,f'-std={a.std}','-O2','-I',str(a.repo/'include'),str(cpp),'-o',str(exe)],check=True)
        subprocess.run([str(exe)],check=True)


if __name__=='__main__':
    main()
