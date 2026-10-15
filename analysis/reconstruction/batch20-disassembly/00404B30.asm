
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00404b30 <.text+0x3b30>:
  404b30:	51                   	push   ecx
  404b31:	53                   	push   ebx
  404b32:	55                   	push   ebp
  404b33:	56                   	push   esi
  404b34:	8b f1                	mov    esi,ecx
  404b36:	57                   	push   edi
  404b37:	83 7e 7c 01          	cmp    DWORD PTR [esi+0x7c],0x1
  404b3b:	75 0b                	jne    0x404b48
  404b3d:	8b 46 04             	mov    eax,DWORD PTR [esi+0x4]
  404b40:	6a 00                	push   0x0
  404b42:	50                   	push   eax
  404b43:	e9 f5 00 00 00       	jmp    0x404c3d
  404b48:	8b 7e 34             	mov    edi,DWORD PTR [esi+0x34]
  404b4b:	8b 4e 04             	mov    ecx,DWORD PTR [esi+0x4]
  404b4e:	8d 46 34             	lea    eax,[esi+0x34]
  404b51:	81 cf 00 00 00 10    	or     edi,0x10000000
  404b57:	85 c9                	test   ecx,ecx
  404b59:	89 38                	mov    DWORD PTR [eax],edi
  404b5b:	75 0b                	jne    0x404b68
  404b5d:	5f                   	pop    edi
  404b5e:	5e                   	pop    esi
  404b5f:	5d                   	pop    ebp
  404b60:	b8 f0 01 04 80       	mov    eax,0x800401f0
  404b65:	5b                   	pop    ebx
  404b66:	59                   	pop    ecx
  404b67:	c3                   	ret
  404b68:	6a 00                	push   0x0
  404b6a:	50                   	push   eax
  404b6b:	51                   	push   ecx
  404b6c:	ff 15 84 b2 43 00    	call   DWORD PTR ds:0x43b284
  404b72:	85 c0                	test   eax,eax
  404b74:	74 0b                	je     0x404b81
  404b76:	5f                   	pop    edi
  404b77:	5e                   	pop    esi
  404b78:	5d                   	pop    ebp
  404b79:	b8 05 40 00 80       	mov    eax,0x80004005
  404b7e:	5b                   	pop    ebx
  404b7f:	59                   	pop    ecx
  404b80:	c3                   	ret
  404b81:	8b 4e 04             	mov    ecx,DWORD PTR [esi+0x4]
  404b84:	8b 2d 6c b2 43 00    	mov    ebp,DWORD PTR ds:0x43b26c
  404b8a:	8d 5e 08             	lea    ebx,[esi+0x8]
  404b8d:	6a 00                	push   0x0
  404b8f:	53                   	push   ebx
  404b90:	51                   	push   ecx
  404b91:	ff d5                	call   ebp
  404b93:	85 c0                	test   eax,eax
  404b95:	74 0b                	je     0x404ba2
  404b97:	5f                   	pop    edi
  404b98:	5e                   	pop    esi
  404b99:	5d                   	pop    ebp
  404b9a:	b8 05 40 00 80       	mov    eax,0x80004005
  404b9f:	5b                   	pop    ebx
  404ba0:	59                   	pop    ecx
  404ba1:	c3                   	ret
  404ba2:	8b 56 04             	mov    edx,DWORD PTR [esi+0x4]
  404ba5:	8d 7e 1c             	lea    edi,[esi+0x1c]
  404ba8:	6a 00                	push   0x0
  404baa:	57                   	push   edi
  404bab:	52                   	push   edx
  404bac:	ff d5                	call   ebp
  404bae:	85 c0                	test   eax,eax
  404bb0:	74 0b                	je     0x404bbd
  404bb2:	5f                   	pop    edi
  404bb3:	5e                   	pop    esi
  404bb4:	5d                   	pop    ebp
  404bb5:	b8 05 40 00 80       	mov    eax,0x80004005
  404bba:	5b                   	pop    ebx
  404bbb:	59                   	pop    ecx
  404bbc:	c3                   	ret
  404bbd:	8b 46 04             	mov    eax,DWORD PTR [esi+0x4]
  404bc0:	6a 00                	push   0x0
  404bc2:	6a 00                	push   0x0
  404bc4:	50                   	push   eax
  404bc5:	ff 15 80 b2 43 00    	call   DWORD PTR ds:0x43b280
  404bcb:	8b 4e 04             	mov    ecx,DWORD PTR [esi+0x4]
  404bce:	6a 00                	push   0x0
  404bd0:	6a 00                	push   0x0
  404bd2:	57                   	push   edi
  404bd3:	51                   	push   ecx
  404bd4:	ff 15 74 b2 43 00    	call   DWORD PTR ds:0x43b274
  404bda:	85 c0                	test   eax,eax
  404bdc:	74 0b                	je     0x404be9
  404bde:	5f                   	pop    edi
  404bdf:	5e                   	pop    esi
  404be0:	5d                   	pop    ebp
  404be1:	b8 05 40 00 80       	mov    eax,0x80004005
  404be6:	5b                   	pop    ebx
  404be7:	59                   	pop    ecx
  404be8:	c3                   	ret
  404be9:	8b 56 04             	mov    edx,DWORD PTR [esi+0x4]
  404bec:	6a 10                	push   0x10
  404bee:	57                   	push   edi
  404bef:	53                   	push   ebx
  404bf0:	52                   	push   edx
  404bf1:	c7 03 66 61 63 74    	mov    DWORD PTR [ebx],0x74636166
  404bf7:	ff 15 74 b2 43 00    	call   DWORD PTR ds:0x43b274
  404bfd:	85 c0                	test   eax,eax
  404bff:	75 1e                	jne    0x404c1f
  404c01:	8b 4e 04             	mov    ecx,DWORD PTR [esi+0x4]
  404c04:	89 44 24 10          	mov    DWORD PTR [esp+0x10],eax
  404c08:	8d 44 24 10          	lea    eax,[esp+0x10]
  404c0c:	6a 04                	push   0x4
  404c0e:	50                   	push   eax
  404c0f:	51                   	push   ecx
  404c10:	ff 15 8c b2 43 00    	call   DWORD PTR ds:0x43b28c
  404c16:	8b 56 04             	mov    edx,DWORD PTR [esi+0x4]
  404c19:	6a 00                	push   0x0
  404c1b:	53                   	push   ebx
  404c1c:	52                   	push   edx
  404c1d:	ff d5                	call   ebp
  404c1f:	8b 46 04             	mov    eax,DWORD PTR [esi+0x4]
  404c22:	6a 00                	push   0x0
  404c24:	57                   	push   edi
  404c25:	50                   	push   eax
  404c26:	ff d5                	call   ebp
  404c28:	85 c0                	test   eax,eax
  404c2a:	74 0b                	je     0x404c37
  404c2c:	5f                   	pop    edi
  404c2d:	5e                   	pop    esi
  404c2e:	5d                   	pop    ebp
  404c2f:	b8 05 40 00 80       	mov    eax,0x80004005
  404c34:	5b                   	pop    ebx
  404c35:	59                   	pop    ecx
  404c36:	c3                   	ret
  404c37:	8b 4e 04             	mov    ecx,DWORD PTR [esi+0x4]
  404c3a:	6a 00                	push   0x0
  404c3c:	51                   	push   ecx
  404c3d:	ff 15 90 b2 43 00    	call   DWORD PTR ds:0x43b290
  404c43:	c7 46 04 00 00 00 00 	mov    DWORD PTR [esi+0x4],0x0
  404c4a:	5f                   	pop    edi
  404c4b:	5e                   	pop    esi
  404c4c:	5d                   	pop    ebp
  404c4d:	33 c0                	xor    eax,eax
  404c4f:	5b                   	pop    ebx
  404c50:	59                   	pop    ecx
  404c51:	c3                   	ret
