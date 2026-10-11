/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00435a04; function: FUN_00435a04; body bytes: 153
 * callers: 1; callees: 5; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00435a04(void)

{
  undefined4 *puVar1;
  byte *pbVar2;
  int local_c;
  int local_8;
  
  if (DAT_0051da50 == 0) {
    FUN_00437097();
  }
  GetModuleFileNameA((HMODULE)0x0,&DAT_0051c45c,0x104);
  _DAT_0051c400 = &DAT_0051c45c;
  pbVar2 = &DAT_0051c45c;
  if (*DAT_0051da48 != 0) {
    pbVar2 = DAT_0051da48;
  }
  FUN_00435a9d(pbVar2,(undefined4 *)0x0,(byte *)0x0,&local_8,&local_c);
  puVar1 = (undefined4 *)_malloc(local_c + local_8 * 4);
  if (puVar1 == (undefined4 *)0x0) {
    __amsg_exit(8);
  }
  FUN_00435a9d(pbVar2,puVar1,(byte *)(puVar1 + local_8),&local_8,&local_c);
  _DAT_0051c3e8 = puVar1;
  _DAT_0051c3e4 = local_8 + -1;
  return;
}

