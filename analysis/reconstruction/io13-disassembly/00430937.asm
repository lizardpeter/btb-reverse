
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00430937 <.text+0x2f937>:
  430937:	55                   	push   ebp
  430938:	8b ec                	mov    ebp,esp
  43093a:	51                   	push   ecx
  43093b:	53                   	push   ebx
  43093c:	56                   	push   esi
  43093d:	57                   	push   edi
  43093e:	8b 7d 0c             	mov    edi,DWORD PTR [ebp+0xc]
  430941:	0f af 7d 10          	imul   edi,DWORD PTR [ebp+0x10]
  430945:	8b 5d 08             	mov    ebx,DWORD PTR [ebp+0x8]
  430948:	8b cf                	mov    ecx,edi
  43094a:	85 ff                	test   edi,edi
  43094c:	89 7d fc             	mov    DWORD PTR [ebp-0x4],edi
  43094f:	89 4d 08             	mov    DWORD PTR [ebp+0x8],ecx
  430952:	75 07                	jne    0x43095b
  430954:	33 c0                	xor    eax,eax
  430956:	e9 a9 00 00 00       	jmp    0x430a04
  43095b:	8b 75 14             	mov    esi,DWORD PTR [ebp+0x14]
  43095e:	66 f7 46 0c 0c 01    	test   WORD PTR [esi+0xc],0x10c
  430964:	74 08                	je     0x43096e
  430966:	8b 46 18             	mov    eax,DWORD PTR [esi+0x18]
  430969:	89 45 14             	mov    DWORD PTR [ebp+0x14],eax
  43096c:	eb 0c                	jmp    0x43097a
  43096e:	c7 45 14 00 10 00 00 	mov    DWORD PTR [ebp+0x14],0x1000
  430975:	eb 03                	jmp    0x43097a
  430977:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  43097a:	66 f7 46 0c 0c 01    	test   WORD PTR [esi+0xc],0x10c
  430980:	74 2a                	je     0x4309ac
  430982:	8b 46 04             	mov    eax,DWORD PTR [esi+0x4]
  430985:	85 c0                	test   eax,eax
  430987:	74 23                	je     0x4309ac
  430989:	3b c8                	cmp    ecx,eax
  43098b:	8b f9                	mov    edi,ecx
  43098d:	72 02                	jb     0x430991
  43098f:	8b f8                	mov    edi,eax
  430991:	57                   	push   edi
  430992:	ff 36                	push   DWORD PTR [esi]
  430994:	53                   	push   ebx
  430995:	e8 c6 31 00 00       	call   0x433b60
  43099a:	29 7d 08             	sub    DWORD PTR [ebp+0x8],edi
  43099d:	29 7e 04             	sub    DWORD PTR [esi+0x4],edi
  4309a0:	01 3e                	add    DWORD PTR [esi],edi
  4309a2:	83 c4 0c             	add    esp,0xc
  4309a5:	03 df                	add    ebx,edi
  4309a7:	8b 7d fc             	mov    edi,DWORD PTR [ebp-0x4]
  4309aa:	eb 4b                	jmp    0x4309f7
  4309ac:	3b 4d 14             	cmp    ecx,DWORD PTR [ebp+0x14]
  4309af:	72 2e                	jb     0x4309df
  4309b1:	83 7d 14 00          	cmp    DWORD PTR [ebp+0x14],0x0
  4309b5:	8b c1                	mov    eax,ecx
  4309b7:	74 09                	je     0x4309c2
  4309b9:	33 d2                	xor    edx,edx
  4309bb:	f7 75 14             	div    DWORD PTR [ebp+0x14]
  4309be:	8b c1                	mov    eax,ecx
  4309c0:	2b c2                	sub    eax,edx
  4309c2:	50                   	push   eax
  4309c3:	53                   	push   ebx
  4309c4:	ff 76 10             	push   DWORD PTR [esi+0x10]
  4309c7:	e8 95 2f 00 00       	call   0x433961
  4309cc:	83 c4 0c             	add    esp,0xc
  4309cf:	85 c0                	test   eax,eax
  4309d1:	74 36                	je     0x430a09
  4309d3:	83 f8 ff             	cmp    eax,0xffffffff
  4309d6:	74 37                	je     0x430a0f
  4309d8:	29 45 08             	sub    DWORD PTR [ebp+0x8],eax
  4309db:	03 d8                	add    ebx,eax
  4309dd:	eb 18                	jmp    0x4309f7
  4309df:	56                   	push   esi
  4309e0:	e8 a3 2e 00 00       	call   0x433888
  4309e5:	83 f8 ff             	cmp    eax,0xffffffff
  4309e8:	59                   	pop    ecx
  4309e9:	74 28                	je     0x430a13
  4309eb:	88 03                	mov    BYTE PTR [ebx],al
  4309ed:	8b 46 18             	mov    eax,DWORD PTR [esi+0x18]
  4309f0:	43                   	inc    ebx
  4309f1:	ff 4d 08             	dec    DWORD PTR [ebp+0x8]
  4309f4:	89 45 14             	mov    DWORD PTR [ebp+0x14],eax
  4309f7:	83 7d 08 00          	cmp    DWORD PTR [ebp+0x8],0x0
  4309fb:	0f 85 76 ff ff ff    	jne    0x430977
  430a01:	8b 45 10             	mov    eax,DWORD PTR [ebp+0x10]
  430a04:	5f                   	pop    edi
  430a05:	5e                   	pop    esi
  430a06:	5b                   	pop    ebx
  430a07:	c9                   	leave
  430a08:	c3                   	ret
  430a09:	83 4e 0c 10          	or     DWORD PTR [esi+0xc],0x10
  430a0d:	eb 04                	jmp    0x430a13
  430a0f:	83 4e 0c 20          	or     DWORD PTR [esi+0xc],0x20
  430a13:	8b c7                	mov    eax,edi
  430a15:	33 d2                	xor    edx,edx
  430a17:	2b 45 08             	sub    eax,DWORD PTR [ebp+0x8]
  430a1a:	f7 75 0c             	div    DWORD PTR [ebp+0xc]
  430a1d:	eb e5                	jmp    0x430a04
