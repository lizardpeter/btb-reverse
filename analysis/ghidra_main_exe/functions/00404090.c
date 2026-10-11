/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00404090; function: CSound_Destructor; body bytes: 109
 * callers: 1; callees: 2; success: True
 */


void __fastcall CSound_Destructor(undefined4 *param_1)

{
  int *piVar1;
  undefined4 *puVar2;
  uint uVar3;
  
  uVar3 = 0;
  *param_1 = &PTR_CSound_ScalarDeletingDestructor_0043b2e4;
  if (param_1[4] != 0) {
    do {
      piVar1 = *(int **)(param_1[1] + uVar3 * 4);
      if (piVar1 != (int *)0x0) {
        (**(code **)(*piVar1 + 8))(piVar1);
        *(undefined4 *)(param_1[1] + uVar3 * 4) = 0;
      }
      uVar3 = uVar3 + 1;
    } while (uVar3 < (uint)param_1[4]);
  }
  if ((undefined *)param_1[1] != (undefined *)0x0) {
    FUN_0042fbdc((undefined *)param_1[1]);
    param_1[1] = 0;
  }
  puVar2 = (undefined4 *)param_1[3];
  if (puVar2 != (undefined4 *)0x0) {
    CWaveFile_Destructor(puVar2);
    FUN_0042fbdc((undefined *)puVar2);
    param_1[3] = 0;
  }
  return;
}

