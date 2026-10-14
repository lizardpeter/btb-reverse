
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00439f23 <.text+0x38f23>:
  439f23:	b8 94 a5 43 00       	mov    eax,0x43a594
  439f28:	e8 fb 02 00 00       	call   0x43a228
  439f2d:	51                   	push   ecx
  439f2e:	51                   	push   ecx
  439f2f:	53                   	push   ebx
  439f30:	56                   	push   esi
  439f31:	8d 45 f0             	lea    eax,[ebp-0x10]
  439f34:	57                   	push   edi
  439f35:	8b f1                	mov    esi,ecx
  439f37:	50                   	push   eax
  439f38:	89 75 ec             	mov    DWORD PTR [ebp-0x14],esi
  439f3b:	c7 45 f0 74 25 48 00 	mov    DWORD PTR [ebp-0x10],0x482574
  439f42:	e8 73 64 ff ff       	call   0x4303ba
  439f47:	8b 5d 08             	mov    ebx,DWORD PTR [ebp+0x8]
  439f4a:	83 65 fc 00          	and    DWORD PTR [ebp-0x4],0x0
  439f4e:	8d 7e 0c             	lea    edi,[esi+0xc]
  439f51:	6a 00                	push   0x0
  439f53:	8a 03                	mov    al,BYTE PTR [ebx]
  439f55:	8b cf                	mov    ecx,edi
  439f57:	88 07                	mov    BYTE PTR [edi],al
  439f59:	e8 f2 c3 fc ff       	call   0x406350
  439f5e:	ff 35 34 b3 43 00    	push   DWORD PTR ds:0x43b334
  439f64:	8b cf                	mov    ecx,edi
  439f66:	6a 00                	push   0x0
  439f68:	53                   	push   ebx
  439f69:	e8 e2 c1 fc ff       	call   0x406150
  439f6e:	8b 4d f4             	mov    ecx,DWORD PTR [ebp-0xc]
  439f71:	c7 06 74 bb 43 00    	mov    DWORD PTR [esi],0x43bb74
  439f77:	8b c6                	mov    eax,esi
  439f79:	5f                   	pop    edi
  439f7a:	5e                   	pop    esi
  439f7b:	5b                   	pop    ebx
  439f7c:	64 89 0d 00 00 00 00 	mov    DWORD PTR fs:0x0,ecx
  439f83:	c9                   	leave
  439f84:	c2 04 00             	ret    0x4
