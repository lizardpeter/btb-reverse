
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00432662 <.text+0x31662>:
  432662:	55                   	push   ebp
  432663:	8b ec                	mov    ebp,esp
  432665:	6a ff                	push   0xffffffff
  432667:	68 70 b6 43 00       	push   0x43b670
  43266c:	68 38 5f 43 00       	push   0x435f38
  432671:	64 a1 00 00 00 00    	mov    eax,fs:0x0
  432677:	50                   	push   eax
  432678:	64 89 25 00 00 00 00 	mov    DWORD PTR fs:0x0,esp
  43267f:	51                   	push   ecx
  432680:	51                   	push   ecx
  432681:	53                   	push   ebx
  432682:	56                   	push   esi
  432683:	57                   	push   edi
  432684:	89 65 e8             	mov    DWORD PTR [ebp-0x18],esp
  432687:	83 65 fc 00          	and    DWORD PTR [ebp-0x4],0x0
  43268b:	a1 b4 75 44 00       	mov    eax,ds:0x4475b4
  432690:	85 c0                	test   eax,eax
  432692:	74 16                	je     0x4326aa
  432694:	c7 45 fc 01 00 00 00 	mov    DWORD PTR [ebp-0x4],0x1
  43269b:	ff d0                	call   eax
  43269d:	eb 07                	jmp    0x4326a6
  43269f:	6a 01                	push   0x1
  4326a1:	58                   	pop    eax
  4326a2:	c3                   	ret
  4326a3:	8b 65 e8             	mov    esp,DWORD PTR [ebp-0x18]
  4326a6:	83 65 fc 00          	and    DWORD PTR [ebp-0x4],0x0
  4326aa:	83 4d fc ff          	or     DWORD PTR [ebp-0x4],0xffffffff
  4326ae:	e8                   	.byte 0xe8
	...
