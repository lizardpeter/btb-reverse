
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00439945 <.text+0x38945>:
  439945:	55                   	push   ebp
  439946:	8b ec                	mov    ebp,esp
  439948:	51                   	push   ecx
  439949:	51                   	push   ecx
  43994a:	53                   	push   ebx
  43994b:	56                   	push   esi
  43994c:	57                   	push   edi
  43994d:	33 ff                	xor    edi,edi
  43994f:	39 7d 08             	cmp    DWORD PTR [ebp+0x8],edi
  439952:	74 55                	je     0x4399a9
  439954:	6a 3d                	push   0x3d
  439956:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  439959:	e8 cd 04 00 00       	call   0x439e2b
  43995e:	8b f0                	mov    esi,eax
  439960:	59                   	pop    ecx
  439961:	3b f7                	cmp    esi,edi
  439963:	59                   	pop    ecx
  439964:	89 75 f8             	mov    DWORD PTR [ebp-0x8],esi
  439967:	74 40                	je     0x4399a9
  439969:	39 75 08             	cmp    DWORD PTR [ebp+0x8],esi
  43996c:	74 3b                	je     0x4399a9
  43996e:	a1 f0 c3 51 00       	mov    eax,ds:0x51c3f0
  439973:	33 db                	xor    ebx,ebx
  439975:	38 5e 01             	cmp    BYTE PTR [esi+0x1],bl
  439978:	0f 94 c3             	sete   bl
  43997b:	3b 05 f4 c3 51 00    	cmp    eax,DWORD PTR ds:0x51c3f4
  439981:	75 0c                	jne    0x43998f
  439983:	50                   	push   eax
  439984:	e8 9b 01 00 00       	call   0x439b24
  439989:	59                   	pop    ecx
  43998a:	a3 f0 c3 51 00       	mov    ds:0x51c3f0,eax
  43998f:	3b c7                	cmp    eax,edi
  439991:	75 54                	jne    0x4399e7
  439993:	39 7d 0c             	cmp    DWORD PTR [ebp+0xc],edi
  439996:	74 19                	je     0x4399b1
  439998:	39 3d f8 c3 51 00    	cmp    DWORD PTR ds:0x51c3f8,edi
  43999e:	74 11                	je     0x4399b1
  4399a0:	e8 78 f9 ff ff       	call   0x43931d
  4399a5:	85 c0                	test   eax,eax
  4399a7:	74 3e                	je     0x4399e7
  4399a9:	83 c8 ff             	or     eax,0xffffffff
  4399ac:	5f                   	pop    edi
  4399ad:	5e                   	pop    esi
  4399ae:	5b                   	pop    ebx
  4399af:	c9                   	leave
  4399b0:	c3                   	ret
  4399b1:	3b df                	cmp    ebx,edi
  4399b3:	0f 85 0c 01 00 00    	jne    0x439ac5
  4399b9:	6a 04                	push   0x4
  4399bb:	e8 d3 73 ff ff       	call   0x430d93
  4399c0:	3b c7                	cmp    eax,edi
  4399c2:	59                   	pop    ecx
  4399c3:	a3 f0 c3 51 00       	mov    ds:0x51c3f0,eax
  4399c8:	74 df                	je     0x4399a9
  4399ca:	89 38                	mov    DWORD PTR [eax],edi
  4399cc:	39 3d f8 c3 51 00    	cmp    DWORD PTR ds:0x51c3f8,edi
  4399d2:	75 13                	jne    0x4399e7
  4399d4:	6a 04                	push   0x4
  4399d6:	e8 b8 73 ff ff       	call   0x430d93
  4399db:	3b c7                	cmp    eax,edi
  4399dd:	59                   	pop    ecx
  4399de:	a3 f8 c3 51 00       	mov    ds:0x51c3f8,eax
  4399e3:	74 c4                	je     0x4399a9
  4399e5:	89 38                	mov    DWORD PTR [eax],edi
  4399e7:	2b 75 08             	sub    esi,DWORD PTR [ebp+0x8]
  4399ea:	8b 3d f0 c3 51 00    	mov    edi,DWORD PTR ds:0x51c3f0
  4399f0:	89 7d fc             	mov    DWORD PTR [ebp-0x4],edi
  4399f3:	56                   	push   esi
  4399f4:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  4399f7:	e8 d0 00 00 00       	call   0x439acc
  4399fc:	8b f0                	mov    esi,eax
  4399fe:	59                   	pop    ecx
  4399ff:	85 f6                	test   esi,esi
  439a01:	59                   	pop    ecx
  439a02:	7c 43                	jl     0x439a47
  439a04:	83 3f 00             	cmp    DWORD PTR [edi],0x0
  439a07:	74 3e                	je     0x439a47
  439a09:	85 db                	test   ebx,ebx
  439a0b:	74 32                	je     0x439a3f
  439a0d:	ff 34 b7             	push   DWORD PTR [edi+esi*4]
  439a10:	8d 3c b7             	lea    edi,[edi+esi*4]
  439a13:	e8 12 73 ff ff       	call   0x430d2a
  439a18:	59                   	pop    ecx
  439a19:	83 3f 00             	cmp    DWORD PTR [edi],0x0
  439a1c:	74 0b                	je     0x439a29
  439a1e:	8b 47 04             	mov    eax,DWORD PTR [edi+0x4]
  439a21:	46                   	inc    esi
  439a22:	89 07                	mov    DWORD PTR [edi],eax
  439a24:	83 c7 04             	add    edi,0x4
  439a27:	eb f0                	jmp    0x439a19
  439a29:	8b c6                	mov    eax,esi
  439a2b:	c1 e0 02             	shl    eax,0x2
  439a2e:	50                   	push   eax
  439a2f:	ff 75 fc             	push   DWORD PTR [ebp-0x4]
  439a32:	e8 54 01 00 00       	call   0x439b8b
  439a37:	59                   	pop    ecx
  439a38:	85 c0                	test   eax,eax
  439a3a:	59                   	pop    ecx
  439a3b:	74 3c                	je     0x439a79
  439a3d:	eb 35                	jmp    0x439a74
  439a3f:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  439a42:	89 04 b7             	mov    DWORD PTR [edi+esi*4],eax
  439a45:	eb 32                	jmp    0x439a79
  439a47:	85 db                	test   ebx,ebx
  439a49:	75 7a                	jne    0x439ac5
  439a4b:	85 f6                	test   esi,esi
  439a4d:	7d 02                	jge    0x439a51
  439a4f:	f7 de                	neg    esi
  439a51:	8d 04 b5 08 00 00 00 	lea    eax,[esi*4+0x8]
  439a58:	50                   	push   eax
  439a59:	57                   	push   edi
  439a5a:	e8 2c 01 00 00       	call   0x439b8b
  439a5f:	59                   	pop    ecx
  439a60:	85 c0                	test   eax,eax
  439a62:	59                   	pop    ecx
  439a63:	0f 84 40 ff ff ff    	je     0x4399a9
  439a69:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  439a6c:	89 0c b0             	mov    DWORD PTR [eax+esi*4],ecx
  439a6f:	83 64 b0 04 00       	and    DWORD PTR [eax+esi*4+0x4],0x0
  439a74:	a3 f0 c3 51 00       	mov    ds:0x51c3f0,eax
  439a79:	83 7d 0c 00          	cmp    DWORD PTR [ebp+0xc],0x0
  439a7d:	74 46                	je     0x439ac5
  439a7f:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  439a82:	e8 99 9c ff ff       	call   0x433720
  439a87:	40                   	inc    eax
  439a88:	40                   	inc    eax
  439a89:	50                   	push   eax
  439a8a:	e8 04 73 ff ff       	call   0x430d93
  439a8f:	8b f0                	mov    esi,eax
  439a91:	59                   	pop    ecx
  439a92:	85 f6                	test   esi,esi
  439a94:	59                   	pop    ecx
  439a95:	74 2e                	je     0x439ac5
  439a97:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  439a9a:	56                   	push   esi
  439a9b:	e8 90 9b ff ff       	call   0x433630
  439aa0:	8b c6                	mov    eax,esi
  439aa2:	59                   	pop    ecx
  439aa3:	2b 45 08             	sub    eax,DWORD PTR [ebp+0x8]
  439aa6:	59                   	pop    ecx
  439aa7:	03 45 f8             	add    eax,DWORD PTR [ebp-0x8]
  439aaa:	80 20 00             	and    BYTE PTR [eax],0x0
  439aad:	40                   	inc    eax
  439aae:	f7 db                	neg    ebx
  439ab0:	1b db                	sbb    ebx,ebx
  439ab2:	f7 d3                	not    ebx
  439ab4:	23 d8                	and    ebx,eax
  439ab6:	53                   	push   ebx
  439ab7:	56                   	push   esi
  439ab8:	ff 15 28 b1 43 00    	call   DWORD PTR ds:0x43b128
  439abe:	56                   	push   esi
  439abf:	e8 66 72 ff ff       	call   0x430d2a
  439ac4:	59                   	pop    ecx
  439ac5:	33 c0                	xor    eax,eax
  439ac7:	e9 e0 fe ff ff       	jmp    0x4399ac
