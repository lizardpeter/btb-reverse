# source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05

verified retail PE32:     file format pei-i386


Disassembly of section .text:

004063a0 <.text+0x53a0>:
  4063a0:	53                   	push   ebx
  4063a1:	8b 5c 24 0c          	mov    ebx,DWORD PTR [esp+0xc]
  4063a5:	55                   	push   ebp
  4063a6:	83 fb fd             	cmp    ebx,0xfffffffd
  4063a9:	8b e9                	mov    ebp,ecx
  4063ab:	76 05                	jbe    0x4063b2
  4063ad:	e8 17 3b 03 00       	call   0x439ec9
  4063b2:	8b 4d 04             	mov    ecx,DWORD PTR [ebp+0x4]
  4063b5:	33 d2                	xor    edx,edx
  4063b7:	3b ca                	cmp    ecx,edx
  4063b9:	74 24                	je     0x4063df
  4063bb:	8a 41 ff             	mov    al,BYTE PTR [ecx-0x1]
  4063be:	84 c0                	test   al,al
  4063c0:	74 1d                	je     0x4063df
  4063c2:	3c ff                	cmp    al,0xff
  4063c4:	74 19                	je     0x4063df
  4063c6:	3b da                	cmp    ebx,edx
  4063c8:	75 3e                	jne    0x406408
  4063ca:	fe c8                	dec    al
  4063cc:	88 41 ff             	mov    BYTE PTR [ecx-0x1],al
  4063cf:	89 55 04             	mov    DWORD PTR [ebp+0x4],edx
  4063d2:	89 55 08             	mov    DWORD PTR [ebp+0x8],edx
  4063d5:	89 55 0c             	mov    DWORD PTR [ebp+0xc],edx
  4063d8:	8b c5                	mov    eax,ebp
  4063da:	5d                   	pop    ebp
  4063db:	5b                   	pop    ebx
  4063dc:	c2 08 00             	ret    0x8
  4063df:	3b da                	cmp    ebx,edx
  4063e1:	75 10                	jne    0x4063f3
  4063e3:	6a 01                	push   0x1
  4063e5:	8b cd                	mov    ecx,ebp
  4063e7:	e8 64 ff ff ff       	call   0x406350
  4063ec:	8b c5                	mov    eax,ebp
  4063ee:	5d                   	pop    ebp
  4063ef:	5b                   	pop    ebx
  4063f0:	c2 08 00             	ret    0x8
  4063f3:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  4063f6:	83 f8 1f             	cmp    eax,0x1f
  4063f9:	77 04                	ja     0x4063ff
  4063fb:	3b c3                	cmp    eax,ebx
  4063fd:	73 11                	jae    0x406410
  4063ff:	6a 01                	push   0x1
  406401:	8b cd                	mov    ecx,ebp
  406403:	e8 48 ff ff ff       	call   0x406350
  406408:	53                   	push   ebx
  406409:	8b cd                	mov    ecx,ebp
  40640b:	e8 80 03 00 00       	call   0x406790
  406410:	8b cb                	mov    ecx,ebx
  406412:	56                   	push   esi
  406413:	8b 74 24 10          	mov    esi,DWORD PTR [esp+0x10]
  406417:	8b c1                	mov    eax,ecx
  406419:	57                   	push   edi
  40641a:	8b 7d 04             	mov    edi,DWORD PTR [ebp+0x4]
  40641d:	c1 e9 02             	shr    ecx,0x2
  406420:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  406422:	8b c8                	mov    ecx,eax
  406424:	8b c5                	mov    eax,ebp
  406426:	83 e1 03             	and    ecx,0x3
  406429:	f3 a4                	rep movs BYTE PTR es:[edi],BYTE PTR ds:[esi]
  40642b:	8b 4d 04             	mov    ecx,DWORD PTR [ebp+0x4]
  40642e:	5f                   	pop    edi
  40642f:	89 5d 08             	mov    DWORD PTR [ebp+0x8],ebx
  406432:	5e                   	pop    esi
  406433:	c6 04 0b 00          	mov    BYTE PTR [ebx+ecx*1],0x0
  406437:	5d                   	pop    ebp
  406438:	5b                   	pop    ebx
  406439:	c2 08 00             	ret    0x8
  40643c:	90                   	nop
  40643d:	90                   	nop
  40643e:	90                   	nop
  40643f:	90                   	nop
