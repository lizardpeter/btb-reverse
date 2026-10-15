
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00404270 <.text+0x3270>:
  404270:	53                   	push   ebx
  404271:	56                   	push   esi
  404272:	8b 74 24 0c          	mov    esi,DWORD PTR [esp+0xc]
  404276:	85 f6                	test   esi,esi
  404278:	75 0a                	jne    0x404284
  40427a:	5e                   	pop    esi
  40427b:	b8 f0 01 04 80       	mov    eax,0x800401f0
  404280:	5b                   	pop    ebx
  404281:	c2 08 00             	ret    0x8
  404284:	8b 5c 24 10          	mov    ebx,DWORD PTR [esp+0x10]
  404288:	85 db                	test   ebx,ebx
  40428a:	74 06                	je     0x404292
  40428c:	c7 03 00 00 00 00    	mov    DWORD PTR [ebx],0x0
  404292:	8b 06                	mov    eax,DWORD PTR [esi]
  404294:	8d 4c 24 0c          	lea    ecx,[esp+0xc]
  404298:	51                   	push   ecx
  404299:	56                   	push   esi
  40429a:	ff 50 24             	call   DWORD PTR [eax+0x24]
  40429d:	85 c0                	test   eax,eax
  40429f:	7c 40                	jl     0x4042e1
  4042a1:	f6 44 24 0c 02       	test   BYTE PTR [esp+0xc],0x2
  4042a6:	74 34                	je     0x4042dc
  4042a8:	57                   	push   edi
  4042a9:	8b 3d 70 b1 43 00    	mov    edi,DWORD PTR ds:0x43b170
  4042af:	8b 16                	mov    edx,DWORD PTR [esi]
  4042b1:	56                   	push   esi
  4042b2:	ff 52 50             	call   DWORD PTR [edx+0x50]
  4042b5:	3d 96 00 78 88       	cmp    eax,0x88780096
  4042ba:	75 04                	jne    0x4042c0
  4042bc:	6a 0a                	push   0xa
  4042be:	ff d7                	call   edi
  4042c0:	8b 06                	mov    eax,DWORD PTR [esi]
  4042c2:	56                   	push   esi
  4042c3:	ff 50 50             	call   DWORD PTR [eax+0x50]
  4042c6:	85 c0                	test   eax,eax
  4042c8:	75 e5                	jne    0x4042af
  4042ca:	85 db                	test   ebx,ebx
  4042cc:	5f                   	pop    edi
  4042cd:	74 06                	je     0x4042d5
  4042cf:	c7 03 01 00 00 00    	mov    DWORD PTR [ebx],0x1
  4042d5:	5e                   	pop    esi
  4042d6:	33 c0                	xor    eax,eax
  4042d8:	5b                   	pop    ebx
  4042d9:	c2 08 00             	ret    0x8
  4042dc:	b8 01 00 00 00       	mov    eax,0x1
  4042e1:	5e                   	pop    esi
  4042e2:	5b                   	pop    ebx
  4042e3:	c2 08 00             	ret    0x8
