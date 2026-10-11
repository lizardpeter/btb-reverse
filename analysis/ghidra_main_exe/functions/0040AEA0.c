/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040aea0; function: LoadParkDesignerData; body bytes: 742
 * callers: 1; callees: 6; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void LoadParkDesignerData(void)

{
  int iVar1;
  int iVar2;
  FILE *pFVar3;
  uint uVar4;
  int *piVar5;
  int iVar6;
  char *pcVar7;
  int local_10;
  undefined4 local_c;
  int local_8;
  int local_4;
  
  pFVar3 = (FILE *)OpenGameDataFileWithCDFallback
                             (s_data_subgamedyp_boundareas_txt_00441f44,&DAT_0043e070);
  iVar6 = 0;
  piVar5 = &DAT_00508b04;
  local_4 = 4;
  local_8 = 5;
  while( true ) {
    while( true ) {
      do {
        local_10 = 0;
        local_c = 0;
        *piVar5 = 0;
        do {
          crt_fscanf(&local_10,(int *)pFVar3,(byte *)s__d__d_0043e920);
          iVar2 = *piVar5;
          iVar1 = iVar2 + iVar6;
          (&DAT_005041b8)[iVar1 * 2] = local_10;
          (&DAT_005041bc)[iVar1 * 2] = local_c;
          *piVar5 = iVar2 + 1;
        } while (local_10 != -1);
        iVar6 = iVar6 + 0x1e;
        piVar5 = piVar5 + 1;
        local_8 = local_8 + -1;
      } while (local_8 != 0);
      local_4 = local_4 + -1;
      if (local_4 == 0) break;
      local_8 = 5;
    }
    if (0x508bf3 < (int)piVar5) break;
    local_4 = 4;
    local_8 = 5;
  }
  crt_fclose(pFVar3);
  pFVar3 = (FILE *)crt_fopen(s_dypdata1_txt_0043f9c8 + DAT_00519934 * 0x32,&DAT_00441f40);
  if (pFVar3 != (FILE *)0x0) {
    pcVar7 = (char *)&DAT_004fcab0;
    do {
      uVar4 = FUN_00430937(pcVar7,0x4c,1,(int *)pFVar3);
      if (uVar4 == 0) {
        return;
      }
      pcVar7 = pcVar7 + 0x4c;
    } while ((int)pcVar7 < 0x504170);
    _DAT_00508c08 = 1;
    FUN_00430937((char *)&DAT_00507a28,4,1,(int *)pFVar3);
    FUN_00430937((char *)&DAT_00507e38,4,1,(int *)pFVar3);
    FUN_00430937((char *)&DAT_005079fc,4,1,(int *)pFVar3);
    FUN_00430937((char *)&DAT_00507e3c,4,1,(int *)pFVar3);
    FUN_00430937((char *)&DAT_00507a40,4,1,(int *)pFVar3);
    FUN_00430937((char *)&DAT_00507c54,4,1,(int *)pFVar3);
    FUN_00430937((char *)&DAT_00507a2c,4,1,(int *)pFVar3);
    FUN_00430937((char *)&DAT_00507a30,4,1,(int *)pFVar3);
    FUN_00430937(&DAT_00507a34,4,1,(int *)pFVar3);
    FUN_00430937(&DAT_00507a38,4,1,(int *)pFVar3);
    FUN_00430937((char *)&DAT_00507b5c,4,1,(int *)pFVar3);
    FUN_00430937(&DAT_00507a3c,4,1,(int *)pFVar3);
    FUN_00430937((char *)&DAT_00507ae4,4,1,(int *)pFVar3);
    FUN_00430937((char *)&DAT_00507ae8,4,1,(int *)pFVar3);
    FUN_00430937((char *)&DAT_00507aec,4,1,(int *)pFVar3);
    FUN_00430937(&DAT_00507af0,4,1,(int *)pFVar3);
    FUN_00430937(&DAT_00507af4,4,1,(int *)pFVar3);
    FUN_00430937((char *)&DAT_00508bf4,4,1,(int *)pFVar3);
    FUN_00430937(&DAT_00508bf8,4,1,(int *)pFVar3);
    FUN_00430937(&DAT_00508bfc,4,1,(int *)pFVar3);
    FUN_00430937(&DAT_00508c00,4,1,(int *)pFVar3);
    FUN_00430937(&DAT_00508c04,4,1,(int *)pFVar3);
    FUN_00430937((char *)&DAT_00509340,4,1,(int *)pFVar3);
    FUN_00430937((char *)&DAT_00441dc8,4,1,(int *)pFVar3);
    FUN_00430937((char *)&DAT_00441dcc,4,1,(int *)pFVar3);
    FUN_00430937((char *)&DAT_00441dd0,4,1,(int *)pFVar3);
    FUN_00430937((char *)&DAT_00441dd4,4,1,(int *)pFVar3);
    FUN_00430937((char *)&DAT_00509344,4,1,(int *)pFVar3);
    if (DAT_00509344 == 1) {
      ApplyParkDesignerSeason(1);
    }
    crt_fclose(pFVar3);
  }
  return;
}

