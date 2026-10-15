
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004395ab <.text+0x385ab>:
  4395ab:	55                   	push   ebp
  4395ac:	8b ec                	mov    ebp,esp
  4395ae:	83 ec 0c             	sub    esp,0xc
  4395b1:	53                   	push   ebx
  4395b2:	bb 80 9f 44 00       	mov    ebx,0x449f80
  4395b7:	33 c9                	xor    ecx,ecx
  4395b9:	83 eb 60             	sub    ebx,0x60
  4395bc:	39 4d 0c             	cmp    DWORD PTR [ebp+0xc],ecx
  4395bf:	74 63                	je     0x439624
  4395c1:	7d 10                	jge    0x4395d3
  4395c3:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  4395c6:	bb e0 a0 44 00       	mov    ebx,0x44a0e0
  4395cb:	f7 d8                	neg    eax
  4395cd:	89 45 0c             	mov    DWORD PTR [ebp+0xc],eax
  4395d0:	83 eb 60             	sub    ebx,0x60
  4395d3:	39 4d 10             	cmp    DWORD PTR [ebp+0x10],ecx
  4395d6:	75 06                	jne    0x4395de
  4395d8:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  4395db:	66 89 08             	mov    WORD PTR [eax],cx
  4395de:	39 4d 0c             	cmp    DWORD PTR [ebp+0xc],ecx
  4395e1:	74 41                	je     0x439624
  4395e3:	56                   	push   esi
  4395e4:	57                   	push   edi
  4395e5:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  4395e8:	83 c3 54             	add    ebx,0x54
  4395eb:	c1 7d 0c 03          	sar    DWORD PTR [ebp+0xc],0x3
  4395ef:	83 e0 07             	and    eax,0x7
  4395f2:	3b c1                	cmp    eax,ecx
  4395f4:	74 27                	je     0x43961d
  4395f6:	8d 04 40             	lea    eax,[eax+eax*2]
  4395f9:	66 81 3c 83 00 80    	cmp    WORD PTR [ebx+eax*4],0x8000
  4395ff:	8d 34 83             	lea    esi,[ebx+eax*4]
  439602:	72 0c                	jb     0x439610
  439604:	8d 7d f4             	lea    edi,[ebp-0xc]
  439607:	a5                   	movs   DWORD PTR es:[edi],DWORD PTR ds:[esi]
  439608:	a5                   	movs   DWORD PTR es:[edi],DWORD PTR ds:[esi]
  439609:	a5                   	movs   DWORD PTR es:[edi],DWORD PTR ds:[esi]
  43960a:	ff 4d f6             	dec    DWORD PTR [ebp-0xa]
  43960d:	8d 75 f4             	lea    esi,[ebp-0xc]
  439610:	56                   	push   esi
  439611:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  439614:	e8 72 fd ff ff       	call   0x43938b
  439619:	59                   	pop    ecx
  43961a:	59                   	pop    ecx
  43961b:	33 c9                	xor    ecx,ecx
  43961d:	39 4d 0c             	cmp    DWORD PTR [ebp+0xc],ecx
  439620:	75 c3                	jne    0x4395e5
  439622:	5f                   	pop    edi
  439623:	5e                   	pop    esi
  439624:	5b                   	pop    ebx
  439625:	c9                   	leave
  439626:	c3                   	ret
