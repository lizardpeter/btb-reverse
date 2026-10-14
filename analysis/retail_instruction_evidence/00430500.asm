# source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05

verified retail PE32:     file format pei-i386


Disassembly of section .text:

00430500 <.text+0x2f500>:
  430500:	55                   	push   ebp
  430501:	8b ec                	mov    ebp,esp
  430503:	57                   	push   edi
  430504:	56                   	push   esi
  430505:	8b 75 0c             	mov    esi,DWORD PTR [ebp+0xc]
  430508:	8b 4d 10             	mov    ecx,DWORD PTR [ebp+0x10]
  43050b:	8b 7d 08             	mov    edi,DWORD PTR [ebp+0x8]
  43050e:	8b c1                	mov    eax,ecx
  430510:	8b d1                	mov    edx,ecx
  430512:	03 c6                	add    eax,esi
  430514:	3b fe                	cmp    edi,esi
  430516:	76 08                	jbe    0x430520
  430518:	3b f8                	cmp    edi,eax
  43051a:	0f 82 78 01 00 00    	jb     0x430698
  430520:	f7 c7 03 00 00 00    	test   edi,0x3
  430526:	75 14                	jne    0x43053c
  430528:	c1 e9 02             	shr    ecx,0x2
  43052b:	83 e2 03             	and    edx,0x3
  43052e:	83 f9 08             	cmp    ecx,0x8
  430531:	72 29                	jb     0x43055c
  430533:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  430535:	ff 24 95 48 06 43 00 	jmp    DWORD PTR [edx*4+0x430648]
  43053c:	8b c7                	mov    eax,edi
  43053e:	ba 03 00 00 00       	mov    edx,0x3
  430543:	83 e9 04             	sub    ecx,0x4
  430546:	72 0c                	jb     0x430554
  430548:	83 e0 03             	and    eax,0x3
  43054b:	03 c8                	add    ecx,eax
  43054d:	ff 24 85 60 05 43 00 	jmp    DWORD PTR [eax*4+0x430560]
  430554:	ff 24 8d 58 06 43 00 	jmp    DWORD PTR [ecx*4+0x430658]
  43055b:	90                   	nop
  43055c:	ff 24 8d dc 05 43 00 	jmp    DWORD PTR [ecx*4+0x4305dc]
  430563:	90                   	nop
  430564:	70 05                	jo     0x43056b
  430566:	43                   	inc    ebx
  430567:	00 9c 05 43 00 c0 05 	add    BYTE PTR [ebp+eax*1+0x5c00043],bl
  43056e:	43                   	inc    ebx
  43056f:	00 23                	add    BYTE PTR [ebx],ah
  430571:	d1 8a 06 88 07 8a    	ror    DWORD PTR [edx-0x75f877fa],1
  430577:	46                   	inc    esi
  430578:	01 88 47 01 8a 46    	add    DWORD PTR [eax+0x468a0147],ecx
  43057e:	02 c1                	add    al,cl
  430580:	e9 02 88 47 02       	jmp    0x28a8d87
  430585:	83 c6 03             	add    esi,0x3
  430588:	83 c7 03             	add    edi,0x3
  43058b:	83 f9 08             	cmp    ecx,0x8
  43058e:	72 cc                	jb     0x43055c
  430590:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  430592:	ff 24 95 48 06 43 00 	jmp    DWORD PTR [edx*4+0x430648]
  430599:	8d 49 00             	lea    ecx,[ecx+0x0]
  43059c:	23 d1                	and    edx,ecx
  43059e:	8a 06                	mov    al,BYTE PTR [esi]
  4305a0:	88 07                	mov    BYTE PTR [edi],al
  4305a2:	8a 46 01             	mov    al,BYTE PTR [esi+0x1]
  4305a5:	c1 e9 02             	shr    ecx,0x2
  4305a8:	88 47 01             	mov    BYTE PTR [edi+0x1],al
  4305ab:	83 c6 02             	add    esi,0x2
  4305ae:	83 c7 02             	add    edi,0x2
  4305b1:	83 f9 08             	cmp    ecx,0x8
  4305b4:	72 a6                	jb     0x43055c
  4305b6:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  4305b8:	ff 24 95 48 06 43 00 	jmp    DWORD PTR [edx*4+0x430648]
  4305bf:	90                   	nop
  4305c0:	23 d1                	and    edx,ecx
  4305c2:	8a 06                	mov    al,BYTE PTR [esi]
  4305c4:	88 07                	mov    BYTE PTR [edi],al
  4305c6:	46                   	inc    esi
  4305c7:	c1 e9 02             	shr    ecx,0x2
  4305ca:	47                   	inc    edi
  4305cb:	83 f9 08             	cmp    ecx,0x8
  4305ce:	72 8c                	jb     0x43055c
  4305d0:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  4305d2:	ff 24 95 48 06 43 00 	jmp    DWORD PTR [edx*4+0x430648]
  4305d9:	8d 49 00             	lea    ecx,[ecx+0x0]
  4305dc:	3f                   	aas
  4305dd:	06                   	push   es
  4305de:	43                   	inc    ebx
  4305df:	00 2c 06             	add    BYTE PTR [esi+eax*1],ch
  4305e2:	43                   	inc    ebx
  4305e3:	00 24 06             	add    BYTE PTR [esi+eax*1],ah
  4305e6:	43                   	inc    ebx
  4305e7:	00 1c 06             	add    BYTE PTR [esi+eax*1],bl
  4305ea:	43                   	inc    ebx
  4305eb:	00 14 06             	add    BYTE PTR [esi+eax*1],dl
  4305ee:	43                   	inc    ebx
  4305ef:	00 0c 06             	add    BYTE PTR [esi+eax*1],cl
  4305f2:	43                   	inc    ebx
  4305f3:	00 04 06             	add    BYTE PTR [esi+eax*1],al
  4305f6:	43                   	inc    ebx
  4305f7:	00 fc                	add    ah,bh
  4305f9:	05 43 00 8b 44       	add    eax,0x448b0043
  4305fe:	8e e4                	mov    fs,esp
  430600:	89 44 8f e4          	mov    DWORD PTR [edi+ecx*4-0x1c],eax
  430604:	8b 44 8e e8          	mov    eax,DWORD PTR [esi+ecx*4-0x18]
  430608:	89 44 8f e8          	mov    DWORD PTR [edi+ecx*4-0x18],eax
  43060c:	8b 44 8e ec          	mov    eax,DWORD PTR [esi+ecx*4-0x14]
  430610:	89 44 8f ec          	mov    DWORD PTR [edi+ecx*4-0x14],eax
  430614:	8b 44 8e f0          	mov    eax,DWORD PTR [esi+ecx*4-0x10]
  430618:	89 44 8f f0          	mov    DWORD PTR [edi+ecx*4-0x10],eax
  43061c:	8b 44 8e f4          	mov    eax,DWORD PTR [esi+ecx*4-0xc]
  430620:	89 44 8f f4          	mov    DWORD PTR [edi+ecx*4-0xc],eax
  430624:	8b 44 8e f8          	mov    eax,DWORD PTR [esi+ecx*4-0x8]
  430628:	89 44 8f f8          	mov    DWORD PTR [edi+ecx*4-0x8],eax
  43062c:	8b 44 8e fc          	mov    eax,DWORD PTR [esi+ecx*4-0x4]
  430630:	89 44 8f fc          	mov    DWORD PTR [edi+ecx*4-0x4],eax
  430634:	8d 04 8d 00 00 00 00 	lea    eax,[ecx*4+0x0]
  43063b:	03 f0                	add    esi,eax
  43063d:	03 f8                	add    edi,eax
  43063f:	ff 24 95 48 06 43 00 	jmp    DWORD PTR [edx*4+0x430648]
  430646:	8b ff                	mov    edi,edi
  430648:	58                   	pop    eax
  430649:	06                   	push   es
  43064a:	43                   	inc    ebx
  43064b:	00 60 06             	add    BYTE PTR [eax+0x6],ah
  43064e:	43                   	inc    ebx
  43064f:	00 6c 06 43          	add    BYTE PTR [esi+eax*1+0x43],ch
  430653:	00 80 06 43 00 8b    	add    BYTE PTR [eax-0x74ffbcfa],al
  430659:	45                   	inc    ebp
  43065a:	08 5e 5f             	or     BYTE PTR [esi+0x5f],bl
  43065d:	c9                   	leave
  43065e:	c3                   	ret
  43065f:	90                   	nop
  430660:	8a 06                	mov    al,BYTE PTR [esi]
  430662:	88 07                	mov    BYTE PTR [edi],al
  430664:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  430667:	5e                   	pop    esi
  430668:	5f                   	pop    edi
  430669:	c9                   	leave
  43066a:	c3                   	ret
  43066b:	90                   	nop
  43066c:	8a 06                	mov    al,BYTE PTR [esi]
  43066e:	88 07                	mov    BYTE PTR [edi],al
  430670:	8a 46 01             	mov    al,BYTE PTR [esi+0x1]
  430673:	88 47 01             	mov    BYTE PTR [edi+0x1],al
  430676:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  430679:	5e                   	pop    esi
  43067a:	5f                   	pop    edi
  43067b:	c9                   	leave
  43067c:	c3                   	ret
  43067d:	8d 49 00             	lea    ecx,[ecx+0x0]
  430680:	8a 06                	mov    al,BYTE PTR [esi]
  430682:	88 07                	mov    BYTE PTR [edi],al
  430684:	8a 46 01             	mov    al,BYTE PTR [esi+0x1]
  430687:	88 47 01             	mov    BYTE PTR [edi+0x1],al
  43068a:	8a 46 02             	mov    al,BYTE PTR [esi+0x2]
  43068d:	88 47 02             	mov    BYTE PTR [edi+0x2],al
  430690:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  430693:	5e                   	pop    esi
  430694:	5f                   	pop    edi
  430695:	c9                   	leave
  430696:	c3                   	ret
  430697:	90                   	nop
  430698:	8d 74 31 fc          	lea    esi,[ecx+esi*1-0x4]
  43069c:	8d 7c 39 fc          	lea    edi,[ecx+edi*1-0x4]
  4306a0:	f7 c7 03 00 00 00    	test   edi,0x3
  4306a6:	75 24                	jne    0x4306cc
  4306a8:	c1 e9 02             	shr    ecx,0x2
  4306ab:	83 e2 03             	and    edx,0x3
  4306ae:	83 f9 08             	cmp    ecx,0x8
  4306b1:	72 0d                	jb     0x4306c0
  4306b3:	fd                   	std
  4306b4:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  4306b6:	fc                   	cld
  4306b7:	ff 24 95 e0 07 43 00 	jmp    DWORD PTR [edx*4+0x4307e0]
  4306be:	8b ff                	mov    edi,edi
  4306c0:	f7 d9                	neg    ecx
  4306c2:	ff 24 8d 90 07 43 00 	jmp    DWORD PTR [ecx*4+0x430790]
  4306c9:	8d 49 00             	lea    ecx,[ecx+0x0]
  4306cc:	8b c7                	mov    eax,edi
  4306ce:	ba 03 00 00 00       	mov    edx,0x3
  4306d3:	83 f9 04             	cmp    ecx,0x4
  4306d6:	72 0c                	jb     0x4306e4
  4306d8:	83 e0 03             	and    eax,0x3
  4306db:	2b c8                	sub    ecx,eax
  4306dd:	ff 24 85 e8 06 43 00 	jmp    DWORD PTR [eax*4+0x4306e8]
  4306e4:	ff 24 8d e0 07 43 00 	jmp    DWORD PTR [ecx*4+0x4307e0]
  4306eb:	90                   	nop
  4306ec:	f8                   	clc
  4306ed:	06                   	push   es
  4306ee:	43                   	inc    ebx
  4306ef:	00 18                	add    BYTE PTR [eax],bl
  4306f1:	07                   	pop    es
  4306f2:	43                   	inc    ebx
  4306f3:	00 40 07             	add    BYTE PTR [eax+0x7],al
  4306f6:	43                   	inc    ebx
  4306f7:	00 8a 46 03 23 d1    	add    BYTE PTR [edx-0x2edcfcba],cl
  4306fd:	88 47 03             	mov    BYTE PTR [edi+0x3],al
  430700:	4e                   	dec    esi
  430701:	c1 e9 02             	shr    ecx,0x2
  430704:	4f                   	dec    edi
  430705:	83 f9 08             	cmp    ecx,0x8
  430708:	72 b6                	jb     0x4306c0
  43070a:	fd                   	std
  43070b:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  43070d:	fc                   	cld
  43070e:	ff 24 95 e0 07 43 00 	jmp    DWORD PTR [edx*4+0x4307e0]
  430715:	8d 49 00             	lea    ecx,[ecx+0x0]
  430718:	8a 46 03             	mov    al,BYTE PTR [esi+0x3]
  43071b:	23 d1                	and    edx,ecx
  43071d:	88 47 03             	mov    BYTE PTR [edi+0x3],al
  430720:	8a 46 02             	mov    al,BYTE PTR [esi+0x2]
  430723:	c1 e9 02             	shr    ecx,0x2
  430726:	88 47 02             	mov    BYTE PTR [edi+0x2],al
  430729:	83 ee 02             	sub    esi,0x2
  43072c:	83 ef 02             	sub    edi,0x2
  43072f:	83 f9 08             	cmp    ecx,0x8
  430732:	72 8c                	jb     0x4306c0
  430734:	fd                   	std
  430735:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  430737:	fc                   	cld
  430738:	ff 24 95 e0 07 43 00 	jmp    DWORD PTR [edx*4+0x4307e0]
  43073f:	90                   	nop
  430740:	8a 46 03             	mov    al,BYTE PTR [esi+0x3]
  430743:	23 d1                	and    edx,ecx
  430745:	88 47 03             	mov    BYTE PTR [edi+0x3],al
  430748:	8a 46 02             	mov    al,BYTE PTR [esi+0x2]
  43074b:	88 47 02             	mov    BYTE PTR [edi+0x2],al
  43074e:	8a 46 01             	mov    al,BYTE PTR [esi+0x1]
  430751:	c1 e9 02             	shr    ecx,0x2
  430754:	88 47 01             	mov    BYTE PTR [edi+0x1],al
  430757:	83 ee 03             	sub    esi,0x3
  43075a:	83 ef 03             	sub    edi,0x3
  43075d:	83 f9 08             	cmp    ecx,0x8
  430760:	0f 82 5a ff ff ff    	jb     0x4306c0
  430766:	fd                   	std
  430767:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  430769:	fc                   	cld
  43076a:	ff 24 95 e0 07 43 00 	jmp    DWORD PTR [edx*4+0x4307e0]
  430771:	8d 49 00             	lea    ecx,[ecx+0x0]
  430774:	94                   	xchg   esp,eax
  430775:	07                   	pop    es
  430776:	43                   	inc    ebx
  430777:	00 9c 07 43 00 a4 07 	add    BYTE PTR [edi+eax*1+0x7a40043],bl
  43077e:	43                   	inc    ebx
  43077f:	00 ac 07 43 00 b4 07 	add    BYTE PTR [edi+eax*1+0x7b40043],ch
  430786:	43                   	inc    ebx
  430787:	00 bc 07 43 00 c4 07 	add    BYTE PTR [edi+eax*1+0x7c40043],bh
  43078e:	43                   	inc    ebx
  43078f:	00 d7                	add    bh,dl
  430791:	07                   	pop    es
  430792:	43                   	inc    ebx
  430793:	00 8b 44 8e 1c 89    	add    BYTE PTR [ebx-0x76e371bc],cl
  430799:	44                   	inc    esp
  43079a:	8f                   	(bad)
  43079b:	1c 8b                	sbb    al,0x8b
  43079d:	44                   	inc    esp
  43079e:	8e 18                	mov    ds,WORD PTR [eax]
  4307a0:	89 44 8f 18          	mov    DWORD PTR [edi+ecx*4+0x18],eax
  4307a4:	8b 44 8e 14          	mov    eax,DWORD PTR [esi+ecx*4+0x14]
  4307a8:	89 44 8f 14          	mov    DWORD PTR [edi+ecx*4+0x14],eax
  4307ac:	8b 44 8e 10          	mov    eax,DWORD PTR [esi+ecx*4+0x10]
  4307b0:	89 44 8f 10          	mov    DWORD PTR [edi+ecx*4+0x10],eax
  4307b4:	8b 44 8e 0c          	mov    eax,DWORD PTR [esi+ecx*4+0xc]
  4307b8:	89 44 8f 0c          	mov    DWORD PTR [edi+ecx*4+0xc],eax
  4307bc:	8b 44 8e 08          	mov    eax,DWORD PTR [esi+ecx*4+0x8]
  4307c0:	89 44 8f 08          	mov    DWORD PTR [edi+ecx*4+0x8],eax
  4307c4:	8b 44 8e 04          	mov    eax,DWORD PTR [esi+ecx*4+0x4]
  4307c8:	89 44 8f 04          	mov    DWORD PTR [edi+ecx*4+0x4],eax
  4307cc:	8d 04 8d 00 00 00 00 	lea    eax,[ecx*4+0x0]
  4307d3:	03 f0                	add    esi,eax
  4307d5:	03 f8                	add    edi,eax
  4307d7:	ff 24 95 e0 07 43 00 	jmp    DWORD PTR [edx*4+0x4307e0]
  4307de:	8b ff                	mov    edi,edi
  4307e0:	f0 07                	lock pop es
  4307e2:	43                   	inc    ebx
  4307e3:	00 f8                	add    al,bh
  4307e5:	07                   	pop    es
  4307e6:	43                   	inc    ebx
  4307e7:	00 08                	add    BYTE PTR [eax],cl
  4307e9:	08 43 00             	or     BYTE PTR [ebx+0x0],al
  4307ec:	1c 08                	sbb    al,0x8
  4307ee:	43                   	inc    ebx
  4307ef:	00 8b 45 08 5e 5f    	add    BYTE PTR [ebx+0x5f5e0845],cl
  4307f5:	c9                   	leave
  4307f6:	c3                   	ret
  4307f7:	90                   	nop
  4307f8:	8a 46 03             	mov    al,BYTE PTR [esi+0x3]
  4307fb:	88 47 03             	mov    BYTE PTR [edi+0x3],al
  4307fe:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  430801:	5e                   	pop    esi
  430802:	5f                   	pop    edi
  430803:	c9                   	leave
  430804:	c3                   	ret
  430805:	8d 49 00             	lea    ecx,[ecx+0x0]
  430808:	8a 46 03             	mov    al,BYTE PTR [esi+0x3]
  43080b:	88 47 03             	mov    BYTE PTR [edi+0x3],al
  43080e:	8a 46 02             	mov    al,BYTE PTR [esi+0x2]
  430811:	88 47 02             	mov    BYTE PTR [edi+0x2],al
  430814:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  430817:	5e                   	pop    esi
  430818:	5f                   	pop    edi
  430819:	c9                   	leave
  43081a:	c3                   	ret
  43081b:	90                   	nop
  43081c:	8a 46 03             	mov    al,BYTE PTR [esi+0x3]
  43081f:	88 47 03             	mov    BYTE PTR [edi+0x3],al
  430822:	8a 46 02             	mov    al,BYTE PTR [esi+0x2]
  430825:	88 47 02             	mov    BYTE PTR [edi+0x2],al
  430828:	8a 46 01             	mov    al,BYTE PTR [esi+0x1]
  43082b:	88 47 01             	mov    BYTE PTR [edi+0x1],al
  43082e:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  430831:	5e                   	pop    esi
  430832:	5f                   	pop    edi
  430833:	c9                   	leave
  430834:	c3                   	ret
