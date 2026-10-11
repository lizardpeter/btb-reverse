/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00404070; function: CSound_ScalarDeletingDestructor; body bytes: 30
 * callers: 0; callees: 2; success: True
 */


undefined4 * __thiscall CSound_ScalarDeletingDestructor(void *this,byte param_1)

{
  CSound_Destructor((undefined4 *)this);
  if ((param_1 & 1) != 0) {
    FUN_0042fbdc((undefined *)this);
  }
  return (undefined4 *)this;
}

