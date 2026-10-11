/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00428e30; function: DrawGenericUIScreen; body bytes: 150
 * callers: 1; callees: 0; success: True
 */


undefined4 DrawGenericUIScreen(void)

{
  int *piVar1;
  int iVar2;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;
  
  piVar1 = *(int **)(DAT_0044de08 + 0xc);
  if (DAT_0051c300 != 0) {
    (**(code **)(*piVar1 + 0x1c))(piVar1,0,0,DAT_0051c298,0,0);
    (**(code **)(*piVar1 + 0x1c))(piVar1,0,0,DAT_0051c268,0,1);
    return 1;
  }
  local_10 = 0;
  local_c = 0;
  local_8 = 0x280;
  local_4 = 0x1e0;
  do {
    iVar2 = (**(code **)(*piVar1 + 0x1c))(piVar1,0,0,DAT_0051c268,&local_10,0);
    if (iVar2 == 0) {
      return 1;
    }
  } while (iVar2 == -0x7789fde4);
  return 1;
}

