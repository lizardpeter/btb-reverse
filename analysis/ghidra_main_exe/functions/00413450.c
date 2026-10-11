/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00413450; function: UpdateFireworksShowPlayback; body bytes: 1102
 * callers: 1; callees: 13; success: True
 */


void UpdateFireworksShowPlayback(void)

{
  bool bVar1;
  int iVar2;
  uint uVar3;
  undefined3 extraout_var;
  int iVar4;
  int iVar5;
  int *piVar6;
  int *piVar7;
  int *piVar8;
  undefined4 uVar9;
  int iVar10;
  int *piVar11;
  
  GetSystemTime((LPSYSTEMTIME)&DAT_00512598);
  SystemTimeToFileTime((SYSTEMTIME *)&DAT_00512598,(LPFILETIME)&DAT_005120f0);
  DAT_005109c8 = FileTimeDeltaCentiseconds((int *)&DAT_00510d10,(int *)&DAT_005120f0);
  FileTimeDeltaMilliseconds((int *)&DAT_00510d10,(int *)&DAT_005120f0);
  iVar5 = DAT_005109c8 / 400;
  piVar7 = *(int **)(DAT_0044de08 + 0xc);
  piVar11 = (int *)0x0;
  iVar10 = 0;
  (**(code **)(*piVar7 + 0x1c))(piVar7,0,0,DAT_0050ab10);
  if (DAT_0050ab6c < 3) {
    DecodeAndBlitBinkFrame(iVar5,DAT_0050ab00,DAT_005093d0);
    if (DAT_0050ab00[2] == DAT_0050ab00[3]) {
      DAT_0050ab6c = DAT_0050ab6c + 1;
      RestartBinkMovie(DAT_0050ab00);
    }
    iVar2 = AnyManagedSoundPlaying(DAT_0044ddd8);
    if ((iVar2 == 0) && (uVar3 = FUN_0042ffc4(), (int)uVar3 % 10 == 0)) {
      iVar2 = 2;
      uVar9 = 0x32;
      uVar3 = FUN_0042ffc4();
      PlayManagedSoundById(DAT_0044ddd8,(int)uVar3 % 0x19 + 0x143,uVar9,iVar2);
    }
  }
  else {
    bVar1 = IsSoundIdPlaying(DAT_0044ddd8,0x15d);
    if (CONCAT31(extraout_var,bVar1) == 0) {
      StopAllManagedSounds(DAT_0044ddd8);
      PlayManagedSoundById(DAT_0044ddd8,0x15d,0x32,1);
    }
    DecodeAndBlitBinkFrame(iVar5,DAT_0050ab04,DAT_005093d4);
    if (DAT_0050ab04[2] == DAT_0050ab04[3]) {
      DAT_0050ab6c = 7;
    }
  }
  if (1 < iVar5) {
    if (DAT_0050ab6c == 7) {
      StopAllManagedSounds(DAT_0044ddd8);
      DAT_0050a5bc = 0x10;
      CSound_Reset(DAT_004fc0ac);
      CSound_Stop(DAT_004fc0ac);
      (**(code **)(*piVar7 + 0x1c))(piVar7,0,0,DAT_0050a4b4,0,1);
      return;
    }
    (**(code **)(*piVar7 + 0x1c))(piVar7,0,0,DAT_0050a4b4,0,1);
  }
  iVar2 = DAT_0051218c / 400;
  iVar4 = DAT_005109c8 / 400;
  if (((iVar2 != iVar4) && (iVar2 != iVar4)) && (DAT_0050ab60 = 1, iVar2 != iVar4)) {
    DAT_0050ab64 = 1;
  }
  if ((DAT_0050ab60 != 0) && (99 < DAT_005109c8 % 400)) {
    DAT_0050ab60 = 0;
  }
  if ((DAT_0050ab64 != 0) && (199 < DAT_005109c8 % 400)) {
    DAT_0050ab64 = 0;
  }
  DAT_0051218c = DAT_005109c8;
  piVar7 = &DAT_005093fc;
  do {
    if (0 < DAT_0050ab38) {
      DAT_0050ab38 = DAT_0050ab38 + 1;
    }
    iVar2 = piVar7[-3];
    if (iVar2 != -1) {
      iVar4 = *piVar7;
      if ((((iVar4 == 2) || (iVar4 == 0)) || (iVar4 == 1)) && (iVar4 == 1)) {
        iVar2 = iVar2 + 0xc;
      }
      DAT_0050ab3c = DAT_0050ab3c + 1;
      iVar4 = DecodeAndBlitBinkFrame(iVar5,(int *)(&DAT_0050aaac)[iVar2],(&DAT_0050937c)[iVar2]);
      if (iVar4 != 0) {
        RestartBinkMovie((&DAT_0050aaac)[iVar2]);
        piVar7[-3] = -1;
      }
    }
    piVar7 = piVar7 + 4;
  } while ((int)piVar7 < 0x50967c);
  DAT_00442a2c = 0;
  iVar5 = DecodeAndBlitBinkFrame(iVar5,DAT_0050aafc,DAT_005093cc);
  if (iVar5 != 0) {
    RestartBinkMovie(DAT_0050aafc);
  }
  if (iVar10 < 6) {
    iVar5 = 0;
    piVar8 = (int *)&stack0xffffffdc;
    piVar7 = &DAT_0050a678 + iVar10;
    do {
      if (((*piVar8 != 0) && (iVar10 < 0x14)) && ((iVar2 = *piVar7, -1 < iVar2 && (iVar2 < 0xc)))) {
        iVar4 = 0;
        piVar6 = &DAT_005093f0;
        do {
          if (*piVar6 == -1) {
            (&DAT_005093f0)[iVar4 * 4] = iVar2;
            (&DAT_005093f4)[iVar4 * 4] = 0;
            (&DAT_005093f8)[iVar4 * 4] = 0;
            (&DAT_005093fc)[iVar4 * 4] = iVar5;
            DAT_0050ab38 = DAT_0050ab38 + 1;
            break;
          }
          piVar6 = piVar6 + 4;
          iVar4 = iVar4 + 1;
        } while ((int)piVar6 < 0x509670);
      }
      iVar5 = iVar5 + 1;
      piVar8 = piVar8 + 1;
      piVar7 = piVar7 + 6;
      DAT_00442a00 = 0;
    } while (iVar5 < 3);
  }
  (**(code **)(*piVar11 + 0x1c))(piVar11,0,0,DAT_0050a4b4,0,1);
  return;
}

