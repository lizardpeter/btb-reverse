/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00406460; function: MsvcStringGrow; body bytes: 282
 * callers: 9; callees: 3; success: True
 */


int __thiscall MsvcStringGrow(void *this,uint param_1,char param_2)

{
  char cVar1;
  uint3 uVar4;
  undefined4 uVar2;
  undefined1 *puVar3;
  
  if (0xfffffffd < param_1) {
    FUN_00439ec9();
  }
  puVar3 = *(undefined1 **)((int)this + 4);
  uVar4 = (uint3)((uint)puVar3 >> 8);
  if (((puVar3 == (undefined1 *)0x0) || (cVar1 = puVar3[-1], cVar1 == '\0')) || (cVar1 == -1)) {
    if (param_1 == 0) {
      if (param_2 == '\0') {
        if (puVar3 != (undefined1 *)0x0) {
          *(undefined4 *)((int)this + 8) = 0;
          *puVar3 = 0;
        }
        return (uint)uVar4 << 8;
      }
      if (puVar3 != (undefined1 *)0x0) {
        cVar1 = puVar3[-1];
        if ((cVar1 != '\0') && (cVar1 != -1)) {
          puVar3[-1] = cVar1 + -1;
          *(undefined4 *)((int)this + 4) = 0;
          *(undefined4 *)((int)this + 8) = 0;
          *(undefined4 *)((int)this + 0xc) = 0;
          return (uint)uVar4 << 8;
        }
        puVar3 = (undefined1 *)FUN_0042fbdc(puVar3 + -1);
      }
      *(undefined4 *)((int)this + 4) = 0;
      *(undefined4 *)((int)this + 8) = 0;
      *(undefined4 *)((int)this + 0xc) = 0;
      return (uint)puVar3 & 0xffffff00;
    }
    if (param_2 != '\0') {
      if ((0x1f < *(uint *)((int)this + 0xc)) || (*(uint *)((int)this + 0xc) < param_1)) {
        if (puVar3 != (undefined1 *)0x0) {
          cVar1 = puVar3[-1];
          if ((cVar1 != '\0') && (cVar1 != -1)) {
            puVar3[-1] = cVar1 + -1;
            *(undefined4 *)((int)this + 4) = 0;
            *(undefined4 *)((int)this + 8) = 0;
            *(undefined4 *)((int)this + 0xc) = 0;
            uVar2 = MsvcStringAllocateCopyBuffer(param_1);
            return CONCAT31((int3)((uint)uVar2 >> 8),1);
          }
          FUN_0042fbdc(puVar3 + -1);
        }
        *(undefined4 *)((int)this + 4) = 0;
        *(undefined4 *)((int)this + 8) = 0;
        *(undefined4 *)((int)this + 0xc) = 0;
        uVar2 = MsvcStringAllocateCopyBuffer(param_1);
        return CONCAT31((int3)((uint)uVar2 >> 8),1);
      }
      goto LAB_00406572;
    }
    if (param_1 <= *(uint *)((int)this + 0xc)) goto LAB_00406572;
  }
  else if (param_1 == 0) {
    puVar3[-1] = cVar1 + -1;
    *(undefined4 *)((int)this + 4) = 0;
    *(undefined4 *)((int)this + 8) = 0;
    *(undefined4 *)((int)this + 0xc) = 0;
    return (uint)uVar4 << 8;
  }
  puVar3 = (undefined1 *)MsvcStringAllocateCopyBuffer(param_1);
LAB_00406572:
  return CONCAT31((int3)((uint)puVar3 >> 8),1);
}

