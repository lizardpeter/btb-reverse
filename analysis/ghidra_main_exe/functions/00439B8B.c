/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00439b8b; function: FUN_00439b8b; body bytes: 672
 * callers: 1; callees: 14; success: True
 */


int * __thiscall FUN_00439b8b(void *this,int *param_1,uint *param_2)

{
  int *piVar1;
  uint *puVar2;
  int iVar3;
  uint *puVar4;
  byte *pbVar5;
  uint *puVar6;
  void *local_8;
  
  local_8 = this;
  if (param_1 == (int *)0x0) {
    piVar1 = (int *)_malloc((size_t)param_2);
  }
  else {
    if (param_2 == (uint *)0x0) {
      FUN_00430d2a((undefined *)param_1);
    }
    else {
      puVar6 = param_2;
      if (DAT_0051da44 == 3) {
        do {
          piVar1 = (int *)0x0;
          if (puVar6 < (uint *)0xffffffe1) {
            puVar2 = (uint *)FUN_00434444((int)param_1);
            if (puVar2 == (uint *)0x0) {
LAB_00439c82:
              if (puVar6 == (uint *)0x0) {
                puVar6 = (uint *)0x1;
              }
              puVar6 = (uint *)((int)puVar6 + 0xfU & 0xfffffff0);
              piVar1 = (int *)HeapReAlloc(DAT_0051da40,0,param_1,(SIZE_T)puVar6);
            }
            else {
              if (DAT_0051da3c < puVar6) {
LAB_00439c3b:
                if (puVar6 == (uint *)0x0) {
                  puVar6 = (uint *)0x1;
                }
                puVar6 = (uint *)((int)puVar6 + 0xfU & 0xfffffff0);
                piVar1 = (int *)HeapAlloc(DAT_0051da40,0,(SIZE_T)puVar6);
                if (piVar1 != (int *)0x0) {
                  puVar4 = (uint *)(param_1[-1] - 1U);
                  if (puVar6 <= (uint *)(param_1[-1] - 1U)) {
                    puVar4 = puVar6;
                  }
                  FUN_00433b60(piVar1,param_1,(uint)puVar4);
                  FUN_0043446f(puVar2,(int)param_1);
                }
              }
              else {
                iVar3 = FUN_00434c4d(puVar2,(int)param_1,(int)puVar6);
                piVar1 = param_1;
                if (iVar3 == 0) {
                  piVar1 = FUN_00434798(puVar6);
                  if (piVar1 == (int *)0x0) goto LAB_00439c3b;
                  puVar2 = (uint *)(param_1[-1] - 1U);
                  if (puVar6 <= (uint *)(param_1[-1] - 1U)) {
                    puVar2 = puVar6;
                  }
                  FUN_00433b60(piVar1,param_1,(uint)puVar2);
                  puVar2 = (uint *)FUN_00434444((int)param_1);
                  FUN_0043446f(puVar2,(int)param_1);
                }
                if (piVar1 == (int *)0x0) goto LAB_00439c3b;
              }
              if (puVar2 == (uint *)0x0) goto LAB_00439c82;
            }
            if (piVar1 != (int *)0x0) {
              return piVar1;
            }
          }
          if (DAT_0051c448 == 0) {
            return piVar1;
          }
          iVar3 = FUN_00435610(puVar6);
        } while (iVar3 != 0);
      }
      else if (DAT_0051da44 == 2) {
        if (param_2 < (uint *)0xffffffe1) {
          if (param_2 == (uint *)0x0) {
            puVar6 = (uint *)0x10;
          }
          else {
            puVar6 = (uint *)((int)param_2 + 0xfU & 0xfffffff0);
          }
        }
        do {
          piVar1 = (int *)0x0;
          if (puVar6 < (uint *)0xffffffe1) {
            pbVar5 = (byte *)FUN_0043519f((undefined *)param_1,&local_8,(uint *)&param_2);
            if (pbVar5 == (byte *)0x0) {
              piVar1 = (int *)HeapReAlloc(DAT_0051da40,0,param_1,(SIZE_T)puVar6);
            }
            else {
              if (puVar6 < DAT_0044976c) {
                iVar3 = FUN_00435567((int)local_8,(int *)param_2,pbVar5,(uint)puVar6 >> 4);
                piVar1 = param_1;
                if (iVar3 == 0) {
                  piVar1 = FUN_0043523b((uint)puVar6 >> 4);
                  if (piVar1 == (int *)0x0) goto LAB_00439d70;
                  puVar2 = (uint *)((uint)*pbVar5 << 4);
                  if (puVar6 <= (uint *)((uint)*pbVar5 << 4)) {
                    puVar2 = puVar6;
                  }
                  FUN_00433b60(piVar1,param_1,(uint)puVar2);
                  FUN_004351f6((int)local_8,(int)param_2,pbVar5);
                }
                if (piVar1 != (int *)0x0) {
                  return piVar1;
                }
              }
LAB_00439d70:
              piVar1 = (int *)HeapAlloc(DAT_0051da40,0,(SIZE_T)puVar6);
              if (piVar1 == (int *)0x0) goto LAB_00439dc8;
              puVar2 = (uint *)((uint)*pbVar5 << 4);
              if (puVar6 <= (uint *)((uint)*pbVar5 << 4)) {
                puVar2 = puVar6;
              }
              FUN_00433b60(piVar1,param_1,(uint)puVar2);
              FUN_004351f6((int)local_8,(int)param_2,pbVar5);
            }
            if (piVar1 != (int *)0x0) {
              return piVar1;
            }
          }
LAB_00439dc8:
          if (DAT_0051c448 == 0) {
            return piVar1;
          }
          iVar3 = FUN_00435610(puVar6);
        } while (iVar3 != 0);
      }
      else {
        do {
          piVar1 = (int *)0x0;
          if (puVar6 < (uint *)0xffffffe1) {
            if (puVar6 == (uint *)0x0) {
              puVar6 = (uint *)0x1;
            }
            puVar6 = (uint *)((int)puVar6 + 0xfU & 0xfffffff0);
            piVar1 = (int *)HeapReAlloc(DAT_0051da40,0,param_1,(SIZE_T)puVar6);
            if (piVar1 != (int *)0x0) {
              return piVar1;
            }
          }
          if (DAT_0051c448 == 0) {
            return piVar1;
          }
          iVar3 = FUN_00435610(puVar6);
        } while (iVar3 != 0);
      }
    }
    piVar1 = (int *)0x0;
  }
  return piVar1;
}

