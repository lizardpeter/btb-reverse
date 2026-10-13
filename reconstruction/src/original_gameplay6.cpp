#include "btb/original_gameplay6.hpp"
namespace btb::original_gameplay6 {
Plan initialize_golf_activity(GolfInit& s,const btb::golf::Data& source){
 Plan p{};s=GolfInit{};s.data=source;s.ball=source.initial_ball_position();
 p.emit(Op::ParseGolf);p.emit(Op::InstallUnload,0x00414410);
 p.emit(Op::LoadBitmap,0x0050aedc);p.emit(Op::RegisterBitmap,0x0050aedc);
 for(int j=0;j<3;++j){
  for(I32 slot:{0x0050ab88+4*j,0x0050addc+4*j}){
   p.emit(Op::LoadBitmap,slot);p.emit(Op::RegisterBitmap,slot);
  }
 }
 for(I32 slot:{0x0050aee0,0x0050aee4,0x0050aee8,0x0050aeec,
               0x0050aef0,0x0050aef4,0x0050aef8,0x0050adec}){
  p.emit(Op::LoadBitmap,slot);p.emit(Op::RegisterBitmap,slot);
 }
 for(int j=0;j<3;++j){
  I32 slot=0x0050adfc+4*j;
  p.emit(Op::LoadBitmap,slot);p.emit(Op::RegisterBitmap,slot);
 }
 for(int j=0;j<3;++j)
  for(I32 slot:{0x0050ab88+4*j,0x0050addc+4*j})
   p.emit(Op::MarkColorKey,slot,0xff00ff);
 for(I32 slot:{0x0050aedc,0x0050aee0,0x0050aee4,0x0050aee8,
               0x0050aef0,0x0050aef4,0x0050aef8,0x0050adec,
               0x0050adfc,0x0050ae00,0x0050ae04})
  p.emit(Op::MarkColorKey,slot,0xff00ff);
 p.emit(Op::EnableIntro);return p;
}
Plan initialize_spud_skate(
 SkateInit& s,const btb::spud_skate::TimingData& original,
 const std::array<std::string,5>& movie_paths,
 const std::function<std::optional<std::vector<U8>>(const std::string&,std::size_t)>& read){
 Plan p{};s=SkateInit{};s.sound_ids.fill(-1);
 s.timing=original;p.emit(Op::ParseSkate);p.emit(Op::InstallUnload,0x00424100);
 for(I32 addr:{0x00514ea0,0x005148b4,0x00514b60,0x00514b64,0x00514b68}){
  p.emit(Op::LoadBitmap,addr);p.emit(Op::RegisterBitmap,addr);
  p.emit(Op::MarkColorKey,addr,0xff00ff);
 }
 constexpr std::size_t limit=7000000;
 for(std::size_t i=0;i<5;++i){
  if(!read){p.failed=true;p.detail="Retail Spud Skate Bink asset source unavailable";return p;}
  const auto content=read(movie_paths[i],limit);
  if(!content||content->size()>limit){
   p.failed=true;p.detail="Retail Spud Skate Bink asset missing or oversized";return p;
  }
  s.movie_bytes[i]=*content;
  p.emit(Op::LoadMovie,static_cast<I32>(i),static_cast<I32>(content->size()));
  p.emit(Op::CreateMovieSurface,static_cast<I32>(i),600,380);
  p.emit(Op::OpenBink,static_cast<I32>(i));
 }
 p.emit(Op::LoadSoundTable);s.intro_pending=true;s.valid=true;
 return p;
}
Plan update_grand_opening_toolbar(BandToolbar& s,U32 random_roll){
 Plan p{};
 if(s.state!=0)return p;
 if(s.play_pending&&!s.managed_playing){
  s.play_pending=false;s.state=8;p.early_return=true;return p;
 }
 I32 hit=-1;
 for(I32 i=0;i<4;++i){
  const auto r=s.buttons[static_cast<std::size_t>(i)];
  if(r.x0<s.cursor_x&&s.cursor_x<r.x1&&
     r.y0<s.cursor_y&&s.cursor_y<r.y1){hit=i;break;}
 }
 if(hit==-1){s.last_hover=-1;return p;}
 p.emit(Op::DrawToolbar,hit);
 if(!s.delete_mode&&s.last_hover!=hit){
  p.emit(Op::StartManagedVoice,s.start_voice+6+hit,50,2);s.last_hover=hit;
 }
 if(s.mouse_down)p.emit(Op::DrawToolbar,hit+4);
 if(!s.pressed)return p;
 switch(hit){
 case 0:
  if(s.state==0&&!s.delete_mode){
   s.play_pending=true;p.emit(Op::StopSounds);
   p.emit(Op::StartManagedVoice,s.start_voice+1,50,1);
  }break;
 case 1:
  if(s.state==9&&!s.delete_mode){
   s.state=0;p.emit(Op::StopLoop);
   p.emit(Op::StartManagedVoice,
     s.start_voice+3+static_cast<I32>(random_roll%2),50,1);
  }break;
 case 2:
  if(s.state==0&&!s.delete_mode)p.emit(Op::OpenConfirmation,2);
  break;
 case 3:
  if(s.state==0){
   if(!s.delete_mode){
    s.delete_mode=true;
    p.emit(Op::StartManagedVoice,
       s.start_voice+9+static_cast<I32>(random_roll%2),50,1);
    p.emit(Op::SetCursor,1);
   }else{s.delete_mode=false;p.emit(Op::SetCursor,0);}
  }break;
 }
 return p;
}
Plan play_draw_grand_opening(BandPlayback& s){
 Plan p{};p.emit(Op::DrawBandGrid);
 const I32 second=s.elapsed_centiseconds/100;
 if(second<0||second>=24)return p;
 for(I32 row=0;row<5;++row)
  for(I32 col=0;col<=second;++col){
   const I32 type=s.score.at({row,col});
   if(!btb::bobs_band::valid_machine(type))continue;
   const bool active= (type%2!=0&&(col==second||col+1==second))||
                      (type%2==0&&col==second);
   const I32 width=active
       ?((s.elapsed_centiseconds-col*100)*22)/100:22;
   p.emit(Op::BlitNote,row*24+col,type,width);
  }
 if(s.last_centiseconds/100!=second){
  for(I32 row=0;row<5;++row){
   const I32 type=s.score.at({row,second});
   if(!btb::bobs_band::valid_machine(type))continue;
   const I32 pitch=btb::bobs_band::pitch_sample_index(row,type);
   p.emit(Op::ResetSound,pitch);p.emit(Op::PlaySoundDirect,pitch);
  }
 }
 s.conductor_accum+=s.frame_delta;
 if(s.conductor_accum>3){
  ++s.conductor_frame;s.conductor_accum=0;
  constexpr std::array<I32,3> minimum{50,50,50},maximum{90,85,105};
  if(s.conductor_index>=0&&s.conductor_index<3){
   const auto i=static_cast<std::size_t>(s.conductor_index);
   if(s.conductor_frame>=maximum[i]||s.conductor_frame<minimum[i])
    s.conductor_frame=minimum[i];
  }
 }
 p.emit(Op::DrawBandConductor,s.conductor_index,s.conductor_frame);
 s.last_centiseconds=s.elapsed_centiseconds;
 return p;
}
Plan update_fireworks_editor_controls(FireControls& s){
 Plan p{};
 if(s.debounce>0){--s.debounce;p.early_return=true;return p;}
 I32 x=s.cursor_x,y=s.cursor_y;
 if(s.selected_left>0||s.selected_right>0){x+=40;y+=20;}
 for(const auto& r:s.regions){
  const I32 action=r.action;
  if(action==-1 || !(r.rect.left<x&&x<r.rect.right&&
                     r.rect.top<y&&y<r.rect.bottom))
      || (action!=25&&action!=26&&action!=27))continue;
  if(action==27){
   if(s.selected_left==0&&s.selected_right==0){
    if(s.click&&s.mode==0){
     s.mode=14;p.emit(Op::StartManagedVoice,880,50,2);
     p.early_return=true;
    }else if(s.hover!=27&&!s.mouse_down)
     p.emit(Op::StartManagedVoice,881,50,2);
   }
  }else if(action==26){
   if(s.object_selected&&(s.selected_left==1||s.selected_right==1)){
    s.selected_left=0;s.selected_right=0;s.mode=0;
    p.emit(Op::SetCursor,0);s.debounce=10;p.early_return=true;
   }else if(s.click&&s.debounce==0){
    if(s.mode==13){s.mode=0;p.emit(Op::SetCursor,0);}
    else{s.mode=13;p.emit(Op::SetCursor,1);
      p.emit(Op::StartManagedVoice,890,50,2);}
   }else if(s.hover!=26&&!s.mouse_down)
    p.emit(Op::StartManagedVoice,882,50,2);
  }else if(action==25){
   if(s.click&&s.mode==0){
    s.confirmation_open=true;p.emit(Op::OpenConfirmation,3);
    p.emit(Op::StartManagedVoice,885,50,2);
   }else if(s.hover!=25&&!s.mouse_down)
    p.emit(Op::StartManagedVoice,883,50,2);
  }
  s.hover=action;p.emit(Op::DrawToolbar,action);return p;
 }
 return p;
}
Plan draw_grand_opening_machines(BandDraw& s){
 Plan p{};p.emit(Op::DrawBitmap,0x00513f1c);
 for(I32 row=0;row<5;++row)
  for(I32 second=0;second<24;++second){
   const I32 type=s.score.at({row,second});
   if(btb::bobs_band::valid_machine(type))
    p.emit(Op::DrawSprite,type,row,second);
  }
 for(I32 i=0;i<5;++i){
  auto& m=s.machines[static_cast<std::size_t>(i)];
  if(m.mode!=0){
   m.tick+=s.frame_delta;
   if(m.tick>3){
    m.tick=0;++m.frame;
    if(m.frame>=m.frame_count){m.frame=0;m.mode=0;}
   }
  }
  p.emit(Op::DrawBandMachine,i,m.mode,m.frame);
 }
 if(++s.conductor_accum>5){s.conductor_accum=0;++s.conductor_frame;}
 p.emit(Op::DrawBandConductor,s.conductor_index,s.conductor_frame);
 p.emit(Op::DrawBitmap,0x00513f20);
 return p;
}
}
