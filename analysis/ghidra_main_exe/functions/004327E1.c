/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004327e1; function: FUN_004327e1; body bytes: 123
 * callers: 1; callees: 1; success: True
 */


uint __cdecl FUN_004327e1(uint param_1)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  undefined1 uVar5;
  undefined2 local_8;
  
  uVar4 = param_1;
  if (param_1 < 0x100) {
    if ((*(byte *)((int)&DAT_0051c7e0 + param_1 + 1) & 0x20) == 0x20) {
      uVar4 = (uint)(byte)(&DAT_0051c6e0)[param_1];
    }
  }
  else {
    uVar5 = (undefined1)param_1;
    uVar2 = param_1 >> 8;
    uVar1 = param_1 >> 8;
    param_1 = CONCAT13(uVar5,CONCAT12((char)uVar1,(undefined2)param_1));
    if (((*(byte *)((int)&DAT_0051c7e0 + (uVar2 & 0xff) + 1) & 4) != 0) &&
       (iVar3 = FUN_004370b3(DAT_0051c8e4,0x200,(char *)((int)&param_1 + 2),2,&local_8,2,
                             DAT_0051c6b8,1), iVar3 != 0)) {
      uVar4 = (uint)CONCAT11((undefined1)local_8,local_8._1_1_);
    }
  }
  return uVar4;
}

