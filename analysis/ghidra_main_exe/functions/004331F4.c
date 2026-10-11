/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004331f4; function: FUN_004331f4; body bytes: 90
 * callers: 0; callees: 2; success: True
 */


void __thiscall FUN_004331f4(void *this,char *param_1)

{
  char cVar1;
  char cVar2;
  undefined *this_00;
  uint uVar3;
  undefined *puVar4;
  
  this_00 = (undefined *)(int)*param_1;
  uVar3 = FUN_004375a9(this,(uint)this_00);
  if (uVar3 != 0x65) {
    do {
      param_1 = param_1 + 1;
      if (DAT_00449b10 < 2) {
        uVar3 = (byte)PTR_DAT_00449b1c[*param_1 * 2] & 4;
        this_00 = PTR_DAT_00449b1c;
      }
      else {
        puVar4 = (undefined *)0x4;
        uVar3 = FUN_004365c0(this_00,(int)*param_1,4);
        this_00 = puVar4;
      }
    } while (uVar3 != 0);
  }
  cVar2 = *param_1;
  *param_1 = DAT_00449b14;
  do {
    param_1 = param_1 + 1;
    cVar1 = *param_1;
    *param_1 = cVar2;
    cVar2 = cVar1;
  } while (*param_1 != '\0');
  return;
}

