
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0043594b <.text+0x3494b>:
  43594b:	53                   	push   ebx
  43594c:	33 db                	xor    ebx,ebx
  43594e:	39 1d 50 da 51 00    	cmp    DWORD PTR ds:0x51da50,ebx
  435954:	56                   	push   esi
  435955:	57                   	push   edi
  435956:	75 05                	jne    0x43595d
  435958:	e8 3a 17 00 00       	call   0x437097
  43595d:	8b 35 14 c4 51 00    	mov    esi,DWORD PTR ds:0x51c414
  435963:	33 ff                	xor    edi,edi
  435965:	8a 06                	mov    al,BYTE PTR [esi]
  435967:	3a c3                	cmp    al,bl
  435969:	74 12                	je     0x43597d
  43596b:	3c 3d                	cmp    al,0x3d
  43596d:	74 01                	je     0x435970
  43596f:	47                   	inc    edi
  435970:	56                   	push   esi
  435971:	e8 aa dd ff ff       	call   0x433720
  435976:	59                   	pop    ecx
  435977:	8d 74 06 01          	lea    esi,[esi+eax*1+0x1]
  43597b:	eb e8                	jmp    0x435965
  43597d:	8d 04 bd 04 00 00 00 	lea    eax,[edi*4+0x4]
  435984:	50                   	push   eax
  435985:	e8 09 b4 ff ff       	call   0x430d93
  43598a:	8b f0                	mov    esi,eax
  43598c:	59                   	pop    ecx
  43598d:	3b f3                	cmp    esi,ebx
  43598f:	89 35 f0 c3 51 00    	mov    DWORD PTR ds:0x51c3f0,esi
  435995:	75 08                	jne    0x43599f
  435997:	6a 09                	push   0x9
  435999:	e8 42 b7 ff ff       	call   0x4310e0
  43599e:	59                   	pop    ecx
  43599f:	8b 3d 14 c4 51 00    	mov    edi,DWORD PTR ds:0x51c414
  4359a5:	38 1f                	cmp    BYTE PTR [edi],bl
  4359a7:	74 39                	je     0x4359e2
  4359a9:	55                   	push   ebp
  4359aa:	57                   	push   edi
  4359ab:	e8 70 dd ff ff       	call   0x433720
  4359b0:	8b e8                	mov    ebp,eax
  4359b2:	59                   	pop    ecx
  4359b3:	45                   	inc    ebp
  4359b4:	80 3f 3d             	cmp    BYTE PTR [edi],0x3d
  4359b7:	74 22                	je     0x4359db
  4359b9:	55                   	push   ebp
  4359ba:	e8 d4 b3 ff ff       	call   0x430d93
  4359bf:	3b c3                	cmp    eax,ebx
  4359c1:	59                   	pop    ecx
  4359c2:	89 06                	mov    DWORD PTR [esi],eax
  4359c4:	75 08                	jne    0x4359ce
  4359c6:	6a 09                	push   0x9
  4359c8:	e8 13 b7 ff ff       	call   0x4310e0
  4359cd:	59                   	pop    ecx
  4359ce:	57                   	push   edi
  4359cf:	ff 36                	push   DWORD PTR [esi]
  4359d1:	e8 5a dc ff ff       	call   0x433630
  4359d6:	59                   	pop    ecx
  4359d7:	83 c6 04             	add    esi,0x4
  4359da:	59                   	pop    ecx
  4359db:	03 fd                	add    edi,ebp
  4359dd:	38 1f                	cmp    BYTE PTR [edi],bl
  4359df:	75 c9                	jne    0x4359aa
  4359e1:	5d                   	pop    ebp
  4359e2:	ff 35 14 c4 51 00    	push   DWORD PTR ds:0x51c414
  4359e8:	e8 3d b3 ff ff       	call   0x430d2a
  4359ed:	59                   	pop    ecx
  4359ee:	89 1d 14 c4 51 00    	mov    DWORD PTR ds:0x51c414,ebx
  4359f4:	89 1e                	mov    DWORD PTR [esi],ebx
  4359f6:	5f                   	pop    edi
  4359f7:	5e                   	pop    esi
  4359f8:	c7 05 4c da 51 00 01 	mov    DWORD PTR ds:0x51da4c,0x1
  4359ff:	00 00 00 
  435a02:	5b                   	pop    ebx
  435a03:	c3                   	ret
