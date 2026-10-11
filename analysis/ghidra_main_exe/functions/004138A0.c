/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004138a0; function: DrawFireworksCertificateScreen; body bytes: 701
 * callers: 1; callees: 5; success: True
 */


void DrawFireworksCertificateScreen(void)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int *piVar6;
  int iVar7;
  undefined4 uVar8;
  int *piVar9;
  int iStack_28;
  
  piVar1 = *(int **)(DAT_0044de08 + 0xc);
  iStack_28 = 0;
  (**(code **)(*piVar1 + 0x1c))(piVar1,0,0,DAT_0050a5cc,0);
  iVar3 = 0;
  iVar4 = (&DAT_0051b404)[DAT_00519934];
  piVar9 = &DAT_0051b404 + DAT_00519934;
  if (0 < iVar4) {
    piVar6 = &DAT_0051b41c + DAT_00519934 * 9;
    iVar7 = iVar4;
    do {
      iVar2 = *piVar6;
      piVar6 = piVar6 + 1;
      iVar3 = iVar3 + (*(int *)(&DAT_00518948 + iVar2 * 0x10) -
                      *(int *)(&DAT_00518940 + iVar2 * 0x10));
      iVar7 = iVar7 + -1;
    } while (iVar7 != 0);
  }
  iVar7 = 0x1eb - iVar3 / 2;
  iVar3 = 0;
  if (0 < iVar4) {
    piVar6 = &DAT_0051b41c + DAT_00519934 * 9;
    do {
      iStack_28 = *(int *)(&DAT_00518940 + *piVar6 * 0x10);
      iVar4 = *(int *)(&DAT_00518948 + *piVar6 * 0x10);
      (**(code **)(*piVar1 + 0x1c))(piVar1,iVar7,0x87,DAT_0051993c,&iStack_28,1);
      iVar7 = iVar7 + (iVar4 - iStack_28);
      iVar3 = iVar3 + 1;
      piVar6 = piVar6 + 1;
    } while (iVar3 < *piVar9);
  }
  if (-1 < (int)(&DAT_0051c24c)[DAT_00519934]) {
    iStack_28 = (&DAT_0051c24c)[DAT_00519934] * 0x32;
    (**(code **)(*piVar1 + 0x1c))(piVar1,0x1d7,0x9c,DAT_00519958,&iStack_28,1);
  }
  if (DAT_004fbfb4 == 0) {
    if ((((DAT_004fbd24 < 0x129) || (0x15b < DAT_004fbd24)) || (DAT_004fbd30 < 0x1a3)) ||
       (0x1d1 < DAT_004fbd30)) {
      DAT_0050ab68 = 0;
      iVar4 = AnyManagedSoundPlaying(DAT_0044ddd8);
      if ((iVar4 == 0) && (DAT_0050ab78 == 0)) {
        uVar5 = FUN_0042ffc4();
        uVar5 = uVar5 & 0x80000007;
        if ((int)uVar5 < 0) {
          uVar5 = (uVar5 - 1 | 0xfffffff8) + 1;
        }
        if (DAT_0050ab80 != uVar5 + 0x90) {
          iVar4 = 0;
          uVar8 = 0x32;
          DAT_0050ab78 = 1;
          DAT_0050ab80 = uVar5 + 0x90;
          uVar5 = FUN_0042ffc4();
          uVar5 = uVar5 & 0x80000007;
          if ((int)uVar5 < 0) {
            uVar5 = (uVar5 - 1 | 0xfffffff8) + 1;
          }
          PlayManagedSoundById(DAT_0044ddd8,uVar5 + 0x90,uVar8,iVar4);
        }
      }
    }
    else {
      if (DAT_004fbfb8 == 0) {
        (**(code **)(*piVar1 + 0x1c))(piVar1,0x124,0x1a1,DAT_0050a5c4,0,1);
      }
      else {
        (**(code **)(*piVar1 + 0x1c))(piVar1,0x124,0x1a1,DAT_0050a5c8,0,1);
      }
      if (DAT_0050ab68 == 0) {
        PlayManagedSoundById(DAT_0044ddd8,0x8f,0x32,2);
        DAT_0050ab68 = 1;
        return;
      }
    }
  }
  else if (((0x128 < DAT_004fbd24) && (DAT_004fbd24 < 0x15c)) &&
          ((0x1a2 < DAT_004fbd30 && (DAT_004fbd30 < 0x1d2)))) {
    StopAllManagedSounds(DAT_0044ddd8);
    (**(code **)(*piVar1 + 0x1c))(piVar1,0,0,DAT_00508ad0,0,1);
    ExportAndPrintGameImage(piVar9);
    return;
  }
  return;
}

