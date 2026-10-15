
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004057e0 <.text+0x47e0>:
  4057e0:	64 a1 00 00 00 00    	mov    eax,fs:0x0
  4057e6:	6a ff                	push   0xffffffff
  4057e8:	68 a8 a4 43 00       	push   0x43a4a8
  4057ed:	50                   	push   eax
  4057ee:	64 89 25 00 00 00 00 	mov    DWORD PTR fs:0x0,esp
  4057f5:	81 ec f8 00 00 00    	sub    esp,0xf8
  4057fb:	53                   	push   ebx
  4057fc:	55                   	push   ebp
  4057fd:	56                   	push   esi
  4057fe:	8b f1                	mov    esi,ecx
  405800:	57                   	push   edi
  405801:	8d 86 10 01 00 00    	lea    eax,[esi+0x110]
  405807:	50                   	push   eax
  405808:	ff 15 c4 b1 43 00    	call   DWORD PTR ds:0x43b1c4
  40580e:	85 c0                	test   eax,eax
  405810:	0f 84 ca 00 00 00    	je     0x4058e0
  405816:	8a 44 24 13          	mov    al,BYTE PTR [esp+0x13]
  40581a:	6a 00                	push   0x0
  40581c:	8d 4c 24 1c          	lea    ecx,[esp+0x1c]
  405820:	88 44 24 1c          	mov    BYTE PTR [esp+0x1c],al
  405824:	e8 27 0b 00 00       	call   0x406350
  405829:	bf 2c e2 43 00       	mov    edi,0x43e22c
  40582e:	83 c9 ff             	or     ecx,0xffffffff
  405831:	33 c0                	xor    eax,eax
  405833:	6a 01                	push   0x1
  405835:	f2 ae                	repnz scas al,BYTE PTR es:[edi]
  405837:	f7 d1                	not    ecx
  405839:	49                   	dec    ecx
  40583a:	8b d9                	mov    ebx,ecx
  40583c:	8d 4c 24 1c          	lea    ecx,[esp+0x1c]
  405840:	53                   	push   ebx
  405841:	e8 1a 0c 00 00       	call   0x406460
  405846:	84 c0                	test   al,al
  405848:	74 25                	je     0x40586f
  40584a:	8b 7c 24 1c          	mov    edi,DWORD PTR [esp+0x1c]
  40584e:	8b cb                	mov    ecx,ebx
  405850:	8b d1                	mov    edx,ecx
  405852:	be 2c e2 43 00       	mov    esi,0x43e22c
  405857:	c1 e9 02             	shr    ecx,0x2
  40585a:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  40585c:	8b ca                	mov    ecx,edx
  40585e:	83 e1 03             	and    ecx,0x3
  405861:	f3 a4                	rep movs BYTE PTR es:[edi],BYTE PTR ds:[esi]
  405863:	8b 44 24 1c          	mov    eax,DWORD PTR [esp+0x1c]
  405867:	89 5c 24 20          	mov    DWORD PTR [esp+0x20],ebx
  40586b:	c6 04 18 00          	mov    BYTE PTR [eax+ebx*1],0x0
  40586f:	8d 4c 24 28          	lea    ecx,[esp+0x28]
  405873:	c7 84 24 10 01 00 00 	mov    DWORD PTR [esp+0x110],0x0
  40587a:	00 00 00 00 
  40587e:	51                   	push   ecx
  40587f:	8d 4c 24 30          	lea    ecx,[esp+0x30]
  405883:	c7 44 24 2c 74 25 48 	mov    DWORD PTR [esp+0x2c],0x482574
  40588a:	00 
  40588b:	e8 2a ab 02 00       	call   0x4303ba
  405890:	8a 54 24 18          	mov    dl,BYTE PTR [esp+0x18]
  405894:	6a 00                	push   0x0
  405896:	8d 4c 24 3c          	lea    ecx,[esp+0x3c]
  40589a:	c6 84 24 14 01 00 00 	mov    BYTE PTR [esp+0x114],0x1
  4058a1:	01 
  4058a2:	88 54 24 3c          	mov    BYTE PTR [esp+0x3c],dl
  4058a6:	e8 a5 0a 00 00       	call   0x406350
  4058ab:	a1 34 b3 43 00       	mov    eax,ds:0x43b334
  4058b0:	8d 4c 24 18          	lea    ecx,[esp+0x18]
  4058b4:	50                   	push   eax
  4058b5:	6a 00                	push   0x0
  4058b7:	51                   	push   ecx
  4058b8:	8d 4c 24 44          	lea    ecx,[esp+0x44]
  4058bc:	e8 8f 08 00 00       	call   0x406150
  4058c1:	8d 54 24 2c          	lea    edx,[esp+0x2c]
  4058c5:	68 d0 bd 43 00       	push   0x43bdd0
  4058ca:	52                   	push   edx
  4058cb:	c7 44 24 34 28 b3 43 	mov    DWORD PTR [esp+0x34],0x43b328
  4058d2:	00 
  4058d3:	c6 84 24 18 01 00 00 	mov    BYTE PTR [esp+0x118],0x0
  4058da:	00 
  4058db:	e8 b5 ab 02 00       	call   0x430495
  4058e0:	8b ac 24 18 01 00 00 	mov    ebp,DWORD PTR [esp+0x118]
  4058e7:	6a 26                	push   0x26
  4058e9:	55                   	push   ebp
  4058ea:	ff 15 6c b0 43 00    	call   DWORD PTR ds:0x43b06c
  4058f0:	25 00 01 00 00       	and    eax,0x100
  4058f5:	89 44 24 14          	mov    DWORD PTR [esp+0x14],eax
  4058f9:	0f 84 ca 00 00 00    	je     0x4059c9
  4058ff:	8a 44 24 13          	mov    al,BYTE PTR [esp+0x13]
  405903:	6a 00                	push   0x0
  405905:	8d 4c 24 1c          	lea    ecx,[esp+0x1c]
  405909:	88 44 24 1c          	mov    BYTE PTR [esp+0x1c],al
  40590d:	e8 3e 0a 00 00       	call   0x406350
  405912:	bf 14 e2 43 00       	mov    edi,0x43e214
  405917:	83 c9 ff             	or     ecx,0xffffffff
  40591a:	33 c0                	xor    eax,eax
  40591c:	6a 01                	push   0x1
  40591e:	f2 ae                	repnz scas al,BYTE PTR es:[edi]
  405920:	f7 d1                	not    ecx
  405922:	49                   	dec    ecx
  405923:	8b d9                	mov    ebx,ecx
  405925:	8d 4c 24 1c          	lea    ecx,[esp+0x1c]
  405929:	53                   	push   ebx
  40592a:	e8 31 0b 00 00       	call   0x406460
  40592f:	84 c0                	test   al,al
  405931:	74 25                	je     0x405958
  405933:	8b 7c 24 1c          	mov    edi,DWORD PTR [esp+0x1c]
  405937:	8b cb                	mov    ecx,ebx
  405939:	8b d1                	mov    edx,ecx
  40593b:	be 14 e2 43 00       	mov    esi,0x43e214
  405940:	c1 e9 02             	shr    ecx,0x2
  405943:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  405945:	8b ca                	mov    ecx,edx
  405947:	83 e1 03             	and    ecx,0x3
  40594a:	f3 a4                	rep movs BYTE PTR es:[edi],BYTE PTR ds:[esi]
  40594c:	8b 44 24 1c          	mov    eax,DWORD PTR [esp+0x1c]
  405950:	89 5c 24 20          	mov    DWORD PTR [esp+0x20],ebx
  405954:	c6 04 18 00          	mov    BYTE PTR [eax+ebx*1],0x0
  405958:	8d 4c 24 28          	lea    ecx,[esp+0x28]
  40595c:	bb 02 00 00 00       	mov    ebx,0x2
  405961:	51                   	push   ecx
  405962:	8d 4c 24 30          	lea    ecx,[esp+0x30]
  405966:	89 9c 24 14 01 00 00 	mov    DWORD PTR [esp+0x114],ebx
  40596d:	c7 44 24 2c 74 25 48 	mov    DWORD PTR [esp+0x2c],0x482574
  405974:	00 
  405975:	e8 40 aa 02 00       	call   0x4303ba
  40597a:	8a 54 24 18          	mov    dl,BYTE PTR [esp+0x18]
  40597e:	6a 00                	push   0x0
  405980:	8d 4c 24 3c          	lea    ecx,[esp+0x3c]
  405984:	c6 84 24 14 01 00 00 	mov    BYTE PTR [esp+0x114],0x3
  40598b:	03 
  40598c:	88 54 24 3c          	mov    BYTE PTR [esp+0x3c],dl
  405990:	e8 bb 09 00 00       	call   0x406350
  405995:	a1 34 b3 43 00       	mov    eax,ds:0x43b334
  40599a:	8d 4c 24 18          	lea    ecx,[esp+0x18]
  40599e:	50                   	push   eax
  40599f:	6a 00                	push   0x0
  4059a1:	51                   	push   ecx
  4059a2:	8d 4c 24 44          	lea    ecx,[esp+0x44]
  4059a6:	e8 a5 07 00 00       	call   0x406150
  4059ab:	8d 54 24 2c          	lea    edx,[esp+0x2c]
  4059af:	68 d0 bd 43 00       	push   0x43bdd0
  4059b4:	52                   	push   edx
  4059b5:	c7 44 24 34 28 b3 43 	mov    DWORD PTR [esp+0x34],0x43b328
  4059bc:	00 
  4059bd:	88 9c 24 18 01 00 00 	mov    BYTE PTR [esp+0x118],bl
  4059c4:	e8 cc aa 02 00       	call   0x430495
  4059c9:	8b 46 04             	mov    eax,DWORD PTR [esi+0x4]
  4059cc:	85 c0                	test   eax,eax
  4059ce:	0f 84 78 01 00 00    	je     0x405b4c
  4059d4:	8b 4e 08             	mov    ecx,DWORD PTR [esi+0x8]
  4059d7:	85 c9                	test   ecx,ecx
  4059d9:	0f 84 6d 01 00 00    	je     0x405b4c
  4059df:	8b 58 04             	mov    ebx,DWORD PTR [eax+0x4]
  4059e2:	8b 40 08             	mov    eax,DWORD PTR [eax+0x8]
  4059e5:	8d 4c 24 28          	lea    ecx,[esp+0x28]
  4059e9:	6a 00                	push   0x0
  4059eb:	51                   	push   ecx
  4059ec:	6a 04                	push   0x4
  4059ee:	6a 08                	push   0x8
  4059f0:	55                   	push   ebp
  4059f1:	89 44 24 28          	mov    DWORD PTR [esp+0x28],eax
  4059f5:	c7 44 24 3c 03 00 00 	mov    DWORD PTR [esp+0x3c],0x3
  4059fc:	00 
  4059fd:	ff 15 78 b0 43 00    	call   DWORD PTR ds:0x43b078
  405a03:	b9 24 00 00 00       	mov    ecx,0x24
  405a08:	33 c0                	xor    eax,eax
  405a0a:	8d 7c 24 78          	lea    edi,[esp+0x78]
  405a0e:	8d 54 24 74          	lea    edx,[esp+0x74]
  405a12:	c7 44 24 74 94 00 00 	mov    DWORD PTR [esp+0x74],0x94
  405a19:	00 
  405a1a:	52                   	push   edx
  405a1b:	f3 ab                	rep stos DWORD PTR es:[edi],eax
  405a1d:	ff 15 88 b0 43 00    	call   DWORD PTR ds:0x43b088
  405a23:	6a 03                	push   0x3
  405a25:	55                   	push   ebp
  405a26:	ff 15 74 b0 43 00    	call   DWORD PTR ds:0x43b074
  405a2c:	8b 56 04             	mov    edx,DWORD PTR [esi+0x4]
  405a2f:	68 20 00 cc 00       	push   0xcc0020
  405a34:	8b 86 14 01 00 00    	mov    eax,DWORD PTR [esi+0x114]
  405a3a:	6a 00                	push   0x0
  405a3c:	52                   	push   edx
  405a3d:	8b 56 08             	mov    edx,DWORD PTR [esi+0x8]
  405a40:	52                   	push   edx
  405a41:	8b 54 24 24          	mov    edx,DWORD PTR [esp+0x24]
  405a45:	8b 8e 10 01 00 00    	mov    ecx,DWORD PTR [esi+0x110]
  405a4b:	52                   	push   edx
  405a4c:	8b 96 1c 01 00 00    	mov    edx,DWORD PTR [esi+0x11c]
  405a52:	53                   	push   ebx
  405a53:	6a 00                	push   0x0
  405a55:	2b d0                	sub    edx,eax
  405a57:	6a 00                	push   0x0
  405a59:	52                   	push   edx
  405a5a:	8b 96 18 01 00 00    	mov    edx,DWORD PTR [esi+0x118]
  405a60:	2b d1                	sub    edx,ecx
  405a62:	52                   	push   edx
  405a63:	50                   	push   eax
  405a64:	51                   	push   ecx
  405a65:	55                   	push   ebp
  405a66:	ff 15 70 b0 43 00    	call   DWORD PTR ds:0x43b070
  405a6c:	33 c9                	xor    ecx,ecx
  405a6e:	83 f8 ff             	cmp    eax,0xffffffff
  405a71:	0f 95 c1             	setne  cl
  405a74:	83 e1 01             	and    ecx,0x1
  405a77:	89 4c 24 14          	mov    DWORD PTR [esp+0x14],ecx
  405a7b:	0f 85 95 01 00 00    	jne    0x405c16
  405a81:	8a 54 24 13          	mov    dl,BYTE PTR [esp+0x13]
  405a85:	6a 00                	push   0x0
  405a87:	8d 4c 24 1c          	lea    ecx,[esp+0x1c]
  405a8b:	88 54 24 1c          	mov    BYTE PTR [esp+0x1c],dl
  405a8f:	e8 bc 08 00 00       	call   0x406350
  405a94:	bf 00 e2 43 00       	mov    edi,0x43e200
  405a99:	83 c9 ff             	or     ecx,0xffffffff
  405a9c:	33 c0                	xor    eax,eax
  405a9e:	6a 01                	push   0x1
  405aa0:	f2 ae                	repnz scas al,BYTE PTR es:[edi]
  405aa2:	f7 d1                	not    ecx
  405aa4:	49                   	dec    ecx
  405aa5:	8b d9                	mov    ebx,ecx
  405aa7:	8d 4c 24 1c          	lea    ecx,[esp+0x1c]
  405aab:	53                   	push   ebx
  405aac:	e8 af 09 00 00       	call   0x406460
  405ab1:	84 c0                	test   al,al
  405ab3:	74 25                	je     0x405ada
  405ab5:	8b 7c 24 1c          	mov    edi,DWORD PTR [esp+0x1c]
  405ab9:	8b cb                	mov    ecx,ebx
  405abb:	8b c1                	mov    eax,ecx
  405abd:	be 00 e2 43 00       	mov    esi,0x43e200
  405ac2:	c1 e9 02             	shr    ecx,0x2
  405ac5:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  405ac7:	8b c8                	mov    ecx,eax
  405ac9:	83 e1 03             	and    ecx,0x3
  405acc:	f3 a4                	rep movs BYTE PTR es:[edi],BYTE PTR ds:[esi]
  405ace:	8b 4c 24 1c          	mov    ecx,DWORD PTR [esp+0x1c]
  405ad2:	89 5c 24 20          	mov    DWORD PTR [esp+0x20],ebx
  405ad6:	c6 04 19 00          	mov    BYTE PTR [ecx+ebx*1],0x0
  405ada:	8d 54 24 14          	lea    edx,[esp+0x14]
  405ade:	8d 4c 24 2c          	lea    ecx,[esp+0x2c]
  405ae2:	52                   	push   edx
  405ae3:	c7 84 24 14 01 00 00 	mov    DWORD PTR [esp+0x114],0x4
  405aea:	04 00 00 00 
  405aee:	c7 44 24 18 74 25 48 	mov    DWORD PTR [esp+0x18],0x482574
  405af5:	00 
  405af6:	e8 bf a8 02 00       	call   0x4303ba
  405afb:	8a 44 24 18          	mov    al,BYTE PTR [esp+0x18]
  405aff:	6a 00                	push   0x0
  405b01:	8d 4c 24 3c          	lea    ecx,[esp+0x3c]
  405b05:	c6 84 24 14 01 00 00 	mov    BYTE PTR [esp+0x114],0x5
  405b0c:	05 
  405b0d:	88 44 24 3c          	mov    BYTE PTR [esp+0x3c],al
  405b11:	e8 3a 08 00 00       	call   0x406350
  405b16:	8b 0d 34 b3 43 00    	mov    ecx,DWORD PTR ds:0x43b334
  405b1c:	8d 54 24 18          	lea    edx,[esp+0x18]
  405b20:	51                   	push   ecx
  405b21:	6a 00                	push   0x0
  405b23:	52                   	push   edx
  405b24:	8d 4c 24 44          	lea    ecx,[esp+0x44]
  405b28:	e8 23 06 00 00       	call   0x406150
  405b2d:	8d 44 24 2c          	lea    eax,[esp+0x2c]
  405b31:	68 d0 bd 43 00       	push   0x43bdd0
  405b36:	50                   	push   eax
  405b37:	c7 44 24 34 28 b3 43 	mov    DWORD PTR [esp+0x34],0x43b328
  405b3e:	00 
  405b3f:	c6 84 24 18 01 00 00 	mov    BYTE PTR [esp+0x118],0x4
  405b46:	04 
  405b47:	e8 49 a9 02 00       	call   0x430495
  405b4c:	8a 4c 24 13          	mov    cl,BYTE PTR [esp+0x13]
  405b50:	6a 00                	push   0x0
  405b52:	88 4c 24 4c          	mov    BYTE PTR [esp+0x4c],cl
  405b56:	8d 4c 24 4c          	lea    ecx,[esp+0x4c]
  405b5a:	e8 f1 07 00 00       	call   0x406350
  405b5f:	bf e8 e1 43 00       	mov    edi,0x43e1e8
  405b64:	83 c9 ff             	or     ecx,0xffffffff
  405b67:	33 c0                	xor    eax,eax
  405b69:	6a 01                	push   0x1
  405b6b:	f2 ae                	repnz scas al,BYTE PTR es:[edi]
  405b6d:	f7 d1                	not    ecx
  405b6f:	49                   	dec    ecx
  405b70:	8b d9                	mov    ebx,ecx
  405b72:	8d 4c 24 4c          	lea    ecx,[esp+0x4c]
  405b76:	53                   	push   ebx
  405b77:	e8 e4 08 00 00       	call   0x406460
  405b7c:	84 c0                	test   al,al
  405b7e:	74 25                	je     0x405ba5
  405b80:	8b 7c 24 4c          	mov    edi,DWORD PTR [esp+0x4c]
  405b84:	8b cb                	mov    ecx,ebx
  405b86:	8b d1                	mov    edx,ecx
  405b88:	be e8 e1 43 00       	mov    esi,0x43e1e8
  405b8d:	c1 e9 02             	shr    ecx,0x2
  405b90:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  405b92:	8b ca                	mov    ecx,edx
  405b94:	83 e1 03             	and    ecx,0x3
  405b97:	f3 a4                	rep movs BYTE PTR es:[edi],BYTE PTR ds:[esi]
  405b99:	8b 44 24 4c          	mov    eax,DWORD PTR [esp+0x4c]
  405b9d:	89 5c 24 50          	mov    DWORD PTR [esp+0x50],ebx
  405ba1:	c6 04 18 00          	mov    BYTE PTR [eax+ebx*1],0x0
  405ba5:	8d 4c 24 14          	lea    ecx,[esp+0x14]
  405ba9:	bb 06 00 00 00       	mov    ebx,0x6
  405bae:	51                   	push   ecx
  405baf:	8d 4c 24 5c          	lea    ecx,[esp+0x5c]
  405bb3:	89 9c 24 14 01 00 00 	mov    DWORD PTR [esp+0x114],ebx
  405bba:	c7 44 24 18 74 25 48 	mov    DWORD PTR [esp+0x18],0x482574
  405bc1:	00 
  405bc2:	e8 f3 a7 02 00       	call   0x4303ba
  405bc7:	8a 54 24 48          	mov    dl,BYTE PTR [esp+0x48]
  405bcb:	6a 00                	push   0x0
  405bcd:	8d 4c 24 68          	lea    ecx,[esp+0x68]
  405bd1:	c6 84 24 14 01 00 00 	mov    BYTE PTR [esp+0x114],0x7
  405bd8:	07 
  405bd9:	88 54 24 68          	mov    BYTE PTR [esp+0x68],dl
  405bdd:	e8 6e 07 00 00       	call   0x406350
  405be2:	a1 34 b3 43 00       	mov    eax,ds:0x43b334
  405be7:	8d 4c 24 48          	lea    ecx,[esp+0x48]
  405beb:	50                   	push   eax
  405bec:	6a 00                	push   0x0
  405bee:	51                   	push   ecx
  405bef:	8d 4c 24 70          	lea    ecx,[esp+0x70]
  405bf3:	e8 58 05 00 00       	call   0x406150
  405bf8:	8d 54 24 58          	lea    edx,[esp+0x58]
  405bfc:	68 d0 bd 43 00       	push   0x43bdd0
  405c01:	52                   	push   edx
  405c02:	c7 44 24 60 28 b3 43 	mov    DWORD PTR [esp+0x60],0x43b328
  405c09:	00 
  405c0a:	88 9c 24 18 01 00 00 	mov    BYTE PTR [esp+0x118],bl
  405c11:	e8 7f a8 02 00       	call   0x430495
  405c16:	8b 8c 24 08 01 00 00 	mov    ecx,DWORD PTR [esp+0x108]
  405c1d:	5f                   	pop    edi
  405c1e:	5e                   	pop    esi
  405c1f:	5d                   	pop    ebp
  405c20:	5b                   	pop    ebx
  405c21:	64 89 0d 00 00 00 00 	mov    DWORD PTR fs:0x0,ecx
  405c28:	81 c4 04 01 00 00    	add    esp,0x104
  405c2e:	c2 04 00             	ret    0x4
