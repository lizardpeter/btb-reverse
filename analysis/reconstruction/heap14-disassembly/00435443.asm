
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00435443 <.text+0x34443>:
  435443:	55                   	push   ebp
  435444:	8b ec                	mov    ebp,esp
  435446:	51                   	push   ecx
  435447:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  43544a:	8b 55 10             	mov    edx,DWORD PTR [ebp+0x10]
  43544d:	53                   	push   ebx
  43544e:	56                   	push   esi
  43544f:	8b 71 04             	mov    esi,DWORD PTR [ecx+0x4]
  435452:	57                   	push   edi
  435453:	8b 39                	mov    edi,DWORD PTR [ecx]
  435455:	8d 99 f8 00 00 00    	lea    ebx,[ecx+0xf8]
  43545b:	3b f2                	cmp    esi,edx
  43545d:	89 7d fc             	mov    DWORD PTR [ebp-0x4],edi
  435460:	8b c7                	mov    eax,edi
  435462:	89 5d 08             	mov    DWORD PTR [ebp+0x8],ebx
  435465:	72 21                	jb     0x435488
  435467:	8d 04 17             	lea    eax,[edi+edx*1]
  43546a:	88 17                	mov    BYTE PTR [edi],dl
  43546c:	3b c3                	cmp    eax,ebx
  43546e:	73 07                	jae    0x435477
  435470:	01 11                	add    DWORD PTR [ecx],edx
  435472:	29 51 04             	sub    DWORD PTR [ecx+0x4],edx
  435475:	eb 09                	jmp    0x435480
  435477:	83 61 04 00          	and    DWORD PTR [ecx+0x4],0x0
  43547b:	8d 41 08             	lea    eax,[ecx+0x8]
  43547e:	89 01                	mov    DWORD PTR [ecx],eax
  435480:	8d 47 08             	lea    eax,[edi+0x8]
  435483:	e9 ce 00 00 00       	jmp    0x435556
  435488:	03 f7                	add    esi,edi
  43548a:	80 3e 00             	cmp    BYTE PTR [esi],0x0
  43548d:	74 02                	je     0x435491
  43548f:	8b c6                	mov    eax,esi
  435491:	8d 34 10             	lea    esi,[eax+edx*1]
  435494:	3b f3                	cmp    esi,ebx
  435496:	73 43                	jae    0x4354db
  435498:	8a 18                	mov    bl,BYTE PTR [eax]
  43549a:	84 db                	test   bl,bl
  43549c:	75 30                	jne    0x4354ce
  43549e:	6a 01                	push   0x1
  4354a0:	8d 58 01             	lea    ebx,[eax+0x1]
  4354a3:	5e                   	pop    esi
  4354a4:	80 3b 00             	cmp    BYTE PTR [ebx],0x0
  4354a7:	75 04                	jne    0x4354ad
  4354a9:	43                   	inc    ebx
  4354aa:	46                   	inc    esi
  4354ab:	eb f7                	jmp    0x4354a4
  4354ad:	3b f2                	cmp    esi,edx
  4354af:	73 4e                	jae    0x4354ff
  4354b1:	3b 45 fc             	cmp    eax,DWORD PTR [ebp-0x4]
  4354b4:	75 05                	jne    0x4354bb
  4354b6:	89 71 04             	mov    DWORD PTR [ecx+0x4],esi
  4354b9:	eb 0c                	jmp    0x4354c7
  4354bb:	29 75 0c             	sub    DWORD PTR [ebp+0xc],esi
  4354be:	39 55 0c             	cmp    DWORD PTR [ebp+0xc],edx
  4354c1:	0f 82 99 00 00 00    	jb     0x435560
  4354c7:	8b 7d fc             	mov    edi,DWORD PTR [ebp-0x4]
  4354ca:	8b c3                	mov    eax,ebx
  4354cc:	eb 05                	jmp    0x4354d3
  4354ce:	0f b6 f3             	movzx  esi,bl
  4354d1:	03 c6                	add    eax,esi
  4354d3:	8d 34 10             	lea    esi,[eax+edx*1]
  4354d6:	3b 75 08             	cmp    esi,DWORD PTR [ebp+0x8]
  4354d9:	72 bd                	jb     0x435498
  4354db:	8d 71 08             	lea    esi,[ecx+0x8]
  4354de:	3b f7                	cmp    esi,edi
  4354e0:	73 7e                	jae    0x435560
  4354e2:	8d 04 16             	lea    eax,[esi+edx*1]
  4354e5:	3b 45 08             	cmp    eax,DWORD PTR [ebp+0x8]
  4354e8:	73 76                	jae    0x435560
  4354ea:	8a 06                	mov    al,BYTE PTR [esi]
  4354ec:	84 c0                	test   al,al
  4354ee:	75 40                	jne    0x435530
  4354f0:	6a 01                	push   0x1
  4354f2:	8d 5e 01             	lea    ebx,[esi+0x1]
  4354f5:	58                   	pop    eax
  4354f6:	80 3b 00             	cmp    BYTE PTR [ebx],0x0
  4354f9:	75 25                	jne    0x435520
  4354fb:	43                   	inc    ebx
  4354fc:	40                   	inc    eax
  4354fd:	eb f7                	jmp    0x4354f6
  4354ff:	8d 1c 10             	lea    ebx,[eax+edx*1]
  435502:	3b 5d 08             	cmp    ebx,DWORD PTR [ebp+0x8]
  435505:	73 09                	jae    0x435510
  435507:	2b f2                	sub    esi,edx
  435509:	89 19                	mov    DWORD PTR [ecx],ebx
  43550b:	89 71 04             	mov    DWORD PTR [ecx+0x4],esi
  43550e:	eb 09                	jmp    0x435519
  435510:	83 61 04 00          	and    DWORD PTR [ecx+0x4],0x0
  435514:	8d 71 08             	lea    esi,[ecx+0x8]
  435517:	89 31                	mov    DWORD PTR [ecx],esi
  435519:	88 10                	mov    BYTE PTR [eax],dl
  43551b:	83 c0 08             	add    eax,0x8
  43551e:	eb 36                	jmp    0x435556
  435520:	3b c2                	cmp    eax,edx
  435522:	73 13                	jae    0x435537
  435524:	29 45 0c             	sub    DWORD PTR [ebp+0xc],eax
  435527:	39 55 0c             	cmp    DWORD PTR [ebp+0xc],edx
  43552a:	72 34                	jb     0x435560
  43552c:	8b f3                	mov    esi,ebx
  43552e:	eb ae                	jmp    0x4354de
  435530:	0f b6 c0             	movzx  eax,al
  435533:	03 f0                	add    esi,eax
  435535:	eb a7                	jmp    0x4354de
  435537:	8d 1c 16             	lea    ebx,[esi+edx*1]
  43553a:	3b 5d 08             	cmp    ebx,DWORD PTR [ebp+0x8]
  43553d:	73 09                	jae    0x435548
  43553f:	2b c2                	sub    eax,edx
  435541:	89 19                	mov    DWORD PTR [ecx],ebx
  435543:	89 41 04             	mov    DWORD PTR [ecx+0x4],eax
  435546:	eb 09                	jmp    0x435551
  435548:	83 61 04 00          	and    DWORD PTR [ecx+0x4],0x0
  43554c:	8d 41 08             	lea    eax,[ecx+0x8]
  43554f:	89 01                	mov    DWORD PTR [ecx],eax
  435551:	88 16                	mov    BYTE PTR [esi],dl
  435553:	8d 46 08             	lea    eax,[esi+0x8]
  435556:	6b c9 0f             	imul   ecx,ecx,0xf
  435559:	c1 e0 04             	shl    eax,0x4
  43555c:	2b c1                	sub    eax,ecx
  43555e:	eb 02                	jmp    0x435562
  435560:	33 c0                	xor    eax,eax
  435562:	5f                   	pop    edi
  435563:	5e                   	pop    esi
  435564:	5b                   	pop    ebx
  435565:	c9                   	leave
  435566:	c3                   	ret
