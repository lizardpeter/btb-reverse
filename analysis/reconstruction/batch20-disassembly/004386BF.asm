
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004386bf <.text+0x376bf>:
  4386bf:	53                   	push   ebx
  4386c0:	55                   	push   ebp
  4386c1:	56                   	push   esi
  4386c2:	57                   	push   edi
  4386c3:	8b 7c 24 14          	mov    edi,DWORD PTR [esp+0x14]
  4386c7:	83 3d 10 9b 44 00 01 	cmp    DWORD PTR ds:0x449b10,0x1
  4386ce:	7e 0f                	jle    0x4386df
  4386d0:	0f b6 07             	movzx  eax,BYTE PTR [edi]
  4386d3:	6a 08                	push   0x8
  4386d5:	50                   	push   eax
  4386d6:	e8 e5 de ff ff       	call   0x4365c0
  4386db:	59                   	pop    ecx
  4386dc:	59                   	pop    ecx
  4386dd:	eb 0f                	jmp    0x4386ee
  4386df:	0f b6 07             	movzx  eax,BYTE PTR [edi]
  4386e2:	8b 0d 1c 9b 44 00    	mov    ecx,DWORD PTR ds:0x449b1c
  4386e8:	8a 04 41             	mov    al,BYTE PTR [ecx+eax*2]
  4386eb:	83 e0 08             	and    eax,0x8
  4386ee:	85 c0                	test   eax,eax
  4386f0:	74 03                	je     0x4386f5
  4386f2:	47                   	inc    edi
  4386f3:	eb d2                	jmp    0x4386c7
  4386f5:	0f b6 37             	movzx  esi,BYTE PTR [edi]
  4386f8:	47                   	inc    edi
  4386f9:	83 fe 2d             	cmp    esi,0x2d
  4386fc:	8b ee                	mov    ebp,esi
  4386fe:	74 05                	je     0x438705
  438700:	83 fe 2b             	cmp    esi,0x2b
  438703:	75 04                	jne    0x438709
  438705:	0f b6 37             	movzx  esi,BYTE PTR [edi]
  438708:	47                   	inc    edi
  438709:	33 db                	xor    ebx,ebx
  43870b:	83 3d 10 9b 44 00 01 	cmp    DWORD PTR ds:0x449b10,0x1
  438712:	7e 0c                	jle    0x438720
  438714:	6a 04                	push   0x4
  438716:	56                   	push   esi
  438717:	e8 a4 de ff ff       	call   0x4365c0
  43871c:	59                   	pop    ecx
  43871d:	59                   	pop    ecx
  43871e:	eb 0b                	jmp    0x43872b
  438720:	a1 1c 9b 44 00       	mov    eax,ds:0x449b1c
  438725:	8a 04 70             	mov    al,BYTE PTR [eax+esi*2]
  438728:	83 e0 04             	and    eax,0x4
  43872b:	85 c0                	test   eax,eax
  43872d:	74 0d                	je     0x43873c
  43872f:	8d 04 9b             	lea    eax,[ebx+ebx*4]
  438732:	8d 5c 46 d0          	lea    ebx,[esi+eax*2-0x30]
  438736:	0f b6 37             	movzx  esi,BYTE PTR [edi]
  438739:	47                   	inc    edi
  43873a:	eb cf                	jmp    0x43870b
  43873c:	83 fd 2d             	cmp    ebp,0x2d
  43873f:	8b c3                	mov    eax,ebx
  438741:	75 02                	jne    0x438745
  438743:	f7 d8                	neg    eax
  438745:	5f                   	pop    edi
  438746:	5e                   	pop    esi
  438747:	5d                   	pop    ebp
  438748:	5b                   	pop    ebx
  438749:	c3                   	ret
