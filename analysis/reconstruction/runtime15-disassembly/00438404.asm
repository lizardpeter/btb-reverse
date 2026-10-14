
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00438404 <.text+0x37404>:
  438404:	55                   	push   ebp
  438405:	8b ec                	mov    ebp,esp
  438407:	6a ff                	push   0xffffffff
  438409:	68 08 bb 43 00       	push   0x43bb08
  43840e:	68 38 5f 43 00       	push   0x435f38
  438413:	64 a1 00 00 00 00    	mov    eax,fs:0x0
  438419:	50                   	push   eax
  43841a:	64 89 25 00 00 00 00 	mov    DWORD PTR fs:0x0,esp
  438421:	83 ec 18             	sub    esp,0x18
  438424:	53                   	push   ebx
  438425:	56                   	push   esi
  438426:	57                   	push   edi
  438427:	89 65 e8             	mov    DWORD PTR [ebp-0x18],esp
  43842a:	a1 98 c6 51 00       	mov    eax,ds:0x51c698
  43842f:	33 db                	xor    ebx,ebx
  438431:	3b c3                	cmp    eax,ebx
  438433:	75 3e                	jne    0x438473
  438435:	8d 45 e4             	lea    eax,[ebp-0x1c]
  438438:	50                   	push   eax
  438439:	6a 01                	push   0x1
  43843b:	5e                   	pop    esi
  43843c:	56                   	push   esi
  43843d:	68 a8 ba 43 00       	push   0x43baa8
  438442:	56                   	push   esi
  438443:	ff 15 90 b1 43 00    	call   DWORD PTR ds:0x43b190
  438449:	85 c0                	test   eax,eax
  43844b:	74 04                	je     0x438451
  43844d:	8b c6                	mov    eax,esi
  43844f:	eb 1d                	jmp    0x43846e
  438451:	8d 45 e4             	lea    eax,[ebp-0x1c]
  438454:	50                   	push   eax
  438455:	56                   	push   esi
  438456:	68 a4 ba 43 00       	push   0x43baa4
  43845b:	56                   	push   esi
  43845c:	53                   	push   ebx
  43845d:	ff 15 8c b1 43 00    	call   DWORD PTR ds:0x43b18c
  438463:	85 c0                	test   eax,eax
  438465:	0f 84 ce 00 00 00    	je     0x438539
  43846b:	6a 02                	push   0x2
  43846d:	58                   	pop    eax
  43846e:	a3 98 c6 51 00       	mov    ds:0x51c698,eax
  438473:	83 f8 02             	cmp    eax,0x2
  438476:	75 24                	jne    0x43849c
  438478:	8b 45 1c             	mov    eax,DWORD PTR [ebp+0x1c]
  43847b:	3b c3                	cmp    eax,ebx
  43847d:	75 05                	jne    0x438484
  43847f:	a1 80 c6 51 00       	mov    eax,ds:0x51c680
  438484:	ff 75 14             	push   DWORD PTR [ebp+0x14]
  438487:	ff 75 10             	push   DWORD PTR [ebp+0x10]
  43848a:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  43848d:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  438490:	50                   	push   eax
  438491:	ff 15 8c b1 43 00    	call   DWORD PTR ds:0x43b18c
  438497:	e9 9f 00 00 00       	jmp    0x43853b
  43849c:	83 f8 01             	cmp    eax,0x1
  43849f:	0f 85 94 00 00 00    	jne    0x438539
  4384a5:	39 5d 18             	cmp    DWORD PTR [ebp+0x18],ebx
  4384a8:	75 08                	jne    0x4384b2
  4384aa:	a1 90 c6 51 00       	mov    eax,ds:0x51c690
  4384af:	89 45 18             	mov    DWORD PTR [ebp+0x18],eax
  4384b2:	53                   	push   ebx
  4384b3:	53                   	push   ebx
  4384b4:	ff 75 10             	push   DWORD PTR [ebp+0x10]
  4384b7:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  4384ba:	8b 45 20             	mov    eax,DWORD PTR [ebp+0x20]
  4384bd:	f7 d8                	neg    eax
  4384bf:	1b c0                	sbb    eax,eax
  4384c1:	83 e0 08             	and    eax,0x8
  4384c4:	40                   	inc    eax
  4384c5:	50                   	push   eax
  4384c6:	ff 75 18             	push   DWORD PTR [ebp+0x18]
  4384c9:	ff 15 d0 b0 43 00    	call   DWORD PTR ds:0x43b0d0
  4384cf:	89 45 e0             	mov    DWORD PTR [ebp-0x20],eax
  4384d2:	3b c3                	cmp    eax,ebx
  4384d4:	74 63                	je     0x438539
  4384d6:	89 5d fc             	mov    DWORD PTR [ebp-0x4],ebx
  4384d9:	8d 3c 00             	lea    edi,[eax+eax*1]
  4384dc:	8b c7                	mov    eax,edi
  4384de:	83 c0 03             	add    eax,0x3
  4384e1:	24 fc                	and    al,0xfc
  4384e3:	e8 18 8a ff ff       	call   0x430f00
  4384e8:	89 65 e8             	mov    DWORD PTR [ebp-0x18],esp
  4384eb:	8b f4                	mov    esi,esp
  4384ed:	89 75 dc             	mov    DWORD PTR [ebp-0x24],esi
  4384f0:	57                   	push   edi
  4384f1:	53                   	push   ebx
  4384f2:	56                   	push   esi
  4384f3:	e8 48 e0 ff ff       	call   0x436540
  4384f8:	83 c4 0c             	add    esp,0xc
  4384fb:	eb 0b                	jmp    0x438508
  4384fd:	6a 01                	push   0x1
  4384ff:	58                   	pop    eax
  438500:	c3                   	ret
  438501:	8b 65 e8             	mov    esp,DWORD PTR [ebp-0x18]
  438504:	33 db                	xor    ebx,ebx
  438506:	33 f6                	xor    esi,esi
  438508:	83 4d fc ff          	or     DWORD PTR [ebp-0x4],0xffffffff
  43850c:	3b f3                	cmp    esi,ebx
  43850e:	74 29                	je     0x438539
  438510:	ff 75 e0             	push   DWORD PTR [ebp-0x20]
  438513:	56                   	push   esi
  438514:	ff 75 10             	push   DWORD PTR [ebp+0x10]
  438517:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  43851a:	6a 01                	push   0x1
  43851c:	ff 75 18             	push   DWORD PTR [ebp+0x18]
  43851f:	ff 15 d0 b0 43 00    	call   DWORD PTR ds:0x43b0d0
  438525:	3b c3                	cmp    eax,ebx
  438527:	74 10                	je     0x438539
  438529:	ff 75 14             	push   DWORD PTR [ebp+0x14]
  43852c:	50                   	push   eax
  43852d:	56                   	push   esi
  43852e:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  438531:	ff 15 90 b1 43 00    	call   DWORD PTR ds:0x43b190
  438537:	eb 02                	jmp    0x43853b
  438539:	33 c0                	xor    eax,eax
  43853b:	8d 65 cc             	lea    esp,[ebp-0x34]
  43853e:	8b 4d f0             	mov    ecx,DWORD PTR [ebp-0x10]
  438541:	64                   	fs
