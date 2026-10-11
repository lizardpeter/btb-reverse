/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0042ffe2; function: FUN_0042ffe2; body bytes: 220
 * callers: 1; callees: 4; success: True
 */


void __cdecl FUN_0042ffe2(int *param_1)

{
  DWORD DVar1;
  int iVar2;
  _TIME_ZONE_INFORMATION local_d0;
  _SYSTEMTIME local_24;
  _SYSTEMTIME local_14;
  
  GetLocalTime(&local_14);
  GetSystemTime(&local_24);
  if (local_24.wMinute == DAT_0051c3b8._2_2_) {
    if (local_24.wHour == (WORD)DAT_0051c3b8) {
      if (local_24.wDay == DAT_0051c3b4._2_2_) {
        if (local_24.wMonth == DAT_0051c3b0._2_2_) {
          if (local_24.wYear == (WORD)DAT_0051c3b0) goto LAB_0043008c;
        }
      }
    }
  }
  DVar1 = GetTimeZoneInformation(&local_d0);
  if (DVar1 == 0xffffffff) {
    DAT_0051c3a8 = -1;
  }
  else if (((DVar1 == 2) && (local_d0.DaylightDate.wMonth != 0)) && (local_d0.DaylightBias != 0)) {
    DAT_0051c3a8 = 1;
  }
  else {
    DAT_0051c3a8 = 0;
  }
  DAT_0051c3b0._0_2_ = local_24.wYear;
  DAT_0051c3b0._2_2_ = local_24.wMonth;
  DAT_0051c3b4._0_2_ = local_24.wDayOfWeek;
  DAT_0051c3b4._2_2_ = local_24.wDay;
  DAT_0051c3b8._0_2_ = local_24.wHour;
  DAT_0051c3b8._2_2_ = local_24.wMinute;
  DAT_0051c3bc._0_2_ = local_24.wSecond;
  DAT_0051c3bc._2_2_ = local_24.wMilliseconds;
LAB_0043008c:
  iVar2 = FUN_004326b8((uint)local_14.wYear,(uint)local_14.wMonth,(uint)local_14.wDay,
                       (uint)local_14.wHour,(uint)local_14.wMinute,(uint)local_14.wSecond,
                       DAT_0051c3a8);
  if (param_1 != (int *)0x0) {
    *param_1 = iVar2;
  }
  return;
}

