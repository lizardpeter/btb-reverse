/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00436049; function: FUN_00436049; body bytes: 339
 * callers: 4; callees: 8; success: True
 */


void __cdecl FUN_00436049(undefined *param_1)

{
  undefined4 *puVar1;
  undefined **ppuVar2;
  DWORD DVar3;
  size_t sVar4;
  HANDLE hFile;
  int iVar5;
  uint *_Dest;
  undefined1 auStackY_1e3 [7];
  LPCVOID lpBuffer;
  LPOVERLAPPED lpOverlapped;
  uint local_1a8 [65];
  uint local_a4 [40];
  
  iVar5 = 0;
  ppuVar2 = (undefined **)&DAT_00449800;
  do {
    if (param_1 == *ppuVar2) break;
    ppuVar2 = ppuVar2 + 2;
    iVar5 = iVar5 + 1;
  } while (ppuVar2 < &PTR_DAT_00449890);
  if (param_1 == (undefined *)(&DAT_00449800)[iVar5 * 2]) {
    if ((DAT_0051c41c == 1) || ((DAT_0051c41c == 0 && (DAT_00447594 == 1)))) {
      ppuVar2 = &param_1;
      puVar1 = (undefined4 *)(iVar5 * 8 + 0x449804);
      lpOverlapped = (LPOVERLAPPED)0x0;
      sVar4 = _strlen((char *)*puVar1);
      lpBuffer = (LPCVOID)*puVar1;
      hFile = GetStdHandle(0xfffffff4);
      WriteFile(hFile,lpBuffer,sVar4,(LPDWORD)ppuVar2,lpOverlapped);
    }
    else if (param_1 != (undefined *)0xfc) {
      DVar3 = GetModuleFileNameA((HMODULE)0x0,(LPSTR)local_1a8,0x104);
      if (DVar3 == 0) {
        FUN_00433630(local_1a8,(uint *)"<program name unknown>");
      }
      _Dest = local_1a8;
      sVar4 = _strlen((char *)local_1a8);
      if (0x3c < sVar4 + 1) {
        sVar4 = _strlen((char *)local_1a8);
        _Dest = (uint *)(auStackY_1e3 + sVar4);
        _strncpy((char *)_Dest,"...",3);
      }
      FUN_00433630(local_a4,(uint *)"Runtime Error!\n\nProgram: ");
      FUN_00433640(local_a4,_Dest);
      FUN_00433640(local_a4,(uint *)&DAT_0043ba24);
      FUN_00433640(local_a4,*(uint **)(iVar5 * 8 + 0x449804));
      auStackY_1e3._3_4_ = 0x43616d;
      FUN_00438272(local_a4,"Microsoft Visual C++ Runtime Library",0x12010);
    }
  }
  return;
}

