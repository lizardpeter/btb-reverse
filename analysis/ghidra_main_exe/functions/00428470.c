/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00428470; function: UpdateGenericHelpHoverVoice; body bytes: 167
 * callers: 4; callees: 1; success: True
 */


void UpdateGenericHelpHoverVoice(void)

{
  undefined4 uVar1;
  int iVar2;
  
  if (DAT_004fbe68 == DAT_004fbf88) {
    if ((DAT_004fbfbc & 4) != 0) {
      if (DAT_0051c27c == 0xc) {
        uVar1 = DAT_0051b3dc;
        if (DAT_004fbfb8 == 0) {
          uVar1 = DAT_0051b3d8;
        }
        (**(code **)(**(int **)(DAT_0044de08 + 0xc) + 0x1c))
                  (*(int **)(DAT_0044de08 + 0xc),0x23e,0x1a2,uVar1,0,0);
      }
      if (DAT_0051c374 == 0) {
        if (DAT_0051c370 == 0) {
          iVar2 = 0x1c8;
        }
        else {
          if (DAT_0051c370 != 1) {
            DAT_0051c374 = 1;
            return;
          }
          iVar2 = 0x1db;
        }
        PlayManagedSoundById(DAT_0044ddd8,iVar2,0x32,2);
      }
      DAT_0051c374 = 1;
      return;
    }
    DAT_0051c374 = 0;
  }
  return;
}

