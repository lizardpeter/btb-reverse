/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00403830; function: ReloadRegisteredBitmapSurfaces; body bytes: 149
 * callers: 1; callees: 2; success: True
 */


void ReloadRegisteredBitmapSurfaces(void)

{
  int *piVar1;
  int *piVar2;
  int *piVar3;
  LPCSTR pCVar4;
  int iVar5;
  
  iVar5 = 0;
  piVar1 = *(int **)(DAT_0044de08 + 4);
  DAT_00482420 = 1;
  if (0 < DAT_0048241c) {
    pCVar4 = &DAT_0044f79c;
    do {
      piVar2 = (int *)(&DAT_0044eb1c)[iVar5];
      if ((piVar2 != (int *)0x0) && (piVar3 = (int *)*piVar2, piVar3 != (int *)0x0)) {
        (**(code **)(*piVar3 + 8))(piVar3);
        *piVar2 = 0;
        piVar3 = LoadBitmapToDirectDrawSurface(piVar1,pCVar4,0,0);
        *piVar2 = (int)piVar3;
        if ((&DAT_0044de9c)[iVar5] == 1) {
          SetSurfaceTransparencyColorKey(piVar3,0xff00ff);
        }
      }
      iVar5 = iVar5 + 1;
      pCVar4 = pCVar4 + 0x104;
    } while (iVar5 < DAT_0048241c);
    DAT_00482420 = 0;
    return;
  }
  DAT_00482420 = 0;
  return;
}

