
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004321c0 <.text+0x311c0>:
  4321c0:	55                   	push   ebp
  4321c1:	8b ec                	mov    ebp,esp
  4321c3:	83 7d 20 00          	cmp    DWORD PTR [ebp+0x20],0x0
  4321c7:	53                   	push   ebx
  4321c8:	8b 5d 1c             	mov    ebx,DWORD PTR [ebp+0x1c]
  4321cb:	56                   	push   esi
  4321cc:	57                   	push   edi
  4321cd:	8b 7d 0c             	mov    edi,DWORD PTR [ebp+0xc]
  4321d0:	74 10                	je     0x4321e2
  4321d2:	ff 75 20             	push   DWORD PTR [ebp+0x20]
  4321d5:	53                   	push   ebx
  4321d6:	57                   	push   edi
  4321d7:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  4321da:	e8 92 01 00 00       	call   0x432371
  4321df:	83 c4 10             	add    esp,0x10
  4321e2:	83 7d 2c 00          	cmp    DWORD PTR [ebp+0x2c],0x0
  4321e6:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  4321e9:	75 03                	jne    0x4321ee
  4321eb:	57                   	push   edi
  4321ec:	eb 03                	jmp    0x4321f1
  4321ee:	ff 75 2c             	push   DWORD PTR [ebp+0x2c]
  4321f1:	e8 33 da ff ff       	call   0x42fc29
  4321f6:	8b 75 24             	mov    esi,DWORD PTR [ebp+0x24]
  4321f9:	ff 36                	push   DWORD PTR [esi]
  4321fb:	ff 75 18             	push   DWORD PTR [ebp+0x18]
  4321fe:	ff 75 14             	push   DWORD PTR [ebp+0x14]
  432201:	57                   	push   edi
  432202:	e8 05 ff ff ff       	call   0x43210c
  432207:	8b 46 04             	mov    eax,DWORD PTR [esi+0x4]
  43220a:	68 00 01 00 00       	push   0x100
  43220f:	ff 75 28             	push   DWORD PTR [ebp+0x28]
  432212:	40                   	inc    eax
  432213:	89 47 08             	mov    DWORD PTR [edi+0x8],eax
  432216:	ff 73 0c             	push   DWORD PTR [ebx+0xc]
  432219:	ff 75 18             	push   DWORD PTR [ebp+0x18]
  43221c:	ff 75 10             	push   DWORD PTR [ebp+0x10]
  43221f:	57                   	push   edi
  432220:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  432223:	e8 13 00 00 00       	call   0x43223b
  432228:	83 c4 2c             	add    esp,0x2c
  43222b:	85 c0                	test   eax,eax
  43222d:	74 07                	je     0x432236
  43222f:	57                   	push   edi
  432230:	50                   	push   eax
  432231:	e8 b1 d9 ff ff       	call   0x42fbe7
  432236:	5f                   	pop    edi
  432237:	5e                   	pop    esi
  432238:	5b                   	pop    ebx
  432239:	5d                   	pop    ebp
  43223a:	c3                   	ret
