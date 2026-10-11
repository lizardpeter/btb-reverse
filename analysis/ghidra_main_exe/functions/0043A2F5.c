/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0043a2f5; function: FUN_0043a2f5; body bytes: 97
 * callers: 0; callees: 3; success: True
 */


SIZE_T __cdecl FUN_0043a2f5(undefined *param_1)

{
  uint uVar1;
  byte *pbVar2;
  SIZE_T SVar3;
  undefined4 local_c;
  uint local_8;
  
  if (DAT_0051da44 == 3) {
    uVar1 = FUN_00434444((int)param_1);
    if (uVar1 != 0) {
      return *(int *)(param_1 + -4) - 9;
    }
  }
  else if ((DAT_0051da44 == 2) &&
          (pbVar2 = (byte *)FUN_0043519f(param_1,&local_c,&local_8), pbVar2 != (byte *)0x0)) {
    return (uint)*pbVar2 << 4;
  }
  SVar3 = HeapSize(DAT_0051da40,0,param_1);
  return SVar3;
}

