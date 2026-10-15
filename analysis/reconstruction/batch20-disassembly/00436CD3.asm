
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00436cd3 <.text+0x35cd3>:
  436cd3:	55                   	push   ebp
  436cd4:	8b ec                	mov    ebp,esp
  436cd6:	83 ec 18             	sub    esp,0x18
  436cd9:	53                   	push   ebx
  436cda:	56                   	push   esi
  436cdb:	57                   	push   edi
  436cdc:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  436cdf:	e8 88 01 00 00       	call   0x436e6c
  436ce4:	8b f0                	mov    esi,eax
  436ce6:	59                   	pop    ecx
  436ce7:	3b 35 b8 c6 51 00    	cmp    esi,DWORD PTR ds:0x51c6b8
  436ced:	89 75 08             	mov    DWORD PTR [ebp+0x8],esi
  436cf0:	0f 84 6a 01 00 00    	je     0x436e60
  436cf6:	33 db                	xor    ebx,ebx
  436cf8:	3b f3                	cmp    esi,ebx
  436cfa:	0f 84 56 01 00 00    	je     0x436e56
  436d00:	33 d2                	xor    edx,edx
  436d02:	b8 58 9e 44 00       	mov    eax,0x449e58
  436d07:	39 30                	cmp    DWORD PTR [eax],esi
  436d09:	74 72                	je     0x436d7d
  436d0b:	83 c0 30             	add    eax,0x30
  436d0e:	42                   	inc    edx
  436d0f:	3d 48 9f 44 00       	cmp    eax,0x449f48
  436d14:	72 f1                	jb     0x436d07
  436d16:	8d 45 e8             	lea    eax,[ebp-0x18]
  436d19:	50                   	push   eax
  436d1a:	56                   	push   esi
  436d1b:	ff 15 9c b0 43 00    	call   DWORD PTR ds:0x43b09c
  436d21:	83 f8 01             	cmp    eax,0x1
  436d24:	0f 85 24 01 00 00    	jne    0x436e4e
  436d2a:	6a 40                	push   0x40
  436d2c:	33 c0                	xor    eax,eax
  436d2e:	59                   	pop    ecx
  436d2f:	bf e0 c7 51 00       	mov    edi,0x51c7e0
  436d34:	83 7d e8 01          	cmp    DWORD PTR [ebp-0x18],0x1
  436d38:	89 35 b8 c6 51 00    	mov    DWORD PTR ds:0x51c6b8,esi
  436d3e:	f3 ab                	rep stos DWORD PTR es:[edi],eax
  436d40:	aa                   	stos   BYTE PTR es:[edi],al
  436d41:	89 1d e4 c8 51 00    	mov    DWORD PTR ds:0x51c8e4,ebx
  436d47:	0f 86 ef 00 00 00    	jbe    0x436e3c
  436d4d:	80 7d ee 00          	cmp    BYTE PTR [ebp-0x12],0x0
  436d51:	0f 84 bb 00 00 00    	je     0x436e12
  436d57:	8d 4d ef             	lea    ecx,[ebp-0x11]
  436d5a:	8a 11                	mov    dl,BYTE PTR [ecx]
  436d5c:	84 d2                	test   dl,dl
  436d5e:	0f 84 ae 00 00 00    	je     0x436e12
  436d64:	0f b6 41 ff          	movzx  eax,BYTE PTR [ecx-0x1]
  436d68:	0f b6 d2             	movzx  edx,dl
  436d6b:	3b c2                	cmp    eax,edx
  436d6d:	0f 87 93 00 00 00    	ja     0x436e06
  436d73:	80 88 e1 c7 51 00 04 	or     BYTE PTR [eax+0x51c7e1],0x4
  436d7a:	40                   	inc    eax
  436d7b:	eb ee                	jmp    0x436d6b
  436d7d:	6a 40                	push   0x40
  436d7f:	33 c0                	xor    eax,eax
  436d81:	59                   	pop    ecx
  436d82:	bf e0 c7 51 00       	mov    edi,0x51c7e0
  436d87:	f3 ab                	rep stos DWORD PTR es:[edi],eax
  436d89:	8d 34 52             	lea    esi,[edx+edx*2]
  436d8c:	89 5d fc             	mov    DWORD PTR [ebp-0x4],ebx
  436d8f:	c1 e6 04             	shl    esi,0x4
  436d92:	aa                   	stos   BYTE PTR es:[edi],al
  436d93:	8d 9e 68 9e 44 00    	lea    ebx,[esi+0x449e68]
  436d99:	80 3b 00             	cmp    BYTE PTR [ebx],0x0
  436d9c:	8b cb                	mov    ecx,ebx
  436d9e:	74 2c                	je     0x436dcc
  436da0:	8a 51 01             	mov    dl,BYTE PTR [ecx+0x1]
  436da3:	84 d2                	test   dl,dl
  436da5:	74 25                	je     0x436dcc
  436da7:	0f b6 01             	movzx  eax,BYTE PTR [ecx]
  436daa:	0f b6 fa             	movzx  edi,dl
  436dad:	3b c7                	cmp    eax,edi
  436daf:	77 14                	ja     0x436dc5
  436db1:	8b 55 fc             	mov    edx,DWORD PTR [ebp-0x4]
  436db4:	8a 92 50 9e 44 00    	mov    dl,BYTE PTR [edx+0x449e50]
  436dba:	08 90 e1 c7 51 00    	or     BYTE PTR [eax+0x51c7e1],dl
  436dc0:	40                   	inc    eax
  436dc1:	3b c7                	cmp    eax,edi
  436dc3:	76 f5                	jbe    0x436dba
  436dc5:	41                   	inc    ecx
  436dc6:	41                   	inc    ecx
  436dc7:	80 39 00             	cmp    BYTE PTR [ecx],0x0
  436dca:	75 d4                	jne    0x436da0
  436dcc:	ff 45 fc             	inc    DWORD PTR [ebp-0x4]
  436dcf:	83 c3 08             	add    ebx,0x8
  436dd2:	83 7d fc 04          	cmp    DWORD PTR [ebp-0x4],0x4
  436dd6:	72 c1                	jb     0x436d99
  436dd8:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  436ddb:	c7 05 cc c6 51 00 01 	mov    DWORD PTR ds:0x51c6cc,0x1
  436de2:	00 00 00 
  436de5:	50                   	push   eax
  436de6:	a3 b8 c6 51 00       	mov    ds:0x51c6b8,eax
  436deb:	e8 c6 00 00 00       	call   0x436eb6
  436df0:	8d b6 5c 9e 44 00    	lea    esi,[esi+0x449e5c]
  436df6:	bf c0 c6 51 00       	mov    edi,0x51c6c0
  436dfb:	a5                   	movs   DWORD PTR es:[edi],DWORD PTR ds:[esi]
  436dfc:	a5                   	movs   DWORD PTR es:[edi],DWORD PTR ds:[esi]
  436dfd:	59                   	pop    ecx
  436dfe:	a3 e4 c8 51 00       	mov    ds:0x51c8e4,eax
  436e03:	a5                   	movs   DWORD PTR es:[edi],DWORD PTR ds:[esi]
  436e04:	eb 55                	jmp    0x436e5b
  436e06:	41                   	inc    ecx
  436e07:	41                   	inc    ecx
  436e08:	80 79 ff 00          	cmp    BYTE PTR [ecx-0x1],0x0
  436e0c:	0f 85 48 ff ff ff    	jne    0x436d5a
  436e12:	6a 01                	push   0x1
  436e14:	58                   	pop    eax
  436e15:	80 88 e1 c7 51 00 08 	or     BYTE PTR [eax+0x51c7e1],0x8
  436e1c:	40                   	inc    eax
  436e1d:	3d ff 00 00 00       	cmp    eax,0xff
  436e22:	72 f1                	jb     0x436e15
  436e24:	56                   	push   esi
  436e25:	e8 8c 00 00 00       	call   0x436eb6
  436e2a:	59                   	pop    ecx
  436e2b:	a3 e4 c8 51 00       	mov    ds:0x51c8e4,eax
  436e30:	c7 05 cc c6 51 00 01 	mov    DWORD PTR ds:0x51c6cc,0x1
  436e37:	00 00 00 
  436e3a:	eb 06                	jmp    0x436e42
  436e3c:	89 1d cc c6 51 00    	mov    DWORD PTR ds:0x51c6cc,ebx
  436e42:	33 c0                	xor    eax,eax
  436e44:	bf c0 c6 51 00       	mov    edi,0x51c6c0
  436e49:	ab                   	stos   DWORD PTR es:[edi],eax
  436e4a:	ab                   	stos   DWORD PTR es:[edi],eax
  436e4b:	ab                   	stos   DWORD PTR es:[edi],eax
  436e4c:	eb 0d                	jmp    0x436e5b
  436e4e:	39 1d 2c c6 51 00    	cmp    DWORD PTR ds:0x51c62c,ebx
  436e54:	74 0e                	je     0x436e64
  436e56:	e8 8e 00 00 00       	call   0x436ee9
  436e5b:	e8 b2 00 00 00       	call   0x436f12
  436e60:	33 c0                	xor    eax,eax
  436e62:	eb 03                	jmp    0x436e67
  436e64:	83 c8 ff             	or     eax,0xffffffff
  436e67:	5f                   	pop    edi
  436e68:	5e                   	pop    esi
  436e69:	5b                   	pop    ebx
  436e6a:	c9                   	leave
  436e6b:	c3                   	ret
