/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00428010; function: PlayActivityMusicByIndex; body bytes: 143
 * callers: 10; callees: 3; success: True
 */


void __cdecl PlayActivityMusicByIndex(int param_1)

{
  if (DAT_0051c2b4 != (undefined4 *)0x0) {
    CSound_Stop((int)DAT_0051c2b4);
    if (DAT_0051c2b4 != (undefined4 *)0x0) {
      (**(code **)*DAT_0051c2b4)(1);
      DAT_0051c2b4 = (undefined4 *)0x0;
    }
  }
  CSoundManager_Create
            (DAT_004fc174,&DAT_0051c2b4,s_data_music_certificate_wav_00446cf0 + param_1 * 0x32,0,
             DAT_0043b5a8,DAT_0043b5ac,DAT_0043b5b0,DAT_0043b5b4,1);
  CSound_Play(DAT_0051c2b4,0,1);
  DAT_0051c368 = 1;
  return;
}

