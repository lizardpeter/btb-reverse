/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041f090; function: DrawGrandOpeningCompositionAndMachines; body bytes: 775
 * callers: 3; callees: 2; success: True
 */


/* WARNING: Type propagation algorithm not settling */

void DrawGrandOpeningCompositionAndMachines(void)

{
  int *piVar1;
  int iVar2;
  bool bVar3;
  uint uVar4;
  int iVar5;
  int *piVar6;
  int iVar7;
  int iVar8;
  int iStack_48;
  int aiStack_2c [4];
  undefined4 uStack_1c;
  
  piVar1 = *(int **)(DAT_0044de08 + 0xc);
  iStack_48 = 0;
  (**(code **)(*piVar1 + 0x1c))(piVar1,0,0,DAT_00513f1c,0);
  iVar8 = 0;
  piVar6 = &DAT_005123b8;
  do {
    iVar7 = 0;
    do {
      iVar5 = *piVar6;
      if ((-1 < iVar5) && (iVar5 < 10)) {
        BlitColorKeyedSurfaceClipped
                  ((int *)(&DAT_00512390)[iVar5],DAT_00444b10 * iVar7 + DAT_00444b04,
                   DAT_00444b0c * iVar8 + DAT_00444b08,(int *)0x0);
      }
      iVar7 = iVar7 + 1;
      piVar6 = piVar6 + 1;
    } while (iVar7 < 0x18);
    iVar8 = iVar8 + 1;
  } while ((int)piVar6 < 0x512598);
  bVar3 = false;
  aiStack_2c[0] = 8;
  iVar8 = 0;
  do {
    iVar7 = *(int *)((int)&DAT_00512388 + iVar8);
    if (iVar7 == 0) {
      iVar7 = *(int *)((int)aiStack_2c + iVar8);
      aiStack_2c[2] = 0;
      aiStack_2c[1] = 0;
      aiStack_2c[3] = *(int *)(&DAT_00444cd0 + iVar7 * 8);
      uStack_1c = *(undefined4 *)(&DAT_00444cd4 + iVar7 * 8);
      (**(code **)(*piVar1 + 0x1c))
                (piVar1,*(undefined4 *)(&DAT_00444c80 + iVar7 * 8),
                 *(undefined4 *)(&DAT_00444c84 + iVar7 * 8),(&DAT_00512718)[iVar7],aiStack_2c + 1,1)
      ;
    }
    else {
      iVar5 = *(int *)(iVar8 + 0x512170) + DAT_00446fdc;
      *(int *)(iVar8 + 0x512170) = iVar5;
      if (3 < iVar5) {
        iVar5 = *(int *)(&DAT_00513f10 + iVar8);
        iVar2 = *(int *)(&DAT_00444d78 + iVar7 * 4);
        *(undefined4 *)(iVar8 + 0x512170) = 0;
        *(int *)(&DAT_00513f10 + iVar8) = iVar5 + 1;
        if (iVar2 <= iVar5 + 1) {
          *(undefined4 *)(&DAT_00513f10 + iVar8) = 0;
          bVar3 = true;
        }
      }
      if (iVar7 == 1) {
        iVar7 = *(int *)((int)aiStack_2c + iVar8);
        aiStack_2c[2] = 0;
        uStack_1c = *(undefined4 *)(&DAT_00444cd4 + iVar7 * 8);
        aiStack_2c[1] = *(int *)(&DAT_00513f10 + iVar8) * *(int *)(&DAT_00444cd0 + iVar7 * 8);
        aiStack_2c[3] = *(int *)(&DAT_00444cd0 + iVar7 * 8) + aiStack_2c[1];
        (**(code **)(*piVar1 + 0x1c))
                  (piVar1,*(undefined4 *)(&DAT_00444c80 + iVar7 * 8),
                   *(undefined4 *)(&DAT_00444c84 + iVar7 * 8),(&DAT_00512718)[iVar7],aiStack_2c + 1,
                   1);
      }
      else {
        if (iVar7 != 2) goto LAB_0041f29b;
        iVar7 = *(int *)((int)aiStack_2c + iVar8);
        aiStack_2c[2] = 0;
        uStack_1c = *(undefined4 *)(&DAT_00444cdc + iVar7 * 8);
        aiStack_2c[1] = *(int *)(&DAT_00513f10 + iVar8) * *(int *)(&DAT_00444cd8 + iVar7 * 8);
        aiStack_2c[3] = *(int *)(&DAT_00444cd8 + iVar7 * 8) + aiStack_2c[1];
        (**(code **)(*piVar1 + 0x1c))
                  (piVar1,*(undefined4 *)(&DAT_00444c88 + iVar7 * 8),
                   *(undefined4 *)(&DAT_00444c8c + iVar7 * 8),
                   *(undefined4 *)(&DAT_0051271c + iVar7 * 4),aiStack_2c + 1,1);
      }
      if (bVar3) {
        *(undefined4 *)((int)&DAT_00512388 + iVar8) = 0;
        bVar3 = false;
      }
    }
LAB_0041f29b:
    iVar8 = iVar8 + -4;
    if (iVar8 < -0x10) {
      DAT_00513f30 = DAT_00513f30 + 1;
      iStack_48 = 0x14;
      if (5 < DAT_00513f30) {
        DAT_00513f2c = DAT_00513f2c + 1;
        DAT_00513f30 = 0;
        if ((DAT_00513f2c == (&iStack_48)[DAT_0051c284]) &&
           (uVar4 = FUN_0042ffc4(), (int)uVar4 % 3 != 0)) {
          DAT_00513f2c = 0;
        }
        if (*(int *)(&stack0xffffffc4 + DAT_0051c284 * 4) <= DAT_00513f2c) {
          DAT_00513f2c = 0;
        }
      }
      uStack_1c = *(undefined4 *)(&DAT_00444d3c + DAT_0051c284 * 8);
      aiStack_2c[1] = *(int *)(&DAT_00444d38 + DAT_0051c284 * 8) * DAT_00513f2c;
      aiStack_2c[3] = *(int *)(&DAT_00444d38 + DAT_0051c284 * 8) + aiStack_2c[1];
      aiStack_2c[2] = 0;
      (**(code **)(*piVar1 + 0x1c))
                (piVar1,*(undefined4 *)(&DAT_00444d20 + DAT_0051c284 * 8),
                 *(undefined4 *)(&DAT_00444d24 + DAT_0051c284 * 8),DAT_005125a8,aiStack_2c + 1,1);
      (**(code **)(*piVar1 + 0x1c))(piVar1,0,0,DAT_00513f20,0,1);
      return;
    }
  } while( true );
}

