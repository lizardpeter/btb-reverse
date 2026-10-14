
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00437520 <.text+0x36520>:
  437520:	53                   	push   ebx
  437521:	8b 5c 24 08          	mov    ebx,DWORD PTR [esp+0x8]
  437525:	33 c0                	xor    eax,eax
  437527:	56                   	push   esi
  437528:	f6 c3 10             	test   bl,0x10
  43752b:	74 03                	je     0x437530
  43752d:	6a 01                	push   0x1
  43752f:	58                   	pop    eax
  437530:	f6 c3 08             	test   bl,0x8
  437533:	74 02                	je     0x437537
  437535:	0c 04                	or     al,0x4
  437537:	f6 c3 04             	test   bl,0x4
  43753a:	74 02                	je     0x43753e
  43753c:	0c 08                	or     al,0x8
  43753e:	f6 c3 02             	test   bl,0x2
  437541:	74 02                	je     0x437545
  437543:	0c 10                	or     al,0x10
  437545:	f6 c3 01             	test   bl,0x1
  437548:	74 02                	je     0x43754c
  43754a:	0c 20                	or     al,0x20
  43754c:	f7 c3 00 00 08 00    	test   ebx,0x80000
  437552:	74 02                	je     0x437556
  437554:	0c 02                	or     al,0x2
  437556:	8b cb                	mov    ecx,ebx
  437558:	ba 00 03 00 00       	mov    edx,0x300
  43755d:	23 ca                	and    ecx,edx
  43755f:	be 00 02 00 00       	mov    esi,0x200
  437564:	74 1d                	je     0x437583
  437566:	81 f9 00 01 00 00    	cmp    ecx,0x100
  43756c:	74 12                	je     0x437580
  43756e:	3b ce                	cmp    ecx,esi
  437570:	74 09                	je     0x43757b
  437572:	3b ca                	cmp    ecx,edx
  437574:	75 0d                	jne    0x437583
  437576:	80 cc 0c             	or     ah,0xc
  437579:	eb 08                	jmp    0x437583
  43757b:	80 cc 08             	or     ah,0x8
  43757e:	eb 03                	jmp    0x437583
  437580:	80 cc 04             	or     ah,0x4
  437583:	8b cb                	mov    ecx,ebx
  437585:	81 e1 00 00 03 00    	and    ecx,0x30000
  43758b:	74 0c                	je     0x437599
  43758d:	81 f9 00 00 01 00    	cmp    ecx,0x10000
  437593:	75 06                	jne    0x43759b
  437595:	0b c6                	or     eax,esi
  437597:	eb 02                	jmp    0x43759b
  437599:	0b c2                	or     eax,edx
  43759b:	5e                   	pop    esi
  43759c:	f7 c3 00 00 04 00    	test   ebx,0x40000
  4375a2:	5b                   	pop    ebx
  4375a3:	74 03                	je     0x4375a8
  4375a5:	80 cc 10             	or     ah,0x10
  4375a8:	c3                   	ret
