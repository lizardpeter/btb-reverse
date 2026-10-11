/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004090d0; function: UpdateGlobalBinkMovie; body bytes: 308
 * callers: 4; callees: 6; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 UpdateGlobalBinkMovie(void)

{
  int iVar1;
  undefined4 uVar2;
  
  _BinkDoFrame_4(DAT_004fc070);
  iVar1 = (**(code **)(*DAT_004fbfec + 100))(DAT_004fbfec,0,&DAT_004fbff0,1,0);
  if (iVar1 == 0) {
    uVar2 = _BinkDDSurfaceType_4(DAT_004fbfec);
    _BinkCopyToBuffer_28
              (DAT_004fc070,DAT_004fc014,DAT_004fc000,*(undefined4 *)(DAT_004fc070 + 4),0,0,uVar2);
    (**(code **)(*DAT_004fbfec + 0x80))(DAT_004fbfec,0);
    if ((*(int *)(DAT_004fc070 + 0xc) != *(int *)(DAT_004fc070 + 8)) &&
       ((DAT_0043ee6c != 1 || (((DAT_004fbe54 == 0 && (DAT_004fbd50 == 0)) && (_DAT_004fbe5c == 0)))
        ))) {
      _BinkNextFrame_4(DAT_004fc070);
      iVar1 = _BinkWait_4(DAT_004fc070);
      while (iVar1 != 0) {
        iVar1 = _BinkWait_4(DAT_004fc070);
      }
      (**(code **)(**(int **)(DAT_0044de08 + 0xc) + 0x1c))
                (*(int **)(DAT_0044de08 + 0xc),0,0,DAT_004fbfec,0,0);
      if (DAT_004fc07c != 0) {
        if (DAT_004fc074 < *(uint *)(DAT_004fc070 + 0x32c)) {
          DAT_004fc074 = *(uint *)(DAT_004fc070 + 0x32c);
        }
        if (*(int *)(DAT_004fc070 + 0x2fc) != 0) {
          _DAT_004fc078 = _DAT_004fc078 + 1;
        }
      }
      return 0;
    }
    CloseGlobalBinkMovie();
  }
  return 1;
}

