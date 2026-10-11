/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0043223b; function: FUN_0043223b; body bytes: 156
 * callers: 1; callees: 2; success: True
 */


undefined4 __cdecl
FUN_0043223b(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5,int param_6,int param_7)

{
  undefined4 uVar1;
  void *local_14;
  undefined1 *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_0043b620;
  puStack_10 = &LAB_00435f38;
  local_14 = ExceptionList;
  DAT_0051c420 = param_1;
  DAT_0051c424 = param_3;
  local_8 = 1;
  ExceptionList = &local_14;
  uVar1 = FUN_0042fcae(param_2,param_4,param_5,param_6,param_7);
  local_8 = 0xffffffff;
  FUN_00432301();
  ExceptionList = local_14;
  return uVar1;
}

