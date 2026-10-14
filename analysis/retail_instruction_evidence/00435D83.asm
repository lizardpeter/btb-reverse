
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00435d83 <.text+0x34d83>:
  435d83:	83 ec 44             	sub    esp,0x44
  435d86:	53                   	push   ebx
  435d87:	55                   	push   ebp
  435d88:	56                   	push   esi
  435d89:	57                   	push   edi
  435d8a:	68 00 01 00 00       	push   0x100
  435d8f:	e8 ff af ff ff       	call   0x430d93
  435d94:	8b f0                	mov    esi,eax
  435d96:	59                   	pop    ecx
  435d97:	85 f6                	test   esi,esi
  435d99:	75 08                	jne    0x435da3
  435d9b:	6a 1b                	push   0x1b
  435d9d:	e8 3e b3 ff ff       	call   0x4310e0
  435da2:	59                   	pop    ecx
  435da3:	89 35 20 d9 51 00    	mov    DWORD PTR ds:0x51d920,esi
  435da9:	c7 05 20 da 51 00 20 	mov    DWORD PTR ds:0x51da20,0x20
  435db0:	00 00 00 
  435db3:	8d 86 00 01 00 00    	lea    eax,[esi+0x100]
  435db9:	3b f0                	cmp    esi,eax
  435dbb:	73 1a                	jae    0x435dd7
  435dbd:	80 66 04 00          	and    BYTE PTR [esi+0x4],0x0
  435dc1:	83 0e ff             	or     DWORD PTR [esi],0xffffffff
  435dc4:	c6 46 05 0a          	mov    BYTE PTR [esi+0x5],0xa
  435dc8:	a1 20 d9 51 00       	mov    eax,ds:0x51d920
  435dcd:	83 c6 08             	add    esi,0x8
  435dd0:	05 00 01 00 00       	add    eax,0x100
  435dd5:	eb e2                	jmp    0x435db9
  435dd7:	8d 44 24 10          	lea    eax,[esp+0x10]
  435ddb:	50                   	push   eax
  435ddc:	ff 15 04 b1 43 00    	call   DWORD PTR ds:0x43b104
  435de2:	66 83 7c 24 42 00    	cmp    WORD PTR [esp+0x42],0x0
  435de8:	0f 84 c5 00 00 00    	je     0x435eb3
  435dee:	8b 44 24 44          	mov    eax,DWORD PTR [esp+0x44]
  435df2:	85 c0                	test   eax,eax
  435df4:	0f 84 b9 00 00 00    	je     0x435eb3
  435dfa:	8b 30                	mov    esi,DWORD PTR [eax]
  435dfc:	8d 68 04             	lea    ebp,[eax+0x4]
  435dff:	b8 00 08 00 00       	mov    eax,0x800
  435e04:	3b f0                	cmp    esi,eax
  435e06:	8d 1c 2e             	lea    ebx,[esi+ebp*1]
  435e09:	7c 02                	jl     0x435e0d
  435e0b:	8b f0                	mov    esi,eax
  435e0d:	39 35 20 da 51 00    	cmp    DWORD PTR ds:0x51da20,esi
  435e13:	7d 52                	jge    0x435e67
  435e15:	bf 24 d9 51 00       	mov    edi,0x51d924
  435e1a:	68 00 01 00 00       	push   0x100
  435e1f:	e8 6f af ff ff       	call   0x430d93
  435e24:	85 c0                	test   eax,eax
  435e26:	59                   	pop    ecx
  435e27:	74 38                	je     0x435e61
  435e29:	83 05 20 da 51 00 20 	add    DWORD PTR ds:0x51da20,0x20
  435e30:	89 07                	mov    DWORD PTR [edi],eax
  435e32:	8d 88 00 01 00 00    	lea    ecx,[eax+0x100]
  435e38:	3b c1                	cmp    eax,ecx
  435e3a:	73 18                	jae    0x435e54
  435e3c:	80 60 04 00          	and    BYTE PTR [eax+0x4],0x0
  435e40:	83 08 ff             	or     DWORD PTR [eax],0xffffffff
  435e43:	c6 40 05 0a          	mov    BYTE PTR [eax+0x5],0xa
  435e47:	8b 0f                	mov    ecx,DWORD PTR [edi]
  435e49:	83 c0 08             	add    eax,0x8
  435e4c:	81 c1 00 01 00 00    	add    ecx,0x100
  435e52:	eb e4                	jmp    0x435e38
  435e54:	83 c7 04             	add    edi,0x4
  435e57:	39 35 20 da 51 00    	cmp    DWORD PTR ds:0x51da20,esi
  435e5d:	7c bb                	jl     0x435e1a
  435e5f:	eb 06                	jmp    0x435e67
  435e61:	8b 35 20 da 51 00    	mov    esi,DWORD PTR ds:0x51da20
  435e67:	33 ff                	xor    edi,edi
  435e69:	85 f6                	test   esi,esi
  435e6b:	7e 46                	jle    0x435eb3
  435e6d:	8b 03                	mov    eax,DWORD PTR [ebx]
  435e6f:	83 f8 ff             	cmp    eax,0xffffffff
  435e72:	74 36                	je     0x435eaa
  435e74:	8a 4d 00             	mov    cl,BYTE PTR [ebp+0x0]
  435e77:	f6 c1 01             	test   cl,0x1
  435e7a:	74 2e                	je     0x435eaa
  435e7c:	f6 c1 08             	test   cl,0x8
  435e7f:	75 0b                	jne    0x435e8c
  435e81:	50                   	push   eax
  435e82:	ff 15 b0 b0 43 00    	call   DWORD PTR ds:0x43b0b0
  435e88:	85 c0                	test   eax,eax
  435e8a:	74 1e                	je     0x435eaa
  435e8c:	8b c7                	mov    eax,edi
  435e8e:	8b cf                	mov    ecx,edi
  435e90:	c1 f8 05             	sar    eax,0x5
  435e93:	83 e1 1f             	and    ecx,0x1f
  435e96:	8b 04 85 20 d9 51 00 	mov    eax,DWORD PTR [eax*4+0x51d920]
  435e9d:	8d 04 c8             	lea    eax,[eax+ecx*8]
  435ea0:	8b 0b                	mov    ecx,DWORD PTR [ebx]
  435ea2:	89 08                	mov    DWORD PTR [eax],ecx
  435ea4:	8a 4d 00             	mov    cl,BYTE PTR [ebp+0x0]
  435ea7:	88 48 04             	mov    BYTE PTR [eax+0x4],cl
  435eaa:	47                   	inc    edi
  435eab:	45                   	inc    ebp
  435eac:	83 c3 04             	add    ebx,0x4
  435eaf:	3b fe                	cmp    edi,esi
  435eb1:	7c ba                	jl     0x435e6d
  435eb3:	33 db                	xor    ebx,ebx
  435eb5:	a1 20 d9 51 00       	mov    eax,ds:0x51d920
  435eba:	83 3c d8 ff          	cmp    DWORD PTR [eax+ebx*8],0xffffffff
  435ebe:	8d 34 d8             	lea    esi,[eax+ebx*8]
  435ec1:	75 4d                	jne    0x435f10
  435ec3:	85 db                	test   ebx,ebx
  435ec5:	c6 46 04 81          	mov    BYTE PTR [esi+0x4],0x81
  435ec9:	75 05                	jne    0x435ed0
  435ecb:	6a f6                	push   0xfffffff6
  435ecd:	58                   	pop    eax
  435ece:	eb 0a                	jmp    0x435eda
  435ed0:	8b c3                	mov    eax,ebx
  435ed2:	48                   	dec    eax
  435ed3:	f7 d8                	neg    eax
  435ed5:	1b c0                	sbb    eax,eax
  435ed7:	83 c0 f5             	add    eax,0xfffffff5
  435eda:	50                   	push   eax
  435edb:	ff 15 b4 b0 43 00    	call   DWORD PTR ds:0x43b0b4
  435ee1:	8b f8                	mov    edi,eax
  435ee3:	83 ff ff             	cmp    edi,0xffffffff
  435ee6:	74 17                	je     0x435eff
  435ee8:	57                   	push   edi
  435ee9:	ff 15 b0 b0 43 00    	call   DWORD PTR ds:0x43b0b0
  435eef:	85 c0                	test   eax,eax
  435ef1:	74 0c                	je     0x435eff
  435ef3:	25 ff 00 00 00       	and    eax,0xff
  435ef8:	89 3e                	mov    DWORD PTR [esi],edi
  435efa:	83 f8 02             	cmp    eax,0x2
  435efd:	75 06                	jne    0x435f05
  435eff:	80 4e 04 40          	or     BYTE PTR [esi+0x4],0x40
  435f03:	eb 0f                	jmp    0x435f14
  435f05:	83 f8 03             	cmp    eax,0x3
  435f08:	75 0a                	jne    0x435f14
  435f0a:	80 4e 04 08          	or     BYTE PTR [esi+0x4],0x8
  435f0e:	eb 04                	jmp    0x435f14
  435f10:	80 4e 04 80          	or     BYTE PTR [esi+0x4],0x80
  435f14:	43                   	inc    ebx
  435f15:	83 fb 03             	cmp    ebx,0x3
  435f18:	7c 9b                	jl     0x435eb5
  435f1a:	ff 35 20 da 51 00    	push   DWORD PTR ds:0x51da20
  435f20:	ff 15 b8 b0 43 00    	call   DWORD PTR ds:0x43b0b8
  435f26:	5f                   	pop    edi
  435f27:	5e                   	pop    esi
  435f28:	5d                   	pop    ebp
  435f29:	5b                   	pop    ebx
  435f2a:	83 c4 44             	add    esp,0x44
  435f2d:	c3                   	ret
