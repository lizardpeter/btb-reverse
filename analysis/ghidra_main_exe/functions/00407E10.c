/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00407e10; function: UpdateDirectInputAcquireState; body bytes: 62
 * callers: 2; callees: 0; success: True
 */


undefined4 UpdateDirectInputAcquireState(void)

{
  if (DAT_004fbf94 == (int *)0x0) {
    return 1;
  }
  if (DAT_0044de10 != 0) {
    (**(code **)(*DAT_004fbf94 + 0x1c))();
    (**(code **)(*DAT_004fbf98 + 0x1c))(DAT_004fbf98);
    return 0;
  }
  (**(code **)(*DAT_004fbf94 + 0x20))(DAT_004fbf94);
  (**(code **)(*DAT_004fbf98 + 0x20))(DAT_004fbf98);
  return 0;
}

