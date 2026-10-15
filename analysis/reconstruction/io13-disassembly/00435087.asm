
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00435087 <.text+0x34087>:
  435087:	56                   	push   esi
  435088:	8b 74 24 08          	mov    esi,DWORD PTR [esp+0x8]
  43508c:	68 00 80 00 00       	push   0x8000
  435091:	6a 00                	push   0x0
  435093:	ff 76 10             	push   DWORD PTR [esi+0x10]
  435096:	ff 15 dc b0 43 00    	call   DWORD PTR ds:0x43b0dc
  43509c:	39 35 68 97 44 00    	cmp    DWORD PTR ds:0x449768,esi
  4350a2:	75 08                	jne    0x4350ac
  4350a4:	8b 46 04             	mov    eax,DWORD PTR [esi+0x4]
  4350a7:	a3 68 97 44 00       	mov    ds:0x449768,eax
  4350ac:	81 fe 48 77 44 00    	cmp    esi,0x447748
  4350b2:	74 20                	je     0x4350d4
  4350b4:	8b 46 04             	mov    eax,DWORD PTR [esi+0x4]
  4350b7:	8b 0e                	mov    ecx,DWORD PTR [esi]
  4350b9:	56                   	push   esi
  4350ba:	6a 00                	push   0x0
  4350bc:	89 08                	mov    DWORD PTR [eax],ecx
  4350be:	8b 06                	mov    eax,DWORD PTR [esi]
  4350c0:	8b 4e 04             	mov    ecx,DWORD PTR [esi+0x4]
  4350c3:	89 48 04             	mov    DWORD PTR [eax+0x4],ecx
  4350c6:	ff 35 40 da 51 00    	push   DWORD PTR ds:0x51da40
  4350cc:	ff 15 0c b1 43 00    	call   DWORD PTR ds:0x43b10c
  4350d2:	5e                   	pop    esi
  4350d3:	c3                   	ret
  4350d4:	83 0d 58 77 44 00 ff 	or     DWORD PTR ds:0x447758,0xffffffff
  4350db:	5e                   	pop    esi
  4350dc:	c3                   	ret
