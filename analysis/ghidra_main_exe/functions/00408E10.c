/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00408e10; function: OpenBinkMovieWithFallback; body bytes: 141
 * callers: 3; callees: 2; success: True
 */


int __cdecl OpenBinkMovieWithFallback(char *param_1)

{
  char cVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  char *pcVar6;
  undefined4 *puVar7;
  char *pcVar8;
  char *pcVar9;
  
  iVar2 = _BinkOpen_8(param_1,0x4000000);
  if (iVar2 == 0) {
    puVar7 = (undefined4 *)&stack0xffffff98;
    for (iVar3 = 0x18; iVar3 != 0; iVar3 = iVar3 + -1) {
      *puVar7 = 0;
      puVar7 = puVar7 + 1;
    }
    if (DAT_0043e874 != -1) {
      uVar4 = 0xffffffff;
      do {
        pcVar6 = param_1;
        if (uVar4 == 0) break;
        uVar4 = uVar4 - 1;
        pcVar6 = param_1 + 1;
        cVar1 = *param_1;
        param_1 = pcVar6;
      } while (cVar1 != '\0');
      uVar4 = ~uVar4;
      iVar2 = -1;
      pcVar9 = &stack0xffffff94;
      do {
        pcVar8 = pcVar9;
        if (iVar2 == 0) break;
        iVar2 = iVar2 + -1;
        pcVar8 = pcVar9 + 1;
        cVar1 = *pcVar9;
        pcVar9 = pcVar8;
      } while (cVar1 != '\0');
      pcVar6 = pcVar6 + -uVar4;
      pcVar9 = pcVar8 + -1;
      for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
        *(undefined4 *)pcVar9 = *(undefined4 *)pcVar6;
        pcVar6 = pcVar6 + 4;
        pcVar9 = pcVar9 + 4;
      }
      for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
        *pcVar9 = *pcVar6;
        pcVar6 = pcVar6 + 1;
        pcVar9 = pcVar9 + 1;
      }
      iVar2 = _BinkOpen_8(&stack0xffffff94,0x4000000);
    }
  }
  ApplyGlobalBinkVolume();
  return iVar2;
}

