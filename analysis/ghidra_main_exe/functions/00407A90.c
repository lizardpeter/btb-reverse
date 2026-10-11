/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00407a90; function: ClearCursorHotspotAndClampNonnegative; body bytes: 41
 * callers: 1; callees: 0; success: True
 */


void ClearCursorHotspotAndClampNonnegative(void)

{
  DAT_004fbd48 = 0;
  DAT_004fbd4c = 0;
  if (DAT_004fbd24 < 0) {
    DAT_004fbd24 = 0;
  }
  if (DAT_004fbd30 < 0) {
    DAT_004fbd30 = 0;
  }
  return;
}

