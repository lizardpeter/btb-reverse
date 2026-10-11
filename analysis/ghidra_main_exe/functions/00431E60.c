/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00431e60; function: FUN_00431e60; body bytes: 423
 * callers: 1; callees: 8; success: True
 */


void __cdecl
FUN_00431e60(PEXCEPTION_RECORD param_1,PVOID param_2,undefined4 param_3,undefined4 param_4,
            uint param_5,char param_6,int param_7,PVOID param_8)

{
  byte *pbVar1;
  PEXCEPTION_RECORD pEVar2;
  bool bVar3;
  undefined3 extraout_var;
  int *piVar4;
  int iVar5;
  int *piVar6;
  uint local_1c;
  uint local_18;
  int local_14;
  int local_10;
  int local_c;
  uint local_8;
  
  local_18 = local_18 & 0xffffff00;
  local_14 = *(int *)((int)param_2 + 8);
  if ((local_14 < -1) || (*(int *)(param_5 + 4) <= local_14)) {
    FUN_00432662();
  }
  pEVar2 = DAT_0051c420;
  if (param_1->ExceptionCode == 0xe06d7363) {
    if (((param_1->NumberParameters == 3) && (param_1->ExceptionInformation[0] == 0x19930520)) &&
       (param_1->ExceptionInformation[2] == 0)) {
      if (DAT_0051c420 == (PEXCEPTION_RECORD)0x0) {
        return;
      }
      param_3 = DAT_0051c424;
      local_18 = CONCAT31(local_18._1_3_,1);
      bVar3 = FUN_0043670d(DAT_0051c420,1);
      if (CONCAT31(extraout_var,bVar3) == 0) {
        FUN_00432662();
      }
      param_1 = pEVar2;
      if (pEVar2->ExceptionCode != 0xe06d7363) goto LAB_00431fdc;
      if (((pEVar2->NumberParameters == 3) && (pEVar2->ExceptionInformation[0] == 0x19930520)) &&
         (pEVar2->ExceptionInformation[2] == 0)) {
        FUN_00432662();
      }
    }
    iVar5 = local_14;
    if (((param_1->ExceptionCode == 0xe06d7363) && (param_1->NumberParameters == 3)) &&
       (param_1->ExceptionInformation[0] == 0x19930520)) {
      piVar4 = (int *)FUN_0042fe50(param_5,param_7,local_14,&local_8,&local_1c);
      do {
        if (local_1c <= local_8) {
          if (param_6 == '\0') {
            return;
          }
          FUN_00432535((int)param_1);
          return;
        }
        if ((*piVar4 <= iVar5) && (iVar5 <= piVar4[1])) {
          pbVar1 = (byte *)piVar4[4];
          for (local_10 = piVar4[3]; iVar5 = local_14, 0 < local_10; local_10 = local_10 + -1) {
            piVar6 = *(int **)(param_1->ExceptionInformation[2] + 0xc);
            for (local_c = *piVar6; 0 < local_c; local_c = local_c + -1) {
              piVar6 = piVar6 + 1;
              iVar5 = FUN_004320af(pbVar1,(byte *)*piVar6,(uint *)param_1->ExceptionInformation[2]);
              if (iVar5 != 0) {
                FUN_004321c0(param_1,param_2,param_3,param_4,param_5,pbVar1,(byte *)*piVar6,piVar4,
                             param_7,param_8);
                iVar5 = local_14;
                goto LAB_00431fbc;
              }
            }
            pbVar1 = pbVar1 + 0x10;
          }
        }
LAB_00431fbc:
        local_8 = local_8 + 1;
        piVar4 = piVar4 + 5;
      } while( true );
    }
  }
LAB_00431fdc:
  if (param_6 == '\0') {
    FUN_00432007(param_1,param_2,param_3,param_4,param_5,local_14,param_7,param_8);
    return;
  }
  FUN_0043260c();
  return;
}

