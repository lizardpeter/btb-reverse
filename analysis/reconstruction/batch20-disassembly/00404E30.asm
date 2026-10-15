
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00404e30 <.text+0x3e30>:
  404e30:	6a ff                	push   0xffffffff
  404e32:	68 c0 a3 43 00       	push   0x43a3c0
  404e37:	64 a1 00 00 00 00    	mov    eax,fs:0x0
  404e3d:	50                   	push   eax
  404e3e:	64 89 25 00 00 00 00 	mov    DWORD PTR fs:0x0,esp
  404e45:	83 ec 44             	sub    esp,0x44
  404e48:	33 c0                	xor    eax,eax
  404e4a:	53                   	push   ebx
  404e4b:	56                   	push   esi
  404e4c:	89 44 24 1c          	mov    DWORD PTR [esp+0x1c],eax
  404e50:	8b f1                	mov    esi,ecx
  404e52:	89 44 24 20          	mov    DWORD PTR [esp+0x20],eax
  404e56:	8d 4c 24 18          	lea    ecx,[esp+0x18]
  404e5a:	57                   	push   edi
  404e5b:	8b 7c 24 60          	mov    edi,DWORD PTR [esp+0x60]
  404e5f:	89 44 24 28          	mov    DWORD PTR [esp+0x28],eax
  404e63:	51                   	push   ecx
  404e64:	33 db                	xor    ebx,ebx
  404e66:	89 44 24 30          	mov    DWORD PTR [esp+0x30],eax
  404e6a:	6a 18                	push   0x18
  404e6c:	57                   	push   edi
  404e6d:	89 5c 24 28          	mov    DWORD PTR [esp+0x28],ebx
  404e71:	89 44 24 3c          	mov    DWORD PTR [esp+0x3c],eax
  404e75:	ff 15 4c b0 43 00    	call   DWORD PTR ds:0x43b04c
  404e7b:	83 f8 18             	cmp    eax,0x18
  404e7e:	0f 84 bf 00 00 00    	je     0x404f43
  404e84:	bf a0 e1 43 00       	mov    edi,0x43e1a0
  404e89:	83 c9 ff             	or     ecx,0xffffffff
  404e8c:	33 c0                	xor    eax,eax
  404e8e:	8a 54 24 60          	mov    dl,BYTE PTR [esp+0x60]
  404e92:	f2 ae                	repnz scas al,BYTE PTR es:[edi]
  404e94:	f7 d1                	not    ecx
  404e96:	49                   	dec    ecx
  404e97:	55                   	push   ebp
  404e98:	8b e9                	mov    ebp,ecx
  404e9a:	6a 01                	push   0x1
  404e9c:	55                   	push   ebp
  404e9d:	8d 4c 24 18          	lea    ecx,[esp+0x18]
  404ea1:	88 54 24 18          	mov    BYTE PTR [esp+0x18],dl
  404ea5:	89 5c 24 1c          	mov    DWORD PTR [esp+0x1c],ebx
  404ea9:	89 5c 24 20          	mov    DWORD PTR [esp+0x20],ebx
  404ead:	89 5c 24 24          	mov    DWORD PTR [esp+0x24],ebx
  404eb1:	e8 aa 15 00 00       	call   0x406460
  404eb6:	84 c0                	test   al,al
  404eb8:	74 24                	je     0x404ede
  404eba:	8b 7c 24 14          	mov    edi,DWORD PTR [esp+0x14]
  404ebe:	8b cd                	mov    ecx,ebp
  404ec0:	8b c1                	mov    eax,ecx
  404ec2:	be a0 e1 43 00       	mov    esi,0x43e1a0
  404ec7:	c1 e9 02             	shr    ecx,0x2
  404eca:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  404ecc:	8b c8                	mov    ecx,eax
  404ece:	83 e1 03             	and    ecx,0x3
  404ed1:	f3 a4                	rep movs BYTE PTR es:[edi],BYTE PTR ds:[esi]
  404ed3:	8b 4c 24 14          	mov    ecx,DWORD PTR [esp+0x14]
  404ed7:	89 6c 24 18          	mov    DWORD PTR [esp+0x18],ebp
  404edb:	88 1c 29             	mov    BYTE PTR [ecx+ebp*1],bl
  404ede:	8d 54 24 64          	lea    edx,[esp+0x64]
  404ee2:	8d 4c 24 38          	lea    ecx,[esp+0x38]
  404ee6:	52                   	push   edx
  404ee7:	89 5c 24 60          	mov    DWORD PTR [esp+0x60],ebx
  404eeb:	c7 44 24 68 74 25 48 	mov    DWORD PTR [esp+0x68],0x482574
  404ef2:	00 
  404ef3:	e8 c2 b4 02 00       	call   0x4303ba
  404ef8:	8b 0d 34 b3 43 00    	mov    ecx,DWORD PTR ds:0x43b334
  404efe:	8a 44 24 10          	mov    al,BYTE PTR [esp+0x10]
  404f02:	51                   	push   ecx
  404f03:	8d 54 24 14          	lea    edx,[esp+0x14]
  404f07:	53                   	push   ebx
  404f08:	52                   	push   edx
  404f09:	8d 4c 24 50          	lea    ecx,[esp+0x50]
  404f0d:	c6 44 24 68 01       	mov    BYTE PTR [esp+0x68],0x1
  404f12:	88 44 24 50          	mov    BYTE PTR [esp+0x50],al
  404f16:	89 5c 24 54          	mov    DWORD PTR [esp+0x54],ebx
  404f1a:	89 5c 24 58          	mov    DWORD PTR [esp+0x58],ebx
  404f1e:	89 5c 24 5c          	mov    DWORD PTR [esp+0x5c],ebx
  404f22:	e8 29 12 00 00       	call   0x406150
  404f27:	8d 44 24 38          	lea    eax,[esp+0x38]
  404f2b:	68 d0 bd 43 00       	push   0x43bdd0
  404f30:	50                   	push   eax
  404f31:	c7 44 24 40 28 b3 43 	mov    DWORD PTR [esp+0x40],0x43b328
  404f38:	00 
  404f39:	88 5c 24 64          	mov    BYTE PTR [esp+0x64],bl
  404f3d:	e8 53 b5 02 00       	call   0x430495
  404f42:	5d                   	pop    ebp
  404f43:	39 5c 24 30          	cmp    DWORD PTR [esp+0x30],ebx
  404f47:	74 1c                	je     0x404f65
  404f49:	8b 16                	mov    edx,DWORD PTR [esi]
  404f4b:	57                   	push   edi
  404f4c:	8b ce                	mov    ecx,esi
  404f4e:	ff 52 30             	call   DWORD PTR [edx+0x30]
  404f51:	5f                   	pop    edi
  404f52:	5e                   	pop    esi
  404f53:	5b                   	pop    ebx
  404f54:	8b 4c 24 44          	mov    ecx,DWORD PTR [esp+0x44]
  404f58:	64 89 0d 00 00 00 00 	mov    DWORD PTR fs:0x0,ecx
  404f5f:	83 c4 50             	add    esp,0x50
  404f62:	c2 08 00             	ret    0x8
  404f65:	8b 4c 24 64          	mov    ecx,DWORD PTR [esp+0x64]
  404f69:	8b 06                	mov    eax,DWORD PTR [esi]
  404f6b:	51                   	push   ecx
  404f6c:	57                   	push   edi
  404f6d:	8b ce                	mov    ecx,esi
  404f6f:	ff 50 2c             	call   DWORD PTR [eax+0x2c]
  404f72:	8b 4c 24 50          	mov    ecx,DWORD PTR [esp+0x50]
  404f76:	5f                   	pop    edi
  404f77:	5e                   	pop    esi
  404f78:	5b                   	pop    ebx
  404f79:	64 89 0d 00 00 00 00 	mov    DWORD PTR fs:0x0,ecx
  404f80:	83 c4 50             	add    esp,0x50
  404f83:	c2                   	.byte 0xc2
  404f84:	08                   	.byte 0x8
