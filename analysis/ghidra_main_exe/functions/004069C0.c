/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004069c0; function: LoadCreditsSlideshow; body bytes: 271
 * callers: 1; callees: 9; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 LoadCreditsSlideshow(void)

{
  int *piVar1;
  FILE *pFVar2;
  int iVar3;
  int *piVar4;
  void *extraout_ECX;
  void *extraout_ECX_00;
  void *this;
  void *extraout_ECX_01;
  LPCSTR pCVar5;
  undefined4 *puVar6;
  undefined8 uVar7;
  _SYSTEMTIME local_20;
  _SYSTEMTIME local_10;
  
  piVar1 = *(int **)(DAT_0044de08 + 4);
  pFVar2 = (FILE *)OpenGameDataFileWithCDFallback(s_loaddata_credits_txt_0043e88c,&DAT_0043e070);
  this = extraout_ECX;
  if (pFVar2 == (FILE *)0x0) {
    NoOpLegacyHook();
    this = extraout_ECX_00;
  }
  while ((iVar3 = crt_fscanf(this,(int *)pFVar2,&DAT_0043e054), iVar3 != -1 && (DAT_004fbd04 < 10)))
  {
    DAT_004fbd04 = DAT_004fbd04 + 1;
    this = extraout_ECX_01;
  }
  crt_fclose(pFVar2);
  iVar3 = 0;
  if (0 < DAT_004fbd04) {
    puVar6 = &DAT_0048d5c8;
    pCVar5 = &DAT_0048d5f0;
    do {
      piVar4 = LoadBitmapToDirectDrawSurface(piVar1,pCVar5,0,0);
      *puVar6 = piVar4;
      RegisterBitmapSurface(puVar6,pCVar5);
      iVar3 = iVar3 + 1;
      pCVar5 = pCVar5 + 0x80;
      puVar6 = puVar6 + 1;
    } while (iVar3 < DAT_004fbd04);
  }
  _DAT_004fbd08 = 0xf0;
  DAT_00510d08 = 4;
  GetSystemTime(&local_20);
  SystemTimeToFileTime(&local_20,(LPFILETIME)&DAT_00510d10);
  GetSystemTime(&local_10);
  SystemTimeToFileTime(&local_10,(LPFILETIME)&DAT_005120f0);
  uVar7 = Divide64BitDeltaByTenMillion((uint *)&DAT_00510d10,(uint *)&DAT_005120f0);
  return uVar7;
}

