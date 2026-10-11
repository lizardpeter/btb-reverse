/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041fc90; function: PlayAndDrawGrandOpeningComposition; body bytes: 918
 * callers: 1; callees: 9; success: True
 */


void PlayAndDrawGrandOpeningComposition(void)

{
  int *piVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int *piVar9;
  uint *puStack_38;
  uint *puStack_34;
  int aiStack_28 [7];
  undefined4 uStack_c;
  int iStack_8;
  undefined4 uStack_4;
  
  piVar1 = *(int **)(DAT_0044de08 + 0xc);
  DrawGrandOpeningCompositionAndMachines();
  GetSystemTime((LPSYSTEMTIME)&DAT_00512598);
  SystemTimeToFileTime((SYSTEMTIME *)&DAT_00512598,(LPFILETIME)&DAT_005120f0);
  iVar2 = FileTimeDeltaCentiseconds((int *)&DAT_00510d10,(int *)&DAT_005120f0);
  iVar5 = iVar2 / 100;
  DAT_005109c8 = iVar2;
  if (iVar5 < 0x18) {
    iVar7 = 0;
    puStack_34 = &DAT_005123b8;
    do {
      iVar8 = 0;
      if (iVar5 != -1 && -1 < iVar5 + 1) {
        iVar6 = 0;
        puStack_38 = puStack_34;
        do {
          uVar4 = *puStack_38;
          if ((-1 < (int)uVar4) && ((int)uVar4 < 10)) {
            uVar3 = uVar4 & 0x80000001;
            if ((int)uVar3 < 0) {
              uVar3 = (uVar3 - 1 | 0xfffffffe) + 1;
            }
            if ((uVar3 == 1) && ((iVar8 == iVar5 || (iVar8 + 1 == iVar5)))) {
              if (DAT_004fbe54 != 0) {
                DAT_004fbe54 = 1;
              }
LAB_0041fda6:
              uStack_c = 0;
              uStack_4 = 10;
              iStack_8 = ((iVar2 - iVar6) * 0x16) / 100;
              piVar9 = aiStack_28 + 6;
              aiStack_28[6] = uStack_c;
            }
            else {
              if ((uVar3 == 0) && (iVar8 == iVar5)) goto LAB_0041fda6;
              piVar9 = (int *)0x0;
            }
            BlitColorKeyedSurfaceClipped
                      ((int *)(&DAT_00512744)[uVar4],DAT_00444b10 * iVar8 + DAT_00444b04,
                       DAT_00444b0c * iVar7 + DAT_00444b08,piVar9);
            iVar2 = DAT_005109c8;
          }
          iVar5 = iVar2 / 100;
          iVar8 = iVar8 + 1;
          puStack_38 = puStack_38 + 1;
          iVar6 = iVar6 + 100;
        } while (iVar8 < iVar5 + 1);
      }
      puStack_34 = puStack_34 + 0x18;
      iVar7 = iVar7 + 1;
    } while ((int)puStack_34 < 0x512598);
    iVar5 = iVar2 / 100;
    if (DAT_0051218c / 100 != iVar5) {
      iVar8 = 0;
      iVar7 = 0x28;
      do {
        iVar6 = (&DAT_005123b8)[iVar5 + iVar8];
        if ((iVar6 != -1) && (iVar6 != 10)) {
          iVar6 = iVar7 + iVar6;
          uVar4 = CSound_IsSoundPlaying((&DAT_004fc084)[iVar6]);
          if (uVar4 != 0) {
            CSound_Stop((&DAT_004fc084)[iVar6]);
            CSound_Reset((&DAT_004fc084)[iVar6]);
          }
          CSound_Play((void *)(&DAT_004fc084)[iVar6],0,0);
          iVar2 = DAT_005109c8;
          iVar5 = DAT_005109c8 / 100;
          iVar6 = (int)(&DAT_005123b8)[iVar5 + iVar8] / 2;
          if ((&DAT_00512378)[iVar6] == 0) {
            uVar4 = (&DAT_005123b8)[iVar5 + iVar8] & 0x80000001;
            if ((int)uVar4 < 0) {
              uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
            }
            (&DAT_00512378)[iVar6] = uVar4 + 1;
          }
        }
        iVar7 = iVar7 + -10;
        iVar8 = iVar8 + 0x18;
      } while (-10 < iVar7);
    }
    aiStack_28[0] = 0x32;
    aiStack_28[1] = 0x32;
    aiStack_28[2] = 0x32;
    DAT_00512174 = DAT_00512174 + DAT_00446fdc;
    aiStack_28[3] = 0x5a;
    aiStack_28[4] = 0x55;
    aiStack_28[5] = 0x69;
    if (3 < DAT_00512174) {
      DAT_00513f14 = DAT_00513f14 + 1;
      DAT_00512174 = 0;
      if (aiStack_28[DAT_0051c284 + 3] <= DAT_00513f14) {
        DAT_00513f14 = aiStack_28[DAT_0051c284];
      }
      if (DAT_00513f14 < aiStack_28[DAT_0051c284]) {
        DAT_00513f14 = aiStack_28[DAT_0051c284];
      }
    }
    uStack_4 = *(undefined4 *)(&DAT_00444d3c + DAT_0051c284 * 8);
    aiStack_28[6] = *(int *)(&DAT_00444d38 + DAT_0051c284 * 8) * DAT_00513f14;
    iStack_8 = *(int *)(&DAT_00444d38 + DAT_0051c284 * 8) + aiStack_28[6];
    uStack_c = 0;
    DAT_0051218c = iVar2;
    (**(code **)(*piVar1 + 0x1c))
              (piVar1,*(undefined4 *)(&DAT_00444d20 + DAT_0051c284 * 8),
               *(undefined4 *)(&DAT_00444d24 + DAT_0051c284 * 8),DAT_005125a8,aiStack_28 + 6,1);
    (**(code **)(*piVar1 + 0x1c))(piVar1,0,0,DAT_00513f20,0,1);
  }
  return;
}

