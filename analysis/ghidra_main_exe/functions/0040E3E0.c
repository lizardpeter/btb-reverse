/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040e3e0; function: FinishParkDesignerObjectDrag; body bytes: 84
 * callers: 1; callees: 1; success: True
 */


void FinishParkDesignerObjectDrag(void)

{
  if ((DAT_00507b14 == 2) &&
     (((DAT_00507b58 < 0xc9 || (299 < DAT_00507b58)) || ((&DAT_004fcae8)[DAT_00507b58 * 0x13] != 1))
     )) {
    (&DAT_004fcad4)[DAT_00507b58 * 0x13] = 1;
  }
  DAT_00507b14 = 0;
  SetCursorSurface((int *)0x0);
  return;
}

