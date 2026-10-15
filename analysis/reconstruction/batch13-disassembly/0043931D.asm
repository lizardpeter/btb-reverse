
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0043931d <.text+0x3831d>:
  43931d:	51                   	push   ecx
  43931e:	53                   	push   ebx
  43931f:	55                   	push   ebp
  439320:	56                   	push   esi
  439321:	8b 35 f8 c3 51 00    	mov    esi,DWORD PTR ds:0x51c3f8
  439327:	57                   	push   edi
  439328:	33 ff                	xor    edi,edi
  43932a:	8b 06                	mov    eax,DWORD PTR [esi]
  43932c:	3b c7                	cmp    eax,edi
  43932e:	74 4e                	je     0x43937e
  439330:	8b 1d 80 b0 43 00    	mov    ebx,DWORD PTR ds:0x43b080
  439336:	57                   	push   edi
  439337:	57                   	push   edi
  439338:	57                   	push   edi
  439339:	57                   	push   edi
  43933a:	6a ff                	push   0xffffffff
  43933c:	50                   	push   eax
  43933d:	57                   	push   edi
  43933e:	6a 01                	push   0x1
  439340:	ff d3                	call   ebx
  439342:	8b e8                	mov    ebp,eax
  439344:	3b ef                	cmp    ebp,edi
  439346:	74 3e                	je     0x439386
  439348:	55                   	push   ebp
  439349:	e8 45 7a ff ff       	call   0x430d93
  43934e:	3b c7                	cmp    eax,edi
  439350:	59                   	pop    ecx
  439351:	89 44 24 10          	mov    DWORD PTR [esp+0x10],eax
  439355:	74 2f                	je     0x439386
  439357:	57                   	push   edi
  439358:	57                   	push   edi
  439359:	55                   	push   ebp
  43935a:	50                   	push   eax
  43935b:	6a ff                	push   0xffffffff
  43935d:	ff 36                	push   DWORD PTR [esi]
  43935f:	57                   	push   edi
  439360:	6a 01                	push   0x1
  439362:	ff d3                	call   ebx
  439364:	85 c0                	test   eax,eax
  439366:	74 1e                	je     0x439386
  439368:	57                   	push   edi
  439369:	ff 74 24 14          	push   DWORD PTR [esp+0x14]
  43936d:	e8 d3 05 00 00       	call   0x439945
  439372:	8b 46 04             	mov    eax,DWORD PTR [esi+0x4]
  439375:	83 c6 04             	add    esi,0x4
  439378:	59                   	pop    ecx
  439379:	3b c7                	cmp    eax,edi
  43937b:	59                   	pop    ecx
  43937c:	75 b8                	jne    0x439336
  43937e:	33 c0                	xor    eax,eax
  439380:	5f                   	pop    edi
  439381:	5e                   	pop    esi
  439382:	5d                   	pop    ebp
  439383:	5b                   	pop    ebx
  439384:	59                   	pop    ecx
  439385:	c3                   	ret
  439386:	83 c8 ff             	or     eax,0xffffffff
  439389:	eb f5                	jmp    0x439380
