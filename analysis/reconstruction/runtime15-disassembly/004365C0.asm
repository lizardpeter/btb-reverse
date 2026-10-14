
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004365c0 <.text+0x355c0>:
  4365c0:	55                   	push   ebp
  4365c1:	8b ec                	mov    ebp,esp
  4365c3:	51                   	push   ecx
  4365c4:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  4365c7:	8d 48 01             	lea    ecx,[eax+0x1]
  4365ca:	81 f9 00 01 00 00    	cmp    ecx,0x100
  4365d0:	77 0c                	ja     0x4365de
  4365d2:	8b 0d 1c 9b 44 00    	mov    ecx,DWORD PTR ds:0x449b1c
  4365d8:	0f b7 04 41          	movzx  eax,WORD PTR [ecx+eax*2]
  4365dc:	eb 52                	jmp    0x436630
  4365de:	8b c8                	mov    ecx,eax
  4365e0:	56                   	push   esi
  4365e1:	8b 35 1c 9b 44 00    	mov    esi,DWORD PTR ds:0x449b1c
  4365e7:	c1 f9 08             	sar    ecx,0x8
  4365ea:	0f b6 d1             	movzx  edx,cl
  4365ed:	f6 44 56 01 80       	test   BYTE PTR [esi+edx*2+0x1],0x80
  4365f2:	5e                   	pop    esi
  4365f3:	74 0e                	je     0x436603
  4365f5:	80 65 fe 00          	and    BYTE PTR [ebp-0x2],0x0
  4365f9:	88 4d fc             	mov    BYTE PTR [ebp-0x4],cl
  4365fc:	88 45 fd             	mov    BYTE PTR [ebp-0x3],al
  4365ff:	6a 02                	push   0x2
  436601:	eb 09                	jmp    0x43660c
  436603:	80 65 fd 00          	and    BYTE PTR [ebp-0x3],0x0
  436607:	88 45 fc             	mov    BYTE PTR [ebp-0x4],al
  43660a:	6a 01                	push   0x1
  43660c:	58                   	pop    eax
  43660d:	8d 4d 0a             	lea    ecx,[ebp+0xa]
  436610:	6a 01                	push   0x1
  436612:	6a 00                	push   0x0
  436614:	6a 00                	push   0x0
  436616:	51                   	push   ecx
  436617:	50                   	push   eax
  436618:	8d 45 fc             	lea    eax,[ebp-0x4]
  43661b:	50                   	push   eax
  43661c:	6a 01                	push   0x1
  43661e:	e8 e1 1d 00 00       	call   0x438404
  436623:	83 c4 1c             	add    esp,0x1c
  436626:	85 c0                	test   eax,eax
  436628:	75 02                	jne    0x43662c
  43662a:	c9                   	leave
  43662b:	c3                   	ret
  43662c:	0f b7 45 0a          	movzx  eax,WORD PTR [ebp+0xa]
  436630:	23 45 0c             	and    eax,DWORD PTR [ebp+0xc]
  436633:	c9                   	leave
  436634:	c3                   	ret
