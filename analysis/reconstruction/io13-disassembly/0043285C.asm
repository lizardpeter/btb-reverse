
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0043285c <.text+0x3185c>:
  43285c:	55                   	push   ebp
  43285d:	8b ec                	mov    ebp,esp
  43285f:	53                   	push   ebx
  432860:	56                   	push   esi
  432861:	8b 75 0c             	mov    esi,DWORD PTR [ebp+0xc]
  432864:	8b 46 0c             	mov    eax,DWORD PTR [esi+0xc]
  432867:	8b 5e 10             	mov    ebx,DWORD PTR [esi+0x10]
  43286a:	a8 82                	test   al,0x82
  43286c:	0f 84 f3 00 00 00    	je     0x432965
  432872:	a8 40                	test   al,0x40
  432874:	0f 85 eb 00 00 00    	jne    0x432965
  43287a:	a8 01                	test   al,0x1
  43287c:	74 16                	je     0x432894
  43287e:	83 66 04 00          	and    DWORD PTR [esi+0x4],0x0
  432882:	a8 10                	test   al,0x10
  432884:	0f 84 db 00 00 00    	je     0x432965
  43288a:	8b 4e 08             	mov    ecx,DWORD PTR [esi+0x8]
  43288d:	24 fe                	and    al,0xfe
  43288f:	89 0e                	mov    DWORD PTR [esi],ecx
  432891:	89 46 0c             	mov    DWORD PTR [esi+0xc],eax
  432894:	8b 46 0c             	mov    eax,DWORD PTR [esi+0xc]
  432897:	83 66 04 00          	and    DWORD PTR [esi+0x4],0x0
  43289b:	83 65 0c 00          	and    DWORD PTR [ebp+0xc],0x0
  43289f:	24 ef                	and    al,0xef
  4328a1:	0c 02                	or     al,0x2
  4328a3:	66 a9 0c 01          	test   ax,0x10c
  4328a7:	89 46 0c             	mov    DWORD PTR [esi+0xc],eax
  4328aa:	75 22                	jne    0x4328ce
  4328ac:	81 fe b0 98 44 00    	cmp    esi,0x4498b0
  4328b2:	74 08                	je     0x4328bc
  4328b4:	81 fe d0 98 44 00    	cmp    esi,0x4498d0
  4328ba:	75 0b                	jne    0x4328c7
  4328bc:	53                   	push   ebx
  4328bd:	e8 f3 4a 00 00       	call   0x4373b5
  4328c2:	85 c0                	test   eax,eax
  4328c4:	59                   	pop    ecx
  4328c5:	75 07                	jne    0x4328ce
  4328c7:	56                   	push   esi
  4328c8:	e8 a4 4a 00 00       	call   0x437371
  4328cd:	59                   	pop    ecx
  4328ce:	66 f7 46 0c 08 01    	test   WORD PTR [esi+0xc],0x108
  4328d4:	57                   	push   edi
  4328d5:	74 64                	je     0x43293b
  4328d7:	8b 46 08             	mov    eax,DWORD PTR [esi+0x8]
  4328da:	8b 3e                	mov    edi,DWORD PTR [esi]
  4328dc:	2b f8                	sub    edi,eax
  4328de:	8d 48 01             	lea    ecx,[eax+0x1]
  4328e1:	89 0e                	mov    DWORD PTR [esi],ecx
  4328e3:	8b 4e 18             	mov    ecx,DWORD PTR [esi+0x18]
  4328e6:	49                   	dec    ecx
  4328e7:	85 ff                	test   edi,edi
  4328e9:	89 4e 04             	mov    DWORD PTR [esi+0x4],ecx
  4328ec:	7e 10                	jle    0x4328fe
  4328ee:	57                   	push   edi
  4328ef:	50                   	push   eax
  4328f0:	53                   	push   ebx
  4328f1:	e8 87 17 00 00       	call   0x43407d
  4328f6:	83 c4 0c             	add    esp,0xc
  4328f9:	89 45 0c             	mov    DWORD PTR [ebp+0xc],eax
  4328fc:	eb 33                	jmp    0x432931
  4328fe:	83 fb ff             	cmp    ebx,0xffffffff
  432901:	74 16                	je     0x432919
  432903:	8b c3                	mov    eax,ebx
  432905:	8b cb                	mov    ecx,ebx
  432907:	c1 f8 05             	sar    eax,0x5
  43290a:	83 e1 1f             	and    ecx,0x1f
  43290d:	8b 04 85 20 d9 51 00 	mov    eax,DWORD PTR [eax*4+0x51d920]
  432914:	8d 04 c8             	lea    eax,[eax+ecx*8]
  432917:	eb 05                	jmp    0x43291e
  432919:	b8 f8 97 44 00       	mov    eax,0x4497f8
  43291e:	f6 40 04 20          	test   BYTE PTR [eax+0x4],0x20
  432922:	74 0d                	je     0x432931
  432924:	6a 02                	push   0x2
  432926:	6a 00                	push   0x0
  432928:	53                   	push   ebx
  432929:	e8 a9 49 00 00       	call   0x4372d7
  43292e:	83 c4 0c             	add    esp,0xc
  432931:	8b 46 08             	mov    eax,DWORD PTR [esi+0x8]
  432934:	8a 4d 08             	mov    cl,BYTE PTR [ebp+0x8]
  432937:	88 08                	mov    BYTE PTR [eax],cl
  432939:	eb 14                	jmp    0x43294f
  43293b:	6a 01                	push   0x1
  43293d:	8d 45 08             	lea    eax,[ebp+0x8]
  432940:	5f                   	pop    edi
  432941:	57                   	push   edi
  432942:	50                   	push   eax
  432943:	53                   	push   ebx
  432944:	e8 34 17 00 00       	call   0x43407d
  432949:	83 c4 0c             	add    esp,0xc
  43294c:	89 45 0c             	mov    DWORD PTR [ebp+0xc],eax
  43294f:	39 7d 0c             	cmp    DWORD PTR [ebp+0xc],edi
  432952:	5f                   	pop    edi
  432953:	74 06                	je     0x43295b
  432955:	83 4e 0c 20          	or     DWORD PTR [esi+0xc],0x20
  432959:	eb 0f                	jmp    0x43296a
  43295b:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  43295e:	25 ff 00 00 00       	and    eax,0xff
  432963:	eb 08                	jmp    0x43296d
  432965:	0c 20                	or     al,0x20
  432967:	89 46 0c             	mov    DWORD PTR [esi+0xc],eax
  43296a:	83 c8 ff             	or     eax,0xffffffff
  43296d:	5e                   	pop    esi
  43296e:	5b                   	pop    ebx
  43296f:	5d                   	pop    ebp
  432970:	c3                   	ret
