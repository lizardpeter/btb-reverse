/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004282c0; function: UpdateHelpButtonController; body bytes: 423
 * callers: 4; callees: 4; success: True
 */


void UpdateHelpButtonController(void)

{
  int *piVar1;
  
  if (DAT_004fbe68 == DAT_004fbf88) {
    if (DAT_0044a2a0 == 1) {
      SetCursorSurface(DAT_00519944);
      DAT_0044a2a0 = DAT_0044a2a0 + 1;
      StopAllManagedSounds(DAT_0044ddd8);
      piVar1 = &DAT_004fc084;
      do {
        if (*piVar1 != 0) {
          CSound_Stop(*piVar1);
        }
        piVar1 = piVar1 + 1;
      } while ((int)piVar1 < 0x4fc0d4);
      return;
    }
    piVar1 = *(int **)(DAT_0044de08 + 0xc);
    if ((DAT_004fbfbc & 8) == 0) {
      DAT_0051c36c = 0;
    }
    else {
      if (DAT_0051c36c == 0) {
        PlayManagedSoundById(DAT_0044ddd8,0x1c7,0x32,2);
      }
      DAT_0051c36c = 1;
      if ((DAT_0044de14 == 1) && (DAT_0051c2f4 == 2)) {
        (**(code **)(*piVar1 + 0x1c))(piVar1,DAT_00446f28,DAT_00446f2c,DAT_0051b3c8,0,0);
      }
      if (DAT_0051c27c == 0xc) {
        if (DAT_004fbfb8 == 0) {
          (**(code **)(*piVar1 + 0x1c))(piVar1,0xe,0x1a2,DAT_0051b3c8,0,0);
        }
        else {
          (**(code **)(*piVar1 + 0x1c))(piVar1,0xe,0x1a2,DAT_0051b3cc,0,0);
        }
      }
    }
    if (DAT_004fbfb4 != 0) {
      if ((DAT_0044de14 == 1) && (DAT_0051c2f4 == 2)) {
        if (((DAT_00446f28 < DAT_004fbd24) && (DAT_004fbd24 < DAT_00446f30)) &&
           ((DAT_00446f2c < DAT_004fbd30 && (DAT_004fbd30 < DAT_00446f34)))) {
          DAT_0044a2a0 = 1;
          return;
        }
      }
      else if ((DAT_00446f18 < DAT_004fbd24) &&
              (((DAT_004fbd24 < DAT_00446f20 && (DAT_00446f1c < DAT_004fbd30)) &&
               (DAT_004fbd30 < DAT_00446f24)))) {
        DAT_0044a2a0 = 1;
      }
    }
  }
  return;
}

