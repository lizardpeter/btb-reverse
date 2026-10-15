
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00434798 <.text+0x33798>:
  434798:	55                   	push   ebp
  434799:	8b ec                	mov    ebp,esp
  43479b:	83 ec 14             	sub    esp,0x14
  43479e:	a1 34 da 51 00       	mov    eax,ds:0x51da34
  4347a3:	8b 15 38 da 51 00    	mov    edx,DWORD PTR ds:0x51da38
  4347a9:	53                   	push   ebx
  4347aa:	56                   	push   esi
  4347ab:	8d 04 80             	lea    eax,[eax+eax*4]
  4347ae:	57                   	push   edi
  4347af:	8d 3c 82             	lea    edi,[edx+eax*4]
  4347b2:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  4347b5:	89 7d fc             	mov    DWORD PTR [ebp-0x4],edi
  4347b8:	8d 48 17             	lea    ecx,[eax+0x17]
  4347bb:	83 e1 f0             	and    ecx,0xfffffff0
  4347be:	89 4d f0             	mov    DWORD PTR [ebp-0x10],ecx
  4347c1:	c1 f9 04             	sar    ecx,0x4
  4347c4:	49                   	dec    ecx
  4347c5:	83 f9 20             	cmp    ecx,0x20
  4347c8:	7d 0e                	jge    0x4347d8
  4347ca:	83 ce ff             	or     esi,0xffffffff
  4347cd:	d3 ee                	shr    esi,cl
  4347cf:	83 4d f8 ff          	or     DWORD PTR [ebp-0x8],0xffffffff
  4347d3:	89 75 f4             	mov    DWORD PTR [ebp-0xc],esi
  4347d6:	eb 10                	jmp    0x4347e8
  4347d8:	83 c1 e0             	add    ecx,0xffffffe0
  4347db:	83 c8 ff             	or     eax,0xffffffff
  4347de:	33 f6                	xor    esi,esi
  4347e0:	d3 e8                	shr    eax,cl
  4347e2:	89 75 f4             	mov    DWORD PTR [ebp-0xc],esi
  4347e5:	89 45 f8             	mov    DWORD PTR [ebp-0x8],eax
  4347e8:	a1 2c da 51 00       	mov    eax,ds:0x51da2c
  4347ed:	8b d8                	mov    ebx,eax
  4347ef:	3b df                	cmp    ebx,edi
  4347f1:	89 5d 08             	mov    DWORD PTR [ebp+0x8],ebx
  4347f4:	73 19                	jae    0x43480f
  4347f6:	8b 4b 04             	mov    ecx,DWORD PTR [ebx+0x4]
  4347f9:	8b 3b                	mov    edi,DWORD PTR [ebx]
  4347fb:	23 4d f8             	and    ecx,DWORD PTR [ebp-0x8]
  4347fe:	23 fe                	and    edi,esi
  434800:	0b cf                	or     ecx,edi
  434802:	75 0b                	jne    0x43480f
  434804:	83 c3 14             	add    ebx,0x14
  434807:	3b 5d fc             	cmp    ebx,DWORD PTR [ebp-0x4]
  43480a:	89 5d 08             	mov    DWORD PTR [ebp+0x8],ebx
  43480d:	72 e7                	jb     0x4347f6
  43480f:	3b 5d fc             	cmp    ebx,DWORD PTR [ebp-0x4]
  434812:	75 79                	jne    0x43488d
  434814:	8b da                	mov    ebx,edx
  434816:	3b d8                	cmp    ebx,eax
  434818:	89 5d 08             	mov    DWORD PTR [ebp+0x8],ebx
  43481b:	73 15                	jae    0x434832
  43481d:	8b 4b 04             	mov    ecx,DWORD PTR [ebx+0x4]
  434820:	8b 3b                	mov    edi,DWORD PTR [ebx]
  434822:	23 4d f8             	and    ecx,DWORD PTR [ebp-0x8]
  434825:	23 fe                	and    edi,esi
  434827:	0b cf                	or     ecx,edi
  434829:	75 05                	jne    0x434830
  43482b:	83 c3 14             	add    ebx,0x14
  43482e:	eb e6                	jmp    0x434816
  434830:	3b d8                	cmp    ebx,eax
  434832:	75 59                	jne    0x43488d
  434834:	3b 5d fc             	cmp    ebx,DWORD PTR [ebp-0x4]
  434837:	73 11                	jae    0x43484a
  434839:	83 7b 08 00          	cmp    DWORD PTR [ebx+0x8],0x0
  43483d:	75 08                	jne    0x434847
  43483f:	83 c3 14             	add    ebx,0x14
  434842:	89 5d 08             	mov    DWORD PTR [ebp+0x8],ebx
  434845:	eb ed                	jmp    0x434834
  434847:	3b 5d fc             	cmp    ebx,DWORD PTR [ebp-0x4]
  43484a:	75 26                	jne    0x434872
  43484c:	8b da                	mov    ebx,edx
  43484e:	3b d8                	cmp    ebx,eax
  434850:	89 5d 08             	mov    DWORD PTR [ebp+0x8],ebx
  434853:	73 0d                	jae    0x434862
  434855:	83 7b 08 00          	cmp    DWORD PTR [ebx+0x8],0x0
  434859:	75 05                	jne    0x434860
  43485b:	83 c3 14             	add    ebx,0x14
  43485e:	eb ee                	jmp    0x43484e
  434860:	3b d8                	cmp    ebx,eax
  434862:	75 0e                	jne    0x434872
  434864:	e8 38 02 00 00       	call   0x434aa1
  434869:	8b d8                	mov    ebx,eax
  43486b:	85 db                	test   ebx,ebx
  43486d:	89 5d 08             	mov    DWORD PTR [ebp+0x8],ebx
  434870:	74 14                	je     0x434886
  434872:	53                   	push   ebx
  434873:	e8 da 02 00 00       	call   0x434b52
  434878:	59                   	pop    ecx
  434879:	8b 4b 10             	mov    ecx,DWORD PTR [ebx+0x10]
  43487c:	89 01                	mov    DWORD PTR [ecx],eax
  43487e:	8b 43 10             	mov    eax,DWORD PTR [ebx+0x10]
  434881:	83 38 ff             	cmp    DWORD PTR [eax],0xffffffff
  434884:	75 07                	jne    0x43488d
  434886:	33 c0                	xor    eax,eax
  434888:	e9 0f 02 00 00       	jmp    0x434a9c
  43488d:	89 1d 2c da 51 00    	mov    DWORD PTR ds:0x51da2c,ebx
  434893:	8b 43 10             	mov    eax,DWORD PTR [ebx+0x10]
  434896:	8b 10                	mov    edx,DWORD PTR [eax]
  434898:	83 fa ff             	cmp    edx,0xffffffff
  43489b:	89 55 fc             	mov    DWORD PTR [ebp-0x4],edx
  43489e:	74 14                	je     0x4348b4
  4348a0:	8b 8c 90 c4 00 00 00 	mov    ecx,DWORD PTR [eax+edx*4+0xc4]
  4348a7:	8b 7c 90 44          	mov    edi,DWORD PTR [eax+edx*4+0x44]
  4348ab:	23 4d f8             	and    ecx,DWORD PTR [ebp-0x8]
  4348ae:	23 fe                	and    edi,esi
  4348b0:	0b cf                	or     ecx,edi
  4348b2:	75 37                	jne    0x4348eb
  4348b4:	8b 90 c4 00 00 00    	mov    edx,DWORD PTR [eax+0xc4]
  4348ba:	8b 70 44             	mov    esi,DWORD PTR [eax+0x44]
  4348bd:	23 55 f8             	and    edx,DWORD PTR [ebp-0x8]
  4348c0:	23 75 f4             	and    esi,DWORD PTR [ebp-0xc]
  4348c3:	83 65 fc 00          	and    DWORD PTR [ebp-0x4],0x0
  4348c7:	8d 48 44             	lea    ecx,[eax+0x44]
  4348ca:	0b d6                	or     edx,esi
  4348cc:	8b 75 f4             	mov    esi,DWORD PTR [ebp-0xc]
  4348cf:	75 17                	jne    0x4348e8
  4348d1:	8b 91 84 00 00 00    	mov    edx,DWORD PTR [ecx+0x84]
  4348d7:	ff 45 fc             	inc    DWORD PTR [ebp-0x4]
  4348da:	23 55 f8             	and    edx,DWORD PTR [ebp-0x8]
  4348dd:	83 c1 04             	add    ecx,0x4
  4348e0:	8b fe                	mov    edi,esi
  4348e2:	23 39                	and    edi,DWORD PTR [ecx]
  4348e4:	0b d7                	or     edx,edi
  4348e6:	74 e9                	je     0x4348d1
  4348e8:	8b 55 fc             	mov    edx,DWORD PTR [ebp-0x4]
  4348eb:	8b ca                	mov    ecx,edx
  4348ed:	33 ff                	xor    edi,edi
  4348ef:	69 c9 04 02 00 00    	imul   ecx,ecx,0x204
  4348f5:	8d 8c 01 44 01 00 00 	lea    ecx,[ecx+eax*1+0x144]
  4348fc:	89 4d f4             	mov    DWORD PTR [ebp-0xc],ecx
  4348ff:	8b 4c 90 44          	mov    ecx,DWORD PTR [eax+edx*4+0x44]
  434903:	23 ce                	and    ecx,esi
  434905:	75 0d                	jne    0x434914
  434907:	8b 8c 90 c4 00 00 00 	mov    ecx,DWORD PTR [eax+edx*4+0xc4]
  43490e:	6a 20                	push   0x20
  434910:	23 4d f8             	and    ecx,DWORD PTR [ebp-0x8]
  434913:	5f                   	pop    edi
  434914:	85 c9                	test   ecx,ecx
  434916:	7c 05                	jl     0x43491d
  434918:	d1 e1                	shl    ecx,1
  43491a:	47                   	inc    edi
  43491b:	eb f7                	jmp    0x434914
  43491d:	8b 4d f4             	mov    ecx,DWORD PTR [ebp-0xc]
  434920:	8b 54 f9 04          	mov    edx,DWORD PTR [ecx+edi*8+0x4]
  434924:	8b 0a                	mov    ecx,DWORD PTR [edx]
  434926:	2b 4d f0             	sub    ecx,DWORD PTR [ebp-0x10]
  434929:	8b f1                	mov    esi,ecx
  43492b:	89 4d f8             	mov    DWORD PTR [ebp-0x8],ecx
  43492e:	c1 fe 04             	sar    esi,0x4
  434931:	4e                   	dec    esi
  434932:	83 fe 3f             	cmp    esi,0x3f
  434935:	7e 03                	jle    0x43493a
  434937:	6a 3f                	push   0x3f
  434939:	5e                   	pop    esi
  43493a:	3b f7                	cmp    esi,edi
  43493c:	0f 84 0d 01 00 00    	je     0x434a4f
  434942:	8b 4a 04             	mov    ecx,DWORD PTR [edx+0x4]
  434945:	3b 4a 08             	cmp    ecx,DWORD PTR [edx+0x8]
  434948:	75 61                	jne    0x4349ab
  43494a:	83 ff 20             	cmp    edi,0x20
  43494d:	7d 2b                	jge    0x43497a
  43494f:	bb 00 00 00 80       	mov    ebx,0x80000000
  434954:	8b cf                	mov    ecx,edi
  434956:	d3 eb                	shr    ebx,cl
  434958:	8b 4d fc             	mov    ecx,DWORD PTR [ebp-0x4]
  43495b:	8d 7c 38 04          	lea    edi,[eax+edi*1+0x4]
  43495f:	f7 d3                	not    ebx
  434961:	89 5d ec             	mov    DWORD PTR [ebp-0x14],ebx
  434964:	23 5c 88 44          	and    ebx,DWORD PTR [eax+ecx*4+0x44]
  434968:	89 5c 88 44          	mov    DWORD PTR [eax+ecx*4+0x44],ebx
  43496c:	fe 0f                	dec    BYTE PTR [edi]
  43496e:	75 38                	jne    0x4349a8
  434970:	8b 5d 08             	mov    ebx,DWORD PTR [ebp+0x8]
  434973:	8b 4d ec             	mov    ecx,DWORD PTR [ebp-0x14]
  434976:	21 0b                	and    DWORD PTR [ebx],ecx
  434978:	eb 31                	jmp    0x4349ab
  43497a:	8d 4f e0             	lea    ecx,[edi-0x20]
  43497d:	bb 00 00 00 80       	mov    ebx,0x80000000
  434982:	d3 eb                	shr    ebx,cl
  434984:	8b 4d fc             	mov    ecx,DWORD PTR [ebp-0x4]
  434987:	8d 7c 38 04          	lea    edi,[eax+edi*1+0x4]
  43498b:	8d 8c 88 c4 00 00 00 	lea    ecx,[eax+ecx*4+0xc4]
  434992:	f7 d3                	not    ebx
  434994:	21 19                	and    DWORD PTR [ecx],ebx
  434996:	fe 0f                	dec    BYTE PTR [edi]
  434998:	89 5d ec             	mov    DWORD PTR [ebp-0x14],ebx
  43499b:	75 0b                	jne    0x4349a8
  43499d:	8b 5d 08             	mov    ebx,DWORD PTR [ebp+0x8]
  4349a0:	8b 4d ec             	mov    ecx,DWORD PTR [ebp-0x14]
  4349a3:	21 4b 04             	and    DWORD PTR [ebx+0x4],ecx
  4349a6:	eb 03                	jmp    0x4349ab
  4349a8:	8b 5d 08             	mov    ebx,DWORD PTR [ebp+0x8]
  4349ab:	8b 4a 08             	mov    ecx,DWORD PTR [edx+0x8]
  4349ae:	8b 7a 04             	mov    edi,DWORD PTR [edx+0x4]
  4349b1:	83 7d f8 00          	cmp    DWORD PTR [ebp-0x8],0x0
  4349b5:	89 79 04             	mov    DWORD PTR [ecx+0x4],edi
  4349b8:	8b 4a 04             	mov    ecx,DWORD PTR [edx+0x4]
  4349bb:	8b 7a 08             	mov    edi,DWORD PTR [edx+0x8]
  4349be:	89 79 08             	mov    DWORD PTR [ecx+0x8],edi
  4349c1:	0f 84 94 00 00 00    	je     0x434a5b
  4349c7:	8b 4d f4             	mov    ecx,DWORD PTR [ebp-0xc]
  4349ca:	8b 7c f1 04          	mov    edi,DWORD PTR [ecx+esi*8+0x4]
  4349ce:	8d 0c f1             	lea    ecx,[ecx+esi*8]
  4349d1:	89 7a 04             	mov    DWORD PTR [edx+0x4],edi
  4349d4:	89 4a 08             	mov    DWORD PTR [edx+0x8],ecx
  4349d7:	89 51 04             	mov    DWORD PTR [ecx+0x4],edx
  4349da:	8b 4a 04             	mov    ecx,DWORD PTR [edx+0x4]
  4349dd:	89 51 08             	mov    DWORD PTR [ecx+0x8],edx
  4349e0:	8b 4a 04             	mov    ecx,DWORD PTR [edx+0x4]
  4349e3:	3b 4a 08             	cmp    ecx,DWORD PTR [edx+0x8]
  4349e6:	75 64                	jne    0x434a4c
  4349e8:	8a 4c 06 04          	mov    cl,BYTE PTR [esi+eax*1+0x4]
  4349ec:	83 fe 20             	cmp    esi,0x20
  4349ef:	88 4d 0b             	mov    BYTE PTR [ebp+0xb],cl
  4349f2:	7d 29                	jge    0x434a1d
  4349f4:	fe c1                	inc    cl
  4349f6:	80 7d 0b 00          	cmp    BYTE PTR [ebp+0xb],0x0
  4349fa:	88 4c 06 04          	mov    BYTE PTR [esi+eax*1+0x4],cl
  4349fe:	75 0b                	jne    0x434a0b
  434a00:	bf 00 00 00 80       	mov    edi,0x80000000
  434a05:	8b ce                	mov    ecx,esi
  434a07:	d3 ef                	shr    edi,cl
  434a09:	09 3b                	or     DWORD PTR [ebx],edi
  434a0b:	bf 00 00 00 80       	mov    edi,0x80000000
  434a10:	8b ce                	mov    ecx,esi
  434a12:	d3 ef                	shr    edi,cl
  434a14:	8b 4d fc             	mov    ecx,DWORD PTR [ebp-0x4]
  434a17:	09 7c 88 44          	or     DWORD PTR [eax+ecx*4+0x44],edi
  434a1b:	eb 2f                	jmp    0x434a4c
  434a1d:	fe c1                	inc    cl
  434a1f:	80 7d 0b 00          	cmp    BYTE PTR [ebp+0xb],0x0
  434a23:	88 4c 06 04          	mov    BYTE PTR [esi+eax*1+0x4],cl
  434a27:	75 0d                	jne    0x434a36
  434a29:	8d 4e e0             	lea    ecx,[esi-0x20]
  434a2c:	bf 00 00 00 80       	mov    edi,0x80000000
  434a31:	d3 ef                	shr    edi,cl
  434a33:	09 7b 04             	or     DWORD PTR [ebx+0x4],edi
  434a36:	8b 4d fc             	mov    ecx,DWORD PTR [ebp-0x4]
  434a39:	8d bc 88 c4 00 00 00 	lea    edi,[eax+ecx*4+0xc4]
  434a40:	8d 4e e0             	lea    ecx,[esi-0x20]
  434a43:	be 00 00 00 80       	mov    esi,0x80000000
  434a48:	d3 ee                	shr    esi,cl
  434a4a:	09 37                	or     DWORD PTR [edi],esi
  434a4c:	8b 4d f8             	mov    ecx,DWORD PTR [ebp-0x8]
  434a4f:	85 c9                	test   ecx,ecx
  434a51:	74 0b                	je     0x434a5e
  434a53:	89 0a                	mov    DWORD PTR [edx],ecx
  434a55:	89 4c 11 fc          	mov    DWORD PTR [ecx+edx*1-0x4],ecx
  434a59:	eb 03                	jmp    0x434a5e
  434a5b:	8b 4d f8             	mov    ecx,DWORD PTR [ebp-0x8]
  434a5e:	8b 75 f0             	mov    esi,DWORD PTR [ebp-0x10]
  434a61:	03 d1                	add    edx,ecx
  434a63:	8d 4e 01             	lea    ecx,[esi+0x1]
  434a66:	89 0a                	mov    DWORD PTR [edx],ecx
  434a68:	89 4c 32 fc          	mov    DWORD PTR [edx+esi*1-0x4],ecx
  434a6c:	8b 75 f4             	mov    esi,DWORD PTR [ebp-0xc]
  434a6f:	8b 0e                	mov    ecx,DWORD PTR [esi]
  434a71:	85 c9                	test   ecx,ecx
  434a73:	8d 79 01             	lea    edi,[ecx+0x1]
  434a76:	89 3e                	mov    DWORD PTR [esi],edi
  434a78:	75 1a                	jne    0x434a94
  434a7a:	3b 1d 30 da 51 00    	cmp    ebx,DWORD PTR ds:0x51da30
  434a80:	75 12                	jne    0x434a94
  434a82:	8b 4d fc             	mov    ecx,DWORD PTR [ebp-0x4]
  434a85:	3b 0d 28 da 51 00    	cmp    ecx,DWORD PTR ds:0x51da28
  434a8b:	75 07                	jne    0x434a94
  434a8d:	83 25 30 da 51 00 00 	and    DWORD PTR ds:0x51da30,0x0
  434a94:	8b 4d fc             	mov    ecx,DWORD PTR [ebp-0x4]
  434a97:	89 08                	mov    DWORD PTR [eax],ecx
  434a99:	8d 42 04             	lea    eax,[edx+0x4]
  434a9c:	5f                   	pop    edi
  434a9d:	5e                   	pop    esi
  434a9e:	5b                   	pop    ebx
  434a9f:	c9                   	leave
  434aa0:	c3                   	ret
