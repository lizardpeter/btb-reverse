
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004049b0 <.text+0x39b0>:
  4049b0:	83 ec 48             	sub    esp,0x48
  4049b3:	53                   	push   ebx
  4049b4:	55                   	push   ebp
  4049b5:	56                   	push   esi
  4049b6:	8b f1                	mov    esi,ecx
  4049b8:	57                   	push   edi
  4049b9:	8b 86 80 00 00 00    	mov    eax,DWORD PTR [esi+0x80]
  4049bf:	85 c0                	test   eax,eax
  4049c1:	74 73                	je     0x404a36
  4049c3:	8b 86 88 00 00 00    	mov    eax,DWORD PTR [esi+0x88]
  4049c9:	85 c0                	test   eax,eax
  4049cb:	75 0f                	jne    0x4049dc
  4049cd:	5f                   	pop    edi
  4049ce:	5e                   	pop    esi
  4049cf:	5d                   	pop    ebp
  4049d0:	b8 f0 01 04 80       	mov    eax,0x800401f0
  4049d5:	5b                   	pop    ebx
  4049d6:	83 c4 48             	add    esp,0x48
  4049d9:	c2 0c 00             	ret    0xc
  4049dc:	8b 5c 24 64          	mov    ebx,DWORD PTR [esp+0x64]
  4049e0:	85 db                	test   ebx,ebx
  4049e2:	74 06                	je     0x4049ea
  4049e4:	c7 03 00 00 00 00    	mov    DWORD PTR [ebx],0x0
  4049ea:	8b 8e 8c 00 00 00    	mov    ecx,DWORD PTR [esi+0x8c]
  4049f0:	8b 86 84 00 00 00    	mov    eax,DWORD PTR [esi+0x84]
  4049f6:	8b b6 88 00 00 00    	mov    esi,DWORD PTR [esi+0x88]
  4049fc:	8b 54 24 60          	mov    edx,DWORD PTR [esp+0x60]
  404a00:	8d 3c 08             	lea    edi,[eax+ecx*1]
  404a03:	8d 2c 16             	lea    ebp,[esi+edx*1]
  404a06:	3b ef                	cmp    ebp,edi
  404a08:	76 06                	jbe    0x404a10
  404a0a:	2b c6                	sub    eax,esi
  404a0c:	03 c1                	add    eax,ecx
  404a0e:	8b d0                	mov    edx,eax
  404a10:	8b 7c 24 5c          	mov    edi,DWORD PTR [esp+0x5c]
  404a14:	8b ca                	mov    ecx,edx
  404a16:	8b c1                	mov    eax,ecx
  404a18:	c1 e9 02             	shr    ecx,0x2
  404a1b:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  404a1d:	8b c8                	mov    ecx,eax
  404a1f:	83 e1 03             	and    ecx,0x3
  404a22:	85 db                	test   ebx,ebx
  404a24:	f3 a4                	rep movs BYTE PTR es:[edi],BYTE PTR ds:[esi]
  404a26:	74 02                	je     0x404a2a
  404a28:	89 13                	mov    DWORD PTR [ebx],edx
  404a2a:	5f                   	pop    edi
  404a2b:	5e                   	pop    esi
  404a2c:	5d                   	pop    ebp
  404a2d:	33 c0                	xor    eax,eax
  404a2f:	5b                   	pop    ebx
  404a30:	83 c4 48             	add    esp,0x48
  404a33:	c2 0c 00             	ret    0xc
  404a36:	8b 46 04             	mov    eax,DWORD PTR [esi+0x4]
  404a39:	85 c0                	test   eax,eax
  404a3b:	75 0f                	jne    0x404a4c
  404a3d:	5f                   	pop    edi
  404a3e:	5e                   	pop    esi
  404a3f:	5d                   	pop    ebp
  404a40:	b8 f0 01 04 80       	mov    eax,0x800401f0
  404a45:	5b                   	pop    ebx
  404a46:	83 c4 48             	add    esp,0x48
  404a49:	c2 0c 00             	ret    0xc
  404a4c:	8b 44 24 5c          	mov    eax,DWORD PTR [esp+0x5c]
  404a50:	85 c0                	test   eax,eax
  404a52:	0f 84 c6 00 00 00    	je     0x404b1e
  404a58:	8b 6c 24 64          	mov    ebp,DWORD PTR [esp+0x64]
  404a5c:	85 ed                	test   ebp,ebp
  404a5e:	0f 84 ba 00 00 00    	je     0x404b1e
  404a64:	c7 45 00 00 00 00 00 	mov    DWORD PTR [ebp+0x0],0x0
  404a6b:	8b 56 04             	mov    edx,DWORD PTR [esi+0x4]
  404a6e:	8d 4c 24 10          	lea    ecx,[esp+0x10]
  404a72:	6a 00                	push   0x0
  404a74:	51                   	push   ecx
  404a75:	52                   	push   edx
  404a76:	ff 15 78 b2 43 00    	call   DWORD PTR ds:0x43b278
  404a7c:	85 c0                	test   eax,eax
  404a7e:	74 0f                	je     0x404a8f
  404a80:	5f                   	pop    edi
  404a81:	5e                   	pop    esi
  404a82:	5d                   	pop    ebp
  404a83:	b8 05 40 00 80       	mov    eax,0x80004005
  404a88:	5b                   	pop    ebx
  404a89:	83 c4 48             	add    esp,0x48
  404a8c:	c2 0c 00             	ret    0xc
  404a8f:	8b 5c 24 60          	mov    ebx,DWORD PTR [esp+0x60]
  404a93:	8b 46 0c             	mov    eax,DWORD PTR [esi+0xc]
  404a96:	3b d8                	cmp    ebx,eax
  404a98:	76 02                	jbe    0x404a9c
  404a9a:	8b d8                	mov    ebx,eax
  404a9c:	2b c3                	sub    eax,ebx
  404a9e:	33 ff                	xor    edi,edi
  404aa0:	85 db                	test   ebx,ebx
  404aa2:	89 46 0c             	mov    DWORD PTR [esi+0xc],eax
  404aa5:	76 44                	jbe    0x404aeb
  404aa7:	8b 6c 24 30          	mov    ebp,DWORD PTR [esp+0x30]
  404aab:	8b 44 24 2c          	mov    eax,DWORD PTR [esp+0x2c]
  404aaf:	3b c5                	cmp    eax,ebp
  404ab1:	75 21                	jne    0x404ad4
  404ab3:	8b 4e 04             	mov    ecx,DWORD PTR [esi+0x4]
  404ab6:	8d 44 24 10          	lea    eax,[esp+0x10]
  404aba:	6a 00                	push   0x0
  404abc:	50                   	push   eax
  404abd:	51                   	push   ecx
  404abe:	ff 15 88 b2 43 00    	call   DWORD PTR ds:0x43b288
  404ac4:	85 c0                	test   eax,eax
  404ac6:	75 b8                	jne    0x404a80
  404ac8:	8b 44 24 2c          	mov    eax,DWORD PTR [esp+0x2c]
  404acc:	8b 6c 24 30          	mov    ebp,DWORD PTR [esp+0x30]
  404ad0:	3b c5                	cmp    eax,ebp
  404ad2:	74 ac                	je     0x404a80
  404ad4:	8b 4c 24 5c          	mov    ecx,DWORD PTR [esp+0x5c]
  404ad8:	8a 10                	mov    dl,BYTE PTR [eax]
  404ada:	40                   	inc    eax
  404adb:	88 14 0f             	mov    BYTE PTR [edi+ecx*1],dl
  404ade:	47                   	inc    edi
  404adf:	3b fb                	cmp    edi,ebx
  404ae1:	89 44 24 2c          	mov    DWORD PTR [esp+0x2c],eax
  404ae5:	72 c8                	jb     0x404aaf
  404ae7:	8b 6c 24 64          	mov    ebp,DWORD PTR [esp+0x64]
  404aeb:	8b 46 04             	mov    eax,DWORD PTR [esi+0x4]
  404aee:	8d 54 24 10          	lea    edx,[esp+0x10]
  404af2:	6a 00                	push   0x0
  404af4:	52                   	push   edx
  404af5:	50                   	push   eax
  404af6:	ff 15 84 b2 43 00    	call   DWORD PTR ds:0x43b284
  404afc:	85 c0                	test   eax,eax
  404afe:	74 0f                	je     0x404b0f
  404b00:	5f                   	pop    edi
  404b01:	5e                   	pop    esi
  404b02:	5d                   	pop    ebp
  404b03:	b8 05 40 00 80       	mov    eax,0x80004005
  404b08:	5b                   	pop    ebx
  404b09:	83 c4 48             	add    esp,0x48
  404b0c:	c2 0c 00             	ret    0xc
  404b0f:	5f                   	pop    edi
  404b10:	89 5d 00             	mov    DWORD PTR [ebp+0x0],ebx
  404b13:	5e                   	pop    esi
  404b14:	5d                   	pop    ebp
  404b15:	33 c0                	xor    eax,eax
  404b17:	5b                   	pop    ebx
  404b18:	83 c4 48             	add    esp,0x48
  404b1b:	c2 0c 00             	ret    0xc
  404b1e:	5f                   	pop    edi
  404b1f:	5e                   	pop    esi
  404b20:	5d                   	pop    ebp
  404b21:	b8 57 00 07 80       	mov    eax,0x80070057
  404b26:	5b                   	pop    ebx
  404b27:	83 c4 48             	add    esp,0x48
  404b2a:	c2 0c 00             	ret    0xc
