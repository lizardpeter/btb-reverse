
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00406620 <.text+0x5620>:
  406620:	6a ff                	push   0xffffffff
  406622:	68 18 a5 43 00       	push   0x43a518
  406627:	64 a1 00 00 00 00    	mov    eax,fs:0x0
  40662d:	50                   	push   eax
  40662e:	64 89 25 00 00 00 00 	mov    DWORD PTR fs:0x0,esp
  406635:	51                   	push   ecx
  406636:	53                   	push   ebx
  406637:	55                   	push   ebp
  406638:	56                   	push   esi
  406639:	8b 74 24 20          	mov    esi,DWORD PTR [esp+0x20]
  40663d:	57                   	push   edi
  40663e:	8b f9                	mov    edi,ecx
  406640:	56                   	push   esi
  406641:	89 7c 24 14          	mov    DWORD PTR [esp+0x14],edi
  406645:	e8 ad 9d 02 00       	call   0x4303f7
  40664a:	8a 46 0c             	mov    al,BYTE PTR [esi+0xc]
  40664d:	83 c6 0c             	add    esi,0xc
  406650:	8d 6f 0c             	lea    ebp,[edi+0xc]
  406653:	33 d2                	xor    edx,edx
  406655:	89 54 24 1c          	mov    DWORD PTR [esp+0x1c],edx
  406659:	88 45 00             	mov    BYTE PTR [ebp+0x0],al
  40665c:	89 55 04             	mov    DWORD PTR [ebp+0x4],edx
  40665f:	89 55 08             	mov    DWORD PTR [ebp+0x8],edx
  406662:	89 55 0c             	mov    DWORD PTR [ebp+0xc],edx
  406665:	8b 46 08             	mov    eax,DWORD PTR [esi+0x8]
  406668:	8b 0d 34 b3 43 00    	mov    ecx,DWORD PTR ds:0x43b334
  40666e:	8b d8                	mov    ebx,eax
  406670:	3b cb                	cmp    ecx,ebx
  406672:	73 02                	jae    0x406676
  406674:	8b d9                	mov    ebx,ecx
  406676:	3b ee                	cmp    ebp,esi
  406678:	75 75                	jne    0x4066ef
  40667a:	3b da                	cmp    ebx,edx
  40667c:	8b f1                	mov    esi,ecx
  40667e:	76 05                	jbe    0x406685
  406680:	e8 70 3a 03 00       	call   0x43a0f5
  406685:	8b cd                	mov    ecx,ebp
  406687:	e8 14 02 00 00       	call   0x4068a0
  40668c:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  40668f:	2b c3                	sub    eax,ebx
  406691:	3b c6                	cmp    eax,esi
  406693:	73 02                	jae    0x406697
  406695:	8b f0                	mov    esi,eax
  406697:	85 f6                	test   esi,esi
  406699:	76 30                	jbe    0x4066cb
  40669b:	8b 4d 04             	mov    ecx,DWORD PTR [ebp+0x4]
  40669e:	2b c6                	sub    eax,esi
  4066a0:	03 d9                	add    ebx,ecx
  4066a2:	50                   	push   eax
  4066a3:	8d 14 33             	lea    edx,[ebx+esi*1]
  4066a6:	52                   	push   edx
  4066a7:	53                   	push   ebx
  4066a8:	e8 53 9e 02 00       	call   0x430500
  4066ad:	8b 5d 08             	mov    ebx,DWORD PTR [ebp+0x8]
  4066b0:	83 c4 0c             	add    esp,0xc
  4066b3:	2b de                	sub    ebx,esi
  4066b5:	8b cd                	mov    ecx,ebp
  4066b7:	6a 00                	push   0x0
  4066b9:	53                   	push   ebx
  4066ba:	e8 a1 fd ff ff       	call   0x406460
  4066bf:	84 c0                	test   al,al
  4066c1:	74 08                	je     0x4066cb
  4066c3:	53                   	push   ebx
  4066c4:	8b cd                	mov    ecx,ebp
  4066c6:	e8 75 fd ff ff       	call   0x406440
  4066cb:	8b cd                	mov    ecx,ebp
  4066cd:	e8 ce 01 00 00       	call   0x4068a0
  4066d2:	c7 07 28 b3 43 00    	mov    DWORD PTR [edi],0x43b328
  4066d8:	8b c7                	mov    eax,edi
  4066da:	8b 4c 24 14          	mov    ecx,DWORD PTR [esp+0x14]
  4066de:	5f                   	pop    edi
  4066df:	5e                   	pop    esi
  4066e0:	5d                   	pop    ebp
  4066e1:	5b                   	pop    ebx
  4066e2:	64 89 0d 00 00 00 00 	mov    DWORD PTR fs:0x0,ecx
  4066e9:	83 c4 10             	add    esp,0x10
  4066ec:	c2 04 00             	ret    0x4
  4066ef:	3b da                	cmp    ebx,edx
  4066f1:	76 44                	jbe    0x406737
  4066f3:	3b d8                	cmp    ebx,eax
  4066f5:	75 40                	jne    0x406737
  4066f7:	8b 46 04             	mov    eax,DWORD PTR [esi+0x4]
  4066fa:	3b c2                	cmp    eax,edx
  4066fc:	75 05                	jne    0x406703
  4066fe:	b8 58 b3 43 00       	mov    eax,0x43b358
  406703:	80 78 ff fe          	cmp    BYTE PTR [eax-0x1],0xfe
  406707:	73 2e                	jae    0x406737
  406709:	6a 01                	push   0x1
  40670b:	8b cd                	mov    ecx,ebp
  40670d:	e8 3e fc ff ff       	call   0x406350
  406712:	8b 46 04             	mov    eax,DWORD PTR [esi+0x4]
  406715:	85 c0                	test   eax,eax
  406717:	75 05                	jne    0x40671e
  406719:	b8 58 b3 43 00       	mov    eax,0x43b358
  40671e:	89 45 04             	mov    DWORD PTR [ebp+0x4],eax
  406721:	8b 4e 08             	mov    ecx,DWORD PTR [esi+0x8]
  406724:	89 4d 08             	mov    DWORD PTR [ebp+0x8],ecx
  406727:	8b 56 0c             	mov    edx,DWORD PTR [esi+0xc]
  40672a:	89 55 0c             	mov    DWORD PTR [ebp+0xc],edx
  40672d:	8a 48 ff             	mov    cl,BYTE PTR [eax-0x1]
  406730:	fe c1                	inc    cl
  406732:	88 48 ff             	mov    BYTE PTR [eax-0x1],cl
  406735:	eb 9b                	jmp    0x4066d2
  406737:	6a 01                	push   0x1
  406739:	53                   	push   ebx
  40673a:	8b cd                	mov    ecx,ebp
  40673c:	e8 1f fd ff ff       	call   0x406460
  406741:	84 c0                	test   al,al
  406743:	74 8d                	je     0x4066d2
  406745:	8b 76 04             	mov    esi,DWORD PTR [esi+0x4]
  406748:	85 f6                	test   esi,esi
  40674a:	75 05                	jne    0x406751
  40674c:	be 58 b3 43 00       	mov    esi,0x43b358
  406751:	8b 7d 04             	mov    edi,DWORD PTR [ebp+0x4]
  406754:	8b cb                	mov    ecx,ebx
  406756:	8b c1                	mov    eax,ecx
  406758:	c1 e9 02             	shr    ecx,0x2
  40675b:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  40675d:	8b c8                	mov    ecx,eax
  40675f:	8b 44 24 10          	mov    eax,DWORD PTR [esp+0x10]
  406763:	83 e1 03             	and    ecx,0x3
  406766:	f3 a4                	rep movs BYTE PTR es:[edi],BYTE PTR ds:[esi]
  406768:	8b 4d 04             	mov    ecx,DWORD PTR [ebp+0x4]
  40676b:	89 5d 08             	mov    DWORD PTR [ebp+0x8],ebx
  40676e:	c6 04 19 00          	mov    BYTE PTR [ecx+ebx*1],0x0
  406772:	c7 00 28 b3 43 00    	mov    DWORD PTR [eax],0x43b328
  406778:	e9 5d ff ff ff       	jmp    0x4066da
