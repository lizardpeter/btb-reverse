/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004036e0; function: MarkRegisteredSurfaceColorKeyed; body bytes: 49
 * callers: 23; callees: 0; success: True
 */


void __cdecl MarkRegisteredSurfaceColorKeyed(int param_1)

{
  int iVar1;
  int *piVar2;
  
  iVar1 = 0;
  if (0 < DAT_0048241c) {
    piVar2 = &DAT_0044eb1c;
    while (*piVar2 != param_1) {
      iVar1 = iVar1 + 1;
      piVar2 = piVar2 + 1;
      if (DAT_0048241c <= iVar1) {
        return;
      }
    }
    (&DAT_0044de9c)[iVar1] = 1;
  }
  return;
}

