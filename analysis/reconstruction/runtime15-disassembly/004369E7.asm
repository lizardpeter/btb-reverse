
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004369e7 <.text+0x359e7>:
  4369e7:	53                   	push   ebx
  4369e8:	56                   	push   esi
  4369e9:	57                   	push   edi
  4369ea:	33 ff                	xor    edi,edi
  4369ec:	39 3d 34 9d 44 00    	cmp    DWORD PTR ds:0x449d34,edi
  4369f2:	75 07                	jne    0x4369fb
  4369f4:	33 c0                	xor    eax,eax
  4369f6:	e9 4c 01 00 00       	jmp    0x436b47
  4369fb:	8b 74 24 10          	mov    esi,DWORD PTR [esp+0x10]
  4369ff:	6a 01                	push   0x1
  436a01:	5b                   	pop    ebx
  436a02:	8b 46 14             	mov    eax,DWORD PTR [esi+0x14]
  436a05:	3b 05 c8 9d 44 00    	cmp    eax,DWORD PTR ds:0x449dc8
  436a0b:	75 0c                	jne    0x436a19
  436a0d:	3b 05 d8 9d 44 00    	cmp    eax,DWORD PTR ds:0x449dd8
  436a13:	0f 84 02 01 00 00    	je     0x436b1b
  436a19:	39 3d 70 c5 51 00    	cmp    DWORD PTR ds:0x51c570,edi
  436a1f:	0f 84 cc 00 00 00    	je     0x436af1
  436a25:	0f b7 0d 1e c6 51 00 	movzx  ecx,WORD PTR ds:0x51c61e
  436a2c:	51                   	push   ecx
  436a2d:	66 39 3d 10 c6 51 00 	cmp    WORD PTR ds:0x51c610,di
  436a34:	0f b7 0d 1c c6 51 00 	movzx  ecx,WORD PTR ds:0x51c61c
  436a3b:	51                   	push   ecx
  436a3c:	0f b7 0d 1a c6 51 00 	movzx  ecx,WORD PTR ds:0x51c61a
  436a43:	51                   	push   ecx
  436a44:	0f b7 0d 18 c6 51 00 	movzx  ecx,WORD PTR ds:0x51c618
  436a4b:	51                   	push   ecx
  436a4c:	75 1d                	jne    0x436a6b
  436a4e:	0f b7 0d 14 c6 51 00 	movzx  ecx,WORD PTR ds:0x51c614
  436a55:	57                   	push   edi
  436a56:	51                   	push   ecx
  436a57:	0f b7 0d 16 c6 51 00 	movzx  ecx,WORD PTR ds:0x51c616
  436a5e:	51                   	push   ecx
  436a5f:	0f b7 0d 12 c6 51 00 	movzx  ecx,WORD PTR ds:0x51c612
  436a66:	51                   	push   ecx
  436a67:	50                   	push   eax
  436a68:	53                   	push   ebx
  436a69:	eb 14                	jmp    0x436a7f
  436a6b:	0f b7 0d 16 c6 51 00 	movzx  ecx,WORD PTR ds:0x51c616
  436a72:	51                   	push   ecx
  436a73:	57                   	push   edi
  436a74:	0f b7 0d 12 c6 51 00 	movzx  ecx,WORD PTR ds:0x51c612
  436a7b:	57                   	push   edi
  436a7c:	51                   	push   ecx
  436a7d:	50                   	push   eax
  436a7e:	57                   	push   edi
  436a7f:	53                   	push   ebx
  436a80:	e8 0e 01 00 00       	call   0x436b93
  436a85:	0f b7 05 ca c5 51 00 	movzx  eax,WORD PTR ds:0x51c5ca
  436a8c:	83 c4 2c             	add    esp,0x2c
  436a8f:	66 39 3d bc c5 51 00 	cmp    WORD PTR ds:0x51c5bc,di
  436a96:	50                   	push   eax
  436a97:	0f b7 05 c8 c5 51 00 	movzx  eax,WORD PTR ds:0x51c5c8
  436a9e:	50                   	push   eax
  436a9f:	0f b7 05 c6 c5 51 00 	movzx  eax,WORD PTR ds:0x51c5c6
  436aa6:	50                   	push   eax
  436aa7:	0f b7 05 c4 c5 51 00 	movzx  eax,WORD PTR ds:0x51c5c4
  436aae:	50                   	push   eax
  436aaf:	75 28                	jne    0x436ad9
  436ab1:	0f b7 05 c0 c5 51 00 	movzx  eax,WORD PTR ds:0x51c5c0
  436ab8:	57                   	push   edi
  436ab9:	50                   	push   eax
  436aba:	0f b7 05 c2 c5 51 00 	movzx  eax,WORD PTR ds:0x51c5c2
  436ac1:	50                   	push   eax
  436ac2:	0f b7 05 be c5 51 00 	movzx  eax,WORD PTR ds:0x51c5be
  436ac9:	50                   	push   eax
  436aca:	ff 76 14             	push   DWORD PTR [esi+0x14]
  436acd:	53                   	push   ebx
  436ace:	57                   	push   edi
  436acf:	e8 bf 00 00 00       	call   0x436b93
  436ad4:	83 c4 2c             	add    esp,0x2c
  436ad7:	eb 42                	jmp    0x436b1b
  436ad9:	0f b7 05 c2 c5 51 00 	movzx  eax,WORD PTR ds:0x51c5c2
  436ae0:	50                   	push   eax
  436ae1:	57                   	push   edi
  436ae2:	0f b7 05 be c5 51 00 	movzx  eax,WORD PTR ds:0x51c5be
  436ae9:	57                   	push   edi
  436aea:	50                   	push   eax
  436aeb:	ff 76 14             	push   DWORD PTR [esi+0x14]
  436aee:	57                   	push   edi
  436aef:	eb dd                	jmp    0x436ace
  436af1:	57                   	push   edi
  436af2:	57                   	push   edi
  436af3:	57                   	push   edi
  436af4:	6a 02                	push   0x2
  436af6:	57                   	push   edi
  436af7:	57                   	push   edi
  436af8:	53                   	push   ebx
  436af9:	6a 04                	push   0x4
  436afb:	50                   	push   eax
  436afc:	53                   	push   ebx
  436afd:	53                   	push   ebx
  436afe:	e8 90 00 00 00       	call   0x436b93
  436b03:	57                   	push   edi
  436b04:	57                   	push   edi
  436b05:	57                   	push   edi
  436b06:	6a 02                	push   0x2
  436b08:	57                   	push   edi
  436b09:	57                   	push   edi
  436b0a:	6a 05                	push   0x5
  436b0c:	6a 0a                	push   0xa
  436b0e:	ff 76 14             	push   DWORD PTR [esi+0x14]
  436b11:	53                   	push   ebx
  436b12:	57                   	push   edi
  436b13:	e8 7b 00 00 00       	call   0x436b93
  436b18:	83 c4 58             	add    esp,0x58
  436b1b:	8b 15 cc 9d 44 00    	mov    edx,DWORD PTR ds:0x449dcc
  436b21:	a1 dc 9d 44 00       	mov    eax,ds:0x449ddc
  436b26:	8b 4e 1c             	mov    ecx,DWORD PTR [esi+0x1c]
  436b29:	3b d0                	cmp    edx,eax
  436b2b:	7d 1e                	jge    0x436b4b
  436b2d:	3b ca                	cmp    ecx,edx
  436b2f:	0f 8c bf fe ff ff    	jl     0x4369f4
  436b35:	3b c8                	cmp    ecx,eax
  436b37:	0f 8f b7 fe ff ff    	jg     0x4369f4
  436b3d:	3b ca                	cmp    ecx,edx
  436b3f:	7e 1e                	jle    0x436b5f
  436b41:	3b c8                	cmp    ecx,eax
  436b43:	7d 1a                	jge    0x436b5f
  436b45:	8b c3                	mov    eax,ebx
  436b47:	5f                   	pop    edi
  436b48:	5e                   	pop    esi
  436b49:	5b                   	pop    ebx
  436b4a:	c3                   	ret
  436b4b:	3b c8                	cmp    ecx,eax
  436b4d:	7c f6                	jl     0x436b45
  436b4f:	3b ca                	cmp    ecx,edx
  436b51:	7f f2                	jg     0x436b45
  436b53:	3b c8                	cmp    ecx,eax
  436b55:	7e 08                	jle    0x436b5f
  436b57:	3b ca                	cmp    ecx,edx
  436b59:	0f 8c 95 fe ff ff    	jl     0x4369f4
  436b5f:	8b 46 08             	mov    eax,DWORD PTR [esi+0x8]
  436b62:	6b c0 3c             	imul   eax,eax,0x3c
  436b65:	03 46 04             	add    eax,DWORD PTR [esi+0x4]
  436b68:	6b c0 3c             	imul   eax,eax,0x3c
  436b6b:	03 06                	add    eax,DWORD PTR [esi]
  436b6d:	69 c0 e8 03 00 00    	imul   eax,eax,0x3e8
  436b73:	3b ca                	cmp    ecx,edx
  436b75:	75 0f                	jne    0x436b86
  436b77:	33 c9                	xor    ecx,ecx
  436b79:	3b 05 d0 9d 44 00    	cmp    eax,DWORD PTR ds:0x449dd0
  436b7f:	0f 9d c1             	setge  cl
  436b82:	8b c1                	mov    eax,ecx
  436b84:	eb c1                	jmp    0x436b47
  436b86:	33 c9                	xor    ecx,ecx
  436b88:	3b 05 e0 9d 44 00    	cmp    eax,DWORD PTR ds:0x449de0
  436b8e:	0f 9c c1             	setl   cl
  436b91:	eb ef                	jmp    0x436b82
