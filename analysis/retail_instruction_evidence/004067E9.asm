# source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05

verified retail PE32:     file format pei-i386


Disassembly of section .text:

004067e9 <.text+0x57e9>:
  4067e9:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  4067ec:	89 45 ec             	mov    DWORD PTR [ebp-0x14],eax
  4067ef:	83 c0 02             	add    eax,0x2
  4067f2:	85 c0                	test   eax,eax
  4067f4:	7d 02                	jge    0x4067f8
  4067f6:	33 c0                	xor    eax,eax
  4067f8:	50                   	push   eax
  4067f9:	e8 d0 93 02 00       	call   0x42fbce
  4067fe:	89 45 08             	mov    DWORD PTR [ebp+0x8],eax
  406801:	83 c4 04             	add    esp,0x4
  406804:	b8 0a 68 40 00       	mov    eax,0x40680a
  406809:	c3                   	ret
  40680a:	8b 5d e8             	mov    ebx,DWORD PTR [ebp-0x18]
  40680d:	8b 75 ec             	mov    esi,DWORD PTR [ebp-0x14]
