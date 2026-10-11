/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0043854d; function: FUN_0043854d; body bytes: 301
 * callers: 1; callees: 2; success: True
 */


undefined4 __cdecl FUN_0043854d(int param_1)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  int iVar4;
  int iVar5;
  code *pcVar6;
  
  iVar2 = param_1;
  if (param_1 == 2) {
    puVar3 = &DAT_0051c69c;
    pcVar6 = DAT_0051c69c;
  }
  else if (((param_1 == 4) || (param_1 == 8)) || (param_1 == 0xb)) {
    puVar3 = FUN_0043867a(param_1);
    pcVar6 = (code *)puVar3[2];
    puVar3 = puVar3 + 2;
  }
  else if (param_1 == 0xf) {
    puVar3 = &DAT_0051c6a8;
    pcVar6 = DAT_0051c6a8;
  }
  else if (param_1 == 0x15) {
    puVar3 = &DAT_0051c6a0;
    pcVar6 = DAT_0051c6a0;
  }
  else {
    if (param_1 != 0x16) {
      return 0xffffffff;
    }
    puVar3 = &DAT_0051c6a4;
    pcVar6 = DAT_0051c6a4;
  }
  iVar1 = DAT_0051c458;
  iVar4 = DAT_004497f4;
  if (pcVar6 == (code *)0x1) {
    return 0;
  }
  if (pcVar6 == (code *)0x0) {
                    /* WARNING: Subroutine does not return */
    __exit(3);
  }
  if (((param_1 == 8) || (param_1 == 0xb)) || (iVar5 = param_1, param_1 == 4)) {
    DAT_0051c458 = 0;
    iVar5 = iVar1;
    if (param_1 == 8) {
      DAT_004497f4 = 0x8c;
      param_1 = iVar4;
      goto LAB_00438611;
    }
LAB_0043863d:
    *puVar3 = 0;
    if (iVar2 != 8) {
      (*pcVar6)(iVar2);
      if ((iVar2 != 0xb) && (iVar2 != 4)) {
        return 0;
      }
      goto LAB_00438660;
    }
  }
  else {
LAB_00438611:
    if (iVar2 != 8) goto LAB_0043863d;
    if (DAT_004497e8 < DAT_004497ec + DAT_004497e8) {
      iVar4 = (DAT_004497ec + DAT_004497e8) - DAT_004497e8;
      puVar3 = (undefined4 *)(DAT_004497e8 * 0xc + 0x449778);
      do {
        *puVar3 = 0;
        puVar3 = puVar3 + 3;
        iVar4 = iVar4 + -1;
      } while (iVar4 != 0);
    }
  }
  (*pcVar6)(8,DAT_004497f4);
LAB_00438660:
  if (iVar2 == 8) {
    DAT_004497f4 = param_1;
  }
  DAT_0051c458 = iVar5;
  return 0;
}

