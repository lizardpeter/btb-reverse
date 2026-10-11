/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00415e50; function: BlitColorKeyedSurfaceClipped; body bytes: 927
 * callers: 13; callees: 0; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl BlitColorKeyedSurfaceClipped(int *param_1,int param_2,int param_3,int *param_4)

{
  int iVar1;
  int iVar2;
  int local_a0;
  int local_9c;
  int local_98;
  int local_94;
  int *local_90;
  int iStack_8c;
  int iStack_88;
  int iStack_84;
  int iStack_80;
  undefined4 local_7c;
  undefined4 local_78;
  int iStack_74;
  int iStack_70;
  
  local_90 = *(int **)(DAT_0044de08 + 0xc);
  if (param_4 == (int *)0x0) {
    local_7c = 0x7c;
    local_78 = 6;
    (**(code **)(*param_1 + 0x58))(param_1,&local_7c);
    local_a0 = 0;
    local_9c = 0;
  }
  else {
    local_a0 = *param_4;
    local_9c = param_4[1];
    iStack_70 = param_4[2];
    iStack_74 = param_4[3];
  }
  iStack_8c = param_2 - DAT_0051071c;
  iStack_88 = param_3 - DAT_00510720;
  iVar1 = iStack_70 - local_a0;
  iVar2 = iStack_74 - local_9c;
  if ((((-1 < iVar1 + iStack_8c) && (iStack_8c - iVar1 < 0x281)) && (-1 < iVar2 + iStack_88)) &&
     (iStack_88 - iVar2 < 0x1e1)) {
    if (iStack_8c < 1) {
      local_a0 = local_a0 - iStack_8c;
      iStack_8c = 0;
    }
    local_98 = iStack_70;
    if (0x27f < iVar1 + iStack_8c) {
      local_98 = iStack_70 + ((0x280 - iVar1) - iStack_8c);
    }
    if (iStack_88 < 1) {
      local_9c = local_9c - iStack_88;
      iStack_88 = 0;
    }
    if (0x1df < iVar2 + iStack_88) {
      iStack_74 = iStack_74 + ((0x1e0 - iVar2) - iStack_88);
    }
    if ((local_9c < iStack_74) && (local_a0 < local_98)) {
      iStack_84 = (iStack_8c - local_a0) + local_98;
      iStack_80 = (iStack_88 - local_9c) + iStack_74;
      local_94 = iStack_74;
      iVar1 = (**(code **)(*local_90 + 0x14))(local_90,&iStack_8c,param_1,&local_a0,0x8000,0);
      if (iVar1 != 0) {
        if (iVar1 < -0x7789fee7) {
          if (iVar1 == -0x7789fee8) {
            _DAT_004439a8 = 10;
            return;
          }
          if (iVar1 < -0x7789ff7d) {
            if (iVar1 == -0x7789ff7e) {
              _DAT_004439a8 = 2;
              return;
            }
            if (iVar1 < -0x7ff8ffa8) {
              if (iVar1 == -0x7ff8ffa9) {
                _DAT_004439a8 = 3;
                return;
              }
              if (iVar1 == -0x7fffbfff) {
                _DAT_004439a8 = 0x10;
                return;
              }
              if (iVar1 == -0x7fffbffb) {
                _DAT_004439a8 = 0;
                return;
              }
            }
            else if (iVar1 == -0x7789ff92) {
              _DAT_004439a8 = 1;
              return;
            }
          }
          else {
            switch(iVar1) {
            case -0x7789ff6a:
              _DAT_004439a8 = 4;
              return;
            case -0x7789ff4c:
              _DAT_004439a8 = 5;
              return;
            case -0x7789ff33:
              _DAT_004439a8 = 7;
              return;
            case -0x7789ff06:
              _DAT_004439a8 = 9;
              return;
            }
          }
        }
        else if (iVar1 < -0x7789fe3d) {
          if (iVar1 == -0x7789fe3e) {
            _DAT_004439a8 = 0xf;
            return;
          }
          switch(iVar1) {
          case -0x7789fede:
            _DAT_004439a8 = 0xb;
            return;
          case -0x7789feca:
            _DAT_004439a8 = 0xc;
            return;
          case -0x7789feac:
            _DAT_004439a8 = 0xd;
            return;
          case -0x7789fe52:
            _DAT_004439a8 = 0xe;
            return;
          }
        }
        else if (iVar1 == -0x7789fde4) {
          _DAT_004439a8 = 0x11;
        }
        else {
          if (iVar1 == -0x7789fdc1) {
            _DAT_004439a8 = 6;
            return;
          }
          if (iVar1 == -0x7789fdc0) {
            _DAT_004439a8 = 8;
            return;
          }
        }
      }
    }
  }
  return;
}

