/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004333f6; function: FUN_004333f6; body bytes: 222
 * callers: 2; callees: 4; success: True
 */


char * __cdecl FUN_004333f6(undefined4 param_1,char *param_2,size_t param_3)

{
  int *piVar1;
  int iVar2;
  char *pcVar3;
  
  piVar1 = DAT_0051c430;
  if (DAT_0051c434 == '\0') {
    piVar1 = (int *)FUN_00437ad7();
    FUN_00437a60(param_2 + (*piVar1 == 0x2d),piVar1[1] + param_3,(int)piVar1);
  }
  else if (DAT_0051c438 == param_3) {
    iVar2 = (*DAT_0051c430 == 0x2d) + DAT_0051c438;
    param_2[iVar2] = '0';
    (param_2 + iVar2)[1] = '\0';
  }
  pcVar3 = param_2;
  if (*piVar1 == 0x2d) {
    *param_2 = '-';
    pcVar3 = param_2 + 1;
  }
  if (piVar1[1] < 1) {
    FUN_0043360a(pcVar3,1);
    *pcVar3 = '0';
    pcVar3 = pcVar3 + 1;
  }
  else {
    pcVar3 = pcVar3 + piVar1[1];
  }
  if (0 < (int)param_3) {
    FUN_0043360a(pcVar3,1);
    *pcVar3 = DAT_00449b14;
    iVar2 = piVar1[1];
    if (iVar2 < 0) {
      if ((DAT_0051c434 != '\0') || (-iVar2 <= (int)param_3)) {
        param_3 = -iVar2;
      }
      FUN_0043360a(pcVar3 + 1,param_3);
      _memset(pcVar3 + 1,0x30,param_3);
    }
  }
  return param_2;
}

