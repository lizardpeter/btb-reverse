/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 00407e50; function: InitializeDirectInput; body bytes: 1492
 * callers: 1; callees: 12; success: True
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int InitializeDirectInput(void)

{
  int *piVar1;
  int iVar2;
  undefined4 *puVar3;
  LPSTR pCVar4;
  HANDLE pvVar5;
  undefined4 uStack_54;
  int *piStack_50;
  undefined *puStack_4c;
  int *piStack_48;
  undefined *puStack_44;
  undefined4 *puStack_40;
  undefined4 uStack_3c;
  
  uStack_3c = 0x407e70;
  GetModuleHandleA((LPCSTR)0x0);
  uStack_3c = 0x407e76;
  iVar2 = DirectInput8Create();
  if (-1 < iVar2) {
    uStack_3c = 0;
    puStack_40 = &DAT_004fbf94;
    puStack_44 = &DAT_0043b508;
    piStack_48 = DAT_004fbf90;
    puStack_4c = (undefined *)0x407e94;
    iVar2 = (**(code **)(*DAT_004fbf90 + 0xc))();
    if (-1 < iVar2) {
      puStack_4c = &DAT_0043b590;
      piStack_50 = DAT_004fbf94;
      uStack_54 = 0x407eac;
      iVar2 = (**(code **)(*DAT_004fbf94 + 0x2c))();
      if (-1 < iVar2) {
        uStack_54 = 5;
        iVar2 = (**(code **)(*DAT_004fbf94 + 0x34))(DAT_004fbf94,0);
        if (-1 < iVar2) {
          _DAT_004fbf9c = CreateEventA((LPSECURITY_ATTRIBUTES)0x0,0,0,(LPCSTR)0x0);
          if (_DAT_004fbf9c == (HANDLE)0x0) {
            return -0x7fffbffb;
          }
          pvVar5 = _DAT_004fbf9c;
          iVar2 = (**(code **)(*DAT_004fbf94 + 0x30))(DAT_004fbf94,_DAT_004fbf9c);
          if (-1 < iVar2) {
            piStack_50 = (int *)0x10;
            puStack_44 = (undefined *)0x10;
            uStack_54 = 0x14;
            puStack_4c = (undefined *)0x0;
            piStack_48 = (int *)0x0;
            iVar2 = (**(code **)(*DAT_004fbf94 + 0x18))(DAT_004fbf94,1,&uStack_54);
            if (-1 < iVar2) {
              ResetMouseBoundsToGameViewport();
              DAT_004fbd24 = 0x140;
              DAT_004fbd30 = 0;
              DAT_004fbe54 = 0;
              DAT_004fbd50 = 0;
              piVar1 = *(int **)(DAT_0044de08 + 4);
              DAT_004fbf88 = LoadBitmapToDirectDrawSurface(piVar1,s_data_ui_cursor_bmp_0043ee58,0,0)
              ;
              RegisterBitmapSurface(&DAT_004fbf88,s_data_ui_cursor_bmp_0043ee58);
              MarkRegisteredSurfaceColorKeyed(0x4fbf88);
              SetSurfaceTransparencyColorKey(DAT_004fbf88,0xff00ff);
              InitializeAndLoadPlayerProfiles();
              puVar3 = (undefined4 *)&DAT_005168c4;
              pCVar4 = &DAT_0051624c;
              do {
                if (*pCVar4 != '\0') {
                  CSoundManager_Create
                            (DAT_004fc174,puVar3,pCVar4,0,DAT_0043b5a8,DAT_0043b5ac,DAT_0043b5b0,
                             DAT_0043b5b4,1);
                }
                pCVar4 = pCVar4 + 0x80;
                puVar3 = puVar3 + 1;
              } while ((int)pCVar4 < 0x51654c);
              DAT_0051c288 = LoadBitmapToDirectDrawSurface
                                       (piVar1,s_data_ui_options_optionsscreen_bm_0043ee34,0,0);
              RegisterBitmapSurface(&DAT_0051c288,s_data_ui_options_optionsscreen_bm_0043ee34);
              DAT_0051c28c = LoadBitmapToDirectDrawSurface
                                       (piVar1,s_data_ui_options_pointer_bmp_0043ee18,0,0);
              RegisterBitmapSurface(&DAT_0051c28c,s_data_ui_options_pointer_bmp_0043ee18);
              MarkRegisteredSurfaceColorKeyed(0x51c288);
              SetSurfaceTransparencyColorKey(DAT_0051c288,0xff00ff);
              MarkRegisteredSurfaceColorKeyed(0x51c28c);
              SetSurfaceTransparencyColorKey(DAT_0051c28c,0xff00ff);
              DAT_0051c290 = LoadBitmapToDirectDrawSurface
                                       (piVar1,s_data_ui_main_quitmain_bmp_0043edfc,0,0);
              RegisterBitmapSurface(&DAT_0051c290,s_data_ui_main_quitmain_bmp_0043edfc);
              MarkRegisteredSurfaceColorKeyed(0x51c290);
              SetSurfaceTransparencyColorKey(DAT_0051c290,0xff00ff);
              DAT_0051c2a0 = LoadBitmapToDirectDrawSurface
                                       (piVar1,s_data_ui_playagain_payessel_bmp_0043eddc,0,0);
              RegisterBitmapSurface(&DAT_0051c2a0,s_data_ui_playagain_payessel_bmp_0043eddc);
              DAT_0051c2a4 = LoadBitmapToDirectDrawSurface
                                       (piVar1,s_data_ui_playagain_panosel_bmp_0043edbc,0,0);
              RegisterBitmapSurface(&DAT_0051c2a4,s_data_ui_playagain_panosel_bmp_0043edbc);
              MarkRegisteredSurfaceColorKeyed(0x51c2a0);
              SetSurfaceTransparencyColorKey(DAT_0051c2a0,0xff00ff);
              MarkRegisteredSurfaceColorKeyed(0x51c2a4);
              SetSurfaceTransparencyColorKey(DAT_0051c2a4,0xff00ff);
              DAT_0051c2a8 = LoadBitmapToDirectDrawSurface
                                       (piVar1,s_data_ui_playagain_payesdep_bmp_0043ed9c,0,0);
              RegisterBitmapSurface(&DAT_0051c2a8,s_data_ui_playagain_payesdep_bmp_0043ed9c);
              DAT_0051c2ac = LoadBitmapToDirectDrawSurface
                                       (piVar1,s_data_ui_playagain_panodep_bmp_0043ed7c,0,0);
              RegisterBitmapSurface(&DAT_0051c2ac,s_data_ui_playagain_panodep_bmp_0043ed7c);
              MarkRegisteredSurfaceColorKeyed(0x51c2a8);
              SetSurfaceTransparencyColorKey(DAT_0051c2a8,0xff00ff);
              MarkRegisteredSurfaceColorKeyed(0x51c2ac);
              SetSurfaceTransparencyColorKey(DAT_0051c2ac,0xff00ff);
              DAT_0051c298 = LoadBitmapToDirectDrawSurface
                                       (piVar1,s_data_ui_main_quitmain_bmp_0043edfc,0,0);
              RegisterBitmapSurface(&DAT_0051c298,s_data_ui_main_quitmain_bmp_0043edfc);
              DAT_00519954 = LoadBitmapToDirectDrawSurface
                                       (piVar1,s_data_ui_textcursor_bmp_0043ed64,0,0);
              RegisterBitmapSurface(&DAT_00519954,s_data_ui_textcursor_bmp_0043ed64);
              MarkRegisteredSurfaceColorKeyed(0x519954);
              SetSurfaceTransparencyColorKey(DAT_00519954,0xff00ff);
              DAT_0051be40 = LoadBitmapToDirectDrawSurface
                                       (piVar1,s_data_ui_textcursor_bmp_0043ed64,0,0);
              RegisterBitmapSurface(&DAT_0051be40,s_data_ui_textcursor_bmp_0043ed64);
              MarkRegisteredSurfaceColorKeyed(0x51be40);
              SetSurfaceTransparencyColorKey(DAT_0051be40,0xff00ff);
              DAT_0051be44 = LoadBitmapToDirectDrawSurface
                                       (piVar1,s_data_ui_textcursor_bmp_0043ed64,0,0);
              RegisterBitmapSurface(&DAT_0051be44,s_data_ui_textcursor_bmp_0043ed64);
              MarkRegisteredSurfaceColorKeyed(0x51be44);
              SetSurfaceTransparencyColorKey(DAT_0051be44,0xff00ff);
              DAT_0051be34 = LoadBitmapToDirectDrawSurface
                                       (piVar1,s_data_ui_textcursor_bmp_0043ed64,0,0);
              RegisterBitmapSurface(&DAT_0051be34,s_data_ui_textcursor_bmp_0043ed64);
              MarkRegisteredSurfaceColorKeyed(0x51be34);
              SetSurfaceTransparencyColorKey(DAT_0051be34,0xff00ff);
              DAT_0051be38 = LoadBitmapToDirectDrawSurface
                                       (piVar1,s_data_ui_textcursor_bmp_0043ed64,0,0);
              RegisterBitmapSurface(&DAT_0051be38,s_data_ui_textcursor_bmp_0043ed64);
              MarkRegisteredSurfaceColorKeyed(0x51be38);
              SetSurfaceTransparencyColorKey(DAT_0051be38,0xff00ff);
              DAT_00519944 = LoadBitmapToDirectDrawSurface
                                       (piVar1,s_data_ui_help_cursor_bmp_0043ed4c,0,0);
              RegisterBitmapSurface(&DAT_00519944,s_data_ui_help_cursor_bmp_0043ed4c);
              MarkRegisteredSurfaceColorKeyed(0x519944);
              SetSurfaceTransparencyColorKey(DAT_00519944,0xff00ff);
              CSoundManager_Create
                        (DAT_004fc174,&DAT_004fbf8c,s_Data_ui_click_wav_0043ed38,0,DAT_0043b5a8,
                         DAT_0043b5ac,DAT_0043b5b0,DAT_0043b5b4,1);
              CSoundManager_Create
                        (DAT_004fc174,&DAT_0051c2b8,s_data_sound_sting_1_out_wav_0043ed1c,0,
                         DAT_0043b5a8,DAT_0043b5ac,DAT_0043b5b0,DAT_0043b5b4,1);
              SetCursorSurface((int *)0x0);
              iVar2 = (**(code **)(*DAT_004fbf90 + 0xc))(DAT_004fbf90,&DAT_0043b4f8,&DAT_004fbf98,0)
              ;
              if (-1 < iVar2) {
                iVar2 = (**(code **)(*DAT_004fbf98 + 0x2c))(DAT_004fbf98,&DAT_0043b578);
                if (-1 < iVar2) {
                  iVar2 = (**(code **)(*DAT_004fbf98 + 0x34))(DAT_004fbf98,pvVar5,0x16);
                  if (-1 < iVar2) {
                    UpdateDirectInputAcquireState();
                    DAT_0043ed14 = 1;
                    iVar2 = 0;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return iVar2;
}

