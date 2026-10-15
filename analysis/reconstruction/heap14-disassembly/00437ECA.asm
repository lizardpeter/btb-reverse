
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00437eca <.text+0x36eca>:
  437eca:	55                   	push   ebp
  437ecb:	8b ec                	mov    ebp,esp
  437ecd:	83 ec 0c             	sub    esp,0xc
  437ed0:	53                   	push   ebx
  437ed1:	83 65 f8 00          	and    DWORD PTR [ebp-0x8],0x0
  437ed5:	56                   	push   esi
  437ed6:	57                   	push   edi
  437ed7:	8b 7d 08             	mov    edi,DWORD PTR [ebp+0x8]
  437eda:	8a 1f                	mov    bl,BYTE PTR [edi]
  437edc:	8d 77 01             	lea    esi,[edi+0x1]
  437edf:	89 75 fc             	mov    DWORD PTR [ebp-0x4],esi
  437ee2:	83 3d 10 9b 44 00 01 	cmp    DWORD PTR ds:0x449b10,0x1
  437ee9:	7e 0f                	jle    0x437efa
  437eeb:	0f b6 c3             	movzx  eax,bl
  437eee:	6a 08                	push   0x8
  437ef0:	50                   	push   eax
  437ef1:	e8 ca e6 ff ff       	call   0x4365c0
  437ef6:	59                   	pop    ecx
  437ef7:	59                   	pop    ecx
  437ef8:	eb 0f                	jmp    0x437f09
  437efa:	8b 0d 1c 9b 44 00    	mov    ecx,DWORD PTR ds:0x449b1c
  437f00:	0f b6 c3             	movzx  eax,bl
  437f03:	8a 04 41             	mov    al,BYTE PTR [ecx+eax*2]
  437f06:	83 e0 08             	and    eax,0x8
  437f09:	85 c0                	test   eax,eax
  437f0b:	74 05                	je     0x437f12
  437f0d:	8a 1e                	mov    bl,BYTE PTR [esi]
  437f0f:	46                   	inc    esi
  437f10:	eb d0                	jmp    0x437ee2
  437f12:	80 fb 2d             	cmp    bl,0x2d
  437f15:	89 75 fc             	mov    DWORD PTR [ebp-0x4],esi
  437f18:	75 06                	jne    0x437f20
  437f1a:	83 4d 14 02          	or     DWORD PTR [ebp+0x14],0x2
  437f1e:	eb 05                	jmp    0x437f25
  437f20:	80 fb 2b             	cmp    bl,0x2b
  437f23:	75 06                	jne    0x437f2b
  437f25:	8a 1e                	mov    bl,BYTE PTR [esi]
  437f27:	46                   	inc    esi
  437f28:	89 75 fc             	mov    DWORD PTR [ebp-0x4],esi
  437f2b:	8b 45 10             	mov    eax,DWORD PTR [ebp+0x10]
  437f2e:	85 c0                	test   eax,eax
  437f30:	0f 8c 8c 01 00 00    	jl     0x4380c2
  437f36:	83 f8 01             	cmp    eax,0x1
  437f39:	0f 84 83 01 00 00    	je     0x4380c2
  437f3f:	83 f8 24             	cmp    eax,0x24
  437f42:	0f 8f 7a 01 00 00    	jg     0x4380c2
  437f48:	6a 10                	push   0x10
  437f4a:	85 c0                	test   eax,eax
  437f4c:	59                   	pop    ecx
  437f4d:	75 24                	jne    0x437f73
  437f4f:	80 fb 30             	cmp    bl,0x30
  437f52:	74 09                	je     0x437f5d
  437f54:	c7 45 10 0a 00 00 00 	mov    DWORD PTR [ebp+0x10],0xa
  437f5b:	eb 32                	jmp    0x437f8f
  437f5d:	8a 06                	mov    al,BYTE PTR [esi]
  437f5f:	3c 78                	cmp    al,0x78
  437f61:	74 0d                	je     0x437f70
  437f63:	3c 58                	cmp    al,0x58
  437f65:	74 09                	je     0x437f70
  437f67:	c7 45 10 08 00 00 00 	mov    DWORD PTR [ebp+0x10],0x8
  437f6e:	eb 1f                	jmp    0x437f8f
  437f70:	89 4d 10             	mov    DWORD PTR [ebp+0x10],ecx
  437f73:	39 4d 10             	cmp    DWORD PTR [ebp+0x10],ecx
  437f76:	75 17                	jne    0x437f8f
  437f78:	80 fb 30             	cmp    bl,0x30
  437f7b:	75 12                	jne    0x437f8f
  437f7d:	8a 06                	mov    al,BYTE PTR [esi]
  437f7f:	3c 78                	cmp    al,0x78
  437f81:	74 04                	je     0x437f87
  437f83:	3c 58                	cmp    al,0x58
  437f85:	75 08                	jne    0x437f8f
  437f87:	8a 5e 01             	mov    bl,BYTE PTR [esi+0x1]
  437f8a:	46                   	inc    esi
  437f8b:	46                   	inc    esi
  437f8c:	89 75 fc             	mov    DWORD PTR [ebp-0x4],esi
  437f8f:	83 c8 ff             	or     eax,0xffffffff
  437f92:	33 d2                	xor    edx,edx
  437f94:	f7 75 10             	div    DWORD PTR [ebp+0x10]
  437f97:	bf 03 01 00 00       	mov    edi,0x103
  437f9c:	89 45 f4             	mov    DWORD PTR [ebp-0xc],eax
  437f9f:	83 3d 10 9b 44 00 01 	cmp    DWORD PTR ds:0x449b10,0x1
  437fa6:	0f b6 f3             	movzx  esi,bl
  437fa9:	7e 0c                	jle    0x437fb7
  437fab:	6a 04                	push   0x4
  437fad:	56                   	push   esi
  437fae:	e8 0d e6 ff ff       	call   0x4365c0
  437fb3:	59                   	pop    ecx
  437fb4:	59                   	pop    ecx
  437fb5:	eb 0b                	jmp    0x437fc2
  437fb7:	a1 1c 9b 44 00       	mov    eax,ds:0x449b1c
  437fbc:	8a 04 70             	mov    al,BYTE PTR [eax+esi*2]
  437fbf:	83 e0 04             	and    eax,0x4
  437fc2:	85 c0                	test   eax,eax
  437fc4:	74 08                	je     0x437fce
  437fc6:	0f be cb             	movsx  ecx,bl
  437fc9:	83 e9 30             	sub    ecx,0x30
  437fcc:	eb 32                	jmp    0x438000
  437fce:	83 3d 10 9b 44 00 01 	cmp    DWORD PTR ds:0x449b10,0x1
  437fd5:	7e 0b                	jle    0x437fe2
  437fd7:	57                   	push   edi
  437fd8:	56                   	push   esi
  437fd9:	e8 e2 e5 ff ff       	call   0x4365c0
  437fde:	59                   	pop    ecx
  437fdf:	59                   	pop    ecx
  437fe0:	eb 0b                	jmp    0x437fed
  437fe2:	a1 1c 9b 44 00       	mov    eax,ds:0x449b1c
  437fe7:	66 8b 04 70          	mov    ax,WORD PTR [eax+esi*2]
  437feb:	23 c7                	and    eax,edi
  437fed:	85 c0                	test   eax,eax
  437fef:	74 4a                	je     0x43803b
  437ff1:	0f be c3             	movsx  eax,bl
  437ff4:	50                   	push   eax
  437ff5:	e8 18 12 00 00       	call   0x439212
  437ffa:	59                   	pop    ecx
  437ffb:	8b c8                	mov    ecx,eax
  437ffd:	83 e9 37             	sub    ecx,0x37
  438000:	3b 4d 10             	cmp    ecx,DWORD PTR [ebp+0x10]
  438003:	73 36                	jae    0x43803b
  438005:	8b 75 f8             	mov    esi,DWORD PTR [ebp-0x8]
  438008:	83 4d 14 08          	or     DWORD PTR [ebp+0x14],0x8
  43800c:	3b 75 f4             	cmp    esi,DWORD PTR [ebp-0xc]
  43800f:	72 14                	jb     0x438025
  438011:	75 0c                	jne    0x43801f
  438013:	83 c8 ff             	or     eax,0xffffffff
  438016:	33 d2                	xor    edx,edx
  438018:	f7 75 10             	div    DWORD PTR [ebp+0x10]
  43801b:	3b ca                	cmp    ecx,edx
  43801d:	76 06                	jbe    0x438025
  43801f:	83 4d 14 04          	or     DWORD PTR [ebp+0x14],0x4
  438023:	eb 09                	jmp    0x43802e
  438025:	0f af 75 10          	imul   esi,DWORD PTR [ebp+0x10]
  438029:	03 f1                	add    esi,ecx
  43802b:	89 75 f8             	mov    DWORD PTR [ebp-0x8],esi
  43802e:	8b 45 fc             	mov    eax,DWORD PTR [ebp-0x4]
  438031:	ff 45 fc             	inc    DWORD PTR [ebp-0x4]
  438034:	8a 18                	mov    bl,BYTE PTR [eax]
  438036:	e9 64 ff ff ff       	jmp    0x437f9f
  43803b:	8b 4d 14             	mov    ecx,DWORD PTR [ebp+0x14]
  43803e:	ff 4d fc             	dec    DWORD PTR [ebp-0x4]
  438041:	8b 55 0c             	mov    edx,DWORD PTR [ebp+0xc]
  438044:	f6 c1 08             	test   cl,0x8
  438047:	75 10                	jne    0x438059
  438049:	85 d2                	test   edx,edx
  43804b:	74 06                	je     0x438053
  43804d:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  438050:	89 45 fc             	mov    DWORD PTR [ebp-0x4],eax
  438053:	83 65 f8 00          	and    DWORD PTR [ebp-0x8],0x0
  438057:	eb 4d                	jmp    0x4380a6
  438059:	f6 c1 04             	test   cl,0x4
  43805c:	b8 ff ff ff 7f       	mov    eax,0x7fffffff
  438061:	75 1c                	jne    0x43807f
  438063:	f6 c1 01             	test   cl,0x1
  438066:	75 3e                	jne    0x4380a6
  438068:	83 e1 02             	and    ecx,0x2
  43806b:	74 09                	je     0x438076
  43806d:	81 7d f8 00 00 00 80 	cmp    DWORD PTR [ebp-0x8],0x80000000
  438074:	77 09                	ja     0x43807f
  438076:	85 c9                	test   ecx,ecx
  438078:	75 2c                	jne    0x4380a6
  43807a:	39 45 f8             	cmp    DWORD PTR [ebp-0x8],eax
  43807d:	76 27                	jbe    0x4380a6
  43807f:	f6 45 14 01          	test   BYTE PTR [ebp+0x14],0x1
  438083:	c7 05 c8 c3 51 00 22 	mov    DWORD PTR ds:0x51c3c8,0x22
  43808a:	00 00 00 
  43808d:	74 06                	je     0x438095
  43808f:	83 4d f8 ff          	or     DWORD PTR [ebp-0x8],0xffffffff
  438093:	eb 11                	jmp    0x4380a6
  438095:	8b 4d 14             	mov    ecx,DWORD PTR [ebp+0x14]
  438098:	80 e1 02             	and    cl,0x2
  43809b:	f6 d9                	neg    cl
  43809d:	1b c9                	sbb    ecx,ecx
  43809f:	f7 d9                	neg    ecx
  4380a1:	03 c8                	add    ecx,eax
  4380a3:	89 4d f8             	mov    DWORD PTR [ebp-0x8],ecx
  4380a6:	85 d2                	test   edx,edx
  4380a8:	74 05                	je     0x4380af
  4380aa:	8b 45 fc             	mov    eax,DWORD PTR [ebp-0x4]
  4380ad:	89 02                	mov    DWORD PTR [edx],eax
  4380af:	f6 45 14 02          	test   BYTE PTR [ebp+0x14],0x2
  4380b3:	74 08                	je     0x4380bd
  4380b5:	8b 45 f8             	mov    eax,DWORD PTR [ebp-0x8]
  4380b8:	f7 d8                	neg    eax
  4380ba:	89 45 f8             	mov    DWORD PTR [ebp-0x8],eax
  4380bd:	8b 45 f8             	mov    eax,DWORD PTR [ebp-0x8]
  4380c0:	eb 0b                	jmp    0x4380cd
  4380c2:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  4380c5:	85 c0                	test   eax,eax
  4380c7:	74 02                	je     0x4380cb
  4380c9:	89 38                	mov    DWORD PTR [eax],edi
  4380cb:	33 c0                	xor    eax,eax
  4380cd:	5f                   	pop    edi
  4380ce:	5e                   	pop    esi
  4380cf:	5b                   	pop    ebx
  4380d0:	c9                   	leave
  4380d1:	c3                   	ret
