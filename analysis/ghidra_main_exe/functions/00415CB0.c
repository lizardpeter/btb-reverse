/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00415cb0; function: LoadHerdingCoordinateData; body bytes: 119
 * callers: 1; callees: 3; success: True
 */


void LoadHerdingCoordinateData(void)

{
  FILE *pFVar1;
  int iVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined4 local_8;
  undefined4 local_4;
  
  iVar4 = 0;
  pFVar1 = (FILE *)OpenGameDataFileWithCDFallback(s_Data_SubGame1_herd_txt_00443b28,&DAT_0043e070);
  puVar3 = &DAT_005101cc;
  while( true ) {
    iVar2 = crt_fscanf(&local_8,(int *)pFVar1,(byte *)s__d__d_0043e920);
    if (iVar2 == -1) break;
    puVar3[-1] = local_8;
    *puVar3 = local_4;
    iVar4 = iVar4 + 1;
    puVar3 = puVar3 + 2;
  }
  crt_fclose(pFVar1);
  (&DAT_005101c8)[iVar4 * 2] = 0xffffffff;
  (&DAT_005101cc)[iVar4 * 2] = 0xffffffff;
  return;
}

