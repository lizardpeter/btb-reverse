/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00406ad0; function: UpdateCreditsSlideshow; body bytes: 294
 * callers: 1; callees: 4; success: True
 */


void UpdateCreditsSlideshow(void)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  undefined8 uVar4;
  _SYSTEMTIME local_30;
  _SYSTEMTIME local_20;
  _SYSTEMTIME local_10;
  
  GetSystemTime(&local_30);
  SystemTimeToFileTime(&local_30,(LPFILETIME)&DAT_005120f0);
  uVar4 = Divide64BitDeltaByTenMillion((uint *)&DAT_00510d10,(uint *)&DAT_005120f0);
  DAT_005109c8 = (DAT_00510950 + DAT_00510d08) - (int)uVar4;
  if ((DAT_004fbe54 != 0) || (DAT_005109c8 < 0)) {
    DAT_00510d08 = 4;
    GetSystemTime(&local_20);
    GetSystemTime(&local_10);
    SystemTimeToFileTime(&local_20,(LPFILETIME)&DAT_00510d10);
    SystemTimeToFileTime(&local_10,(LPFILETIME)&DAT_005120f0);
    Divide64BitDeltaByTenMillion((uint *)&DAT_00510d10,(uint *)&DAT_005120f0);
    DAT_004fbd0c = DAT_004fbd0c + 1;
    if (DAT_004fbd04 <= DAT_004fbd0c) {
      iVar3 = 0;
      DAT_0044ddb0 = DAT_0044ddb0 + 1;
      if (DAT_004fbd04 < 1) {
        return;
      }
      piVar2 = &DAT_0048d5c8;
      do {
        UnregisterBitmapSurface((int)piVar2);
        piVar1 = (int *)*piVar2;
        if (piVar1 != (int *)0x0) {
          (**(code **)(*piVar1 + 8))(piVar1);
          *piVar2 = 0;
        }
        iVar3 = iVar3 + 1;
        piVar2 = piVar2 + 1;
      } while (iVar3 < DAT_004fbd04);
      return;
    }
  }
  (**(code **)(**(int **)(DAT_0044de08 + 0xc) + 0x1c))
            (*(int **)(DAT_0044de08 + 0xc),0,0,(&DAT_0048d5c8)[DAT_004fbd0c],0,0);
  return;
}

