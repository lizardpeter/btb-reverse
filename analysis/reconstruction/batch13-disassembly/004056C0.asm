
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004056c0 <.text+0x46c0>:
  4056c0:	83 ec 08             	sub    esp,0x8
  4056c3:	53                   	push   ebx
  4056c4:	8b d9                	mov    ebx,ecx
  4056c6:	56                   	push   esi
  4056c7:	57                   	push   edi
  4056c8:	8b 7c 24 18          	mov    edi,DWORD PTR [esp+0x18]
  4056cc:	8d b3 10 01 00 00    	lea    esi,[ebx+0x110]
  4056d2:	56                   	push   esi
  4056d3:	57                   	push   edi
  4056d4:	57                   	push   edi
  4056d5:	ff 15 c8 b1 43 00    	call   DWORD PTR ds:0x43b1c8
  4056db:	85 c0                	test   eax,eax
  4056dd:	0f 84 e5 00 00 00    	je     0x4057c8
  4056e3:	8b 06                	mov    eax,DWORD PTR [esi]
  4056e5:	8b 74 24 1c          	mov    esi,DWORD PTR [esp+0x1c]
  4056e9:	8b 8b 18 01 00 00    	mov    ecx,DWORD PTR [ebx+0x118]
  4056ef:	55                   	push   ebp
  4056f0:	8b 56 08             	mov    edx,DWORD PTR [esi+0x8]
  4056f3:	8b 2e                	mov    ebp,DWORD PTR [esi]
  4056f5:	2b c8                	sub    ecx,eax
  4056f7:	2b d5                	sub    edx,ebp
  4056f9:	89 4c 24 1c          	mov    DWORD PTR [esp+0x1c],ecx
  4056fd:	8b 8b 1c 01 00 00    	mov    ecx,DWORD PTR [ebx+0x11c]
  405703:	db 44 24 1c          	fild   DWORD PTR [esp+0x1c]
  405707:	89 54 24 1c          	mov    DWORD PTR [esp+0x1c],edx
  40570b:	8b 93 14 01 00 00    	mov    edx,DWORD PTR [ebx+0x114]
  405711:	db 44 24 1c          	fild   DWORD PTR [esp+0x1c]
  405715:	8b 6e 04             	mov    ebp,DWORD PTR [esi+0x4]
  405718:	2b ca                	sub    ecx,edx
  40571a:	8b 56 0c             	mov    edx,DWORD PTR [esi+0xc]
  40571d:	89 4c 24 1c          	mov    DWORD PTR [esp+0x1c],ecx
  405721:	de f9                	fdivp  st(1),st
  405723:	2b d5                	sub    edx,ebp
  405725:	8b 0f                	mov    ecx,DWORD PTR [edi]
  405727:	2b c8                	sub    ecx,eax
  405729:	db 44 24 1c          	fild   DWORD PTR [esp+0x1c]
  40572d:	89 54 24 1c          	mov    DWORD PTR [esp+0x1c],edx
  405731:	db 44 24 1c          	fild   DWORD PTR [esp+0x1c]
  405735:	89 4c 24 1c          	mov    DWORD PTR [esp+0x1c],ecx
  405739:	de f9                	fdivp  st(1),st
  40573b:	dd 5c 24 10          	fstp   QWORD PTR [esp+0x10]
  40573f:	db 44 24 1c          	fild   DWORD PTR [esp+0x1c]
  405743:	d8 f1                	fdiv   st,st(1)
  405745:	dc 05 38 b3 43 00    	fadd   QWORD PTR ds:0x43b338
  40574b:	e8 80 ad 02 00       	call   0x4304d0
  405750:	8b e8                	mov    ebp,eax
  405752:	89 2e                	mov    DWORD PTR [esi],ebp
  405754:	8b 57 08             	mov    edx,DWORD PTR [edi+0x8]
  405757:	2b 17                	sub    edx,DWORD PTR [edi]
  405759:	89 54 24 1c          	mov    DWORD PTR [esp+0x1c],edx
  40575d:	db 44 24 1c          	fild   DWORD PTR [esp+0x1c]
  405761:	d8 f1                	fdiv   st,st(1)
  405763:	dc 05 38 b3 43 00    	fadd   QWORD PTR ds:0x43b338
  405769:	e8 62 ad 02 00       	call   0x4304d0
  40576e:	03 c5                	add    eax,ebp
  405770:	89 46 08             	mov    DWORD PTR [esi+0x8],eax
  405773:	8b 47 04             	mov    eax,DWORD PTR [edi+0x4]
  405776:	2b 83 14 01 00 00    	sub    eax,DWORD PTR [ebx+0x114]
  40577c:	dd d8                	fstp   st(0)
  40577e:	89 44 24 1c          	mov    DWORD PTR [esp+0x1c],eax
  405782:	db 44 24 1c          	fild   DWORD PTR [esp+0x1c]
  405786:	dc 74 24 10          	fdiv   QWORD PTR [esp+0x10]
  40578a:	dc 05 38 b3 43 00    	fadd   QWORD PTR ds:0x43b338
  405790:	e8 3b ad 02 00       	call   0x4304d0
  405795:	8b d8                	mov    ebx,eax
  405797:	89 5e 04             	mov    DWORD PTR [esi+0x4],ebx
  40579a:	8b 4f 0c             	mov    ecx,DWORD PTR [edi+0xc]
  40579d:	2b 4f 04             	sub    ecx,DWORD PTR [edi+0x4]
  4057a0:	89 4c 24 1c          	mov    DWORD PTR [esp+0x1c],ecx
  4057a4:	db 44 24 1c          	fild   DWORD PTR [esp+0x1c]
  4057a8:	dc 74 24 10          	fdiv   QWORD PTR [esp+0x10]
  4057ac:	dc 05 38 b3 43 00    	fadd   QWORD PTR ds:0x43b338
  4057b2:	e8 19 ad 02 00       	call   0x4304d0
  4057b7:	03 c3                	add    eax,ebx
  4057b9:	5d                   	pop    ebp
  4057ba:	89 46 0c             	mov    DWORD PTR [esi+0xc],eax
  4057bd:	5f                   	pop    edi
  4057be:	5e                   	pop    esi
  4057bf:	b0 01                	mov    al,0x1
  4057c1:	5b                   	pop    ebx
  4057c2:	83 c4 08             	add    esp,0x8
  4057c5:	c2 08 00             	ret    0x8
  4057c8:	5f                   	pop    edi
  4057c9:	5e                   	pop    esi
  4057ca:	32 c0                	xor    al,al
  4057cc:	5b                   	pop    ebx
  4057cd:	83 c4 08             	add    esp,0x8
  4057d0:	c2 08 00             	ret    0x8
