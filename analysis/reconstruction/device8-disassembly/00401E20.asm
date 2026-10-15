
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00401e20 <.text+0xe20>:
  401e20:	83 ec 34             	sub    esp,0x34
  401e23:	53                   	push   ebx
  401e24:	56                   	push   esi
  401e25:	68 8c e0 43 00       	push   0x43e08c
  401e2a:	68 84 e0 43 00       	push   0x43e084
  401e2f:	ff 15 e8 b1 43 00    	call   DWORD PTR ds:0x43b1e8
  401e35:	85 c0                	test   eax,eax
  401e37:	0f 85 32 01 00 00    	jne    0x401f6f
  401e3d:	8b 5c 24 40          	mov    ebx,DWORD PTR [esp+0x40]
  401e41:	8b 35 e4 b1 43 00    	mov    esi,DWORD PTR ds:0x43b1e4
  401e47:	6a 65                	push   0x65
  401e49:	53                   	push   ebx
  401e4a:	c7 44 24 14 30 00 00 	mov    DWORD PTR [esp+0x14],0x30
  401e51:	00 
  401e52:	c7 44 24 3c 84 e0 43 	mov    DWORD PTR [esp+0x3c],0x43e084
  401e59:	00 
  401e5a:	c7 44 24 1c d0 20 40 	mov    DWORD PTR [esp+0x1c],0x4020d0
  401e61:	00 
  401e62:	c7 44 24 18 03 00 00 	mov    DWORD PTR [esp+0x18],0x3
  401e69:	00 
  401e6a:	89 5c 24 28          	mov    DWORD PTR [esp+0x28],ebx
  401e6e:	ff d6                	call   esi
  401e70:	6a 65                	push   0x65
  401e72:	53                   	push   ebx
  401e73:	89 44 24 2c          	mov    DWORD PTR [esp+0x2c],eax
  401e77:	ff d6                	call   esi
  401e79:	33 f6                	xor    esi,esi
  401e7b:	68 00 7f 00 00       	push   0x7f00
  401e80:	56                   	push   esi
  401e81:	89 44 24 40          	mov    DWORD PTR [esp+0x40],eax
  401e85:	ff 15 e0 b1 43 00    	call   DWORD PTR ds:0x43b1e0
  401e8b:	89 44 24 28          	mov    DWORD PTR [esp+0x28],eax
  401e8f:	8d 44 24 0c          	lea    eax,[esp+0xc]
  401e93:	50                   	push   eax
  401e94:	c7 44 24 30 06 00 00 	mov    DWORD PTR [esp+0x30],0x6
  401e9b:	00 
  401e9c:	c7 44 24 34 66 00 00 	mov    DWORD PTR [esp+0x34],0x66
  401ea3:	00 
  401ea4:	89 74 24 1c          	mov    DWORD PTR [esp+0x1c],esi
  401ea8:	89 74 24 20          	mov    DWORD PTR [esp+0x20],esi
  401eac:	ff 15 dc b1 43 00    	call   DWORD PTR ds:0x43b1dc
  401eb2:	66 85 c0             	test   ax,ax
  401eb5:	75 0b                	jne    0x401ec2
  401eb7:	5e                   	pop    esi
  401eb8:	b8 05 40 00 80       	mov    eax,0x80004005
  401ebd:	5b                   	pop    ebx
  401ebe:	83 c4 34             	add    esp,0x34
  401ec1:	c3                   	ret
  401ec2:	55                   	push   ebp
  401ec3:	57                   	push   edi
  401ec4:	6a 67                	push   0x67
  401ec6:	53                   	push   ebx
  401ec7:	ff 15 d8 b1 43 00    	call   DWORD PTR ds:0x43b1d8
  401ecd:	8b 35 d4 b1 43 00    	mov    esi,DWORD PTR ds:0x43b1d4
  401ed3:	6a 20                	push   0x20
  401ed5:	89 44 24 14          	mov    DWORD PTR [esp+0x14],eax
  401ed9:	ff d6                	call   esi
  401edb:	6a 21                	push   0x21
  401edd:	8b e8                	mov    ebp,eax
  401edf:	ff d6                	call   esi
  401ee1:	6a 0f                	push   0xf
  401ee3:	89 44 24 4c          	mov    DWORD PTR [esp+0x4c],eax
  401ee7:	ff d6                	call   esi
  401ee9:	6a 04                	push   0x4
  401eeb:	8b f8                	mov    edi,eax
  401eed:	ff d6                	call   esi
  401eef:	8b 4c 24 48          	mov    ecx,DWORD PTR [esp+0x48]
  401ef3:	03 f8                	add    edi,eax
  401ef5:	6a 00                	push   0x0
  401ef7:	53                   	push   ebx
  401ef8:	8d bc 4f e0 01 00 00 	lea    edi,[edi+ecx*2+0x1e0]
  401eff:	6a 00                	push   0x0
  401f01:	6a 00                	push   0x0
  401f03:	8d 94 2d 80 02 00 00 	lea    edx,[ebp+ebp*1+0x280]
  401f0a:	57                   	push   edi
  401f0b:	52                   	push   edx
  401f0c:	68 00 00 00 80       	push   0x80000000
  401f11:	68 00 00 00 80       	push   0x80000000
  401f16:	68 00 00 ce 00       	push   0xce0000
  401f1b:	68 74 e0 43 00       	push   0x43e074
  401f20:	68 84 e0 43 00       	push   0x43e084
  401f25:	6a 00                	push   0x0
  401f27:	ff 15 d0 b1 43 00    	call   DWORD PTR ds:0x43b1d0
  401f2d:	8b f0                	mov    esi,eax
  401f2f:	5f                   	pop    edi
  401f30:	85 f6                	test   esi,esi
  401f32:	5d                   	pop    ebp
  401f33:	75 0b                	jne    0x401f40
  401f35:	5e                   	pop    esi
  401f36:	b8 05 40 00 80       	mov    eax,0x80004005
  401f3b:	5b                   	pop    ebx
  401f3c:	83 c4 34             	add    esp,0x34
  401f3f:	c3                   	ret
  401f40:	8b 44 24 44          	mov    eax,DWORD PTR [esp+0x44]
  401f44:	50                   	push   eax
  401f45:	56                   	push   esi
  401f46:	ff 15 cc b1 43 00    	call   DWORD PTR ds:0x43b1cc
  401f4c:	56                   	push   esi
  401f4d:	ff 15 b8 b1 43 00    	call   DWORD PTR ds:0x43b1b8
  401f53:	68 f8 dd 44 00       	push   0x44ddf8
  401f58:	56                   	push   esi
  401f59:	ff 15 b4 b1 43 00    	call   DWORD PTR ds:0x43b1b4
  401f5f:	8b 4c 24 48          	mov    ecx,DWORD PTR [esp+0x48]
  401f63:	8b 54 24 4c          	mov    edx,DWORD PTR [esp+0x4c]
  401f67:	8b 44 24 08          	mov    eax,DWORD PTR [esp+0x8]
  401f6b:	89 31                	mov    DWORD PTR [ecx],esi
  401f6d:	89 02                	mov    DWORD PTR [edx],eax
  401f6f:	5e                   	pop    esi
  401f70:	33 c0                	xor    eax,eax
  401f72:	5b                   	pop    ebx
  401f73:	83 c4 34             	add    esp,0x34
  401f76:	c3                   	ret
