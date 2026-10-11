/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004111c0; function: UnloadFireworkMovieBank; body bytes: 217
 * callers: 2; callees: 3; success: True
 */


void UnloadFireworkMovieBank(void)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  
  iVar3 = 0;
  do {
    CloseBinkMovie(*(undefined4 *)((int)&DAT_0050aaac + iVar3));
    piVar1 = (int *)((int)&DAT_0050937c + iVar3);
    UnregisterBitmapSurface((int)piVar1);
    piVar2 = (int *)*piVar1;
    if (piVar2 != (int *)0x0) {
      (**(code **)(*piVar2 + 8))(piVar2);
      *piVar1 = 0;
    }
    FUN_00430d2a(*(undefined **)((int)&DAT_0050a50c + iVar3));
    *(undefined4 *)((int)&DAT_0050aaac + iVar3) = 0;
    iVar3 = iVar3 + 4;
  } while (iVar3 < 0x5c);
  UnregisterBitmapSurface(0x50a4b4);
  if (DAT_0050a4b4 != (int *)0x0) {
    (**(code **)(*DAT_0050a4b4 + 8))(DAT_0050a4b4);
    DAT_0050a4b4 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x50a5cc);
  if (DAT_0050a5cc != (int *)0x0) {
    (**(code **)(*DAT_0050a5cc + 8))(DAT_0050a5cc);
    DAT_0050a5cc = (int *)0x0;
  }
  UnregisterBitmapSurface(0x50a5c4);
  if (DAT_0050a5c4 != (int *)0x0) {
    (**(code **)(*DAT_0050a5c4 + 8))(DAT_0050a5c4);
    DAT_0050a5c4 = (int *)0x0;
  }
  UnregisterBitmapSurface(0x50a5c8);
  if (DAT_0050a5c8 != (int *)0x0) {
    (**(code **)(*DAT_0050a5c8 + 8))(DAT_0050a5c8);
    DAT_0050a5c8 = (int *)0x0;
  }
  return;
}

