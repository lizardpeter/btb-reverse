/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00430441; function: ~exception; body bytes: 22
 * callers: 24; callees: 1; success: True
 */


/* Library Function - Single Match
    public: virtual __thiscall exception::~exception(void)
   
   Libraries: Visual Studio 2003 Release, Visual Studio 2005 Release */

void __thiscall exception::~exception(exception *this)

{
  *(undefined ***)this = &PTR_FUN_0043b5bc;
  if (*(int *)(this + 8) != 0) {
    FUN_0042fbdc(*(undefined **)(this + 4));
  }
  return;
}

