
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00432301 <.text+0x31301>:
  432301:	8b 45 d8             	mov    eax,DWORD PTR [ebp-0x28]
  432304:	89 46 fc             	mov    DWORD PTR [esi-0x4],eax
  432307:	8b 45 e4             	mov    eax,DWORD PTR [ebp-0x1c]
  43230a:	a3 20 c4 51 00       	mov    ds:0x51c420,eax
  43230f:	8b 45 e0             	mov    eax,DWORD PTR [ebp-0x20]
  432312:	a3 24 c4 51 00       	mov    ds:0x51c424,eax
  432317:	81 3f 63 73 6d e0    	cmp    DWORD PTR [edi],0xe06d7363
  43231d:	75 27                	jne    0x432346
  43231f:	83 7f 10 03          	cmp    DWORD PTR [edi+0x10],0x3
  432323:	75 21                	jne    0x432346
  432325:	81 7f 14 20 05 93 19 	cmp    DWORD PTR [edi+0x14],0x19930520
  43232c:	75 18                	jne    0x432346
  43232e:	39 5d dc             	cmp    DWORD PTR [ebp-0x24],ebx
  432331:	75 13                	jne    0x432346
  432333:	39 5d d4             	cmp    DWORD PTR [ebp-0x2c],ebx
  432336:	74 0e                	je     0x432346
  432338:	e8 39 dc ff ff       	call   0x42ff76
  43233d:	50                   	push   eax
  43233e:	57                   	push   edi
  43233f:	e8 f1 01 00 00       	call   0x432535
  432344:	59                   	pop    ecx
  432345:	59                   	pop    ecx
  432346:	c3                   	ret
