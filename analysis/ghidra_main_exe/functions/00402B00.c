/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00402b00; function: SoundManagerConstructor; body bytes: 182
 * callers: 1; callees: 3; success: True
 */


int __fastcall SoundManagerConstructor(int param_1)

{
  undefined4 *puVar1;
  undefined1 *puVar2;
  FILE *pFVar3;
  int iVar4;
  void *extraout_ECX;
  void *this;
  void *extraout_ECX_00;
  
  puVar1 = (undefined4 *)(param_1 + 0xb18);
  iVar4 = 0x50;
  do {
    puVar1[-0x2c6] = 0;
    puVar1[-0x276] = 0xffffffff;
    *puVar1 = 0x65;
    puVar1[0x50] = 0;
    puVar1[0xf0] = 0xffffffff;
    puVar1 = puVar1 + 1;
    iVar4 = iVar4 + -1;
  } while (iVar4 != 0);
  puVar2 = (undefined1 *)(param_1 + 0x6cc);
  do {
    puVar2[-0x44c] = 0xff;
    *puVar2 = 1;
    puVar2 = puVar2 + 1;
  } while ((int)(puVar2 + (-0x6cc - param_1)) < 0x44c);
  pFVar3 = (FILE *)OpenGameDataFileWithCDFallback(s_data_sound_binklist_txt_0043e108,&DAT_0043e070);
  this = extraout_ECX;
  if (pFVar3 != (FILE *)0x0) {
    do {
      iVar4 = crt_fscanf(this,(int *)pFVar3,(byte *)s__s__d_0043e100);
      this = extraout_ECX_00;
    } while (iVar4 != -1);
  }
  crt_fclose(pFVar3);
  return param_1;
}

