/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00406cb0; function: FUN_00406cb0; body bytes: 348
 * callers: 1; callees: 7; success: True
 */


void FUN_00406cb0(void)

{
  char cVar1;
  byte bVar2;
  int iVar3;
  FILE *pFVar4;
  UINT UVar5;
  byte *pbVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  char *pcVar10;
  char *pcVar11;
  char *pcVar12;
  byte *pbVar13;
  int iVar14;
  char *pcVar15;
  bool bVar16;
  undefined4 local_26c;
  undefined1 local_268;
  DWORD local_264;
  DWORD local_260;
  byte local_25c [100];
  char local_1f8 [504];
  
  pFVar4 = (FILE *)crt_fopen(s_error_txt_0043e8a4,&DAT_0043e8b0);
  if (pFVar4 != (FILE *)0x0) {
    pcVar10 = s_CD_Removed_0043e298;
    do {
      iVar14 = 0;
      pcVar12 = local_1f8;
      while( true ) {
        FUN_00430937(pcVar12,1,1,(int *)pFVar4);
        if ((*pcVar12 == 0x2f6e) || (*pcVar12 == '\n')) break;
        iVar14 = iVar14 + 1;
        pcVar12 = pcVar12 + 1;
      }
      local_1f8[iVar14] = '\0';
      uVar8 = 0xffffffff;
      pcVar11 = pcVar10 + 500;
      pcVar12 = local_1f8;
      do {
        pcVar15 = pcVar12;
        if (uVar8 == 0) break;
        uVar8 = uVar8 - 1;
        pcVar15 = pcVar12 + 1;
        cVar1 = *pcVar12;
        pcVar12 = pcVar15;
      } while (cVar1 != '\0');
      uVar8 = ~uVar8;
      pcVar12 = pcVar15 + -uVar8;
      for (uVar9 = uVar8 >> 2; uVar9 != 0; uVar9 = uVar9 - 1) {
        *(undefined4 *)pcVar10 = *(undefined4 *)pcVar12;
        pcVar12 = pcVar12 + 4;
        pcVar10 = pcVar10 + 4;
      }
      for (uVar8 = uVar8 & 3; uVar8 != 0; uVar8 = uVar8 - 1) {
        *pcVar10 = *pcVar12;
        pcVar12 = pcVar12 + 1;
        pcVar10 = pcVar10 + 1;
      }
      pcVar10 = pcVar11;
    } while ((int)pcVar11 < 0x43e874);
  }
  crt_fclose(pFVar4);
  local_26c = DAT_0043e0a4;
  local_268 = 0;
  iVar14 = 0;
  do {
    local_26c = CONCAT31(local_26c._1_3_,(char)iVar14 + 'a');
    UVar5 = GetDriveTypeA((LPCSTR)&local_26c);
    if (UVar5 == 5) {
      GetVolumeInformationA
                ((LPCSTR)&local_26c,(LPSTR)local_25c,100,(LPDWORD)0x0,&local_260,&local_264,
                 (LPSTR)0x0,100);
      pbVar13 = local_25c;
      pbVar6 = &DAT_0043e09c;
      do {
        bVar2 = *pbVar6;
        bVar16 = bVar2 < *pbVar13;
        if (bVar2 != *pbVar13) {
LAB_00406dc5:
          iVar7 = (1 - (uint)bVar16) - (uint)(bVar16 != 0);
          goto LAB_00406dca;
        }
        if (bVar2 == 0) break;
        bVar2 = pbVar6[1];
        bVar16 = bVar2 < pbVar13[1];
        if (bVar2 != pbVar13[1]) goto LAB_00406dc5;
        pbVar6 = pbVar6 + 2;
        pbVar13 = pbVar13 + 2;
      } while (bVar2 != 0);
      iVar7 = 0;
LAB_00406dca:
      iVar3 = iVar14;
      if (iVar7 == 0) break;
    }
    iVar14 = iVar14 + 1;
    iVar3 = DAT_0043e874;
  } while (iVar14 < 0x1a);
  DAT_0043e874 = iVar3;
  if (DAT_0043e874 == -1) {
    MessageBoxA((HWND)0x0,s_Please_insert_the_CD_and_re_run_t_0043e48c,s_Error_0043e680,0);
    FUN_00430862(1);
  }
  return;
}

