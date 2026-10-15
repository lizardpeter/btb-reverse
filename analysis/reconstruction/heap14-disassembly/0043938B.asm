
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0043938b <.text+0x3838b>:
  43938b:	55                   	push   ebp
  43938c:	8b ec                	mov    ebp,esp
  43938e:	83 ec 24             	sub    esp,0x24
  439391:	53                   	push   ebx
  439392:	8b 5d 0c             	mov    ebx,DWORD PTR [ebp+0xc]
  439395:	56                   	push   esi
  439396:	8b 75 08             	mov    esi,DWORD PTR [ebp+0x8]
  439399:	66 8b 4b 0a          	mov    cx,WORD PTR [ebx+0xa]
  43939d:	33 c0                	xor    eax,eax
  43939f:	57                   	push   edi
  4393a0:	89 45 ec             	mov    DWORD PTR [ebp-0x14],eax
  4393a3:	89 45 dc             	mov    DWORD PTR [ebp-0x24],eax
  4393a6:	89 45 e0             	mov    DWORD PTR [ebp-0x20],eax
  4393a9:	89 45 e4             	mov    DWORD PTR [ebp-0x1c],eax
  4393ac:	66 8b 46 0a          	mov    ax,WORD PTR [esi+0xa]
  4393b0:	8b f9                	mov    edi,ecx
  4393b2:	ba ff 7f 00 00       	mov    edx,0x7fff
  4393b7:	33 f8                	xor    edi,eax
  4393b9:	23 c2                	and    eax,edx
  4393bb:	23 ca                	and    ecx,edx
  4393bd:	81 e7 00 80 00 00    	and    edi,0x8000
  4393c3:	66 3d ff 7f          	cmp    ax,0x7fff
  4393c7:	8d 14 01             	lea    edx,[ecx+eax*1]
  4393ca:	89 55 08             	mov    DWORD PTR [ebp+0x8],edx
  4393cd:	0f 83 b8 01 00 00    	jae    0x43958b
  4393d3:	66 81 f9 ff 7f       	cmp    cx,0x7fff
  4393d8:	0f 83 ad 01 00 00    	jae    0x43958b
  4393de:	66 81 fa fd bf       	cmp    dx,0xbffd
  4393e3:	0f 87 a2 01 00 00    	ja     0x43958b
  4393e9:	66 81 fa bf 3f       	cmp    dx,0x3fbf
  4393ee:	77 04                	ja     0x4393f4
  4393f0:	33 c0                	xor    eax,eax
  4393f2:	eb 3a                	jmp    0x43942e
  4393f4:	66 85 c0             	test   ax,ax
  4393f7:	ba ff ff ff 7f       	mov    edx,0x7fffffff
  4393fc:	75 18                	jne    0x439416
  4393fe:	ff 45 08             	inc    DWORD PTR [ebp+0x8]
  439401:	85 56 08             	test   DWORD PTR [esi+0x8],edx
  439404:	75 10                	jne    0x439416
  439406:	33 c0                	xor    eax,eax
  439408:	39 46 04             	cmp    DWORD PTR [esi+0x4],eax
  43940b:	75 0b                	jne    0x439418
  43940d:	39 06                	cmp    DWORD PTR [esi],eax
  43940f:	75 07                	jne    0x439418
  439411:	e9 6f 01 00 00       	jmp    0x439585
  439416:	33 c0                	xor    eax,eax
  439418:	66 3b c8             	cmp    cx,ax
  43941b:	75 1e                	jne    0x43943b
  43941d:	ff 45 08             	inc    DWORD PTR [ebp+0x8]
  439420:	85 53 08             	test   DWORD PTR [ebx+0x8],edx
  439423:	75 16                	jne    0x43943b
  439425:	39 43 04             	cmp    DWORD PTR [ebx+0x4],eax
  439428:	75 11                	jne    0x43943b
  43942a:	39 03                	cmp    DWORD PTR [ebx],eax
  43942c:	75 0d                	jne    0x43943b
  43942e:	89 46 08             	mov    DWORD PTR [esi+0x8],eax
  439431:	89 46 04             	mov    DWORD PTR [esi+0x4],eax
  439434:	89 06                	mov    DWORD PTR [esi],eax
  439436:	e9 6b 01 00 00       	jmp    0x4395a6
  43943b:	89 45 f0             	mov    DWORD PTR [ebp-0x10],eax
  43943e:	8d 45 e0             	lea    eax,[ebp-0x20]
  439441:	89 45 fc             	mov    DWORD PTR [ebp-0x4],eax
  439444:	c7 45 0c 05 00 00 00 	mov    DWORD PTR [ebp+0xc],0x5
  43944b:	8b 45 f0             	mov    eax,DWORD PTR [ebp-0x10]
  43944e:	03 c0                	add    eax,eax
  439450:	83 7d 0c 00          	cmp    DWORD PTR [ebp+0xc],0x0
  439454:	7e 49                	jle    0x43949f
  439456:	03 c6                	add    eax,esi
  439458:	8d 4b 08             	lea    ecx,[ebx+0x8]
  43945b:	89 45 f8             	mov    DWORD PTR [ebp-0x8],eax
  43945e:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  439461:	89 4d f4             	mov    DWORD PTR [ebp-0xc],ecx
  439464:	89 45 e8             	mov    DWORD PTR [ebp-0x18],eax
  439467:	8b 45 f8             	mov    eax,DWORD PTR [ebp-0x8]
  43946a:	8b 4d f4             	mov    ecx,DWORD PTR [ebp-0xc]
  43946d:	0f b7 00             	movzx  eax,WORD PTR [eax]
  439470:	0f b7 09             	movzx  ecx,WORD PTR [ecx]
  439473:	0f af c1             	imul   eax,ecx
  439476:	8b 4d fc             	mov    ecx,DWORD PTR [ebp-0x4]
  439479:	83 c1 fc             	add    ecx,0xfffffffc
  43947c:	51                   	push   ecx
  43947d:	50                   	push   eax
  43947e:	ff 31                	push   DWORD PTR [ecx]
  439480:	e8 42 f3 ff ff       	call   0x4387c7
  439485:	83 c4 0c             	add    esp,0xc
  439488:	85 c0                	test   eax,eax
  43948a:	74 06                	je     0x439492
  43948c:	8b 45 fc             	mov    eax,DWORD PTR [ebp-0x4]
  43948f:	66 ff 00             	inc    WORD PTR [eax]
  439492:	83 45 f8 02          	add    DWORD PTR [ebp-0x8],0x2
  439496:	83 6d f4 02          	sub    DWORD PTR [ebp-0xc],0x2
  43949a:	ff 4d e8             	dec    DWORD PTR [ebp-0x18]
  43949d:	75 c8                	jne    0x439467
  43949f:	83 45 fc 02          	add    DWORD PTR [ebp-0x4],0x2
  4394a3:	ff 45 f0             	inc    DWORD PTR [ebp-0x10]
  4394a6:	ff 4d 0c             	dec    DWORD PTR [ebp+0xc]
  4394a9:	83 7d 0c 00          	cmp    DWORD PTR [ebp+0xc],0x0
  4394ad:	7f 9c                	jg     0x43944b
  4394af:	81 45 08 02 c0 00 00 	add    DWORD PTR [ebp+0x8],0xc002
  4394b6:	66 83 7d 08 00       	cmp    WORD PTR [ebp+0x8],0x0
  4394bb:	7e 25                	jle    0x4394e2
  4394bd:	f6 45 e7 80          	test   BYTE PTR [ebp-0x19],0x80
  4394c1:	75 18                	jne    0x4394db
  4394c3:	8d 45 dc             	lea    eax,[ebp-0x24]
  4394c6:	50                   	push   eax
  4394c7:	e8 7a f3 ff ff       	call   0x438846
  4394cc:	81 45 08 ff ff 00 00 	add    DWORD PTR [ebp+0x8],0xffff
  4394d3:	59                   	pop    ecx
  4394d4:	66 83 7d 08 00       	cmp    WORD PTR [ebp+0x8],0x0
  4394d9:	7f e2                	jg     0x4394bd
  4394db:	66 83 7d 08 00       	cmp    WORD PTR [ebp+0x8],0x0
  4394e0:	7f 39                	jg     0x43951b
  4394e2:	81 45 08 ff ff 00 00 	add    DWORD PTR [ebp+0x8],0xffff
  4394e9:	66 83 7d 08 00       	cmp    WORD PTR [ebp+0x8],0x0
  4394ee:	7d 2b                	jge    0x43951b
  4394f0:	0f bf 45 08          	movsx  eax,WORD PTR [ebp+0x8]
  4394f4:	f7 d8                	neg    eax
  4394f6:	01 45 08             	add    DWORD PTR [ebp+0x8],eax
  4394f9:	8b d8                	mov    ebx,eax
  4394fb:	f6 45 dc 01          	test   BYTE PTR [ebp-0x24],0x1
  4394ff:	74 03                	je     0x439504
  439501:	ff 45 ec             	inc    DWORD PTR [ebp-0x14]
  439504:	8d 45 dc             	lea    eax,[ebp-0x24]
  439507:	50                   	push   eax
  439508:	e8 67 f3 ff ff       	call   0x438874
  43950d:	4b                   	dec    ebx
  43950e:	59                   	pop    ecx
  43950f:	75 ea                	jne    0x4394fb
  439511:	83 7d ec 00          	cmp    DWORD PTR [ebp-0x14],0x0
  439515:	74 04                	je     0x43951b
  439517:	80 4d dc 01          	or     BYTE PTR [ebp-0x24],0x1
  43951b:	66 81 7d dc 00 80    	cmp    WORD PTR [ebp-0x24],0x8000
  439521:	77 0f                	ja     0x439532
  439523:	8b 45 dc             	mov    eax,DWORD PTR [ebp-0x24]
  439526:	25 ff ff 01 00       	and    eax,0x1ffff
  43952b:	3d 00 80 01 00       	cmp    eax,0x18000
  439530:	75 35                	jne    0x439567
  439532:	83 7d de ff          	cmp    DWORD PTR [ebp-0x22],0xffffffff
  439536:	75 2c                	jne    0x439564
  439538:	83 65 de 00          	and    DWORD PTR [ebp-0x22],0x0
  43953c:	83 7d e2 ff          	cmp    DWORD PTR [ebp-0x1e],0xffffffff
  439540:	75 1d                	jne    0x43955f
  439542:	83 65 e2 00          	and    DWORD PTR [ebp-0x1e],0x0
  439546:	66 81 7d e6 ff ff    	cmp    WORD PTR [ebp-0x1a],0xffff
  43954c:	75 0b                	jne    0x439559
  43954e:	ff 45 08             	inc    DWORD PTR [ebp+0x8]
  439551:	66 c7 45 e6 00 80    	mov    WORD PTR [ebp-0x1a],0x8000
  439557:	eb 0e                	jmp    0x439567
  439559:	66 ff 45 e6          	inc    WORD PTR [ebp-0x1a]
  43955d:	eb 08                	jmp    0x439567
  43955f:	ff 45 e2             	inc    DWORD PTR [ebp-0x1e]
  439562:	eb 03                	jmp    0x439567
  439564:	ff 45 de             	inc    DWORD PTR [ebp-0x22]
  439567:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  43956a:	66 3d ff 7f          	cmp    ax,0x7fff
  43956e:	73 1b                	jae    0x43958b
  439570:	66 8b 4d de          	mov    cx,WORD PTR [ebp-0x22]
  439574:	0b c7                	or     eax,edi
  439576:	66 89 0e             	mov    WORD PTR [esi],cx
  439579:	8b 4d e0             	mov    ecx,DWORD PTR [ebp-0x20]
  43957c:	89 4e 02             	mov    DWORD PTR [esi+0x2],ecx
  43957f:	8b 4d e4             	mov    ecx,DWORD PTR [ebp-0x1c]
  439582:	89 4e 06             	mov    DWORD PTR [esi+0x6],ecx
  439585:	66 89 46 0a          	mov    WORD PTR [esi+0xa],ax
  439589:	eb 1b                	jmp    0x4395a6
  43958b:	66 f7 df             	neg    di
  43958e:	1b ff                	sbb    edi,edi
  439590:	83 66 04 00          	and    DWORD PTR [esi+0x4],0x0
  439594:	81 e7 00 00 00 80    	and    edi,0x80000000
  43959a:	81 c7 00 80 ff 7f    	add    edi,0x7fff8000
  4395a0:	83 26 00             	and    DWORD PTR [esi],0x0
  4395a3:	89 7e 08             	mov    DWORD PTR [esi+0x8],edi
  4395a6:	5f                   	pop    edi
  4395a7:	5e                   	pop    esi
  4395a8:	5b                   	pop    ebx
  4395a9:	c9                   	leave
  4395aa:	c3                   	ret
