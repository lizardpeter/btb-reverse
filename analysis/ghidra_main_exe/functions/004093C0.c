/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004093c0; function: ApplyGlobalGameVolume; body bytes: 158
 * callers: 7; callees: 4; success: True
 */


void __cdecl ApplyGlobalGameVolume(undefined4 param_1)

{
  int *piVar1;
  
  if (DAT_0051c27c < 0xc) {
    if (DAT_0051c2b0 != (void *)0x0) {
      CSound_SetVolume(DAT_0051c2b0,param_1);
    }
  }
  else if (DAT_0051c27c == 0xc) {
    piVar1 = &DAT_004fc084;
    do {
      if (*piVar1 != 0) {
        CSound_Stop(*piVar1);
        CSound_SetVolume((void *)*piVar1,param_1);
      }
      piVar1 = piVar1 + 1;
    } while ((int)piVar1 < 0x4fc174);
  }
  if (DAT_0051c2b4 != (void *)0x0) {
    CSound_Stop((int)DAT_0051c2b4);
    CSound_SetVolume(DAT_0051c2b4,param_1);
    CSound_Play(DAT_0051c2b4,0,(uint)(DAT_0051c368 != 0));
  }
  ApplyGlobalBinkVolume();
  CSound_SetVolume(DAT_004fbf8c,param_1);
  return;
}

