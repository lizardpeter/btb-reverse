
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00433640 <.text+0x32640>:
  433640:	8b 4c 24 04          	mov    ecx,DWORD PTR [esp+0x4]
  433644:	57                   	push   edi
  433645:	f7 c1 03 00 00 00    	test   ecx,0x3
  43364b:	74 0f                	je     0x43365c
  43364d:	8a 01                	mov    al,BYTE PTR [ecx]
  43364f:	41                   	inc    ecx
  433650:	84 c0                	test   al,al
  433652:	74 3b                	je     0x43368f
  433654:	f7 c1 03 00 00 00    	test   ecx,0x3
  43365a:	75 f1                	jne    0x43364d
  43365c:	8b 01                	mov    eax,DWORD PTR [ecx]
  43365e:	ba ff fe fe 7e       	mov    edx,0x7efefeff
  433663:	03 d0                	add    edx,eax
  433665:	83 f0 ff             	xor    eax,0xffffffff
  433668:	33 c2                	xor    eax,edx
  43366a:	83 c1 04             	add    ecx,0x4
  43366d:	a9 00 01 01 81       	test   eax,0x81010100
  433672:	74 e8                	je     0x43365c
  433674:	8b 41 fc             	mov    eax,DWORD PTR [ecx-0x4]
  433677:	84 c0                	test   al,al
  433679:	74 23                	je     0x43369e
  43367b:	84 e4                	test   ah,ah
  43367d:	74 1a                	je     0x433699
  43367f:	a9 00 00 ff 00       	test   eax,0xff0000
  433684:	74 0e                	je     0x433694
  433686:	a9 00 00 00 ff       	test   eax,0xff000000
  43368b:	74 02                	je     0x43368f
  43368d:	eb cd                	jmp    0x43365c
  43368f:	8d 79 ff             	lea    edi,[ecx-0x1]
  433692:	eb 0d                	jmp    0x4336a1
  433694:	8d 79 fe             	lea    edi,[ecx-0x2]
  433697:	eb 08                	jmp    0x4336a1
  433699:	8d 79 fd             	lea    edi,[ecx-0x3]
  43369c:	eb 03                	jmp    0x4336a1
  43369e:	8d 79 fc             	lea    edi,[ecx-0x4]
  4336a1:	8b 4c 24 0c          	mov    ecx,DWORD PTR [esp+0xc]
  4336a5:	f7 c1 03 00 00 00    	test   ecx,0x3
  4336ab:	74 19                	je     0x4336c6
  4336ad:	8a 11                	mov    dl,BYTE PTR [ecx]
  4336af:	41                   	inc    ecx
  4336b0:	84 d2                	test   dl,dl
  4336b2:	74 64                	je     0x433718
  4336b4:	88 17                	mov    BYTE PTR [edi],dl
  4336b6:	47                   	inc    edi
  4336b7:	f7 c1 03 00 00 00    	test   ecx,0x3
  4336bd:	75 ee                	jne    0x4336ad
  4336bf:	eb 05                	jmp    0x4336c6
  4336c1:	89 17                	mov    DWORD PTR [edi],edx
  4336c3:	83 c7 04             	add    edi,0x4
  4336c6:	ba ff fe fe 7e       	mov    edx,0x7efefeff
  4336cb:	8b 01                	mov    eax,DWORD PTR [ecx]
  4336cd:	03 d0                	add    edx,eax
  4336cf:	83 f0 ff             	xor    eax,0xffffffff
  4336d2:	33 c2                	xor    eax,edx
  4336d4:	8b 11                	mov    edx,DWORD PTR [ecx]
  4336d6:	83 c1 04             	add    ecx,0x4
  4336d9:	a9 00 01 01 81       	test   eax,0x81010100
  4336de:	74 e1                	je     0x4336c1
  4336e0:	84 d2                	test   dl,dl
  4336e2:	74 34                	je     0x433718
  4336e4:	84 f6                	test   dh,dh
  4336e6:	74 27                	je     0x43370f
  4336e8:	f7 c2 00 00 ff 00    	test   edx,0xff0000
  4336ee:	74 12                	je     0x433702
  4336f0:	f7 c2 00 00 00 ff    	test   edx,0xff000000
  4336f6:	74 02                	je     0x4336fa
  4336f8:	eb c7                	jmp    0x4336c1
  4336fa:	89 17                	mov    DWORD PTR [edi],edx
  4336fc:	8b 44 24 08          	mov    eax,DWORD PTR [esp+0x8]
  433700:	5f                   	pop    edi
  433701:	c3                   	ret
  433702:	66 89 17             	mov    WORD PTR [edi],dx
  433705:	8b 44 24 08          	mov    eax,DWORD PTR [esp+0x8]
  433709:	c6 47 02 00          	mov    BYTE PTR [edi+0x2],0x0
  43370d:	5f                   	pop    edi
  43370e:	c3                   	ret
  43370f:	66 89 17             	mov    WORD PTR [edi],dx
  433712:	8b 44 24 08          	mov    eax,DWORD PTR [esp+0x8]
  433716:	5f                   	pop    edi
  433717:	c3                   	ret
  433718:	88 17                	mov    BYTE PTR [edi],dl
  43371a:	8b 44 24 08          	mov    eax,DWORD PTR [esp+0x8]
  43371e:	5f                   	pop    edi
  43371f:	c3                   	ret
