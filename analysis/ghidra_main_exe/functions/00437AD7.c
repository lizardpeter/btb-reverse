/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00437ad7; function: FUN_00437ad7; body bytes: 100
 * callers: 3; callees: 2; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined * FUN_00437ad7(void)

{
  undefined4 in_stack_ffffffd8;
  undefined2 uVar1;
  uint local_10;
  uint uStack_c;
  undefined2 uStack_8;
  
  uVar1 = (undefined2)((uint)in_stack_ffffffd8 >> 0x10);
  FUN_00437b3b(&local_10,(uint *)&stack0x00000004);
  _DAT_0051c660 = FUN_00438e39(local_10,uStack_c,CONCAT22(uVar1,uStack_8),0x11,0,&DAT_0051c638);
  _DAT_0051c658 = (int)DAT_0051c63a;
  _DAT_0051c65c = (int)DAT_0051c638;
  _DAT_0051c664 = &DAT_0051c63c;
  return &DAT_0051c658;
}

