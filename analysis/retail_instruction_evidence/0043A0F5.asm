
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0043a0f5 <.text+0x390f5>:
  43a0f5:	b8 e4 a5 43 00       	mov    eax,0x43a5e4
  43a0fa:	e8 29 01 00 00       	call   0x43a228
  43a0ff:	83 ec 30             	sub    esp,0x30
  43a102:	8a 45 f3             	mov    al,BYTE PTR [ebp-0xd]
  43a105:	56                   	push   esi
  43a106:	6a 00                	push   0x0
  43a108:	8d 4d e0             	lea    ecx,[ebp-0x20]
  43a10b:	88 45 e0             	mov    BYTE PTR [ebp-0x20],al
  43a10e:	e8 3d c2 fc ff       	call   0x406350
  43a113:	be 90 bb 43 00       	mov    esi,0x43bb90
  43a118:	56                   	push   esi
  43a119:	e8 02 96 ff ff       	call   0x433720
  43a11e:	59                   	pop    ecx
  43a11f:	50                   	push   eax
  43a120:	56                   	push   esi
  43a121:	8d 4d e0             	lea    ecx,[ebp-0x20]
  43a124:	e8 77 c2 fc ff       	call   0x4063a0
  43a129:	83 65 fc 00          	and    DWORD PTR [ebp-0x4],0x0
  43a12d:	8d 45 e0             	lea    eax,[ebp-0x20]
  43a130:	50                   	push   eax
  43a131:	8d 4d c4             	lea    ecx,[ebp-0x3c]
  43a134:	e8 ea fd ff ff       	call   0x439f23
  43a139:	8d 45 c4             	lea    eax,[ebp-0x3c]
  43a13c:	68 10 c2 43 00       	push   0x43c210
  43a141:	50                   	push   eax
  43a142:	c7 45 c4 84 bb 43 00 	mov    DWORD PTR [ebp-0x3c],0x43bb84
  43a149:	e8 47 63 ff ff       	call   0x430495
