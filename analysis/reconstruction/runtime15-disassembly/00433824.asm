
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00433824 <.text+0x32824>:
  433824:	56                   	push   esi
  433825:	8b 74 24 08          	mov    esi,DWORD PTR [esp+0x8]
  433829:	8b 06                	mov    eax,DWORD PTR [esi]
  43382b:	81 38 63 73 6d e0    	cmp    DWORD PTR [eax],0xe06d7363
  433831:	75 14                	jne    0x433847
  433833:	83 78 10 03          	cmp    DWORD PTR [eax+0x10],0x3
  433837:	75 0e                	jne    0x433847
  433839:	81 78 14 20 05 93 19 	cmp    DWORD PTR [eax+0x14],0x19930520
  433840:	75 05                	jne    0x433847
  433842:	e9 c5 ed ff ff       	jmp    0x43260c
  433847:	a1 40 c4 51 00       	mov    eax,ds:0x51c440
  43384c:	85 c0                	test   eax,eax
  43384e:	74 14                	je     0x433864
  433850:	50                   	push   eax
  433851:	e8 ef 2e 00 00       	call   0x436745
  433856:	85 c0                	test   eax,eax
  433858:	59                   	pop    ecx
  433859:	74 09                	je     0x433864
  43385b:	56                   	push   esi
  43385c:	ff 15 40 c4 51 00    	call   DWORD PTR ds:0x51c440
  433862:	eb 02                	jmp    0x433866
  433864:	33 c0                	xor    eax,eax
  433866:	5e                   	pop    esi
  433867:	c2 04 00             	ret    0x4
