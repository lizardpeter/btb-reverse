
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0043576f <.text+0x3476f>:
  43576f:	55                   	push   ebp
  435770:	8b ec                	mov    ebp,esp
  435772:	53                   	push   ebx
  435773:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  435776:	e8 35 01 00 00       	call   0x4358b0
  43577b:	85 c0                	test   eax,eax
  43577d:	59                   	pop    ecx
  43577e:	0f 84 20 01 00 00    	je     0x4358a4
  435784:	8b 58 08             	mov    ebx,DWORD PTR [eax+0x8]
  435787:	85 db                	test   ebx,ebx
  435789:	0f 84 15 01 00 00    	je     0x4358a4
  43578f:	83 fb 05             	cmp    ebx,0x5
  435792:	75 0c                	jne    0x4357a0
  435794:	83 60 08 00          	and    DWORD PTR [eax+0x8],0x0
  435798:	6a 01                	push   0x1
  43579a:	58                   	pop    eax
  43579b:	e9 0d 01 00 00       	jmp    0x4358ad
  4357a0:	83 fb 01             	cmp    ebx,0x1
  4357a3:	0f 84 f6 00 00 00    	je     0x43589f
  4357a9:	8b 0d 58 c4 51 00    	mov    ecx,DWORD PTR ds:0x51c458
  4357af:	89 4d 08             	mov    DWORD PTR [ebp+0x8],ecx
  4357b2:	8b 4d 0c             	mov    ecx,DWORD PTR [ebp+0xc]
  4357b5:	89 0d 58 c4 51 00    	mov    DWORD PTR ds:0x51c458,ecx
  4357bb:	8b 48 04             	mov    ecx,DWORD PTR [eax+0x4]
  4357be:	83 f9 08             	cmp    ecx,0x8
  4357c1:	0f 85 c8 00 00 00    	jne    0x43588f
  4357c7:	8b 0d e8 97 44 00    	mov    ecx,DWORD PTR ds:0x4497e8
  4357cd:	8b 15 ec 97 44 00    	mov    edx,DWORD PTR ds:0x4497ec
  4357d3:	03 d1                	add    edx,ecx
  4357d5:	56                   	push   esi
  4357d6:	3b ca                	cmp    ecx,edx
  4357d8:	7d 15                	jge    0x4357ef
  4357da:	8d 34 49             	lea    esi,[ecx+ecx*2]
  4357dd:	2b d1                	sub    edx,ecx
  4357df:	8d 34 b5 78 97 44 00 	lea    esi,[esi*4+0x449778]
  4357e6:	83 26 00             	and    DWORD PTR [esi],0x0
  4357e9:	83 c6 0c             	add    esi,0xc
  4357ec:	4a                   	dec    edx
  4357ed:	75 f7                	jne    0x4357e6
  4357ef:	8b 00                	mov    eax,DWORD PTR [eax]
  4357f1:	8b 35 f4 97 44 00    	mov    esi,DWORD PTR ds:0x4497f4
  4357f7:	3d 8e 00 00 c0       	cmp    eax,0xc000008e
  4357fc:	75 0c                	jne    0x43580a
  4357fe:	c7 05 f4 97 44 00 83 	mov    DWORD PTR ds:0x4497f4,0x83
  435805:	00 00 00 
  435808:	eb 70                	jmp    0x43587a
  43580a:	3d 90 00 00 c0       	cmp    eax,0xc0000090
  43580f:	75 0c                	jne    0x43581d
  435811:	c7 05 f4 97 44 00 81 	mov    DWORD PTR ds:0x4497f4,0x81
  435818:	00 00 00 
  43581b:	eb 5d                	jmp    0x43587a
  43581d:	3d 91 00 00 c0       	cmp    eax,0xc0000091
  435822:	75 0c                	jne    0x435830
  435824:	c7 05 f4 97 44 00 84 	mov    DWORD PTR ds:0x4497f4,0x84
  43582b:	00 00 00 
  43582e:	eb 4a                	jmp    0x43587a
  435830:	3d 93 00 00 c0       	cmp    eax,0xc0000093
  435835:	75 0c                	jne    0x435843
  435837:	c7 05 f4 97 44 00 85 	mov    DWORD PTR ds:0x4497f4,0x85
  43583e:	00 00 00 
  435841:	eb 37                	jmp    0x43587a
  435843:	3d 8d 00 00 c0       	cmp    eax,0xc000008d
  435848:	75 0c                	jne    0x435856
  43584a:	c7 05 f4 97 44 00 82 	mov    DWORD PTR ds:0x4497f4,0x82
  435851:	00 00 00 
  435854:	eb 24                	jmp    0x43587a
  435856:	3d 8f 00 00 c0       	cmp    eax,0xc000008f
  43585b:	75 0c                	jne    0x435869
  43585d:	c7 05 f4 97 44 00 86 	mov    DWORD PTR ds:0x4497f4,0x86
  435864:	00 00 00 
  435867:	eb 11                	jmp    0x43587a
  435869:	3d 92 00 00 c0       	cmp    eax,0xc0000092
  43586e:	75 0a                	jne    0x43587a
  435870:	c7 05 f4 97 44 00 8a 	mov    DWORD PTR ds:0x4497f4,0x8a
  435877:	00 00 00 
  43587a:	ff 35 f4 97 44 00    	push   DWORD PTR ds:0x4497f4
  435880:	6a 08                	push   0x8
  435882:	ff d3                	call   ebx
  435884:	59                   	pop    ecx
  435885:	89 35 f4 97 44 00    	mov    DWORD PTR ds:0x4497f4,esi
  43588b:	59                   	pop    ecx
  43588c:	5e                   	pop    esi
  43588d:	eb 08                	jmp    0x435897
  43588f:	83 60 08 00          	and    DWORD PTR [eax+0x8],0x0
  435893:	51                   	push   ecx
  435894:	ff d3                	call   ebx
  435896:	59                   	pop    ecx
  435897:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  43589a:	a3 58 c4 51 00       	mov    ds:0x51c458,eax
  43589f:	83 c8 ff             	or     eax,0xffffffff
  4358a2:	eb 09                	jmp    0x4358ad
  4358a4:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  4358a7:	ff 15 cc b0 43 00    	call   DWORD PTR ds:0x43b0cc
  4358ad:	5b                   	pop    ebx
  4358ae:	5d                   	pop    ebp
  4358af:	c3                   	ret
