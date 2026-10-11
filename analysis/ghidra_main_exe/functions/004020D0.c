/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004020d0; function: MainWindowProc; body bytes: 816
 * callers: 0; callees: 16; success: True
 */


LRESULT MainWindowProc(HWND param_1,uint param_2,uint param_3,int param_4)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  LRESULT LVar7;
  
  if (param_2 < 0x105) {
    if (param_2 == 0x104) {
      return 0;
    }
    switch(param_2) {
    case 2:
      DAT_0044ddb0 = 3;
      break;
    case 3:
      if (DAT_0044de08 != (undefined4 *)0x0) {
        UpdateDisplayDestinationRect((int)DAT_0044de08);
      }
      return 0;
    case 5:
      if ((param_3 == 4) || (DAT_0044de10 = 1, param_3 == 1)) {
        DAT_0044de10 = 0;
      }
      if (DAT_0044de08 != (undefined4 *)0x0) {
        UpdateDisplayDestinationRect((int)DAT_0044de08);
      }
      break;
    case 6:
      if (param_3 != 0) {
        UpdateDirectInputAcquireState();
      }
      break;
    case 0x20:
      if (DAT_0044de0c == 0) {
        SetCursor((HCURSOR)0x0);
        return 1;
      }
      break;
    case 0x24:
      iVar6 = GetSystemMetrics(0x20);
      iVar2 = GetSystemMetrics(0x21);
      iVar3 = GetSystemMetrics(0xf);
      iVar4 = GetSystemMetrics(4);
      iVar6 = iVar6 * 2 + 0x280;
      iVar3 = iVar4 + 0x1e0 + iVar2 * 2 + iVar3;
      *(int *)(param_4 + 0x1c) = iVar3;
      *(int *)(param_4 + 0x24) = iVar3;
      *(int *)(param_4 + 0x18) = iVar6;
      *(int *)(param_4 + 0x20) = iVar6;
      return 0;
    }
  }
  else if (param_2 < 0x114) {
    if (param_2 == 0x113) {
      if (param_3 == 99) {
        UpdateLoadingCursorAnimation();
        DrawLoadingCursorAnimation();
        return 0;
      }
    }
    else if (param_2 != 0x111) {
      if (param_2 == 0x112) {
        if (param_3 < 0xf031) {
          if ((((param_3 == 0xf030) || (param_3 == 0xf000)) || (param_3 == 0xf010)) &&
             (DAT_0044de0c == 0)) {
            return 1;
          }
        }
        else {
          if (param_3 == 0xf140) {
            return 0;
          }
          if (param_3 == 0xf170) {
            return 0;
          }
        }
      }
      goto switchD_00402107_caseD_4;
    }
    uVar5 = param_3 & 0xffff;
    if (uVar5 == 0x3e9) {
      DAT_0044ddb0 = 1;
      return 0;
    }
    if (uVar5 == 0x3ea) {
      if (DAT_0044de0c != 0) {
        GetWindowRect(param_1,(LPRECT)&DAT_0044ddf8);
      }
      DAT_0044de0c = (uint)(DAT_0044de0c == 0);
      iVar6 = CreateOrResetDisplayManager(param_1,DAT_0044de0c);
      puVar1 = DAT_0044de08;
      if (iVar6 < 0) {
        if (DAT_0044de08 != (undefined4 *)0x0) {
          DisplayManagerDestructor(DAT_0044de08);
          FUN_0042fbdc((undefined *)puVar1);
          DAT_0044de08 = (undefined4 *)0x0;
        }
        PostMessageA(param_1,0x10,0,0);
      }
      return 0;
    }
    if (((uVar5 == 0x9c41) && (DAT_0044dda0 == 0)) && ((DAT_0051c2c0 == 0 && (DAT_0051c2bc == 0))))
    {
      (**(code **)(*DAT_0051c298 + 0x1c))(DAT_0051c298,0,0,DAT_0044de08[3],0,0);
      if (DAT_0051c2bc == 0) {
        if (DAT_004fbe68 == DAT_004fbf88) {
          DAT_0051b36c = (int *)0x0;
        }
        else {
          DAT_0051b36c = DAT_004fbe68;
          SetCursorSurface((int *)0x0);
        }
        StopAllManagedSounds(DAT_0044ddd8);
        PlayManagedSoundById(DAT_0044ddd8,0x23c,0x32,1);
        DAT_0051c2bc = DAT_0051c2bc + 1;
      }
      else {
        if (DAT_0051b36c != (int *)0x0) {
          SetCursorSurface(DAT_0051b36c);
        }
        DAT_0051c2bc = DAT_0051c2bc + 1;
      }
    }
  }
  else if ((param_2 == 0x212) || (param_2 == 0x232)) {
    DAT_0044ddd4 = timeGetTime();
  }
switchD_00402107_caseD_4:
  LVar7 = DefWindowProcA(param_1,param_2,param_3,param_4);
  return LVar7;
}

