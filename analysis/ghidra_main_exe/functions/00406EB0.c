/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00406eb0; function: LoadHelpWavTable; body bytes: 172
 * callers: 1; callees: 4; success: True
 */


void LoadHelpWavTable(void)

{
  char cVar1;
  undefined1 *puVar2;
  FILE *pFVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  char *pcVar7;
  char *pcVar8;
  int local_108;
  char local_104 [260];
  
  puVar2 = &DAT_004f5770;
  do {
    *puVar2 = 0;
    puVar2 = puVar2 + 0x104;
  } while ((int)puVar2 < 0x4fbd00);
  pFVar3 = (FILE *)OpenGameDataFileWithCDFallback(s_loaddata_HelpWav_txt_0043e8cc,&DAT_0043e070);
  if (pFVar3 == (FILE *)0x0) {
    NoOpLegacyHook();
  }
  do {
    iVar4 = crt_fscanf(local_104,(int *)pFVar3,(byte *)s__s__d_0043e100);
    if ((iVar4 == -1) || (local_108 == -1)) {
      crt_fclose(pFVar3);
      return;
    }
    uVar5 = 0xffffffff;
    pcVar7 = local_104;
    do {
      pcVar8 = pcVar7;
      if (uVar5 == 0) break;
      uVar5 = uVar5 - 1;
      pcVar8 = pcVar7 + 1;
      cVar1 = *pcVar7;
      pcVar7 = pcVar8;
    } while (cVar1 != '\0');
    uVar5 = ~uVar5;
    pcVar7 = pcVar8 + -uVar5;
    pcVar8 = &DAT_004f5770 + local_108 * 0x104;
    for (uVar6 = uVar5 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
      *(undefined4 *)pcVar8 = *(undefined4 *)pcVar7;
      pcVar7 = pcVar7 + 4;
      pcVar8 = pcVar8 + 4;
    }
    for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
      *pcVar8 = *pcVar7;
      pcVar7 = pcVar7 + 1;
      pcVar8 = pcVar8 + 1;
    }
  } while( true );
}

