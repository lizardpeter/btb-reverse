/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00409210; function: ApplyGlobalBinkVolume; body bytes: 74
 * callers: 4; callees: 2; success: True
 */


void ApplyGlobalBinkVolume(void)

{
  int iVar1;
  int iVar2;
  longlong lVar3;
  
  iVar1 = DAT_004fc070;
  if (DAT_004fc070 == 0) {
    return;
  }
  if (DAT_004fbfe8 != 0) {
    return;
  }
  if (4 < DAT_00446ce0) {
    lVar3 = __ftol();
    iVar2 = (int)lVar3;
    if (0x24 < iVar2) goto LAB_0040924e;
  }
  iVar2 = 0x25;
LAB_0040924e:
  _BinkSetVolume_12(iVar1,0,iVar2);
  return;
}

