/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00404e30; function: BitmapPrinterSetBitmap; body bytes: 341
 * callers: 0; callees: 5; success: True
 */


void __thiscall BitmapPrinterSetBitmap(void *this,undefined1 *param_1,undefined4 param_2)

{
  char cVar1;
  undefined1 *puVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  uint uVar6;
  char *pcVar7;
  char *pcVar8;
  undefined1 auStack_50 [4];
  char *pcStack_4c;
  uint uStack_48;
  undefined4 uStack_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  int local_2c;
  undefined **appuStack_28 [3];
  undefined1 auStack_1c [4];
  undefined4 uStack_18;
  void *pvStack_14;
  void *pvStack_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  int iStack_4;
  
  puVar2 = param_1;
  iStack_4 = 0xffffffff;
  puStack_8 = &LAB_0043a3c0;
  pvStack_c = ExceptionList;
  local_3c = 0;
  local_38 = 0;
  local_34 = 0;
  local_30 = 0;
  local_40 = 0;
  local_2c = 0;
  ExceptionList = &pvStack_c;
  iVar3 = GetObjectA(param_1,0x18,&local_40);
  if (iVar3 == 0x18) {
    if (local_2c != 0) {
                    /* WARNING: Load size is inaccurate */
      (**(code **)(*this + 0x30))(puVar2);
      ExceptionList = pvStack_10;
      return;
    }
                    /* WARNING: Load size is inaccurate */
    (**(code **)(*this + 0x2c))(puVar2,param_2);
    ExceptionList = pvStack_14;
    return;
  }
  uVar5 = 0xffffffff;
  pcVar7 = s_Invalid_bitmap__0043e1a0;
  do {
    if (uVar5 == 0) break;
    uVar5 = uVar5 - 1;
    cVar1 = *pcVar7;
    pcVar7 = pcVar7 + 1;
  } while (cVar1 != '\0');
  uVar5 = ~uVar5 - 1;
  auStack_50[0] = param_1._0_1_;
  pcStack_4c = (char *)0x0;
  uStack_48 = 0;
  uStack_44 = 0;
  uVar4 = MsvcStringGrow(auStack_50,uVar5,'\x01');
  if ((char)uVar4 != '\0') {
    pcVar7 = s_Invalid_bitmap__0043e1a0;
    pcVar8 = pcStack_4c;
    for (uVar6 = uVar5 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
      *(undefined4 *)pcVar8 = *(undefined4 *)pcVar7;
      pcVar7 = pcVar7 + 4;
      pcVar8 = pcVar8 + 4;
    }
    for (uVar6 = uVar5 & 3; uVar6 != 0; uVar6 = uVar6 - 1) {
      *pcVar8 = *pcVar7;
      pcVar7 = pcVar7 + 1;
      pcVar8 = pcVar8 + 1;
    }
    pcStack_4c[uVar5] = '\0';
    uStack_48 = uVar5;
  }
  iStack_4 = 0;
  param_1 = &DAT_00482574;
  FUN_004303ba(appuStack_28,&param_1);
  iStack_4._0_1_ = 1;
  auStack_1c[0] = auStack_50[0];
  uStack_18 = 0;
  pvStack_14 = (void *)0x0;
  pvStack_10 = (void *)0x0;
  MsvcStringAssignSubstring(auStack_1c,auStack_50,0,DAT_0043b334);
  appuStack_28[0] = &PTR_FUN_0043b328;
  iStack_4 = (uint)iStack_4._1_3_ << 8;
                    /* WARNING: Subroutine does not return */
  __CxxThrowException_8(appuStack_28,&DAT_0043bdd0);
}

