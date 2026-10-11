/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004310e0; function: __amsg_exit; body bytes: 34
 * callers: 5; callees: 3; success: True
 */


/* Library Function - Single Match
    __amsg_exit
   
   Library: Visual Studio 2003 Release */

void __cdecl __amsg_exit(int param_1)

{
  if (DAT_0051c41c == 1) {
    FUN_00436010();
  }
  FUN_00436049((undefined *)param_1);
  (*(code *)PTR___exit_00447590)(0xff);
  return;
}

