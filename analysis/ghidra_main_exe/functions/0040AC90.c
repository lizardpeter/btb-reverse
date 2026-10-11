/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040ac90; function: SaveParkDesignerData; body bytes: 527
 * callers: 1; callees: 3; success: True
 */


void SaveParkDesignerData(void)

{
  FILE *pFVar1;
  char *pcVar2;
  
  pFVar1 = (FILE *)crt_fopen(s_dypdata1_txt_0043f9c8 + DAT_00519934 * 0x32,&DAT_0043ee80);
  if (pFVar1 != (FILE *)0x0) {
    pcVar2 = (char *)&DAT_004fcab0;
    do {
      FUN_00430a52(pcVar2,0x4c,1,(int *)pFVar1);
      pcVar2 = pcVar2 + 0x4c;
    } while ((int)pcVar2 < 0x504170);
    FUN_00430a52((char *)&DAT_00507a28,4,1,(int *)pFVar1);
    FUN_00430a52((char *)&DAT_00507e38,4,1,(int *)pFVar1);
    FUN_00430a52((char *)&DAT_005079fc,4,1,(int *)pFVar1);
    FUN_00430a52((char *)&DAT_00507e3c,4,1,(int *)pFVar1);
    FUN_00430a52((char *)&DAT_00507a40,4,1,(int *)pFVar1);
    FUN_00430a52((char *)&DAT_00507c54,4,1,(int *)pFVar1);
    FUN_00430a52((char *)&DAT_00507a2c,4,1,(int *)pFVar1);
    FUN_00430a52((char *)&DAT_00507a30,4,1,(int *)pFVar1);
    FUN_00430a52(&DAT_00507a34,4,1,(int *)pFVar1);
    FUN_00430a52(&DAT_00507a38,4,1,(int *)pFVar1);
    FUN_00430a52((char *)&DAT_00507b5c,4,1,(int *)pFVar1);
    FUN_00430a52(&DAT_00507a3c,4,1,(int *)pFVar1);
    FUN_00430a52((char *)&DAT_00507ae4,4,1,(int *)pFVar1);
    FUN_00430a52((char *)&DAT_00507ae8,4,1,(int *)pFVar1);
    FUN_00430a52((char *)&DAT_00507aec,4,1,(int *)pFVar1);
    FUN_00430a52(&DAT_00507af0,4,1,(int *)pFVar1);
    FUN_00430a52(&DAT_00507af4,4,1,(int *)pFVar1);
    FUN_00430a52((char *)&DAT_00508bf4,4,1,(int *)pFVar1);
    FUN_00430a52(&DAT_00508bf8,4,1,(int *)pFVar1);
    FUN_00430a52(&DAT_00508bfc,4,1,(int *)pFVar1);
    FUN_00430a52(&DAT_00508c00,4,1,(int *)pFVar1);
    FUN_00430a52(&DAT_00508c04,4,1,(int *)pFVar1);
    FUN_00430a52((char *)&DAT_00509340,4,1,(int *)pFVar1);
    FUN_00430a52((char *)&DAT_00441dc8,4,1,(int *)pFVar1);
    FUN_00430a52((char *)&DAT_00441dcc,4,1,(int *)pFVar1);
    FUN_00430a52((char *)&DAT_00441dd0,4,1,(int *)pFVar1);
    FUN_00430a52((char *)&DAT_00441dd4,4,1,(int *)pFVar1);
    FUN_00430a52((char *)&DAT_00509344,4,1,(int *)pFVar1);
    crt_fclose(pFVar1);
  }
  return;
}

