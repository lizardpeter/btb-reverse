/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00427270; function: StepSquirrelPlacementActorTowardTarget; body bytes: 162
 * callers: 1; callees: 0; success: True
 */


undefined4 __cdecl StepSquirrelPlacementActorTowardTarget(int param_1,int param_2)

{
  uint uVar1;
  
  if (DAT_0044699c < param_1) {
    DAT_0044699c = DAT_0044699c + 2;
  }
  else if (param_1 < DAT_0044699c) {
    DAT_0044699c = DAT_0044699c + -2;
  }
  if (DAT_004469a0 < param_2) {
    DAT_004469a0 = DAT_004469a0 + 2;
  }
  else if (param_2 < DAT_004469a0) {
    DAT_004469a0 = DAT_004469a0 + -2;
  }
  uVar1 = DAT_0044699c - param_1 >> 0x1f;
  if ((int)((DAT_0044699c - param_1 ^ uVar1) - uVar1) < 4) {
    DAT_0044699c = param_1;
  }
  uVar1 = DAT_004469a0 - param_2 >> 0x1f;
  if ((int)((DAT_004469a0 - param_2 ^ uVar1) - uVar1) < 4) {
    DAT_004469a0 = param_2;
  }
  uVar1 = DAT_0044699c - param_1 >> 0x1f;
  if (((int)((DAT_0044699c - param_1 ^ uVar1) - uVar1) < 4) &&
     (uVar1 = DAT_004469a0 - param_2 >> 0x1f, (int)((DAT_004469a0 - param_2 ^ uVar1) - uVar1) < 4))
  {
    DAT_004469a0 = param_2;
    DAT_0044699c = param_1;
    return 1;
  }
  return 0;
}

