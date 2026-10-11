/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041d920; function: SaveAndUnloadBobsBandActivity; body bytes: 508
 * callers: 1; callees: 5; success: True
 */


void SaveAndUnloadBobsBandActivity(void)

{
  int *piVar1;
  int *piVar2;
  FILE *pFVar3;
  int *piVar4;
  char *pcVar5;
  int iVar6;
  
  UnregisterBitmapSurface(0x513f1c);
  if (DAT_00513f1c != (int *)0x0) {
    (**(code **)(*DAT_00513f1c + 8))(DAT_00513f1c);
    DAT_00513f1c = (int *)0x0;
  }
  UnregisterBitmapSurface(0x513f20);
  if (DAT_00513f20 != (int *)0x0) {
    (**(code **)(*DAT_00513f20 + 8))(DAT_00513f20);
    DAT_00513f20 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x508c0c);
  if (DAT_00508c0c != (int *)0x0) {
    (**(code **)(*DAT_00508c0c + 8))(DAT_00508c0c);
    DAT_00508c0c = (int *)0x0;
  }
  piVar4 = &DAT_00512718;
  do {
    UnregisterBitmapSurface((int)piVar4);
    piVar1 = (int *)*piVar4;
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 8))(piVar1);
      *piVar4 = 0;
    }
    piVar4 = piVar4 + 1;
  } while ((int)piVar4 < 0x512740);
  UnregisterBitmapSurface(0x5125a8);
  if (DAT_005125a8 != (int *)0x0) {
    (**(code **)(*DAT_005125a8 + 8))(DAT_005125a8);
    DAT_005125a8 = (int *)0x0;
  }
  piVar4 = &DAT_004fc084;
  do {
    if ((undefined4 *)*piVar4 != (undefined4 *)0x0) {
      (*(code *)**(undefined4 **)*piVar4)(1);
      *piVar4 = 0;
    }
    piVar4 = piVar4 + 1;
  } while ((int)piVar4 < 0x4fc174);
  piVar4 = &DAT_00512770;
  do {
    piVar1 = piVar4 + -1;
    UnregisterBitmapSurface((int)piVar1);
    piVar2 = (int *)*piVar1;
    if (piVar2 != (int *)0x0) {
      (**(code **)(*piVar2 + 8))(piVar2);
      *piVar1 = 0;
    }
    UnregisterBitmapSurface((int)piVar4);
    piVar1 = (int *)*piVar4;
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 8))(piVar1);
      *piVar4 = 0;
    }
    piVar4 = piVar4 + 2;
  } while ((int)piVar4 < 0x512790);
  iVar6 = 0;
  do {
    piVar4 = (int *)((int)&DAT_00512390 + iVar6);
    UnregisterBitmapSurface((int)piVar4);
    piVar1 = (int *)*piVar4;
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 8))(piVar1);
      *piVar4 = 0;
    }
    piVar4 = (int *)((int)&DAT_00512744 + iVar6);
    UnregisterBitmapSurface((int)piVar4);
    piVar1 = (int *)*piVar4;
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 8))(piVar1);
      *piVar4 = 0;
    }
    iVar6 = iVar6 + 4;
  } while (iVar6 < 0x28);
  SetCursorSurface((int *)0x0);
  DAT_00519940 = 0;
  pFVar3 = (FILE *)crt_fopen(s_musicbob1_txt_00444384 + (DAT_00519934 + DAT_0051c284 * 5) * 0x80,
                             &DAT_0043ee80);
  if (pFVar3 != (FILE *)0x0) {
    pcVar5 = (char *)&DAT_005123b8;
    do {
      iVar6 = 0x18;
      do {
        FUN_00430a52(pcVar5,4,1,(int *)pFVar3);
        pcVar5 = pcVar5 + 4;
        iVar6 = iVar6 + -1;
      } while (iVar6 != 0);
    } while ((int)pcVar5 < 0x512598);
    crt_fclose(pFVar3);
    pFVar3 = (FILE *)crt_fopen(s_last_txt_00444db8,&DAT_0043ee80);
    if (pFVar3 != (FILE *)0x0) {
      iVar6 = 5;
      do {
        FUN_00430a52((char *)&DAT_0051c284,4,1,(int *)pFVar3);
        iVar6 = iVar6 + -1;
      } while (iVar6 != 0);
      crt_fclose(pFVar3);
    }
  }
  return;
}

