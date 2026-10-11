/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 0041cbe0; function: FindMazeShortestPath; body bytes: 49
 * callers: 1; callees: 1; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulonglong __fastcall
FindMazeShortestPath(undefined4 param_1,int *param_2,undefined4 param_3,undefined4 param_4)

{
  ulonglong uVar1;
  
  _DAT_00510a0c = 0;
  DAT_005107cc = 9999999;
  DAT_00510954 = param_3;
  uVar1 = SearchMazePathRecursive(param_4,param_2,(int *)0x0,param_4,0,0);
  return uVar1;
}

