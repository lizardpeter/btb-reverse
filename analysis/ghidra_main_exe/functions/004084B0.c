/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004084b0; function: PollDirectInputAndUpdateState; body bytes: 2255
 * callers: 2; callees: 6; success: True
 */


/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void PollDirectInputAndUpdateState(void)

{
  int *piVar1;
  short sVar2;
  int iVar3;
  uint uVar4;
  UINT UVar5;
  bool bVar6;
  DWORD aDStack_28 [7];
  int iStack_c;
  byte bStack_4;
  byte bStack_3;
  
  aDStack_28[0] = timeGetTime();
  aDStack_28[1] = 0;
  _DAT_004fbe58 = (float)aDStack_28[0] * _DAT_0043b360;
  if (DAT_004fbf94 == (int *)0x0) goto LAB_00408a00;
  iVar3 = (**(code **)(*DAT_004fbf94 + 100))(DAT_004fbf94);
  if (iVar3 < 0) {
    return;
  }
  iVar3 = (**(code **)(*DAT_004fbf98 + 0x24))(DAT_004fbf98,0x100,&DAT_004fbe70);
  if (DAT_0043ece4 != 0) {
    iVar3 = (**(code **)(*DAT_004fbf94 + 0x24))(DAT_004fbf94,0x10,aDStack_28 + 6);
  }
  if (DAT_004fbd50 == 0) {
    if (DAT_004fbfd0 != 0) goto LAB_00408550;
  }
  else if (DAT_004fbfd0 == 0) {
    DAT_004fbfd0 = 1;
LAB_00408550:
    DAT_004fbfd4 = DAT_004fbfd4 + 1;
    uVar4 = DAT_004fbfd4 & 0x80000001;
    bVar6 = uVar4 == 0;
    if ((int)uVar4 < 0) {
      bVar6 = (uVar4 - 1 | 0xfffffffe) == 0xffffffff;
    }
    if (bVar6) {
      bStack_4 = bStack_4 | 0x80;
    }
  }
  else {
    DAT_004fbfd0 = 0;
  }
  if ((iVar3 == -0x7ff8ffe2) &&
     (iVar3 = (**(code **)(*DAT_004fbf94 + 0x1c))(DAT_004fbf94), -1 < iVar3)) {
    return;
  }
  UVar5 = 0;
  do {
    if ((((((&DAT_004fbe70)[UVar5] & 0x80) != 0) && (UVar5 != 0x2a)) && (UVar5 != 0x36)) &&
       (iVar3 = FUN_00408460(UVar5,(LPWORD)aDStack_28), 0 < iVar3)) break;
    UVar5 = UVar5 + 1;
  } while ((int)UVar5 < 0xff);
  sVar2 = (short)aDStack_28[0];
  if ((DAT_004fbe60 == sVar2) || (sVar2 == -0x3334)) {
    DAT_004fbe60 = -1;
    DAT_004fbd44 = 0xffffffff;
  }
  else {
    DAT_004fbd44 = aDStack_28[0] & 0xff;
    DAT_004fbe60 = sVar2;
  }
  _DAT_004fbe5c = (uint)((DAT_004fbf3b & 0x80) != 0);
  if ((DAT_004fbf3d & 0x80) != 0) {
    _DAT_004fbe5c = _DAT_004fbe5c | 2;
  }
  if ((DAT_004fbf38 & 0x80) != 0) {
    _DAT_004fbe5c = _DAT_004fbe5c | 4;
  }
  if ((DAT_004fbf40 & 0x80) != 0) {
    _DAT_004fbe5c = _DAT_004fbe5c | 8;
  }
  if ((DAT_004fbea9 & 0x80) != 0) {
    _DAT_004fbe5c = _DAT_004fbe5c | 0x10;
  }
  if (DAT_0043ece4 == 0) goto LAB_00408a00;
  if (iStack_c < 0) {
    _DAT_004fbfd8 = _DAT_004fbe58 + _DAT_0043b35c;
    _DAT_004fbfdc = 0.0;
  }
  if (0 < iStack_c) {
    _DAT_004fbfdc = _DAT_004fbe58 + _DAT_0043b35c;
    _DAT_004fbfd8 = 0.0;
  }
  if (DAT_004fbfc4 != 0) {
    DrawLoadingCursorAnimation();
    return;
  }
  _DAT_004fbfbc = 0;
  if ((DAT_0044de14 == 1) && (DAT_0051c2f4 == 2)) {
    if (((DAT_00446f28 < DAT_004fbd24) && (DAT_004fbd24 < DAT_00446f30)) &&
       (iVar3 = DAT_00446f34, DAT_00446f2c < DAT_004fbd30)) {
joined_r0x00408742:
      if ((DAT_004fbd30 < iVar3) && (DAT_004fbe68 == DAT_004fbf88)) {
        _DAT_004fbfbc = 8;
      }
    }
  }
  else if ((DAT_0043ed0c < DAT_004fbd24) &&
          ((DAT_004fbd24 < DAT_0043ecec + DAT_0043ed0c && (DAT_0043ed10 < DAT_004fbd30)))) {
    iVar3 = DAT_0043ecf0 + DAT_0043ed10;
    goto joined_r0x00408742;
  }
  if (((DAT_0043ecfc < DAT_004fbd24) && (DAT_004fbd24 < DAT_0043ecf4 + DAT_0043ecfc)) &&
     ((DAT_0043ed00 < DAT_004fbd30 &&
      ((DAT_004fbd30 < DAT_0043ecf8 + DAT_0043ed00 && (DAT_004fbe68 == DAT_004fbf88)))))) {
    _DAT_004fbfbc = _DAT_004fbfbc | 4;
  }
  if (DAT_004fbfb4 == 1) {
    DAT_004fbfb4 = 0;
    DAT_004fbfb0 = 0;
  }
  if (((bStack_4 & 0x80) == 0) && ((bStack_3 & 0x80) == 0)) {
    DAT_004fbe54 = 0;
    DAT_004fbfa8 = 0;
    DAT_004fbfb8 = 0;
    if (DAT_004fbfb0 == 1) {
      DAT_004fbfb4 = 1;
      if (((DAT_0043ed0c < DAT_004fbd24) && (DAT_004fbd24 < DAT_0043ecec + DAT_0043ed0c)) &&
         ((DAT_0043ed10 < DAT_004fbd30 &&
          ((DAT_004fbd30 < DAT_0043ecf0 + DAT_0043ed10 && (DAT_004fbe68 == DAT_004fbf88)))))) {
        _DAT_004fbfbc = _DAT_004fbfbc | 2;
      }
      if ((((DAT_0043ecfc < DAT_004fbd24) && (DAT_004fbd24 < DAT_0043ecf4 + DAT_0043ecfc)) &&
          (DAT_0043ed00 < DAT_004fbd30)) &&
         ((DAT_004fbd30 < DAT_0043ecf8 + DAT_0043ed00 && (DAT_004fbe68 == DAT_004fbf88)))) {
        _DAT_004fbfbc = _DAT_004fbfbc | 1;
      }
    }
  }
  else {
    DAT_004fbfb8 = 1;
    DAT_004fbfb0 = 1;
    if (DAT_004fbfa8 == 0) {
      if ((((DAT_0043ed0c < DAT_004fbd24) && (DAT_004fbd24 < DAT_0043ecec + DAT_0043ed0c)) &&
          (DAT_0043ed10 < DAT_004fbd30)) &&
         (((DAT_004fbd30 < DAT_0043ecf0 + DAT_0043ed10 && (DAT_0051c27c == 0xc)) &&
          (DAT_004fbe68 == DAT_004fbf88)))) {
        _DAT_004fbfbc = _DAT_004fbfbc | 2;
      }
      if (((DAT_0043ecfc < DAT_004fbd24) && (DAT_004fbd24 < DAT_0043ecf4 + DAT_0043ecfc)) &&
         ((DAT_0043ed00 < DAT_004fbd30 &&
          (((DAT_004fbd30 < DAT_0043ecf8 + DAT_0043ed00 && (DAT_0051c27c == 0xc)) &&
           (DAT_004fbe68 == DAT_004fbf88)))))) {
        _DAT_004fbfbc = _DAT_004fbfbc | 1;
      }
      DAT_004fbe54 = 1;
      DAT_004fbfa8 = 1;
      uVar4 = CSound_IsSoundPlaying((int)DAT_004fbf8c);
      if ((uVar4 == 0) && (DAT_0043ed14 != 0)) {
        CSound_Play(DAT_004fbf8c,0,0);
      }
    }
    else {
      DAT_004fbe54 = 0;
    }
  }
  DAT_004fbd50 = 0;
  _DAT_004fbfac = 0;
  DAT_004fbd24 = DAT_004fbd24 + aDStack_28[6];
  if ((float)iStack_c <= _DAT_0043b2f0) {
    if ((float)iStack_c < _DAT_0043b2f0) {
      DAT_004fbd30 = DAT_004fbd30 + iStack_c;
    }
  }
  else {
    DAT_004fbd30 = DAT_004fbd30 + iStack_c;
  }
LAB_00408a00:
  if (DAT_0051c334 == 0) {
    iVar3 = 1;
    if (DAT_0051bca8 != 0) {
      iVar3 = 4;
    }
    if ((_DAT_004fbe5c & 1) != 0) {
      DAT_004fbd24 = DAT_004fbd24 - iVar3;
    }
    if ((_DAT_004fbe5c & 2) != 0) {
      DAT_004fbd24 = DAT_004fbd24 + iVar3;
    }
    if ((_DAT_004fbe5c & 8) != 0) {
      DAT_004fbd30 = DAT_004fbd30 + iVar3;
    }
    if ((_DAT_004fbe5c & 4) != 0) {
      DAT_004fbd30 = DAT_004fbd30 - iVar3;
    }
  }
  if ((_DAT_004fbe5c & 0x10) == 0) {
    DAT_004fbfe0 = 0;
  }
  else if (DAT_004fbfe0 == 0) {
    DAT_004fbe54 = 1;
    DAT_004fbfb4 = 1;
    DAT_004fbfe0 = 1;
    if (((DAT_0043ed0c < DAT_004fbd24) && (DAT_004fbd24 < DAT_0043ecec + DAT_0043ed0c)) &&
       ((DAT_0043ed10 < DAT_004fbd30 &&
        ((DAT_004fbd30 < DAT_0043ecf0 + DAT_0043ed10 && (DAT_004fbe68 == DAT_004fbf88)))))) {
      _DAT_004fbfbc = _DAT_004fbfbc | 2;
    }
    if ((((DAT_0043ecfc < DAT_004fbd24) && (DAT_004fbd24 < DAT_0043ecf4 + DAT_0043ecfc)) &&
        (DAT_0043ed00 < DAT_004fbd30)) &&
       ((DAT_004fbd30 < DAT_0043ecf8 + DAT_0043ed00 && (DAT_004fbe68 == DAT_004fbf88)))) {
      _DAT_004fbfbc = _DAT_004fbfbc | 1;
    }
  }
  if (DAT_004fbf80 < DAT_004fbd48 + DAT_004fbd24) {
    DAT_004fbd24 = DAT_004fbf80 - DAT_004fbd48;
  }
  if (DAT_004fbf84 < DAT_004fbd4c + DAT_004fbd30) {
    DAT_004fbd30 = DAT_004fbf84 - DAT_004fbd4c;
  }
  if (DAT_004fbd48 + DAT_004fbd24 < DAT_004fbf78) {
    DAT_004fbd24 = DAT_004fbf78 - DAT_004fbd48;
  }
  if (DAT_004fbd4c + DAT_004fbd30 < DAT_004fbf7c) {
    DAT_004fbd30 = DAT_004fbf7c - DAT_004fbd4c;
  }
  piVar1 = *(int **)(DAT_0044de08 + 0xc);
  if (DAT_0043ed14 != 0) {
    if (DAT_004fbe68 == DAT_004fbf88) {
      aDStack_28[2] = 0;
      aDStack_28[3] = 0;
      aDStack_28[4] = 0x13;
      aDStack_28[5] = 0x13;
      if (0x26d < DAT_004fbd24) {
        DAT_004fbd24 = 0x26d;
      }
      if (0x1cd < DAT_004fbd30) {
        DAT_004fbd30 = 0x1cd;
      }
      if (DAT_0043ed18 != 0) {
        (**(code **)(*piVar1 + 0x1c))
                  (piVar1,DAT_004fbd24,DAT_004fbd30,DAT_004fbf88,aDStack_28 + 2,1);
      }
    }
    else {
      if (((DAT_0050936c == 1) && (DAT_00507b5c < 2)) &&
         (((&DAT_00507ae4)[DAT_00507b5c] == 0 && (0x280 - DAT_004fbd18 < DAT_004fbd24)))) {
        DAT_004fbd24 = 0x280 - DAT_004fbd18;
      }
      if (DAT_004fbe68 == DAT_00519944) {
        if (0x280 - DAT_004fbd18 < DAT_004fbd24) {
          DAT_004fbd24 = 0x280 - DAT_004fbd18;
        }
        if (0x1cd < DAT_004fbd30) {
          DAT_004fbd30 = 0x1cd;
        }
      }
      if ((DAT_004fbd48 == 0) && (DAT_004fbd4c == 0)) {
        aDStack_28[2] = 0;
        aDStack_28[3] = 0;
        aDStack_28[4] = DAT_004fbd18;
        aDStack_28[5] = DAT_004fbd1c;
        if (0x280 < DAT_004fbd18 + DAT_004fbd24) {
          aDStack_28[4] = 0x280 - DAT_004fbd24;
        }
        if (0x1e0 < DAT_004fbd1c + DAT_004fbd30) {
          aDStack_28[5] = 0x1e0 - DAT_004fbd30;
        }
        (**(code **)(*piVar1 + 0x1c))
                  (piVar1,DAT_004fbd24,DAT_004fbd30,DAT_004fbe68,aDStack_28 + 2,1);
      }
      else {
        aDStack_28[2] = 0;
        aDStack_28[3] = 0;
        aDStack_28[4] = DAT_004fbd18;
        aDStack_28[5] = DAT_004fbd1c;
        if (0x280 < DAT_004fbd18 + DAT_004fbd24) {
          aDStack_28[4] = 0x280 - DAT_004fbd24;
        }
        if (0x1e0 < DAT_004fbd1c + DAT_004fbd30) {
          aDStack_28[5] = 0x1e0 - DAT_004fbd30;
        }
        BlitColorKeyedSurfaceClipped(DAT_004fbe68,DAT_004fbd24,DAT_004fbd30,(int *)(aDStack_28 + 2))
        ;
      }
    }
  }
  DAT_004fbfe4 = DAT_004fbfe4 + 1;
  if (0x1f < DAT_004fbfe4) {
    DAT_004fbfe4 = 0;
  }
  if ((DAT_004fbfc8 != DAT_004fbd24) || (DAT_004fbfcc != DAT_004fbd30)) {
    DAT_0043ed18 = 1;
    DAT_004fbfc8 = DAT_004fbd24;
    DAT_004fbfcc = DAT_004fbd30;
  }
  return;
}

