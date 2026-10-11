/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00401700; function: UpdateContextualHelpMode; body bytes: 1816
 * callers: 1; callees: 4; success: True
 */


void UpdateContextualHelpMode(void)

{
  int iVar1;
  int *piVar2;
  undefined4 uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  int iStack_28;
  int iStack_24;
  int iStack_20;
  
  iStack_20 = 0;
  iStack_24 = 0;
  iStack_28 = DAT_0051c298;
  (**(code **)(**(int **)(DAT_0044de08 + 0xc) + 0x1c))(*(int **)(DAT_0044de08 + 0xc),0,0);
  iVar5 = DAT_0044ddac;
  if (DAT_0044ddac == 0x12) {
    if (7 < DAT_0050a5bc) {
      iVar5 = 0;
      piVar2 = &DAT_0044cd40;
      while ((((DAT_004fbd24 <= piVar2[-2] || (*piVar2 <= DAT_004fbd24)) ||
              (DAT_004fbd30 <= piVar2[-1])) || (piVar2[1] <= DAT_004fbd30))) {
        piVar2 = piVar2 + 5;
        iVar5 = iVar5 + 1;
        if (0x44cd67 < (int)piVar2) {
LAB_004017af:
          if (((300 < DAT_004fbd24) && (DAT_004fbd24 < 0x15a)) &&
             ((0x1a4 < DAT_004fbd30 && (DAT_004fbd30 < 0x1d4)))) {
            if (DAT_0044dda8 == 3) {
              return;
            }
            DAT_0044dda8 = 3;
            PlayHelpVoice(0xa3,1);
            return;
          }
          if (iVar5 != 2) {
            return;
          }
          DAT_0044dda8 = 0xffffffff;
          return;
        }
      }
      if ((iVar5 == 0) && (DAT_004fbfb4 != 0)) goto LAB_00401de7;
      if (DAT_0044dda8 != iVar5) {
        PlayHelpVoice(*(int *)(iVar5 * 0x14 + 0x44cd48),1);
        DAT_0044dda8 = iVar5;
      }
      goto LAB_004017af;
    }
    goto LAB_00401cee;
  }
  if (DAT_0044ddac == 0xc) {
    if (DAT_004fca84 != 0) {
      if ((((0x129 < DAT_004fbd24) && (0x1a4 < DAT_004fbd30)) && (DAT_004fbd24 < 0x159)) &&
         ((DAT_004fbd30 < 0x1d4 && (DAT_0044dda8 != 0x32)))) {
        DAT_0044dda8 = 0x32;
        PlayHelpVoice(200,1);
        return;
      }
      goto LAB_00401cee;
    }
    iVar7 = 0;
    if (DAT_0043ee8c < 1) goto LAB_00401cee;
    piVar2 = &DAT_004fc454;
    do {
      if (piVar2[1] == 0) {
        iStack_24 = *piVar2;
        iStack_28 = piVar2[-1];
        iStack_20 = piVar2[4] + iStack_28;
        uVar3 = IsDinoCursorInsideRect(&iStack_28);
        iVar5 = DAT_0044ddac;
        if (((char)uVar3 != '\0') && (DAT_0044dda8 != iVar7 + 10)) {
          DAT_0044dda8 = iVar7 + 10;
          PlayHelpVoice(199,1);
          return;
        }
      }
      iVar7 = iVar7 + 1;
      piVar2 = piVar2 + 0xc;
    } while (iVar7 < DAT_0043ee8c);
  }
  if (iVar5 == 0xe) {
    uVar3 = IsDinoCursorInsideRect((int *)&DAT_00510940);
    if (((char)uVar3 != '\0') && (DAT_0044dda8 != 0xb)) {
      PlayHelpVoice(0x62,1);
      DAT_0044dda8 = 0xb;
      return;
    }
    uVar3 = IsDinoCursorInsideRect((int *)&DAT_005107f0);
    iVar5 = DAT_0044ddac;
    if (((char)uVar3 != '\0') && (DAT_0044dda8 != 0xc)) {
      PlayHelpVoice(0x61,1);
      DAT_0044dda8 = 0xc;
      return;
    }
  }
  if (iVar5 == 0x10) {
    uVar8 = 0;
    piVar2 = &DAT_00444b18;
    do {
      iStack_24 = *piVar2;
      iStack_28 = piVar2[-1];
      iStack_20 = piVar2[1];
      uVar3 = IsDinoCursorInsideRect(&iStack_28);
      if (((char)uVar3 != '\0') && (DAT_0044dda8 != uVar8 + 10)) {
        uVar4 = uVar8 & 0x80000001;
        if ((int)uVar4 < 0) {
          uVar4 = (uVar4 - 1 | 0xfffffffe) + 1;
        }
        PlayHelpVoice(uVar4 + 0x231,1);
        DAT_0044dda8 = uVar8 + 10;
        return;
      }
      piVar2 = piVar2 + 4;
      uVar8 = uVar8 + 1;
      iVar5 = DAT_0044ddac;
    } while ((int)piVar2 < 0x444bb8);
  }
  if (iVar5 == 0x11) {
    iVar5 = 0;
    piVar2 = &DAT_00443ab4;
    do {
      iStack_28 = piVar2[-1] - DAT_0051071c;
      iStack_20 = iStack_28 + 0x46;
      iStack_24 = *piVar2 - DAT_00510720;
      uVar3 = IsDinoCursorInsideRect(&iStack_28);
      if ((char)uVar3 != '\0') {
        if (DAT_0044dda8 == iVar5 + 10) {
          return;
        }
        PlayHelpVoice(iVar5 + 0x269,1);
        DAT_0044dda8 = iVar5 + 10;
        return;
      }
      piVar2 = piVar2 + 2;
      iVar5 = iVar5 + 1;
    } while ((int)piVar2 < 0x443acc);
    uVar3 = IsDinoCursorInsideRect((int *)&DAT_00510590);
    if (((char)uVar3 != '\0') && (DAT_0044dda8 != 0x1e)) {
      PlayHelpVoice(0x26c,1);
      DAT_0044dda8 = 0x1e;
      return;
    }
    uVar3 = IsDinoCursorInsideRect((int *)&DAT_00510618);
    if (((char)uVar3 != '\0') && (DAT_0044dda8 != 0x28)) {
      PlayHelpVoice(0x271,1);
      DAT_0044dda8 = 0x28;
      return;
    }
    iStack_28 = 0xd0 - DAT_0051071c;
    iStack_24 = 0x14c - DAT_00510720;
    iStack_20 = 0x17f - DAT_0051071c;
    uVar3 = IsDinoCursorInsideRect(&iStack_28);
    if (((char)uVar3 != '\0') && (DAT_0044dda8 != 0x1f)) {
      PlayHelpVoice(0x268,1);
      DAT_0044dda8 = 0x1f;
      return;
    }
    iStack_28 = 0x1dc - DAT_0051071c;
    iStack_24 = 0x5f - DAT_00510720;
    iStack_20 = 0x306 - DAT_0051071c;
    uVar3 = IsDinoCursorInsideRect(&iStack_28);
    if (((char)uVar3 != '\0') && (DAT_0044dda8 != 0x20)) {
      PlayHelpVoice(0x26f,1);
      DAT_0044dda8 = 0x20;
      return;
    }
    iStack_28 = 0x307 - DAT_0051071c;
    iStack_24 = 0x25 - DAT_00510720;
    iStack_20 = 0x3ce - DAT_0051071c;
    uVar3 = IsDinoCursorInsideRect(&iStack_28);
    if (((char)uVar3 != '\0') && (DAT_0044dda8 != 0x21)) {
      PlayHelpVoice(0x26e,1);
      DAT_0044dda8 = 0x21;
      return;
    }
    iStack_28 = 0x3d5 - DAT_0051071c;
    iStack_24 = 0xca - DAT_00510720;
    iStack_20 = 0x490 - DAT_0051071c;
    uVar3 = IsDinoCursorInsideRect(&iStack_28);
    iVar5 = DAT_0044ddac;
    if (((char)uVar3 != '\0') && (DAT_0044dda8 != 0x22)) {
      PlayHelpVoice(0x26d,1);
      DAT_0044dda8 = 0x22;
      return;
    }
  }
LAB_00401cee:
  iVar6 = 0;
  iVar7 = DAT_00507b5c;
  if ((int)(&DAT_0044a2a4)[iVar5] < 1) {
    return;
  }
  do {
    iVar1 = iVar6 + iVar5 * 0x1e;
    if (((((int)(&DAT_0044a308)[iVar1 * 5] < DAT_004fbd24) &&
         (DAT_004fbd24 < (int)(&DAT_0044a310)[iVar1 * 5])) &&
        ((int)(&DAT_0044a30c)[iVar1 * 5] < DAT_004fbd30)) &&
       (DAT_004fbd30 < (int)(&DAT_0044a314)[iVar1 * 5])) {
      if ((iVar6 == 0) && (DAT_004fbfb4 != 0)) {
LAB_00401de7:
        DAT_0044dda0 = 0;
        DAT_004fbe54 = 0;
        StopAllManagedSounds(DAT_0044ddd8);
        SetCursorSurface((int *)0x0);
        DAT_0051c31c = 0x32;
        return;
      }
      if (DAT_0044dda8 != iVar6) {
        if (iVar5 == 9) {
          if (iVar7 == 3) {
            if (((iVar6 != 7) && (iVar6 != 9)) && (iVar6 != 6)) goto LAB_00401d87;
          }
          else if (iVar6 < 10) goto LAB_00401d87;
        }
        else {
LAB_00401d87:
          if ((iVar6 == 2) && (iVar5 == 0xc)) {
            if (DAT_004fca84 < 1) {
LAB_00401dad:
              PlayHelpVoice((&DAT_0044a318)[iVar1 * 5],1);
              iVar5 = DAT_0044ddac;
              iVar7 = DAT_00507b5c;
              DAT_0044dda8 = iVar6;
            }
          }
          else if ((iVar5 != 9) || ((iVar6 != 9 || ((&DAT_00508bf4)[iVar7] != 0))))
          goto LAB_00401dad;
        }
      }
    }
    iVar6 = iVar6 + 1;
    if ((int)(&DAT_0044a2a4)[iVar5] <= iVar6) {
      return;
    }
  } while( true );
}

