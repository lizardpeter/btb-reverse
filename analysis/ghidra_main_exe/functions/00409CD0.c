/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00409cd0; function: UpdateDinoPrintButton; body bytes: 294
 * callers: 1; callees: 3; success: True
 */


void UpdateDinoPrintButton(void)

{
  int *piVar1;
  HGLOBAL pvVar2;
  
  piVar1 = *(int **)(DAT_0044de08 + 0xc);
  (**(code **)(*piVar1 + 0x1c))(piVar1,0,0,DAT_004fc2a8,0,0);
  if (DAT_004fbfb4 == 0) {
    if ((((DAT_004fbd24 < 299) || (0x15a < DAT_004fbd24)) || (DAT_004fbd30 < 0x1a6)) ||
       (0x1d0 < DAT_004fbd30)) {
      DAT_004fca88 = 0;
    }
    else {
      if (DAT_004fbfb8 == 0) {
        (**(code **)(*piVar1 + 0x1c))(piVar1,0x124,0x1a1,DAT_0050a5c4,0,1);
      }
      else {
        (**(code **)(*piVar1 + 0x1c))(piVar1,0x124,0x1a1,DAT_0050a5c8,0,1);
      }
      if (DAT_004fca88 == 0) {
        PlayManagedSoundById(DAT_0044ddd8,0x8f,0x32,2);
        DAT_004fca88 = 1;
        return;
      }
    }
  }
  else if (((0x12a < DAT_004fbd24) && (DAT_004fbd24 < 0x15b)) &&
          ((0x1a5 < DAT_004fbd30 && (DAT_004fbd30 < 0x1d1)))) {
    StopAllManagedSounds(DAT_0044ddd8);
    pvVar2 = (HGLOBAL)0x0;
    (**(code **)(*piVar1 + 0x1c))(piVar1,0,0,DAT_00508ad0,0,1);
    ExportAndPrintGameImage(pvVar2);
    return;
  }
  return;
}

