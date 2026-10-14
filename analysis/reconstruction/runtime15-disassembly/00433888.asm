
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00433888 <.text+0x32888>:
  433888:	56                   	push   esi
  433889:	8b 74 24 08          	mov    esi,DWORD PTR [esp+0x8]
  43388d:	8b 46 0c             	mov    eax,DWORD PTR [esi+0xc]
  433890:	a8 83                	test   al,0x83
  433892:	0f 84 c4 00 00 00    	je     0x43395c
  433898:	a8 40                	test   al,0x40
  43389a:	0f 85 bc 00 00 00    	jne    0x43395c
  4338a0:	a8 02                	test   al,0x2
  4338a2:	74 0a                	je     0x4338ae
  4338a4:	0c 20                	or     al,0x20
  4338a6:	89 46 0c             	mov    DWORD PTR [esi+0xc],eax
  4338a9:	e9 ae 00 00 00       	jmp    0x43395c
  4338ae:	0c 01                	or     al,0x1
  4338b0:	66 a9 0c 01          	test   ax,0x10c
  4338b4:	89 46 0c             	mov    DWORD PTR [esi+0xc],eax
  4338b7:	75 09                	jne    0x4338c2
  4338b9:	56                   	push   esi
  4338ba:	e8 b2 3a 00 00       	call   0x437371
  4338bf:	59                   	pop    ecx
  4338c0:	eb 05                	jmp    0x4338c7
  4338c2:	8b 46 08             	mov    eax,DWORD PTR [esi+0x8]
  4338c5:	89 06                	mov    DWORD PTR [esi],eax
  4338c7:	ff 76 18             	push   DWORD PTR [esi+0x18]
  4338ca:	ff 76 08             	push   DWORD PTR [esi+0x8]
  4338cd:	ff 76 10             	push   DWORD PTR [esi+0x10]
  4338d0:	e8 8c 00 00 00       	call   0x433961
  4338d5:	83 c4 0c             	add    esp,0xc
  4338d8:	89 46 04             	mov    DWORD PTR [esi+0x4],eax
  4338db:	85 c0                	test   eax,eax
  4338dd:	74 6c                	je     0x43394b
  4338df:	83 f8 ff             	cmp    eax,0xffffffff
  4338e2:	74 67                	je     0x43394b
  4338e4:	8b 56 0c             	mov    edx,DWORD PTR [esi+0xc]
  4338e7:	f6 c2 82             	test   dl,0x82
  4338ea:	75 34                	jne    0x433920
  4338ec:	8b 4e 10             	mov    ecx,DWORD PTR [esi+0x10]
  4338ef:	57                   	push   edi
  4338f0:	83 f9 ff             	cmp    ecx,0xffffffff
  4338f3:	74 14                	je     0x433909
  4338f5:	8b f9                	mov    edi,ecx
  4338f7:	c1 ff 05             	sar    edi,0x5
  4338fa:	83 e1 1f             	and    ecx,0x1f
  4338fd:	8b 3c bd 20 d9 51 00 	mov    edi,DWORD PTR [edi*4+0x51d920]
  433904:	8d 3c cf             	lea    edi,[edi+ecx*8]
  433907:	eb 05                	jmp    0x43390e
  433909:	bf f8 97 44 00       	mov    edi,0x4497f8
  43390e:	8a 4f 04             	mov    cl,BYTE PTR [edi+0x4]
  433911:	5f                   	pop    edi
  433912:	80 e1 82             	and    cl,0x82
  433915:	80 f9 82             	cmp    cl,0x82
  433918:	75 06                	jne    0x433920
  43391a:	80 ce 20             	or     dh,0x20
  43391d:	89 56 0c             	mov    DWORD PTR [esi+0xc],edx
  433920:	81 7e 18 00 02 00 00 	cmp    DWORD PTR [esi+0x18],0x200
  433927:	75 14                	jne    0x43393d
  433929:	8b 4e 0c             	mov    ecx,DWORD PTR [esi+0xc]
  43392c:	f6 c1 08             	test   cl,0x8
  43392f:	74 0c                	je     0x43393d
  433931:	f6 c5 04             	test   ch,0x4
  433934:	75 07                	jne    0x43393d
  433936:	c7 46 18 00 10 00 00 	mov    DWORD PTR [esi+0x18],0x1000
  43393d:	8b 0e                	mov    ecx,DWORD PTR [esi]
  43393f:	48                   	dec    eax
  433940:	89 46 04             	mov    DWORD PTR [esi+0x4],eax
  433943:	0f b6 01             	movzx  eax,BYTE PTR [ecx]
  433946:	41                   	inc    ecx
  433947:	89 0e                	mov    DWORD PTR [esi],ecx
  433949:	5e                   	pop    esi
  43394a:	c3                   	ret
  43394b:	f7 d8                	neg    eax
  43394d:	1b c0                	sbb    eax,eax
  43394f:	83 e0 10             	and    eax,0x10
  433952:	83 c0 10             	add    eax,0x10
  433955:	09 46 0c             	or     DWORD PTR [esi+0xc],eax
  433958:	83 66 04 00          	and    DWORD PTR [esi+0x4],0x0
  43395c:	83 c8 ff             	or     eax,0xffffffff
  43395f:	5e                   	pop    esi
  433960:	c3                   	ret
