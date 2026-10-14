# source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05

verified retail PE32:     file format pei-i386


Disassembly of section .text:

00406460 <.text+0x5460>:
  406460:	53                   	push   ebx
  406461:	56                   	push   esi
  406462:	57                   	push   edi
  406463:	8b 7c 24 10          	mov    edi,DWORD PTR [esp+0x10]
  406467:	83 ff fd             	cmp    edi,0xfffffffd
  40646a:	8b f1                	mov    esi,ecx
  40646c:	76 05                	jbe    0x406473
  40646e:	e8 56 3a 03 00       	call   0x439ec9
  406473:	8b 46 04             	mov    eax,DWORD PTR [esi+0x4]
  406476:	33 db                	xor    ebx,ebx
  406478:	3b c3                	cmp    eax,ebx
  40647a:	74 2a                	je     0x4064a6
  40647c:	8a 48 ff             	mov    cl,BYTE PTR [eax-0x1]
  40647f:	3a cb                	cmp    cl,bl
  406481:	74 23                	je     0x4064a6
  406483:	80 f9 ff             	cmp    cl,0xff
  406486:	74 1e                	je     0x4064a6
  406488:	3b fb                	cmp    edi,ebx
  40648a:	0f 85 da 00 00 00    	jne    0x40656a
  406490:	fe c9                	dec    cl
  406492:	5f                   	pop    edi
  406493:	88 48 ff             	mov    BYTE PTR [eax-0x1],cl
  406496:	89 5e 04             	mov    DWORD PTR [esi+0x4],ebx
  406499:	89 5e 08             	mov    DWORD PTR [esi+0x8],ebx
  40649c:	89 5e 0c             	mov    DWORD PTR [esi+0xc],ebx
  40649f:	5e                   	pop    esi
  4064a0:	32 c0                	xor    al,al
  4064a2:	5b                   	pop    ebx
  4064a3:	c2 08 00             	ret    0x8
  4064a6:	8a 4c 24 14          	mov    cl,BYTE PTR [esp+0x14]
  4064aa:	3b fb                	cmp    edi,ebx
  4064ac:	75 56                	jne    0x406504
  4064ae:	3a cb                	cmp    cl,bl
  4064b0:	74 41                	je     0x4064f3
  4064b2:	3b c3                	cmp    eax,ebx
  4064b4:	74 2c                	je     0x4064e2
  4064b6:	8d 48 ff             	lea    ecx,[eax-0x1]
  4064b9:	8a 40 ff             	mov    al,BYTE PTR [eax-0x1]
  4064bc:	3a c3                	cmp    al,bl
  4064be:	74 19                	je     0x4064d9
  4064c0:	3c ff                	cmp    al,0xff
  4064c2:	74 15                	je     0x4064d9
  4064c4:	fe c8                	dec    al
  4064c6:	5f                   	pop    edi
  4064c7:	88 01                	mov    BYTE PTR [ecx],al
  4064c9:	89 5e 04             	mov    DWORD PTR [esi+0x4],ebx
  4064cc:	89 5e 08             	mov    DWORD PTR [esi+0x8],ebx
  4064cf:	89 5e 0c             	mov    DWORD PTR [esi+0xc],ebx
  4064d2:	5e                   	pop    esi
  4064d3:	32 c0                	xor    al,al
  4064d5:	5b                   	pop    ebx
  4064d6:	c2 08 00             	ret    0x8
  4064d9:	51                   	push   ecx
  4064da:	e8 fd 96 02 00       	call   0x42fbdc
  4064df:	83 c4 04             	add    esp,0x4
  4064e2:	89 5e 04             	mov    DWORD PTR [esi+0x4],ebx
  4064e5:	89 5e 08             	mov    DWORD PTR [esi+0x8],ebx
  4064e8:	89 5e 0c             	mov    DWORD PTR [esi+0xc],ebx
  4064eb:	5f                   	pop    edi
  4064ec:	5e                   	pop    esi
  4064ed:	32 c0                	xor    al,al
  4064ef:	5b                   	pop    ebx
  4064f0:	c2 08 00             	ret    0x8
  4064f3:	3b c3                	cmp    eax,ebx
  4064f5:	74 05                	je     0x4064fc
  4064f7:	89 5e 08             	mov    DWORD PTR [esi+0x8],ebx
  4064fa:	88 18                	mov    BYTE PTR [eax],bl
  4064fc:	5f                   	pop    edi
  4064fd:	5e                   	pop    esi
  4064fe:	32 c0                	xor    al,al
  406500:	5b                   	pop    ebx
  406501:	c2 08 00             	ret    0x8
  406504:	3a cb                	cmp    cl,bl
  406506:	74 5d                	je     0x406565
  406508:	8b 4e 0c             	mov    ecx,DWORD PTR [esi+0xc]
  40650b:	83 f9 1f             	cmp    ecx,0x1f
  40650e:	77 04                	ja     0x406514
  406510:	3b cf                	cmp    ecx,edi
  406512:	73 5e                	jae    0x406572
  406514:	3b c3                	cmp    eax,ebx
  406516:	74 34                	je     0x40654c
  406518:	8d 48 ff             	lea    ecx,[eax-0x1]
  40651b:	8a 40 ff             	mov    al,BYTE PTR [eax-0x1]
  40651e:	3a c3                	cmp    al,bl
  406520:	74 21                	je     0x406543
  406522:	3c ff                	cmp    al,0xff
  406524:	74 1d                	je     0x406543
  406526:	fe c8                	dec    al
  406528:	57                   	push   edi
  406529:	88 01                	mov    BYTE PTR [ecx],al
  40652b:	8b ce                	mov    ecx,esi
  40652d:	89 5e 04             	mov    DWORD PTR [esi+0x4],ebx
  406530:	89 5e 08             	mov    DWORD PTR [esi+0x8],ebx
  406533:	89 5e 0c             	mov    DWORD PTR [esi+0xc],ebx
  406536:	e8 55 02 00 00       	call   0x406790
  40653b:	5f                   	pop    edi
  40653c:	5e                   	pop    esi
  40653d:	b0 01                	mov    al,0x1
  40653f:	5b                   	pop    ebx
  406540:	c2 08 00             	ret    0x8
  406543:	51                   	push   ecx
  406544:	e8 93 96 02 00       	call   0x42fbdc
  406549:	83 c4 04             	add    esp,0x4
  40654c:	57                   	push   edi
  40654d:	8b ce                	mov    ecx,esi
  40654f:	89 5e 04             	mov    DWORD PTR [esi+0x4],ebx
  406552:	89 5e 08             	mov    DWORD PTR [esi+0x8],ebx
  406555:	89 5e 0c             	mov    DWORD PTR [esi+0xc],ebx
  406558:	e8 33 02 00 00       	call   0x406790
  40655d:	5f                   	pop    edi
  40655e:	5e                   	pop    esi
  40655f:	b0 01                	mov    al,0x1
  406561:	5b                   	pop    ebx
  406562:	c2 08 00             	ret    0x8
  406565:	39 7e 0c             	cmp    DWORD PTR [esi+0xc],edi
  406568:	73 08                	jae    0x406572
  40656a:	57                   	push   edi
  40656b:	8b ce                	mov    ecx,esi
  40656d:	e8 1e 02 00 00       	call   0x406790
  406572:	5f                   	pop    edi
  406573:	5e                   	pop    esi
  406574:	b0 01                	mov    al,0x1
  406576:	5b                   	pop    ebx
  406577:	c2 08 00             	ret    0x8
  40657a:	90                   	nop
  40657b:	90                   	nop
  40657c:	90                   	nop
  40657d:	90                   	nop
  40657e:	90                   	nop
  40657f:	90                   	nop
