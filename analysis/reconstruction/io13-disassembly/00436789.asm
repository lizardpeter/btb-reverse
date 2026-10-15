
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00436789 <.text+0x35789>:
  436789:	51                   	push   ecx
  43678a:	53                   	push   ebx
  43678b:	55                   	push   ebp
  43678c:	56                   	push   esi
  43678d:	57                   	push   edi
  43678e:	33 ed                	xor    ebp,ebp
  436790:	83 cb ff             	or     ebx,0xffffffff
  436793:	68 a0 ba 43 00       	push   0x43baa0
  436798:	33 ff                	xor    edi,edi
  43679a:	89 2d 70 c5 51 00    	mov    DWORD PTR ds:0x51c570,ebp
  4367a0:	89 1d d8 9d 44 00    	mov    DWORD PTR ds:0x449dd8,ebx
  4367a6:	89 1d c8 9d 44 00    	mov    DWORD PTR ds:0x449dc8,ebx
  4367ac:	e8 99 1f 00 00       	call   0x43874a
  4367b1:	8b f0                	mov    esi,eax
  4367b3:	59                   	pop    ecx
  4367b4:	3b f5                	cmp    esi,ebp
  4367b6:	0f 85 f6 00 00 00    	jne    0x4368b2
  4367bc:	68 78 c5 51 00       	push   0x51c578
  4367c1:	ff 15 34 b1 43 00    	call   DWORD PTR ds:0x43b134
  4367c7:	3b c3                	cmp    eax,ebx
  4367c9:	0f 84 12 02 00 00    	je     0x4369e1
  4367cf:	a1 78 c5 51 00       	mov    eax,ds:0x51c578
  4367d4:	8b 0d cc c5 51 00    	mov    ecx,DWORD PTR ds:0x51c5cc
  4367da:	6b c0 3c             	imul   eax,eax,0x3c
  4367dd:	66 39 2d be c5 51 00 	cmp    WORD PTR ds:0x51c5be,bp
  4367e4:	6a 01                	push   0x1
  4367e6:	5a                   	pop    edx
  4367e7:	a3 30 9d 44 00       	mov    ds:0x449d30,eax
  4367ec:	89 15 70 c5 51 00    	mov    DWORD PTR ds:0x51c570,edx
  4367f2:	74 0c                	je     0x436800
  4367f4:	8b f1                	mov    esi,ecx
  4367f6:	6b f6 3c             	imul   esi,esi,0x3c
  4367f9:	03 c6                	add    eax,esi
  4367fb:	a3 30 9d 44 00       	mov    ds:0x449d30,eax
  436800:	66 39 2d 12 c6 51 00 	cmp    WORD PTR ds:0x51c612,bp
  436807:	74 1b                	je     0x436824
  436809:	a1 20 c6 51 00       	mov    eax,ds:0x51c620
  43680e:	3b c5                	cmp    eax,ebp
  436810:	74 12                	je     0x436824
  436812:	2b c1                	sub    eax,ecx
  436814:	89 15 34 9d 44 00    	mov    DWORD PTR ds:0x449d34,edx
  43681a:	6b c0 3c             	imul   eax,eax,0x3c
  43681d:	a3 38 9d 44 00       	mov    ds:0x449d38,eax
  436822:	eb 0c                	jmp    0x436830
  436824:	89 2d 34 9d 44 00    	mov    DWORD PTR ds:0x449d34,ebp
  43682a:	89 2d 38 9d 44 00    	mov    DWORD PTR ds:0x449d38,ebp
  436830:	8d 44 24 10          	lea    eax,[esp+0x10]
  436834:	8b 35 80 b0 43 00    	mov    esi,DWORD PTR ds:0x43b080
  43683a:	50                   	push   eax
  43683b:	55                   	push   ebp
  43683c:	6a 3f                	push   0x3f
  43683e:	bf 20 02 00 00       	mov    edi,0x220
  436843:	ff 35 bc 9d 44 00    	push   DWORD PTR ds:0x449dbc
  436849:	53                   	push   ebx
  43684a:	68 7c c5 51 00       	push   0x51c57c
  43684f:	57                   	push   edi
  436850:	ff 35 90 c6 51 00    	push   DWORD PTR ds:0x51c690
  436856:	ff d6                	call   esi
  436858:	85 c0                	test   eax,eax
  43685a:	74 11                	je     0x43686d
  43685c:	39 6c 24 10          	cmp    DWORD PTR [esp+0x10],ebp
  436860:	75 0b                	jne    0x43686d
  436862:	a1 bc 9d 44 00       	mov    eax,ds:0x449dbc
  436867:	80 60 3f 00          	and    BYTE PTR [eax+0x3f],0x0
  43686b:	eb 08                	jmp    0x436875
  43686d:	a1 bc 9d 44 00       	mov    eax,ds:0x449dbc
  436872:	80 20 00             	and    BYTE PTR [eax],0x0
  436875:	8d 44 24 10          	lea    eax,[esp+0x10]
  436879:	50                   	push   eax
  43687a:	55                   	push   ebp
  43687b:	6a 3f                	push   0x3f
  43687d:	ff 35 c0 9d 44 00    	push   DWORD PTR ds:0x449dc0
  436883:	53                   	push   ebx
  436884:	68 d0 c5 51 00       	push   0x51c5d0
  436889:	57                   	push   edi
  43688a:	ff 35 90 c6 51 00    	push   DWORD PTR ds:0x51c690
  436890:	ff d6                	call   esi
  436892:	85 c0                	test   eax,eax
  436894:	0f 84 3f 01 00 00    	je     0x4369d9
  43689a:	39 6c 24 10          	cmp    DWORD PTR [esp+0x10],ebp
  43689e:	0f 85 35 01 00 00    	jne    0x4369d9
  4368a4:	a1 c0 9d 44 00       	mov    eax,ds:0x449dc0
  4368a9:	80 60 3f 00          	and    BYTE PTR [eax+0x3f],0x0
  4368ad:	e9 2f 01 00 00       	jmp    0x4369e1
  4368b2:	80 3e 00             	cmp    BYTE PTR [esi],0x0
  4368b5:	0f 84 26 01 00 00    	je     0x4369e1
  4368bb:	a1 24 c6 51 00       	mov    eax,ds:0x51c624
  4368c0:	3b c5                	cmp    eax,ebp
  4368c2:	74 11                	je     0x4368d5
  4368c4:	50                   	push   eax
  4368c5:	56                   	push   esi
  4368c6:	e8 d5 ce ff ff       	call   0x4337a0
  4368cb:	59                   	pop    ecx
  4368cc:	85 c0                	test   eax,eax
  4368ce:	59                   	pop    ecx
  4368cf:	0f 84 0c 01 00 00    	je     0x4369e1
  4368d5:	ff 35 24 c6 51 00    	push   DWORD PTR ds:0x51c624
  4368db:	e8 4a a4 ff ff       	call   0x430d2a
  4368e0:	56                   	push   esi
  4368e1:	e8 3a ce ff ff       	call   0x433720
  4368e6:	40                   	inc    eax
  4368e7:	50                   	push   eax
  4368e8:	e8 a6 a4 ff ff       	call   0x430d93
  4368ed:	83 c4 0c             	add    esp,0xc
  4368f0:	3b c5                	cmp    eax,ebp
  4368f2:	a3 24 c6 51 00       	mov    ds:0x51c624,eax
  4368f7:	0f 84 e4 00 00 00    	je     0x4369e1
  4368fd:	56                   	push   esi
  4368fe:	50                   	push   eax
  4368ff:	e8 2c cd ff ff       	call   0x433630
  436904:	6a 03                	push   0x3
  436906:	56                   	push   esi
  436907:	ff 35 bc 9d 44 00    	push   DWORD PTR ds:0x449dbc
  43690d:	e8 8e 99 ff ff       	call   0x4302a0
  436912:	a1 bc 9d 44 00       	mov    eax,ds:0x449dbc
  436917:	83 c6 03             	add    esi,0x3
  43691a:	83 c4 14             	add    esp,0x14
  43691d:	80 60 03 00          	and    BYTE PTR [eax+0x3],0x0
  436921:	80 3e 2d             	cmp    BYTE PTR [esi],0x2d
  436924:	75 04                	jne    0x43692a
  436926:	6a 01                	push   0x1
  436928:	46                   	inc    esi
  436929:	5f                   	pop    edi
  43692a:	56                   	push   esi
  43692b:	e8 8f 1d 00 00       	call   0x4386bf
  436930:	59                   	pop    ecx
  436931:	b3 30                	mov    bl,0x30
  436933:	8b c8                	mov    ecx,eax
  436935:	69 c9 10 0e 00 00    	imul   ecx,ecx,0xe10
  43693b:	89 0d 30 9d 44 00    	mov    DWORD PTR ds:0x449d30,ecx
  436941:	8a 06                	mov    al,BYTE PTR [esi]
  436943:	3c 2b                	cmp    al,0x2b
  436945:	74 08                	je     0x43694f
  436947:	3a c3                	cmp    al,bl
  436949:	7c 07                	jl     0x436952
  43694b:	3c 39                	cmp    al,0x39
  43694d:	7f 03                	jg     0x436952
  43694f:	46                   	inc    esi
  436950:	eb ef                	jmp    0x436941
  436952:	80 3e 3a             	cmp    BYTE PTR [esi],0x3a
  436955:	75 4e                	jne    0x4369a5
  436957:	46                   	inc    esi
  436958:	56                   	push   esi
  436959:	e8 61 1d 00 00       	call   0x4386bf
  43695e:	6b c0 3c             	imul   eax,eax,0x3c
  436961:	59                   	pop    ecx
  436962:	8b 0d 30 9d 44 00    	mov    ecx,DWORD PTR ds:0x449d30
  436968:	03 c8                	add    ecx,eax
  43696a:	89 0d 30 9d 44 00    	mov    DWORD PTR ds:0x449d30,ecx
  436970:	8a 06                	mov    al,BYTE PTR [esi]
  436972:	3a c3                	cmp    al,bl
  436974:	7c 07                	jl     0x43697d
  436976:	3c 39                	cmp    al,0x39
  436978:	7f 03                	jg     0x43697d
  43697a:	46                   	inc    esi
  43697b:	eb f3                	jmp    0x436970
  43697d:	80 3e 3a             	cmp    BYTE PTR [esi],0x3a
  436980:	75 23                	jne    0x4369a5
  436982:	46                   	inc    esi
  436983:	56                   	push   esi
  436984:	e8 36 1d 00 00       	call   0x4386bf
  436989:	59                   	pop    ecx
  43698a:	8b 0d 30 9d 44 00    	mov    ecx,DWORD PTR ds:0x449d30
  436990:	03 c8                	add    ecx,eax
  436992:	89 0d 30 9d 44 00    	mov    DWORD PTR ds:0x449d30,ecx
  436998:	8a 06                	mov    al,BYTE PTR [esi]
  43699a:	3a c3                	cmp    al,bl
  43699c:	7c 07                	jl     0x4369a5
  43699e:	3c 39                	cmp    al,0x39
  4369a0:	7f 03                	jg     0x4369a5
  4369a2:	46                   	inc    esi
  4369a3:	eb f3                	jmp    0x436998
  4369a5:	3b fd                	cmp    edi,ebp
  4369a7:	74 08                	je     0x4369b1
  4369a9:	f7 d9                	neg    ecx
  4369ab:	89 0d 30 9d 44 00    	mov    DWORD PTR ds:0x449d30,ecx
  4369b1:	0f be 06             	movsx  eax,BYTE PTR [esi]
  4369b4:	3b c5                	cmp    eax,ebp
  4369b6:	a3 34 9d 44 00       	mov    ds:0x449d34,eax
  4369bb:	74 1c                	je     0x4369d9
  4369bd:	6a 03                	push   0x3
  4369bf:	56                   	push   esi
  4369c0:	ff 35 c0 9d 44 00    	push   DWORD PTR ds:0x449dc0
  4369c6:	e8 d5 98 ff ff       	call   0x4302a0
  4369cb:	a1 c0 9d 44 00       	mov    eax,ds:0x449dc0
  4369d0:	83 c4 0c             	add    esp,0xc
  4369d3:	80 60 03 00          	and    BYTE PTR [eax+0x3],0x0
  4369d7:	eb 08                	jmp    0x4369e1
  4369d9:	a1 c0 9d 44 00       	mov    eax,ds:0x449dc0
  4369de:	80 20 00             	and    BYTE PTR [eax],0x0
  4369e1:	5f                   	pop    edi
  4369e2:	5e                   	pop    esi
  4369e3:	5d                   	pop    ebp
  4369e4:	5b                   	pop    ebx
  4369e5:	59                   	pop    ecx
  4369e6:	c3                   	ret
