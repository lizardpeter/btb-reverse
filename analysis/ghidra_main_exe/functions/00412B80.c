/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00412b80; function: DeleteFireworkAtSelectedRegion; body bytes: 68
 * callers: 1; callees: 2; success: True
 */


void __cdecl DeleteFireworkAtSelectedRegion(int param_1)

{
  int local_4;
  
  if (param_1 == 0xc) {
    DecodeSelectedFireworkGridCell(&param_1,&local_4);
    if ((-1 < (int)(&DAT_0050a678)[local_4 + param_1 * 6]) &&
       ((int)(&DAT_0050a678)[local_4 + param_1 * 6] < 0xc)) {
      RemoveFireworkGridItem(param_1,local_4);
    }
  }
  return;
}

