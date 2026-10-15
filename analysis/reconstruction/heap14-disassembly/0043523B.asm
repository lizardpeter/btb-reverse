
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0043523b <.text+0x3423b>:
  43523b:	55                   	push   ebp
  43523c:	8b ec                	mov    ebp,esp
  43523e:	51                   	push   ecx
  43523f:	51                   	push   ecx
  435240:	53                   	push   ebx
  435241:	56                   	push   esi
  435242:	8b 35 68 97 44 00    	mov    esi,DWORD PTR ds:0x449768
  435248:	57                   	push   edi
  435249:	8b 56 10             	mov    edx,DWORD PTR [esi+0x10]
  43524c:	83 fa ff             	cmp    edx,0xffffffff
  43524f:	0f 84 9f 00 00 00    	je     0x4352f4
  435255:	8b 7e 08             	mov    edi,DWORD PTR [esi+0x8]
  435258:	8d 8e 18 20 00 00    	lea    ecx,[esi+0x2018]
  43525e:	8b c7                	mov    eax,edi
  435260:	2b c6                	sub    eax,esi
  435262:	83 e8 18             	sub    eax,0x18
  435265:	c1 f8 03             	sar    eax,0x3
  435268:	c1 e0 0c             	shl    eax,0xc
  43526b:	03 c2                	add    eax,edx
  43526d:	3b f9                	cmp    edi,ecx
  43526f:	89 45 fc             	mov    DWORD PTR [ebp-0x4],eax
  435272:	73 3a                	jae    0x4352ae
  435274:	8b 0f                	mov    ecx,DWORD PTR [edi]
  435276:	8b 5d 08             	mov    ebx,DWORD PTR [ebp+0x8]
  435279:	3b cb                	cmp    ecx,ebx
  43527b:	7c 1a                	jl     0x435297
  43527d:	39 5f 04             	cmp    DWORD PTR [edi+0x4],ebx
  435280:	76 15                	jbe    0x435297
  435282:	53                   	push   ebx
  435283:	51                   	push   ecx
  435284:	50                   	push   eax
  435285:	e8 b9 01 00 00       	call   0x435443
  43528a:	83 c4 0c             	add    esp,0xc
  43528d:	85 c0                	test   eax,eax
  43528f:	75 75                	jne    0x435306
  435291:	8b 45 fc             	mov    eax,DWORD PTR [ebp-0x4]
  435294:	89 5f 04             	mov    DWORD PTR [edi+0x4],ebx
  435297:	83 c7 08             	add    edi,0x8
  43529a:	8d 8e 18 20 00 00    	lea    ecx,[esi+0x2018]
  4352a0:	05 00 10 00 00       	add    eax,0x1000
  4352a5:	3b f9                	cmp    edi,ecx
  4352a7:	89 45 fc             	mov    DWORD PTR [ebp-0x4],eax
  4352aa:	72 c8                	jb     0x435274
  4352ac:	eb 03                	jmp    0x4352b1
  4352ae:	8b 5d 08             	mov    ebx,DWORD PTR [ebp+0x8]
  4352b1:	8b 46 08             	mov    eax,DWORD PTR [esi+0x8]
  4352b4:	8b 4e 10             	mov    ecx,DWORD PTR [esi+0x10]
  4352b7:	8d 7e 18             	lea    edi,[esi+0x18]
  4352ba:	89 45 f8             	mov    DWORD PTR [ebp-0x8],eax
  4352bd:	3b f8                	cmp    edi,eax
  4352bf:	89 4d fc             	mov    DWORD PTR [ebp-0x4],ecx
  4352c2:	73 33                	jae    0x4352f7
  4352c4:	8b 07                	mov    eax,DWORD PTR [edi]
  4352c6:	3b c3                	cmp    eax,ebx
  4352c8:	7c 19                	jl     0x4352e3
  4352ca:	39 5f 04             	cmp    DWORD PTR [edi+0x4],ebx
  4352cd:	76 14                	jbe    0x4352e3
  4352cf:	53                   	push   ebx
  4352d0:	50                   	push   eax
  4352d1:	ff 75 fc             	push   DWORD PTR [ebp-0x4]
  4352d4:	e8 6a 01 00 00       	call   0x435443
  4352d9:	83 c4 0c             	add    esp,0xc
  4352dc:	85 c0                	test   eax,eax
  4352de:	75 26                	jne    0x435306
  4352e0:	89 5f 04             	mov    DWORD PTR [edi+0x4],ebx
  4352e3:	81 45 fc 00 10 00 00 	add    DWORD PTR [ebp-0x4],0x1000
  4352ea:	83 c7 08             	add    edi,0x8
  4352ed:	3b 7d f8             	cmp    edi,DWORD PTR [ebp-0x8]
  4352f0:	72 d2                	jb     0x4352c4
  4352f2:	eb 03                	jmp    0x4352f7
  4352f4:	8b 5d 08             	mov    ebx,DWORD PTR [ebp+0x8]
  4352f7:	8b 36                	mov    esi,DWORD PTR [esi]
  4352f9:	3b 35 68 97 44 00    	cmp    esi,DWORD PTR ds:0x449768
  4352ff:	74 15                	je     0x435316
  435301:	e9 43 ff ff ff       	jmp    0x435249
  435306:	89 35 68 97 44 00    	mov    DWORD PTR ds:0x449768,esi
  43530c:	29 1f                	sub    DWORD PTR [edi],ebx
  43530e:	89 7e 08             	mov    DWORD PTR [esi+0x8],edi
  435311:	e9 28 01 00 00       	jmp    0x43543e
  435316:	b8 48 77 44 00       	mov    eax,0x447748
  43531b:	8b f8                	mov    edi,eax
  43531d:	83 7f 10 ff          	cmp    DWORD PTR [edi+0x10],0xffffffff
  435321:	74 06                	je     0x435329
  435323:	83 7f 0c 00          	cmp    DWORD PTR [edi+0xc],0x0
  435327:	75 0c                	jne    0x435335
  435329:	8b 3f                	mov    edi,DWORD PTR [edi]
  43532b:	3b f8                	cmp    edi,eax
  43532d:	0f 84 d7 00 00 00    	je     0x43540a
  435333:	eb e8                	jmp    0x43531d
  435335:	8b 5f 0c             	mov    ebx,DWORD PTR [edi+0xc]
  435338:	83 65 fc 00          	and    DWORD PTR [ebp-0x4],0x0
  43533c:	8b f3                	mov    esi,ebx
  43533e:	8b c3                	mov    eax,ebx
  435340:	2b f7                	sub    esi,edi
  435342:	83 ee 18             	sub    esi,0x18
  435345:	c1 fe 03             	sar    esi,0x3
  435348:	c1 e6 0c             	shl    esi,0xc
  43534b:	03 77 10             	add    esi,DWORD PTR [edi+0x10]
  43534e:	83 3b ff             	cmp    DWORD PTR [ebx],0xffffffff
  435351:	75 11                	jne    0x435364
  435353:	83 7d fc 10          	cmp    DWORD PTR [ebp-0x4],0x10
  435357:	7d 0b                	jge    0x435364
  435359:	83 c0 08             	add    eax,0x8
  43535c:	ff 45 fc             	inc    DWORD PTR [ebp-0x4]
  43535f:	83 38 ff             	cmp    DWORD PTR [eax],0xffffffff
  435362:	74 ef                	je     0x435353
  435364:	8b 45 fc             	mov    eax,DWORD PTR [ebp-0x4]
  435367:	6a 04                	push   0x4
  435369:	c1 e0 0c             	shl    eax,0xc
  43536c:	68 00 10 00 00       	push   0x1000
  435371:	50                   	push   eax
  435372:	56                   	push   esi
  435373:	89 45 f8             	mov    DWORD PTR [ebp-0x8],eax
  435376:	ff 15 d8 b0 43 00    	call   DWORD PTR ds:0x43b0d8
  43537c:	3b c6                	cmp    eax,esi
  43537e:	0f 85 b8 00 00 00    	jne    0x43543c
  435384:	6a 00                	push   0x0
  435386:	ff 75 f8             	push   DWORD PTR [ebp-0x8]
  435389:	56                   	push   esi
  43538a:	e8 b1 11 00 00       	call   0x436540
  43538f:	8b 55 fc             	mov    edx,DWORD PTR [ebp-0x4]
  435392:	83 c4 0c             	add    esp,0xc
  435395:	85 d2                	test   edx,edx
  435397:	8b cb                	mov    ecx,ebx
  435399:	7e 30                	jle    0x4353cb
  43539b:	8d 46 04             	lea    eax,[esi+0x4]
  43539e:	89 55 fc             	mov    DWORD PTR [ebp-0x4],edx
  4353a1:	80 88 f4 00 00 00 ff 	or     BYTE PTR [eax+0xf4],0xff
  4353a8:	8d 50 04             	lea    edx,[eax+0x4]
  4353ab:	89 50 fc             	mov    DWORD PTR [eax-0x4],edx
  4353ae:	ba f0 00 00 00       	mov    edx,0xf0
  4353b3:	89 10                	mov    DWORD PTR [eax],edx
  4353b5:	89 11                	mov    DWORD PTR [ecx],edx
  4353b7:	c7 41 04 f1 00 00 00 	mov    DWORD PTR [ecx+0x4],0xf1
  4353be:	05 00 10 00 00       	add    eax,0x1000
  4353c3:	83 c1 08             	add    ecx,0x8
  4353c6:	ff 4d fc             	dec    DWORD PTR [ebp-0x4]
  4353c9:	75 d6                	jne    0x4353a1
  4353cb:	89 3d 68 97 44 00    	mov    DWORD PTR ds:0x449768,edi
  4353d1:	8d 87 18 20 00 00    	lea    eax,[edi+0x2018]
  4353d7:	3b c8                	cmp    ecx,eax
  4353d9:	73 0c                	jae    0x4353e7
  4353db:	83 39 ff             	cmp    DWORD PTR [ecx],0xffffffff
  4353de:	74 05                	je     0x4353e5
  4353e0:	83 c1 08             	add    ecx,0x8
  4353e3:	eb f2                	jmp    0x4353d7
  4353e5:	3b c8                	cmp    ecx,eax
  4353e7:	1b c0                	sbb    eax,eax
  4353e9:	23 c1                	and    eax,ecx
  4353eb:	89 47 0c             	mov    DWORD PTR [edi+0xc],eax
  4353ee:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  4353f1:	88 46 08             	mov    BYTE PTR [esi+0x8],al
  4353f4:	89 5f 08             	mov    DWORD PTR [edi+0x8],ebx
  4353f7:	29 03                	sub    DWORD PTR [ebx],eax
  4353f9:	29 46 04             	sub    DWORD PTR [esi+0x4],eax
  4353fc:	8d 4c 06 08          	lea    ecx,[esi+eax*1+0x8]
  435400:	8d 86 00 01 00 00    	lea    eax,[esi+0x100]
  435406:	89 0e                	mov    DWORD PTR [esi],ecx
  435408:	eb 34                	jmp    0x43543e
  43540a:	e8 34 fb ff ff       	call   0x434f43
  43540f:	85 c0                	test   eax,eax
  435411:	74 29                	je     0x43543c
  435413:	8b 48 10             	mov    ecx,DWORD PTR [eax+0x10]
  435416:	88 59 08             	mov    BYTE PTR [ecx+0x8],bl
  435419:	8d 54 19 08          	lea    edx,[ecx+ebx*1+0x8]
  43541d:	a3 68 97 44 00       	mov    ds:0x449768,eax
  435422:	89 11                	mov    DWORD PTR [ecx],edx
  435424:	ba f0 00 00 00       	mov    edx,0xf0
  435429:	2b d3                	sub    edx,ebx
  43542b:	89 51 04             	mov    DWORD PTR [ecx+0x4],edx
  43542e:	0f b6 d3             	movzx  edx,bl
  435431:	29 50 18             	sub    DWORD PTR [eax+0x18],edx
  435434:	8d 81 00 01 00 00    	lea    eax,[ecx+0x100]
  43543a:	eb 02                	jmp    0x43543e
  43543c:	33 c0                	xor    eax,eax
  43543e:	5f                   	pop    edi
  43543f:	5e                   	pop    esi
  435440:	5b                   	pop    ebx
  435441:	c9                   	leave
  435442:	c3                   	ret
