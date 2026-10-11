/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004382fb; function: FUN_004382fb; body bytes: 177
 * callers: 0; callees: 5; success: True
 */


int * __cdecl FUN_004382fb(int param_1,int param_2)

{
  uint *_Size;
  int iVar1;
  uint *puVar2;
  uint *puVar3;
  int *piVar4;
  
  puVar2 = (uint *)(param_1 * param_2);
  puVar3 = puVar2;
  if (puVar2 < (uint *)0xffffffe1) {
    if (puVar2 == (uint *)0x0) {
      puVar3 = (uint *)0x1;
    }
    puVar3 = (uint *)((int)puVar3 + 0xfU & 0xfffffff0);
  }
  do {
    piVar4 = (int *)0x0;
    if (puVar3 < (uint *)0xffffffe1) {
      if (DAT_0051da44 == 3) {
        if (puVar2 <= DAT_0051da3c) {
          piVar4 = FUN_00434798(puVar2);
          _Size = puVar2;
joined_r0x00438364:
          if (piVar4 != (int *)0x0) {
            _memset(piVar4,0,(size_t)_Size);
            return piVar4;
          }
        }
      }
      else if ((DAT_0051da44 == 2) && (puVar3 <= DAT_0044976c)) {
        piVar4 = FUN_0043523b((uint)puVar3 >> 4);
        _Size = puVar3;
        goto joined_r0x00438364;
      }
      piVar4 = (int *)HeapAlloc(DAT_0051da40,8,(SIZE_T)puVar3);
      if (piVar4 != (int *)0x0) {
        return piVar4;
      }
    }
    if (DAT_0051c448 == 0) {
      return piVar4;
    }
    iVar1 = FUN_00435610(puVar3);
    if (iVar1 == 0) {
      return (int *)0x0;
    }
  } while( true );
}

