/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004392de; function: __mbsnbicoll; body bytes: 63
 * callers: 2; callees: 1; success: True
 */


/* Library Function - Single Match
    __mbsnbicoll
   
   Library: Visual Studio 2003 Release */

int __cdecl __mbsnbicoll(uchar *_Str1,uchar *_Str2,size_t _MaxCount)

{
  int iVar1;
  
  if (_MaxCount == 0) {
    return 0;
  }
  iVar1 = FUN_0043969d(DAT_0051c8e4,1,_Str1,_MaxCount,_Str2,_MaxCount,DAT_0051c6b8);
  if (iVar1 == 0) {
    return 0x7fffffff;
  }
  return iVar1 + -2;
}

