/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00423260; function: DistanceFromActorToCurrentScreenNode; body bytes: 77
 * callers: 2; callees: 2; success: True
 */


longlong __cdecl DistanceFromActorToCurrentScreenNode(int param_1)

{
  int iVar1;
  longlong lVar2;
  longlong lVar3;
  
  lVar2 = __ftol();
  lVar3 = __ftol();
  iVar1 = param_1 + DAT_00510d18 * 0x1e;
  DistanceBetweenIntegerPoints
            ((int)lVar2,(int)lVar3,(&DAT_00510e18)[iVar1 * 8],(&DAT_00510e1c)[iVar1 * 8]);
  lVar2 = __ftol();
  return lVar2;
}

