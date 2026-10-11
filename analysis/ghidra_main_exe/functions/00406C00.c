/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00406c00; function: AbortForRemovedRetailCD; body bytes: 176
 * callers: 4; callees: 9; success: True
 */


void AbortForRemovedRetailCD(void)

{
  char cVar1;
  byte bVar2;
  undefined *puVar3;
  undefined4 *puVar4;
  int iVar5;
  FILE *pFVar6;
  UINT UVar7;
  byte *pbVar8;
  int iVar9;
  uint uVar10;
  uint uVar11;
  char *pcVar12;
  char *pcVar13;
  char *pcVar14;
  byte *pbVar15;
  int iVar16;
  char *pcVar17;
  bool bVar18;
  undefined4 uStack_270;
  undefined1 uStack_26c;
  DWORD DStack_268;
  DWORD DStack_264;
  byte abStack_260 [100];
  char acStack_1fc [484];
  undefined4 uStack_18;
  
  puVar4 = DAT_0044de08;
  DAT_004fbd00 = 1;
  if (DAT_0044de08 != (undefined4 *)0x0) {
    DisplayManagerDestructor(DAT_0044de08);
    FUN_0042fbdc((undefined *)puVar4);
    DAT_0044de08 = (undefined4 *)0x0;
  }
  uStack_18 = 0x406c43;
  MessageBoxA((HWND)0x0,s_CD_Removed_0043e298,s_Error_0043e680,0x10);
  DAT_0044ddb0 = 1;
  PostQuitMessage(0);
  ReleaseInputAndCursorResources();
  ShutdownDirectSound8();
  puVar3 = DAT_0044ddd8;
  if (DAT_0044ddd8 != (undefined *)0x0) {
    NoOpLegacyHook();
    FUN_0042fbdc(puVar3);
    DAT_0044ddd8 = (undefined *)0x0;
  }
  if (DAT_0044de0c == 0) {
    UnhookWindowsHookEx(DAT_0044ddc8);
  }
  DAT_0044de10 = 0;
  FUN_00430862(0);
  pFVar6 = (FILE *)crt_fopen(s_error_txt_0043e8a4,&DAT_0043e8b0);
  if (pFVar6 != (FILE *)0x0) {
    pcVar12 = s_CD_Removed_0043e298;
    do {
      iVar16 = 0;
      pcVar14 = acStack_1fc;
      while( true ) {
        FUN_00430937(pcVar14,1,1,(int *)pFVar6);
        if ((*pcVar14 == 0x2f6e) || (*pcVar14 == '\n')) break;
        iVar16 = iVar16 + 1;
        pcVar14 = pcVar14 + 1;
      }
      acStack_1fc[iVar16] = '\0';
      uVar10 = 0xffffffff;
      pcVar13 = pcVar12 + 500;
      pcVar14 = acStack_1fc;
      do {
        pcVar17 = pcVar14;
        if (uVar10 == 0) break;
        uVar10 = uVar10 - 1;
        pcVar17 = pcVar14 + 1;
        cVar1 = *pcVar14;
        pcVar14 = pcVar17;
      } while (cVar1 != '\0');
      uVar10 = ~uVar10;
      pcVar14 = pcVar17 + -uVar10;
      for (uVar11 = uVar10 >> 2; uVar11 != 0; uVar11 = uVar11 - 1) {
        *(undefined4 *)pcVar12 = *(undefined4 *)pcVar14;
        pcVar14 = pcVar14 + 4;
        pcVar12 = pcVar12 + 4;
      }
      for (uVar10 = uVar10 & 3; uVar10 != 0; uVar10 = uVar10 - 1) {
        *pcVar12 = *pcVar14;
        pcVar14 = pcVar14 + 1;
        pcVar12 = pcVar12 + 1;
      }
      pcVar12 = pcVar13;
    } while ((int)pcVar13 < 0x43e874);
  }
  crt_fclose(pFVar6);
  uStack_270 = DAT_0043e0a4;
  uStack_26c = 0;
  iVar16 = 0;
  do {
    uStack_270 = CONCAT31(uStack_270._1_3_,(char)iVar16 + 'a');
    UVar7 = GetDriveTypeA((LPCSTR)&uStack_270);
    if (UVar7 == 5) {
      GetVolumeInformationA
                ((LPCSTR)&uStack_270,(LPSTR)abStack_260,100,(LPDWORD)0x0,&DStack_264,&DStack_268,
                 (LPSTR)0x0,100);
      pbVar15 = abStack_260;
      pbVar8 = &DAT_0043e09c;
      do {
        bVar2 = *pbVar8;
        bVar18 = bVar2 < *pbVar15;
        if (bVar2 != *pbVar15) {
LAB_00406dc5:
          iVar9 = (1 - (uint)bVar18) - (uint)(bVar18 != 0);
          goto LAB_00406dca;
        }
        if (bVar2 == 0) break;
        bVar2 = pbVar8[1];
        bVar18 = bVar2 < pbVar15[1];
        if (bVar2 != pbVar15[1]) goto LAB_00406dc5;
        pbVar8 = pbVar8 + 2;
        pbVar15 = pbVar15 + 2;
      } while (bVar2 != 0);
      iVar9 = 0;
LAB_00406dca:
      iVar5 = iVar16;
      if (iVar9 == 0) break;
    }
    iVar16 = iVar16 + 1;
    iVar5 = DAT_0043e874;
  } while (iVar16 < 0x1a);
  DAT_0043e874 = iVar5;
  if (DAT_0043e874 == -1) {
    MessageBoxA((HWND)0x0,s_Please_insert_the_CD_and_re_run_t_0043e48c,s_Error_0043e680,0);
    FUN_00430862(1);
  }
  return;
}

