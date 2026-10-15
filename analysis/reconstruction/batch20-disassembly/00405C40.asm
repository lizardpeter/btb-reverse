
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00405c40 <.text+0x4c40>:
  405c40:	64 a1 00 00 00 00    	mov    eax,fs:0x0
  405c46:	6a ff                	push   0xffffffff
  405c48:	68 e0 a4 43 00       	push   0x43a4e0
  405c4d:	50                   	push   eax
  405c4e:	64 89 25 00 00 00 00 	mov    DWORD PTR fs:0x0,esp
  405c55:	83 ec 78             	sub    esp,0x78
  405c58:	8d 44 24 28          	lea    eax,[esp+0x28]
  405c5c:	53                   	push   ebx
  405c5d:	55                   	push   ebp
  405c5e:	56                   	push   esi
  405c5f:	8b f1                	mov    esi,ecx
  405c61:	8b 8c 24 94 00 00 00 	mov    ecx,DWORD PTR [esp+0x94]
  405c68:	57                   	push   edi
  405c69:	50                   	push   eax
  405c6a:	6a 18                	push   0x18
  405c6c:	51                   	push   ecx
  405c6d:	ff 15 4c b0 43 00    	call   DWORD PTR ds:0x43b04c
  405c73:	83 f8 18             	cmp    eax,0x18
  405c76:	0f 85 75 01 00 00    	jne    0x405df1
  405c7c:	8b 44 24 3c          	mov    eax,DWORD PTR [esp+0x3c]
  405c80:	8b 4c 24 40          	mov    ecx,DWORD PTR [esp+0x40]
  405c84:	8d 14 40             	lea    edx,[eax+eax*2]
  405c87:	8d 04 d5 1f 00 00 00 	lea    eax,[edx*8+0x1f]
  405c8e:	c1 f8 03             	sar    eax,0x3
  405c91:	24 fc                	and    al,0xfc
  405c93:	0f af c8             	imul   ecx,eax
  405c96:	8b 46 04             	mov    eax,DWORD PTR [esi+0x4]
  405c99:	50                   	push   eax
  405c9a:	8d 79 28             	lea    edi,[ecx+0x28]
  405c9d:	e8 3a 9f 02 00       	call   0x42fbdc
  405ca2:	57                   	push   edi
  405ca3:	e8 26 9f 02 00       	call   0x42fbce
  405ca8:	8b d0                	mov    edx,eax
  405caa:	8b cf                	mov    ecx,edi
  405cac:	8b d9                	mov    ebx,ecx
  405cae:	8b fa                	mov    edi,edx
  405cb0:	8d 42 28             	lea    eax,[edx+0x28]
  405cb3:	89 56 04             	mov    DWORD PTR [esi+0x4],edx
  405cb6:	89 46 08             	mov    DWORD PTR [esi+0x8],eax
  405cb9:	33 c0                	xor    eax,eax
  405cbb:	c1 e9 02             	shr    ecx,0x2
  405cbe:	f3 ab                	rep stos DWORD PTR es:[edi],eax
  405cc0:	8b cb                	mov    ecx,ebx
  405cc2:	83 c4 08             	add    esp,0x8
  405cc5:	83 e1 03             	and    ecx,0x3
  405cc8:	33 db                	xor    ebx,ebx
  405cca:	f3 aa                	rep stos BYTE PTR es:[edi],al
  405ccc:	c7 02 28 00 00 00    	mov    DWORD PTR [edx],0x28
  405cd2:	8b 4c 24 3c          	mov    ecx,DWORD PTR [esp+0x3c]
  405cd6:	89 4a 04             	mov    DWORD PTR [edx+0x4],ecx
  405cd9:	8b 44 24 40          	mov    eax,DWORD PTR [esp+0x40]
  405cdd:	53                   	push   ebx
  405cde:	89 42 08             	mov    DWORD PTR [edx+0x8],eax
  405ce1:	66 c7 42 0e 18 00    	mov    WORD PTR [edx+0xe],0x18
  405ce7:	66 c7 42 0c 01 00    	mov    WORD PTR [edx+0xc],0x1
  405ced:	89 5a 10             	mov    DWORD PTR [edx+0x10],ebx
  405cf0:	ff 15 0c b2 43 00    	call   DWORD PTR ds:0x43b20c
  405cf6:	8b ac 24 9c 00 00 00 	mov    ebp,DWORD PTR [esp+0x9c]
  405cfd:	8b f8                	mov    edi,eax
  405cff:	3b eb                	cmp    ebp,ebx
  405d01:	74 13                	je     0x405d16
  405d03:	6a 01                	push   0x1
  405d05:	55                   	push   ebp
  405d06:	57                   	push   edi
  405d07:	ff 15 5c b0 43 00    	call   DWORD PTR ds:0x43b05c
  405d0d:	57                   	push   edi
  405d0e:	8b e8                	mov    ebp,eax
  405d10:	ff 15 58 b0 43 00    	call   DWORD PTR ds:0x43b058
  405d16:	8b 4e 04             	mov    ecx,DWORD PTR [esi+0x4]
  405d19:	8b 56 08             	mov    edx,DWORD PTR [esi+0x8]
  405d1c:	8b 44 24 40          	mov    eax,DWORD PTR [esp+0x40]
  405d20:	53                   	push   ebx
  405d21:	51                   	push   ecx
  405d22:	8b 8c 24 a0 00 00 00 	mov    ecx,DWORD PTR [esp+0xa0]
  405d29:	52                   	push   edx
  405d2a:	50                   	push   eax
  405d2b:	53                   	push   ebx
  405d2c:	51                   	push   ecx
  405d2d:	57                   	push   edi
  405d2e:	ff 15 54 b0 43 00    	call   DWORD PTR ds:0x43b054
  405d34:	3b eb                	cmp    ebp,ebx
  405d36:	8b f0                	mov    esi,eax
  405d38:	74 0a                	je     0x405d44
  405d3a:	6a 01                	push   0x1
  405d3c:	55                   	push   ebp
  405d3d:	57                   	push   edi
  405d3e:	ff 15 5c b0 43 00    	call   DWORD PTR ds:0x43b05c
  405d44:	57                   	push   edi
  405d45:	53                   	push   ebx
  405d46:	ff 15 10 b2 43 00    	call   DWORD PTR ds:0x43b210
  405d4c:	3b f3                	cmp    esi,ebx
  405d4e:	0f 85 69 01 00 00    	jne    0x405ebd
  405d54:	bf 60 e2 43 00       	mov    edi,0x43e260
  405d59:	83 c9 ff             	or     ecx,0xffffffff
  405d5c:	33 c0                	xor    eax,eax
  405d5e:	8a 54 24 13          	mov    dl,BYTE PTR [esp+0x13]
  405d62:	f2 ae                	repnz scas al,BYTE PTR es:[edi]
  405d64:	f7 d1                	not    ecx
  405d66:	49                   	dec    ecx
  405d67:	88 54 24 28          	mov    BYTE PTR [esp+0x28],dl
  405d6b:	51                   	push   ecx
  405d6c:	68 60 e2 43 00       	push   0x43e260
  405d71:	8d 4c 24 30          	lea    ecx,[esp+0x30]
  405d75:	89 5c 24 34          	mov    DWORD PTR [esp+0x34],ebx
  405d79:	89 5c 24 38          	mov    DWORD PTR [esp+0x38],ebx
  405d7d:	89 5c 24 3c          	mov    DWORD PTR [esp+0x3c],ebx
  405d81:	e8 1a 06 00 00       	call   0x4063a0
  405d86:	8d 44 24 14          	lea    eax,[esp+0x14]
  405d8a:	8d 4c 24 50          	lea    ecx,[esp+0x50]
  405d8e:	50                   	push   eax
  405d8f:	89 9c 24 94 00 00 00 	mov    DWORD PTR [esp+0x94],ebx
  405d96:	c7 44 24 18 74 25 48 	mov    DWORD PTR [esp+0x18],0x482574
  405d9d:	00 
  405d9e:	e8 17 a6 02 00       	call   0x4303ba
  405da3:	8a 4c 24 28          	mov    cl,BYTE PTR [esp+0x28]
  405da7:	53                   	push   ebx
  405da8:	88 4c 24 60          	mov    BYTE PTR [esp+0x60],cl
  405dac:	8d 4c 24 60          	lea    ecx,[esp+0x60]
  405db0:	c6 84 24 94 00 00 00 	mov    BYTE PTR [esp+0x94],0x1
  405db7:	01 
  405db8:	e8 93 05 00 00       	call   0x406350
  405dbd:	8b 15 34 b3 43 00    	mov    edx,DWORD PTR ds:0x43b334
  405dc3:	8d 44 24 28          	lea    eax,[esp+0x28]
  405dc7:	52                   	push   edx
  405dc8:	53                   	push   ebx
  405dc9:	50                   	push   eax
  405dca:	8d 4c 24 68          	lea    ecx,[esp+0x68]
  405dce:	e8 7d 03 00 00       	call   0x406150
  405dd3:	8d 4c 24 50          	lea    ecx,[esp+0x50]
  405dd7:	68 d0 bd 43 00       	push   0x43bdd0
  405ddc:	51                   	push   ecx
  405ddd:	c7 44 24 58 28 b3 43 	mov    DWORD PTR [esp+0x58],0x43b328
  405de4:	00 
  405de5:	88 9c 24 98 00 00 00 	mov    BYTE PTR [esp+0x98],bl
  405dec:	e8 a4 a6 02 00       	call   0x430495
  405df1:	8a 54 24 13          	mov    dl,BYTE PTR [esp+0x13]
  405df5:	33 db                	xor    ebx,ebx
  405df7:	53                   	push   ebx
  405df8:	8d 4c 24 1c          	lea    ecx,[esp+0x1c]
  405dfc:	88 54 24 1c          	mov    BYTE PTR [esp+0x1c],dl
  405e00:	e8 4b 05 00 00       	call   0x406350
  405e05:	bf 48 e2 43 00       	mov    edi,0x43e248
  405e0a:	83 c9 ff             	or     ecx,0xffffffff
  405e0d:	33 c0                	xor    eax,eax
  405e0f:	6a 01                	push   0x1
  405e11:	f2 ae                	repnz scas al,BYTE PTR es:[edi]
  405e13:	f7 d1                	not    ecx
  405e15:	49                   	dec    ecx
  405e16:	8b e9                	mov    ebp,ecx
  405e18:	8d 4c 24 1c          	lea    ecx,[esp+0x1c]
  405e1c:	55                   	push   ebp
  405e1d:	e8 3e 06 00 00       	call   0x406460
  405e22:	84 c0                	test   al,al
  405e24:	74 24                	je     0x405e4a
  405e26:	8b 7c 24 1c          	mov    edi,DWORD PTR [esp+0x1c]
  405e2a:	8b cd                	mov    ecx,ebp
  405e2c:	8b c1                	mov    eax,ecx
  405e2e:	be 48 e2 43 00       	mov    esi,0x43e248
  405e33:	c1 e9 02             	shr    ecx,0x2
  405e36:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  405e38:	8b c8                	mov    ecx,eax
  405e3a:	83 e1 03             	and    ecx,0x3
  405e3d:	f3 a4                	rep movs BYTE PTR es:[edi],BYTE PTR ds:[esi]
  405e3f:	8b 4c 24 1c          	mov    ecx,DWORD PTR [esp+0x1c]
  405e43:	89 6c 24 20          	mov    DWORD PTR [esp+0x20],ebp
  405e47:	88 1c 29             	mov    BYTE PTR [ecx+ebp*1],bl
  405e4a:	8d 54 24 14          	lea    edx,[esp+0x14]
  405e4e:	8d 4c 24 6c          	lea    ecx,[esp+0x6c]
  405e52:	52                   	push   edx
  405e53:	c7 84 24 94 00 00 00 	mov    DWORD PTR [esp+0x94],0x2
  405e5a:	02 00 00 00 
  405e5e:	c7 44 24 18 74 25 48 	mov    DWORD PTR [esp+0x18],0x482574
  405e65:	00 
  405e66:	e8 4f a5 02 00       	call   0x4303ba
  405e6b:	8a 44 24 18          	mov    al,BYTE PTR [esp+0x18]
  405e6f:	53                   	push   ebx
  405e70:	8d 4c 24 7c          	lea    ecx,[esp+0x7c]
  405e74:	c6 84 24 94 00 00 00 	mov    BYTE PTR [esp+0x94],0x3
  405e7b:	03 
  405e7c:	88 44 24 7c          	mov    BYTE PTR [esp+0x7c],al
  405e80:	e8 cb 04 00 00       	call   0x406350
  405e85:	8b 0d 34 b3 43 00    	mov    ecx,DWORD PTR ds:0x43b334
  405e8b:	8d 54 24 18          	lea    edx,[esp+0x18]
  405e8f:	51                   	push   ecx
  405e90:	53                   	push   ebx
  405e91:	52                   	push   edx
  405e92:	8d 8c 24 84 00 00 00 	lea    ecx,[esp+0x84]
  405e99:	e8 b2 02 00 00       	call   0x406150
  405e9e:	8d 44 24 6c          	lea    eax,[esp+0x6c]
  405ea2:	68 d0 bd 43 00       	push   0x43bdd0
  405ea7:	50                   	push   eax
  405ea8:	c7 44 24 74 28 b3 43 	mov    DWORD PTR [esp+0x74],0x43b328
  405eaf:	00 
  405eb0:	c6 84 24 98 00 00 00 	mov    BYTE PTR [esp+0x98],0x2
  405eb7:	02 
  405eb8:	e8 d8 a5 02 00       	call   0x430495
  405ebd:	8b 8c 24 88 00 00 00 	mov    ecx,DWORD PTR [esp+0x88]
  405ec4:	5f                   	pop    edi
  405ec5:	5e                   	pop    esi
  405ec6:	5d                   	pop    ebp
  405ec7:	5b                   	pop    ebx
  405ec8:	64 89 0d 00 00 00 00 	mov    DWORD PTR fs:0x0,ecx
  405ecf:	81 c4 84 00 00 00    	add    esp,0x84
  405ed5:	c2 08 00             	ret    0x8
