
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00435a9d <.text+0x34a9d>:
  435a9d:	55                   	push   ebp
  435a9e:	8b ec                	mov    ebp,esp
  435aa0:	8b 4d 18             	mov    ecx,DWORD PTR [ebp+0x18]
  435aa3:	8b 45 14             	mov    eax,DWORD PTR [ebp+0x14]
  435aa6:	53                   	push   ebx
  435aa7:	56                   	push   esi
  435aa8:	83 21 00             	and    DWORD PTR [ecx],0x0
  435aab:	8b 75 10             	mov    esi,DWORD PTR [ebp+0x10]
  435aae:	57                   	push   edi
  435aaf:	8b 7d 0c             	mov    edi,DWORD PTR [ebp+0xc]
  435ab2:	c7 00 01 00 00 00    	mov    DWORD PTR [eax],0x1
  435ab8:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  435abb:	85 ff                	test   edi,edi
  435abd:	74 08                	je     0x435ac7
  435abf:	89 37                	mov    DWORD PTR [edi],esi
  435ac1:	83 c7 04             	add    edi,0x4
  435ac4:	89 7d 0c             	mov    DWORD PTR [ebp+0xc],edi
  435ac7:	80 38 22             	cmp    BYTE PTR [eax],0x22
  435aca:	75 44                	jne    0x435b10
  435acc:	8a 50 01             	mov    dl,BYTE PTR [eax+0x1]
  435acf:	40                   	inc    eax
  435ad0:	80 fa 22             	cmp    dl,0x22
  435ad3:	74 29                	je     0x435afe
  435ad5:	84 d2                	test   dl,dl
  435ad7:	74 25                	je     0x435afe
  435ad9:	0f b6 d2             	movzx  edx,dl
  435adc:	f6 82 e1 c7 51 00 04 	test   BYTE PTR [edx+0x51c7e1],0x4
  435ae3:	74 0c                	je     0x435af1
  435ae5:	ff 01                	inc    DWORD PTR [ecx]
  435ae7:	85 f6                	test   esi,esi
  435ae9:	74 06                	je     0x435af1
  435aeb:	8a 10                	mov    dl,BYTE PTR [eax]
  435aed:	88 16                	mov    BYTE PTR [esi],dl
  435aef:	46                   	inc    esi
  435af0:	40                   	inc    eax
  435af1:	ff 01                	inc    DWORD PTR [ecx]
  435af3:	85 f6                	test   esi,esi
  435af5:	74 d5                	je     0x435acc
  435af7:	8a 10                	mov    dl,BYTE PTR [eax]
  435af9:	88 16                	mov    BYTE PTR [esi],dl
  435afb:	46                   	inc    esi
  435afc:	eb ce                	jmp    0x435acc
  435afe:	ff 01                	inc    DWORD PTR [ecx]
  435b00:	85 f6                	test   esi,esi
  435b02:	74 04                	je     0x435b08
  435b04:	80 26 00             	and    BYTE PTR [esi],0x0
  435b07:	46                   	inc    esi
  435b08:	80 38 22             	cmp    BYTE PTR [eax],0x22
  435b0b:	75 46                	jne    0x435b53
  435b0d:	40                   	inc    eax
  435b0e:	eb 43                	jmp    0x435b53
  435b10:	ff 01                	inc    DWORD PTR [ecx]
  435b12:	85 f6                	test   esi,esi
  435b14:	74 05                	je     0x435b1b
  435b16:	8a 10                	mov    dl,BYTE PTR [eax]
  435b18:	88 16                	mov    BYTE PTR [esi],dl
  435b1a:	46                   	inc    esi
  435b1b:	8a 10                	mov    dl,BYTE PTR [eax]
  435b1d:	40                   	inc    eax
  435b1e:	0f b6 da             	movzx  ebx,dl
  435b21:	f6 83 e1 c7 51 00 04 	test   BYTE PTR [ebx+0x51c7e1],0x4
  435b28:	74 0c                	je     0x435b36
  435b2a:	ff 01                	inc    DWORD PTR [ecx]
  435b2c:	85 f6                	test   esi,esi
  435b2e:	74 05                	je     0x435b35
  435b30:	8a 18                	mov    bl,BYTE PTR [eax]
  435b32:	88 1e                	mov    BYTE PTR [esi],bl
  435b34:	46                   	inc    esi
  435b35:	40                   	inc    eax
  435b36:	80 fa 20             	cmp    dl,0x20
  435b39:	74 09                	je     0x435b44
  435b3b:	84 d2                	test   dl,dl
  435b3d:	74 09                	je     0x435b48
  435b3f:	80 fa 09             	cmp    dl,0x9
  435b42:	75 cc                	jne    0x435b10
  435b44:	84 d2                	test   dl,dl
  435b46:	75 03                	jne    0x435b4b
  435b48:	48                   	dec    eax
  435b49:	eb 08                	jmp    0x435b53
  435b4b:	85 f6                	test   esi,esi
  435b4d:	74 04                	je     0x435b53
  435b4f:	80 66 ff 00          	and    BYTE PTR [esi-0x1],0x0
  435b53:	83 65 18 00          	and    DWORD PTR [ebp+0x18],0x0
  435b57:	80 38 00             	cmp    BYTE PTR [eax],0x0
  435b5a:	0f 84 e0 00 00 00    	je     0x435c40
  435b60:	8a 10                	mov    dl,BYTE PTR [eax]
  435b62:	80 fa 20             	cmp    dl,0x20
  435b65:	74 05                	je     0x435b6c
  435b67:	80 fa 09             	cmp    dl,0x9
  435b6a:	75 03                	jne    0x435b6f
  435b6c:	40                   	inc    eax
  435b6d:	eb f1                	jmp    0x435b60
  435b6f:	80 38 00             	cmp    BYTE PTR [eax],0x0
  435b72:	0f 84 c8 00 00 00    	je     0x435c40
  435b78:	85 ff                	test   edi,edi
  435b7a:	74 08                	je     0x435b84
  435b7c:	89 37                	mov    DWORD PTR [edi],esi
  435b7e:	83 c7 04             	add    edi,0x4
  435b81:	89 7d 0c             	mov    DWORD PTR [ebp+0xc],edi
  435b84:	8b 55 14             	mov    edx,DWORD PTR [ebp+0x14]
  435b87:	ff 02                	inc    DWORD PTR [edx]
  435b89:	c7 45 08 01 00 00 00 	mov    DWORD PTR [ebp+0x8],0x1
  435b90:	33 db                	xor    ebx,ebx
  435b92:	80 38 5c             	cmp    BYTE PTR [eax],0x5c
  435b95:	75 04                	jne    0x435b9b
  435b97:	40                   	inc    eax
  435b98:	43                   	inc    ebx
  435b99:	eb f7                	jmp    0x435b92
  435b9b:	80 38 22             	cmp    BYTE PTR [eax],0x22
  435b9e:	75 2c                	jne    0x435bcc
  435ba0:	f6 c3 01             	test   bl,0x1
  435ba3:	75 25                	jne    0x435bca
  435ba5:	33 ff                	xor    edi,edi
  435ba7:	39 7d 18             	cmp    DWORD PTR [ebp+0x18],edi
  435baa:	74 0d                	je     0x435bb9
  435bac:	80 78 01 22          	cmp    BYTE PTR [eax+0x1],0x22
  435bb0:	8d 50 01             	lea    edx,[eax+0x1]
  435bb3:	75 04                	jne    0x435bb9
  435bb5:	8b c2                	mov    eax,edx
  435bb7:	eb 03                	jmp    0x435bbc
  435bb9:	89 7d 08             	mov    DWORD PTR [ebp+0x8],edi
  435bbc:	8b 7d 0c             	mov    edi,DWORD PTR [ebp+0xc]
  435bbf:	33 d2                	xor    edx,edx
  435bc1:	39 55 18             	cmp    DWORD PTR [ebp+0x18],edx
  435bc4:	0f 94 c2             	sete   dl
  435bc7:	89 55 18             	mov    DWORD PTR [ebp+0x18],edx
  435bca:	d1 eb                	shr    ebx,1
  435bcc:	8b d3                	mov    edx,ebx
  435bce:	4b                   	dec    ebx
  435bcf:	85 d2                	test   edx,edx
  435bd1:	74 0e                	je     0x435be1
  435bd3:	43                   	inc    ebx
  435bd4:	85 f6                	test   esi,esi
  435bd6:	74 04                	je     0x435bdc
  435bd8:	c6 06 5c             	mov    BYTE PTR [esi],0x5c
  435bdb:	46                   	inc    esi
  435bdc:	ff 01                	inc    DWORD PTR [ecx]
  435bde:	4b                   	dec    ebx
  435bdf:	75 f3                	jne    0x435bd4
  435be1:	8a 10                	mov    dl,BYTE PTR [eax]
  435be3:	84 d2                	test   dl,dl
  435be5:	74 4a                	je     0x435c31
  435be7:	83 7d 18 00          	cmp    DWORD PTR [ebp+0x18],0x0
  435beb:	75 0a                	jne    0x435bf7
  435bed:	80 fa 20             	cmp    dl,0x20
  435bf0:	74 3f                	je     0x435c31
  435bf2:	80 fa 09             	cmp    dl,0x9
  435bf5:	74 3a                	je     0x435c31
  435bf7:	83 7d 08 00          	cmp    DWORD PTR [ebp+0x8],0x0
  435bfb:	74 2e                	je     0x435c2b
  435bfd:	85 f6                	test   esi,esi
  435bff:	74 19                	je     0x435c1a
  435c01:	0f b6 da             	movzx  ebx,dl
  435c04:	f6 83 e1 c7 51 00 04 	test   BYTE PTR [ebx+0x51c7e1],0x4
  435c0b:	74 06                	je     0x435c13
  435c0d:	88 16                	mov    BYTE PTR [esi],dl
  435c0f:	46                   	inc    esi
  435c10:	40                   	inc    eax
  435c11:	ff 01                	inc    DWORD PTR [ecx]
  435c13:	8a 10                	mov    dl,BYTE PTR [eax]
  435c15:	88 16                	mov    BYTE PTR [esi],dl
  435c17:	46                   	inc    esi
  435c18:	eb 0f                	jmp    0x435c29
  435c1a:	0f b6 d2             	movzx  edx,dl
  435c1d:	f6 82 e1 c7 51 00 04 	test   BYTE PTR [edx+0x51c7e1],0x4
  435c24:	74 03                	je     0x435c29
  435c26:	40                   	inc    eax
  435c27:	ff 01                	inc    DWORD PTR [ecx]
  435c29:	ff 01                	inc    DWORD PTR [ecx]
  435c2b:	40                   	inc    eax
  435c2c:	e9 58 ff ff ff       	jmp    0x435b89
  435c31:	85 f6                	test   esi,esi
  435c33:	74 04                	je     0x435c39
  435c35:	80 26 00             	and    BYTE PTR [esi],0x0
  435c38:	46                   	inc    esi
  435c39:	ff 01                	inc    DWORD PTR [ecx]
  435c3b:	e9 17 ff ff ff       	jmp    0x435b57
  435c40:	85 ff                	test   edi,edi
  435c42:	74 03                	je     0x435c47
  435c44:	83 27 00             	and    DWORD PTR [edi],0x0
  435c47:	8b 45 14             	mov    eax,DWORD PTR [ebp+0x14]
  435c4a:	5f                   	pop    edi
  435c4b:	5e                   	pop    esi
  435c4c:	5b                   	pop    ebx
  435c4d:	ff 00                	inc    DWORD PTR [eax]
  435c4f:	5d                   	pop    ebp
  435c50:	c3                   	ret
