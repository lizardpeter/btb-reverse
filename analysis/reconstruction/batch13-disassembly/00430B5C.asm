
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00430b5c <.text+0x2fb5c>:
  430b5c:	55                   	push   ebp
  430b5d:	8b ec                	mov    ebp,esp
  430b5f:	81 ec f8 00 00 00    	sub    esp,0xf8
  430b65:	53                   	push   ebx
  430b66:	56                   	push   esi
  430b67:	8b 75 0c             	mov    esi,DWORD PTR [ebp+0xc]
  430b6a:	57                   	push   edi
  430b6b:	83 fe 02             	cmp    esi,0x2
  430b6e:	0f 82 37 01 00 00    	jb     0x430cab
  430b74:	83 7d 10 00          	cmp    DWORD PTR [ebp+0x10],0x0
  430b78:	0f 84 2d 01 00 00    	je     0x430cab
  430b7e:	83 65 fc 00          	and    DWORD PTR [ebp-0x4],0x0
  430b82:	4e                   	dec    esi
  430b83:	0f af 75 10          	imul   esi,DWORD PTR [ebp+0x10]
  430b87:	8b 5d 08             	mov    ebx,DWORD PTR [ebp+0x8]
  430b8a:	8d 45 80             	lea    eax,[ebp-0x80]
  430b8d:	89 45 08             	mov    DWORD PTR [ebp+0x8],eax
  430b90:	8d 85 08 ff ff ff    	lea    eax,[ebp-0xf8]
  430b96:	03 f3                	add    esi,ebx
  430b98:	89 45 0c             	mov    DWORD PTR [ebp+0xc],eax
  430b9b:	8b 7d 10             	mov    edi,DWORD PTR [ebp+0x10]
  430b9e:	8b c6                	mov    eax,esi
  430ba0:	2b c3                	sub    eax,ebx
  430ba2:	33 d2                	xor    edx,edx
  430ba4:	f7 f7                	div    edi
  430ba6:	40                   	inc    eax
  430ba7:	83 f8 08             	cmp    eax,0x8
  430baa:	77 2f                	ja     0x430bdb
  430bac:	ff 75 14             	push   DWORD PTR [ebp+0x14]
  430baf:	57                   	push   edi
  430bb0:	56                   	push   esi
  430bb1:	53                   	push   ebx
  430bb2:	e8 f9 00 00 00       	call   0x430cb0
  430bb7:	83 c4 10             	add    esp,0x10
  430bba:	ff 4d fc             	dec    DWORD PTR [ebp-0x4]
  430bbd:	83 6d 0c 04          	sub    DWORD PTR [ebp+0xc],0x4
  430bc1:	83 6d 08 04          	sub    DWORD PTR [ebp+0x8],0x4
  430bc5:	83 7d fc 00          	cmp    DWORD PTR [ebp-0x4],0x0
  430bc9:	0f 8c dc 00 00 00    	jl     0x430cab
  430bcf:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  430bd2:	8b 18                	mov    ebx,DWORD PTR [eax]
  430bd4:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  430bd7:	8b 30                	mov    esi,DWORD PTR [eax]
  430bd9:	eb c0                	jmp    0x430b9b
  430bdb:	d1 e8                	shr    eax,1
  430bdd:	0f af c7             	imul   eax,edi
  430be0:	57                   	push   edi
  430be1:	03 c3                	add    eax,ebx
  430be3:	53                   	push   ebx
  430be4:	50                   	push   eax
  430be5:	e8 14 01 00 00       	call   0x430cfe
  430bea:	83 c4 0c             	add    esp,0xc
  430bed:	89 5d f8             	mov    DWORD PTR [ebp-0x8],ebx
  430bf0:	03 fe                	add    edi,esi
  430bf2:	8b 45 f8             	mov    eax,DWORD PTR [ebp-0x8]
  430bf5:	03 45 10             	add    eax,DWORD PTR [ebp+0x10]
  430bf8:	3b c6                	cmp    eax,esi
  430bfa:	89 45 f8             	mov    DWORD PTR [ebp-0x8],eax
  430bfd:	77 0b                	ja     0x430c0a
  430bff:	53                   	push   ebx
  430c00:	50                   	push   eax
  430c01:	ff 55 14             	call   DWORD PTR [ebp+0x14]
  430c04:	59                   	pop    ecx
  430c05:	85 c0                	test   eax,eax
  430c07:	59                   	pop    ecx
  430c08:	7e e8                	jle    0x430bf2
  430c0a:	2b 7d 10             	sub    edi,DWORD PTR [ebp+0x10]
  430c0d:	3b fb                	cmp    edi,ebx
  430c0f:	76 0b                	jbe    0x430c1c
  430c11:	53                   	push   ebx
  430c12:	57                   	push   edi
  430c13:	ff 55 14             	call   DWORD PTR [ebp+0x14]
  430c16:	59                   	pop    ecx
  430c17:	85 c0                	test   eax,eax
  430c19:	59                   	pop    ecx
  430c1a:	7d ee                	jge    0x430c0a
  430c1c:	ff 75 10             	push   DWORD PTR [ebp+0x10]
  430c1f:	3b 7d f8             	cmp    edi,DWORD PTR [ebp-0x8]
  430c22:	57                   	push   edi
  430c23:	72 0d                	jb     0x430c32
  430c25:	ff 75 f8             	push   DWORD PTR [ebp-0x8]
  430c28:	e8 d1 00 00 00       	call   0x430cfe
  430c2d:	83 c4 0c             	add    esp,0xc
  430c30:	eb c0                	jmp    0x430bf2
  430c32:	53                   	push   ebx
  430c33:	e8 c6 00 00 00       	call   0x430cfe
  430c38:	8b 45 f8             	mov    eax,DWORD PTR [ebp-0x8]
  430c3b:	8b cf                	mov    ecx,edi
  430c3d:	2b cb                	sub    ecx,ebx
  430c3f:	8b d6                	mov    edx,esi
  430c41:	83 c4 0c             	add    esp,0xc
  430c44:	49                   	dec    ecx
  430c45:	2b d0                	sub    edx,eax
  430c47:	3b ca                	cmp    ecx,edx
  430c49:	7c 30                	jl     0x430c7b
  430c4b:	8b 4d 10             	mov    ecx,DWORD PTR [ebp+0x10]
  430c4e:	8d 14 0b             	lea    edx,[ebx+ecx*1]
  430c51:	3b d7                	cmp    edx,edi
  430c53:	73 17                	jae    0x430c6c
  430c55:	8b 55 0c             	mov    edx,DWORD PTR [ebp+0xc]
  430c58:	83 45 0c 04          	add    DWORD PTR [ebp+0xc],0x4
  430c5c:	2b f9                	sub    edi,ecx
  430c5e:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  430c61:	ff 45 fc             	inc    DWORD PTR [ebp-0x4]
  430c64:	83 45 08 04          	add    DWORD PTR [ebp+0x8],0x4
  430c68:	89 1a                	mov    DWORD PTR [edx],ebx
  430c6a:	89 39                	mov    DWORD PTR [ecx],edi
  430c6c:	3b c6                	cmp    eax,esi
  430c6e:	0f 83 46 ff ff ff    	jae    0x430bba
  430c74:	8b d8                	mov    ebx,eax
  430c76:	e9 20 ff ff ff       	jmp    0x430b9b
  430c7b:	3b c6                	cmp    eax,esi
  430c7d:	73 15                	jae    0x430c94
  430c7f:	8b 4d 0c             	mov    ecx,DWORD PTR [ebp+0xc]
  430c82:	ff 45 fc             	inc    DWORD PTR [ebp-0x4]
  430c85:	83 45 0c 04          	add    DWORD PTR [ebp+0xc],0x4
  430c89:	89 01                	mov    DWORD PTR [ecx],eax
  430c8b:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  430c8e:	83 45 08 04          	add    DWORD PTR [ebp+0x8],0x4
  430c92:	89 30                	mov    DWORD PTR [eax],esi
  430c94:	8b 45 10             	mov    eax,DWORD PTR [ebp+0x10]
  430c97:	03 c3                	add    eax,ebx
  430c99:	3b c7                	cmp    eax,edi
  430c9b:	0f 83 19 ff ff ff    	jae    0x430bba
  430ca1:	2b 7d 10             	sub    edi,DWORD PTR [ebp+0x10]
  430ca4:	8b f7                	mov    esi,edi
  430ca6:	e9 f0 fe ff ff       	jmp    0x430b9b
  430cab:	5f                   	pop    edi
  430cac:	5e                   	pop    esi
  430cad:	5b                   	pop    ebx
  430cae:	c9                   	leave
  430caf:	c3                   	ret
