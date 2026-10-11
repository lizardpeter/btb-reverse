/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00429500; function: UpdatePlayAgainPrompt; body bytes: 759
 * callers: 1; callees: 4; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void UpdatePlayAgainPrompt(void)

{
  int iVar1;
  bool bVar2;
  
  if (DAT_0051c2cc < 2) {
    (**(code **)(**(int **)(DAT_0044de08 + 0xc) + 0x1c))
              (*(int **)(DAT_0044de08 + 0xc),0,0,DAT_0051c29c,0,0);
    if (DAT_004fbe54 != 0) {
      iVar1 = PointInRectInclusive(DAT_004fbd24,DAT_004fbd30,(int *)&DAT_0051a670);
      if (iVar1 == 0) {
        iVar1 = PointInRectInclusive(DAT_004fbd24,DAT_004fbd30,(int *)&DAT_0051a680);
        if (iVar1 == 0) {
          iVar1 = PointInRectInclusive(DAT_004fbd24,DAT_004fbd30,(int *)&DAT_0051a690);
          if (iVar1 == 0) {
            iVar1 = PointInRectInclusive(DAT_004fbd24,DAT_004fbd30,(int *)&DAT_0051a6a0);
            if (iVar1 == 0) {
              iVar1 = PointInRectInclusive(DAT_004fbd24,DAT_004fbd30,(int *)&DAT_0051a6b4);
              if (iVar1 == 0) {
                iVar1 = PointInRectInclusive(DAT_004fbd24,DAT_004fbd30,(int *)&DAT_0051a6cc);
                if (iVar1 != 0) {
                  DAT_00446cec = 6;
                }
              }
              else {
                DAT_00446cec = 2;
              }
            }
            else {
              DAT_00446cec = 5;
            }
          }
          else {
            DAT_00446cec = 4;
          }
        }
        else {
          DAT_00446cec = 3;
        }
      }
      else {
        DAT_00446cec = 1;
      }
    }
    if (DAT_004fbfb4 != 0) {
      iVar1 = PointInRectInclusive(DAT_004fbd24,DAT_004fbd30,(int *)&DAT_0051a670);
      if (iVar1 == 0) {
        iVar1 = PointInRectInclusive(DAT_004fbd24,DAT_004fbd30,(int *)&DAT_0051a680);
        if (iVar1 != 0) {
          if (DAT_00446cec != 3) {
            return;
          }
          DAT_0051c2cc = 0;
          _DAT_0051c2ec = 3;
          DAT_00446cec = 0xffffffff;
          return;
        }
        iVar1 = PointInRectInclusive(DAT_004fbd24,DAT_004fbd30,(int *)&DAT_0051a690);
        if (iVar1 != 0) {
          if (DAT_00446cec != 4) {
            return;
          }
          DAT_0051c2cc = 0;
          _DAT_0051c2ec = 4;
          DAT_00446cec = 0xffffffff;
          return;
        }
        iVar1 = PointInRectInclusive(DAT_004fbd24,DAT_004fbd30,(int *)&DAT_0051a6a0);
        if (iVar1 != 0) {
          if (DAT_00446cec != 5) {
            return;
          }
          DAT_0051c2cc = 0;
          _DAT_0051c2ec = 5;
          DAT_00446cec = 0xffffffff;
          return;
        }
        iVar1 = PointInRectInclusive(DAT_004fbd24,DAT_004fbd30,(int *)&DAT_0051a6b4);
        if (iVar1 == 0) {
          iVar1 = PointInRectInclusive(DAT_004fbd24,DAT_004fbd30,(int *)&DAT_0051a6cc);
          if (iVar1 == 0) {
            return;
          }
          if (DAT_00446cec != 6) {
            return;
          }
          DAT_00446cec = 0xffffffff;
          return;
        }
        bVar2 = DAT_00446cec == 2;
      }
      else {
        bVar2 = DAT_00446cec == 1;
      }
      if (bVar2) {
        DAT_0051c2cc = 0;
        _DAT_0051c2ec = 2;
        DAT_00446cec = 0xffffffff;
        return;
      }
    }
  }
  else {
    DAT_0051c2bc = 0;
    ResetMouseBoundsToGameViewport();
    ResumeGlobalBinkMovie();
    UnregisterBitmapSurface(0x51c29c);
    if (DAT_0051c29c != (int *)0x0) {
      (**(code **)(*DAT_0051c29c + 8))(DAT_0051c29c);
      DAT_0051c29c = (int *)0x0;
      return;
    }
  }
  return;
}

