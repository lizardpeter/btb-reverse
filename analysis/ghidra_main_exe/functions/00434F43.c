/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00434f43; function: FUN_00434f43; body bytes: 324
 * callers: 2; callees: 5; success: True
 */


undefined ** FUN_00434f43(void)

{
  bool bVar1;
  int *lpAddress;
  LPVOID pvVar2;
  undefined **ppuVar3;
  int iVar4;
  undefined **lpMem;
  
  if (DAT_00447758 == -1) {
    lpMem = &PTR_LOOP_00447748;
  }
  else {
    lpMem = (undefined **)HeapAlloc(DAT_0051da40,0,0x2020);
    if (lpMem == (undefined **)0x0) {
      return (undefined **)0x0;
    }
  }
  lpAddress = (int *)VirtualAlloc((LPVOID)0x0,0x400000,0x2000,4);
  if (lpAddress != (int *)0x0) {
    pvVar2 = VirtualAlloc(lpAddress,0x10000,0x1000,4);
    if (pvVar2 != (LPVOID)0x0) {
      if (lpMem == &PTR_LOOP_00447748) {
        if (PTR_LOOP_00447748 == (undefined *)0x0) {
          PTR_LOOP_00447748 = (undefined *)&PTR_LOOP_00447748;
        }
        if (PTR_PTR_LOOP_0044774c == (undefined *)0x0) {
          PTR_PTR_LOOP_0044774c = (undefined *)&PTR_LOOP_00447748;
        }
      }
      else {
        *lpMem = (undefined *)&PTR_LOOP_00447748;
        lpMem[1] = PTR_PTR_LOOP_0044774c;
        PTR_PTR_LOOP_0044774c = (undefined *)lpMem;
        *(undefined ***)lpMem[1] = lpMem;
      }
      lpMem[5] = (undefined *)(lpAddress + 0x100000);
      ppuVar3 = lpMem + 6;
      lpMem[3] = (undefined *)(lpMem + 0x26);
      lpMem[4] = (undefined *)lpAddress;
      lpMem[2] = (undefined *)ppuVar3;
      iVar4 = 0;
      do {
        bVar1 = 0xf < iVar4;
        iVar4 = iVar4 + 1;
        *ppuVar3 = (undefined *)((bVar1 - 1 & 0xf1) - 1);
        ppuVar3[1] = (undefined *)0xf1;
        ppuVar3 = ppuVar3 + 2;
      } while (iVar4 < 0x400);
      _memset(lpAddress,0,0x10000);
      for (; lpAddress < lpMem[4] + 0x10000; lpAddress = lpAddress + 0x400) {
        *(undefined1 *)(lpAddress + 0x3e) = 0xff;
        *lpAddress = (int)(lpAddress + 2);
        lpAddress[1] = 0xf0;
      }
      return lpMem;
    }
    VirtualFree(lpAddress,0,0x8000);
  }
  if (lpMem != &PTR_LOOP_00447748) {
    HeapFree(DAT_0051da40,0,lpMem);
  }
  return (undefined **)0x0;
}

