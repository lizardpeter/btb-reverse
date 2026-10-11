/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00430d2a; function: FUN_00430d2a; body bytes: 105
 * callers: 15; callees: 5; success: True
 */


void __cdecl FUN_00430d2a(undefined *param_1)

{
  undefined *lpMem;
  uint *puVar1;
  byte *pbVar2;
  int local_8;
  
  lpMem = param_1;
  if (param_1 != (undefined *)0x0) {
    if (DAT_0051da44 == 3) {
      puVar1 = (uint *)FUN_00434444((int)param_1);
      if (puVar1 != (uint *)0x0) {
        FUN_0043446f(puVar1,(int)lpMem);
        return;
      }
    }
    else if ((DAT_0051da44 == 2) &&
            (pbVar2 = (byte *)FUN_0043519f(param_1,&local_8,(uint *)&param_1), pbVar2 != (byte *)0x0
            )) {
      FUN_004351f6(local_8,(int)param_1,pbVar2);
      return;
    }
    HeapFree(DAT_0051da40,0,lpMem);
  }
  return;
}

