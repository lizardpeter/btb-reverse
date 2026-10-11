/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004038d0; function: LoadBitmapToDirectDrawSurface; body bytes: 414
 * callers: 26; callees: 6; success: True
 */


int * __cdecl LoadBitmapToDirectDrawSurface(int *param_1,LPCSTR param_2,int param_3,int param_4)

{
  char cVar1;
  HANDLE h;
  int iVar2;
  uint uVar3;
  uint uVar4;
  char *pcVar5;
  int *unaff_EDI;
  undefined4 *puVar6;
  char *pcVar7;
  char *pcVar8;
  int *piVar9;
  undefined1 auStack_fc [4];
  undefined1 auStack_f8 [4];
  int iStack_f4;
  undefined4 auStack_f0 [4];
  int aiStack_e0 [22];
  undefined4 uStack_88;
  int iStack_78;
  undefined4 uStack_64;
  undefined4 auStack_60 [24];
  
  UpdateLoadingCursorAnimation();
  h = LoadImageA((HINSTANCE)0x0,param_2,0,param_3,param_4,0x2010);
  if (h == (HANDLE)0x0) {
    uStack_64 = DAT_0043e14c;
    puVar6 = auStack_60;
    for (iVar2 = 0x18; iVar2 != 0; iVar2 = iVar2 + -1) {
      *puVar6 = 0;
      puVar6 = puVar6 + 1;
    }
    if (DAT_0043e874 != -1) {
      uStack_64 = CONCAT31((int3)((uint)DAT_0043e14c >> 8),(char)DAT_0043e874 + 'a');
      uVar3 = 0xffffffff;
      do {
        pcVar5 = param_2;
        if (uVar3 == 0) break;
        uVar3 = uVar3 - 1;
        pcVar5 = param_2 + 1;
        cVar1 = *param_2;
        param_2 = pcVar5;
      } while (cVar1 != '\0');
      uVar3 = ~uVar3;
      iVar2 = -1;
      pcVar8 = (char *)&uStack_64;
      do {
        pcVar7 = pcVar8;
        if (iVar2 == 0) break;
        iVar2 = iVar2 + -1;
        pcVar7 = pcVar8 + 1;
        cVar1 = *pcVar8;
        pcVar8 = pcVar7;
      } while (cVar1 != '\0');
      pcVar5 = pcVar5 + -uVar3;
      pcVar8 = pcVar7 + -1;
      for (uVar4 = uVar3 >> 2; uVar4 != 0; uVar4 = uVar4 - 1) {
        *(undefined4 *)pcVar8 = *(undefined4 *)pcVar5;
        pcVar5 = pcVar5 + 4;
        pcVar8 = pcVar8 + 4;
      }
      for (uVar3 = uVar3 & 3; uVar3 != 0; uVar3 = uVar3 - 1) {
        *pcVar8 = *pcVar5;
        pcVar5 = pcVar5 + 1;
        pcVar8 = pcVar8 + 1;
      }
      h = LoadImageA((HINSTANCE)0x0,(LPCSTR)&uStack_64,0,param_3,param_4,0x2010);
      if (h != (HANDLE)0x0) goto LAB_004039a1;
    }
    AbortForRemovedRetailCD();
    return (int *)0x0;
  }
LAB_004039a1:
  GetObjectA(h,0x18,auStack_f8);
  piVar9 = aiStack_e0;
  for (iVar2 = 0x1f; iVar2 != 0; iVar2 = iVar2 + -1) {
    *piVar9 = 0;
    piVar9 = piVar9 + 1;
  }
  aiStack_e0[3] = iStack_f4;
  aiStack_e0[2] = auStack_f0[0];
  iStack_78 = ((iStack_f4 < 0x7d1) - 1 & 0x7c0) + 0x40;
  aiStack_e0[0] = 0x7c;
  aiStack_e0[1] = 7;
  iVar2 = (**(code **)(*param_1 + 0x18))(param_1,aiStack_e0,auStack_fc,0);
  if (iVar2 != 0) {
    uStack_88 = 0x800;
    iVar2 = (**(code **)(*param_1 + 0x18))(param_1,auStack_f0,&stack0xfffffef4,0);
    if (iVar2 != 0) {
      return (int *)0x0;
    }
  }
  CopyBitmapToSurface(unaff_EDI,h);
  DeleteObject(h);
  return unaff_EDI;
}

