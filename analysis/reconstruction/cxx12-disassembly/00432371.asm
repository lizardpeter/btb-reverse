
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00432371 <.text+0x31371>:
  432371:	55                   	push   ebp
  432372:	8b ec                	mov    ebp,esp
  432374:	6a ff                	push   0xffffffff
  432376:	68 38 b6 43 00       	push   0x43b638
  43237b:	68 38 5f 43 00       	push   0x435f38
  432380:	64 a1 00 00 00 00    	mov    eax,fs:0x0
  432386:	50                   	push   eax
  432387:	64 89 25 00 00 00 00 	mov    DWORD PTR fs:0x0,esp
  43238e:	83 ec 0c             	sub    esp,0xc
  432391:	53                   	push   ebx
  432392:	56                   	push   esi
  432393:	57                   	push   edi
  432394:	89 65 e8             	mov    DWORD PTR [ebp-0x18],esp
  432397:	8b 4d 10             	mov    ecx,DWORD PTR [ebp+0x10]
  43239a:	8b 41 04             	mov    eax,DWORD PTR [ecx+0x4]
  43239d:	85 c0                	test   eax,eax
  43239f:	0f 84 75 01 00 00    	je     0x43251a
  4323a5:	80 78 08 00          	cmp    BYTE PTR [eax+0x8],0x0
  4323a9:	0f 84 6b 01 00 00    	je     0x43251a
  4323af:	8b 41 08             	mov    eax,DWORD PTR [ecx+0x8]
  4323b2:	85 c0                	test   eax,eax
  4323b4:	0f 84 60 01 00 00    	je     0x43251a
  4323ba:	8b 55 0c             	mov    edx,DWORD PTR [ebp+0xc]
  4323bd:	8d 7c 10 0c          	lea    edi,[eax+edx*1+0xc]
  4323c1:	83 65 fc 00          	and    DWORD PTR [ebp-0x4],0x0
  4323c5:	f6 01 08             	test   BYTE PTR [ecx],0x8
  4323c8:	74 44                	je     0x43240e
  4323ca:	8b 75 08             	mov    esi,DWORD PTR [ebp+0x8]
  4323cd:	6a 01                	push   0x1
  4323cf:	ff 76 18             	push   DWORD PTR [esi+0x18]
  4323d2:	e8 36 43 00 00       	call   0x43670d
  4323d7:	59                   	pop    ecx
  4323d8:	59                   	pop    ecx
  4323d9:	85 c0                	test   eax,eax
  4323db:	0f 84 30 01 00 00    	je     0x432511
  4323e1:	6a 01                	push   0x1
  4323e3:	57                   	push   edi
  4323e4:	e8 40 43 00 00       	call   0x436729
  4323e9:	59                   	pop    ecx
  4323ea:	59                   	pop    ecx
  4323eb:	85 c0                	test   eax,eax
  4323ed:	0f 84 1e 01 00 00    	je     0x432511
  4323f3:	8b 46 18             	mov    eax,DWORD PTR [esi+0x18]
  4323f6:	89 07                	mov    DWORD PTR [edi],eax
  4323f8:	8b 4d 14             	mov    ecx,DWORD PTR [ebp+0x14]
  4323fb:	83 c1 08             	add    ecx,0x8
  4323fe:	51                   	push   ecx
  4323ff:	50                   	push   eax
  432400:	e8 97 01 00 00       	call   0x43259c
  432405:	59                   	pop    ecx
  432406:	59                   	pop    ecx
  432407:	89 07                	mov    DWORD PTR [edi],eax
  432409:	e9 08 01 00 00       	jmp    0x432516
  43240e:	8b 75 14             	mov    esi,DWORD PTR [ebp+0x14]
  432411:	f6 06 01             	test   BYTE PTR [esi],0x1
  432414:	74 52                	je     0x432468
  432416:	8b 5d 08             	mov    ebx,DWORD PTR [ebp+0x8]
  432419:	6a 01                	push   0x1
  43241b:	ff 73 18             	push   DWORD PTR [ebx+0x18]
  43241e:	e8 ea 42 00 00       	call   0x43670d
  432423:	59                   	pop    ecx
  432424:	59                   	pop    ecx
  432425:	85 c0                	test   eax,eax
  432427:	0f 84 e4 00 00 00    	je     0x432511
  43242d:	6a 01                	push   0x1
  43242f:	57                   	push   edi
  432430:	e8 f4 42 00 00       	call   0x436729
  432435:	59                   	pop    ecx
  432436:	59                   	pop    ecx
  432437:	85 c0                	test   eax,eax
  432439:	0f 84 d2 00 00 00    	je     0x432511
  43243f:	ff 76 14             	push   DWORD PTR [esi+0x14]
  432442:	ff 73 18             	push   DWORD PTR [ebx+0x18]
  432445:	57                   	push   edi
  432446:	e8 b5 e0 ff ff       	call   0x430500
  43244b:	83 c4 0c             	add    esp,0xc
  43244e:	83 7e 14 04          	cmp    DWORD PTR [esi+0x14],0x4
  432452:	0f 85 be 00 00 00    	jne    0x432516
  432458:	8b 07                	mov    eax,DWORD PTR [edi]
  43245a:	85 c0                	test   eax,eax
  43245c:	0f 84 b4 00 00 00    	je     0x432516
  432462:	83 c6 08             	add    esi,0x8
  432465:	56                   	push   esi
  432466:	eb 97                	jmp    0x4323ff
  432468:	83 7e 18 00          	cmp    DWORD PTR [esi+0x18],0x0
  43246c:	8b 5d 08             	mov    ebx,DWORD PTR [ebp+0x8]
  43246f:	6a 01                	push   0x1
  432471:	ff 73 18             	push   DWORD PTR [ebx+0x18]
  432474:	75 3a                	jne    0x4324b0
  432476:	e8 92 42 00 00       	call   0x43670d
  43247b:	59                   	pop    ecx
  43247c:	59                   	pop    ecx
  43247d:	85 c0                	test   eax,eax
  43247f:	0f 84 8c 00 00 00    	je     0x432511
  432485:	6a 01                	push   0x1
  432487:	57                   	push   edi
  432488:	e8 9c 42 00 00       	call   0x436729
  43248d:	59                   	pop    ecx
  43248e:	59                   	pop    ecx
  43248f:	85 c0                	test   eax,eax
  432491:	74 7e                	je     0x432511
  432493:	ff 76 14             	push   DWORD PTR [esi+0x14]
  432496:	83 c6 08             	add    esi,0x8
  432499:	56                   	push   esi
  43249a:	ff 73 18             	push   DWORD PTR [ebx+0x18]
  43249d:	e8 fa 00 00 00       	call   0x43259c
  4324a2:	59                   	pop    ecx
  4324a3:	59                   	pop    ecx
  4324a4:	50                   	push   eax
  4324a5:	57                   	push   edi
  4324a6:	e8 55 e0 ff ff       	call   0x430500
  4324ab:	83 c4 0c             	add    esp,0xc
  4324ae:	eb 66                	jmp    0x432516
  4324b0:	e8 58 42 00 00       	call   0x43670d
  4324b5:	59                   	pop    ecx
  4324b6:	59                   	pop    ecx
  4324b7:	85 c0                	test   eax,eax
  4324b9:	74 56                	je     0x432511
  4324bb:	6a 01                	push   0x1
  4324bd:	57                   	push   edi
  4324be:	e8 66 42 00 00       	call   0x436729
  4324c3:	59                   	pop    ecx
  4324c4:	59                   	pop    ecx
  4324c5:	85 c0                	test   eax,eax
  4324c7:	74 48                	je     0x432511
  4324c9:	ff 76 18             	push   DWORD PTR [esi+0x18]
  4324cc:	e8 74 42 00 00       	call   0x436745
  4324d1:	59                   	pop    ecx
  4324d2:	85 c0                	test   eax,eax
  4324d4:	74 3b                	je     0x432511
  4324d6:	f6 06 04             	test   BYTE PTR [esi],0x4
  4324d9:	74 1c                	je     0x4324f7
  4324db:	6a 01                	push   0x1
  4324dd:	8d 46 08             	lea    eax,[esi+0x8]
  4324e0:	50                   	push   eax
  4324e1:	ff 73 18             	push   DWORD PTR [ebx+0x18]
  4324e4:	e8 b3 00 00 00       	call   0x43259c
  4324e9:	59                   	pop    ecx
  4324ea:	59                   	pop    ecx
  4324eb:	50                   	push   eax
  4324ec:	ff 76 18             	push   DWORD PTR [esi+0x18]
  4324ef:	57                   	push   edi
  4324f0:	e8 2d d7 ff ff       	call   0x42fc22
  4324f5:	eb 1f                	jmp    0x432516
  4324f7:	8d 46 08             	lea    eax,[esi+0x8]
  4324fa:	50                   	push   eax
  4324fb:	ff 73 18             	push   DWORD PTR [ebx+0x18]
  4324fe:	e8 99 00 00 00       	call   0x43259c
  432503:	59                   	pop    ecx
  432504:	59                   	pop    ecx
  432505:	50                   	push   eax
  432506:	ff 76 18             	push   DWORD PTR [esi+0x18]
  432509:	57                   	push   edi
  43250a:	e8 0c d7 ff ff       	call   0x42fc1b
  43250f:	eb 05                	jmp    0x432516
  432511:	e8 4c 01 00 00       	call   0x432662
  432516:	83 4d fc ff          	or     DWORD PTR [ebp-0x4],0xffffffff
  43251a:	8b 4d f0             	mov    ecx,DWORD PTR [ebp-0x10]
  43251d:	64 89 0d 00 00 00 00 	mov    DWORD PTR fs:0x0,ecx
  432524:	5f                   	pop    edi
  432525:	5e                   	pop    esi
  432526:	5b                   	pop    ebx
  432527:	c9                   	leave
  432528:	c3                   	ret
