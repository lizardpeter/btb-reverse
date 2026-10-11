/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00432971; function: FUN_00432971; body bytes: 1825
 * callers: 2; callees: 11; success: True
 */


int __cdecl FUN_00432971(int *param_1,byte *param_2,undefined4 *param_3)

{
  int iVar1;
  uint uVar2;
  WCHAR *pWVar3;
  WCHAR *pWVar4;
  undefined4 uVar5;
  short *psVar6;
  int *piVar7;
  LPSTR pCVar8;
  byte bVar9;
  int iVar10;
  uint uVar11;
  LPSTR pCVar12;
  ulonglong uVar13;
  undefined8 uVar14;
  ulonglong uVar15;
  undefined1 local_24c [511];
  undefined1 local_4d;
  undefined4 local_4c;
  undefined4 local_48;
  uint local_44;
  uint local_40;
  CHAR local_3c [4];
  undefined4 local_38;
  int local_34;
  int local_30;
  int local_2c;
  int local_28;
  int local_24;
  int local_20;
  char local_1a;
  char local_19;
  int local_18;
  int local_14;
  LPSTR local_10;
  WCHAR *local_c;
  uint local_8;
  
  local_34 = 0;
  bVar9 = *param_2;
  param_2 = param_2 + 1;
  local_10 = (LPSTR)0x0;
  local_18 = 0;
  do {
    if ((bVar9 == 0) || (local_18 < 0)) {
      return local_18;
    }
    if (((char)bVar9 < ' ') || ('x' < (char)bVar9)) {
      uVar2 = 0;
    }
    else {
      uVar2 = *(byte *)((int)&PTR_LAB_0043b668 + (int)(char)bVar9) & 0xf;
    }
    local_34 = (int)(char)(&DAT_0043b688)[uVar2 * 8 + local_34] >> 4;
    switch(local_34) {
    case 0:
switchD_004329df_caseD_0:
      local_28 = 0;
      if ((PTR_DAT_00449b1c[(uint)bVar9 * 2 + 1] & 0x80) != 0) {
        FUN_004330b2((int)(char)bVar9,param_1,&local_18);
        bVar9 = *param_2;
        param_2 = param_2 + 1;
      }
      FUN_004330b2((int)(char)bVar9,param_1,&local_18);
      break;
    case 1:
      local_14 = -1;
      local_38 = 0;
      local_2c = 0;
      local_24 = 0;
      local_20 = 0;
      local_8 = 0;
      local_28 = 0;
      break;
    case 2:
      if (bVar9 == 0x20) {
        local_8 = local_8 | 2;
      }
      else if (bVar9 == 0x23) {
        local_8 = local_8 | 0x80;
      }
      else if (bVar9 == 0x2b) {
        local_8 = local_8 | 1;
      }
      else if (bVar9 == 0x2d) {
        local_8 = local_8 | 4;
      }
      else if (bVar9 == 0x30) {
        local_8 = local_8 | 8;
      }
      break;
    case 3:
      if (bVar9 == 0x2a) {
        local_24 = FUN_00433150((int *)&param_3);
        if (local_24 < 0) {
          local_8 = local_8 | 4;
          local_24 = -local_24;
        }
      }
      else {
        local_24 = (char)bVar9 + -0x30 + local_24 * 10;
      }
      break;
    case 4:
      local_14 = 0;
      break;
    case 5:
      if (bVar9 == 0x2a) {
        local_14 = FUN_00433150((int *)&param_3);
        if (local_14 < 0) {
          local_14 = -1;
        }
      }
      else {
        local_14 = (char)bVar9 + -0x30 + local_14 * 10;
      }
      break;
    case 6:
      if (bVar9 == 0x49) {
        if ((*param_2 != 0x36) || (param_2[1] != 0x34)) {
          local_34 = 0;
          goto switchD_004329df_caseD_0;
        }
        param_2 = param_2 + 2;
        local_8 = local_8 | 0x8000;
      }
      else if (bVar9 == 0x68) {
        local_8 = local_8 | 0x20;
      }
      else if (bVar9 == 0x6c) {
        local_8 = local_8 | 0x10;
      }
      else if (bVar9 == 0x77) {
        local_8 = local_8 | 0x800;
      }
      break;
    case 7:
      pWVar4 = local_c;
      if ((char)bVar9 < 'h') {
        if ((char)bVar9 < 'e') {
          if ((char)bVar9 < 'Y') {
            if (bVar9 == 0x58) {
LAB_00432df0:
              local_30 = 7;
LAB_00432df7:
              local_10 = (LPSTR)0x10;
              if ((local_8 & 0x80) != 0) {
                local_1a = '0';
                local_19 = (char)local_30 + 'Q';
                local_20 = 2;
              }
              goto LAB_00432e61;
            }
            if (bVar9 != 0x43) {
              if ((bVar9 != 0x45) && (bVar9 != 0x47)) {
                if (bVar9 == 0x53) {
                  if ((local_8 & 0x830) == 0) {
                    local_8 = local_8 | 0x800;
                  }
                  goto LAB_00432b9e;
                }
                goto LAB_00432f7b;
              }
              local_38 = 1;
              bVar9 = bVar9 + 0x20;
              goto LAB_00432bff;
            }
            if ((local_8 & 0x830) == 0) {
              local_8 = local_8 | 0x800;
            }
LAB_00432c2c:
            if ((local_8 & 0x810) == 0) {
              uVar5 = FUN_00433150((int *)&param_3);
              local_24c[0] = (char)uVar5;
              local_10 = (LPSTR)0x1;
            }
            else {
              uVar5 = FUN_0043316d((int *)&param_3);
              local_10 = FUN_004373db(local_24c,(WCHAR)uVar5);
              if ((int)local_10 < 0) {
                local_2c = 1;
              }
            }
            pWVar4 = (WCHAR *)local_24c;
          }
          else if (bVar9 == 0x5a) {
            psVar6 = (short *)FUN_00433150((int *)&param_3);
            if ((psVar6 == (short *)0x0) ||
               (pWVar4 = *(WCHAR **)(psVar6 + 2), pWVar4 == (WCHAR *)0x0)) {
              local_c = (WCHAR *)PTR_DAT_00447720;
              pWVar4 = (WCHAR *)PTR_DAT_00447720;
              goto LAB_00432d71;
            }
            if ((local_8 & 0x800) == 0) {
              local_28 = 0;
              local_10 = (LPSTR)(int)*psVar6;
            }
            else {
              local_28 = 1;
              local_10 = (LPSTR)((uint)(int)*psVar6 >> 1);
            }
          }
          else {
            if (bVar9 == 99) goto LAB_00432c2c;
            if (bVar9 == 100) goto LAB_00432e56;
          }
        }
        else {
LAB_00432bff:
          local_8 = local_8 | 0x40;
          pWVar4 = (WCHAR *)local_24c;
          if (local_14 < 0) {
            local_14 = 6;
          }
          else if ((local_14 == 0) && (bVar9 == 0x67)) {
            local_14 = 1;
          }
          local_4c = *param_3;
          local_48 = param_3[1];
          param_3 = param_3 + 2;
          local_c = pWVar4;
          (*(code *)PTR_FUN_00447728)(&local_4c,local_24c,(int)(char)bVar9,local_14,local_38);
          uVar2 = local_8 & 0x80;
          if ((uVar2 != 0) && (local_14 == 0)) {
            (*(code *)PTR_FUN_00447734)(local_24c);
          }
          if ((bVar9 == 0x67) && (uVar2 == 0)) {
            (*(code *)PTR_FUN_0044772c)(local_24c);
          }
          if (local_24c[0] == '-') {
            local_8 = local_8 | 0x100;
            pWVar4 = (WCHAR *)(local_24c + 1);
            local_c = pWVar4;
          }
LAB_00432d71:
          local_10 = (LPSTR)_strlen((char *)pWVar4);
          pWVar4 = local_c;
        }
      }
      else {
        if (bVar9 == 0x69) {
LAB_00432e56:
          local_8 = local_8 | 0x40;
        }
        else {
          if (bVar9 == 0x6e) {
            piVar7 = (int *)FUN_00433150((int *)&param_3);
            if ((local_8 & 0x20) == 0) {
              *piVar7 = local_18;
            }
            else {
              *(undefined2 *)piVar7 = (undefined2)local_18;
            }
            local_2c = 1;
            break;
          }
          if (bVar9 == 0x6f) {
            local_10 = (LPSTR)0x8;
            if ((local_8 & 0x80) != 0) {
              local_8 = local_8 | 0x200;
            }
            goto LAB_00432e61;
          }
          if (bVar9 == 0x70) {
            local_14 = 8;
            goto LAB_00432df0;
          }
          if (bVar9 == 0x73) {
LAB_00432b9e:
            iVar10 = local_14;
            if (local_14 == -1) {
              iVar10 = 0x7fffffff;
            }
            pWVar3 = (WCHAR *)FUN_00433150((int *)&param_3);
            if ((local_8 & 0x810) == 0) {
              pWVar4 = pWVar3;
              if (pWVar3 == (WCHAR *)0x0) {
                pWVar3 = (WCHAR *)PTR_DAT_00447720;
                pWVar4 = (WCHAR *)PTR_DAT_00447720;
              }
              for (; (iVar10 != 0 && ((char)*pWVar3 != '\0')); pWVar3 = (WCHAR *)((int)pWVar3 + 1))
              {
                iVar10 = iVar10 + -1;
              }
              local_10 = (LPSTR)((int)pWVar3 - (int)pWVar4);
            }
            else {
              if (pWVar3 == (WCHAR *)0x0) {
                pWVar3 = (WCHAR *)PTR_DAT_00447724;
              }
              local_28 = 1;
              for (pWVar4 = pWVar3; (iVar10 != 0 && (*pWVar4 != L'\0')); pWVar4 = pWVar4 + 1) {
                iVar10 = iVar10 + -1;
              }
              local_10 = (LPSTR)((int)pWVar4 - (int)pWVar3 >> 1);
              pWVar4 = pWVar3;
            }
            goto LAB_00432f7b;
          }
          if (bVar9 != 0x75) {
            if (bVar9 != 0x78) goto LAB_00432f7b;
            local_30 = 0x27;
            goto LAB_00432df7;
          }
        }
        local_10 = (LPSTR)0xa;
LAB_00432e61:
        if ((local_8 & 0x8000) == 0) {
          if ((local_8 & 0x20) == 0) {
            if ((local_8 & 0x40) == 0) {
              uVar2 = FUN_00433150((int *)&param_3);
              uVar13 = (ulonglong)uVar2;
              goto LAB_00432eb4;
            }
            uVar2 = FUN_00433150((int *)&param_3);
          }
          else if ((local_8 & 0x40) == 0) {
            uVar2 = FUN_00433150((int *)&param_3);
            uVar2 = uVar2 & 0xffff;
          }
          else {
            uVar5 = FUN_00433150((int *)&param_3);
            uVar2 = (uint)(short)uVar5;
          }
          uVar13 = (ulonglong)(int)uVar2;
        }
        else {
          uVar13 = FUN_0043315d((int *)&param_3);
        }
LAB_00432eb4:
        iVar10 = (int)(uVar13 >> 0x20);
        if ((((local_8 & 0x40) != 0) && (iVar10 == 0 || (longlong)uVar13 < 0)) &&
           ((longlong)uVar13 < 0)) {
          local_8 = local_8 | 0x100;
          uVar13 = CONCAT44(-(iVar10 + (uint)((int)uVar13 != 0)),-(int)uVar13);
        }
        uVar2 = (uint)(uVar13 >> 0x20);
        uVar15 = uVar13 & 0xffffffff;
        if ((local_8 & 0x8000) == 0) {
          uVar2 = 0;
        }
        if (local_14 < 0) {
          local_14 = 1;
        }
        else {
          local_8 = local_8 & 0xfffffff7;
        }
        if ((int)uVar13 == 0 && uVar2 == 0) {
          local_20 = 0;
        }
        local_c = (WCHAR *)&local_4d;
        while( true ) {
          uVar11 = (uint)uVar15;
          iVar10 = local_14 + -1;
          if ((local_14 < 1) && (uVar11 == 0 && uVar2 == 0)) break;
          local_40 = (int)local_10 >> 0x1f;
          local_44 = (uint)local_10;
          local_14 = iVar10;
          uVar14 = __aullrem(uVar11,uVar2,(uint)local_10,local_40);
          iVar10 = (int)uVar14 + 0x30;
          uVar15 = __aulldiv(uVar11,uVar2,local_44,local_40);
          uVar2 = (uint)(uVar15 >> 0x20);
          if (0x39 < iVar10) {
            iVar10 = iVar10 + local_30;
          }
          pWVar4 = (WCHAR *)((int)local_c + -1);
          *(char *)local_c = (char)iVar10;
          local_c = pWVar4;
        }
        iVar1 = -(int)local_c;
        local_10 = &local_4d + iVar1;
        pWVar4 = (WCHAR *)((int)local_c + 1);
        local_14 = iVar10;
        if (((local_8 & 0x200) != 0) && ((*(char *)pWVar4 != '0' || (local_10 == (LPSTR)0x0)))) {
          *(char *)local_c = '0';
          local_10 = (LPSTR)((int)&local_4c + iVar1);
          pWVar4 = local_c;
        }
      }
LAB_00432f7b:
      local_c = pWVar4;
      uVar2 = local_8;
      if (local_2c == 0) {
        if ((local_8 & 0x40) != 0) {
          if ((local_8 & 0x100) == 0) {
            if ((local_8 & 1) == 0) {
              if ((local_8 & 2) == 0) goto LAB_00432fb3;
              local_1a = ' ';
            }
            else {
              local_1a = '+';
            }
          }
          else {
            local_1a = '-';
          }
          local_20 = 1;
        }
LAB_00432fb3:
        iVar10 = (local_24 - local_20) - (int)local_10;
        if ((local_8 & 0xc) == 0) {
          FUN_004330e7(0x20,iVar10,param_1,&local_18);
        }
        FUN_00433118(&local_1a,local_20,param_1,&local_18);
        if (((uVar2 & 8) != 0) && ((uVar2 & 4) == 0)) {
          FUN_004330e7(0x30,iVar10,param_1,&local_18);
        }
        if ((local_28 == 0) || (pCVar12 = local_10, pWVar4 = local_c, (int)local_10 < 1)) {
          FUN_00433118((char *)local_c,(int)local_10,param_1,&local_18);
        }
        else {
          do {
            pCVar12 = pCVar12 + -1;
            pCVar8 = FUN_004373db(local_3c,*pWVar4);
            if ((int)pCVar8 < 1) break;
            FUN_00433118(local_3c,(int)pCVar8,param_1,&local_18);
            pWVar4 = pWVar4 + 1;
          } while (pCVar12 != (LPSTR)0x0);
        }
        if ((local_8 & 4) != 0) {
          FUN_004330e7(0x20,iVar10,param_1,&local_18);
        }
      }
    }
    bVar9 = *param_2;
    param_2 = param_2 + 1;
  } while( true );
}

