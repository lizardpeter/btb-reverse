# source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05

verified retail PE32:     file format pei-i386


Disassembly of section .text:

004068a0 <.text+0x58a0>:
  4068a0:	53                   	push   ebx
  4068a1:	55                   	push   ebp
  4068a2:	8b e9                	mov    ebp,ecx
  4068a4:	33 d2                	xor    edx,edx
  4068a6:	56                   	push   esi
  4068a7:	57                   	push   edi
  4068a8:	8b 4d 04             	mov    ecx,DWORD PTR [ebp+0x4]
  4068ab:	3b ca                	cmp    ecx,edx
  4068ad:	0f 84 b1 00 00 00    	je     0x406964
  4068b3:	8a 41 ff             	mov    al,BYTE PTR [ecx-0x1]
  4068b6:	84 c0                	test   al,al
  4068b8:	0f 84 a6 00 00 00    	je     0x406964
  4068be:	3c ff                	cmp    al,0xff
  4068c0:	0f 84 9e 00 00 00    	je     0x406964
  4068c6:	fe c8                	dec    al
  4068c8:	8b f1                	mov    esi,ecx
  4068ca:	88 41 ff             	mov    BYTE PTR [ecx-0x1],al
  4068cd:	8b fe                	mov    edi,esi
  4068cf:	83 c9 ff             	or     ecx,0xffffffff
  4068d2:	33 c0                	xor    eax,eax
  4068d4:	89 55 04             	mov    DWORD PTR [ebp+0x4],edx
  4068d7:	89 55 08             	mov    DWORD PTR [ebp+0x8],edx
  4068da:	89 55 0c             	mov    DWORD PTR [ebp+0xc],edx
  4068dd:	f2 ae                	repnz scas al,BYTE PTR es:[edi]
  4068df:	f7 d1                	not    ecx
  4068e1:	49                   	dec    ecx
  4068e2:	8b d9                	mov    ebx,ecx
  4068e4:	83 fb fd             	cmp    ebx,0xfffffffd
  4068e7:	76 07                	jbe    0x4068f0
  4068e9:	e8 db 35 03 00       	call   0x439ec9
  4068ee:	33 d2                	xor    edx,edx
  4068f0:	8b 4d 04             	mov    ecx,DWORD PTR [ebp+0x4]
  4068f3:	3b ca                	cmp    ecx,edx
  4068f5:	74 21                	je     0x406918
  4068f7:	8a 41 ff             	mov    al,BYTE PTR [ecx-0x1]
  4068fa:	84 c0                	test   al,al
  4068fc:	74 1a                	je     0x406918
  4068fe:	3c ff                	cmp    al,0xff
  406900:	74 16                	je     0x406918
  406902:	3b da                	cmp    ebx,edx
  406904:	75 39                	jne    0x40693f
  406906:	fe c8                	dec    al
  406908:	52                   	push   edx
  406909:	88 41 ff             	mov    BYTE PTR [ecx-0x1],al
  40690c:	8b cd                	mov    ecx,ebp
  40690e:	e8 3d fa ff ff       	call   0x406350
  406913:	5f                   	pop    edi
  406914:	5e                   	pop    esi
  406915:	5d                   	pop    ebp
  406916:	5b                   	pop    ebx
  406917:	c3                   	ret
  406918:	3b da                	cmp    ebx,edx
  40691a:	75 0e                	jne    0x40692a
  40691c:	6a 01                	push   0x1
  40691e:	8b cd                	mov    ecx,ebp
  406920:	e8 2b fa ff ff       	call   0x406350
  406925:	5f                   	pop    edi
  406926:	5e                   	pop    esi
  406927:	5d                   	pop    ebp
  406928:	5b                   	pop    ebx
  406929:	c3                   	ret
  40692a:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  40692d:	83 f8 1f             	cmp    eax,0x1f
  406930:	77 04                	ja     0x406936
  406932:	3b c3                	cmp    eax,ebx
  406934:	73 11                	jae    0x406947
  406936:	6a 01                	push   0x1
  406938:	8b cd                	mov    ecx,ebp
  40693a:	e8 11 fa ff ff       	call   0x406350
  40693f:	53                   	push   ebx
  406940:	8b cd                	mov    ecx,ebp
  406942:	e8 49 fe ff ff       	call   0x406790
  406947:	8b 7d 04             	mov    edi,DWORD PTR [ebp+0x4]
  40694a:	8b cb                	mov    ecx,ebx
  40694c:	8b c1                	mov    eax,ecx
  40694e:	c1 e9 02             	shr    ecx,0x2
  406951:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  406953:	8b c8                	mov    ecx,eax
  406955:	83 e1 03             	and    ecx,0x3
  406958:	f3 a4                	rep movs BYTE PTR es:[edi],BYTE PTR ds:[esi]
  40695a:	8b 4d 04             	mov    ecx,DWORD PTR [ebp+0x4]
  40695d:	89 5d 08             	mov    DWORD PTR [ebp+0x8],ebx
  406960:	c6 04 0b 00          	mov    BYTE PTR [ebx+ecx*1],0x0
  406964:	5f                   	pop    edi
  406965:	5e                   	pop    esi
  406966:	5d                   	pop    ebp
  406967:	5b                   	pop    ebx
  406968:	c3                   	ret
  406969:	90                   	nop
  40696a:	90                   	nop
  40696b:	90                   	nop
  40696c:	90                   	nop
  40696d:	90                   	nop
  40696e:	90                   	nop
  40696f:	90                   	nop
