
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00405490 <.text+0x4490>:
  405490:	83 ec 0c             	sub    esp,0xc
  405493:	53                   	push   ebx
  405494:	8b 5c 24 14          	mov    ebx,DWORD PTR [esp+0x14]
  405498:	55                   	push   ebp
  405499:	56                   	push   esi
  40549a:	57                   	push   edi
  40549b:	8b 3d 6c b0 43 00    	mov    edi,DWORD PTR ds:0x43b06c
  4054a1:	6a 08                	push   0x8
  4054a3:	8b f1                	mov    esi,ecx
  4054a5:	53                   	push   ebx
  4054a6:	ff d7                	call   edi
  4054a8:	8b e8                	mov    ebp,eax
  4054aa:	6a 0a                	push   0xa
  4054ac:	53                   	push   ebx
  4054ad:	89 6c 24 20          	mov    DWORD PTR [esp+0x20],ebp
  4054b1:	ff d7                	call   edi
  4054b3:	6a 58                	push   0x58
  4054b5:	53                   	push   ebx
  4054b6:	89 44 24 28          	mov    DWORD PTR [esp+0x28],eax
  4054ba:	ff d7                	call   edi
  4054bc:	6a 5a                	push   0x5a
  4054be:	53                   	push   ebx
  4054bf:	89 44 24 18          	mov    DWORD PTR [esp+0x18],eax
  4054c3:	ff d7                	call   edi
  4054c5:	8b 4e 04             	mov    ecx,DWORD PTR [esi+0x4]
  4054c8:	89 44 24 14          	mov    DWORD PTR [esp+0x14],eax
  4054cc:	8b 86 20 01 00 00    	mov    eax,DWORD PTR [esi+0x120]
  4054d2:	33 d2                	xor    edx,edx
  4054d4:	2b c2                	sub    eax,edx
  4054d6:	0f 84 8e 01 00 00    	je     0x40566a
  4054dc:	48                   	dec    eax
  4054dd:	0f 84 35 01 00 00    	je     0x405618
  4054e3:	48                   	dec    eax
  4054e4:	0f 85 c6 01 00 00    	jne    0x4056b0
  4054ea:	a1 70 25 48 00       	mov    eax,ds:0x482570
  4054ef:	50                   	push   eax
  4054f0:	ff 15 0c b2 43 00    	call   DWORD PTR ds:0x43b20c
  4054f6:	8b e8                	mov    ebp,eax
  4054f8:	6a 58                	push   0x58
  4054fa:	55                   	push   ebp
  4054fb:	ff d7                	call   edi
  4054fd:	89 44 24 20          	mov    DWORD PTR [esp+0x20],eax
  405501:	6a 5a                	push   0x5a
  405503:	db 44 24 24          	fild   DWORD PTR [esp+0x24]
  405507:	55                   	push   ebp
  405508:	d9 5c 24 18          	fstp   DWORD PTR [esp+0x18]
  40550c:	ff d7                	call   edi
  40550e:	89 44 24 20          	mov    DWORD PTR [esp+0x20],eax
  405512:	6a 58                	push   0x58
  405514:	db 44 24 24          	fild   DWORD PTR [esp+0x24]
  405518:	53                   	push   ebx
  405519:	d9 5c 24 1c          	fstp   DWORD PTR [esp+0x1c]
  40551d:	ff d7                	call   edi
  40551f:	89 44 24 20          	mov    DWORD PTR [esp+0x20],eax
  405523:	6a 5a                	push   0x5a
  405525:	db 44 24 24          	fild   DWORD PTR [esp+0x24]
  405529:	53                   	push   ebx
  40552a:	d9 5c 24 28          	fstp   DWORD PTR [esp+0x28]
  40552e:	ff d7                	call   edi
  405530:	89 44 24 18          	mov    DWORD PTR [esp+0x18],eax
  405534:	db 44 24 18          	fild   DWORD PTR [esp+0x18]
  405538:	d9 44 24 10          	fld    DWORD PTR [esp+0x10]
  40553c:	d8 5c 24 20          	fcomp  DWORD PTR [esp+0x20]
  405540:	df e0                	fnstsw ax
  405542:	f6 c4 41             	test   ah,0x41
  405545:	75 0a                	jne    0x405551
  405547:	d9 44 24 10          	fld    DWORD PTR [esp+0x10]
  40554b:	d8 74 24 20          	fdiv   DWORD PTR [esp+0x20]
  40554f:	eb 08                	jmp    0x405559
  405551:	d9 44 24 20          	fld    DWORD PTR [esp+0x20]
  405555:	d8 74 24 10          	fdiv   DWORD PTR [esp+0x10]
  405559:	d9 5c 24 20          	fstp   DWORD PTR [esp+0x20]
  40555d:	d9 44 24 14          	fld    DWORD PTR [esp+0x14]
  405561:	d8 d9                	fcomp  st(1)
  405563:	df e0                	fnstsw ax
  405565:	f6 c4 41             	test   ah,0x41
  405568:	75 14                	jne    0x40557e
  40556a:	d9 44 24 14          	fld    DWORD PTR [esp+0x14]
  40556e:	d8 f1                	fdiv   st,st(1)
  405570:	dc 0d 50 b3 43 00    	fmul   QWORD PTR ds:0x43b350
  405576:	d9 5c 24 14          	fstp   DWORD PTR [esp+0x14]
  40557a:	dd d8                	fstp   st(0)
  40557c:	eb 0e                	jmp    0x40558c
  40557e:	d8 74 24 14          	fdiv   DWORD PTR [esp+0x14]
  405582:	dc 0d 50 b3 43 00    	fmul   QWORD PTR ds:0x43b350
  405588:	d9 5c 24 14          	fstp   DWORD PTR [esp+0x14]
  40558c:	6a 08                	push   0x8
  40558e:	53                   	push   ebx
  40558f:	ff d7                	call   edi
  405591:	6a 0a                	push   0xa
  405593:	53                   	push   ebx
  405594:	8b e8                	mov    ebp,eax
  405596:	ff d7                	call   edi
  405598:	d9 44 24 14          	fld    DWORD PTR [esp+0x14]
  40559c:	d8 0d 4c b3 43 00    	fmul   DWORD PTR ds:0x43b34c
  4055a2:	99                   	cdq
  4055a3:	2b c2                	sub    eax,edx
  4055a5:	8b f8                	mov    edi,eax
  4055a7:	d1 ff                	sar    edi,1
  4055a9:	e8 22 af 02 00       	call   0x4304d0
  4055ae:	d9 44 24 20          	fld    DWORD PTR [esp+0x20]
  4055b2:	d8 0d 48 b3 43 00    	fmul   DWORD PTR ds:0x43b348
  4055b8:	99                   	cdq
  4055b9:	2b c2                	sub    eax,edx
  4055bb:	d1 f8                	sar    eax,1
  4055bd:	f7 d8                	neg    eax
  4055bf:	2b f8                	sub    edi,eax
  4055c1:	e8 0a af 02 00       	call   0x4304d0
  4055c6:	99                   	cdq
  4055c7:	d9 44 24 20          	fld    DWORD PTR [esp+0x20]
  4055cb:	2b c2                	sub    eax,edx
  4055cd:	89 be 14 01 00 00    	mov    DWORD PTR [esi+0x114],edi
  4055d3:	d1 f8                	sar    eax,1
  4055d5:	8b c8                	mov    ecx,eax
  4055d7:	8b c5                	mov    eax,ebp
  4055d9:	d8 0d 44 b3 43 00    	fmul   DWORD PTR ds:0x43b344
  4055df:	99                   	cdq
  4055e0:	2b c2                	sub    eax,edx
  4055e2:	f7 d9                	neg    ecx
  4055e4:	d1 f8                	sar    eax,1
  4055e6:	2b c1                	sub    eax,ecx
  4055e8:	89 86 10 01 00 00    	mov    DWORD PTR [esi+0x110],eax
  4055ee:	e8 dd ae 02 00       	call   0x4304d0
  4055f3:	d9 44 24 14          	fld    DWORD PTR [esp+0x14]
  4055f7:	d8 0d 40 b3 43 00    	fmul   DWORD PTR ds:0x43b340
  4055fd:	89 86 18 01 00 00    	mov    DWORD PTR [esi+0x118],eax
  405603:	e8 c8 ae 02 00       	call   0x4304d0
  405608:	89 86 1c 01 00 00    	mov    DWORD PTR [esi+0x11c],eax
  40560e:	5f                   	pop    edi
  40560f:	5e                   	pop    esi
  405610:	5d                   	pop    ebp
  405611:	5b                   	pop    ebx
  405612:	83 c4 0c             	add    esp,0xc
  405615:	c2 04 00             	ret    0x4
  405618:	89 96 14 01 00 00    	mov    DWORD PTR [esi+0x114],edx
  40561e:	89 96 10 01 00 00    	mov    DWORD PTR [esi+0x110],edx
  405624:	8b 54 24 20          	mov    edx,DWORD PTR [esp+0x20]
  405628:	89 96 1c 01 00 00    	mov    DWORD PTR [esi+0x11c],edx
  40562e:	8b 41 08             	mov    eax,DWORD PTR [ecx+0x8]
  405631:	db 44 24 20          	fild   DWORD PTR [esp+0x20]
  405635:	99                   	cdq
  405636:	33 c2                	xor    eax,edx
  405638:	2b c2                	sub    eax,edx
  40563a:	89 44 24 20          	mov    DWORD PTR [esp+0x20],eax
  40563e:	da 74 24 20          	fidiv  DWORD PTR [esp+0x20]
  405642:	db 44 24 10          	fild   DWORD PTR [esp+0x10]
  405646:	da 74 24 14          	fidiv  DWORD PTR [esp+0x14]
  40564a:	de c9                	fmulp  st(1),st
  40564c:	da 49 04             	fimul  DWORD PTR [ecx+0x4]
  40564f:	dc 05 38 b3 43 00    	fadd   QWORD PTR ds:0x43b338
  405655:	e8 76 ae 02 00       	call   0x4304d0
  40565a:	89 86 18 01 00 00    	mov    DWORD PTR [esi+0x118],eax
  405660:	5f                   	pop    edi
  405661:	5e                   	pop    esi
  405662:	5d                   	pop    ebp
  405663:	5b                   	pop    ebx
  405664:	83 c4 0c             	add    esp,0xc
  405667:	c2 04 00             	ret    0x4
  40566a:	89 96 14 01 00 00    	mov    DWORD PTR [esi+0x114],edx
  405670:	89 96 10 01 00 00    	mov    DWORD PTR [esi+0x110],edx
  405676:	89 ae 18 01 00 00    	mov    DWORD PTR [esi+0x118],ebp
  40567c:	8b 41 08             	mov    eax,DWORD PTR [ecx+0x8]
  40567f:	99                   	cdq
  405680:	33 c2                	xor    eax,edx
  405682:	2b c2                	sub    eax,edx
  405684:	89 44 24 20          	mov    DWORD PTR [esp+0x20],eax
  405688:	db 44 24 20          	fild   DWORD PTR [esp+0x20]
  40568c:	db 44 24 18          	fild   DWORD PTR [esp+0x18]
  405690:	da 71 04             	fidiv  DWORD PTR [ecx+0x4]
  405693:	de c9                	fmulp  st(1),st
  405695:	db 44 24 14          	fild   DWORD PTR [esp+0x14]
  405699:	da 74 24 10          	fidiv  DWORD PTR [esp+0x10]
  40569d:	de c9                	fmulp  st(1),st
  40569f:	dc 05 38 b3 43 00    	fadd   QWORD PTR ds:0x43b338
  4056a5:	e8 26 ae 02 00       	call   0x4304d0
  4056aa:	89 86 1c 01 00 00    	mov    DWORD PTR [esi+0x11c],eax
  4056b0:	5f                   	pop    edi
  4056b1:	5e                   	pop    esi
  4056b2:	5d                   	pop    ebp
  4056b3:	5b                   	pop    ebx
  4056b4:	83 c4 0c             	add    esp,0xc
  4056b7:	c2 04 00             	ret    0x4
