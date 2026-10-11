/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00437eca; function: FUN_00437eca; body bytes: 520
 * callers: 1; callees: 2; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void * __thiscall FUN_00437eca(void *this,byte *param_1,int *param_2,void *param_3,uint param_4)

{
  void *pvVar1;
  uint uVar2;
  void *pvVar3;
  uint uVar4;
  void *this_00;
  byte bVar5;
  undefined *puVar6;
  void *local_c;
  byte *local_8;
  
  local_c = (void *)0x0;
  bVar5 = *param_1;
  local_8 = param_1 + 1;
  while( true ) {
    if (DAT_00449b10 < 2) {
      uVar2 = (byte)PTR_DAT_00449b1c[(uint)bVar5 * 2] & 8;
      this = PTR_DAT_00449b1c;
    }
    else {
      puVar6 = (undefined *)0x8;
      uVar2 = FUN_004365c0(this,(uint)bVar5,8);
      this = puVar6;
    }
    if (uVar2 == 0) break;
    bVar5 = *local_8;
    local_8 = local_8 + 1;
  }
  if (bVar5 == 0x2d) {
    param_4 = param_4 | 2;
LAB_00437f25:
    bVar5 = *local_8;
    local_8 = local_8 + 1;
  }
  else if (bVar5 == 0x2b) goto LAB_00437f25;
  if ((((int)param_3 < 0) || (param_3 == (void *)0x1)) || (0x24 < (int)param_3)) {
    if (param_2 != (int *)0x0) {
      *param_2 = (int)param_1;
    }
    return (void *)0x0;
  }
  this_00 = (void *)0x10;
  if (param_3 == (void *)0x0) {
    if (bVar5 != 0x30) {
      param_3 = (void *)0xa;
      goto LAB_00437f8f;
    }
    if ((*local_8 != 0x78) && (*local_8 != 0x58)) {
      param_3 = (void *)0x8;
      goto LAB_00437f8f;
    }
    param_3 = (void *)0x10;
  }
  if (((param_3 == (void *)0x10) && (bVar5 == 0x30)) && ((*local_8 == 0x78 || (*local_8 == 0x58))))
  {
    bVar5 = local_8[1];
    local_8 = local_8 + 2;
  }
LAB_00437f8f:
  pvVar3 = (void *)(0xffffffff / ZEXT48(param_3));
  do {
    uVar2 = (uint)bVar5;
    if (DAT_00449b10 < 2) {
      uVar4 = (byte)PTR_DAT_00449b1c[uVar2 * 2] & 4;
    }
    else {
      pvVar1 = (void *)0x4;
      uVar4 = FUN_004365c0(this_00,uVar2,4);
      this_00 = pvVar1;
    }
    if (uVar4 == 0) {
      if (DAT_00449b10 < 2) {
        uVar2 = *(ushort *)(PTR_DAT_00449b1c + uVar2 * 2) & 0x103;
      }
      else {
        pvVar1 = (void *)0x103;
        uVar2 = FUN_004365c0(this_00,uVar2,0x103);
        this_00 = pvVar1;
      }
      if (uVar2 == 0) {
LAB_0043803b:
        local_8 = local_8 + -1;
        if ((param_4 & 8) == 0) {
          if (param_2 != (int *)0x0) {
            local_8 = param_1;
          }
          local_c = (void *)0x0;
        }
        else if (((param_4 & 4) != 0) ||
                (((param_4 & 1) == 0 &&
                 ((((param_4 & 2) != 0 && ((void *)0x80000000 < local_c)) ||
                  (((param_4 & 2) == 0 && ((void *)0x7fffffff < local_c)))))))) {
          _DAT_0051c3c8 = 0x22;
          if ((param_4 & 1) == 0) {
            local_c = (void *)(((param_4 & 2) != 0) + 0x7fffffff);
          }
          else {
            local_c = (void *)0xffffffff;
          }
        }
        if (param_2 != (int *)0x0) {
          *param_2 = (int)local_8;
        }
        if ((param_4 & 2) == 0) {
          return local_c;
        }
        return (void *)-(int)local_c;
      }
      uVar2 = FUN_00439212(this_00,(int)(char)bVar5);
      this_00 = (void *)(uVar2 - 0x37);
    }
    else {
      this_00 = (void *)((char)bVar5 + -0x30);
    }
    if (param_3 <= this_00) goto LAB_0043803b;
    if ((local_c < pvVar3) ||
       ((local_c == pvVar3 && (this_00 <= (void *)(0xffffffff % ZEXT48(param_3)))))) {
      local_c = (void *)((int)local_c * (int)param_3 + (int)this_00);
      param_4 = param_4 | 8;
    }
    else {
      param_4 = param_4 | 0xc;
    }
    bVar5 = *local_8;
    local_8 = local_8 + 1;
  } while( true );
}

