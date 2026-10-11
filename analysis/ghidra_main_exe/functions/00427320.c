/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00427320; function: UpdateSquirrelConveyorSlideCycle; body bytes: 203
 * callers: 1; callees: 1; success: True
 */


void UpdateSquirrelConveyorSlideCycle(void)

{
  uint uVar1;
  int iVar2;
  bool bVar3;
  
  iVar2 = 5;
  if (DAT_0044de0c != 0) {
    iVar2 = 0x32;
    DAT_0051511c = DAT_0051511c + 1;
    uVar1 = DAT_0051511c & 0x80000001;
    bVar3 = uVar1 == 0;
    if ((int)uVar1 < 0) {
      bVar3 = (uVar1 - 1 | 0xfffffffe) == 0xffffffff;
    }
    if (!bVar3) {
      return;
    }
  }
  if (DAT_005150e8 != 0) {
    DAT_005150f8 = DAT_005150f8 + -1;
    if (DAT_005150f8 < 1) {
      DAT_005150f8 = 2;
      DAT_005150f4 = DAT_005150f4 + 1;
      if (2 < DAT_005150f4) {
        DAT_005150f4 = 0;
      }
    }
    if (DAT_005150e8 == 1) {
      DAT_005150ec = DAT_005150ec + iVar2;
      if (500 < DAT_005150ec) {
        DAT_005150ec = 0xfffffe5c;
        GenerateSquirrelConveyorChoices();
        DAT_005150e8 = 2;
        return;
      }
    }
    else if ((DAT_005150e8 == 2) && (DAT_005150ec = DAT_005150ec + iVar2, -1 < DAT_005150ec)) {
      DAT_005150ec = 0;
      DAT_005150e8 = 0;
    }
  }
  return;
}

