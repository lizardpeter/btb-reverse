/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00408eb0; function: OpenGlobalBinkMovie; body bytes: 159
 * callers: 4; callees: 4; success: True
 */


void __cdecl OpenGlobalBinkMovie(char *param_1)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  char *pcVar5;
  undefined4 *puVar6;
  char *pcVar7;
  char *pcVar8;
  
  DAT_004fc070 = _BinkOpen_8(param_1,0);
  if (DAT_004fc070 == 0) {
    puVar6 = (undefined4 *)&stack0xffffff98;
    for (iVar2 = 0x18; iVar2 != 0; iVar2 = iVar2 + -1) {
      *puVar6 = 0;
      puVar6 = puVar6 + 1;
    }
    iVar2 = 0;
    if (DAT_0043e874 != -1) {
      uVar3 = 0xffffffff;
      do {
        pcVar5 = param_1;
        if (uVar3 == 0) break;
        uVar3 = uVar3 - 1;
        pcVar5 = param_1 + 1;
        cVar1 = *param_1;
        param_1 = pcVar5;
      } while (cVar1 != '\0');
      uVar3 = ~uVar3;
      iVar2 = -1;
      pcVar8 = &stack0xffffff94;
      do {
        pcVar7 = pcVar8;
        if (iVar2 == 0) break;
        iVar2 = iVar2 + -1;
        pcVar7 = pcVar8 + 1;
        cVar1 = *pcVar8;
        pcVar8 = pcVar7;
      } while (cVar1 != '\0');
      pcVar5 = pcVar5 + -uVar3;
      pcVar8 = pcVar7 + -1;
      for (uVar4 = uVar3 >> 2; uVar4 != 0; uVar4 = uVar4 - 1) {
        *(undefined4 *)pcVar8 = *(undefined4 *)pcVar5;
        pcVar5 = pcVar5 + 4;
        pcVar8 = pcVar8 + 4;
      }
      for (uVar3 = uVar3 & 3; uVar3 != 0; uVar3 = uVar3 - 1) {
        *pcVar8 = *pcVar5;
        pcVar5 = pcVar5 + 1;
        pcVar8 = pcVar8 + 1;
      }
      DAT_004fc070 = _BinkOpen_8(&stack0xffffff94,0);
      iVar2 = DAT_004fc070;
      if (DAT_004fc070 != 0) goto LAB_00408f3e;
    }
    DAT_004fc070 = iVar2;
    AbortForRemovedRetailCD();
    return;
  }
LAB_00408f3e:
  ApplyGlobalBinkVolume();
  DisableInputProcessing();
  return;
}

