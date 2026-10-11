/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00434005; function: FUN_00434005; body bytes: 120
 * callers: 1; callees: 1; success: True
 */


undefined4 * FUN_00434005(void)

{
  int iVar1;
  void *pvVar2;
  undefined4 *puVar3;
  int *piVar4;
  
  iVar1 = 0;
  piVar4 = DAT_0051c8e8;
  if (0 < DAT_0051d900) {
    do {
      if (*piVar4 == 0) {
        pvVar2 = _malloc(0x20);
        DAT_0051c8e8[iVar1] = (int)pvVar2;
        puVar3 = (undefined4 *)DAT_0051c8e8[iVar1];
        if (puVar3 == (undefined4 *)0x0) {
          return (undefined4 *)0x0;
        }
LAB_00434060:
        if (puVar3 == (undefined4 *)0x0) {
          return (undefined4 *)0x0;
        }
        puVar3[4] = 0xffffffff;
        puVar3[1] = 0;
        puVar3[3] = 0;
        puVar3[2] = 0;
        *puVar3 = 0;
        puVar3[7] = 0;
        return puVar3;
      }
      if ((*(byte *)(*piVar4 + 0xc) & 0x83) == 0) {
        puVar3 = (undefined4 *)DAT_0051c8e8[iVar1];
        goto LAB_00434060;
      }
      iVar1 = iVar1 + 1;
      piVar4 = piVar4 + 1;
    } while (iVar1 < DAT_0051d900);
  }
  return (undefined4 *)0x0;
}

