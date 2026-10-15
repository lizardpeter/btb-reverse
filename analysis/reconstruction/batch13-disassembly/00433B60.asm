
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00433b60 <.text+0x32b60>:
  433b60:	55                   	push   ebp
  433b61:	8b ec                	mov    ebp,esp
  433b63:	57                   	push   edi
  433b64:	56                   	push   esi
  433b65:	8b 75 0c             	mov    esi,DWORD PTR [ebp+0xc]
  433b68:	8b 4d 10             	mov    ecx,DWORD PTR [ebp+0x10]
  433b6b:	8b 7d 08             	mov    edi,DWORD PTR [ebp+0x8]
  433b6e:	8b c1                	mov    eax,ecx
  433b70:	8b d1                	mov    edx,ecx
  433b72:	03 c6                	add    eax,esi
  433b74:	3b fe                	cmp    edi,esi
  433b76:	76 08                	jbe    0x433b80
  433b78:	3b f8                	cmp    edi,eax
  433b7a:	0f 82 78 01 00 00    	jb     0x433cf8
  433b80:	f7 c7 03 00 00 00    	test   edi,0x3
  433b86:	75 14                	jne    0x433b9c
  433b88:	c1 e9 02             	shr    ecx,0x2
  433b8b:	83 e2 03             	and    edx,0x3
  433b8e:	83 f9 08             	cmp    ecx,0x8
  433b91:	72 29                	jb     0x433bbc
  433b93:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  433b95:	ff 24 95 a8 3c 43 00 	jmp    DWORD PTR [edx*4+0x433ca8]
  433b9c:	8b c7                	mov    eax,edi
  433b9e:	ba 03 00 00 00       	mov    edx,0x3
  433ba3:	83 e9 04             	sub    ecx,0x4
  433ba6:	72 0c                	jb     0x433bb4
  433ba8:	83 e0 03             	and    eax,0x3
  433bab:	03 c8                	add    ecx,eax
  433bad:	ff 24 85 c0 3b 43 00 	jmp    DWORD PTR [eax*4+0x433bc0]
  433bb4:	ff 24 8d b8 3c 43 00 	jmp    DWORD PTR [ecx*4+0x433cb8]
  433bbb:	90                   	nop
  433bbc:	ff 24 8d 3c 3c 43 00 	jmp    DWORD PTR [ecx*4+0x433c3c]
  433bc3:	90                   	nop
  433bc4:	d0 3b                	sar    BYTE PTR [ebx],1
  433bc6:	43                   	inc    ebx
  433bc7:	00 fc                	add    ah,bh
  433bc9:	3b 43 00             	cmp    eax,DWORD PTR [ebx+0x0]
  433bcc:	20 3c 43             	and    BYTE PTR [ebx+eax*2],bh
  433bcf:	00 23                	add    BYTE PTR [ebx],ah
  433bd1:	d1 8a 06 88 07 8a    	ror    DWORD PTR [edx-0x75f877fa],1
  433bd7:	46                   	inc    esi
  433bd8:	01 88 47 01 8a 46    	add    DWORD PTR [eax+0x468a0147],ecx
  433bde:	02 c1                	add    al,cl
  433be0:	e9 02 88 47 02       	jmp    0x28ac3e7
  433be5:	83 c6 03             	add    esi,0x3
  433be8:	83 c7 03             	add    edi,0x3
  433beb:	83 f9 08             	cmp    ecx,0x8
  433bee:	72 cc                	jb     0x433bbc
  433bf0:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  433bf2:	ff 24 95 a8 3c 43 00 	jmp    DWORD PTR [edx*4+0x433ca8]
  433bf9:	8d 49 00             	lea    ecx,[ecx+0x0]
  433bfc:	23 d1                	and    edx,ecx
  433bfe:	8a 06                	mov    al,BYTE PTR [esi]
  433c00:	88 07                	mov    BYTE PTR [edi],al
  433c02:	8a 46 01             	mov    al,BYTE PTR [esi+0x1]
  433c05:	c1 e9 02             	shr    ecx,0x2
  433c08:	88 47 01             	mov    BYTE PTR [edi+0x1],al
  433c0b:	83 c6 02             	add    esi,0x2
  433c0e:	83 c7 02             	add    edi,0x2
  433c11:	83 f9 08             	cmp    ecx,0x8
  433c14:	72 a6                	jb     0x433bbc
  433c16:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  433c18:	ff 24 95 a8 3c 43 00 	jmp    DWORD PTR [edx*4+0x433ca8]
  433c1f:	90                   	nop
  433c20:	23 d1                	and    edx,ecx
  433c22:	8a 06                	mov    al,BYTE PTR [esi]
  433c24:	88 07                	mov    BYTE PTR [edi],al
  433c26:	46                   	inc    esi
  433c27:	c1 e9 02             	shr    ecx,0x2
  433c2a:	47                   	inc    edi
  433c2b:	83 f9 08             	cmp    ecx,0x8
  433c2e:	72 8c                	jb     0x433bbc
  433c30:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  433c32:	ff 24 95 a8 3c 43 00 	jmp    DWORD PTR [edx*4+0x433ca8]
  433c39:	8d 49 00             	lea    ecx,[ecx+0x0]
  433c3c:	9f                   	lahf
  433c3d:	3c 43                	cmp    al,0x43
  433c3f:	00 8c 3c 43 00 84 3c 	add    BYTE PTR [esp+edi*1+0x3c840043],cl
  433c46:	43                   	inc    ebx
  433c47:	00 7c 3c 43          	add    BYTE PTR [esp+edi*1+0x43],bh
  433c4b:	00 74 3c 43          	add    BYTE PTR [esp+edi*1+0x43],dh
  433c4f:	00 6c 3c 43          	add    BYTE PTR [esp+edi*1+0x43],ch
  433c53:	00 64 3c 43          	add    BYTE PTR [esp+edi*1+0x43],ah
  433c57:	00 5c 3c 43          	add    BYTE PTR [esp+edi*1+0x43],bl
  433c5b:	00 8b 44 8e e4 89    	add    BYTE PTR [ebx-0x761b71bc],cl
  433c61:	44                   	inc    esp
  433c62:	8f                   	(bad)
  433c63:	e4 8b                	in     al,0x8b
  433c65:	44                   	inc    esp
  433c66:	8e e8                	mov    gs,eax
  433c68:	89 44 8f e8          	mov    DWORD PTR [edi+ecx*4-0x18],eax
  433c6c:	8b 44 8e ec          	mov    eax,DWORD PTR [esi+ecx*4-0x14]
  433c70:	89 44 8f ec          	mov    DWORD PTR [edi+ecx*4-0x14],eax
  433c74:	8b 44 8e f0          	mov    eax,DWORD PTR [esi+ecx*4-0x10]
  433c78:	89 44 8f f0          	mov    DWORD PTR [edi+ecx*4-0x10],eax
  433c7c:	8b 44 8e f4          	mov    eax,DWORD PTR [esi+ecx*4-0xc]
  433c80:	89 44 8f f4          	mov    DWORD PTR [edi+ecx*4-0xc],eax
  433c84:	8b 44 8e f8          	mov    eax,DWORD PTR [esi+ecx*4-0x8]
  433c88:	89 44 8f f8          	mov    DWORD PTR [edi+ecx*4-0x8],eax
  433c8c:	8b 44 8e fc          	mov    eax,DWORD PTR [esi+ecx*4-0x4]
  433c90:	89 44 8f fc          	mov    DWORD PTR [edi+ecx*4-0x4],eax
  433c94:	8d 04 8d 00 00 00 00 	lea    eax,[ecx*4+0x0]
  433c9b:	03 f0                	add    esi,eax
  433c9d:	03 f8                	add    edi,eax
  433c9f:	ff 24 95 a8 3c 43 00 	jmp    DWORD PTR [edx*4+0x433ca8]
  433ca6:	8b ff                	mov    edi,edi
  433ca8:	b8 3c 43 00 c0       	mov    eax,0xc000433c
  433cad:	3c 43                	cmp    al,0x43
  433caf:	00 cc                	add    ah,cl
  433cb1:	3c 43                	cmp    al,0x43
  433cb3:	00 e0                	add    al,ah
  433cb5:	3c 43                	cmp    al,0x43
  433cb7:	00 8b 45 08 5e 5f    	add    BYTE PTR [ebx+0x5f5e0845],cl
  433cbd:	c9                   	leave
  433cbe:	c3                   	ret
  433cbf:	90                   	nop
  433cc0:	8a 06                	mov    al,BYTE PTR [esi]
  433cc2:	88 07                	mov    BYTE PTR [edi],al
  433cc4:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  433cc7:	5e                   	pop    esi
  433cc8:	5f                   	pop    edi
  433cc9:	c9                   	leave
  433cca:	c3                   	ret
  433ccb:	90                   	nop
  433ccc:	8a 06                	mov    al,BYTE PTR [esi]
  433cce:	88 07                	mov    BYTE PTR [edi],al
  433cd0:	8a 46 01             	mov    al,BYTE PTR [esi+0x1]
  433cd3:	88 47 01             	mov    BYTE PTR [edi+0x1],al
  433cd6:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  433cd9:	5e                   	pop    esi
  433cda:	5f                   	pop    edi
  433cdb:	c9                   	leave
  433cdc:	c3                   	ret
  433cdd:	8d 49 00             	lea    ecx,[ecx+0x0]
  433ce0:	8a 06                	mov    al,BYTE PTR [esi]
  433ce2:	88 07                	mov    BYTE PTR [edi],al
  433ce4:	8a 46 01             	mov    al,BYTE PTR [esi+0x1]
  433ce7:	88 47 01             	mov    BYTE PTR [edi+0x1],al
  433cea:	8a 46 02             	mov    al,BYTE PTR [esi+0x2]
  433ced:	88 47 02             	mov    BYTE PTR [edi+0x2],al
  433cf0:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  433cf3:	5e                   	pop    esi
  433cf4:	5f                   	pop    edi
  433cf5:	c9                   	leave
  433cf6:	c3                   	ret
  433cf7:	90                   	nop
  433cf8:	8d 74 31 fc          	lea    esi,[ecx+esi*1-0x4]
  433cfc:	8d 7c 39 fc          	lea    edi,[ecx+edi*1-0x4]
  433d00:	f7 c7 03 00 00 00    	test   edi,0x3
  433d06:	75 24                	jne    0x433d2c
  433d08:	c1 e9 02             	shr    ecx,0x2
  433d0b:	83 e2 03             	and    edx,0x3
  433d0e:	83 f9 08             	cmp    ecx,0x8
  433d11:	72 0d                	jb     0x433d20
  433d13:	fd                   	std
  433d14:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  433d16:	fc                   	cld
  433d17:	ff 24 95 40 3e 43 00 	jmp    DWORD PTR [edx*4+0x433e40]
  433d1e:	8b ff                	mov    edi,edi
  433d20:	f7 d9                	neg    ecx
  433d22:	ff 24 8d f0 3d 43 00 	jmp    DWORD PTR [ecx*4+0x433df0]
  433d29:	8d 49 00             	lea    ecx,[ecx+0x0]
  433d2c:	8b c7                	mov    eax,edi
  433d2e:	ba 03 00 00 00       	mov    edx,0x3
  433d33:	83 f9 04             	cmp    ecx,0x4
  433d36:	72 0c                	jb     0x433d44
  433d38:	83 e0 03             	and    eax,0x3
  433d3b:	2b c8                	sub    ecx,eax
  433d3d:	ff 24 85 48 3d 43 00 	jmp    DWORD PTR [eax*4+0x433d48]
  433d44:	ff 24 8d 40 3e 43 00 	jmp    DWORD PTR [ecx*4+0x433e40]
  433d4b:	90                   	nop
  433d4c:	58                   	pop    eax
  433d4d:	3d 43 00 78 3d       	cmp    eax,0x3d780043
  433d52:	43                   	inc    ebx
  433d53:	00 a0 3d 43 00 8a    	add    BYTE PTR [eax-0x75ffbcc3],ah
  433d59:	46                   	inc    esi
  433d5a:	03 23                	add    esp,DWORD PTR [ebx]
  433d5c:	d1 88 47 03 4e c1    	ror    DWORD PTR [eax-0x3eb1fcb9],1
  433d62:	e9 02 4f 83 f9       	jmp    0xf9c68c69
  433d67:	08 72 b6             	or     BYTE PTR [edx-0x4a],dh
  433d6a:	fd                   	std
  433d6b:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  433d6d:	fc                   	cld
  433d6e:	ff 24 95 40 3e 43 00 	jmp    DWORD PTR [edx*4+0x433e40]
  433d75:	8d 49 00             	lea    ecx,[ecx+0x0]
  433d78:	8a 46 03             	mov    al,BYTE PTR [esi+0x3]
  433d7b:	23 d1                	and    edx,ecx
  433d7d:	88 47 03             	mov    BYTE PTR [edi+0x3],al
  433d80:	8a 46 02             	mov    al,BYTE PTR [esi+0x2]
  433d83:	c1 e9 02             	shr    ecx,0x2
  433d86:	88 47 02             	mov    BYTE PTR [edi+0x2],al
  433d89:	83 ee 02             	sub    esi,0x2
  433d8c:	83 ef 02             	sub    edi,0x2
  433d8f:	83 f9 08             	cmp    ecx,0x8
  433d92:	72 8c                	jb     0x433d20
  433d94:	fd                   	std
  433d95:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  433d97:	fc                   	cld
  433d98:	ff 24 95 40 3e 43 00 	jmp    DWORD PTR [edx*4+0x433e40]
  433d9f:	90                   	nop
  433da0:	8a 46 03             	mov    al,BYTE PTR [esi+0x3]
  433da3:	23 d1                	and    edx,ecx
  433da5:	88 47 03             	mov    BYTE PTR [edi+0x3],al
  433da8:	8a 46 02             	mov    al,BYTE PTR [esi+0x2]
  433dab:	88 47 02             	mov    BYTE PTR [edi+0x2],al
  433dae:	8a 46 01             	mov    al,BYTE PTR [esi+0x1]
  433db1:	c1 e9 02             	shr    ecx,0x2
  433db4:	88 47 01             	mov    BYTE PTR [edi+0x1],al
  433db7:	83 ee 03             	sub    esi,0x3
  433dba:	83 ef 03             	sub    edi,0x3
  433dbd:	83 f9 08             	cmp    ecx,0x8
  433dc0:	0f 82 5a ff ff ff    	jb     0x433d20
  433dc6:	fd                   	std
  433dc7:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  433dc9:	fc                   	cld
  433dca:	ff 24 95 40 3e 43 00 	jmp    DWORD PTR [edx*4+0x433e40]
  433dd1:	8d 49 00             	lea    ecx,[ecx+0x0]
  433dd4:	f4                   	hlt
  433dd5:	3d 43 00 fc 3d       	cmp    eax,0x3dfc0043
  433dda:	43                   	inc    ebx
  433ddb:	00 04 3e             	add    BYTE PTR [esi+edi*1],al
  433dde:	43                   	inc    ebx
  433ddf:	00 0c 3e             	add    BYTE PTR [esi+edi*1],cl
  433de2:	43                   	inc    ebx
  433de3:	00 14 3e             	add    BYTE PTR [esi+edi*1],dl
  433de6:	43                   	inc    ebx
  433de7:	00 1c 3e             	add    BYTE PTR [esi+edi*1],bl
  433dea:	43                   	inc    ebx
  433deb:	00 24 3e             	add    BYTE PTR [esi+edi*1],ah
  433dee:	43                   	inc    ebx
  433def:	00 37                	add    BYTE PTR [edi],dh
  433df1:	3e 43                	ds inc ebx
  433df3:	00                   	.byte 0
  433df4:	8b 44 8e 1c          	mov    eax,DWORD PTR [esi+ecx*4+0x1c]
