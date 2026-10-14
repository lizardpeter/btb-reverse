
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004382fb <.text+0x372fb>:
  4382fb:	55                   	push   ebp
  4382fc:	8b ec                	mov    ebp,esp
  4382fe:	56                   	push   esi
  4382ff:	8b 75 08             	mov    esi,DWORD PTR [ebp+0x8]
  438302:	0f af 75 0c          	imul   esi,DWORD PTR [ebp+0xc]
  438306:	83 fe e0             	cmp    esi,0xffffffe0
  438309:	57                   	push   edi
  43830a:	89 75 08             	mov    DWORD PTR [ebp+0x8],esi
  43830d:	77 0d                	ja     0x43831c
  43830f:	85 f6                	test   esi,esi
  438311:	75 03                	jne    0x438316
  438313:	6a 01                	push   0x1
  438315:	5e                   	pop    esi
  438316:	83 c6 0f             	add    esi,0xf
  438319:	83 e6 f0             	and    esi,0xfffffff0
  43831c:	33 ff                	xor    edi,edi
  43831e:	83 fe e0             	cmp    esi,0xffffffe0
  438321:	77 58                	ja     0x43837b
  438323:	a1 44 da 51 00       	mov    eax,ds:0x51da44
  438328:	83 f8 03             	cmp    eax,0x3
  43832b:	75 1a                	jne    0x438347
  43832d:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  438330:	3b 05 3c da 51 00    	cmp    eax,DWORD PTR ds:0x51da3c
  438336:	77 2e                	ja     0x438366
  438338:	50                   	push   eax
  438339:	e8 5a c4 ff ff       	call   0x434798
  43833e:	8b f8                	mov    edi,eax
  438340:	59                   	pop    ecx
  438341:	85 ff                	test   edi,edi
  438343:	75 4c                	jne    0x438391
  438345:	eb 1f                	jmp    0x438366
  438347:	83 f8 02             	cmp    eax,0x2
  43834a:	75 1a                	jne    0x438366
  43834c:	3b 35 6c 97 44 00    	cmp    esi,DWORD PTR ds:0x44976c
  438352:	77 12                	ja     0x438366
  438354:	8b c6                	mov    eax,esi
  438356:	c1 e8 04             	shr    eax,0x4
  438359:	50                   	push   eax
  43835a:	e8 dc ce ff ff       	call   0x43523b
  43835f:	8b f8                	mov    edi,eax
  438361:	59                   	pop    ecx
  438362:	85 ff                	test   edi,edi
  438364:	75 3f                	jne    0x4383a5
  438366:	56                   	push   esi
  438367:	6a 08                	push   0x8
  438369:	ff 35 40 da 51 00    	push   DWORD PTR ds:0x51da40
  43836f:	ff 15 08 b1 43 00    	call   DWORD PTR ds:0x43b108
  438375:	8b f8                	mov    edi,eax
  438377:	85 ff                	test   edi,edi
  438379:	75 24                	jne    0x43839f
  43837b:	83 3d 48 c4 51 00 00 	cmp    DWORD PTR ds:0x51c448,0x0
  438382:	74 1b                	je     0x43839f
  438384:	56                   	push   esi
  438385:	e8 86 d2 ff ff       	call   0x435610
  43838a:	85 c0                	test   eax,eax
  43838c:	59                   	pop    ecx
  43838d:	74 19                	je     0x4383a8
  43838f:	eb 8b                	jmp    0x43831c
  438391:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  438394:	6a 00                	push   0x0
  438396:	57                   	push   edi
  438397:	e8 a4 e1 ff ff       	call   0x436540
  43839c:	83 c4 0c             	add    esp,0xc
  43839f:	8b c7                	mov    eax,edi
  4383a1:	5f                   	pop    edi
  4383a2:	5e                   	pop    esi
  4383a3:	5d                   	pop    ebp
  4383a4:	c3                   	ret
  4383a5:	56                   	push   esi
  4383a6:	eb ec                	jmp    0x438394
  4383a8:	33 c0                	xor    eax,eax
  4383aa:	eb f5                	jmp    0x4383a1
