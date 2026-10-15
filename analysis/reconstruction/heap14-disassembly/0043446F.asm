
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0043446f <.text+0x3346f>:
  43446f:	55                   	push   ebp
  434470:	8b ec                	mov    ebp,esp
  434472:	83 ec 10             	sub    esp,0x10
  434475:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  434478:	53                   	push   ebx
  434479:	56                   	push   esi
  43447a:	8b 75 0c             	mov    esi,DWORD PTR [ebp+0xc]
  43447d:	8b 41 10             	mov    eax,DWORD PTR [ecx+0x10]
  434480:	57                   	push   edi
  434481:	8b fe                	mov    edi,esi
  434483:	83 c6 fc             	add    esi,0xfffffffc
  434486:	2b 79 0c             	sub    edi,DWORD PTR [ecx+0xc]
  434489:	c1 ef 0f             	shr    edi,0xf
  43448c:	8b cf                	mov    ecx,edi
  43448e:	69 c9 04 02 00 00    	imul   ecx,ecx,0x204
  434494:	8d 8c 01 44 01 00 00 	lea    ecx,[ecx+eax*1+0x144]
  43449b:	89 4d f0             	mov    DWORD PTR [ebp-0x10],ecx
  43449e:	8b 0e                	mov    ecx,DWORD PTR [esi]
  4344a0:	49                   	dec    ecx
  4344a1:	f6 c1 01             	test   cl,0x1
  4344a4:	89 4d fc             	mov    DWORD PTR [ebp-0x4],ecx
  4344a7:	0f 85 e6 02 00 00    	jne    0x434793
  4344ad:	8b 14 31             	mov    edx,DWORD PTR [ecx+esi*1]
  4344b0:	8d 1c 31             	lea    ebx,[ecx+esi*1]
  4344b3:	89 55 f4             	mov    DWORD PTR [ebp-0xc],edx
  4344b6:	8b 56 fc             	mov    edx,DWORD PTR [esi-0x4]
  4344b9:	89 55 f8             	mov    DWORD PTR [ebp-0x8],edx
  4344bc:	8b 55 f4             	mov    edx,DWORD PTR [ebp-0xc]
  4344bf:	f6 c2 01             	test   dl,0x1
  4344c2:	89 5d 0c             	mov    DWORD PTR [ebp+0xc],ebx
  4344c5:	75 7e                	jne    0x434545
  4344c7:	c1 fa 04             	sar    edx,0x4
  4344ca:	4a                   	dec    edx
  4344cb:	83 fa 3f             	cmp    edx,0x3f
  4344ce:	76 03                	jbe    0x4344d3
  4344d0:	6a 3f                	push   0x3f
  4344d2:	5a                   	pop    edx
  4344d3:	8b 4b 04             	mov    ecx,DWORD PTR [ebx+0x4]
  4344d6:	3b 4b 08             	cmp    ecx,DWORD PTR [ebx+0x8]
  4344d9:	75 4c                	jne    0x434527
  4344db:	83 fa 20             	cmp    edx,0x20
  4344de:	73 1e                	jae    0x4344fe
  4344e0:	bb 00 00 00 80       	mov    ebx,0x80000000
  4344e5:	8b ca                	mov    ecx,edx
  4344e7:	d3 eb                	shr    ebx,cl
  4344e9:	8d 4c 02 04          	lea    ecx,[edx+eax*1+0x4]
  4344ed:	f7 d3                	not    ebx
  4344ef:	21 5c b8 44          	and    DWORD PTR [eax+edi*4+0x44],ebx
  4344f3:	fe 09                	dec    BYTE PTR [ecx]
  4344f5:	75 28                	jne    0x43451f
  4344f7:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  4344fa:	21 19                	and    DWORD PTR [ecx],ebx
  4344fc:	eb 21                	jmp    0x43451f
  4344fe:	8d 4a e0             	lea    ecx,[edx-0x20]
  434501:	bb 00 00 00 80       	mov    ebx,0x80000000
  434506:	d3 eb                	shr    ebx,cl
  434508:	8d 4c 02 04          	lea    ecx,[edx+eax*1+0x4]
  43450c:	f7 d3                	not    ebx
  43450e:	21 9c b8 c4 00 00 00 	and    DWORD PTR [eax+edi*4+0xc4],ebx
  434515:	fe 09                	dec    BYTE PTR [ecx]
  434517:	75 06                	jne    0x43451f
  434519:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  43451c:	21 59 04             	and    DWORD PTR [ecx+0x4],ebx
  43451f:	8b 4d fc             	mov    ecx,DWORD PTR [ebp-0x4]
  434522:	8b 5d 0c             	mov    ebx,DWORD PTR [ebp+0xc]
  434525:	eb 03                	jmp    0x43452a
  434527:	8b 4d fc             	mov    ecx,DWORD PTR [ebp-0x4]
  43452a:	8b 53 08             	mov    edx,DWORD PTR [ebx+0x8]
  43452d:	8b 5b 04             	mov    ebx,DWORD PTR [ebx+0x4]
  434530:	03 4d f4             	add    ecx,DWORD PTR [ebp-0xc]
  434533:	89 5a 04             	mov    DWORD PTR [edx+0x4],ebx
  434536:	8b 55 0c             	mov    edx,DWORD PTR [ebp+0xc]
  434539:	89 4d fc             	mov    DWORD PTR [ebp-0x4],ecx
  43453c:	8b 5a 04             	mov    ebx,DWORD PTR [edx+0x4]
  43453f:	8b 52 08             	mov    edx,DWORD PTR [edx+0x8]
  434542:	89 53 08             	mov    DWORD PTR [ebx+0x8],edx
  434545:	8b d1                	mov    edx,ecx
  434547:	c1 fa 04             	sar    edx,0x4
  43454a:	4a                   	dec    edx
  43454b:	83 fa 3f             	cmp    edx,0x3f
  43454e:	76 03                	jbe    0x434553
  434550:	6a 3f                	push   0x3f
  434552:	5a                   	pop    edx
  434553:	8b 5d f8             	mov    ebx,DWORD PTR [ebp-0x8]
  434556:	83 e3 01             	and    ebx,0x1
  434559:	89 5d f4             	mov    DWORD PTR [ebp-0xc],ebx
  43455c:	0f 85 94 00 00 00    	jne    0x4345f6
  434562:	2b 75 f8             	sub    esi,DWORD PTR [ebp-0x8]
  434565:	8b 5d f8             	mov    ebx,DWORD PTR [ebp-0x8]
  434568:	c1 fb 04             	sar    ebx,0x4
  43456b:	6a 3f                	push   0x3f
  43456d:	89 75 0c             	mov    DWORD PTR [ebp+0xc],esi
  434570:	4b                   	dec    ebx
  434571:	5e                   	pop    esi
  434572:	3b de                	cmp    ebx,esi
  434574:	76 02                	jbe    0x434578
  434576:	8b de                	mov    ebx,esi
  434578:	03 4d f8             	add    ecx,DWORD PTR [ebp-0x8]
  43457b:	8b d1                	mov    edx,ecx
  43457d:	89 4d fc             	mov    DWORD PTR [ebp-0x4],ecx
  434580:	c1 fa 04             	sar    edx,0x4
  434583:	4a                   	dec    edx
  434584:	3b d6                	cmp    edx,esi
  434586:	76 02                	jbe    0x43458a
  434588:	8b d6                	mov    edx,esi
  43458a:	3b da                	cmp    ebx,edx
  43458c:	74 63                	je     0x4345f1
  43458e:	8b 4d 0c             	mov    ecx,DWORD PTR [ebp+0xc]
  434591:	8b 71 04             	mov    esi,DWORD PTR [ecx+0x4]
  434594:	3b 71 08             	cmp    esi,DWORD PTR [ecx+0x8]
  434597:	75 40                	jne    0x4345d9
  434599:	83 fb 20             	cmp    ebx,0x20
  43459c:	73 1c                	jae    0x4345ba
  43459e:	be 00 00 00 80       	mov    esi,0x80000000
  4345a3:	8b cb                	mov    ecx,ebx
  4345a5:	d3 ee                	shr    esi,cl
  4345a7:	f7 d6                	not    esi
  4345a9:	21 74 b8 44          	and    DWORD PTR [eax+edi*4+0x44],esi
  4345ad:	fe 4c 03 04          	dec    BYTE PTR [ebx+eax*1+0x4]
  4345b1:	75 26                	jne    0x4345d9
  4345b3:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  4345b6:	21 31                	and    DWORD PTR [ecx],esi
  4345b8:	eb 1f                	jmp    0x4345d9
  4345ba:	8d 4b e0             	lea    ecx,[ebx-0x20]
  4345bd:	be 00 00 00 80       	mov    esi,0x80000000
  4345c2:	d3 ee                	shr    esi,cl
  4345c4:	f7 d6                	not    esi
  4345c6:	21 b4 b8 c4 00 00 00 	and    DWORD PTR [eax+edi*4+0xc4],esi
  4345cd:	fe 4c 03 04          	dec    BYTE PTR [ebx+eax*1+0x4]
  4345d1:	75 06                	jne    0x4345d9
  4345d3:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  4345d6:	21 71 04             	and    DWORD PTR [ecx+0x4],esi
  4345d9:	8b 4d 0c             	mov    ecx,DWORD PTR [ebp+0xc]
  4345dc:	8b 71 08             	mov    esi,DWORD PTR [ecx+0x8]
  4345df:	8b 49 04             	mov    ecx,DWORD PTR [ecx+0x4]
  4345e2:	89 4e 04             	mov    DWORD PTR [esi+0x4],ecx
  4345e5:	8b 4d 0c             	mov    ecx,DWORD PTR [ebp+0xc]
  4345e8:	8b 71 04             	mov    esi,DWORD PTR [ecx+0x4]
  4345eb:	8b 49 08             	mov    ecx,DWORD PTR [ecx+0x8]
  4345ee:	89 4e 08             	mov    DWORD PTR [esi+0x8],ecx
  4345f1:	8b 75 0c             	mov    esi,DWORD PTR [ebp+0xc]
  4345f4:	eb 03                	jmp    0x4345f9
  4345f6:	8b 5d 08             	mov    ebx,DWORD PTR [ebp+0x8]
  4345f9:	83 7d f4 00          	cmp    DWORD PTR [ebp-0xc],0x0
  4345fd:	75 08                	jne    0x434607
  4345ff:	3b da                	cmp    ebx,edx
  434601:	0f 84 81 00 00 00    	je     0x434688
  434607:	8b 4d f0             	mov    ecx,DWORD PTR [ebp-0x10]
  43460a:	8b 5c d1 04          	mov    ebx,DWORD PTR [ecx+edx*8+0x4]
  43460e:	8d 0c d1             	lea    ecx,[ecx+edx*8]
  434611:	89 5e 04             	mov    DWORD PTR [esi+0x4],ebx
  434614:	89 4e 08             	mov    DWORD PTR [esi+0x8],ecx
  434617:	89 71 04             	mov    DWORD PTR [ecx+0x4],esi
  43461a:	8b 4e 04             	mov    ecx,DWORD PTR [esi+0x4]
  43461d:	89 71 08             	mov    DWORD PTR [ecx+0x8],esi
  434620:	8b 4e 04             	mov    ecx,DWORD PTR [esi+0x4]
  434623:	3b 4e 08             	cmp    ecx,DWORD PTR [esi+0x8]
  434626:	75 60                	jne    0x434688
  434628:	8a 4c 02 04          	mov    cl,BYTE PTR [edx+eax*1+0x4]
  43462c:	83 fa 20             	cmp    edx,0x20
  43462f:	88 4d 0f             	mov    BYTE PTR [ebp+0xf],cl
  434632:	fe c1                	inc    cl
  434634:	88 4c 02 04          	mov    BYTE PTR [edx+eax*1+0x4],cl
  434638:	73 25                	jae    0x43465f
  43463a:	80 7d 0f 00          	cmp    BYTE PTR [ebp+0xf],0x0
  43463e:	75 0e                	jne    0x43464e
  434640:	bb 00 00 00 80       	mov    ebx,0x80000000
  434645:	8b ca                	mov    ecx,edx
  434647:	d3 eb                	shr    ebx,cl
  434649:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  43464c:	09 19                	or     DWORD PTR [ecx],ebx
  43464e:	bb 00 00 00 80       	mov    ebx,0x80000000
  434653:	8b ca                	mov    ecx,edx
  434655:	d3 eb                	shr    ebx,cl
  434657:	8d 44 b8 44          	lea    eax,[eax+edi*4+0x44]
  43465b:	09 18                	or     DWORD PTR [eax],ebx
  43465d:	eb 29                	jmp    0x434688
  43465f:	80 7d 0f 00          	cmp    BYTE PTR [ebp+0xf],0x0
  434663:	75 10                	jne    0x434675
  434665:	8d 4a e0             	lea    ecx,[edx-0x20]
  434668:	bb 00 00 00 80       	mov    ebx,0x80000000
  43466d:	d3 eb                	shr    ebx,cl
  43466f:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  434672:	09 59 04             	or     DWORD PTR [ecx+0x4],ebx
  434675:	8d 4a e0             	lea    ecx,[edx-0x20]
  434678:	ba 00 00 00 80       	mov    edx,0x80000000
  43467d:	d3 ea                	shr    edx,cl
  43467f:	8d 84 b8 c4 00 00 00 	lea    eax,[eax+edi*4+0xc4]
  434686:	09 10                	or     DWORD PTR [eax],edx
  434688:	8b 45 fc             	mov    eax,DWORD PTR [ebp-0x4]
  43468b:	89 06                	mov    DWORD PTR [esi],eax
  43468d:	89 44 30 fc          	mov    DWORD PTR [eax+esi*1-0x4],eax
  434691:	8b 45 f0             	mov    eax,DWORD PTR [ebp-0x10]
  434694:	ff 08                	dec    DWORD PTR [eax]
  434696:	0f 85 f7 00 00 00    	jne    0x434793
  43469c:	a1 30 da 51 00       	mov    eax,ds:0x51da30
  4346a1:	85 c0                	test   eax,eax
  4346a3:	0f 84 dc 00 00 00    	je     0x434785
  4346a9:	8b 0d 28 da 51 00    	mov    ecx,DWORD PTR ds:0x51da28
  4346af:	8b 35 dc b0 43 00    	mov    esi,DWORD PTR ds:0x43b0dc
  4346b5:	c1 e1 0f             	shl    ecx,0xf
  4346b8:	03 48 0c             	add    ecx,DWORD PTR [eax+0xc]
  4346bb:	bb 00 80 00 00       	mov    ebx,0x8000
  4346c0:	68 00 40 00 00       	push   0x4000
  4346c5:	53                   	push   ebx
  4346c6:	51                   	push   ecx
  4346c7:	ff d6                	call   esi
  4346c9:	8b 0d 28 da 51 00    	mov    ecx,DWORD PTR ds:0x51da28
  4346cf:	a1 30 da 51 00       	mov    eax,ds:0x51da30
  4346d4:	ba 00 00 00 80       	mov    edx,0x80000000
  4346d9:	d3 ea                	shr    edx,cl
  4346db:	09 50 08             	or     DWORD PTR [eax+0x8],edx
  4346de:	a1 30 da 51 00       	mov    eax,ds:0x51da30
  4346e3:	8b 0d 28 da 51 00    	mov    ecx,DWORD PTR ds:0x51da28
  4346e9:	8b 40 10             	mov    eax,DWORD PTR [eax+0x10]
  4346ec:	83 a4 88 c4 00 00 00 	and    DWORD PTR [eax+ecx*4+0xc4],0x0
  4346f3:	00 
  4346f4:	a1 30 da 51 00       	mov    eax,ds:0x51da30
  4346f9:	8b 40 10             	mov    eax,DWORD PTR [eax+0x10]
  4346fc:	fe 48 43             	dec    BYTE PTR [eax+0x43]
  4346ff:	a1 30 da 51 00       	mov    eax,ds:0x51da30
  434704:	8b 48 10             	mov    ecx,DWORD PTR [eax+0x10]
  434707:	80 79 43 00          	cmp    BYTE PTR [ecx+0x43],0x0
  43470b:	75 09                	jne    0x434716
  43470d:	83 60 04 fe          	and    DWORD PTR [eax+0x4],0xfffffffe
  434711:	a1 30 da 51 00       	mov    eax,ds:0x51da30
  434716:	83 78 08 ff          	cmp    DWORD PTR [eax+0x8],0xffffffff
  43471a:	75 69                	jne    0x434785
  43471c:	53                   	push   ebx
  43471d:	6a 00                	push   0x0
  43471f:	ff 70 0c             	push   DWORD PTR [eax+0xc]
  434722:	ff d6                	call   esi
  434724:	a1 30 da 51 00       	mov    eax,ds:0x51da30
  434729:	ff 70 10             	push   DWORD PTR [eax+0x10]
  43472c:	6a 00                	push   0x0
  43472e:	ff 35 40 da 51 00    	push   DWORD PTR ds:0x51da40
  434734:	ff 15 0c b1 43 00    	call   DWORD PTR ds:0x43b10c
  43473a:	a1 34 da 51 00       	mov    eax,ds:0x51da34
  43473f:	8b 15 38 da 51 00    	mov    edx,DWORD PTR ds:0x51da38
  434745:	8d 04 80             	lea    eax,[eax+eax*4]
  434748:	c1 e0 02             	shl    eax,0x2
  43474b:	8b c8                	mov    ecx,eax
  43474d:	a1 30 da 51 00       	mov    eax,ds:0x51da30
  434752:	2b c8                	sub    ecx,eax
  434754:	8d 4c 11 ec          	lea    ecx,[ecx+edx*1-0x14]
  434758:	51                   	push   ecx
  434759:	8d 48 14             	lea    ecx,[eax+0x14]
  43475c:	51                   	push   ecx
  43475d:	50                   	push   eax
  43475e:	e8 9d bd ff ff       	call   0x430500
  434763:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  434766:	83 c4 0c             	add    esp,0xc
  434769:	ff 0d 34 da 51 00    	dec    DWORD PTR ds:0x51da34
  43476f:	3b 05 30 da 51 00    	cmp    eax,DWORD PTR ds:0x51da30
  434775:	76 04                	jbe    0x43477b
  434777:	83 6d 08 14          	sub    DWORD PTR [ebp+0x8],0x14
  43477b:	a1 38 da 51 00       	mov    eax,ds:0x51da38
  434780:	a3 2c da 51 00       	mov    ds:0x51da2c,eax
  434785:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  434788:	89 3d 28 da 51 00    	mov    DWORD PTR ds:0x51da28,edi
  43478e:	a3 30 da 51 00       	mov    ds:0x51da30,eax
  434793:	5f                   	pop    edi
  434794:	5e                   	pop    esi
  434795:	5b                   	pop    ebx
  434796:	c9                   	leave
  434797:	c3                   	ret
