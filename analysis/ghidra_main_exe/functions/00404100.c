/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00404100; function: CSound_FillBufferWithSound; body bytes: 360
 * callers: 2; callees: 3; success: True
 */


int __thiscall CSound_FillBufferWithSound(void *this,int *param_1)

{
  short sVar1;
  byte bVar2;
  int iVar3;
  uint uVar4;
  int unaff_EDI;
  undefined4 *puVar5;
  uint *puVar6;
  undefined4 *puStack_24;
  uint local_c;
  undefined4 local_8 [2];
  
  local_8[0] = 0;
  local_c = 0;
  local_8[1] = 0;
  if (param_1 == (int *)0x0) {
    return -0x7ffbfe10;
  }
  puStack_24 = (undefined4 *)0x404140;
  iVar3 = CSound_RestoreBuffer(param_1,(undefined4 *)0x0);
  if (iVar3 < 0) {
    return iVar3;
  }
  puStack_24 = (undefined4 *)0x0;
  iVar3 = (**(code **)(*param_1 + 0x2c))(param_1,0,*(undefined4 *)((int)this + 8));
  if (iVar3 < 0) {
    return iVar3;
  }
  CWaveFile_ResetFile(*(int *)((int)this + 0xc));
  iVar3 = CWaveFile_Read(*(void **)((int)this + 0xc),&local_c,(uint)local_8,(uint *)&puStack_24);
  if (iVar3 < 0) {
    return iVar3;
  }
  if (puStack_24 == (undefined4 *)0x0) {
    sVar1 = *(short *)(**(int **)((int)this + 0xc) + 0xe);
    puVar5 = local_8;
    puVar6 = &local_c;
  }
  else {
    if (local_8 <= puStack_24) goto LAB_00404248;
    puVar5 = puStack_24;
    if (unaff_EDI != 0) {
      do {
        iVar3 = CWaveFile_ResetFile(*(int *)((int)this + 0xc));
        if (iVar3 < 0) {
          return iVar3;
        }
        iVar3 = CWaveFile_Read(*(void **)((int)this + 0xc),
                               (undefined4 *)((int)puVar5 + (int)&local_c),
                               (int)local_8 - (int)puVar5,(uint *)&puStack_24);
        if (iVar3 < 0) {
          return iVar3;
        }
        puVar5 = (undefined4 *)((int)puVar5 + (int)puStack_24);
      } while (puVar5 < local_8);
      goto LAB_00404248;
    }
    puVar5 = (undefined4 *)((int)local_8 - (int)puStack_24);
    sVar1 = *(short *)(**(int **)((int)this + 0xc) + 0xe);
    puVar6 = (uint *)((int)puStack_24 + (int)&local_c);
  }
  bVar2 = (sVar1 != 8) - 1;
  for (uVar4 = (uint)puVar5 >> 2; uVar4 != 0; uVar4 = uVar4 - 1) {
    *puVar6 = CONCAT22(CONCAT11(bVar2,bVar2),CONCAT11(bVar2,bVar2)) & 0x80808080;
    puVar6 = puVar6 + 1;
  }
  for (uVar4 = (uint)puVar5 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
    *(byte *)puVar6 = bVar2 & 0x80;
    puVar6 = (uint *)((int)puVar6 + 1);
  }
LAB_00404248:
  (**(code **)(*param_1 + 0x4c))(param_1,&local_c,local_8,0,0);
  return 0;
}

