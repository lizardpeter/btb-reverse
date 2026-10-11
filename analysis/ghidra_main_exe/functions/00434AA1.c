/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00434aa1; function: FUN_00434aa1; body bytes: 177
 * callers: 1; callees: 4; success: True
 */


undefined4 * FUN_00434aa1(void)

{
  undefined4 *puVar1;
  LPVOID pvVar2;
  
  if (DAT_0051da34 == DAT_0051da24) {
    pvVar2 = HeapReAlloc(DAT_0051da40,0,DAT_0051da38,(DAT_0051da24 * 5 + 0x50) * 4);
    if (pvVar2 == (LPVOID)0x0) {
      return (undefined4 *)0x0;
    }
    DAT_0051da24 = DAT_0051da24 + 0x10;
    DAT_0051da38 = pvVar2;
  }
  puVar1 = (undefined4 *)((int)DAT_0051da38 + DAT_0051da34 * 0x14);
  pvVar2 = HeapAlloc(DAT_0051da40,8,0x41c4);
  puVar1[4] = pvVar2;
  if (pvVar2 != (LPVOID)0x0) {
    pvVar2 = VirtualAlloc((LPVOID)0x0,0x100000,0x2000,4);
    puVar1[3] = pvVar2;
    if (pvVar2 != (LPVOID)0x0) {
      puVar1[2] = 0xffffffff;
      *puVar1 = 0;
      puVar1[1] = 0;
      DAT_0051da34 = DAT_0051da34 + 1;
      *(undefined4 *)puVar1[4] = 0xffffffff;
      return puVar1;
    }
    HeapFree(DAT_0051da40,0,(LPVOID)puVar1[4]);
  }
  return (undefined4 *)0x0;
}

