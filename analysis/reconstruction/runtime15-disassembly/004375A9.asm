
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004375a9 <.text+0x365a9>:
  4375a9:	55                   	push   ebp
  4375aa:	8b ec                	mov    ebp,esp
  4375ac:	51                   	push   ecx
  4375ad:	83 3d 80 c6 51 00 00 	cmp    DWORD PTR ds:0x51c680,0x0
  4375b4:	53                   	push   ebx
  4375b5:	56                   	push   esi
  4375b6:	57                   	push   edi
  4375b7:	75 1d                	jne    0x4375d6
  4375b9:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  4375bc:	83 f8 41             	cmp    eax,0x41
  4375bf:	0f 8c aa 00 00 00    	jl     0x43766f
  4375c5:	83 f8 5a             	cmp    eax,0x5a
  4375c8:	0f 8f a1 00 00 00    	jg     0x43766f
  4375ce:	83 c0 20             	add    eax,0x20
  4375d1:	e9 99 00 00 00       	jmp    0x43766f
  4375d6:	8b 5d 08             	mov    ebx,DWORD PTR [ebp+0x8]
  4375d9:	bf 00 01 00 00       	mov    edi,0x100
  4375de:	6a 01                	push   0x1
  4375e0:	3b df                	cmp    ebx,edi
  4375e2:	5e                   	pop    esi
  4375e3:	7d 25                	jge    0x43760a
  4375e5:	39 35 10 9b 44 00    	cmp    DWORD PTR ds:0x449b10,esi
  4375eb:	7e 0b                	jle    0x4375f8
  4375ed:	56                   	push   esi
  4375ee:	53                   	push   ebx
  4375ef:	e8 cc ef ff ff       	call   0x4365c0
  4375f4:	59                   	pop    ecx
  4375f5:	59                   	pop    ecx
  4375f6:	eb 0a                	jmp    0x437602
  4375f8:	a1 1c 9b 44 00       	mov    eax,ds:0x449b1c
  4375fd:	8a 04 58             	mov    al,BYTE PTR [eax+ebx*2]
  437600:	23 c6                	and    eax,esi
  437602:	85 c0                	test   eax,eax
  437604:	75 04                	jne    0x43760a
  437606:	8b c3                	mov    eax,ebx
  437608:	eb 65                	jmp    0x43766f
  43760a:	8b 15 1c 9b 44 00    	mov    edx,DWORD PTR ds:0x449b1c
  437610:	8b c3                	mov    eax,ebx
  437612:	c1 f8 08             	sar    eax,0x8
  437615:	0f b6 c8             	movzx  ecx,al
  437618:	f6 44 4a 01 80       	test   BYTE PTR [edx+ecx*2+0x1],0x80
  43761d:	74 0f                	je     0x43762e
  43761f:	80 65 0a 00          	and    BYTE PTR [ebp+0xa],0x0
  437623:	6a 02                	push   0x2
  437625:	88 45 08             	mov    BYTE PTR [ebp+0x8],al
  437628:	88 5d 09             	mov    BYTE PTR [ebp+0x9],bl
  43762b:	58                   	pop    eax
  43762c:	eb 09                	jmp    0x437637
  43762e:	80 65 09 00          	and    BYTE PTR [ebp+0x9],0x0
  437632:	88 5d 08             	mov    BYTE PTR [ebp+0x8],bl
  437635:	8b c6                	mov    eax,esi
  437637:	56                   	push   esi
  437638:	6a 00                	push   0x0
  43763a:	8d 4d fc             	lea    ecx,[ebp-0x4]
  43763d:	6a 03                	push   0x3
  43763f:	51                   	push   ecx
  437640:	50                   	push   eax
  437641:	8d 45 08             	lea    eax,[ebp+0x8]
  437644:	50                   	push   eax
  437645:	57                   	push   edi
  437646:	ff 35 80 c6 51 00    	push   DWORD PTR ds:0x51c680
  43764c:	e8 62 fa ff ff       	call   0x4370b3
  437651:	83 c4 20             	add    esp,0x20
  437654:	85 c0                	test   eax,eax
  437656:	74 ae                	je     0x437606
  437658:	3b c6                	cmp    eax,esi
  43765a:	75 06                	jne    0x437662
  43765c:	0f b6 45 fc          	movzx  eax,BYTE PTR [ebp-0x4]
  437660:	eb 0d                	jmp    0x43766f
  437662:	0f b6 45 fd          	movzx  eax,BYTE PTR [ebp-0x3]
  437666:	0f b6 4d fc          	movzx  ecx,BYTE PTR [ebp-0x4]
  43766a:	c1 e0 08             	shl    eax,0x8
  43766d:	0b c1                	or     eax,ecx
  43766f:	5f                   	pop    edi
  437670:	5e                   	pop    esi
  437671:	5b                   	pop    ebx
  437672:	c9                   	leave
  437673:	c3                   	ret
