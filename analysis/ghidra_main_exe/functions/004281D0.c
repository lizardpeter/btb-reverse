/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004281d0; function: OpenWalkthroughMovie; body bytes: 225
 * callers: 1; callees: 6; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl OpenWalkthroughMovie(int param_1)

{
  int *piVar1;
  char *pcVar2;
  FILE *pFVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  char *pcVar7;
  char *pcVar8;
  undefined4 *puVar9;
  
  pcVar2 = (char *)_malloc(7000000);
  pFVar3 = (FILE *)OpenGameDataFileWithCDFallback(&DAT_0048e6f0 + param_1 * 0x104,&DAT_00441f40);
  uVar4 = FUN_00430937(pcVar2,1,7000000,(int *)pFVar3);
  DAT_0051c2f0 = (char *)_malloc(uVar4);
  pcVar7 = pcVar2;
  pcVar8 = DAT_0051c2f0;
  for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
    *(undefined4 *)pcVar8 = *(undefined4 *)pcVar7;
    pcVar7 = pcVar7 + 4;
    pcVar8 = pcVar8 + 4;
  }
  for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
    *pcVar8 = *pcVar7;
    pcVar7 = pcVar7 + 1;
    pcVar8 = pcVar8 + 1;
  }
  piVar1 = *(int **)(DAT_0044de08 + 4);
  puVar9 = &DAT_005168e0;
  for (iVar6 = 0x1f; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar9 = 0;
    puVar9 = puVar9 + 1;
  }
  DAT_005168e0 = 0x7c;
  DAT_005168e4 = 7;
  _DAT_005168ec = 0x11a;
  _DAT_005168e8 = 0xb3;
  _DAT_00516948 = 0x40;
  (**(code **)(*piVar1 + 0x18))(piVar1,&DAT_005168e0,&DAT_0051be3c,0);
  DAT_0051bd2c = OpenBinkMovieWithFallback(DAT_0051c2f0);
  crt_fclose(pFVar3);
  FUN_00430d2a(pcVar2);
  return;
}

