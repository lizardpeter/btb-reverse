
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004126f0 <.text+0x116f0>:
  4126f0:	51                   	push   ecx
  4126f1:	83 3d bc a5 50 00 01 	cmp    DWORD PTR ds:0x50a5bc,0x1
  4126f8:	75 21                	jne    0x41271b
  4126fa:	8b 15 30 bd 4f 00    	mov    edx,DWORD PTR ds:0x4fbd30
  412700:	8b 0d 24 bd 4f 00    	mov    ecx,DWORD PTR ds:0x4fbd24
  412706:	b8 0a 00 00 00       	mov    eax,0xa
  41270b:	03 d0                	add    edx,eax
  41270d:	03 c8                	add    ecx,eax
  41270f:	89 15 30 bd 4f 00    	mov    DWORD PTR ds:0x4fbd30,edx
  412715:	89 0d 24 bd 4f 00    	mov    DWORD PTR ds:0x4fbd24,ecx
  41271b:	a1 78 93 50 00       	mov    eax,ds:0x509378
  412720:	53                   	push   ebx
  412721:	55                   	push   ebp
  412722:	56                   	push   esi
  412723:	33 f6                	xor    esi,esi
  412725:	57                   	push   edi
  412726:	85 c0                	test   eax,eax
  412728:	0f 8e 9f 03 00 00    	jle    0x412acd
  41272e:	8b 1d 24 ab 50 00    	mov    ebx,DWORD PTR ds:0x50ab24
  412734:	8b 3d 20 ab 50 00    	mov    edi,DWORD PTR ds:0x50ab20
  41273a:	bd c8 a6 50 00       	mov    ebp,0x50a6c8
  41273f:	89 6c 24 10          	mov    DWORD PTR [esp+0x10],ebp
  412743:	8b 55 08             	mov    edx,DWORD PTR [ebp+0x8]
  412746:	83 fa ff             	cmp    edx,0xffffffff
  412749:	0f 84 ae 01 00 00    	je     0x4128fd
  41274f:	81 fd 30 a8 50 00    	cmp    ebp,0x50a830
  412755:	b8 0f 00 00 00       	mov    eax,0xf
  41275a:	b9 32 00 00 00       	mov    ecx,0x32
  41275f:	7c 04                	jl     0x412765
  412761:	33 c0                	xor    eax,eax
  412763:	eb 0d                	jmp    0x412772
  412765:	85 ff                	test   edi,edi
  412767:	75 0b                	jne    0x412774
  412769:	85 db                	test   ebx,ebx
  41276b:	75 07                	jne    0x412774
  41276d:	b8 f0 ff ff ff       	mov    eax,0xfffffff0
  412772:	33 c9                	xor    ecx,ecx
  412774:	8b 2d 24 bd 4f 00    	mov    ebp,DWORD PTR ds:0x4fbd24
  41277a:	03 c5                	add    eax,ebp
  41277c:	8b 6c 24 10          	mov    ebp,DWORD PTR [esp+0x10]
  412780:	3b 45 f8             	cmp    eax,DWORD PTR [ebp-0x8]
  412783:	0f 8e 74 01 00 00    	jle    0x4128fd
  412789:	3b 45 00             	cmp    eax,DWORD PTR [ebp+0x0]
  41278c:	0f 8d 6b 01 00 00    	jge    0x4128fd
  412792:	a1 30 bd 4f 00       	mov    eax,ds:0x4fbd30
  412797:	03 c1                	add    eax,ecx
  412799:	8b 4d fc             	mov    ecx,DWORD PTR [ebp-0x4]
  41279c:	3b c1                	cmp    eax,ecx
  41279e:	0f 8e 59 01 00 00    	jle    0x4128fd
  4127a4:	3b 45 04             	cmp    eax,DWORD PTR [ebp+0x4]
  4127a7:	0f 8d 50 01 00 00    	jge    0x4128fd
  4127ad:	83 3d bc a5 50 00 0d 	cmp    DWORD PTR ds:0x50a5bc,0xd
  4127b4:	0f 84 10 02 00 00    	je     0x4129ca
  4127ba:	81 fd b8 a7 50 00    	cmp    ebp,0x50a7b8
  4127c0:	7d 70                	jge    0x412832
  4127c2:	83 ff 01             	cmp    edi,0x1
  4127c5:	0f 84 4c 01 00 00    	je     0x412917
  4127cb:	83 fb 01             	cmp    ebx,0x1
  4127ce:	75 1c                	jne    0x4127ec
  4127d0:	6a 02                	push   0x2
  4127d2:	6a 32                	push   0x32
  4127d4:	e8 eb d7 01 00       	call   0x42ffc4
  4127d9:	25 01 00 00 80       	and    eax,0x80000001
  4127de:	79 05                	jns    0x4127e5
  4127e0:	48                   	dec    eax
  4127e1:	83 c8 fe             	or     eax,0xfffffffe
  4127e4:	40                   	inc    eax
  4127e5:	05 92 03 00 00       	add    eax,0x392
  4127ea:	eb 7a                	jmp    0x412866
  4127ec:	b8 ab aa aa 2a       	mov    eax,0x2aaaaaab
  4127f1:	bf 06 00 00 00       	mov    edi,0x6
  4127f6:	f7 ee                	imul   esi
  4127f8:	8b ca                	mov    ecx,edx
  4127fa:	8b c6                	mov    eax,esi
  4127fc:	c1 e9 1f             	shr    ecx,0x1f
  4127ff:	03 d1                	add    edx,ecx
  412801:	8d 0c 52             	lea    ecx,[edx+edx*2]
  412804:	99                   	cdq
  412805:	f7 ff                	idiv   edi
  412807:	8d 14 4a             	lea    edx,[edx+ecx*2]
  41280a:	83 3c 95 78 a6 50 00 	cmp    DWORD PTR [edx*4+0x50a678],0xffffffff
  412811:	ff 
  412812:	0f 85 15 01 00 00    	jne    0x41292d
  412818:	6a 02                	push   0x2
  41281a:	6a 32                	push   0x32
  41281c:	e8 a3 d7 01 00       	call   0x42ffc4
  412821:	99                   	cdq
  412822:	b9 03 00 00 00       	mov    ecx,0x3
  412827:	f7 f9                	idiv   ecx
  412829:	81 c2 67 03 00 00    	add    edx,0x367
  41282f:	52                   	push   edx
  412830:	eb 35                	jmp    0x412867
  412832:	81 fd 30 a8 50 00    	cmp    ebp,0x50a830
  412838:	0f 8d 9b 00 00 00    	jge    0x4128d9
  41283e:	83 fb 01             	cmp    ebx,0x1
  412841:	0f 84 83 01 00 00    	je     0x4129ca
  412847:	83 ff 01             	cmp    edi,0x1
  41284a:	6a 02                	push   0x2
  41284c:	6a 32                	push   0x32
  41284e:	75 30                	jne    0x412880
  412850:	e8 6f d7 01 00       	call   0x42ffc4
  412855:	25 01 00 00 80       	and    eax,0x80000001
  41285a:	79 05                	jns    0x412861
  41285c:	48                   	dec    eax
  41285d:	83 c8 fe             	or     eax,0xfffffffe
  412860:	40                   	inc    eax
  412861:	05 6e 03 00 00       	add    eax,0x36e
  412866:	50                   	push   eax
  412867:	8b 0d d8 dd 44 00    	mov    ecx,DWORD PTR ds:0x44ddd8
  41286d:	e8 7e 04 ff ff       	call   0x402cf0
  412872:	8b 1d 24 ab 50 00    	mov    ebx,DWORD PTR ds:0x50ab24
  412878:	8b 3d 20 ab 50 00    	mov    edi,DWORD PTR ds:0x50ab20
  41287e:	eb 7d                	jmp    0x4128fd
  412880:	e8 3f d7 01 00       	call   0x42ffc4
  412885:	99                   	cdq
  412886:	b9 03 00 00 00       	mov    ecx,0x3
  41288b:	f7 f9                	idiv   ecx
  41288d:	8b 0d d8 dd 44 00    	mov    ecx,DWORD PTR ds:0x44ddd8
  412893:	81 c2 8b 03 00 00    	add    edx,0x38b
  412899:	52                   	push   edx
  41289a:	e8 51 04 ff ff       	call   0x402cf0
  41289f:	b8 ab aa aa 2a       	mov    eax,0x2aaaaaab
  4128a4:	bf 06 00 00 00       	mov    edi,0x6
  4128a9:	f7 ee                	imul   esi
  4128ab:	8b c2                	mov    eax,edx
  4128ad:	c1 e8 1f             	shr    eax,0x1f
  4128b0:	03 d0                	add    edx,eax
  4128b2:	8b c6                	mov    eax,esi
  4128b4:	8d 0c 52             	lea    ecx,[edx+edx*2]
  4128b7:	99                   	cdq
  4128b8:	f7 ff                	idiv   edi
  4128ba:	8d 14 4a             	lea    edx,[edx+ecx*2]
  4128bd:	83 3c 95 78 a6 50 00 	cmp    DWORD PTR [edx*4+0x50a678],0xffffffff
  4128c4:	ff 
  4128c5:	0f 85 15 01 00 00    	jne    0x4129e0
  4128cb:	8b 1d 24 ab 50 00    	mov    ebx,DWORD PTR ds:0x50ab24
  4128d1:	8b 3d 20 ab 50 00    	mov    edi,DWORD PTR ds:0x50ab20
  4128d7:	eb 24                	jmp    0x4128fd
  4128d9:	83 fa 08             	cmp    edx,0x8
  4128dc:	7d 0e                	jge    0x4128ec
  4128de:	85 ff                	test   edi,edi
  4128e0:	75 1b                	jne    0x4128fd
  4128e2:	85 db                	test   ebx,ebx
  4128e4:	0f 84 8f 01 00 00    	je     0x412a79
  4128ea:	eb 11                	jmp    0x4128fd
  4128ec:	83 fa 0c             	cmp    edx,0xc
  4128ef:	7d 0c                	jge    0x4128fd
  4128f1:	85 db                	test   ebx,ebx
  4128f3:	75 08                	jne    0x4128fd
  4128f5:	85 ff                	test   edi,edi
  4128f7:	0f 84 95 01 00 00    	je     0x412a92
  4128fd:	a1 78 93 50 00       	mov    eax,ds:0x509378
  412902:	46                   	inc    esi
  412903:	83 c5 14             	add    ebp,0x14
  412906:	3b f0                	cmp    esi,eax
  412908:	89 6c 24 10          	mov    DWORD PTR [esp+0x10],ebp
  41290c:	0f 8c 31 fe ff ff    	jl     0x412743
  412912:	e9 c2 01 00 00       	jmp    0x412ad9
  412917:	89 35 c0 a5 50 00    	mov    DWORD PTR ds:0x50a5c0,esi
  41291d:	8d 0c b6             	lea    ecx,[esi+esi*4]
  412920:	5f                   	pop    edi
  412921:	5e                   	pop    esi
  412922:	8b 04 8d d0 a6 50 00 	mov    eax,DWORD PTR [ecx*4+0x50a6d0]
  412929:	5d                   	pop    ebp
  41292a:	5b                   	pop    ebx
  41292b:	59                   	pop    ecx
  41292c:	c3                   	ret
  41292d:	b8 ab aa aa 2a       	mov    eax,0x2aaaaaab
  412932:	bb 06 00 00 00       	mov    ebx,0x6
  412937:	f7 ee                	imul   esi
  412939:	8b c2                	mov    eax,edx
  41293b:	c1 e8 1f             	shr    eax,0x1f
  41293e:	03 d0                	add    edx,eax
  412940:	8b c6                	mov    eax,esi
  412942:	8b fa                	mov    edi,edx
  412944:	99                   	cdq
  412945:	f7 fb                	idiv   ebx
  412947:	8d 0c 7f             	lea    ecx,[edi+edi*2]
  41294a:	8d 1c 4a             	lea    ebx,[edx+ecx*2]
  41294d:	8b 04 9d 78 a6 50 00 	mov    eax,DWORD PTR [ebx*4+0x50a678]
  412954:	a3 d8 93 50 00       	mov    ds:0x5093d8,eax
  412959:	a3 c0 a5 50 00       	mov    ds:0x50a5c0,eax
  41295e:	8b 14 85 b8 a4 50 00 	mov    edx,DWORD PTR [eax*4+0x50a4b8]
  412965:	52                   	push   edx
  412966:	e8 85 51 ff ff       	call   0x407af0
  41296b:	83 c4 04             	add    esp,0x4
  41296e:	83 ff 02             	cmp    edi,0x2
  412971:	7d 0c                	jge    0x41297f
  412973:	c7 05 20 ab 50 00 01 	mov    DWORD PTR ds:0x50ab20,0x1
  41297a:	00 00 00 
  41297d:	eb 0a                	jmp    0x412989
  41297f:	c7 05 24 ab 50 00 01 	mov    DWORD PTR ds:0x50ab24,0x1
  412986:	00 00 00 
  412989:	8b 04 9d 78 a6 50 00 	mov    eax,DWORD PTR [ebx*4+0x50a678]
  412990:	5f                   	pop    edi
  412991:	a3 d8 93 50 00       	mov    ds:0x5093d8,eax
  412996:	8d 04 b6             	lea    eax,[esi+esi*4]
  412999:	c1 e0 02             	shl    eax,0x2
  41299c:	5e                   	pop    esi
  41299d:	c7 04 9d 78 a6 50 00 	mov    DWORD PTR [ebx*4+0x50a678],0xffffffff
  4129a4:	ff ff ff ff 
  4129a8:	8b 90 c4 a6 50 00    	mov    edx,DWORD PTR [eax+0x50a6c4]
  4129ae:	8b 88 c0 a6 50 00    	mov    ecx,DWORD PTR [eax+0x50a6c0]
  4129b4:	83 ea 19             	sub    edx,0x19
  4129b7:	5d                   	pop    ebp
  4129b8:	89 0d 24 bd 4f 00    	mov    DWORD PTR ds:0x4fbd24,ecx
  4129be:	89 15 30 bd 4f 00    	mov    DWORD PTR ds:0x4fbd30,edx
  4129c4:	83 c8 ff             	or     eax,0xffffffff
  4129c7:	5b                   	pop    ebx
  4129c8:	59                   	pop    ecx
  4129c9:	c3                   	ret
  4129ca:	89 35 c0 a5 50 00    	mov    DWORD PTR ds:0x50a5c0,esi
  4129d0:	8d 04 b6             	lea    eax,[esi+esi*4]
  4129d3:	5f                   	pop    edi
  4129d4:	5e                   	pop    esi
  4129d5:	8b 04 85 d0 a6 50 00 	mov    eax,DWORD PTR [eax*4+0x50a6d0]
  4129dc:	5d                   	pop    ebp
  4129dd:	5b                   	pop    ebx
  4129de:	59                   	pop    ecx
  4129df:	c3                   	ret
  4129e0:	8d 04 b6             	lea    eax,[esi+esi*4]
  4129e3:	c1 e0 02             	shl    eax,0x2
  4129e6:	8b 90 c4 a6 50 00    	mov    edx,DWORD PTR [eax+0x50a6c4]
  4129ec:	8b 88 c0 a6 50 00    	mov    ecx,DWORD PTR [eax+0x50a6c0]
  4129f2:	83 ea 19             	sub    edx,0x19
  4129f5:	b8 ab aa aa 2a       	mov    eax,0x2aaaaaab
  4129fa:	89 15 30 bd 4f 00    	mov    DWORD PTR ds:0x4fbd30,edx
  412a00:	f7 ee                	imul   esi
  412a02:	8b c2                	mov    eax,edx
  412a04:	89 0d 24 bd 4f 00    	mov    DWORD PTR ds:0x4fbd24,ecx
  412a0a:	c1 e8 1f             	shr    eax,0x1f
  412a0d:	03 d0                	add    edx,eax
  412a0f:	8b c6                	mov    eax,esi
  412a11:	8b fa                	mov    edi,edx
  412a13:	be 06 00 00 00       	mov    esi,0x6
  412a18:	99                   	cdq
  412a19:	f7 fe                	idiv   esi
  412a1b:	8d 0c 7f             	lea    ecx,[edi+edi*2]
  412a1e:	8d 34 4a             	lea    esi,[edx+ecx*2]
  412a21:	8b 04 b5 78 a6 50 00 	mov    eax,DWORD PTR [esi*4+0x50a678]
  412a28:	a3 d8 93 50 00       	mov    ds:0x5093d8,eax
  412a2d:	a3 c0 a5 50 00       	mov    ds:0x50a5c0,eax
  412a32:	8b 14 85 b8 a4 50 00 	mov    edx,DWORD PTR [eax*4+0x50a4b8]
  412a39:	52                   	push   edx
  412a3a:	e8 b1 50 ff ff       	call   0x407af0
  412a3f:	83 c4 04             	add    esp,0x4
  412a42:	83 ff 02             	cmp    edi,0x2
  412a45:	7d 0c                	jge    0x412a53
  412a47:	c7 05 20 ab 50 00 01 	mov    DWORD PTR ds:0x50ab20,0x1
  412a4e:	00 00 00 
  412a51:	eb 0a                	jmp    0x412a5d
  412a53:	c7 05 24 ab 50 00 01 	mov    DWORD PTR ds:0x50ab24,0x1
  412a5a:	00 00 00 
  412a5d:	8b 04 b5 78 a6 50 00 	mov    eax,DWORD PTR [esi*4+0x50a678]
  412a64:	5f                   	pop    edi
  412a65:	a3 d8 93 50 00       	mov    ds:0x5093d8,eax
  412a6a:	83 c8 ff             	or     eax,0xffffffff
  412a6d:	89 04 b5 78 a6 50 00 	mov    DWORD PTR [esi*4+0x50a678],eax
  412a74:	5e                   	pop    esi
  412a75:	5d                   	pop    ebp
  412a76:	5b                   	pop    ebx
  412a77:	59                   	pop    ecx
  412a78:	c3                   	ret
  412a79:	8d 0c b6             	lea    ecx,[esi+esi*4]
  412a7c:	89 35 c0 a5 50 00    	mov    DWORD PTR ds:0x50a5c0,esi
  412a82:	c1 e1 02             	shl    ecx,0x2
  412a85:	8b 81 d0 a6 50 00    	mov    eax,DWORD PTR [ecx+0x50a6d0]
  412a8b:	a3 70 96 50 00       	mov    ds:0x509670,eax
  412a90:	eb 17                	jmp    0x412aa9
  412a92:	8d 0c b6             	lea    ecx,[esi+esi*4]
  412a95:	89 35 c0 a5 50 00    	mov    DWORD PTR ds:0x50a5c0,esi
  412a9b:	c1 e1 02             	shl    ecx,0x2
  412a9e:	8b 81 d0 a6 50 00    	mov    eax,DWORD PTR [ecx+0x50a6d0]
  412aa4:	a3 74 96 50 00       	mov    ds:0x509674,eax
  412aa9:	8b 91 c0 a6 50 00    	mov    edx,DWORD PTR [ecx+0x50a6c0]
  412aaf:	8b 89 c4 a6 50 00    	mov    ecx,DWORD PTR [ecx+0x50a6c4]
  412ab5:	83 ea 1e             	sub    edx,0x1e
  412ab8:	5f                   	pop    edi
  412ab9:	83 e9 14             	sub    ecx,0x14
  412abc:	5e                   	pop    esi
  412abd:	5d                   	pop    ebp
  412abe:	89 15 24 bd 4f 00    	mov    DWORD PTR ds:0x4fbd24,edx
  412ac4:	89 0d 30 bd 4f 00    	mov    DWORD PTR ds:0x4fbd30,ecx
  412aca:	5b                   	pop    ebx
  412acb:	59                   	pop    ecx
  412acc:	c3                   	ret
  412acd:	8b 1d 24 ab 50 00    	mov    ebx,DWORD PTR ds:0x50ab24
  412ad3:	8b 3d 20 ab 50 00    	mov    edi,DWORD PTR ds:0x50ab20
  412ad9:	83 3d bc a5 50 00 01 	cmp    DWORD PTR ds:0x50a5bc,0x1
  412ae0:	75 1c                	jne    0x412afe
  412ae2:	8b 0d 30 bd 4f 00    	mov    ecx,DWORD PTR ds:0x4fbd30
  412ae8:	a1 24 bd 4f 00       	mov    eax,ds:0x4fbd24
  412aed:	83 e9 14             	sub    ecx,0x14
  412af0:	83 e8 14             	sub    eax,0x14
  412af3:	89 0d 30 bd 4f 00    	mov    DWORD PTR ds:0x4fbd30,ecx
  412af9:	a3 24 bd 4f 00       	mov    ds:0x4fbd24,eax
  412afe:	83 ff 01             	cmp    edi,0x1
  412b01:	75 19                	jne    0x412b1c
  412b03:	6a 02                	push   0x2
  412b05:	6a 32                	push   0x32
  412b07:	e8 b8 d4 01 00       	call   0x42ffc4
  412b0c:	99                   	cdq
  412b0d:	b9 03 00 00 00       	mov    ecx,0x3
  412b12:	f7 f9                	idiv   ecx
  412b14:	81 c2 6b 03 00 00    	add    edx,0x36b
  412b1a:	eb 1c                	jmp    0x412b38
  412b1c:	83 fb 01             	cmp    ebx,0x1
  412b1f:	75 23                	jne    0x412b44
  412b21:	6a 02                	push   0x2
  412b23:	6a 32                	push   0x32
  412b25:	e8 9a d4 01 00       	call   0x42ffc4
  412b2a:	99                   	cdq
  412b2b:	b9 03 00 00 00       	mov    ecx,0x3
  412b30:	f7 f9                	idiv   ecx
  412b32:	81 c2 8f 03 00 00    	add    edx,0x38f
  412b38:	8b 0d d8 dd 44 00    	mov    ecx,DWORD PTR ds:0x44ddd8
  412b3e:	52                   	push   edx
  412b3f:	e8 ac 01 ff ff       	call   0x402cf0
  412b44:	5f                   	pop    edi
  412b45:	5e                   	pop    esi
  412b46:	5d                   	pop    ebp
  412b47:	83 c8 ff             	or     eax,0xffffffff
  412b4a:	5b                   	pop    ebx
  412b4b:	59                   	pop    ecx
  412b4c:	c3                   	ret
