/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0043874a; function: FUN_0043874a; body bytes: 125
 * callers: 1; callees: 3; success: True
 */


int __cdecl FUN_0043874a(uchar *param_1)

{
  int iVar1;
  size_t _MaxCount;
  size_t sVar2;
  int *piVar3;
  
  if (((DAT_0051da4c != 0) &&
      ((DAT_0051c3f0 != (int *)0x0 ||
       (((DAT_0051c3f8 != 0 && (iVar1 = FUN_0043931d(), iVar1 == 0)) && (DAT_0051c3f0 != (int *)0x0)
        ))))) && (piVar3 = DAT_0051c3f0, param_1 != (uchar *)0x0)) {
    _MaxCount = _strlen((char *)param_1);
    for (; (char *)*piVar3 != (char *)0x0; piVar3 = piVar3 + 1) {
      sVar2 = _strlen((char *)*piVar3);
      if (((_MaxCount < sVar2) && (((uchar *)*piVar3)[_MaxCount] == '=')) &&
         (iVar1 = __mbsnbicoll((uchar *)*piVar3,param_1,_MaxCount), iVar1 == 0)) {
        return *piVar3 + 1 + _MaxCount;
      }
    }
  }
  return 0;
}

