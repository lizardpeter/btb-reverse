
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00430a52 <.text+0x2fa52>:
  430a52:	55                   	push   ebp
  430a53:	8b ec                	mov    ebp,esp
  430a55:	51                   	push   ecx
  430a56:	53                   	push   ebx
  430a57:	56                   	push   esi
  430a58:	57                   	push   edi
  430a59:	8b 7d 0c             	mov    edi,DWORD PTR [ebp+0xc]
  430a5c:	0f af 7d 10          	imul   edi,DWORD PTR [ebp+0x10]
  430a60:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  430a63:	89 7d fc             	mov    DWORD PTR [ebp-0x4],edi
  430a66:	85 ff                	test   edi,edi
  430a68:	89 45 08             	mov    DWORD PTR [ebp+0x8],eax
  430a6b:	8b df                	mov    ebx,edi
  430a6d:	75 07                	jne    0x430a76
  430a6f:	33 c0                	xor    eax,eax
  430a71:	e9 cd 00 00 00       	jmp    0x430b43
  430a76:	8b 75 14             	mov    esi,DWORD PTR [ebp+0x14]
  430a79:	66 f7 46 0c 0c 01    	test   WORD PTR [esi+0xc],0x10c
  430a7f:	74 08                	je     0x430a89
  430a81:	8b 46 18             	mov    eax,DWORD PTR [esi+0x18]
  430a84:	89 45 14             	mov    DWORD PTR [ebp+0x14],eax
  430a87:	eb 07                	jmp    0x430a90
  430a89:	c7 45 14 00 10 00 00 	mov    DWORD PTR [ebp+0x14],0x1000
  430a90:	8b 4e 0c             	mov    ecx,DWORD PTR [esi+0xc]
  430a93:	81 e1 08 01 00 00    	and    ecx,0x108
  430a99:	74 29                	je     0x430ac4
  430a9b:	8b 46 04             	mov    eax,DWORD PTR [esi+0x4]
  430a9e:	85 c0                	test   eax,eax
  430aa0:	74 22                	je     0x430ac4
  430aa2:	3b d8                	cmp    ebx,eax
  430aa4:	8b fb                	mov    edi,ebx
  430aa6:	72 02                	jb     0x430aaa
  430aa8:	8b f8                	mov    edi,eax
  430aaa:	57                   	push   edi
  430aab:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  430aae:	ff 36                	push   DWORD PTR [esi]
  430ab0:	e8 ab 30 00 00       	call   0x433b60
  430ab5:	29 7e 04             	sub    DWORD PTR [esi+0x4],edi
  430ab8:	01 3e                	add    DWORD PTR [esi],edi
  430aba:	83 c4 0c             	add    esp,0xc
  430abd:	2b df                	sub    ebx,edi
  430abf:	01 7d 08             	add    DWORD PTR [ebp+0x8],edi
  430ac2:	eb 46                	jmp    0x430b0a
  430ac4:	3b 5d 14             	cmp    ebx,DWORD PTR [ebp+0x14]
  430ac7:	72 46                	jb     0x430b0f
  430ac9:	85 c9                	test   ecx,ecx
  430acb:	74 0b                	je     0x430ad8
  430acd:	56                   	push   esi
  430ace:	e8 6f 07 00 00       	call   0x431242
  430ad3:	85 c0                	test   eax,eax
  430ad5:	59                   	pop    ecx
  430ad6:	75 79                	jne    0x430b51
  430ad8:	83 7d 14 00          	cmp    DWORD PTR [ebp+0x14],0x0
  430adc:	74 0d                	je     0x430aeb
  430ade:	8b c3                	mov    eax,ebx
  430ae0:	33 d2                	xor    edx,edx
  430ae2:	f7 75 14             	div    DWORD PTR [ebp+0x14]
  430ae5:	8b fb                	mov    edi,ebx
  430ae7:	2b fa                	sub    edi,edx
  430ae9:	eb 02                	jmp    0x430aed
  430aeb:	8b fb                	mov    edi,ebx
  430aed:	57                   	push   edi
  430aee:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  430af1:	ff 76 10             	push   DWORD PTR [esi+0x10]
  430af4:	e8 84 35 00 00       	call   0x43407d
  430af9:	83 c4 0c             	add    esp,0xc
  430afc:	83 f8 ff             	cmp    eax,0xffffffff
  430aff:	74 47                	je     0x430b48
  430b01:	01 45 08             	add    DWORD PTR [ebp+0x8],eax
  430b04:	2b d8                	sub    ebx,eax
  430b06:	3b c7                	cmp    eax,edi
  430b08:	72 3e                	jb     0x430b48
  430b0a:	8b 7d fc             	mov    edi,DWORD PTR [ebp-0x4]
  430b0d:	eb 29                	jmp    0x430b38
  430b0f:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  430b12:	56                   	push   esi
  430b13:	0f be 00             	movsx  eax,BYTE PTR [eax]
  430b16:	50                   	push   eax
  430b17:	e8 40 1d 00 00       	call   0x43285c
  430b1c:	59                   	pop    ecx
  430b1d:	83 f8 ff             	cmp    eax,0xffffffff
  430b20:	59                   	pop    ecx
  430b21:	74 2e                	je     0x430b51
  430b23:	ff 45 08             	inc    DWORD PTR [ebp+0x8]
  430b26:	8b 46 18             	mov    eax,DWORD PTR [esi+0x18]
  430b29:	4b                   	dec    ebx
  430b2a:	89 45 14             	mov    DWORD PTR [ebp+0x14],eax
  430b2d:	85 c0                	test   eax,eax
  430b2f:	7f 07                	jg     0x430b38
  430b31:	c7 45 14 01 00 00 00 	mov    DWORD PTR [ebp+0x14],0x1
  430b38:	85 db                	test   ebx,ebx
  430b3a:	0f 85 50 ff ff ff    	jne    0x430a90
  430b40:	8b 45 10             	mov    eax,DWORD PTR [ebp+0x10]
  430b43:	5f                   	pop    edi
  430b44:	5e                   	pop    esi
  430b45:	5b                   	pop    ebx
  430b46:	c9                   	leave
  430b47:	c3                   	ret
  430b48:	83 4e 0c 20          	or     DWORD PTR [esi+0xc],0x20
  430b4c:	8b 45 fc             	mov    eax,DWORD PTR [ebp-0x4]
  430b4f:	eb 02                	jmp    0x430b53
  430b51:	8b c7                	mov    eax,edi
  430b53:	2b c3                	sub    eax,ebx
  430b55:	33 d2                	xor    edx,edx
  430b57:	f7 75 0c             	div    DWORD PTR [ebp+0xc]
  430b5a:	eb e7                	jmp    0x430b43
