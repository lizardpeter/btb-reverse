
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0043a14f <.text+0x3914f>:
  43a14f:	b8 f8 a5 43 00       	mov    eax,0x43a5f8
  43a154:	e8 cf 00 00 00       	call   0x43a228
  43a159:	51                   	push   ecx
  43a15a:	56                   	push   esi
  43a15b:	8b f1                	mov    esi,ecx
  43a15d:	89 75 f0             	mov    DWORD PTR [ebp-0x10],esi
  43a160:	c7 06 74 bb 43 00    	mov    DWORD PTR [esi],0x43bb74
  43a166:	83 65 fc 00          	and    DWORD PTR [ebp-0x4],0x0
  43a16a:	6a 01                	push   0x1
  43a16c:	8d 4e 0c             	lea    ecx,[esi+0xc]
  43a16f:	e8 dc c1 fc ff       	call   0x406350
  43a174:	83 4d fc ff          	or     DWORD PTR [ebp-0x4],0xffffffff
  43a178:	8b ce                	mov    ecx,esi
  43a17a:	e8 c2 62 ff ff       	call   0x430441
  43a17f:	8b 4d f4             	mov    ecx,DWORD PTR [ebp-0xc]
  43a182:	5e                   	pop    esi
  43a183:	64 89 0d 00 00 00 00 	mov    DWORD PTR fs:0x0,ecx
  43a18a:	c9                   	leave
  43a18b:	c3                   	ret
