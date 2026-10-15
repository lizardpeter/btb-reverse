
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00432535 <.text+0x31535>:
  432535:	55                   	push   ebp
  432536:	8b ec                	mov    ebp,esp
  432538:	6a ff                	push   0xffffffff
  43253a:	68 48 b6 43 00       	push   0x43b648
  43253f:	68 38 5f 43 00       	push   0x435f38
  432544:	64 a1 00 00 00 00    	mov    eax,fs:0x0
  43254a:	50                   	push   eax
  43254b:	64 89 25 00 00 00 00 	mov    DWORD PTR fs:0x0,esp
  432552:	51                   	push   ecx
  432553:	51                   	push   ecx
  432554:	53                   	push   ebx
  432555:	56                   	push   esi
  432556:	57                   	push   edi
  432557:	89 65 e8             	mov    DWORD PTR [ebp-0x18],esp
  43255a:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  43255d:	85 c0                	test   eax,eax
  43255f:	74 1b                	je     0x43257c
  432561:	8b 48 1c             	mov    ecx,DWORD PTR [eax+0x1c]
  432564:	8b 49 04             	mov    ecx,DWORD PTR [ecx+0x4]
  432567:	85 c9                	test   ecx,ecx
  432569:	74 11                	je     0x43257c
  43256b:	83 65 fc 00          	and    DWORD PTR [ebp-0x4],0x0
  43256f:	51                   	push   ecx
  432570:	ff 70 18             	push   DWORD PTR [eax+0x18]
  432573:	e8 a3 d6 ff ff       	call   0x42fc1b
  432578:	83 4d fc ff          	or     DWORD PTR [ebp-0x4],0xffffffff
  43257c:	8b 4d f0             	mov    ecx,DWORD PTR [ebp-0x10]
  43257f:	64 89 0d 00 00 00 00 	mov    DWORD PTR fs:0x0,ecx
  432586:	5f                   	pop    edi
  432587:	5e                   	pop    esi
  432588:	5b                   	pop    ebx
  432589:	c9                   	leave
  43258a:	c3                   	ret
