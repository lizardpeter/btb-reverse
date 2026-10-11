/* Ghidra generated pseudocode. NOT verified original C/C++.
 * source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
 * address: 004011b0; function: EnterContextualHelpMode; body bytes: 1349
 * callers: 1; callees: 1; success: True
 */


void EnterContextualHelpMode(void)

{
  uint auStack_23c [3];
  
  auStack_23c[0] = *(uint *)(DAT_0044de08 + 0xc);
  auStack_23c[2] = 0;
  auStack_23c[1] = 0;
  (**(code **)(*DAT_0051c298 + 0x1c))(DAT_0051c298,0,0);
  DAT_0044a2a0 = 0;
  DAT_0044dda0 = 1;
  PlayHelpVoice(0x1dc,1);
  auStack_23c[1] = 1;
  DAT_0044dda8 = 0;
  auStack_23c[0] = 0xffffffff;
  auStack_23c[2] = 0xffffff9d;
  DAT_0044ddac = auStack_23c[DAT_0044de14 * 2];
  if (DAT_0044ddac == 0xffffff9d) {
    DAT_0044ddac = (uint)(DAT_0051c2f4 != 0);
  }
  else if (DAT_0044ddac == 0xffffffff) {
    DAT_0044dda0 = 0;
  }
  if (DAT_0051c324 != 0) {
    DAT_0044ddac = 0x13;
  }
  return;
}

