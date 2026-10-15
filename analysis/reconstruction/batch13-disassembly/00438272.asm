
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00438272 <.text+0x37272>:
  438272:	53                   	push   ebx
  438273:	33 db                	xor    ebx,ebx
  438275:	39 1d 6c c6 51 00    	cmp    DWORD PTR ds:0x51c66c,ebx
  43827b:	56                   	push   esi
  43827c:	57                   	push   edi
  43827d:	75 42                	jne    0x4382c1
  43827f:	68 f8 ba 43 00       	push   0x43baf8
  438284:	ff 15 88 b1 43 00    	call   DWORD PTR ds:0x43b188
  43828a:	8b f8                	mov    edi,eax
  43828c:	3b fb                	cmp    edi,ebx
  43828e:	74 67                	je     0x4382f7
  438290:	8b 35 f8 b0 43 00    	mov    esi,DWORD PTR ds:0x43b0f8
  438296:	68 ec ba 43 00       	push   0x43baec
  43829b:	57                   	push   edi
  43829c:	ff d6                	call   esi
  43829e:	85 c0                	test   eax,eax
  4382a0:	a3 6c c6 51 00       	mov    ds:0x51c66c,eax
  4382a5:	74 50                	je     0x4382f7
  4382a7:	68 dc ba 43 00       	push   0x43badc
  4382ac:	57                   	push   edi
  4382ad:	ff d6                	call   esi
  4382af:	68 c8 ba 43 00       	push   0x43bac8
  4382b4:	57                   	push   edi
  4382b5:	a3 70 c6 51 00       	mov    ds:0x51c670,eax
  4382ba:	ff d6                	call   esi
  4382bc:	a3 74 c6 51 00       	mov    ds:0x51c674,eax
  4382c1:	a1 70 c6 51 00       	mov    eax,ds:0x51c670
  4382c6:	85 c0                	test   eax,eax
  4382c8:	74 16                	je     0x4382e0
  4382ca:	ff d0                	call   eax
  4382cc:	8b d8                	mov    ebx,eax
  4382ce:	85 db                	test   ebx,ebx
  4382d0:	74 0e                	je     0x4382e0
  4382d2:	a1 74 c6 51 00       	mov    eax,ds:0x51c674
  4382d7:	85 c0                	test   eax,eax
  4382d9:	74 05                	je     0x4382e0
  4382db:	53                   	push   ebx
  4382dc:	ff d0                	call   eax
  4382de:	8b d8                	mov    ebx,eax
  4382e0:	ff 74 24 18          	push   DWORD PTR [esp+0x18]
  4382e4:	ff 74 24 18          	push   DWORD PTR [esp+0x18]
  4382e8:	ff 74 24 18          	push   DWORD PTR [esp+0x18]
  4382ec:	53                   	push   ebx
  4382ed:	ff 15 6c c6 51 00    	call   DWORD PTR ds:0x51c66c
  4382f3:	5f                   	pop    edi
  4382f4:	5e                   	pop    esi
  4382f5:	5b                   	pop    ebx
  4382f6:	c3                   	ret
  4382f7:	33 c0                	xor    eax,eax
  4382f9:	eb f8                	jmp    0x4382f3
