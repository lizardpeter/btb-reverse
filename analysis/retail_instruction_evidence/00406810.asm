# source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05

verified retail PE32:     file format pei-i386


Disassembly of section .text:

00406810 <.text+0x5810>:
  406810:	8b 43 08             	mov    eax,DWORD PTR [ebx+0x8]
  406813:	85 c0                	test   eax,eax
  406815:	76 22                	jbe    0x406839
  406817:	3b c6                	cmp    eax,esi
  406819:	76 02                	jbe    0x40681d
  40681b:	8b c6                	mov    eax,esi
  40681d:	8b 73 04             	mov    esi,DWORD PTR [ebx+0x4]
  406820:	8b c8                	mov    ecx,eax
  406822:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  406825:	8b d1                	mov    edx,ecx
  406827:	c1 e9 02             	shr    ecx,0x2
  40682a:	8d 78 01             	lea    edi,[eax+0x1]
  40682d:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  40682f:	8b ca                	mov    ecx,edx
  406831:	83 e1 03             	and    ecx,0x3
  406834:	f3 a4                	rep movs BYTE PTR es:[edi],BYTE PTR ds:[esi]
  406836:	8b 75 ec             	mov    esi,DWORD PTR [ebp-0x14]
  406839:	8b 43 04             	mov    eax,DWORD PTR [ebx+0x4]
  40683c:	8b 7b 08             	mov    edi,DWORD PTR [ebx+0x8]
  40683f:	85 c0                	test   eax,eax
  406841:	74 1d                	je     0x406860
  406843:	8d 48 ff             	lea    ecx,[eax-0x1]
  406846:	8a 40 ff             	mov    al,BYTE PTR [eax-0x1]
  406849:	84 c0                	test   al,al
  40684b:	74 0a                	je     0x406857
  40684d:	3c ff                	cmp    al,0xff
  40684f:	74 06                	je     0x406857
  406851:	fe c8                	dec    al
  406853:	88 01                	mov    BYTE PTR [ecx],al
  406855:	eb 09                	jmp    0x406860
  406857:	51                   	push   ecx
  406858:	e8 7f 93 02 00       	call   0x42fbdc
  40685d:	83 c4 04             	add    esp,0x4
  406860:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  406863:	c7 43 08 00 00 00 00 	mov    DWORD PTR [ebx+0x8],0x0
  40686a:	40                   	inc    eax
  40686b:	3b fe                	cmp    edi,esi
  40686d:	89 43 04             	mov    DWORD PTR [ebx+0x4],eax
  406870:	c6 40 ff 00          	mov    BYTE PTR [eax-0x1],0x0
  406874:	89 73 0c             	mov    DWORD PTR [ebx+0xc],esi
  406877:	77 02                	ja     0x40687b
  406879:	8b f7                	mov    esi,edi
  40687b:	8b 4b 04             	mov    ecx,DWORD PTR [ebx+0x4]
  40687e:	89 73 08             	mov    DWORD PTR [ebx+0x8],esi
  406881:	5f                   	pop    edi
  406882:	c6 04 31 00          	mov    BYTE PTR [ecx+esi*1],0x0
  406886:	8b 4d f4             	mov    ecx,DWORD PTR [ebp-0xc]
  406889:	5e                   	pop    esi
  40688a:	64 89 0d 00 00 00 00 	mov    DWORD PTR fs:0x0,ecx
  406891:	5b                   	pop    ebx
  406892:	8b e5                	mov    esp,ebp
  406894:	5d                   	pop    ebp
  406895:	c2 04 00             	ret    0x4
  406898:	90                   	nop
  406899:	90                   	nop
  40689a:	90                   	nop
  40689b:	90                   	nop
  40689c:	90                   	nop
  40689d:	90                   	nop
  40689e:	90                   	nop
  40689f:	90                   	nop
