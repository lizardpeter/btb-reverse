
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0042fc29 <.text+0x2ec29>:
  42fc29:	55                   	push   ebp
  42fc2a:	8b ec                	mov    ebp,esp
  42fc2c:	51                   	push   ecx
  42fc2d:	51                   	push   ecx
  42fc2e:	53                   	push   ebx
  42fc2f:	56                   	push   esi
  42fc30:	57                   	push   edi
  42fc31:	64 a1 00 00 00 00    	mov    eax,fs:0x0
  42fc37:	89 45 f8             	mov    DWORD PTR [ebp-0x8],eax
  42fc3a:	c7 45 fc 51 fc 42 00 	mov    DWORD PTR [ebp-0x4],0x42fc51
  42fc41:	6a 00                	push   0x0
  42fc43:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  42fc46:	ff 75 fc             	push   DWORD PTR [ebp-0x4]
  42fc49:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  42fc4c:	e8 cf a5 00 00       	call   0x43a220
  42fc51:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  42fc54:	8b 40 04             	mov    eax,DWORD PTR [eax+0x4]
  42fc57:	24 fd                	and    al,0xfd
  42fc59:	8b 4d 0c             	mov    ecx,DWORD PTR [ebp+0xc]
  42fc5c:	89 41 04             	mov    DWORD PTR [ecx+0x4],eax
  42fc5f:	64 a1 00 00 00 00    	mov    eax,fs:0x0
  42fc65:	8b 5d f8             	mov    ebx,DWORD PTR [ebp-0x8]
  42fc68:	89 03                	mov    DWORD PTR [ebx],eax
  42fc6a:	64 89 1d 00 00 00 00 	mov    DWORD PTR fs:0x0,ebx
  42fc71:	5f                   	pop    edi
  42fc72:	5e                   	pop    esi
  42fc73:	5b                   	pop    ebx
  42fc74:	c9                   	leave
  42fc75:	c2 08 00             	ret    0x8
