
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00433e95 <.text+0x32e95>:
  433e95:	55                   	push   ebp
  433e96:	8b ec                	mov    ebp,esp
  433e98:	51                   	push   ecx
  433e99:	51                   	push   ecx
  433e9a:	53                   	push   ebx
  433e9b:	56                   	push   esi
  433e9c:	8b 35 68 c6 51 00    	mov    esi,DWORD PTR ds:0x51c668
  433ea2:	57                   	push   edi
  433ea3:	8b 7d 0c             	mov    edi,DWORD PTR [ebp+0xc]
  433ea6:	33 db                	xor    ebx,ebx
  433ea8:	89 5d f8             	mov    DWORD PTR [ebp-0x8],ebx
  433eab:	89 5d fc             	mov    DWORD PTR [ebp-0x4],ebx
  433eae:	8a 07                	mov    al,BYTE PTR [edi]
  433eb0:	3c 61                	cmp    al,0x61
  433eb2:	74 1a                	je     0x433ece
  433eb4:	3c 72                	cmp    al,0x72
  433eb6:	74 0f                	je     0x433ec7
  433eb8:	3c 77                	cmp    al,0x77
  433eba:	0f 85 22 01 00 00    	jne    0x433fe2
  433ec0:	b9 01 03 00 00       	mov    ecx,0x301
  433ec5:	eb 0c                	jmp    0x433ed3
  433ec7:	33 c9                	xor    ecx,ecx
  433ec9:	83 ce 01             	or     esi,0x1
  433ecc:	eb 08                	jmp    0x433ed6
  433ece:	b9 09 01 00 00       	mov    ecx,0x109
  433ed3:	83 ce 02             	or     esi,0x2
  433ed6:	6a 01                	push   0x1
  433ed8:	5a                   	pop    edx
  433ed9:	8a 47 01             	mov    al,BYTE PTR [edi+0x1]
  433edc:	47                   	inc    edi
  433edd:	3a c3                	cmp    al,bl
  433edf:	0f 84 e3 00 00 00    	je     0x433fc8
  433ee5:	3b d3                	cmp    edx,ebx
  433ee7:	0f 84 db 00 00 00    	je     0x433fc8
  433eed:	0f be c0             	movsx  eax,al
  433ef0:	83 f8 54             	cmp    eax,0x54
  433ef3:	7f 72                	jg     0x433f67
  433ef5:	74 60                	je     0x433f57
  433ef7:	83 e8 2b             	sub    eax,0x2b
  433efa:	74 45                	je     0x433f41
  433efc:	83 e8 19             	sub    eax,0x19
  433eff:	74 36                	je     0x433f37
  433f01:	83 e8 0e             	sub    eax,0xe
  433f04:	74 1c                	je     0x433f22
  433f06:	48                   	dec    eax
  433f07:	0f 85 ac 00 00 00    	jne    0x433fb9
  433f0d:	39 5d fc             	cmp    DWORD PTR [ebp-0x4],ebx
  433f10:	0f 85 a3 00 00 00    	jne    0x433fb9
  433f16:	c7 45 fc 01 00 00 00 	mov    DWORD PTR [ebp-0x4],0x1
  433f1d:	83 c9 20             	or     ecx,0x20
  433f20:	eb b7                	jmp    0x433ed9
  433f22:	39 5d fc             	cmp    DWORD PTR [ebp-0x4],ebx
  433f25:	0f 85 8e 00 00 00    	jne    0x433fb9
  433f2b:	c7 45 fc 01 00 00 00 	mov    DWORD PTR [ebp-0x4],0x1
  433f32:	83 c9 10             	or     ecx,0x10
  433f35:	eb a2                	jmp    0x433ed9
  433f37:	f6 c1 40             	test   cl,0x40
  433f3a:	75 7d                	jne    0x433fb9
  433f3c:	83 c9 40             	or     ecx,0x40
  433f3f:	eb 98                	jmp    0x433ed9
  433f41:	f6 c1 02             	test   cl,0x2
  433f44:	75 73                	jne    0x433fb9
  433f46:	83 e1 fe             	and    ecx,0xfffffffe
  433f49:	83 e6 fc             	and    esi,0xfffffffc
  433f4c:	83 c9 02             	or     ecx,0x2
  433f4f:	81 ce 80 00 00 00    	or     esi,0x80
  433f55:	eb 82                	jmp    0x433ed9
  433f57:	b8 00 10 00 00       	mov    eax,0x1000
  433f5c:	85 c8                	test   eax,ecx
  433f5e:	75 59                	jne    0x433fb9
  433f60:	0b c8                	or     ecx,eax
  433f62:	e9 72 ff ff ff       	jmp    0x433ed9
  433f67:	83 e8 62             	sub    eax,0x62
  433f6a:	74 48                	je     0x433fb4
  433f6c:	48                   	dec    eax
  433f6d:	74 2e                	je     0x433f9d
  433f6f:	83 e8 0b             	sub    eax,0xb
  433f72:	74 12                	je     0x433f86
  433f74:	83 e8 06             	sub    eax,0x6
  433f77:	75 40                	jne    0x433fb9
  433f79:	f6 c5 c0             	test   ch,0xc0
  433f7c:	75 3b                	jne    0x433fb9
  433f7e:	80 cd 40             	or     ch,0x40
  433f81:	e9 53 ff ff ff       	jmp    0x433ed9
  433f86:	39 5d f8             	cmp    DWORD PTR [ebp-0x8],ebx
  433f89:	75 2e                	jne    0x433fb9
  433f8b:	c7 45 f8 01 00 00 00 	mov    DWORD PTR [ebp-0x8],0x1
  433f92:	81 e6 ff bf ff ff    	and    esi,0xffffbfff
  433f98:	e9 3c ff ff ff       	jmp    0x433ed9
  433f9d:	39 5d f8             	cmp    DWORD PTR [ebp-0x8],ebx
  433fa0:	75 17                	jne    0x433fb9
  433fa2:	c7 45 f8 01 00 00 00 	mov    DWORD PTR [ebp-0x8],0x1
  433fa9:	81 ce 00 40 00 00    	or     esi,0x4000
  433faf:	e9 25 ff ff ff       	jmp    0x433ed9
  433fb4:	f6 c5 c0             	test   ch,0xc0
  433fb7:	74 07                	je     0x433fc0
  433fb9:	33 d2                	xor    edx,edx
  433fbb:	e9 19 ff ff ff       	jmp    0x433ed9
  433fc0:	80 cd 80             	or     ch,0x80
  433fc3:	e9 11 ff ff ff       	jmp    0x433ed9
  433fc8:	68 a4 01 00 00       	push   0x1a4
  433fcd:	ff 75 10             	push   DWORD PTR [ebp+0x10]
  433fd0:	51                   	push   ecx
  433fd1:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  433fd4:	e8 21 3c 00 00       	call   0x437bfa
  433fd9:	8b c8                	mov    ecx,eax
  433fdb:	83 c4 10             	add    esp,0x10
  433fde:	3b cb                	cmp    ecx,ebx
  433fe0:	7d 04                	jge    0x433fe6
  433fe2:	33 c0                	xor    eax,eax
  433fe4:	eb 1a                	jmp    0x434000
  433fe6:	8b 45 14             	mov    eax,DWORD PTR [ebp+0x14]
  433fe9:	ff 05 68 c5 51 00    	inc    DWORD PTR ds:0x51c568
  433fef:	89 70 0c             	mov    DWORD PTR [eax+0xc],esi
  433ff2:	89 58 04             	mov    DWORD PTR [eax+0x4],ebx
  433ff5:	89 18                	mov    DWORD PTR [eax],ebx
  433ff7:	89 58 08             	mov    DWORD PTR [eax+0x8],ebx
  433ffa:	89 58 1c             	mov    DWORD PTR [eax+0x1c],ebx
  433ffd:	89 48 10             	mov    DWORD PTR [eax+0x10],ecx
  434000:	5f                   	pop    edi
  434001:	5e                   	pop    esi
  434002:	5b                   	pop    ebx
  434003:	c9                   	leave
  434004:	c3                   	ret
