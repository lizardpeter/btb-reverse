
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004300be <.text+0x2f0be>:
  4300be:	55                   	push   ebp
  4300bf:	8b ec                	mov    ebp,esp
  4300c1:	81 ec 08 01 00 00    	sub    esp,0x108
  4300c7:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  4300ca:	ff 15 20 b1 43 00    	call   DWORD PTR ds:0x43b120
  4300d0:	85 c0                	test   eax,eax
  4300d2:	74 5e                	je     0x430132
  4300d4:	8d 85 f8 fe ff ff    	lea    eax,[ebp-0x108]
  4300da:	50                   	push   eax
  4300db:	68 05 01 00 00       	push   0x105
  4300e0:	ff 15 24 b1 43 00    	call   DWORD PTR ds:0x43b124
  4300e6:	85 c0                	test   eax,eax
  4300e8:	74 48                	je     0x430132
  4300ea:	8a 85 f8 fe ff ff    	mov    al,BYTE PTR [ebp-0x108]
  4300f0:	3c 5c                	cmp    al,0x5c
  4300f2:	74 04                	je     0x4300f8
  4300f4:	3c 2f                	cmp    al,0x2f
  4300f6:	75 08                	jne    0x430100
  4300f8:	3a 85 f9 fe ff ff    	cmp    al,BYTE PTR [ebp-0x107]
  4300fe:	74 2e                	je     0x43012e
  430100:	0f b6 c0             	movzx  eax,al
  430103:	50                   	push   eax
  430104:	c6 45 08 3d          	mov    BYTE PTR [ebp+0x8],0x3d
  430108:	e8 d4 26 00 00       	call   0x4327e1
  43010d:	80 65 0b 00          	and    BYTE PTR [ebp+0xb],0x0
  430111:	88 45 09             	mov    BYTE PTR [ebp+0x9],al
  430114:	59                   	pop    ecx
  430115:	8d 85 f8 fe ff ff    	lea    eax,[ebp-0x108]
  43011b:	50                   	push   eax
  43011c:	8d 45 08             	lea    eax,[ebp+0x8]
  43011f:	50                   	push   eax
  430120:	c6 45 0a 3a          	mov    BYTE PTR [ebp+0xa],0x3a
  430124:	ff 15 28 b1 43 00    	call   DWORD PTR ds:0x43b128
  43012a:	85 c0                	test   eax,eax
  43012c:	74 04                	je     0x430132
  43012e:	33 c0                	xor    eax,eax
  430130:	c9                   	leave
  430131:	c3                   	ret
  430132:	ff 15 2c b1 43 00    	call   DWORD PTR ds:0x43b12c
  430138:	50                   	push   eax
  430139:	e8 3c 26 00 00       	call   0x43277a
  43013e:	59                   	pop    ecx
  43013f:	83 c8 ff             	or     eax,0xffffffff
  430142:	c9                   	leave
  430143:	c3                   	ret
