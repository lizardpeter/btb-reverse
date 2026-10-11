/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00403d20; function: CSoundManager_Create; body bytes: 713
 * callers: 7; callees: 7; success: True
 */


int __thiscall
CSoundManager_Create
          (void *this,undefined4 *param_1,LPSTR param_2,undefined4 param_3,undefined4 param_4,
          undefined4 param_5,undefined4 param_6,undefined4 param_7,uint param_8)

{
  char cVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined4 uVar5;
  void *this_00;
  uint uVar6;
  uint uVar7;
  char *pcVar8;
  int iVar9;
  char *pcVar10;
  char *pcVar11;
  undefined4 *puVar12;
  uint uStack_a4;
  int local_a0;
  undefined4 local_94 [4];
  undefined4 local_84;
  undefined4 local_80;
  undefined4 local_7c;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c [24];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_0043a39c;
  local_c = ExceptionList;
                    /* WARNING: Load size is inaccurate */
  local_a0 = 0;
  if (*this == 0) {
    return -0x7ffbfe10;
  }
  if (((param_2 == (LPSTR)0x0) || (param_1 == (undefined4 *)0x0)) || (param_8 == 0)) {
    return -0x7ff8ffa9;
  }
  ExceptionList = &local_c;
  puVar2 = (undefined4 *)operator_new(param_8 << 2);
  if (puVar2 == (undefined4 *)0x0) {
    ExceptionList = local_c;
    return -0x7ff8fff2;
  }
  puVar3 = (undefined4 *)operator_new(0x90);
  local_4 = 0;
  if (puVar3 == (undefined4 *)0x0) {
    puVar3 = (undefined4 *)0x0;
  }
  else {
    puVar3 = (undefined4 *)CWaveFile_Constructor(puVar3);
  }
  local_4 = 0xffffffff;
  if (puVar3 == (undefined4 *)0x0) {
    FUN_0042fbdc((undefined *)puVar2);
    ExceptionList = local_c;
    return -0x7ff8fff2;
  }
  CWaveFile_Open(puVar3,param_2,(short *)0x0,1);
  iVar4 = CWaveFile_GetSize((int)puVar3);
  if (iVar4 == 0) {
    local_70 = DAT_0043e14c;
    puVar12 = local_6c;
    for (iVar4 = 0x18; iVar4 != 0; iVar4 = iVar4 + -1) {
      *puVar12 = 0;
      puVar12 = puVar12 + 1;
    }
    if (DAT_0043e874 != -1) {
      local_70 = CONCAT31((int3)((uint)DAT_0043e14c >> 8),(char)DAT_0043e874 + 'a');
      uVar6 = 0xffffffff;
      do {
        pcVar8 = param_2;
        if (uVar6 == 0) break;
        uVar6 = uVar6 - 1;
        pcVar8 = param_2 + 1;
        cVar1 = *param_2;
        param_2 = pcVar8;
      } while (cVar1 != '\0');
      uVar6 = ~uVar6;
      iVar4 = -1;
      pcVar11 = (char *)&local_70;
      do {
        pcVar10 = pcVar11;
        if (iVar4 == 0) break;
        iVar4 = iVar4 + -1;
        pcVar10 = pcVar11 + 1;
        cVar1 = *pcVar11;
        pcVar11 = pcVar10;
      } while (cVar1 != '\0');
      pcVar8 = pcVar8 + -uVar6;
      pcVar11 = pcVar10 + -1;
      for (uVar7 = uVar6 >> 2; uVar7 != 0; uVar7 = uVar7 - 1) {
        *(undefined4 *)pcVar11 = *(undefined4 *)pcVar8;
        pcVar8 = pcVar8 + 4;
        pcVar11 = pcVar11 + 4;
      }
      for (uVar6 = uVar6 & 3; uVar6 != 0; uVar6 = uVar6 - 1) {
        *pcVar11 = *pcVar8;
        pcVar8 = pcVar8 + 1;
        pcVar11 = pcVar11 + 1;
      }
      CWaveFile_Open(puVar3,(LPSTR)&local_70,(short *)0x0,1);
      iVar4 = CWaveFile_GetSize((int)puVar3);
      if (iVar4 == 0) {
        iVar9 = -0x7fffbffb;
        goto LAB_00403e79;
      }
    }
  }
  uVar5 = CWaveFile_GetSize((int)puVar3);
  puVar12 = local_94;
  for (iVar4 = 9; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar12 = 0;
    puVar12 = puVar12 + 1;
  }
  local_80 = param_4;
  local_78 = param_6;
  local_94[0] = 0x24;
  local_94[1] = 0x80;
  local_7c = param_5;
                    /* WARNING: Load size is inaccurate */
  local_74 = param_7;
  local_84 = *puVar3;
  local_94[2] = uVar5;
  iVar9 = (**(code **)(**this + 0xc))(*this,local_94,puVar2,0);
  iVar4 = iVar9;
  if ((iVar9 == 0x878000a) || (iVar4 = local_a0, -1 < iVar9)) {
    local_a0 = iVar4;
    uStack_a4 = 1;
    puVar12 = puVar2;
    if (1 < param_8) {
      do {
                    /* WARNING: Load size is inaccurate */
        iVar9 = (**(code **)(**this + 0x14))(*this,*puVar2,puVar12 + 1);
        if (iVar9 < 0) goto LAB_00403e79;
        uStack_a4 = uStack_a4 + 1;
        puVar12 = puVar12 + 1;
      } while (uStack_a4 < param_8);
    }
    this_00 = operator_new(0x14);
    local_4 = 1;
    if (this_00 == (void *)0x0) {
      puVar3 = (undefined4 *)0x0;
    }
    else {
      puVar3 = CSound_Constructor(this_00,(int)puVar2,uVar5,param_8,puVar3);
    }
    local_4 = 0xffffffff;
    *param_1 = puVar3;
    FUN_0042fbdc((undefined *)puVar2);
    ExceptionList = local_c;
    return local_a0;
  }
LAB_00403e79:
  CWaveFile_Destructor(puVar3);
  FUN_0042fbdc((undefined *)puVar3);
  FUN_0042fbdc((undefined *)puVar2);
  ExceptionList = local_c;
  return iVar9;
}

