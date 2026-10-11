/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00436789; function: FUN_00436789; body bytes: 606
 * callers: 1; callees: 10; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00436789(void)

{
  char cVar1;
  char cVar2;
  uint *_Str1;
  DWORD DVar3;
  int iVar4;
  size_t sVar5;
  void *this;
  uint *_Source;
  int iStack_4;
  
  DAT_0051c570 = 0;
  DAT_00449dd8 = 0xffffffff;
  DAT_00449dc8 = 0xffffffff;
  _Str1 = (uint *)FUN_0043874a("TZ");
  if (_Str1 == (uint *)0x0) {
    DVar3 = GetTimeZoneInformation((LPTIME_ZONE_INFORMATION)&DAT_0051c578);
    if (DVar3 == 0xffffffff) {
      return;
    }
    DAT_00449d30 = (void *)(DAT_0051c578 * 0x3c);
    DAT_0051c570 = 1;
    if (DAT_0051c5be != 0) {
      DAT_00449d30 = (void *)((int)DAT_00449d30 + DAT_0051c5cc * 0x3c);
    }
    if ((DAT_0051c612 == 0) || (DAT_0051c620 == 0)) {
      DAT_00449d34 = 0;
      _DAT_00449d38 = 0;
    }
    else {
      DAT_00449d34 = 1;
      _DAT_00449d38 = (DAT_0051c620 - DAT_0051c5cc) * 0x3c;
    }
    iVar4 = WideCharToMultiByte(DAT_0051c690,0x220,(LPCWSTR)&DAT_0051c57c,-1,PTR_DAT_00449dbc,0x3f,
                                (LPCSTR)0x0,&iStack_4);
    if ((iVar4 == 0) || (iStack_4 != 0)) {
      *PTR_DAT_00449dbc = 0;
    }
    else {
      PTR_DAT_00449dbc[0x3f] = 0;
    }
    iVar4 = WideCharToMultiByte(DAT_0051c690,0x220,(LPCWSTR)&DAT_0051c5d0,-1,PTR_DAT_00449dc0,0x3f,
                                (LPCSTR)0x0,&iStack_4);
    if ((iVar4 != 0) && (iStack_4 == 0)) {
      PTR_DAT_00449dc0[0x3f] = 0;
      return;
    }
  }
  else {
    if ((char)*_Str1 == '\0') {
      return;
    }
    if ((DAT_0051c624 != (uint *)0x0) &&
       (iVar4 = _strcmp((char *)_Str1,(char *)DAT_0051c624), iVar4 == 0)) {
      return;
    }
    FUN_00430d2a((undefined *)DAT_0051c624);
    sVar5 = _strlen((char *)_Str1);
    DAT_0051c624 = (uint *)_malloc(sVar5 + 1);
    if (DAT_0051c624 == (uint *)0x0) {
      return;
    }
    FUN_00433630(DAT_0051c624,_Str1);
    _strncpy(PTR_DAT_00449dbc,(char *)_Str1,3);
    _Source = (uint *)((int)_Str1 + 3);
    PTR_DAT_00449dbc[3] = 0;
    cVar1 = *(char *)_Source;
    if (cVar1 == '-') {
      _Source = _Str1 + 1;
    }
    iVar4 = FUN_004386bf(this,(byte *)_Source);
    DAT_00449d30 = (void *)(iVar4 * 0xe10);
    for (; (cVar2 = (char)*_Source, cVar2 == '+' || (('/' < cVar2 && (cVar2 < ':'))));
        _Source = (uint *)((int)_Source + 1)) {
    }
    if ((char)*_Source == ':') {
      _Source = (uint *)((int)_Source + 1);
      iVar4 = FUN_004386bf(DAT_00449d30,(byte *)_Source);
      DAT_00449d30 = (void *)((int)DAT_00449d30 + iVar4 * 0x3c);
      for (; ('/' < (char)*_Source && ((char)*_Source < ':')); _Source = (uint *)((int)_Source + 1))
      {
      }
      if ((char)*_Source == ':') {
        _Source = (uint *)((int)_Source + 1);
        iVar4 = FUN_004386bf(DAT_00449d30,(byte *)_Source);
        DAT_00449d30 = (void *)((int)DAT_00449d30 + iVar4);
        for (; ('/' < (char)*_Source && ((char)*_Source < ':'));
            _Source = (uint *)((int)_Source + 1)) {
        }
      }
    }
    if (cVar1 == '-') {
      DAT_00449d30 = (void *)-(int)DAT_00449d30;
    }
    DAT_00449d34 = (int)(char)*_Source;
    if (DAT_00449d34 != 0) {
      _strncpy(PTR_DAT_00449dc0,(char *)_Source,3);
      PTR_DAT_00449dc0[3] = 0;
      return;
    }
  }
  *PTR_DAT_00449dc0 = 0;
  return;
}

