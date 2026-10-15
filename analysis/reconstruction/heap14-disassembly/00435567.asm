
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00435567 <.text+0x34567>:
  435567:	55                   	push   ebp
  435568:	8b ec                	mov    ebp,esp
  43556a:	51                   	push   ecx
  43556b:	8b 55 10             	mov    edx,DWORD PTR [ebp+0x10]
  43556e:	53                   	push   ebx
  43556f:	8b 5d 0c             	mov    ebx,DWORD PTR [ebp+0xc]
  435572:	56                   	push   esi
  435573:	0f b6 0a             	movzx  ecx,BYTE PTR [edx]
  435576:	57                   	push   edi
  435577:	8b 7d 08             	mov    edi,DWORD PTR [ebp+0x8]
  43557a:	83 65 fc 00          	and    DWORD PTR [ebp-0x4],0x0
  43557e:	8b c3                	mov    eax,ebx
  435580:	2b 47 10             	sub    eax,DWORD PTR [edi+0x10]
  435583:	c1 f8 0c             	sar    eax,0xc
  435586:	3b 4d 14             	cmp    ecx,DWORD PTR [ebp+0x14]
  435589:	8d 7c c7 18          	lea    edi,[edi+eax*8+0x18]
  43558d:	76 12                	jbe    0x4355a1
  43558f:	8b 45 14             	mov    eax,DWORD PTR [ebp+0x14]
  435592:	2b c8                	sub    ecx,eax
  435594:	88 02                	mov    BYTE PTR [edx],al
  435596:	01 0f                	add    DWORD PTR [edi],ecx
  435598:	c7 47 04 f1 00 00 00 	mov    DWORD PTR [edi+0x4],0xf1
  43559f:	eb 60                	jmp    0x435601
  4355a1:	73 65                	jae    0x435608
  4355a3:	8b 45 14             	mov    eax,DWORD PTR [ebp+0x14]
  4355a6:	8d 34 02             	lea    esi,[edx+eax*1]
  4355a9:	8d 83 f8 00 00 00    	lea    eax,[ebx+0xf8]
  4355af:	3b f0                	cmp    esi,eax
  4355b1:	77 55                	ja     0x435608
  4355b3:	8d 04 11             	lea    eax,[ecx+edx*1]
  4355b6:	3b c6                	cmp    eax,esi
  4355b8:	73 0a                	jae    0x4355c4
  4355ba:	80 38 00             	cmp    BYTE PTR [eax],0x0
  4355bd:	75 03                	jne    0x4355c2
  4355bf:	40                   	inc    eax
  4355c0:	eb f4                	jmp    0x4355b6
  4355c2:	3b c6                	cmp    eax,esi
  4355c4:	75 42                	jne    0x435608
  4355c6:	8a 45 14             	mov    al,BYTE PTR [ebp+0x14]
  4355c9:	88 02                	mov    BYTE PTR [edx],al
  4355cb:	8b 03                	mov    eax,DWORD PTR [ebx]
  4355cd:	3b d0                	cmp    edx,eax
  4355cf:	77 2b                	ja     0x4355fc
  4355d1:	3b f0                	cmp    esi,eax
  4355d3:	76 27                	jbe    0x4355fc
  4355d5:	8d 83 f8 00 00 00    	lea    eax,[ebx+0xf8]
  4355db:	3b f0                	cmp    esi,eax
  4355dd:	73 14                	jae    0x4355f3
  4355df:	33 c0                	xor    eax,eax
  4355e1:	89 33                	mov    DWORD PTR [ebx],esi
  4355e3:	38 06                	cmp    BYTE PTR [esi],al
  4355e5:	75 07                	jne    0x4355ee
  4355e7:	40                   	inc    eax
  4355e8:	80 3c 06 00          	cmp    BYTE PTR [esi+eax*1],0x0
  4355ec:	74 f9                	je     0x4355e7
  4355ee:	89 43 04             	mov    DWORD PTR [ebx+0x4],eax
  4355f1:	eb 09                	jmp    0x4355fc
  4355f3:	83 63 04 00          	and    DWORD PTR [ebx+0x4],0x0
  4355f7:	8d 43 08             	lea    eax,[ebx+0x8]
  4355fa:	89 03                	mov    DWORD PTR [ebx],eax
  4355fc:	2b 4d 14             	sub    ecx,DWORD PTR [ebp+0x14]
  4355ff:	01 0f                	add    DWORD PTR [edi],ecx
  435601:	c7 45 fc 01 00 00 00 	mov    DWORD PTR [ebp-0x4],0x1
  435608:	8b 45 fc             	mov    eax,DWORD PTR [ebp-0x4]
  43560b:	5f                   	pop    edi
  43560c:	5e                   	pop    esi
  43560d:	5b                   	pop    ebx
  43560e:	c9                   	leave
  43560f:	c3                   	ret
