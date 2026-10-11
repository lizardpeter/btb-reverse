/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041f4b0; function: PlaceGrandOpeningEvent; body bytes: 147
 * callers: 1; callees: 2; success: True
 */


char __cdecl PlaceGrandOpeningEvent(int param_1,int param_2,uint param_3)

{
  int iVar1;
  char cVar2;
  int iVar3;
  uint uVar4;
  undefined4 *puVar5;
  
  cVar2 = CanPlaceGrandOpeningEvent(param_1,param_2,param_3);
  if (cVar2 == '\0') {
    return '\0';
  }
  iVar3 = (&DAT_00444be8)[param_3];
  iVar1 = param_2 + param_1 * 0x18;
  (&DAT_005123b8)[iVar1] = param_3;
  if (1 < iVar3) {
    puVar5 = &DAT_005123bc + iVar1;
    while (iVar3 = iVar3 + -1, iVar3 != 0) {
      *puVar5 = 10;
      puVar5 = puVar5 + 1;
    }
  }
  CSound_Play(*(void **)(&DAT_004fc124 + (param_3 + param_1 * -10) * 4),0,0);
  if ((&DAT_00512378)[(int)param_3 / 2] == 0) {
    uVar4 = param_3 & 0x80000001;
    if ((int)uVar4 < 0) {
      uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
    }
    (&DAT_00512378)[(int)param_3 / 2] = uVar4 + 1;
  }
  return '\x01';
}

