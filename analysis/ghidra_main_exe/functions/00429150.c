/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00429150; function: LoadGenericUIScreenResources; body bytes: 890
 * callers: 1; callees: 9; success: True
 */


void LoadGenericUIScreenResources(void)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  uint uVar6;
  char local_6c [8];
  CHAR local_64 [100];
  
  piVar1 = *(int **)(DAT_0044de08 + 4);
  BeginLoadingCursorAnimation();
  iVar5 = 0;
  DAT_00446cec = 0xffffffff;
  iVar3 = DAT_0051c27c;
  if (0 < (int)(&DAT_0048fb40)[DAT_0051c27c]) {
    do {
      uVar6 = 0;
      iVar2 = (iVar3 * 0x20 + iVar5) * 0x40c;
      *(undefined4 *)(&DAT_00494824 + iVar2) = 8;
      if (0 < *(int *)(&DAT_00494714 + iVar2)) {
        do {
          if ((int)uVar6 < 1) {
            crt_sprintf(local_64,&DAT_0043e15c);
          }
          else {
            FUN_00430f2f(uVar6,local_6c,10);
            crt_sprintf(local_64,(byte *)s__s_s_s_00447044);
          }
          DAT_0051c274 = DAT_0051c274 + 1;
          iVar3 = DAT_0051c27c * 0x20;
          piVar4 = LoadBitmapToDirectDrawSurface(piVar1,local_64,0,0);
          *(int **)(&DAT_00494838 + (iVar3 + iVar5) * 0x40c + uVar6 * 4) = piVar4;
          RegisterBitmapSurface(&DAT_00494838 + (iVar3 + iVar5) * 0x40c + uVar6 * 4,local_64);
          uVar6 = uVar6 + 1;
          iVar3 = DAT_0051c27c;
        } while ((int)uVar6 < *(int *)(&DAT_00494714 + (DAT_0051c27c * 0x20 + iVar5) * 0x40c));
      }
      uVar6 = 0;
      if (0 < *(int *)(&DAT_004947a0 + (iVar3 * 0x20 + iVar5) * 0x40c)) {
        do {
          if ((int)uVar6 < 1) {
            crt_sprintf(local_64,&DAT_0043e15c);
          }
          else {
            FUN_00430f2f(uVar6,local_6c,10);
            crt_sprintf(local_64,(byte *)s__s_s_s_00447044);
          }
          iVar3 = DAT_0051c27c * 0x20;
          piVar4 = LoadBitmapToDirectDrawSurface(piVar1,local_64,0,0);
          *(int **)(&DAT_004948d8 + (iVar3 + iVar5) * 0x40c + uVar6 * 4) = piVar4;
          RegisterBitmapSurface(&DAT_004948d8 + (iVar3 + iVar5) * 0x40c + uVar6 * 4,local_64);
          uVar6 = uVar6 + 1;
          iVar3 = DAT_0051c27c;
        } while ((int)uVar6 < *(int *)(&DAT_004947a0 + (DAT_0051c27c * 0x20 + iVar5) * 0x40c));
      }
      iVar2 = (iVar3 * 0x20 + iVar5) * 0x40c;
      *(undefined4 *)(&DAT_0049479c + iVar2) = 0;
      if ((&DAT_00494610)[iVar2] == '\0') {
        *(undefined4 *)(&DAT_00494834 + iVar2) = 0;
      }
      else {
        iVar3 = (iVar3 * 0x20 + iVar5) * 0x40c;
        piVar4 = LoadBitmapToDirectDrawSurface(piVar1,&DAT_00494610 + iVar2,0,0);
        *(int **)(&DAT_00494834 + iVar3) = piVar4;
        RegisterBitmapSurface(&DAT_00494834 + iVar3,&DAT_00494610 + iVar3);
        MarkRegisteredSurfaceColorKeyed
                  ((int)(&DAT_00494834 + (DAT_0051c27c * 0x20 + iVar5) * 0x40c));
        SetSurfaceTransparencyColorKey
                  (*(int **)(&DAT_00494834 + (DAT_0051c27c * 0x20 + iVar5) * 0x40c),0xff00ff);
        iVar3 = DAT_0051c27c;
      }
      iVar2 = (iVar3 * 0x20 + iVar5) * 0x40c;
      if ((&DAT_00494718)[iVar2] == '\0') {
        *(undefined4 *)(&DAT_00494978 + iVar2) = 0;
      }
      else {
        iVar3 = (iVar3 * 0x20 + iVar5) * 0x40c;
        piVar4 = LoadBitmapToDirectDrawSurface(piVar1,&DAT_00494718 + iVar2,0,0);
        *(int **)(&DAT_00494978 + iVar3) = piVar4;
        RegisterBitmapSurface(&DAT_00494978 + iVar3,&DAT_00494718 + iVar3);
        iVar3 = DAT_0051c27c;
      }
      iVar5 = iVar5 + 1;
    } while (iVar5 < (int)(&DAT_0048fb40)[iVar3]);
  }
  NoOpLegacyHook();
  EndLoadingCursorAnimation();
  return;
}

