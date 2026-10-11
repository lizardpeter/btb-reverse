/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004112a0; function: LoadFireworksEditorResources; body bytes: 796
 * callers: 2; callees: 4; success: True
 */


void LoadFireworksEditorResources(void)

{
  int *piVar1;
  
  piVar1 = *(int **)(DAT_0044de08 + 4);
  DAT_0050ab18 = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameFirework_graph_bobsp_00442be4,0,0);
  RegisterBitmapSurface(&DAT_0050ab18,s_Data_SubGameFirework_graph_bobsp_00442be4);
  DAT_0050ab1c = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameFirework_graph_wendy_00442bb4,0,0);
  RegisterBitmapSurface(&DAT_0050ab1c,s_Data_SubGameFirework_graph_wendy_00442bb4);
  DAT_0050a494 = DAT_0050ab18;
  DAT_0050a498 = DAT_0050ab1c;
  MarkRegisteredSurfaceColorKeyed(0x50ab18);
  SetSurfaceTransparencyColorKey(DAT_0050ab18,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x50ab1c);
  SetSurfaceTransparencyColorKey(DAT_0050ab1c,0xff00ff);
  DAT_0050aaa8 = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameFirework_graph_middl_00442b8c,0,0);
  RegisterBitmapSurface(&DAT_0050aaa8,s_Data_SubGameFirework_graph_middl_00442b8c);
  DAT_0050ab08 = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameFirework_graph_botto_00442b64,0,0);
  RegisterBitmapSurface(&DAT_0050ab08,s_Data_SubGameFirework_graph_botto_00442b64);
  MarkRegisteredSurfaceColorKeyed(0x50aaa8);
  SetSurfaceTransparencyColorKey(DAT_0050aaa8,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x50ab08);
  SetSurfaceTransparencyColorKey(DAT_0050ab08,0xff00ff);
  DAT_0050a49c = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameFirework_graph_firew_00442b30,0,0);
  RegisterBitmapSurface(&DAT_0050a49c,s_Data_SubGameFirework_graph_firew_00442b30);
  DAT_0050a4a0 = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameFirework_graph_firew_00442afc,0,0);
  RegisterBitmapSurface(&DAT_0050a49c,s_Data_SubGameFirework_graph_firew_00442afc);
  DAT_0050a4a4 = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameFirework_graph_firew_00442ac8,0,0);
  RegisterBitmapSurface(&DAT_0050a4a4,s_Data_SubGameFirework_graph_firew_00442ac8);
  DAT_0050a4a8 = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameFirework_graph_firew_00442a94,0,0);
  RegisterBitmapSurface(&DAT_0050a4a8,s_Data_SubGameFirework_graph_firew_00442a94);
  DAT_0050a4ac = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameFirework_graph_firew_00442a64,0,0);
  RegisterBitmapSurface(&DAT_0050a4ac,s_Data_SubGameFirework_graph_firew_00442a64);
  DAT_0050a4b0 = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameFirework_graph_firew_00442a34,0,0);
  RegisterBitmapSurface(&DAT_0050a4b0,s_Data_SubGameFirework_graph_firew_00442a34);
  MarkRegisteredSurfaceColorKeyed(0x50a49c);
  SetSurfaceTransparencyColorKey(DAT_0050a49c,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x50a4a0);
  SetSurfaceTransparencyColorKey(DAT_0050a4a0,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x50a4a4);
  SetSurfaceTransparencyColorKey(DAT_0050a4a4,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x50a4a8);
  SetSurfaceTransparencyColorKey(DAT_0050a4a8,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x50a4ac);
  SetSurfaceTransparencyColorKey(DAT_0050a4ac,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x50a4b0);
  SetSurfaceTransparencyColorKey(DAT_0050a4b0,0xff00ff);
  DAT_0050a5c4 = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameFirework_certprint_b_0043f954,0,0);
  RegisterBitmapSurface(&DAT_0050a5c4,s_Data_SubGameFirework_certprint_b_0043f954);
  DAT_0050a5c8 = LoadBitmapToDirectDrawSurface
                           (piVar1,s_Data_SubGameFirework_certprintde_0043f92c,0,0);
  RegisterBitmapSurface(&DAT_0050a5c8,s_Data_SubGameFirework_certprintde_0043f92c);
  MarkRegisteredSurfaceColorKeyed(0x50a5c4);
  SetSurfaceTransparencyColorKey(DAT_0050a5c4,0xff00ff);
  MarkRegisteredSurfaceColorKeyed(0x50a5c8);
  SetSurfaceTransparencyColorKey(DAT_0050a5c8,0xff00ff);
  return;
}

