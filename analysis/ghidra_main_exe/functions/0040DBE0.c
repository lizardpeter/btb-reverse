/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040dbe0; function: RefreshParkDesignerObjectGeometry; body bytes: 171
 * callers: 1; callees: 0; success: True
 */


void __cdecl RefreshParkDesignerObjectGeometry(int param_1)

{
  int iVar1;
  undefined1 local_7c [124];
  
  iVar1 = (&DAT_004fcacc)[param_1 * 0x13];
  if (99 < iVar1) {
    iVar1 = 0;
  }
  (**(code **)(*(int *)(&DAT_00507e40)[iVar1] + 0x58))((int *)(&DAT_00507e40)[iVar1],local_7c);
  (**(code **)(*(int *)(&DAT_00507e40)[iVar1] + 0x58))
            ((int *)(&DAT_00507e40)[iVar1],&stack0xffffff7c);
  (&DAT_004fcabc)[param_1 * 0x13] = (&DAT_004fcab4)[param_1 * 0x13] + 0x7c;
  (&DAT_004fcab8)[param_1 * 0x13] = (&DAT_004fcab0)[param_1 * 0x13] + 6;
  (&DAT_004fcad4)[param_1 * 0x13] = 1;
  (&DAT_004fcad8)[param_1 * 0x13] = 6;
  (&DAT_004fcadc)[param_1 * 0x13] = 0x7c;
  (&DAT_004fcae0)[param_1 * 0x13] = (&DAT_0043fac4)[iVar1];
  (&DAT_004fcae4)[param_1 * 0x13] = param_1;
  return;
}

