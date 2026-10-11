/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00428520; function: PointInPolygon; body bytes: 212
 * callers: 5; callees: 0; success: True
 */


bool __cdecl PointInPolygon(int *param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  uint local_c;
  int local_8;
  
  local_c = 0;
  iVar5 = *param_1;
  iVar6 = param_1[1];
  local_8 = 1;
  if (0 < param_2) {
    do {
      iVar3 = local_8 % param_2;
      iVar1 = param_1[iVar3 * 2 + 1];
      iVar2 = param_1[iVar3 * 2];
      iVar4 = iVar6;
      if (iVar1 <= iVar6) {
        iVar4 = iVar1;
      }
      if (iVar4 < param_4) {
        iVar4 = iVar6;
        if (iVar6 <= iVar1) {
          iVar4 = iVar1;
        }
        if (param_4 <= iVar4) {
          iVar4 = iVar5;
          if (iVar5 <= iVar2) {
            iVar4 = iVar2;
          }
          if (((param_3 <= iVar4) && (iVar6 != iVar1)) &&
             ((iVar5 == iVar2 ||
              ((float10)param_3 <=
               (float10)(((iVar2 - iVar5) * (param_4 - iVar6)) / (iVar1 - iVar6) + iVar5))))) {
            local_c = local_c + 1;
          }
        }
      }
      iVar5 = param_1[iVar3 * 2];
      iVar6 = (param_1 + iVar3 * 2)[1];
      local_8 = local_8 + 1;
    } while (local_8 <= param_2);
  }
  local_c = local_c & 0x80000001;
  if ((int)local_c < 0) {
    local_c = (local_c - 1 | 0xfffffffe) + 1;
  }
  return (bool)('\x01' - (local_c != 0));
}

