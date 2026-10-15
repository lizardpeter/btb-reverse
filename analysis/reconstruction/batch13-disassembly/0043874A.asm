
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0043874a <.text+0x3774a>:
  43874a:	83 3d 4c da 51 00 00 	cmp    DWORD PTR ds:0x51da4c,0x0
  438751:	53                   	push   ebx
  438752:	56                   	push   esi
  438753:	8b 35 f0 c3 51 00    	mov    esi,DWORD PTR ds:0x51c3f0
  438759:	57                   	push   edi
  43875a:	74 65                	je     0x4387c1
  43875c:	85 f6                	test   esi,esi
  43875e:	75 1b                	jne    0x43877b
  438760:	39 35 f8 c3 51 00    	cmp    DWORD PTR ds:0x51c3f8,esi
  438766:	74 59                	je     0x4387c1
  438768:	e8 b0 0b 00 00       	call   0x43931d
  43876d:	85 c0                	test   eax,eax
  43876f:	75 50                	jne    0x4387c1
  438771:	8b 35 f0 c3 51 00    	mov    esi,DWORD PTR ds:0x51c3f0
  438777:	85 f6                	test   esi,esi
  438779:	74 46                	je     0x4387c1
  43877b:	8b 5c 24 10          	mov    ebx,DWORD PTR [esp+0x10]
  43877f:	85 db                	test   ebx,ebx
  438781:	74 3e                	je     0x4387c1
  438783:	53                   	push   ebx
  438784:	e8 97 af ff ff       	call   0x433720
  438789:	59                   	pop    ecx
  43878a:	8b f8                	mov    edi,eax
  43878c:	8b 06                	mov    eax,DWORD PTR [esi]
  43878e:	85 c0                	test   eax,eax
  438790:	74 2f                	je     0x4387c1
  438792:	50                   	push   eax
  438793:	e8 88 af ff ff       	call   0x433720
  438798:	3b c7                	cmp    eax,edi
  43879a:	59                   	pop    ecx
  43879b:	76 17                	jbe    0x4387b4
  43879d:	8b 06                	mov    eax,DWORD PTR [esi]
  43879f:	80 3c 38 3d          	cmp    BYTE PTR [eax+edi*1],0x3d
  4387a3:	75 0f                	jne    0x4387b4
  4387a5:	57                   	push   edi
  4387a6:	53                   	push   ebx
  4387a7:	50                   	push   eax
  4387a8:	e8 31 0b 00 00       	call   0x4392de
  4387ad:	83 c4 0c             	add    esp,0xc
  4387b0:	85 c0                	test   eax,eax
  4387b2:	74 05                	je     0x4387b9
  4387b4:	83 c6 04             	add    esi,0x4
  4387b7:	eb d3                	jmp    0x43878c
  4387b9:	8b 06                	mov    eax,DWORD PTR [esi]
  4387bb:	8d 44 38 01          	lea    eax,[eax+edi*1+0x1]
  4387bf:	eb 02                	jmp    0x4387c3
  4387c1:	33 c0                	xor    eax,eax
  4387c3:	5f                   	pop    edi
  4387c4:	5e                   	pop    esi
  4387c5:	5b                   	pop    ebx
  4387c6:	c3                   	ret
