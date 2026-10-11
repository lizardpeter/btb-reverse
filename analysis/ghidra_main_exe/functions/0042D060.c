/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0042d060; function: SaveAndUnloadPlayerProfiles; body bytes: 523
 * callers: 1; callees: 5; success: True
 */


void SaveAndUnloadPlayerProfiles(void)

{
  int *piVar1;
  FILE *pFVar2;
  int *piVar3;
  int iVar4;
  undefined4 *puStack_4;
  
  UnregisterBitmapSurface(0x51b39c);
  if (DAT_0051b39c != (int *)0x0) {
    (**(code **)(*DAT_0051b39c + 8))(DAT_0051b39c);
    DAT_0051b39c = (int *)0x0;
  }
  UnregisterBitmapSurface(0x51993c);
  if (DAT_0051993c != (int *)0x0) {
    (**(code **)(*DAT_0051993c + 8))(DAT_0051993c);
    DAT_0051993c = (int *)0x0;
  }
  UnregisterBitmapSurface(0x51b3a4);
  if (DAT_0051b3a4 != (int *)0x0) {
    (**(code **)(*DAT_0051b3a4 + 8))(DAT_0051b3a4);
    DAT_0051b3a4 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x519958);
  if (DAT_00519958 != (int *)0x0) {
    (**(code **)(*DAT_00519958 + 8))(DAT_00519958);
    DAT_00519958 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x51b398);
  if (DAT_0051b398 != (int *)0x0) {
    (**(code **)(*DAT_0051b398 + 8))(DAT_0051b398);
    DAT_0051b398 = (int *)0x0;
  }
  piVar3 = &DAT_0051b3a8;
  do {
    iVar4 = 2;
    do {
      UnregisterBitmapSurface((int)piVar3);
      piVar1 = (int *)*piVar3;
      if (piVar1 != (int *)0x0) {
        (**(code **)(*piVar1 + 8))(piVar1);
        *piVar3 = 0;
      }
      piVar3 = piVar3 + 1;
      iVar4 = iVar4 + -1;
    } while (iVar4 != 0);
  } while ((int)piVar3 < 0x51b3f8);
  UnregisterBitmapSurface(0x519938);
  if (DAT_00519938 != (int *)0x0) {
    (**(code **)(*DAT_00519938 + 8))(DAT_00519938);
    DAT_00519938 = (int *)0x0;
  }
  pFVar2 = (FILE *)crt_fopen(s_playerinfo_txt_0044716c,&DAT_0043ee80);
  if (pFVar2 != (FILE *)0x0) {
    iVar4 = 0;
    do {
      FUN_00430fb8((int *)pFVar2,(byte *)s__d__d_00447164);
      iVar4 = iVar4 + 4;
    } while (iVar4 < 0x14);
    piVar3 = &DAT_0051b404;
    do {
      iVar4 = 0;
      if (0 < *piVar3) {
        do {
          FUN_00430fb8((int *)pFVar2,&DAT_00444240);
          iVar4 = iVar4 + 1;
        } while (iVar4 < *piVar3);
      }
      piVar3 = piVar3 + 1;
    } while ((int)piVar3 < 0x51b418);
  }
  crt_fclose(pFVar2);
  puStack_4 = &DAT_0051b4d0;
  do {
    crt_sprintf(&DAT_0051bd30,(byte *)s_player_d_txt_00447154);
    pFVar2 = (FILE *)crt_fopen(&DAT_0051bd30,&DAT_00447150);
    if (pFVar2 != (FILE *)0x0) {
      iVar4 = 100;
      do {
        FUN_00430fb8((int *)pFVar2,&DAT_00444240);
        iVar4 = iVar4 + -1;
      } while (iVar4 != 0);
      crt_fclose(pFVar2);
    }
    puStack_4 = puStack_4 + 100;
  } while ((int)puStack_4 < 0x51bca0);
  return;
}

