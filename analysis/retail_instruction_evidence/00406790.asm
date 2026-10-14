# source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05

verified retail PE32:     file format pei-i386


Disassembly of section .text:

00406790 <.text+0x5790>:
  406790:	55                   	push   ebp
  406791:	8b ec                	mov    ebp,esp
  406793:	6a ff                	push   0xffffffff
  406795:	68 30 a5 43 00       	push   0x43a530
  40679a:	64 a1 00 00 00 00    	mov    eax,fs:0x0
  4067a0:	50                   	push   eax
  4067a1:	64 89 25 00 00 00 00 	mov    DWORD PTR fs:0x0,esp
  4067a8:	83 ec 0c             	sub    esp,0xc
  4067ab:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  4067ae:	53                   	push   ebx
  4067af:	56                   	push   esi
  4067b0:	8b f0                	mov    esi,eax
  4067b2:	83 ce 1f             	or     esi,0x1f
  4067b5:	57                   	push   edi
  4067b6:	8b d9                	mov    ebx,ecx
  4067b8:	83 fe fd             	cmp    esi,0xfffffffd
  4067bb:	89 65 f0             	mov    DWORD PTR [ebp-0x10],esp
  4067be:	89 5d e8             	mov    DWORD PTR [ebp-0x18],ebx
  4067c1:	89 75 ec             	mov    DWORD PTR [ebp-0x14],esi
  4067c4:	76 05                	jbe    0x4067cb
  4067c6:	89 45 ec             	mov    DWORD PTR [ebp-0x14],eax
  4067c9:	8b f0                	mov    esi,eax
  4067cb:	8d 46 02             	lea    eax,[esi+0x2]
  4067ce:	c7 45 fc 00 00 00 00 	mov    DWORD PTR [ebp-0x4],0x0
  4067d5:	85 c0                	test   eax,eax
  4067d7:	7d 02                	jge    0x4067db
  4067d9:	33 c0                	xor    eax,eax
  4067db:	50                   	push   eax
  4067dc:	e8 ed 93 02 00       	call   0x42fbce
  4067e1:	83 c4 04             	add    esp,0x4
  4067e4:	89 45 08             	mov    DWORD PTR [ebp+0x8],eax
  4067e7:	eb 27                	jmp    0x406810
