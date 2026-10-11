/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00406440; function: MsvcStringSetEnd; body bytes: 17
 * callers: 2; callees: 0; success: True
 */


void __thiscall MsvcStringSetEnd(void *this,int param_1)

{
  *(int *)((int)this + 8) = param_1;
  *(undefined1 *)(*(int *)((int)this + 4) + param_1) = 0;
  return;
}

