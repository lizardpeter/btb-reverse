
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004327e1 <.text+0x317e1>:
  4327e1:	55                   	push   ebp
  4327e2:	8b ec                	mov    ebp,esp
  4327e4:	51                   	push   ecx
  4327e5:	53                   	push   ebx
  4327e6:	8b 5d 08             	mov    ebx,DWORD PTR [ebp+0x8]
  4327e9:	81 fb ff 00 00 00    	cmp    ebx,0xff
  4327ef:	76 51                	jbe    0x432842
  4327f1:	8b c3                	mov    eax,ebx
  4327f3:	88 5d 0b             	mov    BYTE PTR [ebp+0xb],bl
  4327f6:	c1 e8 08             	shr    eax,0x8
  4327f9:	88 45 0a             	mov    BYTE PTR [ebp+0xa],al
  4327fc:	0f b6 c0             	movzx  eax,al
  4327ff:	f6 80 e1 c7 51 00 04 	test   BYTE PTR [eax+0x51c7e1],0x4
  432806:	74 4f                	je     0x432857
  432808:	6a 01                	push   0x1
  43280a:	8d 45 fc             	lea    eax,[ebp-0x4]
  43280d:	ff 35 b8 c6 51 00    	push   DWORD PTR ds:0x51c6b8
  432813:	6a 02                	push   0x2
  432815:	50                   	push   eax
  432816:	8d 45 0a             	lea    eax,[ebp+0xa]
  432819:	6a 02                	push   0x2
  43281b:	50                   	push   eax
  43281c:	68 00 02 00 00       	push   0x200
  432821:	ff 35 e4 c8 51 00    	push   DWORD PTR ds:0x51c8e4
  432827:	e8 87 48 00 00       	call   0x4370b3
  43282c:	83 c4 20             	add    esp,0x20
  43282f:	85 c0                	test   eax,eax
  432831:	74 24                	je     0x432857
  432833:	0f b6 45 fc          	movzx  eax,BYTE PTR [ebp-0x4]
  432837:	0f b6 4d fd          	movzx  ecx,BYTE PTR [ebp-0x3]
  43283b:	c1 e0 08             	shl    eax,0x8
  43283e:	03 c1                	add    eax,ecx
  432840:	eb 17                	jmp    0x432859
  432842:	8a 83 e1 c7 51 00    	mov    al,BYTE PTR [ebx+0x51c7e1]
  432848:	24 20                	and    al,0x20
  43284a:	3c 20                	cmp    al,0x20
  43284c:	75 09                	jne    0x432857
  43284e:	0f b6 83 e0 c6 51 00 	movzx  eax,BYTE PTR [ebx+0x51c6e0]
  432855:	eb 02                	jmp    0x432859
  432857:	8b c3                	mov    eax,ebx
  432859:	5b                   	pop    ebx
  43285a:	c9                   	leave
  43285b:	c3                   	ret
