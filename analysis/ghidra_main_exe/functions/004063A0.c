/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004063a0; function: MsvcStringAssignBuffer; body bytes: 156
 * callers: 5; callees: 3; success: True
 */


void * __thiscall MsvcStringAssignBuffer(void *this,undefined4 *param_1,uint param_2)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  undefined4 *puVar4;
  
  if (0xfffffffd < param_2) {
    FUN_00439ec9();
  }
  iVar2 = *(int *)((int)this + 4);
  if (((iVar2 == 0) || (cVar1 = *(char *)(iVar2 + -1), cVar1 == '\0')) || (cVar1 == -1)) {
    if (param_2 == 0) {
      MsvcStringTidy(this,'\x01');
      return this;
    }
    if ((*(uint *)((int)this + 0xc) < 0x20) && (param_2 <= *(uint *)((int)this + 0xc)))
    goto LAB_00406410;
    MsvcStringTidy(this,'\x01');
  }
  else if (param_2 == 0) {
    *(char *)(iVar2 + -1) = cVar1 + -1;
    *(undefined4 *)((int)this + 4) = 0;
    *(undefined4 *)((int)this + 8) = 0;
    *(undefined4 *)((int)this + 0xc) = 0;
    return this;
  }
  MsvcStringAllocateCopyBuffer(param_2);
LAB_00406410:
  puVar4 = *(undefined4 **)((int)this + 4);
  for (uVar3 = param_2 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
    *puVar4 = *param_1;
    param_1 = param_1 + 1;
    puVar4 = puVar4 + 1;
  }
  for (uVar3 = param_2 & 3; uVar3 != 0; uVar3 = uVar3 - 1) {
    *(undefined1 *)puVar4 = *(undefined1 *)param_1;
    param_1 = (undefined4 *)((int)param_1 + 1);
    puVar4 = (undefined4 *)((int)puVar4 + 1);
  }
  *(uint *)((int)this + 8) = param_2;
  *(undefined1 *)(param_2 + *(int *)((int)this + 4)) = 0;
  return this;
}

