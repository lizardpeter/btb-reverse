/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00430884; function: FUN_00430884; body bytes: 153
 * callers: 2; callees: 4; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_00430884(UINT param_1,int param_2,int param_3)

{
  HANDLE hProcess;
  undefined4 *puVar1;
  UINT uExitCode;
  
  if (DAT_0051c410 == 1) {
    uExitCode = param_1;
    hProcess = GetCurrentProcess();
    TerminateProcess(hProcess,uExitCode);
  }
  _DAT_0051c40c = 1;
  DAT_0051c408 = (undefined1)param_3;
  if (param_2 == 0) {
    if ((DAT_0051da58 != (undefined4 *)0x0) &&
       (puVar1 = (undefined4 *)(DAT_0051da54 - 4), DAT_0051da58 <= puVar1)) {
      do {
        if ((code *)*puVar1 != (code *)0x0) {
          (*(code *)*puVar1)();
        }
        puVar1 = puVar1 + -1;
      } while (DAT_0051da58 <= puVar1);
    }
    FUN_0043091d((undefined4 *)&DAT_0043e024,(undefined4 *)&DAT_0043e02c);
  }
  FUN_0043091d((undefined4 *)&DAT_0043e030,(undefined4 *)&DAT_0043e038);
  if (param_3 != 0) {
    return;
  }
  DAT_0051c410 = 1;
                    /* WARNING: Subroutine does not return */
  ExitProcess(param_1);
}

