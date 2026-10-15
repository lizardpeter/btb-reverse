
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00404560 <.text+0x3560>:
  404560:	8b 44 24 0c          	mov    eax,DWORD PTR [esp+0xc]
  404564:	83 ec 48             	sub    esp,0x48
  404567:	83 f8 01             	cmp    eax,0x1
  40456a:	53                   	push   ebx
  40456b:	8b d9                	mov    ebx,ecx
  40456d:	55                   	push   ebp
  40456e:	56                   	push   esi
  40456f:	57                   	push   edi
  404570:	89 43 7c             	mov    DWORD PTR [ebx+0x7c],eax
  404573:	c7 83 80 00 00 00 00 	mov    DWORD PTR [ebx+0x80],0x0
  40457a:	00 00 00 
  40457d:	0f 85 5e 01 00 00    	jne    0x4046e1
  404583:	8b 7c 24 5c          	mov    edi,DWORD PTR [esp+0x5c]
  404587:	85 ff                	test   edi,edi
  404589:	75 0f                	jne    0x40459a
  40458b:	5f                   	pop    edi
  40458c:	5e                   	pop    esi
  40458d:	5d                   	pop    ebp
  40458e:	b8 57 00 07 80       	mov    eax,0x80070057
  404593:	5b                   	pop    ebx
  404594:	83 c4 48             	add    esp,0x48
  404597:	c2 0c 00             	ret    0xc
  40459a:	8b 03                	mov    eax,DWORD PTR [ebx]
  40459c:	85 c0                	test   eax,eax
  40459e:	74 0f                	je     0x4045af
  4045a0:	50                   	push   eax
  4045a1:	e8 36 b6 02 00       	call   0x42fbdc
  4045a6:	83 c4 04             	add    esp,0x4
  4045a9:	c7 03 00 00 00 00    	mov    DWORD PTR [ebx],0x0
  4045af:	68 00 00 01 00       	push   0x10000
  4045b4:	6a 00                	push   0x0
  4045b6:	57                   	push   edi
  4045b7:	ff 15 94 b2 43 00    	call   DWORD PTR ds:0x43b294
  4045bd:	85 c0                	test   eax,eax
  4045bf:	89 43 04             	mov    DWORD PTR [ebx+0x4],eax
  4045c2:	0f 85 d9 00 00 00    	jne    0x4046a1
  4045c8:	8b 2d 44 b1 43 00    	mov    ebp,DWORD PTR ds:0x43b144
  4045ce:	68 54 e1 43 00       	push   0x43e154
  4045d3:	57                   	push   edi
  4045d4:	50                   	push   eax
  4045d5:	ff d5                	call   ebp
  4045d7:	8b f0                	mov    esi,eax
  4045d9:	85 f6                	test   esi,esi
  4045db:	75 1e                	jne    0x4045fb
  4045dd:	68 50 e1 43 00       	push   0x43e150
  4045e2:	57                   	push   edi
  4045e3:	50                   	push   eax
  4045e4:	ff d5                	call   ebp
  4045e6:	8b f0                	mov    esi,eax
  4045e8:	85 f6                	test   esi,esi
  4045ea:	75 0f                	jne    0x4045fb
  4045ec:	5f                   	pop    edi
  4045ed:	5e                   	pop    esi
  4045ee:	5d                   	pop    ebp
  4045ef:	b8 05 40 00 80       	mov    eax,0x80004005
  4045f4:	5b                   	pop    ebx
  4045f5:	83 c4 48             	add    esp,0x48
  4045f8:	c2 0c 00             	ret    0xc
  4045fb:	56                   	push   esi
  4045fc:	6a 00                	push   0x0
  4045fe:	ff 15 48 b1 43 00    	call   DWORD PTR ds:0x43b148
  404604:	8b f8                	mov    edi,eax
  404606:	85 ff                	test   edi,edi
  404608:	75 0f                	jne    0x404619
  40460a:	5f                   	pop    edi
  40460b:	5e                   	pop    esi
  40460c:	5d                   	pop    ebp
  40460d:	b8 05 40 00 80       	mov    eax,0x80004005
  404612:	5b                   	pop    ebx
  404613:	83 c4 48             	add    esp,0x48
  404616:	c2 0c 00             	ret    0xc
  404619:	56                   	push   esi
  40461a:	6a 00                	push   0x0
  40461c:	ff 15 3c b1 43 00    	call   DWORD PTR ds:0x43b13c
  404622:	8b e8                	mov    ebp,eax
  404624:	85 ed                	test   ebp,ebp
  404626:	75 0f                	jne    0x404637
  404628:	5f                   	pop    edi
  404629:	5e                   	pop    esi
  40462a:	5d                   	pop    ebp
  40462b:	b8 05 40 00 80       	mov    eax,0x80004005
  404630:	5b                   	pop    ebx
  404631:	83 c4 48             	add    esp,0x48
  404634:	c2 0c 00             	ret    0xc
  404637:	57                   	push   edi
  404638:	ff 15 4c b1 43 00    	call   DWORD PTR ds:0x43b14c
  40463e:	8b f0                	mov    esi,eax
  404640:	85 f6                	test   esi,esi
  404642:	75 0f                	jne    0x404653
  404644:	5f                   	pop    edi
  404645:	5e                   	pop    esi
  404646:	5d                   	pop    ebp
  404647:	b8 05 40 00 80       	mov    eax,0x80004005
  40464c:	5b                   	pop    ebx
  40464d:	83 c4 48             	add    esp,0x48
  404650:	c2 0c 00             	ret    0xc
  404653:	55                   	push   ebp
  404654:	e8 75 b5 02 00       	call   0x42fbce
  404659:	8b cd                	mov    ecx,ebp
  40465b:	8b d0                	mov    edx,eax
  40465d:	8b c1                	mov    eax,ecx
  40465f:	8b fa                	mov    edi,edx
  404661:	c1 e9 02             	shr    ecx,0x2
  404664:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  404666:	8b c8                	mov    ecx,eax
  404668:	33 c0                	xor    eax,eax
  40466a:	83 e1 03             	and    ecx,0x3
  40466d:	83 c4 04             	add    esp,0x4
  404670:	f3 a4                	rep movs BYTE PTR es:[edi],BYTE PTR ds:[esi]
  404672:	b9 12 00 00 00       	mov    ecx,0x12
  404677:	8d 7c 24 10          	lea    edi,[esp+0x10]
  40467b:	f3 ab                	rep stos DWORD PTR es:[edi],eax
  40467d:	8d 4c 24 10          	lea    ecx,[esp+0x10]
  404681:	68 00 00 01 00       	push   0x10000
  404686:	51                   	push   ecx
  404687:	50                   	push   eax
  404688:	c7 44 24 20 4d 45 4d 	mov    DWORD PTR [esp+0x20],0x204d454d
  40468f:	20 
  404690:	89 6c 24 30          	mov    DWORD PTR [esp+0x30],ebp
  404694:	89 54 24 34          	mov    DWORD PTR [esp+0x34],edx
  404698:	ff 15 94 b2 43 00    	call   DWORD PTR ds:0x43b294
  40469e:	89 43 04             	mov    DWORD PTR [ebx+0x4],eax
  4046a1:	8b cb                	mov    ecx,ebx
  4046a3:	e8 a8 00 00 00       	call   0x404750
  4046a8:	8b f0                	mov    esi,eax
  4046aa:	85 f6                	test   esi,esi
  4046ac:	7d 18                	jge    0x4046c6
  4046ae:	8b 53 04             	mov    edx,DWORD PTR [ebx+0x4]
  4046b1:	6a 00                	push   0x0
  4046b3:	52                   	push   edx
  4046b4:	ff 15 90 b2 43 00    	call   DWORD PTR ds:0x43b290
  4046ba:	8b c6                	mov    eax,esi
  4046bc:	5f                   	pop    edi
  4046bd:	5e                   	pop    esi
  4046be:	5d                   	pop    ebp
  4046bf:	5b                   	pop    ebx
  4046c0:	83 c4 48             	add    esp,0x48
  4046c3:	c2 0c 00             	ret    0xc
  4046c6:	8b cb                	mov    ecx,ebx
  4046c8:	e8 23 02 00 00       	call   0x4048f0
  4046cd:	85 c0                	test   eax,eax
  4046cf:	7c 69                	jl     0x40473a
  4046d1:	8b 4b 0c             	mov    ecx,DWORD PTR [ebx+0xc]
  4046d4:	5f                   	pop    edi
  4046d5:	5e                   	pop    esi
  4046d6:	89 4b 30             	mov    DWORD PTR [ebx+0x30],ecx
  4046d9:	5d                   	pop    ebp
  4046da:	5b                   	pop    ebx
  4046db:	83 c4 48             	add    esp,0x48
  4046de:	c2 0c 00             	ret    0xc
  4046e1:	8b 54 24 5c          	mov    edx,DWORD PTR [esp+0x5c]
  4046e5:	68 02 10 01 00       	push   0x11002
  4046ea:	6a 00                	push   0x0
  4046ec:	52                   	push   edx
  4046ed:	ff 15 94 b2 43 00    	call   DWORD PTR ds:0x43b294
  4046f3:	85 c0                	test   eax,eax
  4046f5:	89 43 04             	mov    DWORD PTR [ebx+0x4],eax
  4046f8:	75 0f                	jne    0x404709
  4046fa:	5f                   	pop    edi
  4046fb:	5e                   	pop    esi
  4046fc:	5d                   	pop    ebp
  4046fd:	b8 05 40 00 80       	mov    eax,0x80004005
  404702:	5b                   	pop    ebx
  404703:	83 c4 48             	add    esp,0x48
  404706:	c2 0c 00             	ret    0xc
  404709:	8b 44 24 60          	mov    eax,DWORD PTR [esp+0x60]
  40470d:	8b cb                	mov    ecx,ebx
  40470f:	50                   	push   eax
  404710:	e8 4b 05 00 00       	call   0x404c60
  404715:	8b f0                	mov    esi,eax
  404717:	85 f6                	test   esi,esi
  404719:	7d 18                	jge    0x404733
  40471b:	8b 4b 04             	mov    ecx,DWORD PTR [ebx+0x4]
  40471e:	6a 00                	push   0x0
  404720:	51                   	push   ecx
  404721:	ff 15 90 b2 43 00    	call   DWORD PTR ds:0x43b290
  404727:	8b c6                	mov    eax,esi
  404729:	5f                   	pop    edi
  40472a:	5e                   	pop    esi
  40472b:	5d                   	pop    ebp
  40472c:	5b                   	pop    ebx
  40472d:	83 c4 48             	add    esp,0x48
  404730:	c2 0c 00             	ret    0xc
  404733:	8b cb                	mov    ecx,ebx
  404735:	e8 b6 01 00 00       	call   0x4048f0
  40473a:	5f                   	pop    edi
  40473b:	5e                   	pop    esi
  40473c:	5d                   	pop    ebp
  40473d:	5b                   	pop    ebx
  40473e:	83 c4 48             	add    esp,0x48
  404741:	c2 0c 00             	ret    0xc
