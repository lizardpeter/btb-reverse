
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00437bfa <.text+0x36bfa>:
  437bfa:	55                   	push   ebp
  437bfb:	8b ec                	mov    ebp,esp
  437bfd:	83 ec 1c             	sub    esp,0x1c
  437c00:	8b 4d 0c             	mov    ecx,DWORD PTR [ebp+0xc]
  437c03:	53                   	push   ebx
  437c04:	33 db                	xor    ebx,ebx
  437c06:	56                   	push   esi
  437c07:	f6 c1 80             	test   cl,0x80
  437c0a:	57                   	push   edi
  437c0b:	c7 45 e4 0c 00 00 00 	mov    DWORD PTR [ebp-0x1c],0xc
  437c12:	89 5d e8             	mov    DWORD PTR [ebp-0x18],ebx
  437c15:	74 09                	je     0x437c20
  437c17:	89 5d ec             	mov    DWORD PTR [ebp-0x14],ebx
  437c1a:	c6 45 ff 10          	mov    BYTE PTR [ebp-0x1],0x10
  437c1e:	eb 0b                	jmp    0x437c2b
  437c20:	80 65 ff 00          	and    BYTE PTR [ebp-0x1],0x0
  437c24:	c7 45 ec 01 00 00 00 	mov    DWORD PTR [ebp-0x14],0x1
  437c2b:	b8 00 80 00 00       	mov    eax,0x8000
  437c30:	85 c8                	test   eax,ecx
  437c32:	75 11                	jne    0x437c45
  437c34:	f6 c5 40             	test   ch,0x40
  437c37:	75 08                	jne    0x437c41
  437c39:	39 05 b0 c6 51 00    	cmp    DWORD PTR ds:0x51c6b0,eax
  437c3f:	74 04                	je     0x437c45
  437c41:	80 4d ff 80          	or     BYTE PTR [ebp-0x1],0x80
  437c45:	6a 03                	push   0x3
  437c47:	8b c1                	mov    eax,ecx
  437c49:	5e                   	pop    esi
  437c4a:	23 c6                	and    eax,esi
  437c4c:	2b c3                	sub    eax,ebx
  437c4e:	74 2d                	je     0x437c7d
  437c50:	48                   	dec    eax
  437c51:	74 21                	je     0x437c74
  437c53:	48                   	dec    eax
  437c54:	74 15                	je     0x437c6b
  437c56:	c7 05 c8 c3 51 00 16 	mov    DWORD PTR ds:0x51c3c8,0x16
  437c5d:	00 00 00 
  437c60:	89 1d cc c3 51 00    	mov    DWORD PTR ds:0x51c3cc,ebx
  437c66:	e9 25 02 00 00       	jmp    0x437e90
  437c6b:	c7 45 f4 00 00 00 c0 	mov    DWORD PTR [ebp-0xc],0xc0000000
  437c72:	eb 10                	jmp    0x437c84
  437c74:	c7 45 f4 00 00 00 40 	mov    DWORD PTR [ebp-0xc],0x40000000
  437c7b:	eb 07                	jmp    0x437c84
  437c7d:	c7 45 f4 00 00 00 80 	mov    DWORD PTR [ebp-0xc],0x80000000
  437c84:	8b 45 10             	mov    eax,DWORD PTR [ebp+0x10]
  437c87:	83 f8 10             	cmp    eax,0x10
  437c8a:	74 26                	je     0x437cb2
  437c8c:	83 f8 20             	cmp    eax,0x20
  437c8f:	74 18                	je     0x437ca9
  437c91:	83 f8 30             	cmp    eax,0x30
  437c94:	74 0a                	je     0x437ca0
  437c96:	83 f8 40             	cmp    eax,0x40
  437c99:	75 bb                	jne    0x437c56
  437c9b:	89 75 f0             	mov    DWORD PTR [ebp-0x10],esi
  437c9e:	eb 15                	jmp    0x437cb5
  437ca0:	c7 45 f0 02 00 00 00 	mov    DWORD PTR [ebp-0x10],0x2
  437ca7:	eb 0c                	jmp    0x437cb5
  437ca9:	c7 45 f0 01 00 00 00 	mov    DWORD PTR [ebp-0x10],0x1
  437cb0:	eb 03                	jmp    0x437cb5
  437cb2:	89 5d f0             	mov    DWORD PTR [ebp-0x10],ebx
  437cb5:	ba 00 07 00 00       	mov    edx,0x700
  437cba:	b8 00 04 00 00       	mov    eax,0x400
  437cbf:	23 ca                	and    ecx,edx
  437cc1:	bf 00 01 00 00       	mov    edi,0x100
  437cc6:	3b c8                	cmp    ecx,eax
  437cc8:	7f 35                	jg     0x437cff
  437cca:	74 2e                	je     0x437cfa
  437ccc:	3b cb                	cmp    ecx,ebx
  437cce:	74 2a                	je     0x437cfa
  437cd0:	3b cf                	cmp    ecx,edi
  437cd2:	74 1d                	je     0x437cf1
  437cd4:	81 f9 00 02 00 00    	cmp    ecx,0x200
  437cda:	74 3c                	je     0x437d18
  437cdc:	81 f9 00 03 00 00    	cmp    ecx,0x300
  437ce2:	0f 85 6e ff ff ff    	jne    0x437c56
  437ce8:	c7 45 f8 02 00 00 00 	mov    DWORD PTR [ebp-0x8],0x2
  437cef:	eb 37                	jmp    0x437d28
  437cf1:	c7 45 f8 04 00 00 00 	mov    DWORD PTR [ebp-0x8],0x4
  437cf8:	eb 2e                	jmp    0x437d28
  437cfa:	89 75 f8             	mov    DWORD PTR [ebp-0x8],esi
  437cfd:	eb 29                	jmp    0x437d28
  437cff:	81 f9 00 05 00 00    	cmp    ecx,0x500
  437d05:	74 1a                	je     0x437d21
  437d07:	81 f9 00 06 00 00    	cmp    ecx,0x600
  437d0d:	74 09                	je     0x437d18
  437d0f:	3b ca                	cmp    ecx,edx
  437d11:	74 0e                	je     0x437d21
  437d13:	e9 3e ff ff ff       	jmp    0x437c56
  437d18:	c7 45 f8 05 00 00 00 	mov    DWORD PTR [ebp-0x8],0x5
  437d1f:	eb 07                	jmp    0x437d28
  437d21:	c7 45 f8 01 00 00 00 	mov    DWORD PTR [ebp-0x8],0x1
  437d28:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  437d2b:	be 80 00 00 00       	mov    esi,0x80
  437d30:	85 c7                	test   edi,eax
  437d32:	74 13                	je     0x437d47
  437d34:	8b 0d d0 c3 51 00    	mov    ecx,DWORD PTR ds:0x51c3d0
  437d3a:	f7 d1                	not    ecx
  437d3c:	23 4d 14             	and    ecx,DWORD PTR [ebp+0x14]
  437d3f:	f6 c1 80             	test   cl,0x80
  437d42:	75 03                	jne    0x437d47
  437d44:	6a 01                	push   0x1
  437d46:	5e                   	pop    esi
  437d47:	a8 40                	test   al,0x40
  437d49:	74 0a                	je     0x437d55
  437d4b:	81 ce 00 00 00 04    	or     esi,0x4000000
  437d51:	80 4d f6 01          	or     BYTE PTR [ebp-0xa],0x1
  437d55:	f6 c4 10             	test   ah,0x10
  437d58:	74 02                	je     0x437d5c
  437d5a:	0b f7                	or     esi,edi
  437d5c:	a8 20                	test   al,0x20
  437d5e:	74 08                	je     0x437d68
  437d60:	81 ce 00 00 00 08    	or     esi,0x8000000
  437d66:	eb 0a                	jmp    0x437d72
  437d68:	a8 10                	test   al,0x10
  437d6a:	74 06                	je     0x437d72
  437d6c:	81 ce 00 00 00 10    	or     esi,0x10000000
  437d72:	e8 25 e4 ff ff       	call   0x43619c
  437d77:	8b d8                	mov    ebx,eax
  437d79:	83 cf ff             	or     edi,0xffffffff
  437d7c:	3b df                	cmp    ebx,edi
  437d7e:	75 13                	jne    0x437d93
  437d80:	83 25 cc c3 51 00 00 	and    DWORD PTR ds:0x51c3cc,0x0
  437d87:	c7 05 c8 c3 51 00 18 	mov    DWORD PTR ds:0x51c3c8,0x18
  437d8e:	00 00 00 
  437d91:	eb 3e                	jmp    0x437dd1
  437d93:	6a 00                	push   0x0
  437d95:	56                   	push   esi
  437d96:	ff 75 f8             	push   DWORD PTR [ebp-0x8]
  437d99:	8d 45 e4             	lea    eax,[ebp-0x1c]
  437d9c:	50                   	push   eax
  437d9d:	ff 75 f0             	push   DWORD PTR [ebp-0x10]
  437da0:	ff 75 f4             	push   DWORD PTR [ebp-0xc]
  437da3:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  437da6:	ff 15 50 b1 43 00    	call   DWORD PTR ds:0x43b150
  437dac:	8b f0                	mov    esi,eax
  437dae:	3b f7                	cmp    esi,edi
  437db0:	74 12                	je     0x437dc4
  437db2:	56                   	push   esi
  437db3:	ff 15 b0 b0 43 00    	call   DWORD PTR ds:0x43b0b0
  437db9:	85 c0                	test   eax,eax
  437dbb:	75 1b                	jne    0x437dd8
  437dbd:	56                   	push   esi
  437dbe:	ff 15 58 b1 43 00    	call   DWORD PTR ds:0x43b158
  437dc4:	ff 15 2c b1 43 00    	call   DWORD PTR ds:0x43b12c
  437dca:	50                   	push   eax
  437dcb:	e8 aa a9 ff ff       	call   0x43277a
  437dd0:	59                   	pop    ecx
  437dd1:	8b c7                	mov    eax,edi
  437dd3:	e9 d6 00 00 00       	jmp    0x437eae
  437dd8:	83 f8 02             	cmp    eax,0x2
  437ddb:	75 06                	jne    0x437de3
  437ddd:	80 4d ff 40          	or     BYTE PTR [ebp-0x1],0x40
  437de1:	eb 09                	jmp    0x437dec
  437de3:	83 f8 03             	cmp    eax,0x3
  437de6:	75 04                	jne    0x437dec
  437de8:	80 4d ff 08          	or     BYTE PTR [ebp-0x1],0x8
  437dec:	56                   	push   esi
  437ded:	53                   	push   ebx
  437dee:	e8 3e e4 ff ff       	call   0x436231
  437df3:	59                   	pop    ecx
  437df4:	8a 45 ff             	mov    al,BYTE PTR [ebp-0x1]
  437df7:	59                   	pop    ecx
  437df8:	8b f3                	mov    esi,ebx
  437dfa:	8b cb                	mov    ecx,ebx
  437dfc:	0c 01                	or     al,0x1
  437dfe:	c1 f9 05             	sar    ecx,0x5
  437e01:	83 e6 1f             	and    esi,0x1f
  437e04:	88 45 0b             	mov    BYTE PTR [ebp+0xb],al
  437e07:	8d 3c 8d 20 d9 51 00 	lea    edi,[ecx*4+0x51d920]
  437e0e:	c1 e6 03             	shl    esi,0x3
  437e11:	8b 0f                	mov    ecx,DWORD PTR [edi]
  437e13:	80 65 0b 48          	and    BYTE PTR [ebp+0xb],0x48
  437e17:	88 44 31 04          	mov    BYTE PTR [ecx+esi*1+0x4],al
  437e1b:	75 78                	jne    0x437e95
  437e1d:	a8 80                	test   al,0x80
  437e1f:	74 74                	je     0x437e95
  437e21:	f6 45 0c 02          	test   BYTE PTR [ebp+0xc],0x2
  437e25:	74 6e                	je     0x437e95
  437e27:	6a 02                	push   0x2
  437e29:	6a ff                	push   0xffffffff
  437e2b:	53                   	push   ebx
  437e2c:	e8 a6 f4 ff ff       	call   0x4372d7
  437e31:	83 c4 0c             	add    esp,0xc
  437e34:	83 f8 ff             	cmp    eax,0xffffffff
  437e37:	89 45 f0             	mov    DWORD PTR [ebp-0x10],eax
  437e3a:	75 0e                	jne    0x437e4a
  437e3c:	81 3d cc c3 51 00 83 	cmp    DWORD PTR ds:0x51c3cc,0x83
  437e43:	00 00 00 
  437e46:	74 4d                	je     0x437e95
  437e48:	eb 3f                	jmp    0x437e89
  437e4a:	80 65 13 00          	and    BYTE PTR [ebp+0x13],0x0
  437e4e:	8d 45 13             	lea    eax,[ebp+0x13]
  437e51:	6a 01                	push   0x1
  437e53:	50                   	push   eax
  437e54:	53                   	push   ebx
  437e55:	e8 07 bb ff ff       	call   0x433961
  437e5a:	83 c4 0c             	add    esp,0xc
  437e5d:	85 c0                	test   eax,eax
  437e5f:	75 16                	jne    0x437e77
  437e61:	80 7d 13 1a          	cmp    BYTE PTR [ebp+0x13],0x1a
  437e65:	75 10                	jne    0x437e77
  437e67:	ff 75 f0             	push   DWORD PTR [ebp-0x10]
  437e6a:	53                   	push   ebx
  437e6b:	e8 5c 12 00 00       	call   0x4390cc
  437e70:	59                   	pop    ecx
  437e71:	83 f8 ff             	cmp    eax,0xffffffff
  437e74:	59                   	pop    ecx
  437e75:	74 12                	je     0x437e89
  437e77:	6a 00                	push   0x0
  437e79:	6a 00                	push   0x0
  437e7b:	53                   	push   ebx
  437e7c:	e8 56 f4 ff ff       	call   0x4372d7
  437e81:	83 c4 0c             	add    esp,0xc
  437e84:	83 f8 ff             	cmp    eax,0xffffffff
  437e87:	75 0c                	jne    0x437e95
  437e89:	53                   	push   ebx
  437e8a:	e8 9a 92 ff ff       	call   0x431129
  437e8f:	59                   	pop    ecx
  437e90:	83 c8 ff             	or     eax,0xffffffff
  437e93:	eb 19                	jmp    0x437eae
  437e95:	80 7d 0b 00          	cmp    BYTE PTR [ebp+0xb],0x0
  437e99:	75 11                	jne    0x437eac
  437e9b:	f6 45 0c 08          	test   BYTE PTR [ebp+0xc],0x8
  437e9f:	74 0b                	je     0x437eac
  437ea1:	8b 07                	mov    eax,DWORD PTR [edi]
  437ea3:	80 4c 30 04 20       	or     BYTE PTR [eax+esi*1+0x4],0x20
  437ea8:	8d 44 30 04          	lea    eax,[eax+esi*1+0x4]
  437eac:	8b c3                	mov    eax,ebx
  437eae:	5f                   	pop    edi
  437eaf:	5e                   	pop    esi
  437eb0:	5b                   	pop    ebx
  437eb1:	c9                   	leave
  437eb2:	c3                   	ret
