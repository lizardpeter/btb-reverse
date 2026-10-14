
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004388a1 <.text+0x378a1>:
  4388a1:	55                   	push   ebp
  4388a2:	8b ec                	mov    ebp,esp
  4388a4:	83 ec 10             	sub    esp,0x10
  4388a7:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  4388aa:	53                   	push   ebx
  4388ab:	8b 5d 10             	mov    ebx,DWORD PTR [ebp+0x10]
  4388ae:	33 d2                	xor    edx,edx
  4388b0:	3b c2                	cmp    eax,edx
  4388b2:	56                   	push   esi
  4388b3:	c7 45 fc 4e 40 00 00 	mov    DWORD PTR [ebp-0x4],0x404e
  4388ba:	89 13                	mov    DWORD PTR [ebx],edx
  4388bc:	89 53 04             	mov    DWORD PTR [ebx+0x4],edx
  4388bf:	89 53 08             	mov    DWORD PTR [ebx+0x8],edx
  4388c2:	76 51                	jbe    0x438915
  4388c4:	57                   	push   edi
  4388c5:	89 45 10             	mov    DWORD PTR [ebp+0x10],eax
  4388c8:	8b f3                	mov    esi,ebx
  4388ca:	8d 7d f0             	lea    edi,[ebp-0x10]
  4388cd:	a5                   	movs   DWORD PTR es:[edi],DWORD PTR ds:[esi]
  4388ce:	a5                   	movs   DWORD PTR es:[edi],DWORD PTR ds:[esi]
  4388cf:	53                   	push   ebx
  4388d0:	a5                   	movs   DWORD PTR es:[edi],DWORD PTR ds:[esi]
  4388d1:	e8 70 ff ff ff       	call   0x438846
  4388d6:	53                   	push   ebx
  4388d7:	e8 6a ff ff ff       	call   0x438846
  4388dc:	8d 45 f0             	lea    eax,[ebp-0x10]
  4388df:	50                   	push   eax
  4388e0:	53                   	push   ebx
  4388e1:	e8 02 ff ff ff       	call   0x4387e8
  4388e6:	53                   	push   ebx
  4388e7:	e8 5a ff ff ff       	call   0x438846
  4388ec:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  4388ef:	83 65 f4 00          	and    DWORD PTR [ebp-0xc],0x0
  4388f3:	83 65 f8 00          	and    DWORD PTR [ebp-0x8],0x0
  4388f7:	0f be 00             	movsx  eax,BYTE PTR [eax]
  4388fa:	89 45 f0             	mov    DWORD PTR [ebp-0x10],eax
  4388fd:	8d 45 f0             	lea    eax,[ebp-0x10]
  438900:	50                   	push   eax
  438901:	53                   	push   ebx
  438902:	e8 e1 fe ff ff       	call   0x4387e8
  438907:	83 c4 1c             	add    esp,0x1c
  43890a:	ff 45 08             	inc    DWORD PTR [ebp+0x8]
  43890d:	ff 4d 10             	dec    DWORD PTR [ebp+0x10]
  438910:	75 b6                	jne    0x4388c8
  438912:	33 d2                	xor    edx,edx
  438914:	5f                   	pop    edi
  438915:	39 53 08             	cmp    DWORD PTR [ebx+0x8],edx
  438918:	75 28                	jne    0x438942
  43891a:	8b 4b 04             	mov    ecx,DWORD PTR [ebx+0x4]
  43891d:	8b c1                	mov    eax,ecx
  43891f:	c1 e8 10             	shr    eax,0x10
  438922:	89 43 08             	mov    DWORD PTR [ebx+0x8],eax
  438925:	8b 03                	mov    eax,DWORD PTR [ebx]
  438927:	8b f0                	mov    esi,eax
  438929:	c1 ee 10             	shr    esi,0x10
  43892c:	c1 e1 10             	shl    ecx,0x10
  43892f:	0b f1                	or     esi,ecx
  438931:	c1 e0 10             	shl    eax,0x10
  438934:	81 45 fc f0 ff 00 00 	add    DWORD PTR [ebp-0x4],0xfff0
  43893b:	89 73 04             	mov    DWORD PTR [ebx+0x4],esi
  43893e:	89 03                	mov    DWORD PTR [ebx],eax
  438940:	eb d3                	jmp    0x438915
  438942:	be 00 80 00 00       	mov    esi,0x8000
  438947:	85 73 08             	test   DWORD PTR [ebx+0x8],esi
  43894a:	75 10                	jne    0x43895c
  43894c:	53                   	push   ebx
  43894d:	e8 f4 fe ff ff       	call   0x438846
  438952:	81 45 fc ff ff 00 00 	add    DWORD PTR [ebp-0x4],0xffff
  438959:	59                   	pop    ecx
  43895a:	eb eb                	jmp    0x438947
  43895c:	66 8b 45 fc          	mov    ax,WORD PTR [ebp-0x4]
  438960:	5e                   	pop    esi
  438961:	66 89 43 0a          	mov    WORD PTR [ebx+0xa],ax
  438965:	5b                   	pop    ebx
  438966:	c9                   	leave
  438967:	c3                   	ret
