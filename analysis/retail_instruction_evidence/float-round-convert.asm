
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00437674 <.text+0x36674>:
  437674:	8b 44 24 08          	mov    eax,DWORD PTR [esp+0x8]
  437678:	56                   	push   esi
  437679:	6a 20                	push   0x20
  43767b:	99                   	cdq
  43767c:	59                   	pop    ecx
  43767d:	f7 f9                	idiv   ecx
  43767f:	6a 1f                	push   0x1f
  437681:	8b f0                	mov    esi,eax
  437683:	8b 44 24 10          	mov    eax,DWORD PTR [esp+0x10]
  437687:	99                   	cdq
  437688:	f7 f9                	idiv   ecx
  43768a:	59                   	pop    ecx
  43768b:	8b 44 24 08          	mov    eax,DWORD PTR [esp+0x8]
  43768f:	2b ca                	sub    ecx,edx
  437691:	83 ca ff             	or     edx,0xffffffff
  437694:	d3 e2                	shl    edx,cl
  437696:	f7 d2                	not    edx
  437698:	85 14 b0             	test   DWORD PTR [eax+esi*4],edx
  43769b:	75 1c                	jne    0x4376b9
  43769d:	46                   	inc    esi
  43769e:	83 fe 03             	cmp    esi,0x3
  4376a1:	7d 11                	jge    0x4376b4
  4376a3:	8d 04 b0             	lea    eax,[eax+esi*4]
  4376a6:	83 38 00             	cmp    DWORD PTR [eax],0x0
  4376a9:	75 0e                	jne    0x4376b9
  4376ab:	46                   	inc    esi
  4376ac:	83 c0 04             	add    eax,0x4
  4376af:	83 fe 03             	cmp    esi,0x3
  4376b2:	7c f2                	jl     0x4376a6
  4376b4:	6a 01                	push   0x1
  4376b6:	58                   	pop    eax
  4376b7:	5e                   	pop    esi
  4376b8:	c3                   	ret
  4376b9:	33 c0                	xor    eax,eax
  4376bb:	5e                   	pop    esi
  4376bc:	c3                   	ret
  4376bd:	8b 44 24 08          	mov    eax,DWORD PTR [esp+0x8]
  4376c1:	53                   	push   ebx
  4376c2:	56                   	push   esi
  4376c3:	57                   	push   edi
  4376c4:	6a 20                	push   0x20
  4376c6:	8b 5c 24 14          	mov    ebx,DWORD PTR [esp+0x14]
  4376ca:	99                   	cdq
  4376cb:	59                   	pop    ecx
  4376cc:	f7 f9                	idiv   ecx
  4376ce:	8b f0                	mov    esi,eax
  4376d0:	8b 44 24 14          	mov    eax,DWORD PTR [esp+0x14]
  4376d4:	99                   	cdq
  4376d5:	f7 f9                	idiv   ecx
  4376d7:	8d 3c b3             	lea    edi,[ebx+esi*4]
  4376da:	57                   	push   edi
  4376db:	6a 1f                	push   0x1f
  4376dd:	59                   	pop    ecx
  4376de:	6a 01                	push   0x1
  4376e0:	58                   	pop    eax
  4376e1:	2b ca                	sub    ecx,edx
  4376e3:	d3 e0                	shl    eax,cl
  4376e5:	50                   	push   eax
  4376e6:	ff 37                	push   DWORD PTR [edi]
  4376e8:	e8 da 10 00 00       	call   0x4387c7
  4376ed:	83 c4 0c             	add    esp,0xc
  4376f0:	4e                   	dec    esi
  4376f1:	78 1c                	js     0x43770f
  4376f3:	8d 3c b3             	lea    edi,[ebx+esi*4]
  4376f6:	85 c0                	test   eax,eax
  4376f8:	74 15                	je     0x43770f
  4376fa:	57                   	push   edi
  4376fb:	6a 01                	push   0x1
  4376fd:	ff 37                	push   DWORD PTR [edi]
  4376ff:	e8 c3 10 00 00       	call   0x4387c7
  437704:	83 c4 0c             	add    esp,0xc
  437707:	4e                   	dec    esi
  437708:	83 ef 04             	sub    edi,0x4
  43770b:	85 f6                	test   esi,esi
  43770d:	7d e7                	jge    0x4376f6
  43770f:	5f                   	pop    edi
  437710:	5e                   	pop    esi
  437711:	5b                   	pop    ebx
  437712:	c3                   	ret
  437713:	55                   	push   ebp
  437714:	8b ec                	mov    ebp,esp
  437716:	51                   	push   ecx
  437717:	51                   	push   ecx
  437718:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  43771b:	53                   	push   ebx
  43771c:	56                   	push   esi
  43771d:	57                   	push   edi
  43771e:	8d 78 ff             	lea    edi,[eax-0x1]
  437721:	6a 20                	push   0x20
  437723:	59                   	pop    ecx
  437724:	83 65 fc 00          	and    DWORD PTR [ebp-0x4],0x0
  437728:	8d 5f 01             	lea    ebx,[edi+0x1]
  43772b:	6a 20                	push   0x20
  43772d:	8b c3                	mov    eax,ebx
  43772f:	5e                   	pop    esi
  437730:	99                   	cdq
  437731:	f7 f9                	idiv   ecx
  437733:	6a 1f                	push   0x1f
  437735:	8b c8                	mov    ecx,eax
  437737:	8b c3                	mov    eax,ebx
  437739:	99                   	cdq
  43773a:	f7 fe                	idiv   esi
  43773c:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  43773f:	5e                   	pop    esi
  437740:	6a 01                	push   0x1
  437742:	89 4d f8             	mov    DWORD PTR [ebp-0x8],ecx
  437745:	8d 04 88             	lea    eax,[eax+ecx*4]
  437748:	89 45 0c             	mov    DWORD PTR [ebp+0xc],eax
  43774b:	2b f2                	sub    esi,edx
  43774d:	5a                   	pop    edx
  43774e:	8b ce                	mov    ecx,esi
  437750:	d3 e2                	shl    edx,cl
  437752:	85 10                	test   DWORD PTR [eax],edx
  437754:	74 21                	je     0x437777
  437756:	43                   	inc    ebx
  437757:	53                   	push   ebx
  437758:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  43775b:	e8 14 ff ff ff       	call   0x437674
  437760:	59                   	pop    ecx
  437761:	85 c0                	test   eax,eax
  437763:	59                   	pop    ecx
  437764:	75 0e                	jne    0x437774
  437766:	57                   	push   edi
  437767:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  43776a:	e8 4e ff ff ff       	call   0x4376bd
  43776f:	59                   	pop    ecx
  437770:	89 45 fc             	mov    DWORD PTR [ebp-0x4],eax
  437773:	59                   	pop    ecx
  437774:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  437777:	83 ca ff             	or     edx,0xffffffff
  43777a:	8b ce                	mov    ecx,esi
  43777c:	d3 e2                	shl    edx,cl
  43777e:	6a 03                	push   0x3
  437780:	59                   	pop    ecx
  437781:	21 10                	and    DWORD PTR [eax],edx
  437783:	8b 45 f8             	mov    eax,DWORD PTR [ebp-0x8]
  437786:	40                   	inc    eax
  437787:	3b c1                	cmp    eax,ecx
  437789:	7d 0c                	jge    0x437797
  43778b:	8b 55 08             	mov    edx,DWORD PTR [ebp+0x8]
  43778e:	2b c8                	sub    ecx,eax
  437790:	8d 3c 82             	lea    edi,[edx+eax*4]
  437793:	33 c0                	xor    eax,eax
  437795:	f3 ab                	rep stos DWORD PTR es:[edi],eax
  437797:	8b 45 fc             	mov    eax,DWORD PTR [ebp-0x4]
  43779a:	5f                   	pop    edi
  43779b:	5e                   	pop    esi
  43779c:	5b                   	pop    ebx
  43779d:	c9                   	leave
  43779e:	c3                   	ret
  43779f:	8b 44 24 08          	mov    eax,DWORD PTR [esp+0x8]
  4377a3:	8b 4c 24 04          	mov    ecx,DWORD PTR [esp+0x4]
  4377a7:	56                   	push   esi
  4377a8:	6a 03                	push   0x3
  4377aa:	2b c8                	sub    ecx,eax
  4377ac:	5a                   	pop    edx
  4377ad:	8b 30                	mov    esi,DWORD PTR [eax]
  4377af:	89 34 01             	mov    DWORD PTR [ecx+eax*1],esi
  4377b2:	83 c0 04             	add    eax,0x4
  4377b5:	4a                   	dec    edx
  4377b6:	75 f5                	jne    0x4377ad
  4377b8:	5e                   	pop    esi
  4377b9:	c3                   	ret
  4377ba:	57                   	push   edi
  4377bb:	8b 7c 24 08          	mov    edi,DWORD PTR [esp+0x8]
  4377bf:	33 c0                	xor    eax,eax
  4377c1:	ab                   	stos   DWORD PTR es:[edi],eax
  4377c2:	ab                   	stos   DWORD PTR es:[edi],eax
  4377c3:	ab                   	stos   DWORD PTR es:[edi],eax
  4377c4:	5f                   	pop    edi
  4377c5:	c3                   	ret
  4377c6:	8b 44 24 04          	mov    eax,DWORD PTR [esp+0x4]
  4377ca:	33 c9                	xor    ecx,ecx
  4377cc:	83 38 00             	cmp    DWORD PTR [eax],0x0
  4377cf:	75 0d                	jne    0x4377de
  4377d1:	41                   	inc    ecx
  4377d2:	83 c0 04             	add    eax,0x4
  4377d5:	83 f9 03             	cmp    ecx,0x3
  4377d8:	7c f2                	jl     0x4377cc
  4377da:	6a 01                	push   0x1
  4377dc:	58                   	pop    eax
  4377dd:	c3                   	ret
  4377de:	33 c0                	xor    eax,eax
  4377e0:	c3                   	ret
  4377e1:	55                   	push   ebp
  4377e2:	8b ec                	mov    ebp,esp
  4377e4:	83 ec 0c             	sub    esp,0xc
  4377e7:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  4377ea:	53                   	push   ebx
  4377eb:	56                   	push   esi
  4377ec:	57                   	push   edi
  4377ed:	6a 20                	push   0x20
  4377ef:	8b 7d 08             	mov    edi,DWORD PTR [ebp+0x8]
  4377f2:	5b                   	pop    ebx
  4377f3:	83 ce ff             	or     esi,0xffffffff
  4377f6:	99                   	cdq
  4377f7:	8b cb                	mov    ecx,ebx
  4377f9:	c7 45 fc 03 00 00 00 	mov    DWORD PTR [ebp-0x4],0x3
  437800:	f7 f9                	idiv   ecx
  437802:	89 45 f4             	mov    DWORD PTR [ebp-0xc],eax
  437805:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  437808:	99                   	cdq
  437809:	f7 f9                	idiv   ecx
  43780b:	83 65 0c 00          	and    DWORD PTR [ebp+0xc],0x0
  43780f:	8b ca                	mov    ecx,edx
  437811:	d3 e6                	shl    esi,cl
  437813:	2b da                	sub    ebx,edx
  437815:	f7 d6                	not    esi
  437817:	8b 07                	mov    eax,DWORD PTR [edi]
  437819:	8b c8                	mov    ecx,eax
  43781b:	23 ce                	and    ecx,esi
  43781d:	89 4d f8             	mov    DWORD PTR [ebp-0x8],ecx
  437820:	8b ca                	mov    ecx,edx
  437822:	d3 e8                	shr    eax,cl
  437824:	0b 45 0c             	or     eax,DWORD PTR [ebp+0xc]
  437827:	89 07                	mov    DWORD PTR [edi],eax
  437829:	8b 45 f8             	mov    eax,DWORD PTR [ebp-0x8]
  43782c:	8b cb                	mov    ecx,ebx
  43782e:	83 c7 04             	add    edi,0x4
  437831:	d3 e0                	shl    eax,cl
  437833:	ff 4d fc             	dec    DWORD PTR [ebp-0x4]
  437836:	89 45 0c             	mov    DWORD PTR [ebp+0xc],eax
  437839:	75 dc                	jne    0x437817
  43783b:	8b 7d f4             	mov    edi,DWORD PTR [ebp-0xc]
  43783e:	6a 02                	push   0x2
  437840:	5b                   	pop    ebx
  437841:	8b f7                	mov    esi,edi
  437843:	6a 08                	push   0x8
  437845:	59                   	pop    ecx
  437846:	c1 e6 02             	shl    esi,0x2
  437849:	3b df                	cmp    ebx,edi
  43784b:	7c 0f                	jl     0x43785c
  43784d:	8b 55 08             	mov    edx,DWORD PTR [ebp+0x8]
  437850:	8b c1                	mov    eax,ecx
  437852:	2b c6                	sub    eax,esi
  437854:	8b 04 10             	mov    eax,DWORD PTR [eax+edx*1]
  437857:	89 04 11             	mov    DWORD PTR [ecx+edx*1],eax
  43785a:	eb 07                	jmp    0x437863
  43785c:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  43785f:	83 24 01 00          	and    DWORD PTR [ecx+eax*1],0x0
  437863:	4b                   	dec    ebx
  437864:	83 e9 04             	sub    ecx,0x4
  437867:	79 e0                	jns    0x437849
  437869:	5f                   	pop    edi
  43786a:	5e                   	pop    esi
  43786b:	5b                   	pop    ebx
  43786c:	c9                   	leave
  43786d:	c3                   	ret
  43786e:	55                   	push   ebp
  43786f:	8b ec                	mov    ebp,esp
  437871:	83 ec 18             	sub    esp,0x18
  437874:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  437877:	53                   	push   ebx
  437878:	56                   	push   esi
  437879:	57                   	push   edi
  43787a:	0f b7 48 0a          	movzx  ecx,WORD PTR [eax+0xa]
  43787e:	8b d9                	mov    ebx,ecx
  437880:	81 e1 00 80 00 00    	and    ecx,0x8000
  437886:	89 4d 08             	mov    DWORD PTR [ebp+0x8],ecx
  437889:	8b 48 06             	mov    ecx,DWORD PTR [eax+0x6]
  43788c:	89 4d f4             	mov    DWORD PTR [ebp-0xc],ecx
  43788f:	8b 48 02             	mov    ecx,DWORD PTR [eax+0x2]
  437892:	0f b7 00             	movzx  eax,WORD PTR [eax]
  437895:	8b 7d 10             	mov    edi,DWORD PTR [ebp+0x10]
  437898:	81 e3 ff 7f 00 00    	and    ebx,0x7fff
  43789e:	81 eb ff 3f 00 00    	sub    ebx,0x3fff
  4378a4:	89 4d f8             	mov    DWORD PTR [ebp-0x8],ecx
  4378a7:	c1 e0 10             	shl    eax,0x10
  4378aa:	81 fb 01 c0 ff ff    	cmp    ebx,0xffffc001
  4378b0:	89 45 fc             	mov    DWORD PTR [ebp-0x4],eax
  4378b3:	75 26                	jne    0x4378db
  4378b5:	8d 45 f4             	lea    eax,[ebp-0xc]
  4378b8:	33 f6                	xor    esi,esi
  4378ba:	50                   	push   eax
  4378bb:	e8 06 ff ff ff       	call   0x4377c6
  4378c0:	85 c0                	test   eax,eax
  4378c2:	59                   	pop    ecx
  4378c3:	0f 85 d1 00 00 00    	jne    0x43799a
  4378c9:	8d 45 f4             	lea    eax,[ebp-0xc]
  4378cc:	50                   	push   eax
  4378cd:	e8 e8 fe ff ff       	call   0x4377ba
  4378d2:	59                   	pop    ecx
  4378d3:	6a 02                	push   0x2
  4378d5:	58                   	pop    eax
  4378d6:	e9 c1 00 00 00       	jmp    0x43799c
  4378db:	8d 45 f4             	lea    eax,[ebp-0xc]
  4378de:	50                   	push   eax
  4378df:	8d 45 e8             	lea    eax,[ebp-0x18]
  4378e2:	50                   	push   eax
  4378e3:	e8 b7 fe ff ff       	call   0x43779f
  4378e8:	ff 77 08             	push   DWORD PTR [edi+0x8]
  4378eb:	8d 45 f4             	lea    eax,[ebp-0xc]
  4378ee:	50                   	push   eax
  4378ef:	e8 1f fe ff ff       	call   0x437713
  4378f4:	83 c4 10             	add    esp,0x10
  4378f7:	85 c0                	test   eax,eax
  4378f9:	74 01                	je     0x4378fc
  4378fb:	43                   	inc    ebx
  4378fc:	8b 47 04             	mov    eax,DWORD PTR [edi+0x4]
  4378ff:	8b c8                	mov    ecx,eax
  437901:	2b 4f 08             	sub    ecx,DWORD PTR [edi+0x8]
  437904:	3b d9                	cmp    ebx,ecx
  437906:	7d 0c                	jge    0x437914
  437908:	8d 45 f4             	lea    eax,[ebp-0xc]
  43790b:	50                   	push   eax
  43790c:	e8 a9 fe ff ff       	call   0x4377ba
  437911:	59                   	pop    ecx
  437912:	eb 3c                	jmp    0x437950
  437914:	3b d8                	cmp    ebx,eax
  437916:	7f 3f                	jg     0x437957
  437918:	2b c3                	sub    eax,ebx
  43791a:	8b f0                	mov    esi,eax
  43791c:	8d 45 e8             	lea    eax,[ebp-0x18]
  43791f:	50                   	push   eax
  437920:	8d 45 f4             	lea    eax,[ebp-0xc]
  437923:	50                   	push   eax
  437924:	e8 76 fe ff ff       	call   0x43779f
  437929:	8d 45 f4             	lea    eax,[ebp-0xc]
  43792c:	56                   	push   esi
  43792d:	50                   	push   eax
  43792e:	e8 ae fe ff ff       	call   0x4377e1
  437933:	ff 77 08             	push   DWORD PTR [edi+0x8]
  437936:	8d 45 f4             	lea    eax,[ebp-0xc]
  437939:	50                   	push   eax
  43793a:	e8 d4 fd ff ff       	call   0x437713
  43793f:	8b 47 0c             	mov    eax,DWORD PTR [edi+0xc]
  437942:	40                   	inc    eax
  437943:	50                   	push   eax
  437944:	8d 45 f4             	lea    eax,[ebp-0xc]
  437947:	50                   	push   eax
  437948:	e8 94 fe ff ff       	call   0x4377e1
  43794d:	83 c4 20             	add    esp,0x20
  437950:	33 f6                	xor    esi,esi
  437952:	e9 7c ff ff ff       	jmp    0x4378d3
  437957:	3b 1f                	cmp    ebx,DWORD PTR [edi]
  437959:	7c 28                	jl     0x437983
  43795b:	8d 45 f4             	lea    eax,[ebp-0xc]
  43795e:	50                   	push   eax
  43795f:	e8 56 fe ff ff       	call   0x4377ba
  437964:	ff 77 0c             	push   DWORD PTR [edi+0xc]
  437967:	80 4d f7 80          	or     BYTE PTR [ebp-0x9],0x80
  43796b:	8d 45 f4             	lea    eax,[ebp-0xc]
  43796e:	50                   	push   eax
  43796f:	e8 6d fe ff ff       	call   0x4377e1
  437974:	8b 77 14             	mov    esi,DWORD PTR [edi+0x14]
  437977:	83 c4 0c             	add    esp,0xc
  43797a:	03 37                	add    esi,DWORD PTR [edi]
  43797c:	6a 01                	push   0x1
  43797e:	e9 52 ff ff ff       	jmp    0x4378d5
  437983:	ff 77 0c             	push   DWORD PTR [edi+0xc]
  437986:	8b 77 14             	mov    esi,DWORD PTR [edi+0x14]
  437989:	80 65 f7 7f          	and    BYTE PTR [ebp-0x9],0x7f
  43798d:	8d 45 f4             	lea    eax,[ebp-0xc]
  437990:	50                   	push   eax
  437991:	03 f3                	add    esi,ebx
  437993:	e8 49 fe ff ff       	call   0x4377e1
  437998:	59                   	pop    ecx
  437999:	59                   	pop    ecx
  43799a:	33 c0                	xor    eax,eax
  43799c:	6a 1f                	push   0x1f
  43799e:	59                   	pop    ecx
  43799f:	2b 4f 0c             	sub    ecx,DWORD PTR [edi+0xc]
  4379a2:	8b 7f 10             	mov    edi,DWORD PTR [edi+0x10]
  4379a5:	d3 e6                	shl    esi,cl
  4379a7:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  4379aa:	f7 d9                	neg    ecx
  4379ac:	1b c9                	sbb    ecx,ecx
  4379ae:	81 e1 00 00 00 80    	and    ecx,0x80000000
  4379b4:	0b f1                	or     esi,ecx
  4379b6:	0b 75 f4             	or     esi,DWORD PTR [ebp-0xc]
  4379b9:	83 ff 40             	cmp    edi,0x40
  4379bc:	75 0d                	jne    0x4379cb
  4379be:	8b 4d 0c             	mov    ecx,DWORD PTR [ebp+0xc]
  4379c1:	8b 55 f8             	mov    edx,DWORD PTR [ebp-0x8]
  4379c4:	89 71 04             	mov    DWORD PTR [ecx+0x4],esi
  4379c7:	89 11                	mov    DWORD PTR [ecx],edx
  4379c9:	eb 0a                	jmp    0x4379d5
  4379cb:	83 ff 20             	cmp    edi,0x20
  4379ce:	75 05                	jne    0x4379d5
  4379d0:	8b 4d 0c             	mov    ecx,DWORD PTR [ebp+0xc]
  4379d3:	89 31                	mov    DWORD PTR [ecx],esi
  4379d5:	5f                   	pop    edi
  4379d6:	5e                   	pop    esi
  4379d7:	5b                   	pop    ebx
  4379d8:	c9                   	leave
  4379d9:	c3                   	ret
