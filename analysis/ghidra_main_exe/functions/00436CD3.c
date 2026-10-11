/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00436cd3; function: FUN_00436cd3; body bytes: 409
 * callers: 1; callees: 5; success: True
 */


undefined4 __cdecl FUN_00436cd3(int param_1)

{
  BYTE *pBVar1;
  byte *pbVar2;
  byte bVar3;
  byte bVar4;
  UINT CodePage;
  UINT *pUVar5;
  BOOL BVar6;
  uint uVar7;
  BYTE *pBVar8;
  int iVar9;
  byte *pbVar10;
  int iVar11;
  byte *pbVar12;
  undefined4 *puVar13;
  _cpinfo local_1c;
  uint local_8;
  
  CodePage = FUN_00436e6c(param_1);
  if (CodePage == DAT_0051c6b8) {
    return 0;
  }
  if (CodePage != 0) {
    iVar11 = 0;
    pUVar5 = &DAT_00449e58;
    do {
      if (*pUVar5 == CodePage) {
        puVar13 = &DAT_0051c7e0;
        for (iVar9 = 0x40; iVar9 != 0; iVar9 = iVar9 + -1) {
          *puVar13 = 0;
          puVar13 = puVar13 + 1;
        }
        local_8 = 0;
        iVar11 = iVar11 * 0x30;
        *(undefined1 *)puVar13 = 0;
        pbVar12 = (byte *)(iVar11 + 0x449e68);
        do {
          bVar3 = *pbVar12;
          pbVar10 = pbVar12;
          while ((bVar3 != 0 && (bVar3 = pbVar10[1], bVar3 != 0))) {
            uVar7 = (uint)*pbVar10;
            if (uVar7 <= bVar3) {
              bVar4 = (&DAT_00449e50)[local_8];
              do {
                pbVar2 = (byte *)((int)&DAT_0051c7e0 + uVar7 + 1);
                *pbVar2 = *pbVar2 | bVar4;
                uVar7 = uVar7 + 1;
              } while (uVar7 <= bVar3);
            }
            pbVar10 = pbVar10 + 2;
            bVar3 = *pbVar10;
          }
          local_8 = local_8 + 1;
          pbVar12 = pbVar12 + 8;
        } while (local_8 < 4);
        DAT_0051c6cc = 1;
        DAT_0051c6b8 = CodePage;
        DAT_0051c8e4 = FUN_00436eb6(CodePage);
        DAT_0051c6c0 = *(undefined4 *)(iVar11 + 0x449e5c);
        DAT_0051c6c4 = *(undefined4 *)(iVar11 + 0x449e60);
        DAT_0051c6c8 = *(undefined4 *)(iVar11 + 0x449e64);
        goto LAB_00436e5b;
      }
      pUVar5 = pUVar5 + 0xc;
      iVar11 = iVar11 + 1;
    } while (pUVar5 < (UINT *)0x449f48);
    BVar6 = GetCPInfo(CodePage,&local_1c);
    if (BVar6 == 1) {
      puVar13 = &DAT_0051c7e0;
      DAT_0051c6b8 = CodePage;
      for (iVar11 = 0x40; iVar11 != 0; iVar11 = iVar11 + -1) {
        *puVar13 = 0;
        puVar13 = puVar13 + 1;
      }
      *(undefined1 *)puVar13 = 0;
      DAT_0051c8e4 = 0;
      if (local_1c.MaxCharSize < 2) {
        DAT_0051c6cc = 0;
      }
      else {
        if (local_1c.LeadByte[0] != '\0') {
          pBVar8 = local_1c.LeadByte + 1;
          do {
            bVar3 = *pBVar8;
            if (bVar3 == 0) break;
            for (uVar7 = (uint)pBVar8[-1]; uVar7 <= bVar3; uVar7 = uVar7 + 1) {
              pbVar12 = (byte *)((int)&DAT_0051c7e0 + uVar7 + 1);
              *pbVar12 = *pbVar12 | 4;
            }
            pBVar1 = pBVar8 + 1;
            pBVar8 = pBVar8 + 2;
          } while (*pBVar1 != 0);
        }
        uVar7 = 1;
        do {
          pbVar12 = (byte *)((int)&DAT_0051c7e0 + uVar7 + 1);
          *pbVar12 = *pbVar12 | 8;
          uVar7 = uVar7 + 1;
        } while (uVar7 < 0xff);
        DAT_0051c8e4 = FUN_00436eb6(CodePage);
        DAT_0051c6cc = 1;
      }
      DAT_0051c6c0 = 0;
      DAT_0051c6c4 = 0;
      DAT_0051c6c8 = 0;
      goto LAB_00436e5b;
    }
    if (DAT_0051c62c == 0) {
      return 0xffffffff;
    }
  }
  FUN_00436ee9();
LAB_00436e5b:
  FUN_00436f12();
  return 0;
}

