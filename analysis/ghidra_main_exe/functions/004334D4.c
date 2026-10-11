/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004334d4; function: FUN_004334d4; body bytes: 155
 * callers: 1; callees: 4; success: True
 */


void __cdecl FUN_004334d4(undefined4 param_1,char *param_2,size_t param_3,int param_4)

{
  int iVar1;
  char *pcVar2;
  char *pcVar3;
  
  DAT_0051c430 = (int *)FUN_00437ad7();
  DAT_0051c438 = DAT_0051c430[1] + -1;
  iVar1 = *DAT_0051c430;
  FUN_00437a60(param_2 + (iVar1 == 0x2d),param_3,(int)DAT_0051c430);
  DAT_0051c43c = DAT_0051c438 < DAT_0051c430[1] + -1;
  DAT_0051c438 = DAT_0051c430[1] + -1;
  if ((DAT_0051c438 < -4) || ((int)param_3 <= DAT_0051c438)) {
    FUN_0043356f(param_1,param_2,param_3,param_4);
  }
  else {
    pcVar2 = param_2 + (iVar1 == 0x2d);
    if ((bool)DAT_0051c43c) {
      do {
        pcVar3 = pcVar2;
        pcVar2 = pcVar3 + 1;
      } while (*pcVar3 != '\0');
      pcVar3[-1] = '\0';
    }
    FUN_00433596(param_1,param_2,param_3);
  }
  return;
}

