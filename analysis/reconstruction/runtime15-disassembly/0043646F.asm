
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0043646f <.text+0x3546f>:
  43646f:	55                   	push   ebp
  436470:	8b ec                	mov    ebp,esp
  436472:	53                   	push   ebx
  436473:	56                   	push   esi
  436474:	8b 75 0c             	mov    esi,DWORD PTR [ebp+0xc]
  436477:	33 db                	xor    ebx,ebx
  436479:	3b f3                	cmp    esi,ebx
  43647b:	74 15                	je     0x436492
  43647d:	39 5d 10             	cmp    DWORD PTR [ebp+0x10],ebx
  436480:	74 10                	je     0x436492
  436482:	8a 06                	mov    al,BYTE PTR [esi]
  436484:	3a c3                	cmp    al,bl
  436486:	75 10                	jne    0x436498
  436488:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  43648b:	3b c3                	cmp    eax,ebx
  43648d:	74 03                	je     0x436492
  43648f:	66 89 18             	mov    WORD PTR [eax],bx
  436492:	33 c0                	xor    eax,eax
  436494:	5e                   	pop    esi
  436495:	5b                   	pop    ebx
  436496:	5d                   	pop    ebp
  436497:	c3                   	ret
  436498:	39 1d 80 c6 51 00    	cmp    DWORD PTR ds:0x51c680,ebx
  43649e:	75 13                	jne    0x4364b3
  4364a0:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  4364a3:	3b cb                	cmp    ecx,ebx
  4364a5:	74 07                	je     0x4364ae
  4364a7:	66 0f b6 c0          	movzx  ax,al
  4364ab:	66 89 01             	mov    WORD PTR [ecx],ax
  4364ae:	6a 01                	push   0x1
  4364b0:	58                   	pop    eax
  4364b1:	eb e1                	jmp    0x436494
  4364b3:	8b 0d 1c 9b 44 00    	mov    ecx,DWORD PTR ds:0x449b1c
  4364b9:	0f b6 c0             	movzx  eax,al
  4364bc:	f6 44 41 01 80       	test   BYTE PTR [ecx+eax*2+0x1],0x80
  4364c1:	74 4d                	je     0x436510
  4364c3:	a1 10 9b 44 00       	mov    eax,ds:0x449b10
  4364c8:	83 f8 01             	cmp    eax,0x1
  4364cb:	7e 2a                	jle    0x4364f7
  4364cd:	39 45 10             	cmp    DWORD PTR [ebp+0x10],eax
  4364d0:	7c 2f                	jl     0x436501
  4364d2:	33 c9                	xor    ecx,ecx
  4364d4:	39 5d 08             	cmp    DWORD PTR [ebp+0x8],ebx
  4364d7:	0f 95 c1             	setne  cl
  4364da:	51                   	push   ecx
  4364db:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  4364de:	50                   	push   eax
  4364df:	56                   	push   esi
  4364e0:	6a 09                	push   0x9
  4364e2:	ff 35 90 c6 51 00    	push   DWORD PTR ds:0x51c690
  4364e8:	ff 15 d0 b0 43 00    	call   DWORD PTR ds:0x43b0d0
  4364ee:	85 c0                	test   eax,eax
  4364f0:	a1 10 9b 44 00       	mov    eax,ds:0x449b10
  4364f5:	75 9d                	jne    0x436494
  4364f7:	39 45 10             	cmp    DWORD PTR [ebp+0x10],eax
  4364fa:	72 05                	jb     0x436501
  4364fc:	38 5e 01             	cmp    BYTE PTR [esi+0x1],bl
  4364ff:	75 93                	jne    0x436494
  436501:	c7 05 c8 c3 51 00 2a 	mov    DWORD PTR ds:0x51c3c8,0x2a
  436508:	00 00 00 
  43650b:	83 c8 ff             	or     eax,0xffffffff
  43650e:	eb 84                	jmp    0x436494
  436510:	33 c0                	xor    eax,eax
  436512:	39 5d 08             	cmp    DWORD PTR [ebp+0x8],ebx
  436515:	0f 95 c0             	setne  al
  436518:	50                   	push   eax
  436519:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  43651c:	6a 01                	push   0x1
  43651e:	56                   	push   esi
  43651f:	6a 09                	push   0x9
  436521:	ff 35 90 c6 51 00    	push   DWORD PTR ds:0x51c690
  436527:	ff 15 d0 b0 43 00    	call   DWORD PTR ds:0x43b0d0
  43652d:	85 c0                	test   eax,eax
  43652f:	0f 85 79 ff ff ff    	jne    0x4364ae
  436535:	eb ca                	jmp    0x436501
