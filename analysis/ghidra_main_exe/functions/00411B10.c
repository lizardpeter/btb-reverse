/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00411b10; function: SaveFireworksLayoutAndUnloadResources; body bytes: 363
 * callers: 1; callees: 7; success: True
 */


void SaveFireworksLayoutAndUnloadResources(void)

{
  int *piVar1;
  FILE *pFVar2;
  char *pcVar3;
  int *piVar4;
  int iVar5;
  
  pFVar2 = (FILE *)crt_fopen(s_firedata1_txt_00442638 + DAT_00519934 * 0x32,&DAT_0043ee80);
  if (pFVar2 != (FILE *)0x0) {
    pcVar3 = (char *)&DAT_0050a678;
    do {
      iVar5 = 6;
      do {
        FUN_00430a52(pcVar3,4,1,(int *)pFVar2);
        pcVar3 = pcVar3 + 4;
        iVar5 = iVar5 + -1;
      } while (iVar5 != 0);
    } while ((int)pcVar3 < 0x50a6c0);
    crt_fclose(pFVar2);
  }
  UnregisterBitmapSurface(0x50ab0c);
  if (DAT_0050ab0c != (int *)0x0) {
    (**(code **)(*DAT_0050ab0c + 8))(DAT_0050ab0c);
    DAT_0050ab0c = (int *)0x0;
  }
  UnregisterBitmapSurface(0x50ab10);
  if (DAT_0050ab10 != (int *)0x0) {
    (**(code **)(*DAT_0050ab10 + 8))(DAT_0050ab10);
    DAT_0050ab10 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x508c0c);
  if (DAT_00508c0c != (int *)0x0) {
    (**(code **)(*DAT_00508c0c + 8))(DAT_00508c0c);
    DAT_00508c0c = (int *)0x0;
  }
  UnregisterBitmapSurface(0x508ad0);
  if (DAT_00508ad0 != (int *)0x0) {
    (**(code **)(*DAT_00508ad0 + 8))(DAT_00508ad0);
    DAT_00508ad0 = (int *)0x0;
  }
  iVar5 = 0;
  do {
    piVar4 = (int *)((int)&DAT_0050a4b8 + iVar5);
    UnregisterBitmapSurface((int)piVar4);
    piVar1 = (int *)*piVar4;
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 8))(piVar1);
      *piVar4 = 0;
    }
    piVar4 = (int *)((int)&DAT_0050a568 + iVar5);
    UnregisterBitmapSurface((int)piVar4);
    piVar1 = (int *)*piVar4;
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 8))(piVar1);
      *piVar4 = 0;
    }
    iVar5 = iVar5 + 4;
  } while (iVar5 < 0x30);
  piVar4 = &DAT_004fc084;
  do {
    if ((undefined4 *)*piVar4 != (undefined4 *)0x0) {
      (*(code *)**(undefined4 **)*piVar4)(1);
      *piVar4 = 0;
    }
    piVar4 = piVar4 + 1;
  } while ((int)piVar4 < 0x4fc0d4);
  SetCursorSurface((int *)0x0);
  DAT_00519940 = 0;
  if (DAT_0050aaac != 0) {
    UnloadFireworkMovieBank();
  }
  UnloadFireworksEditorResources();
  return;
}

