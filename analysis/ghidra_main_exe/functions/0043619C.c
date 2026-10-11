/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0043619c; function: FUN_0043619c; body bytes: 149
 * callers: 1; callees: 1; success: True
 */


int FUN_0043619c(void)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar4 = -1;
  iVar6 = 0;
  iVar5 = 0;
  piVar3 = &DAT_0051d920;
  do {
    puVar2 = (undefined4 *)*piVar3;
    if (puVar2 == (undefined4 *)0x0) {
      puVar2 = (undefined4 *)_malloc(0x100);
      if (puVar2 != (undefined4 *)0x0) {
        DAT_0051da20 = DAT_0051da20 + 0x20;
        (&DAT_0051d920)[iVar6] = puVar2;
        puVar1 = puVar2;
        for (; puVar2 < puVar1 + 0x40; puVar2 = puVar2 + 2) {
          *(undefined1 *)(puVar2 + 1) = 0;
          *puVar2 = 0xffffffff;
          *(undefined1 *)((int)puVar2 + 5) = 10;
          puVar1 = (undefined4 *)(&DAT_0051d920)[iVar6];
        }
        iVar4 = iVar6 << 5;
      }
      return iVar4;
    }
    puVar1 = puVar2 + 0x40;
    for (; puVar2 < puVar1; puVar2 = puVar2 + 2) {
      if ((*(byte *)(puVar2 + 1) & 1) == 0) {
        *puVar2 = 0xffffffff;
        iVar4 = ((int)puVar2 - *piVar3 >> 3) + iVar5;
        if (iVar4 != -1) {
          return iVar4;
        }
        break;
      }
    }
    piVar3 = piVar3 + 1;
    iVar6 = iVar6 + 1;
    iVar5 = iVar5 + 0x20;
    if (0x51da1f < (int)piVar3) {
      return iVar4;
    }
  } while( true );
}

