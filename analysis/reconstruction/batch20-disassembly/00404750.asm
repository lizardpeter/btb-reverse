
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00404750 <.text+0x3750>:
  404750:	83 ec 28             	sub    esp,0x28
  404753:	53                   	push   ebx
  404754:	8b 1d 74 b2 43 00    	mov    ebx,DWORD PTR ds:0x43b274
  40475a:	56                   	push   esi
  40475b:	8b f1                	mov    esi,ecx
  40475d:	57                   	push   edi
  40475e:	6a 00                	push   0x0
  404760:	8b 46 04             	mov    eax,DWORD PTR [esi+0x4]
  404763:	8d 7e 1c             	lea    edi,[esi+0x1c]
  404766:	6a 00                	push   0x0
  404768:	57                   	push   edi
  404769:	50                   	push   eax
  40476a:	c7 06 00 00 00 00    	mov    DWORD PTR [esi],0x0
  404770:	ff d3                	call   ebx
  404772:	85 c0                	test   eax,eax
  404774:	0f 85 51 01 00 00    	jne    0x4048cb
  40477a:	81 3f 52 49 46 46    	cmp    DWORD PTR [edi],0x46464952
  404780:	0f 85 45 01 00 00    	jne    0x4048cb
  404786:	81 7e 24 57 41 56 45 	cmp    DWORD PTR [esi+0x24],0x45564157
  40478d:	0f 85 38 01 00 00    	jne    0x4048cb
  404793:	8b 56 04             	mov    edx,DWORD PTR [esi+0x4]
  404796:	6a 10                	push   0x10
  404798:	8d 4c 24 24          	lea    ecx,[esp+0x24]
  40479c:	57                   	push   edi
  40479d:	51                   	push   ecx
  40479e:	52                   	push   edx
  40479f:	c7 44 24 30 66 6d 74 	mov    DWORD PTR [esp+0x30],0x20746d66
  4047a6:	20 
  4047a7:	ff d3                	call   ebx
  4047a9:	85 c0                	test   eax,eax
  4047ab:	0f 85 1a 01 00 00    	jne    0x4048cb
  4047b1:	83 7c 24 24 10       	cmp    DWORD PTR [esp+0x24],0x10
  4047b6:	0f 82 0f 01 00 00    	jb     0x4048cb
  4047bc:	8b 4e 04             	mov    ecx,DWORD PTR [esi+0x4]
  4047bf:	8b 3d 70 b2 43 00    	mov    edi,DWORD PTR ds:0x43b270
  4047c5:	8d 44 24 10          	lea    eax,[esp+0x10]
  4047c9:	6a 10                	push   0x10
  4047cb:	50                   	push   eax
  4047cc:	51                   	push   ecx
  4047cd:	ff d7                	call   edi
  4047cf:	83 f8 10             	cmp    eax,0x10
  4047d2:	0f 85 f3 00 00 00    	jne    0x4048cb
  4047d8:	66 83 7c 24 10 01    	cmp    WORD PTR [esp+0x10],0x1
  4047de:	75 3c                	jne    0x40481c
  4047e0:	6a 12                	push   0x12
  4047e2:	e8 e7 b3 02 00       	call   0x42fbce
  4047e7:	83 c4 04             	add    esp,0x4
  4047ea:	89 06                	mov    DWORD PTR [esi],eax
  4047ec:	85 c0                	test   eax,eax
  4047ee:	0f 84 d7 00 00 00    	je     0x4048cb
  4047f4:	8b 54 24 10          	mov    edx,DWORD PTR [esp+0x10]
  4047f8:	89 10                	mov    DWORD PTR [eax],edx
  4047fa:	8b 4c 24 14          	mov    ecx,DWORD PTR [esp+0x14]
  4047fe:	89 48 04             	mov    DWORD PTR [eax+0x4],ecx
  404801:	8b 54 24 18          	mov    edx,DWORD PTR [esp+0x18]
  404805:	89 50 08             	mov    DWORD PTR [eax+0x8],edx
  404808:	8b 4c 24 1c          	mov    ecx,DWORD PTR [esp+0x1c]
  40480c:	89 48 0c             	mov    DWORD PTR [eax+0xc],ecx
  40480f:	8b 16                	mov    edx,DWORD PTR [esi]
  404811:	66 c7 42 10 00 00    	mov    WORD PTR [edx+0x10],0x0
  404817:	e9 85 00 00 00       	jmp    0x4048a1
  40481c:	8b 4e 04             	mov    ecx,DWORD PTR [esi+0x4]
  40481f:	8d 44 24 0c          	lea    eax,[esp+0xc]
  404823:	6a 02                	push   0x2
  404825:	50                   	push   eax
  404826:	51                   	push   ecx
  404827:	c7 44 24 18 00 00 00 	mov    DWORD PTR [esp+0x18],0x0
  40482e:	00 
  40482f:	ff d7                	call   edi
  404831:	83 f8 02             	cmp    eax,0x2
  404834:	0f 85 91 00 00 00    	jne    0x4048cb
  40483a:	8b 54 24 0c          	mov    edx,DWORD PTR [esp+0xc]
  40483e:	81 e2 ff ff 00 00    	and    edx,0xffff
  404844:	83 c2 12             	add    edx,0x12
  404847:	52                   	push   edx
  404848:	e8 81 b3 02 00       	call   0x42fbce
  40484d:	83 c4 04             	add    esp,0x4
  404850:	89 06                	mov    DWORD PTR [esi],eax
  404852:	85 c0                	test   eax,eax
  404854:	74 75                	je     0x4048cb
  404856:	8b 4c 24 10          	mov    ecx,DWORD PTR [esp+0x10]
  40485a:	89 08                	mov    DWORD PTR [eax],ecx
  40485c:	8b 54 24 14          	mov    edx,DWORD PTR [esp+0x14]
  404860:	89 50 04             	mov    DWORD PTR [eax+0x4],edx
  404863:	8b 4c 24 18          	mov    ecx,DWORD PTR [esp+0x18]
  404867:	89 48 08             	mov    DWORD PTR [eax+0x8],ecx
  40486a:	8b 54 24 1c          	mov    edx,DWORD PTR [esp+0x1c]
  40486e:	89 50 0c             	mov    DWORD PTR [eax+0xc],edx
  404871:	8b 06                	mov    eax,DWORD PTR [esi]
  404873:	66 8b 4c 24 0c       	mov    cx,WORD PTR [esp+0xc]
  404878:	66 89 48 10          	mov    WORD PTR [eax+0x10],cx
  40487c:	8b 54 24 0c          	mov    edx,DWORD PTR [esp+0xc]
  404880:	8b 06                	mov    eax,DWORD PTR [esi]
  404882:	8b 4e 04             	mov    ecx,DWORD PTR [esi+0x4]
  404885:	81 e2 ff ff 00 00    	and    edx,0xffff
  40488b:	83 c0 12             	add    eax,0x12
  40488e:	52                   	push   edx
  40488f:	50                   	push   eax
  404890:	51                   	push   ecx
  404891:	ff d7                	call   edi
  404893:	8b 54 24 0c          	mov    edx,DWORD PTR [esp+0xc]
  404897:	81 e2 ff ff 00 00    	and    edx,0xffff
  40489d:	3b c2                	cmp    eax,edx
  40489f:	75 15                	jne    0x4048b6
  4048a1:	8b 4e 04             	mov    ecx,DWORD PTR [esi+0x4]
  4048a4:	8d 44 24 20          	lea    eax,[esp+0x20]
  4048a8:	6a 00                	push   0x0
  4048aa:	50                   	push   eax
  4048ab:	51                   	push   ecx
  4048ac:	ff 15 6c b2 43 00    	call   DWORD PTR ds:0x43b26c
  4048b2:	85 c0                	test   eax,eax
  4048b4:	74 21                	je     0x4048d7
  4048b6:	8b 06                	mov    eax,DWORD PTR [esi]
  4048b8:	85 c0                	test   eax,eax
  4048ba:	74 0f                	je     0x4048cb
  4048bc:	50                   	push   eax
  4048bd:	e8 1a b3 02 00       	call   0x42fbdc
  4048c2:	83 c4 04             	add    esp,0x4
  4048c5:	c7 06 00 00 00 00    	mov    DWORD PTR [esi],0x0
  4048cb:	5f                   	pop    edi
  4048cc:	5e                   	pop    esi
  4048cd:	b8 05 40 00 80       	mov    eax,0x80004005
  4048d2:	5b                   	pop    ebx
  4048d3:	83 c4 28             	add    esp,0x28
  4048d6:	c3                   	ret
  4048d7:	5f                   	pop    edi
  4048d8:	5e                   	pop    esi
  4048d9:	33 c0                	xor    eax,eax
  4048db:	5b                   	pop    ebx
  4048dc:	83 c4 28             	add    esp,0x28
  4048df:	c3                   	ret
