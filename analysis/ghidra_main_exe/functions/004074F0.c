/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004074f0; function: LoadQuitConfirmationData; body bytes: 640
 * callers: 1; callees: 4; success: True
 */


void LoadQuitConfirmationData(void)

{
  byte bVar1;
  FILE *pFVar2;
  byte *pbVar3;
  int iVar4;
  void *extraout_ECX;
  void *extraout_ECX_00;
  void *this;
  byte *pbVar5;
  byte *pbVar6;
  bool bVar7;
  
  pFVar2 = (FILE *)OpenGameDataFileWithCDFallback(s_loaddata_quitsure_txt_0043ec04,&DAT_0043e070);
  this = extraout_ECX;
  if (pFVar2 == (FILE *)0x0) {
    NoOpLegacyHook();
    this = extraout_ECX_00;
  }
  crt_fscanf(this,(int *)pFVar2,(byte *)s__d__d__d__d__d__d__d__d__s__s__s_0043eb30);
  pbVar5 = &DAT_00515f4c;
  do {
    pbVar6 = &DAT_0043e95c;
    pbVar3 = pbVar5 + -0x300;
    do {
      bVar1 = *pbVar3;
      bVar7 = bVar1 < *pbVar6;
      if (bVar1 != *pbVar6) {
LAB_00407696:
        iVar4 = (1 - (uint)bVar7) - (uint)(bVar7 != 0);
        goto LAB_0040769b;
      }
      if (bVar1 == 0) break;
      bVar1 = pbVar3[1];
      bVar7 = bVar1 < pbVar6[1];
      if (bVar1 != pbVar6[1]) goto LAB_00407696;
      pbVar3 = pbVar3 + 2;
      pbVar6 = pbVar6 + 2;
    } while (bVar1 != 0);
    iVar4 = 0;
LAB_0040769b:
    if (iVar4 == 0) {
      pbVar5[-0x300] = 0;
    }
    pbVar6 = &DAT_0043e95c;
    pbVar3 = pbVar5;
    do {
      bVar1 = *pbVar3;
      bVar7 = bVar1 < *pbVar6;
      if (bVar1 != *pbVar6) {
LAB_004076cc:
        iVar4 = (1 - (uint)bVar7) - (uint)(bVar7 != 0);
        goto LAB_004076d1;
      }
      if (bVar1 == 0) break;
      bVar1 = pbVar3[1];
      bVar7 = bVar1 < pbVar6[1];
      if (bVar1 != pbVar6[1]) goto LAB_004076cc;
      pbVar3 = pbVar3 + 2;
      pbVar6 = pbVar6 + 2;
    } while (bVar1 != 0);
    iVar4 = 0;
LAB_004076d1:
    if (iVar4 == 0) {
      *pbVar5 = 0;
    }
    pbVar6 = &DAT_0043e95c;
    pbVar3 = pbVar5 + 0x600;
    do {
      bVar1 = *pbVar3;
      bVar7 = bVar1 < *pbVar6;
      if (bVar1 != *pbVar6) {
LAB_00407709:
        iVar4 = (1 - (uint)bVar7) - (uint)(bVar7 != 0);
        goto LAB_0040770e;
      }
      if (bVar1 == 0) break;
      bVar1 = pbVar3[1];
      bVar7 = bVar1 < pbVar6[1];
      if (bVar1 != pbVar6[1]) goto LAB_00407709;
      pbVar3 = pbVar3 + 2;
      pbVar6 = pbVar6 + 2;
    } while (bVar1 != 0);
    iVar4 = 0;
LAB_0040770e:
    if (iVar4 == 0) {
      pbVar5[0x600] = 0;
    }
    pbVar6 = &DAT_0043e95c;
    pbVar3 = pbVar5 + 0x300;
    do {
      bVar1 = *pbVar3;
      bVar7 = bVar1 < *pbVar6;
      if (bVar1 != *pbVar6) {
LAB_00407745:
        iVar4 = (1 - (uint)bVar7) - (uint)(bVar7 != 0);
        goto LAB_0040774a;
      }
      if (bVar1 == 0) break;
      bVar1 = pbVar3[1];
      bVar7 = bVar1 < pbVar6[1];
      if (bVar1 != pbVar6[1]) goto LAB_00407745;
      pbVar3 = pbVar3 + 2;
      pbVar6 = pbVar6 + 2;
    } while (bVar1 != 0);
    iVar4 = 0;
LAB_0040774a:
    if (iVar4 == 0) {
      pbVar5[0x300] = 0;
    }
    pbVar5 = pbVar5 + 0x80;
    if (0x51624b < (int)pbVar5) {
      crt_fclose(pFVar2);
      return;
    }
  } while( true );
}

