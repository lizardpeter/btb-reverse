
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0043260c <.text+0x3160c>:
  43260c:	55                   	push   ebp
  43260d:	8b ec                	mov    ebp,esp
  43260f:	6a ff                	push   0xffffffff
  432611:	68 58 b6 43 00       	push   0x43b658
  432616:	68 38 5f 43 00       	push   0x435f38
  43261b:	64 a1 00 00 00 00    	mov    eax,fs:0x0
  432621:	50                   	push   eax
  432622:	64 89 25 00 00 00 00 	mov    DWORD PTR fs:0x0,esp
  432629:	51                   	push   ecx
  43262a:	51                   	push   ecx
  43262b:	53                   	push   ebx
  43262c:	56                   	push   esi
  43262d:	57                   	push   edi
  43262e:	89 65 e8             	mov    DWORD PTR [ebp-0x18],esp
  432631:	83 65 fc 00          	and    DWORD PTR [ebp-0x4],0x0
  432635:	a1 2c c4 51 00       	mov    eax,ds:0x51c42c
  43263a:	85 c0                	test   eax,eax
  43263c:	74 16                	je     0x432654
  43263e:	c7 45 fc 01 00 00 00 	mov    DWORD PTR [ebp-0x4],0x1
  432645:	ff d0                	call   eax
  432647:	eb 07                	jmp    0x432650
  432649:	6a 01                	push   0x1
  43264b:	58                   	pop    eax
  43264c:	c3                   	ret
  43264d:	8b 65 e8             	mov    esp,DWORD PTR [ebp-0x18]
  432650:	83 65 fc 00          	and    DWORD PTR [ebp-0x4],0x0
  432654:	83 4d fc ff          	or     DWORD PTR [ebp-0x4],0xffffffff
  432658:	e8                   	.byte 0xe8
	...
