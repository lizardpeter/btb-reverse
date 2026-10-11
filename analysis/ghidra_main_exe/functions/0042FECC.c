/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0042fecc; function: __global_unwind2; body bytes: 32
 * callers: 0; callees: 1; success: True
 */


/* Library Function - Single Match
    __global_unwind2
   
   Library: Visual Studio */

void __cdecl __global_unwind2(PVOID param_1)

{
  RtlUnwind(param_1,(PVOID)0x42fee4,(PEXCEPTION_RECORD)0x0,(PVOID)0x0);
  return;
}

