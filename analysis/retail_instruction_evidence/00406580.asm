
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00406580 <.text+0x5580>:
  406580:	56                   	push   esi
  406581:	8b f1                	mov    esi,ecx
  406583:	c7 06 28 b3 43 00    	mov    DWORD PTR [esi],0x43b328
  406589:	8b 46 10             	mov    eax,DWORD PTR [esi+0x10]
  40658c:	85 c0                	test   eax,eax
  40658e:	74 1d                	je     0x4065ad
  406590:	8d 48 ff             	lea    ecx,[eax-0x1]
  406593:	8a 40 ff             	mov    al,BYTE PTR [eax-0x1]
  406596:	84 c0                	test   al,al
  406598:	74 0a                	je     0x4065a4
  40659a:	3c ff                	cmp    al,0xff
  40659c:	74 06                	je     0x4065a4
  40659e:	fe c8                	dec    al
  4065a0:	88 01                	mov    BYTE PTR [ecx],al
  4065a2:	eb 09                	jmp    0x4065ad
  4065a4:	51                   	push   ecx
  4065a5:	e8 32 96 02 00       	call   0x42fbdc
  4065aa:	83 c4 04             	add    esp,0x4
  4065ad:	8b ce                	mov    ecx,esi
  4065af:	c7 46 10 00 00 00 00 	mov    DWORD PTR [esi+0x10],0x0
  4065b6:	c7 46 14 00 00 00 00 	mov    DWORD PTR [esi+0x14],0x0
  4065bd:	c7 46 18 00 00 00 00 	mov    DWORD PTR [esi+0x18],0x0
  4065c4:	e8 78 9e 02 00       	call   0x430441
  4065c9:	5e                   	pop    esi
  4065ca:	c3                   	ret
