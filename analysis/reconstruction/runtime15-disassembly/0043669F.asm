
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0043669f <.text+0x3569f>:
  43669f:	53                   	push   ebx
  4366a0:	8b 5c 24 08          	mov    ebx,DWORD PTR [esp+0x8]
  4366a4:	83 fb ff             	cmp    ebx,0xffffffff
  4366a7:	56                   	push   esi
  4366a8:	74 41                	je     0x4366eb
  4366aa:	8b 74 24 10          	mov    esi,DWORD PTR [esp+0x10]
  4366ae:	8b 46 0c             	mov    eax,DWORD PTR [esi+0xc]
  4366b1:	a8 01                	test   al,0x1
  4366b3:	75 08                	jne    0x4366bd
  4366b5:	a8 80                	test   al,0x80
  4366b7:	74 32                	je     0x4366eb
  4366b9:	a8 02                	test   al,0x2
  4366bb:	75 2e                	jne    0x4366eb
  4366bd:	83 7e 08 00          	cmp    DWORD PTR [esi+0x8],0x0
  4366c1:	75 07                	jne    0x4366ca
  4366c3:	56                   	push   esi
  4366c4:	e8 a8 0c 00 00       	call   0x437371
  4366c9:	59                   	pop    ecx
  4366ca:	8b 06                	mov    eax,DWORD PTR [esi]
  4366cc:	3b 46 08             	cmp    eax,DWORD PTR [esi+0x8]
  4366cf:	75 09                	jne    0x4366da
  4366d1:	83 7e 04 00          	cmp    DWORD PTR [esi+0x4],0x0
  4366d5:	75 14                	jne    0x4366eb
  4366d7:	40                   	inc    eax
  4366d8:	89 06                	mov    DWORD PTR [esi],eax
  4366da:	f6 46 0c 40          	test   BYTE PTR [esi+0xc],0x40
  4366de:	74 11                	je     0x4366f1
  4366e0:	ff 0e                	dec    DWORD PTR [esi]
  4366e2:	8b 06                	mov    eax,DWORD PTR [esi]
  4366e4:	38 18                	cmp    BYTE PTR [eax],bl
  4366e6:	74 0f                	je     0x4366f7
  4366e8:	40                   	inc    eax
  4366e9:	89 06                	mov    DWORD PTR [esi],eax
  4366eb:	83 c8 ff             	or     eax,0xffffffff
  4366ee:	5e                   	pop    esi
  4366ef:	5b                   	pop    ebx
  4366f0:	c3                   	ret
  4366f1:	ff 0e                	dec    DWORD PTR [esi]
  4366f3:	8b 06                	mov    eax,DWORD PTR [esi]
  4366f5:	88 18                	mov    BYTE PTR [eax],bl
  4366f7:	8b 46 0c             	mov    eax,DWORD PTR [esi+0xc]
  4366fa:	ff 46 04             	inc    DWORD PTR [esi+0x4]
  4366fd:	24 ef                	and    al,0xef
  4366ff:	0c 01                	or     al,0x1
  436701:	89 46 0c             	mov    DWORD PTR [esi+0xc],eax
  436704:	8b c3                	mov    eax,ebx
  436706:	25 ff 00 00 00       	and    eax,0xff
  43670b:	eb e1                	jmp    0x4366ee
