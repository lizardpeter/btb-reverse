/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00404530; function: CWaveFile_Destructor; body bytes: 41
 * callers: 2; callees: 2; success: True
 */


void __fastcall CWaveFile_Destructor(undefined4 *param_1)

{
  CWaveFile_Close((MMRESULT)param_1);
  if ((param_1[0x20] == 0) && ((undefined *)*param_1 != (undefined *)0x0)) {
    FUN_0042fbdc((undefined *)*param_1);
    *param_1 = 0;
  }
  return;
}

