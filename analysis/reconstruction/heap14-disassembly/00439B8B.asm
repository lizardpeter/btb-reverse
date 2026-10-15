
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00439b8b <.text+0x38b8b>:
  439b8b:	55                   	push   ebp
  439b8c:	8b ec                	mov    ebp,esp
  439b8e:	51                   	push   ecx
  439b8f:	83 7d 08 00          	cmp    DWORD PTR [ebp+0x8],0x0
  439b93:	53                   	push   ebx
  439b94:	56                   	push   esi
  439b95:	57                   	push   edi
  439b96:	75 0e                	jne    0x439ba6
  439b98:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  439b9b:	e8 f3 71 ff ff       	call   0x430d93
  439ba0:	59                   	pop    ecx
  439ba1:	e9 80 02 00 00       	jmp    0x439e26
  439ba6:	8b 75 0c             	mov    esi,DWORD PTR [ebp+0xc]
  439ba9:	85 f6                	test   esi,esi
  439bab:	75 0e                	jne    0x439bbb
  439bad:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  439bb0:	e8 75 71 ff ff       	call   0x430d2a
  439bb5:	59                   	pop    ecx
  439bb6:	e9 69 02 00 00       	jmp    0x439e24
  439bbb:	a1 44 da 51 00       	mov    eax,ds:0x51da44
  439bc0:	83 f8 03             	cmp    eax,0x3
  439bc3:	0f 85 02 01 00 00    	jne    0x439ccb
  439bc9:	33 ff                	xor    edi,edi
  439bcb:	83 fe e0             	cmp    esi,0xffffffe0
  439bce:	0f 87 d3 00 00 00    	ja     0x439ca7
  439bd4:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  439bd7:	e8 68 a8 ff ff       	call   0x434444
  439bdc:	8b d8                	mov    ebx,eax
  439bde:	59                   	pop    ecx
  439bdf:	85 db                	test   ebx,ebx
  439be1:	0f 84 9b 00 00 00    	je     0x439c82
  439be7:	3b 35 3c da 51 00    	cmp    esi,DWORD PTR ds:0x51da3c
  439bed:	77 4c                	ja     0x439c3b
  439bef:	8b 7d 08             	mov    edi,DWORD PTR [ebp+0x8]
  439bf2:	56                   	push   esi
  439bf3:	57                   	push   edi
  439bf4:	53                   	push   ebx
  439bf5:	e8 53 b0 ff ff       	call   0x434c4d
  439bfa:	83 c4 0c             	add    esp,0xc
  439bfd:	85 c0                	test   eax,eax
  439bff:	75 36                	jne    0x439c37
  439c01:	56                   	push   esi
  439c02:	e8 91 ab ff ff       	call   0x434798
  439c07:	8b f8                	mov    edi,eax
  439c09:	59                   	pop    ecx
  439c0a:	85 ff                	test   edi,edi
  439c0c:	74 2d                	je     0x439c3b
  439c0e:	8b 5d 08             	mov    ebx,DWORD PTR [ebp+0x8]
  439c11:	8b 43 fc             	mov    eax,DWORD PTR [ebx-0x4]
  439c14:	48                   	dec    eax
  439c15:	3b c6                	cmp    eax,esi
  439c17:	72 02                	jb     0x439c1b
  439c19:	8b c6                	mov    eax,esi
  439c1b:	50                   	push   eax
  439c1c:	53                   	push   ebx
  439c1d:	57                   	push   edi
  439c1e:	e8 3d 9f ff ff       	call   0x433b60
  439c23:	53                   	push   ebx
  439c24:	e8 1b a8 ff ff       	call   0x434444
  439c29:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  439c2c:	8b d8                	mov    ebx,eax
  439c2e:	53                   	push   ebx
  439c2f:	e8 3b a8 ff ff       	call   0x43446f
  439c34:	83 c4 18             	add    esp,0x18
  439c37:	85 ff                	test   edi,edi
  439c39:	75 43                	jne    0x439c7e
  439c3b:	85 f6                	test   esi,esi
  439c3d:	75 03                	jne    0x439c42
  439c3f:	6a 01                	push   0x1
  439c41:	5e                   	pop    esi
  439c42:	83 c6 0f             	add    esi,0xf
  439c45:	83 e6 f0             	and    esi,0xfffffff0
  439c48:	56                   	push   esi
  439c49:	6a 00                	push   0x0
  439c4b:	ff 35 40 da 51 00    	push   DWORD PTR ds:0x51da40
  439c51:	ff 15 08 b1 43 00    	call   DWORD PTR ds:0x43b108
  439c57:	8b f8                	mov    edi,eax
  439c59:	85 ff                	test   edi,edi
  439c5b:	74 21                	je     0x439c7e
  439c5d:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  439c60:	8b 41 fc             	mov    eax,DWORD PTR [ecx-0x4]
  439c63:	48                   	dec    eax
  439c64:	3b c6                	cmp    eax,esi
  439c66:	72 02                	jb     0x439c6a
  439c68:	8b c6                	mov    eax,esi
  439c6a:	50                   	push   eax
  439c6b:	51                   	push   ecx
  439c6c:	57                   	push   edi
  439c6d:	e8 ee 9e ff ff       	call   0x433b60
  439c72:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  439c75:	53                   	push   ebx
  439c76:	e8 f4 a7 ff ff       	call   0x43446f
  439c7b:	83 c4 14             	add    esp,0x14
  439c7e:	85 db                	test   ebx,ebx
  439c80:	75 21                	jne    0x439ca3
  439c82:	85 f6                	test   esi,esi
  439c84:	75 03                	jne    0x439c89
  439c86:	6a 01                	push   0x1
  439c88:	5e                   	pop    esi
  439c89:	83 c6 0f             	add    esi,0xf
  439c8c:	83 e6 f0             	and    esi,0xfffffff0
  439c8f:	56                   	push   esi
  439c90:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  439c93:	6a 00                	push   0x0
  439c95:	ff 35 40 da 51 00    	push   DWORD PTR ds:0x51da40
  439c9b:	ff 15 d4 b0 43 00    	call   DWORD PTR ds:0x43b0d4
  439ca1:	8b f8                	mov    edi,eax
  439ca3:	85 ff                	test   edi,edi
  439ca5:	75 1d                	jne    0x439cc4
  439ca7:	83 3d 48 c4 51 00 00 	cmp    DWORD PTR ds:0x51c448,0x0
  439cae:	74 14                	je     0x439cc4
  439cb0:	56                   	push   esi
  439cb1:	e8 5a b9 ff ff       	call   0x435610
  439cb6:	85 c0                	test   eax,eax
  439cb8:	59                   	pop    ecx
  439cb9:	0f 85 0a ff ff ff    	jne    0x439bc9
  439cbf:	e9 60 01 00 00       	jmp    0x439e24
  439cc4:	8b c7                	mov    eax,edi
  439cc6:	e9 5b 01 00 00       	jmp    0x439e26
  439ccb:	83 f8 02             	cmp    eax,0x2
  439cce:	0f 85 12 01 00 00    	jne    0x439de6
  439cd4:	83 fe e0             	cmp    esi,0xffffffe0
  439cd7:	77 0f                	ja     0x439ce8
  439cd9:	85 f6                	test   esi,esi
  439cdb:	76 08                	jbe    0x439ce5
  439cdd:	83 c6 0f             	add    esi,0xf
  439ce0:	83 e6 f0             	and    esi,0xfffffff0
  439ce3:	eb 03                	jmp    0x439ce8
  439ce5:	6a 10                	push   0x10
  439ce7:	5e                   	pop    esi
  439ce8:	33 ff                	xor    edi,edi
  439cea:	83 fe e0             	cmp    esi,0xffffffe0
  439ced:	0f 87 d5 00 00 00    	ja     0x439dc8
  439cf3:	8d 45 0c             	lea    eax,[ebp+0xc]
  439cf6:	50                   	push   eax
  439cf7:	8d 45 fc             	lea    eax,[ebp-0x4]
  439cfa:	50                   	push   eax
  439cfb:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  439cfe:	e8 9c b4 ff ff       	call   0x43519f
  439d03:	8b d8                	mov    ebx,eax
  439d05:	83 c4 0c             	add    esp,0xc
  439d08:	85 db                	test   ebx,ebx
  439d0a:	0f 84 9c 00 00 00    	je     0x439dac
  439d10:	3b 35 6c 97 44 00    	cmp    esi,DWORD PTR ds:0x44976c
  439d16:	73 58                	jae    0x439d70
  439d18:	8b fe                	mov    edi,esi
  439d1a:	c1 ef 04             	shr    edi,0x4
  439d1d:	57                   	push   edi
  439d1e:	53                   	push   ebx
  439d1f:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  439d22:	ff 75 fc             	push   DWORD PTR [ebp-0x4]
  439d25:	e8 3d b8 ff ff       	call   0x435567
  439d2a:	83 c4 10             	add    esp,0x10
  439d2d:	85 c0                	test   eax,eax
  439d2f:	74 05                	je     0x439d36
  439d31:	8b 7d 08             	mov    edi,DWORD PTR [ebp+0x8]
  439d34:	eb 32                	jmp    0x439d68
  439d36:	57                   	push   edi
  439d37:	e8 ff b4 ff ff       	call   0x43523b
  439d3c:	8b f8                	mov    edi,eax
  439d3e:	59                   	pop    ecx
  439d3f:	85 ff                	test   edi,edi
  439d41:	74 2d                	je     0x439d70
  439d43:	0f b6 03             	movzx  eax,BYTE PTR [ebx]
  439d46:	c1 e0 04             	shl    eax,0x4
  439d49:	3b c6                	cmp    eax,esi
  439d4b:	72 02                	jb     0x439d4f
  439d4d:	8b c6                	mov    eax,esi
  439d4f:	50                   	push   eax
  439d50:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  439d53:	57                   	push   edi
  439d54:	e8 07 9e ff ff       	call   0x433b60
  439d59:	53                   	push   ebx
  439d5a:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  439d5d:	ff 75 fc             	push   DWORD PTR [ebp-0x4]
  439d60:	e8 91 b4 ff ff       	call   0x4351f6
  439d65:	83 c4 18             	add    esp,0x18
  439d68:	85 ff                	test   edi,edi
  439d6a:	0f 85 54 ff ff ff    	jne    0x439cc4
  439d70:	56                   	push   esi
  439d71:	6a 00                	push   0x0
  439d73:	ff 35 40 da 51 00    	push   DWORD PTR ds:0x51da40
  439d79:	ff 15 08 b1 43 00    	call   DWORD PTR ds:0x43b108
  439d7f:	8b f8                	mov    edi,eax
  439d81:	85 ff                	test   edi,edi
  439d83:	74 43                	je     0x439dc8
  439d85:	0f b6 03             	movzx  eax,BYTE PTR [ebx]
  439d88:	c1 e0 04             	shl    eax,0x4
  439d8b:	3b c6                	cmp    eax,esi
  439d8d:	72 02                	jb     0x439d91
  439d8f:	8b c6                	mov    eax,esi
  439d91:	50                   	push   eax
  439d92:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  439d95:	57                   	push   edi
  439d96:	e8 c5 9d ff ff       	call   0x433b60
  439d9b:	53                   	push   ebx
  439d9c:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  439d9f:	ff 75 fc             	push   DWORD PTR [ebp-0x4]
  439da2:	e8 4f b4 ff ff       	call   0x4351f6
  439da7:	83 c4 18             	add    esp,0x18
  439daa:	eb 14                	jmp    0x439dc0
  439dac:	56                   	push   esi
  439dad:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  439db0:	6a 00                	push   0x0
  439db2:	ff 35 40 da 51 00    	push   DWORD PTR ds:0x51da40
  439db8:	ff 15 d4 b0 43 00    	call   DWORD PTR ds:0x43b0d4
  439dbe:	8b f8                	mov    edi,eax
  439dc0:	85 ff                	test   edi,edi
  439dc2:	0f 85 fc fe ff ff    	jne    0x439cc4
  439dc8:	83 3d 48 c4 51 00 00 	cmp    DWORD PTR ds:0x51c448,0x0
  439dcf:	0f 84 ef fe ff ff    	je     0x439cc4
  439dd5:	56                   	push   esi
  439dd6:	e8 35 b8 ff ff       	call   0x435610
  439ddb:	85 c0                	test   eax,eax
  439ddd:	59                   	pop    ecx
  439dde:	0f 85 04 ff ff ff    	jne    0x439ce8
  439de4:	eb 3e                	jmp    0x439e24
  439de6:	33 c0                	xor    eax,eax
  439de8:	83 fe e0             	cmp    esi,0xffffffe0
  439deb:	77 23                	ja     0x439e10
  439ded:	85 f6                	test   esi,esi
  439def:	75 03                	jne    0x439df4
  439df1:	6a 01                	push   0x1
  439df3:	5e                   	pop    esi
  439df4:	83 c6 0f             	add    esi,0xf
  439df7:	83 e6 f0             	and    esi,0xfffffff0
  439dfa:	56                   	push   esi
  439dfb:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  439dfe:	6a 00                	push   0x0
  439e00:	ff 35 40 da 51 00    	push   DWORD PTR ds:0x51da40
  439e06:	ff 15 d4 b0 43 00    	call   DWORD PTR ds:0x43b0d4
  439e0c:	85 c0                	test   eax,eax
  439e0e:	75 16                	jne    0x439e26
  439e10:	83 3d 48 c4 51 00 00 	cmp    DWORD PTR ds:0x51c448,0x0
  439e17:	74 0d                	je     0x439e26
  439e19:	56                   	push   esi
  439e1a:	e8 f1 b7 ff ff       	call   0x435610
  439e1f:	85 c0                	test   eax,eax
  439e21:	59                   	pop    ecx
  439e22:	75 c2                	jne    0x439de6
  439e24:	33 c0                	xor    eax,eax
  439e26:	5f                   	pop    edi
  439e27:	5e                   	pop    esi
  439e28:	5b                   	pop    ebx
  439e29:	c9                   	leave
  439e2a:	c3                   	ret
