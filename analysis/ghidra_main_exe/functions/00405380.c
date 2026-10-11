/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00405380; function: BitmapPrinterSetScaleMode; body bytes: 256
 * callers: 1; callees: 4; success: True
 */


void __thiscall BitmapPrinterSetScaleMode(void *this,undefined1 *param_1)

{
  char cVar1;
  undefined4 uVar2;
  uint uVar3;
  uint uVar4;
  char *pcVar5;
  char *pcVar6;
  undefined1 local_38 [4];
  char *local_34;
  uint local_30;
  undefined4 local_2c;
  undefined **local_28 [3];
  undefined1 local_1c [4];
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_0043a440;
  local_c = ExceptionList;
  if ((-1 < (int)param_1) && ((int)param_1 < 4)) {
    *(undefined1 **)((int)this + 0x120) = param_1;
    return;
  }
  local_38[0] = param_1._0_1_;
  uVar3 = 0xffffffff;
  pcVar5 = s_Invalid_scale__0043e1d8;
  do {
    if (uVar3 == 0) break;
    uVar3 = uVar3 - 1;
    cVar1 = *pcVar5;
    pcVar5 = pcVar5 + 1;
  } while (cVar1 != '\0');
  uVar3 = ~uVar3 - 1;
  local_34 = (char *)0x0;
  local_30 = 0;
  local_2c = 0;
  ExceptionList = &local_c;
  uVar2 = MsvcStringGrow(local_38,uVar3,'\x01');
  if ((char)uVar2 != '\0') {
    pcVar5 = s_Invalid_scale__0043e1d8;
    pcVar6 = local_34;
    for (uVar4 = uVar3 >> 2; uVar4 != 0; uVar4 = uVar4 - 1) {
      *(undefined4 *)pcVar6 = *(undefined4 *)pcVar5;
      pcVar5 = pcVar5 + 4;
      pcVar6 = pcVar6 + 4;
    }
    for (uVar4 = uVar3 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
      *pcVar6 = *pcVar5;
      pcVar5 = pcVar5 + 1;
      pcVar6 = pcVar6 + 1;
    }
    local_34[uVar3] = '\0';
    local_30 = uVar3;
  }
  local_4 = 0;
  param_1 = &DAT_00482574;
  FUN_004303ba(local_28,&param_1);
  local_4._0_1_ = 1;
  local_1c[0] = local_38[0];
  local_18 = 0;
  local_14 = 0;
  local_10 = 0;
  MsvcStringAssignSubstring(local_1c,local_38,0,DAT_0043b334);
  local_28[0] = &PTR_FUN_0043b328;
  local_4 = (uint)local_4._1_3_ << 8;
                    /* WARNING: Subroutine does not return */
  __CxxThrowException_8(local_28,&DAT_0043bdd0);
}

