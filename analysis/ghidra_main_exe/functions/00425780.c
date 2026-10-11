/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00425780; function: UpdateAndDrawSquirrelLevelTransition; body bytes: 511
 * callers: 1; callees: 0; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall UpdateAndDrawSquirrelLevelTransition(int param_1)

{
  int *piVar1;
  int iVar2;
  
  piVar1 = *(int **)(DAT_0044de08 + 0xc);
  if (DAT_005150d8 == 1) {
    if (DAT_00446a44 != 0) {
      _DAT_00515104 = 0;
      DAT_00515108 = 0;
      DAT_005150d8 = 0;
      DAT_00515004 = 0;
      DAT_00514fb8 = 0;
      DAT_005150d0 = DAT_005150d0 + 1;
      DAT_00514f94 = 0x2e;
      DAT_00514fb4 = 1;
      DAT_005150e8 = 1;
      return;
    }
    DAT_005150d8 = 2;
    DAT_005150f0 = 0;
    DAT_00515108 = 1;
    DAT_0051510c = DAT_00514f40;
  }
  else if (DAT_00515108 < 1) goto LAB_004258f0;
  DAT_00515100 = DAT_00515100 + 1;
  DAT_00514f48 = DAT_00514f48 + DAT_00446a48 / 5;
  DAT_00514f40 = DAT_00514f40 + DAT_00446a48 / 5;
  param_1 = ((DAT_004467d4 - DAT_004467bc) - DAT_00514f40) + DAT_0051510c;
  if ((DAT_004467d4 - DAT_004467bc) + DAT_0051510c <= DAT_00514f40) {
    DAT_005150d0 = DAT_005150d0 + 1;
    _DAT_00515104 = 0;
    iVar2 = (int)((ulonglong)((longlong)DAT_005150f0 * -0x66666667) >> 0x20);
    DAT_00515108 = 0;
    DAT_005150d8 = 0;
    DAT_00515004 = 0;
    DAT_00514f94 = DAT_00514f94 + ((iVar2 >> 1) - (iVar2 >> 0x1f));
    DAT_00514fb8 = 0;
    DAT_00514fb4 = 1;
    DAT_005150e8 = 1;
  }
  (**(code **)(*piVar1 + 0x1c))(piVar1,10,0,DAT_0051509c,&DAT_00514f40,0);
LAB_004258f0:
  if (DAT_00446a48 < 10) {
    DAT_00446a48 = DAT_00446a48 + 1;
  }
  if (param_1 < DAT_00446a48) {
    DAT_00446a48 = param_1;
  }
  if (DAT_00446a48 < 10) {
    DAT_00446a48 = 10;
  }
  DAT_005150f0 = DAT_005150f0 + DAT_00446a48;
  (**(code **)(*piVar1 + 0x1c))(piVar1,0,0,DAT_00515068,0,1);
  (**(code **)(*piVar1 + 0x1c))(piVar1,0,0x14,DAT_0051506c,0,1);
  (**(code **)(*piVar1 + 0x1c))(piVar1,0x26c,0x14,DAT_00515070,0,1);
  (**(code **)(*piVar1 + 0x1c))(piVar1,0x14,400,DAT_00515074,0,1);
  return;
}

