
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00434257 <.text+0x33257>:
  434257:	55                   	push   ebp
  434258:	8b ec                	mov    ebp,esp
  43425a:	b8 2c 12 00 00       	mov    eax,0x122c
  43425f:	e8 9c cc ff ff       	call   0x430f00
  434264:	8d 85 68 ff ff ff    	lea    eax,[ebp-0x98]
  43426a:	53                   	push   ebx
  43426b:	50                   	push   eax
  43426c:	c7 85 68 ff ff ff 94 	mov    DWORD PTR [ebp-0x98],0x94
  434273:	00 00 00 
  434276:	ff 15 88 b0 43 00    	call   DWORD PTR ds:0x43b088
  43427c:	85 c0                	test   eax,eax
  43427e:	74 1a                	je     0x43429a
  434280:	83 bd 78 ff ff ff 02 	cmp    DWORD PTR [ebp-0x88],0x2
  434287:	75 11                	jne    0x43429a
  434289:	83 bd 6c ff ff ff 05 	cmp    DWORD PTR [ebp-0x94],0x5
  434290:	72 08                	jb     0x43429a
  434292:	6a 01                	push   0x1
  434294:	58                   	pop    eax
  434295:	e9 02 01 00 00       	jmp    0x43439c
  43429a:	8d 85 d4 ed ff ff    	lea    eax,[ebp-0x122c]
  4342a0:	68 90 10 00 00       	push   0x1090
  4342a5:	50                   	push   eax
  4342a6:	68 58 b7 43 00       	push   0x43b758
  4342ab:	ff 15 e8 b0 43 00    	call   DWORD PTR ds:0x43b0e8
  4342b1:	85 c0                	test   eax,eax
  4342b3:	0f 84 d0 00 00 00    	je     0x434389
  4342b9:	33 db                	xor    ebx,ebx
  4342bb:	8d 8d d4 ed ff ff    	lea    ecx,[ebp-0x122c]
  4342c1:	38 9d d4 ed ff ff    	cmp    BYTE PTR [ebp-0x122c],bl
  4342c7:	74 13                	je     0x4342dc
  4342c9:	8a 01                	mov    al,BYTE PTR [ecx]
  4342cb:	3c 61                	cmp    al,0x61
  4342cd:	7c 08                	jl     0x4342d7
  4342cf:	3c 7a                	cmp    al,0x7a
  4342d1:	7f 04                	jg     0x4342d7
  4342d3:	2c 20                	sub    al,0x20
  4342d5:	88 01                	mov    BYTE PTR [ecx],al
  4342d7:	41                   	inc    ecx
  4342d8:	38 19                	cmp    BYTE PTR [ecx],bl
  4342da:	75 ed                	jne    0x4342c9
  4342dc:	8d 85 d4 ed ff ff    	lea    eax,[ebp-0x122c]
  4342e2:	6a 16                	push   0x16
  4342e4:	50                   	push   eax
  4342e5:	68 40 b7 43 00       	push   0x43b740
  4342ea:	e8 61 cb ff ff       	call   0x430e50
  4342ef:	83 c4 0c             	add    esp,0xc
  4342f2:	85 c0                	test   eax,eax
  4342f4:	75 08                	jne    0x4342fe
  4342f6:	8d 85 d4 ed ff ff    	lea    eax,[ebp-0x122c]
  4342fc:	eb 49                	jmp    0x434347
  4342fe:	8d 85 64 fe ff ff    	lea    eax,[ebp-0x19c]
  434304:	68 04 01 00 00       	push   0x104
  434309:	50                   	push   eax
  43430a:	53                   	push   ebx
  43430b:	ff 15 ec b0 43 00    	call   DWORD PTR ds:0x43b0ec
  434311:	38 9d 64 fe ff ff    	cmp    BYTE PTR [ebp-0x19c],bl
  434317:	8d 8d 64 fe ff ff    	lea    ecx,[ebp-0x19c]
  43431d:	74 13                	je     0x434332
  43431f:	8a 01                	mov    al,BYTE PTR [ecx]
  434321:	3c 61                	cmp    al,0x61
  434323:	7c 08                	jl     0x43432d
  434325:	3c 7a                	cmp    al,0x7a
  434327:	7f 04                	jg     0x43432d
  434329:	2c 20                	sub    al,0x20
  43432b:	88 01                	mov    BYTE PTR [ecx],al
  43432d:	41                   	inc    ecx
  43432e:	38 19                	cmp    BYTE PTR [ecx],bl
  434330:	75 ed                	jne    0x43431f
  434332:	8d 85 64 fe ff ff    	lea    eax,[ebp-0x19c]
  434338:	50                   	push   eax
  434339:	8d 85 d4 ed ff ff    	lea    eax,[ebp-0x122c]
  43433f:	50                   	push   eax
  434340:	e8 6b 3e 00 00       	call   0x4381b0
  434345:	59                   	pop    ecx
  434346:	59                   	pop    ecx
  434347:	3b c3                	cmp    eax,ebx
  434349:	74 3e                	je     0x434389
  43434b:	6a 2c                	push   0x2c
  43434d:	50                   	push   eax
  43434e:	e8 9d 3d 00 00       	call   0x4380f0
  434353:	59                   	pop    ecx
  434354:	3b c3                	cmp    eax,ebx
  434356:	59                   	pop    ecx
  434357:	74 30                	je     0x434389
  434359:	40                   	inc    eax
  43435a:	8b c8                	mov    ecx,eax
  43435c:	38 18                	cmp    BYTE PTR [eax],bl
  43435e:	74 0e                	je     0x43436e
  434360:	80 39 3b             	cmp    BYTE PTR [ecx],0x3b
  434363:	75 04                	jne    0x434369
  434365:	88 19                	mov    BYTE PTR [ecx],bl
  434367:	eb 01                	jmp    0x43436a
  434369:	41                   	inc    ecx
  43436a:	38 19                	cmp    BYTE PTR [ecx],bl
  43436c:	75 f2                	jne    0x434360
  43436e:	6a 0a                	push   0xa
  434370:	53                   	push   ebx
  434371:	50                   	push   eax
  434372:	e8 3c 3b 00 00       	call   0x437eb3
  434377:	83 c4 0c             	add    esp,0xc
  43437a:	83 f8 02             	cmp    eax,0x2
  43437d:	74 1d                	je     0x43439c
  43437f:	83 f8 03             	cmp    eax,0x3
  434382:	74 18                	je     0x43439c
  434384:	83 f8 01             	cmp    eax,0x1
  434387:	74 13                	je     0x43439c
  434389:	8d 45 fc             	lea    eax,[ebp-0x4]
  43438c:	50                   	push   eax
  43438d:	e8 98 fe ff ff       	call   0x43422a
  434392:	80 7d fc 06          	cmp    BYTE PTR [ebp-0x4],0x6
  434396:	59                   	pop    ecx
  434397:	1b c0                	sbb    eax,eax
  434399:	83 c0 03             	add    eax,0x3
  43439c:	5b                   	pop    ebx
  43439d:	c9                   	leave
  43439e:	c3                   	ret
