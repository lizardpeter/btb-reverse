/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00428ed0; function: UnloadGenericUIScreenResources; body bytes: 628
 * callers: 1; callees: 2; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void UnloadGenericUIScreenResources(void)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  DAT_00446cec = 0xffffffff;
  if (DAT_0051c27c != 0xc) {
    iVar6 = 0;
    iVar4 = DAT_0051c27c;
    if (0 < (int)(&DAT_0048fb40)[DAT_0051c27c]) {
      do {
        iVar5 = 0;
        iVar2 = iVar4 * 0x20 + iVar6;
        if (0 < *(int *)(&DAT_00494714 + iVar2 * 0x40c)) {
          iVar2 = iVar2 * 0x103;
          do {
            if (*(int *)(&DAT_00494838 + (iVar2 + iVar5) * 4) != 0) {
              _DAT_0051c278 = _DAT_0051c278 + 1;
            }
            UnregisterBitmapSurface((int)(&DAT_00494838 + (iVar2 + iVar5) * 4));
            iVar3 = DAT_0051c27c * 0x20 + iVar6;
            iVar2 = iVar3 * 0x103;
            piVar1 = *(int **)(&DAT_00494838 + (iVar2 + iVar5) * 4);
            iVar4 = DAT_0051c27c;
            if (piVar1 != (int *)0x0) {
              (**(code **)(*piVar1 + 8))(piVar1);
              iVar4 = DAT_0051c27c;
              iVar3 = DAT_0051c27c * 0x20 + iVar6;
              iVar2 = iVar3 * 0x103;
              *(undefined4 *)(&DAT_00494838 + (iVar2 + iVar5) * 4) = 0;
            }
            iVar5 = iVar5 + 1;
          } while (iVar5 < *(int *)(&DAT_00494714 + iVar3 * 0x40c));
        }
        iVar5 = 0;
        iVar2 = iVar4 * 0x20 + iVar6;
        if (0 < *(int *)(&DAT_004947a0 + iVar2 * 0x40c)) {
          iVar2 = iVar2 * 0x103;
          do {
            UnregisterBitmapSurface((int)(&DAT_004948d8 + (iVar2 + iVar5) * 4));
            iVar3 = DAT_0051c27c * 0x20 + iVar6;
            iVar2 = iVar3 * 0x103;
            piVar1 = *(int **)(&DAT_004948d8 + (iVar2 + iVar5) * 4);
            iVar4 = DAT_0051c27c;
            if (piVar1 != (int *)0x0) {
              (**(code **)(*piVar1 + 8))(piVar1);
              iVar4 = DAT_0051c27c;
              iVar3 = DAT_0051c27c * 0x20 + iVar6;
              iVar2 = iVar3 * 0x103;
              *(undefined4 *)(&DAT_004948d8 + (iVar2 + iVar5) * 4) = 0;
            }
            iVar5 = iVar5 + 1;
          } while (iVar5 < *(int *)(&DAT_004947a0 + iVar3 * 0x40c));
        }
        iVar2 = (iVar4 * 0x20 + iVar6) * 0x40c;
        if (*(int *)(&DAT_00494834 + iVar2) != 0) {
          UnregisterBitmapSurface((int)(&DAT_00494834 + iVar2));
          iVar2 = (DAT_0051c27c * 0x20 + iVar6) * 0x40c;
          piVar1 = *(int **)(&DAT_00494834 + iVar2);
          iVar4 = DAT_0051c27c;
          if (piVar1 != (int *)0x0) {
            (**(code **)(*piVar1 + 8))(piVar1);
            iVar4 = DAT_0051c27c;
            iVar2 = (DAT_0051c27c * 0x20 + iVar6) * 0x40c;
            *(undefined4 *)(&DAT_00494834 + iVar2) = 0;
          }
        }
        if (*(int *)(&DAT_00494978 + iVar2) != 0) {
          UnregisterBitmapSurface((int)(&DAT_00494978 + iVar2));
          piVar1 = *(int **)(&DAT_00494978 + (DAT_0051c27c * 0x20 + iVar6) * 0x40c);
          iVar4 = DAT_0051c27c;
          if (piVar1 != (int *)0x0) {
            (**(code **)(*piVar1 + 8))(piVar1);
            iVar4 = DAT_0051c27c;
            *(undefined4 *)(&DAT_00494978 + (DAT_0051c27c * 0x20 + iVar6) * 0x40c) = 0;
          }
        }
        iVar6 = iVar6 + 1;
      } while (iVar6 < (int)(&DAT_0048fb40)[iVar4]);
    }
    StopAllManagedSounds(DAT_0044ddd8);
  }
  return;
}

