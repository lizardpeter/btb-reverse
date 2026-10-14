
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0043854d <.text+0x3754d>:
  43854d:	55                   	push   ebp
  43854e:	8b ec                	mov    ebp,esp
  438550:	56                   	push   esi
  438551:	57                   	push   edi
  438552:	8b 7d 08             	mov    edi,DWORD PTR [ebp+0x8]
  438555:	8b c7                	mov    eax,edi
  438557:	48                   	dec    eax
  438558:	48                   	dec    eax
  438559:	74 59                	je     0x4385b4
  43855b:	48                   	dec    eax
  43855c:	48                   	dec    eax
  43855d:	74 46                	je     0x4385a5
  43855f:	83 e8 04             	sub    eax,0x4
  438562:	74 41                	je     0x4385a5
  438564:	83 e8 03             	sub    eax,0x3
  438567:	74 3c                	je     0x4385a5
  438569:	83 e8 04             	sub    eax,0x4
  43856c:	74 2a                	je     0x438598
  43856e:	83 e8 06             	sub    eax,0x6
  438571:	74 18                	je     0x43858b
  438573:	48                   	dec    eax
  438574:	74 08                	je     0x43857e
  438576:	83 c8 ff             	or     eax,0xffffffff
  438579:	e9 f8 00 00 00       	jmp    0x438676
  43857e:	8b 35 a4 c6 51 00    	mov    esi,DWORD PTR ds:0x51c6a4
  438584:	b8 a4 c6 51 00       	mov    eax,0x51c6a4
  438589:	eb 34                	jmp    0x4385bf
  43858b:	8b 35 a0 c6 51 00    	mov    esi,DWORD PTR ds:0x51c6a0
  438591:	b8 a0 c6 51 00       	mov    eax,0x51c6a0
  438596:	eb 27                	jmp    0x4385bf
  438598:	8b 35 a8 c6 51 00    	mov    esi,DWORD PTR ds:0x51c6a8
  43859e:	b8 a8 c6 51 00       	mov    eax,0x51c6a8
  4385a3:	eb 1a                	jmp    0x4385bf
  4385a5:	57                   	push   edi
  4385a6:	e8 cf 00 00 00       	call   0x43867a
  4385ab:	8b 70 08             	mov    esi,DWORD PTR [eax+0x8]
  4385ae:	83 c0 08             	add    eax,0x8
  4385b1:	59                   	pop    ecx
  4385b2:	eb 0b                	jmp    0x4385bf
  4385b4:	8b 35 9c c6 51 00    	mov    esi,DWORD PTR ds:0x51c69c
  4385ba:	b8 9c c6 51 00       	mov    eax,0x51c69c
  4385bf:	83 fe 01             	cmp    esi,0x1
  4385c2:	75 07                	jne    0x4385cb
  4385c4:	33 c0                	xor    eax,eax
  4385c6:	e9 ab 00 00 00       	jmp    0x438676
  4385cb:	85 f6                	test   esi,esi
  4385cd:	75 07                	jne    0x4385d6
  4385cf:	6a 03                	push   0x3
  4385d1:	e8 9d 82 ff ff       	call   0x430873
  4385d6:	53                   	push   ebx
  4385d7:	6a 08                	push   0x8
  4385d9:	59                   	pop    ecx
  4385da:	3b f9                	cmp    edi,ecx
  4385dc:	74 0a                	je     0x4385e8
  4385de:	83 ff 0b             	cmp    edi,0xb
  4385e1:	74 05                	je     0x4385e8
  4385e3:	83 ff 04             	cmp    edi,0x4
  4385e6:	75 26                	jne    0x43860e
  4385e8:	8b 1d 58 c4 51 00    	mov    ebx,DWORD PTR ds:0x51c458
  4385ee:	83 25 58 c4 51 00 00 	and    DWORD PTR ds:0x51c458,0x0
  4385f5:	3b f9                	cmp    edi,ecx
  4385f7:	75 44                	jne    0x43863d
  4385f9:	8b 15 f4 97 44 00    	mov    edx,DWORD PTR ds:0x4497f4
  4385ff:	c7 05 f4 97 44 00 8c 	mov    DWORD PTR ds:0x4497f4,0x8c
  438606:	00 00 00 
  438609:	89 55 08             	mov    DWORD PTR [ebp+0x8],edx
  43860c:	eb 03                	jmp    0x438611
  43860e:	8b 5d 08             	mov    ebx,DWORD PTR [ebp+0x8]
  438611:	3b f9                	cmp    edi,ecx
  438613:	75 28                	jne    0x43863d
  438615:	a1 e8 97 44 00       	mov    eax,ds:0x4497e8
  43861a:	8b 0d ec 97 44 00    	mov    ecx,DWORD PTR ds:0x4497ec
  438620:	03 c8                	add    ecx,eax
  438622:	3b c1                	cmp    eax,ecx
  438624:	7d 1e                	jge    0x438644
  438626:	8d 14 40             	lea    edx,[eax+eax*2]
  438629:	2b c8                	sub    ecx,eax
  43862b:	8d 14 95 78 97 44 00 	lea    edx,[edx*4+0x449778]
  438632:	83 22 00             	and    DWORD PTR [edx],0x0
  438635:	83 c2 0c             	add    edx,0xc
  438638:	49                   	dec    ecx
  438639:	75 f7                	jne    0x438632
  43863b:	eb 07                	jmp    0x438644
  43863d:	83 20 00             	and    DWORD PTR [eax],0x0
  438640:	3b f9                	cmp    edi,ecx
  438642:	75 0e                	jne    0x438652
  438644:	ff 35 f4 97 44 00    	push   DWORD PTR ds:0x4497f4
  43864a:	6a 08                	push   0x8
  43864c:	ff d6                	call   esi
  43864e:	59                   	pop    ecx
  43864f:	59                   	pop    ecx
  438650:	eb 0e                	jmp    0x438660
  438652:	57                   	push   edi
  438653:	ff d6                	call   esi
  438655:	83 ff 0b             	cmp    edi,0xb
  438658:	59                   	pop    ecx
  438659:	74 05                	je     0x438660
  43865b:	83 ff 04             	cmp    edi,0x4
  43865e:	75 13                	jne    0x438673
  438660:	83 ff 08             	cmp    edi,0x8
  438663:	89 1d 58 c4 51 00    	mov    DWORD PTR ds:0x51c458,ebx
  438669:	75 08                	jne    0x438673
  43866b:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  43866e:	a3 f4 97 44 00       	mov    ds:0x4497f4,eax
  438673:	33 c0                	xor    eax,eax
  438675:	5b                   	pop    ebx
  438676:	5f                   	pop    edi
  438677:	5e                   	pop    esi
  438678:	5d                   	pop    ebp
  438679:	c3                   	ret
