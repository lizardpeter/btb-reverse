
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00436b93 <.text+0x35b93>:
  436b93:	55                   	push   ebp
  436b94:	8b ec                	mov    ebp,esp
  436b96:	83 7d 0c 01          	cmp    DWORD PTR [ebp+0xc],0x1
  436b9a:	53                   	push   ebx
  436b9b:	8b 5d 10             	mov    ebx,DWORD PTR [ebp+0x10]
  436b9e:	56                   	push   esi
  436b9f:	0f 85 89 00 00 00    	jne    0x436c2e
  436ba5:	8b 45 14             	mov    eax,DWORD PTR [ebp+0x14]
  436ba8:	89 5d 10             	mov    DWORD PTR [ebp+0x10],ebx
  436bab:	83 65 10 03          	and    DWORD PTR [ebp+0x10],0x3
  436baf:	8b f0                	mov    esi,eax
  436bb1:	75 0b                	jne    0x436bbe
  436bb3:	c1 e6 02             	shl    esi,0x2
  436bb6:	8b 86 e0 9d 44 00    	mov    eax,DWORD PTR [esi+0x449de0]
  436bbc:	eb 09                	jmp    0x436bc7
  436bbe:	c1 e6 02             	shl    esi,0x2
  436bc1:	8b 86 14 9e 44 00    	mov    eax,DWORD PTR [esi+0x449e14]
  436bc7:	8b d3                	mov    edx,ebx
  436bc9:	8d 48 01             	lea    ecx,[eax+0x1]
  436bcc:	69 d2 6d 01 00 00    	imul   edx,edx,0x16d
  436bd2:	8d 43 ff             	lea    eax,[ebx-0x1]
  436bd5:	57                   	push   edi
  436bd6:	c1 f8 02             	sar    eax,0x2
  436bd9:	8b f9                	mov    edi,ecx
  436bdb:	6a 07                	push   0x7
  436bdd:	03 f8                	add    edi,eax
  436bdf:	8d 84 3a 25 9c ff ff 	lea    eax,[edx+edi*1-0x63db]
  436be6:	5f                   	pop    edi
  436be7:	99                   	cdq
  436be8:	f7 ff                	idiv   edi
  436bea:	8b 45 18             	mov    eax,DWORD PTR [ebp+0x18]
  436bed:	5f                   	pop    edi
  436bee:	3b 55 1c             	cmp    edx,DWORD PTR [ebp+0x1c]
  436bf1:	7f 0e                	jg     0x436c01
  436bf3:	6b c0 07             	imul   eax,eax,0x7
  436bf6:	2b c2                	sub    eax,edx
  436bf8:	03 45 1c             	add    eax,DWORD PTR [ebp+0x1c]
  436bfb:	8d 4c 01 f9          	lea    ecx,[ecx+eax*1-0x7]
  436bff:	eb 0a                	jmp    0x436c0b
  436c01:	6b c0 07             	imul   eax,eax,0x7
  436c04:	2b c2                	sub    eax,edx
  436c06:	03 45 1c             	add    eax,DWORD PTR [ebp+0x1c]
  436c09:	03 c8                	add    ecx,eax
  436c0b:	83 7d 18 05          	cmp    DWORD PTR [ebp+0x18],0x5
  436c0f:	75 38                	jne    0x436c49
  436c11:	83 7d 10 00          	cmp    DWORD PTR [ebp+0x10],0x0
  436c15:	75 08                	jne    0x436c1f
  436c17:	8b b6 e4 9d 44 00    	mov    esi,DWORD PTR [esi+0x449de4]
  436c1d:	eb 06                	jmp    0x436c25
  436c1f:	8b b6 18 9e 44 00    	mov    esi,DWORD PTR [esi+0x449e18]
  436c25:	3b ce                	cmp    ecx,esi
  436c27:	7e 20                	jle    0x436c49
  436c29:	83 e9 07             	sub    ecx,0x7
  436c2c:	eb 1b                	jmp    0x436c49
  436c2e:	8b 45 14             	mov    eax,DWORD PTR [ebp+0x14]
  436c31:	f6 c3 03             	test   bl,0x3
  436c34:	75 09                	jne    0x436c3f
  436c36:	8b 0c 85 e0 9d 44 00 	mov    ecx,DWORD PTR [eax*4+0x449de0]
  436c3d:	eb 07                	jmp    0x436c46
  436c3f:	8b 0c 85 14 9e 44 00 	mov    ecx,DWORD PTR [eax*4+0x449e14]
  436c46:	03 4d 20             	add    ecx,DWORD PTR [ebp+0x20]
  436c49:	83 7d 08 01          	cmp    DWORD PTR [ebp+0x8],0x1
  436c4d:	75 2b                	jne    0x436c7a
  436c4f:	8b 45 24             	mov    eax,DWORD PTR [ebp+0x24]
  436c52:	89 0d cc 9d 44 00    	mov    DWORD PTR ds:0x449dcc,ecx
  436c58:	6b c0 3c             	imul   eax,eax,0x3c
  436c5b:	03 45 28             	add    eax,DWORD PTR [ebp+0x28]
  436c5e:	89 1d c8 9d 44 00    	mov    DWORD PTR ds:0x449dc8,ebx
  436c64:	6b c0 3c             	imul   eax,eax,0x3c
  436c67:	03 45 2c             	add    eax,DWORD PTR [ebp+0x2c]
  436c6a:	69 c0 e8 03 00 00    	imul   eax,eax,0x3e8
  436c70:	03 45 30             	add    eax,DWORD PTR [ebp+0x30]
  436c73:	a3 d0 9d 44 00       	mov    ds:0x449dd0,eax
  436c78:	eb 55                	jmp    0x436ccf
  436c7a:	8b 45 24             	mov    eax,DWORD PTR [ebp+0x24]
  436c7d:	89 0d dc 9d 44 00    	mov    DWORD PTR ds:0x449ddc,ecx
  436c83:	6b c0 3c             	imul   eax,eax,0x3c
  436c86:	03 45 28             	add    eax,DWORD PTR [ebp+0x28]
  436c89:	6b c0 3c             	imul   eax,eax,0x3c
  436c8c:	03 05 38 9d 44 00    	add    eax,DWORD PTR ds:0x449d38
  436c92:	03 45 2c             	add    eax,DWORD PTR [ebp+0x2c]
  436c95:	69 c0 e8 03 00 00    	imul   eax,eax,0x3e8
  436c9b:	03 45 30             	add    eax,DWORD PTR [ebp+0x30]
  436c9e:	a3 e0 9d 44 00       	mov    ds:0x449de0,eax
  436ca3:	79 0d                	jns    0x436cb2
  436ca5:	05 00 5c 26 05       	add    eax,0x5265c00
  436caa:	49                   	dec    ecx
  436cab:	a3 e0 9d 44 00       	mov    ds:0x449de0,eax
  436cb0:	eb 11                	jmp    0x436cc3
  436cb2:	ba 00 5c 26 05       	mov    edx,0x5265c00
  436cb7:	3b c2                	cmp    eax,edx
  436cb9:	7c 0e                	jl     0x436cc9
  436cbb:	2b c2                	sub    eax,edx
  436cbd:	41                   	inc    ecx
  436cbe:	a3 e0 9d 44 00       	mov    ds:0x449de0,eax
  436cc3:	89 0d dc 9d 44 00    	mov    DWORD PTR ds:0x449ddc,ecx
  436cc9:	89 1d d8 9d 44 00    	mov    DWORD PTR ds:0x449dd8,ebx
  436ccf:	5e                   	pop    esi
  436cd0:	5b                   	pop    ebx
  436cd1:	5d                   	pop    ebp
  436cd2:	c3                   	ret
