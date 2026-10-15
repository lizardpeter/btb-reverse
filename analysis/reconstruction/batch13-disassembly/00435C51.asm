
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00435c51 <.text+0x34c51>:
  435c51:	51                   	push   ecx
  435c52:	51                   	push   ecx
  435c53:	a1 60 c5 51 00       	mov    eax,ds:0x51c560
  435c58:	53                   	push   ebx
  435c59:	55                   	push   ebp
  435c5a:	8b 2d bc b0 43 00    	mov    ebp,DWORD PTR ds:0x43b0bc
  435c60:	56                   	push   esi
  435c61:	57                   	push   edi
  435c62:	33 db                	xor    ebx,ebx
  435c64:	33 f6                	xor    esi,esi
  435c66:	33 ff                	xor    edi,edi
  435c68:	3b c3                	cmp    eax,ebx
  435c6a:	75 33                	jne    0x435c9f
  435c6c:	ff d5                	call   ebp
  435c6e:	8b f0                	mov    esi,eax
  435c70:	3b f3                	cmp    esi,ebx
  435c72:	74 0c                	je     0x435c80
  435c74:	c7 05 60 c5 51 00 01 	mov    DWORD PTR ds:0x51c560,0x1
  435c7b:	00 00 00 
  435c7e:	eb 28                	jmp    0x435ca8
  435c80:	ff 15 c0 b0 43 00    	call   DWORD PTR ds:0x43b0c0
  435c86:	8b f8                	mov    edi,eax
  435c88:	3b fb                	cmp    edi,ebx
  435c8a:	0f 84 ea 00 00 00    	je     0x435d7a
  435c90:	c7 05 60 c5 51 00 02 	mov    DWORD PTR ds:0x51c560,0x2
  435c97:	00 00 00 
  435c9a:	e9 8f 00 00 00       	jmp    0x435d2e
  435c9f:	83 f8 01             	cmp    eax,0x1
  435ca2:	0f 85 81 00 00 00    	jne    0x435d29
  435ca8:	3b f3                	cmp    esi,ebx
  435caa:	75 0c                	jne    0x435cb8
  435cac:	ff d5                	call   ebp
  435cae:	8b f0                	mov    esi,eax
  435cb0:	3b f3                	cmp    esi,ebx
  435cb2:	0f 84 c2 00 00 00    	je     0x435d7a
  435cb8:	66 39 1e             	cmp    WORD PTR [esi],bx
  435cbb:	8b c6                	mov    eax,esi
  435cbd:	74 0e                	je     0x435ccd
  435cbf:	40                   	inc    eax
  435cc0:	40                   	inc    eax
  435cc1:	66 39 18             	cmp    WORD PTR [eax],bx
  435cc4:	75 f9                	jne    0x435cbf
  435cc6:	40                   	inc    eax
  435cc7:	40                   	inc    eax
  435cc8:	66 39 18             	cmp    WORD PTR [eax],bx
  435ccb:	75 f2                	jne    0x435cbf
  435ccd:	2b c6                	sub    eax,esi
  435ccf:	8b 3d 80 b0 43 00    	mov    edi,DWORD PTR ds:0x43b080
  435cd5:	d1 f8                	sar    eax,1
  435cd7:	53                   	push   ebx
  435cd8:	53                   	push   ebx
  435cd9:	40                   	inc    eax
  435cda:	53                   	push   ebx
  435cdb:	53                   	push   ebx
  435cdc:	50                   	push   eax
  435cdd:	56                   	push   esi
  435cde:	53                   	push   ebx
  435cdf:	53                   	push   ebx
  435ce0:	89 44 24 34          	mov    DWORD PTR [esp+0x34],eax
  435ce4:	ff d7                	call   edi
  435ce6:	8b e8                	mov    ebp,eax
  435ce8:	3b eb                	cmp    ebp,ebx
  435cea:	74 32                	je     0x435d1e
  435cec:	55                   	push   ebp
  435ced:	e8 a1 b0 ff ff       	call   0x430d93
  435cf2:	3b c3                	cmp    eax,ebx
  435cf4:	59                   	pop    ecx
  435cf5:	89 44 24 10          	mov    DWORD PTR [esp+0x10],eax
  435cf9:	74 23                	je     0x435d1e
  435cfb:	53                   	push   ebx
  435cfc:	53                   	push   ebx
  435cfd:	55                   	push   ebp
  435cfe:	50                   	push   eax
  435cff:	ff 74 24 24          	push   DWORD PTR [esp+0x24]
  435d03:	56                   	push   esi
  435d04:	53                   	push   ebx
  435d05:	53                   	push   ebx
  435d06:	ff d7                	call   edi
  435d08:	85 c0                	test   eax,eax
  435d0a:	75 0e                	jne    0x435d1a
  435d0c:	ff 74 24 10          	push   DWORD PTR [esp+0x10]
  435d10:	e8 15 b0 ff ff       	call   0x430d2a
  435d15:	59                   	pop    ecx
  435d16:	89 5c 24 10          	mov    DWORD PTR [esp+0x10],ebx
  435d1a:	8b 5c 24 10          	mov    ebx,DWORD PTR [esp+0x10]
  435d1e:	56                   	push   esi
  435d1f:	ff 15 c4 b0 43 00    	call   DWORD PTR ds:0x43b0c4
  435d25:	8b c3                	mov    eax,ebx
  435d27:	eb 53                	jmp    0x435d7c
  435d29:	83 f8 02             	cmp    eax,0x2
  435d2c:	75 4c                	jne    0x435d7a
  435d2e:	3b fb                	cmp    edi,ebx
  435d30:	75 0c                	jne    0x435d3e
  435d32:	ff 15 c0 b0 43 00    	call   DWORD PTR ds:0x43b0c0
  435d38:	8b f8                	mov    edi,eax
  435d3a:	3b fb                	cmp    edi,ebx
  435d3c:	74 3c                	je     0x435d7a
  435d3e:	38 1f                	cmp    BYTE PTR [edi],bl
  435d40:	8b c7                	mov    eax,edi
  435d42:	74 0a                	je     0x435d4e
  435d44:	40                   	inc    eax
  435d45:	38 18                	cmp    BYTE PTR [eax],bl
  435d47:	75 fb                	jne    0x435d44
  435d49:	40                   	inc    eax
  435d4a:	38 18                	cmp    BYTE PTR [eax],bl
  435d4c:	75 f6                	jne    0x435d44
  435d4e:	2b c7                	sub    eax,edi
  435d50:	40                   	inc    eax
  435d51:	8b e8                	mov    ebp,eax
  435d53:	55                   	push   ebp
  435d54:	e8 3a b0 ff ff       	call   0x430d93
  435d59:	8b f0                	mov    esi,eax
  435d5b:	59                   	pop    ecx
  435d5c:	3b f3                	cmp    esi,ebx
  435d5e:	75 04                	jne    0x435d64
  435d60:	33 f6                	xor    esi,esi
  435d62:	eb 0b                	jmp    0x435d6f
  435d64:	55                   	push   ebp
  435d65:	57                   	push   edi
  435d66:	56                   	push   esi
  435d67:	e8 f4 dd ff ff       	call   0x433b60
  435d6c:	83 c4 0c             	add    esp,0xc
  435d6f:	57                   	push   edi
  435d70:	ff 15 c8 b0 43 00    	call   DWORD PTR ds:0x43b0c8
  435d76:	8b c6                	mov    eax,esi
  435d78:	eb 02                	jmp    0x435d7c
  435d7a:	33 c0                	xor    eax,eax
  435d7c:	5f                   	pop    edi
  435d7d:	5e                   	pop    esi
  435d7e:	5d                   	pop    ebp
  435d7f:	5b                   	pop    ebx
  435d80:	59                   	pop    ecx
  435d81:	59                   	pop    ecx
  435d82:	c3                   	ret
