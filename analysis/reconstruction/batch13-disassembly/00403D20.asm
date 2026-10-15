
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00403d20 <.text+0x2d20>:
  403d20:	6a ff                	push   0xffffffff
  403d22:	68 9c a3 43 00       	push   0x43a39c
  403d27:	64 a1 00 00 00 00    	mov    eax,fs:0x0
  403d2d:	50                   	push   eax
  403d2e:	64 89 25 00 00 00 00 	mov    DWORD PTR fs:0x0,esp
  403d35:	81 ec 98 00 00 00    	sub    esp,0x98
  403d3b:	8b 01                	mov    eax,DWORD PTR [ecx]
  403d3d:	53                   	push   ebx
  403d3e:	55                   	push   ebp
  403d3f:	56                   	push   esi
  403d40:	57                   	push   edi
  403d41:	33 ff                	xor    edi,edi
  403d43:	3b c7                	cmp    eax,edi
  403d45:	89 4c 24 1c          	mov    DWORD PTR [esp+0x1c],ecx
  403d49:	89 7c 24 14          	mov    DWORD PTR [esp+0x14],edi
  403d4d:	75 0a                	jne    0x403d59
  403d4f:	b8 f0 01 04 80       	mov    eax,0x800401f0
  403d54:	e9 75 02 00 00       	jmp    0x403fce
  403d59:	8b b4 24 bc 00 00 00 	mov    esi,DWORD PTR [esp+0xbc]
  403d60:	3b f7                	cmp    esi,edi
  403d62:	0f 84 61 02 00 00    	je     0x403fc9
  403d68:	39 bc 24 b8 00 00 00 	cmp    DWORD PTR [esp+0xb8],edi
  403d6f:	0f 84 54 02 00 00    	je     0x403fc9
  403d75:	8b 84 24 d4 00 00 00 	mov    eax,DWORD PTR [esp+0xd4]
  403d7c:	83 f8 01             	cmp    eax,0x1
  403d7f:	0f 82 44 02 00 00    	jb     0x403fc9
  403d85:	c1 e0 02             	shl    eax,0x2
  403d88:	50                   	push   eax
  403d89:	e8 40 be 02 00       	call   0x42fbce
  403d8e:	8b e8                	mov    ebp,eax
  403d90:	83 c4 04             	add    esp,0x4
  403d93:	3b ef                	cmp    ebp,edi
  403d95:	75 0c                	jne    0x403da3
  403d97:	be 0e 00 07 80       	mov    esi,0x8007000e
  403d9c:	8b c6                	mov    eax,esi
  403d9e:	e9 2b 02 00 00       	jmp    0x403fce
  403da3:	68 90 00 00 00       	push   0x90
  403da8:	e8 21 be 02 00       	call   0x42fbce
  403dad:	83 c4 04             	add    esp,0x4
  403db0:	89 44 24 18          	mov    DWORD PTR [esp+0x18],eax
  403db4:	3b c7                	cmp    eax,edi
  403db6:	89 bc 24 b0 00 00 00 	mov    DWORD PTR [esp+0xb0],edi
  403dbd:	74 0b                	je     0x403dca
  403dbf:	8b c8                	mov    ecx,eax
  403dc1:	e8 4a 07 00 00       	call   0x404510
  403dc6:	8b d8                	mov    ebx,eax
  403dc8:	eb 02                	jmp    0x403dcc
  403dca:	33 db                	xor    ebx,ebx
  403dcc:	3b df                	cmp    ebx,edi
  403dce:	c7 84 24 b0 00 00 00 	mov    DWORD PTR [esp+0xb0],0xffffffff
  403dd5:	ff ff ff ff 
  403dd9:	75 15                	jne    0x403df0
  403ddb:	55                   	push   ebp
  403ddc:	be 0e 00 07 80       	mov    esi,0x8007000e
  403de1:	e8 f6 bd 02 00       	call   0x42fbdc
  403de6:	83 c4 04             	add    esp,0x4
  403de9:	8b c6                	mov    eax,esi
  403deb:	e9 de 01 00 00       	jmp    0x403fce
  403df0:	6a 01                	push   0x1
  403df2:	57                   	push   edi
  403df3:	56                   	push   esi
  403df4:	8b cb                	mov    ecx,ebx
  403df6:	e8 65 07 00 00       	call   0x404560
  403dfb:	8b cb                	mov    ecx,ebx
  403dfd:	e8 de 0a 00 00       	call   0x4048e0
  403e02:	85 c0                	test   eax,eax
  403e04:	0f 85 8f 00 00 00    	jne    0x403e99
  403e0a:	8b 0d 4c e1 43 00    	mov    ecx,DWORD PTR ds:0x43e14c
  403e10:	8d 7c 24 48          	lea    edi,[esp+0x48]
  403e14:	89 4c 24 44          	mov    DWORD PTR [esp+0x44],ecx
  403e18:	b9 18 00 00 00       	mov    ecx,0x18
  403e1d:	f3 ab                	rep stos DWORD PTR es:[edi],eax
  403e1f:	a1 74 e8 43 00       	mov    eax,ds:0x43e874
  403e24:	83 f8 ff             	cmp    eax,0xffffffff
  403e27:	74 70                	je     0x403e99
  403e29:	04 61                	add    al,0x61
  403e2b:	8b fe                	mov    edi,esi
  403e2d:	88 44 24 44          	mov    BYTE PTR [esp+0x44],al
  403e31:	83 c9 ff             	or     ecx,0xffffffff
  403e34:	33 c0                	xor    eax,eax
  403e36:	8d 54 24 44          	lea    edx,[esp+0x44]
  403e3a:	f2 ae                	repnz scas al,BYTE PTR es:[edi]
  403e3c:	f7 d1                	not    ecx
  403e3e:	2b f9                	sub    edi,ecx
  403e40:	6a 01                	push   0x1
  403e42:	8b f7                	mov    esi,edi
  403e44:	8b fa                	mov    edi,edx
  403e46:	8b d1                	mov    edx,ecx
  403e48:	83 c9 ff             	or     ecx,0xffffffff
  403e4b:	f2 ae                	repnz scas al,BYTE PTR es:[edi]
  403e4d:	8b ca                	mov    ecx,edx
  403e4f:	4f                   	dec    edi
  403e50:	c1 e9 02             	shr    ecx,0x2
  403e53:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  403e55:	8b ca                	mov    ecx,edx
  403e57:	50                   	push   eax
  403e58:	83 e1 03             	and    ecx,0x3
  403e5b:	8d 44 24 4c          	lea    eax,[esp+0x4c]
  403e5f:	f3 a4                	rep movs BYTE PTR es:[edi],BYTE PTR ds:[esi]
  403e61:	50                   	push   eax
  403e62:	8b cb                	mov    ecx,ebx
  403e64:	e8 f7 06 00 00       	call   0x404560
  403e69:	8b cb                	mov    ecx,ebx
  403e6b:	e8 70 0a 00 00       	call   0x4048e0
  403e70:	85 c0                	test   eax,eax
  403e72:	75 25                	jne    0x403e99
  403e74:	be 05 40 00 80       	mov    esi,0x80004005
  403e79:	8b cb                	mov    ecx,ebx
  403e7b:	e8 b0 06 00 00       	call   0x404530
  403e80:	53                   	push   ebx
  403e81:	e8 56 bd 02 00       	call   0x42fbdc
  403e86:	83 c4 04             	add    esp,0x4
  403e89:	55                   	push   ebp
  403e8a:	e8 4d bd 02 00       	call   0x42fbdc
  403e8f:	83 c4 04             	add    esp,0x4
  403e92:	8b c6                	mov    eax,esi
  403e94:	e9 35 01 00 00       	jmp    0x403fce
  403e99:	8b cb                	mov    ecx,ebx
  403e9b:	e8 40 0a 00 00       	call   0x4048e0
  403ea0:	8b d0                	mov    edx,eax
  403ea2:	b9 09 00 00 00       	mov    ecx,0x9
  403ea7:	33 c0                	xor    eax,eax
  403ea9:	8d 7c 24 20          	lea    edi,[esp+0x20]
  403ead:	f3 ab                	rep stos DWORD PTR es:[edi],eax
  403eaf:	8b 8c 24 c4 00 00 00 	mov    ecx,DWORD PTR [esp+0xc4]
  403eb6:	8b 84 24 cc 00 00 00 	mov    eax,DWORD PTR [esp+0xcc]
  403ebd:	89 54 24 18          	mov    DWORD PTR [esp+0x18],edx
  403ec1:	89 54 24 28          	mov    DWORD PTR [esp+0x28],edx
  403ec5:	8b 94 24 c8 00 00 00 	mov    edx,DWORD PTR [esp+0xc8]
  403ecc:	89 4c 24 34          	mov    DWORD PTR [esp+0x34],ecx
  403ed0:	8b 8c 24 d0 00 00 00 	mov    ecx,DWORD PTR [esp+0xd0]
  403ed7:	89 44 24 3c          	mov    DWORD PTR [esp+0x3c],eax
  403edb:	8b 44 24 1c          	mov    eax,DWORD PTR [esp+0x1c]
  403edf:	c7 44 24 20 24 00 00 	mov    DWORD PTR [esp+0x20],0x24
  403ee6:	00 
  403ee7:	c7 44 24 24 80 00 00 	mov    DWORD PTR [esp+0x24],0x80
  403eee:	00 
  403eef:	89 54 24 38          	mov    DWORD PTR [esp+0x38],edx
  403ef3:	8b 00                	mov    eax,DWORD PTR [eax]
  403ef5:	89 4c 24 40          	mov    DWORD PTR [esp+0x40],ecx
  403ef9:	8b 13                	mov    edx,DWORD PTR [ebx]
  403efb:	6a 00                	push   0x0
  403efd:	89 54 24 34          	mov    DWORD PTR [esp+0x34],edx
  403f01:	8b 08                	mov    ecx,DWORD PTR [eax]
  403f03:	8d 54 24 24          	lea    edx,[esp+0x24]
  403f07:	55                   	push   ebp
  403f08:	52                   	push   edx
  403f09:	50                   	push   eax
  403f0a:	ff 51 0c             	call   DWORD PTR [ecx+0xc]
  403f0d:	8b f0                	mov    esi,eax
  403f0f:	81 fe 0a 00 78 08    	cmp    esi,0x878000a
  403f15:	75 06                	jne    0x403f1d
  403f17:	89 44 24 14          	mov    DWORD PTR [esp+0x14],eax
  403f1b:	eb 08                	jmp    0x403f25
  403f1d:	85 f6                	test   esi,esi
  403f1f:	0f 8c 54 ff ff ff    	jl     0x403e79
  403f25:	8b 84 24 d4 00 00 00 	mov    eax,DWORD PTR [esp+0xd4]
  403f2c:	be 01 00 00 00       	mov    esi,0x1
  403f31:	3b c6                	cmp    eax,esi
  403f33:	89 74 24 10          	mov    DWORD PTR [esp+0x10],esi
  403f37:	76 3a                	jbe    0x403f73
  403f39:	8d 7d 04             	lea    edi,[ebp+0x4]
  403f3c:	8b 44 24 1c          	mov    eax,DWORD PTR [esp+0x1c]
  403f40:	8b 55 00             	mov    edx,DWORD PTR [ebp+0x0]
  403f43:	57                   	push   edi
  403f44:	52                   	push   edx
  403f45:	8b 00                	mov    eax,DWORD PTR [eax]
  403f47:	50                   	push   eax
  403f48:	8b 08                	mov    ecx,DWORD PTR [eax]
  403f4a:	ff 51 14             	call   DWORD PTR [ecx+0x14]
  403f4d:	8b f0                	mov    esi,eax
  403f4f:	85 f6                	test   esi,esi
  403f51:	0f 8c 22 ff ff ff    	jl     0x403e79
  403f57:	8b 44 24 10          	mov    eax,DWORD PTR [esp+0x10]
  403f5b:	8b 8c 24 d4 00 00 00 	mov    ecx,DWORD PTR [esp+0xd4]
  403f62:	40                   	inc    eax
  403f63:	83 c7 04             	add    edi,0x4
  403f66:	3b c1                	cmp    eax,ecx
  403f68:	89 44 24 10          	mov    DWORD PTR [esp+0x10],eax
  403f6c:	72 ce                	jb     0x403f3c
  403f6e:	be 01 00 00 00       	mov    esi,0x1
  403f73:	6a 14                	push   0x14
  403f75:	e8 54 bc 02 00       	call   0x42fbce
  403f7a:	83 c4 04             	add    esp,0x4
  403f7d:	89 44 24 10          	mov    DWORD PTR [esp+0x10],eax
  403f81:	85 c0                	test   eax,eax
  403f83:	89 b4 24 b0 00 00 00 	mov    DWORD PTR [esp+0xb0],esi
  403f8a:	74 18                	je     0x403fa4
  403f8c:	8b 8c 24 d4 00 00 00 	mov    ecx,DWORD PTR [esp+0xd4]
  403f93:	8b 54 24 18          	mov    edx,DWORD PTR [esp+0x18]
  403f97:	53                   	push   ebx
  403f98:	51                   	push   ecx
  403f99:	52                   	push   edx
  403f9a:	55                   	push   ebp
  403f9b:	8b c8                	mov    ecx,eax
  403f9d:	e8 4e 00 00 00       	call   0x403ff0
  403fa2:	eb 02                	jmp    0x403fa6
  403fa4:	33 c0                	xor    eax,eax
  403fa6:	8b 8c 24 b8 00 00 00 	mov    ecx,DWORD PTR [esp+0xb8]
  403fad:	55                   	push   ebp
  403fae:	c7 84 24 b4 00 00 00 	mov    DWORD PTR [esp+0xb4],0xffffffff
  403fb5:	ff ff ff ff 
  403fb9:	89 01                	mov    DWORD PTR [ecx],eax
  403fbb:	e8 1c bc 02 00       	call   0x42fbdc
  403fc0:	8b 44 24 18          	mov    eax,DWORD PTR [esp+0x18]
  403fc4:	83 c4 04             	add    esp,0x4
  403fc7:	eb 05                	jmp    0x403fce
  403fc9:	b8 57 00 07 80       	mov    eax,0x80070057
  403fce:	8b 8c 24 a8 00 00 00 	mov    ecx,DWORD PTR [esp+0xa8]
  403fd5:	5f                   	pop    edi
  403fd6:	5e                   	pop    esi
  403fd7:	5d                   	pop    ebp
  403fd8:	5b                   	pop    ebx
  403fd9:	64 89 0d 00 00 00 00 	mov    DWORD PTR fs:0x0,ecx
  403fe0:	81 c4 a4 00 00 00    	add    esp,0xa4
  403fe6:	c2 20 00             	ret    0x20
