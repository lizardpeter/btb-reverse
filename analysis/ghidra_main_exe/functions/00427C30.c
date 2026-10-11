/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00427c30; function: UpdateProgressScreen; body bytes: 991
 * callers: 1; callees: 6; success: True
 */


void UpdateProgressScreen(void)

{
  int *piVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  uint uVar5;
  char cVar6;
  int *piVar7;
  bool bVar8;
  undefined4 uVar9;
  int iVar10;
  
  piVar1 = *(int **)(DAT_0044de08 + 0xc);
  if ((DAT_0051c330 == 1) && (iVar2 = AnyManagedSoundPlaying(DAT_0044ddd8), iVar2 == 0)) {
    DAT_0051c330 = iVar2;
    PlayManagedSoundById(DAT_0044ddd8,0x3ba,0x32,2);
    *(undefined4 *)((int)DAT_0044ddd8 + *(char *)((int)DAT_0044ddd8 + 0x63a) * 4 + 0xd98) = 1;
  }
  (**(code **)(*piVar1 + 0x1c))(piVar1,0,0,DAT_0051bca0);
  UpdateHelpButtonController();
  UpdateGenericHelpHoverVoice();
  iVar2 = 0x32;
  piVar7 = &DAT_00446fa4;
  do {
    uVar3 = IsDinoCursorInsideRect((int *)&stack0xffffffd0);
    if ((char)uVar3 != '\0') {
      if (((piVar7 == (int *)&DAT_00446fb8) || (piVar7 == &DAT_00446fa8)) ||
         (piVar7 == &DAT_00446fa4)) {
        if (DAT_00446fe0 != iVar2) {
          if ((int)(&DAT_0051b4d0)[iVar2 + DAT_00519934 * 100] < 1) {
LAB_00427ddc:
            iVar10 = 2;
            uVar3 = 0x32;
            iVar4 = 0x3bd;
          }
          else {
LAB_00427f8e:
            iVar10 = 2;
            uVar3 = 0x32;
            uVar5 = FUN_0042ffc4();
            uVar5 = uVar5 & 0x80000001;
            if ((int)uVar5 < 0) {
              uVar5 = (uVar5 - 1 | 0xfffffffe) + 1;
            }
            iVar4 = uVar5 + 0x3bb;
          }
LAB_00427de1:
          PlayManagedSoundById(DAT_0044ddd8,iVar4,uVar3,iVar10);
          DAT_00446fe0 = iVar2;
        }
      }
      else if ((piVar7 == (int *)&DAT_00446fc8) || (piVar7 == (int *)&DAT_00446fcc)) {
        if (DAT_00446fe0 != iVar2) {
          iVar4 = *(int *)(&DAT_0051b5bc + DAT_00519934 * 400);
          iVar10 = *(int *)(&DAT_0051b5c0 + DAT_00519934 * 400);
joined_r0x00427f58:
          cVar6 = 0 < iVar4;
          if (0 < iVar10) {
            cVar6 = cVar6 + '\x01';
          }
          if (cVar6 != '\0') {
            bVar8 = cVar6 == '\x01';
LAB_00427dd2:
            if (!bVar8) goto LAB_00427f8e;
            goto LAB_00427ddc;
          }
          iVar10 = 2;
          uVar3 = 0x32;
          iVar4 = 0x3be;
          goto LAB_00427de1;
        }
      }
      else if ((piVar7 == (int *)&DAT_00446fd0) || (piVar7 == (int *)&DAT_00446fd4)) {
        if (DAT_00446fe0 != iVar2) {
          iVar4 = *(int *)(&DAT_0051b5c4 + DAT_00519934 * 400);
          iVar10 = *(int *)(&DAT_0051b5c8 + DAT_00519934 * 400);
          goto joined_r0x00427f58;
        }
      }
      else if (((piVar7 == (int *)&DAT_00446fbc) || (piVar7 == (int *)&DAT_00446fc0)) ||
              (piVar7 == (int *)&DAT_00446fc4)) {
        if (DAT_00446fe0 != iVar2) {
          iVar4 = DAT_00519934 * 400;
          cVar6 = 0 < *(int *)(&DAT_0051b5b0 + iVar4);
          if (0 < *(int *)(&DAT_0051b5b4 + iVar4)) {
            cVar6 = cVar6 + '\x01';
          }
          if (0 < *(int *)(&DAT_0051b5b8 + iVar4)) {
            cVar6 = cVar6 + '\x01';
          }
          if (cVar6 == '\0') {
            iVar10 = 2;
            uVar3 = 0x32;
            iVar4 = 0x3bf;
          }
          else {
            if (cVar6 != '\x01') goto LAB_00427dcf;
            iVar10 = 2;
            uVar3 = 0x32;
            iVar4 = 0x3be;
          }
          goto LAB_00427de1;
        }
      }
      else if ((((piVar7 == (int *)&DAT_00446fac) || (piVar7 == (int *)&DAT_00446fb0)) ||
               (piVar7 == (int *)&DAT_00446fb4)) && (DAT_00446fe0 != iVar2)) {
        iVar4 = DAT_00519934 * 400;
        cVar6 = 0 < *(int *)(&DAT_0051b5a0 + iVar4);
        if (0 < *(int *)(&DAT_0051b5a4 + iVar4)) {
          cVar6 = cVar6 + '\x01';
        }
        if (0 < *(int *)(&DAT_0051b5a8 + iVar4)) {
          cVar6 = cVar6 + '\x01';
        }
        if (cVar6 == '\0') {
          iVar10 = 2;
          uVar3 = 0x32;
          iVar4 = 0x3bf;
        }
        else {
          if (cVar6 != '\x01') {
LAB_00427dcf:
            bVar8 = cVar6 == '\x02';
            goto LAB_00427dd2;
          }
          iVar10 = 2;
          uVar3 = 0x32;
          iVar4 = 0x3be;
        }
        goto LAB_00427de1;
      }
    }
    if (0 < (int)(&DAT_0051b4d0)[iVar2 + DAT_00519934 * 100]) {
      (**(code **)(*piVar1 + 0x1c))
                (piVar1,*(undefined4 *)(*piVar7 * 8 + 0x446f3c),
                 *(undefined4 *)(*piVar7 * 8 + 0x446f40),DAT_0051a664,0,1);
    }
    piVar7 = piVar7 + 1;
    iVar2 = iVar2 + 1;
  } while ((int)piVar7 < 0x446fdd);
  if ((DAT_004fbfbc & 4) == 0) {
    if ((DAT_004fbfbc & 8) == 0) goto LAB_00427fec;
    if (DAT_004fbfb8 != 0) {
      (**(code **)(*piVar1 + 0x1c))(piVar1,0xe,0x1a2,DAT_0051b3cc,0,0);
      goto LAB_00427fec;
    }
    iVar2 = *piVar1;
    uVar9 = 0xe;
    uVar3 = DAT_0051b3c8;
  }
  else {
    iVar2 = *piVar1;
    if (DAT_004fbfb8 == 0) {
      uVar9 = 0x23e;
      uVar3 = DAT_0051b3d8;
    }
    else {
      uVar9 = 0x23e;
      uVar3 = DAT_0051b3dc;
    }
  }
  (**(code **)(iVar2 + 0x1c))(piVar1,uVar9,0x1a2,uVar3,0,0);
LAB_00427fec:
  if (((DAT_004fbfbc & 4) != 0) && (DAT_004fbfb4 != 0)) {
    DAT_0051c324 = 0;
  }
  return;
}

