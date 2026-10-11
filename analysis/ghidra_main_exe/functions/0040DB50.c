/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0040db50; function: DrawParkDesignerActivity; body bytes: 132
 * callers: 2; callees: 2; success: True
 */


void DrawParkDesignerActivity(void)

{
  int *piVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  
  piVar1 = *(int **)(DAT_0044de08 + 0xc);
  (**(code **)(*piVar1 + 0x1c))(piVar1,0,0,DAT_00507b50,0,0);
  puVar3 = &DAT_00508c10;
  puVar2 = &DAT_004fcab0;
  do {
    *puVar3 = puVar2;
    puVar2 = puVar2 + 0x13;
    puVar3 = puVar3 + 1;
  } while ((int)puVar2 < 0x504170);
  FUN_00430b5c(&DAT_00508c10,DAT_00441dc8,4,CompareParkDesignerObjectsBySortLayer);
  FUN_00430b5c(&DAT_00508c10,DAT_00441dc8,4,CompareParkDesignerObjectsForDraw);
  DrawSortedParkDesignerObjects();
  (**(code **)(*piVar1 + 0x1c))(piVar1,0,0,DAT_00509254,0,1);
  return;
}

