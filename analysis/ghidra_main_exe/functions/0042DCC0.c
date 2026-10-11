/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0042dcc0; function: UpdateEnterNamePopup; body bytes: 1100
 * callers: 1; callees: 5; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void UpdateEnterNamePopup(void)

{
  byte *pbVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  uint *puVar7;
  int local_80 [32];
  
  iVar6 = -1;
  piVar2 = *(int **)(DAT_0044de08 + 0xc);
  if (DAT_0051c39c == 1) {
    iVar3 = AnyManagedSoundPlaying(DAT_0044ddd8);
    iVar4 = DAT_0051be48;
    iVar6 = DAT_0051b3a0;
    if (iVar3 == 0) {
      DAT_0044de14 = 0x41;
      DAT_0051c39c = iVar3;
      (&DAT_0051c24c)[DAT_0051b3a0] = DAT_0051c248;
      (&DAT_0051b404)[iVar6] = iVar4;
      if (0 < iVar4) {
        iVar3 = 0;
        puVar7 = &DAT_0051b41c + iVar6 * 9;
        do {
          pbVar1 = &DAT_00519948 + iVar3;
          iVar3 = iVar3 + 1;
          *puVar7 = (uint)*pbVar1;
          puVar7 = puVar7 + 1;
        } while (iVar3 < iVar4);
      }
      DAT_0051c2f4 = 1;
      DAT_00519934 = iVar6;
      (&DAT_0051b4d0)[iVar6 * 100] = 1;
      return;
    }
  }
  else {
    iVar4 = 0;
    do {
      if (((((&DAT_004fbe70)[iVar4] & 0x80) != 0) && (iVar4 != 0x2a)) && (iVar4 != 0x36)) {
        iVar6 = iVar4;
        if ((iVar4 != -1) && (iVar4 != DAT_00446ff4)) {
          if (iVar4 == 0xe) {
            DAT_004fbd44 = -1;
            if (0 < DAT_0051be48) {
              DAT_0051be48 = DAT_0051be48 + -1;
            }
          }
          else if (((-1 < DAT_004fbd44) && (DAT_0051be48 < 8)) &&
                  ((DAT_004fbd44 < 0x80 &&
                   ((DAT_004fbd44 != 0x21 && -1 < DAT_004fbd44 + -0x21 && (iVar4 < 0x36)))))) {
            (&DAT_00519948)[DAT_0051be48] = (char)DAT_004fbd44 + -0x21;
            DAT_0051be48 = DAT_0051be48 + 1;
          }
        }
        break;
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < 0xed);
    local_80[9] = 0x6f;
    local_80[0xb] = 0x95;
    local_80[0xd] = 0x6f;
    local_80[0xf] = 0x95;
    local_80[0x10] = 0x1ee;
    local_80[4] = 0x1ee;
    local_80[0x11] = 0x131;
    local_80[0x14] = 0x114;
    local_80[0x15] = 0x12f;
    local_80[1] = 0x70;
    local_80[3] = 0x70;
    local_80[5] = 0x131;
    local_80[6] = 0x114;
    local_80[7] = 0x12f;
    iVar4 = -1;
    local_80[8] = 0xfc;
    local_80[10] = 0x119;
    local_80[0xc] = 0x167;
    local_80[0xe] = 0x186;
    local_80[0x12] = 0x221;
    local_80[0x13] = 0x165;
    local_80[0x16] = 0x16b;
    local_80[0x17] = 0x169;
    local_80[0x18] = 0xc2;
    local_80[0x19] = 0xbd;
    local_80[0x1a] = 0x1c1;
    local_80[0x1b] = 0xeb;
    local_80[0x1c] = 0xf2;
    local_80[0x1d] = 99;
    local_80[0x1e] = 0x18d;
    local_80[0x1f] = 0x93;
    local_80[0] = 0x102;
    local_80[2] = 0x16f;
    iVar3 = 0;
    piVar5 = local_80 + 10;
    do {
      DAT_00446ff4 = iVar6;
      if ((((piVar5[-2] < DAT_004fbd24) && (DAT_004fbd24 < *piVar5)) && (piVar5[-1] < DAT_004fbd30))
         && (DAT_004fbd30 < piVar5[1])) {
        if ((DAT_00446ffc == iVar3) || (iVar3 != 3)) {
          if ((iVar3 == 2) && (DAT_00446ffc != 2)) {
            PlayManagedSoundById(DAT_0044ddd8,0x1c8,0x32,2);
            DAT_00446ffc = iVar3;
          }
        }
        else {
          PlayManagedSoundById(DAT_0044ddd8,0x1cc,0x32,2);
          DAT_00446ffc = iVar3;
        }
        _DAT_00447000 = iVar3;
        if ((iVar3 == 5) || (iVar3 == 4)) {
          if (DAT_00446ffc != iVar3) {
            DAT_00446ffc = iVar3;
            PlayManagedSoundById(DAT_0044ddd8,iVar3 + 0x13d,0x32,2);
          }
        }
        else {
          (**(code **)(*piVar2 + 0x1c))
                    (piVar2,local_80[iVar3 * 2],local_80[iVar3 * 2 + 1],(&DAT_0051b3a8)[iVar3 * 2],0
                     ,0);
        }
        iVar4 = iVar3;
        if (iVar3 != -1) goto LAB_0042dfcd;
        break;
      }
      iVar3 = iVar3 + 1;
      piVar5 = piVar5 + 4;
    } while (iVar3 < 6);
    DAT_00446ffc = -1;
    iVar3 = iVar4;
LAB_0042dfcd:
    if ((((DAT_004fbfb8 != 0) && (iVar3 != -1)) && (iVar3 != 5)) && (iVar3 != 4)) {
      (**(code **)(*piVar2 + 0x1c))
                (piVar2,local_80[iVar3 * 2],local_80[iVar3 * 2 + 1],
                 *(undefined4 *)(&DAT_0051b3ac + iVar3 * 8),0,0);
    }
    if (DAT_004fbfb4 != 0) {
      if (iVar3 == 0) {
        DAT_0051c248 = DAT_0051c248 + -1;
        if (DAT_0051c248 < 0) {
          DAT_0051c248 = 5;
          return;
        }
      }
      else if (iVar3 == 1) {
        DAT_0051c248 = DAT_0051c248 + 1;
        if (5 < DAT_0051c248) {
          DAT_0051c248 = 0;
          return;
        }
      }
      else if (iVar3 == 3) {
        if ((0 < DAT_0051be48) || (-1 < DAT_0051c248)) {
          PlayManagedSoundById(DAT_0044ddd8,0x140,0x32,2);
          DAT_0051c39c = 1;
          return;
        }
      }
      else if (iVar3 == 2) {
        piVar2 = *(int **)(DAT_0044de08 + 4);
        DAT_0051c2f4 = 0;
        UnregisterBitmapSurface(0x51c268);
        if (DAT_0051c268 != (int *)0x0) {
          (**(code **)(*DAT_0051c268 + 8))(DAT_0051c268);
          DAT_0051c268 = (int *)0x0;
        }
        DAT_0051c268 = LoadBitmapToDirectDrawSurface(piVar2,s_Data_ui_name_signs_bmp_00447138,0,0);
        RegisterBitmapSurface(&DAT_0051c268,s_Data_ui_name_signs_bmp_00447138);
      }
    }
  }
  return;
}

