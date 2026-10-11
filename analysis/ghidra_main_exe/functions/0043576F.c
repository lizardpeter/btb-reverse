/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0043576f; function: FUN_0043576f; body bytes: 321
 * callers: 1; callees: 2; success: True
 */


LONG __cdecl FUN_0043576f(int param_1,_EXCEPTION_POINTERS *param_2)

{
  code *pcVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int *piVar4;
  LONG LVar5;
  int iVar6;
  undefined4 *puVar7;
  
  piVar4 = FUN_004358b0(param_1);
  uVar3 = DAT_0051c458;
  if ((piVar4 == (int *)0x0) || (pcVar1 = (code *)piVar4[2], pcVar1 == (code *)0x0)) {
    LVar5 = UnhandledExceptionFilter(param_2);
  }
  else if (pcVar1 == (code *)0x5) {
    piVar4[2] = 0;
    LVar5 = 1;
  }
  else {
    if (pcVar1 != (code *)0x1) {
      DAT_0051c458 = param_2;
      if (piVar4[1] == 8) {
        if (DAT_004497e8 < DAT_004497ec + DAT_004497e8) {
          iVar6 = (DAT_004497ec + DAT_004497e8) - DAT_004497e8;
          puVar7 = (undefined4 *)(DAT_004497e8 * 0xc + 0x449778);
          do {
            *puVar7 = 0;
            puVar7 = puVar7 + 3;
            iVar6 = iVar6 + -1;
          } while (iVar6 != 0);
        }
        uVar2 = DAT_004497f4;
        iVar6 = *piVar4;
        if (iVar6 == -0x3fffff72) {
          DAT_004497f4 = 0x83;
        }
        else if (iVar6 == -0x3fffff70) {
          DAT_004497f4 = 0x81;
        }
        else if (iVar6 == -0x3fffff6f) {
          DAT_004497f4 = 0x84;
        }
        else if (iVar6 == -0x3fffff6d) {
          DAT_004497f4 = 0x85;
        }
        else if (iVar6 == -0x3fffff73) {
          DAT_004497f4 = 0x82;
        }
        else if (iVar6 == -0x3fffff71) {
          DAT_004497f4 = 0x86;
        }
        else if (iVar6 == -0x3fffff6e) {
          DAT_004497f4 = 0x8a;
        }
        (*pcVar1)(8,DAT_004497f4);
        DAT_004497f4 = uVar2;
      }
      else {
        piVar4[2] = 0;
        (*pcVar1)(piVar4[1]);
      }
    }
    LVar5 = -1;
    DAT_0051c458 = (_EXCEPTION_POINTERS *)uVar3;
  }
  return LVar5;
}

