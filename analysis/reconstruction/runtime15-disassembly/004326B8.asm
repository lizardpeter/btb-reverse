
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004326b8 <.text+0x316b8>:
  4326b8:	55                   	push   ebp
  4326b9:	8b ec                	mov    ebp,esp
  4326bb:	83 ec 24             	sub    esp,0x24
  4326be:	53                   	push   ebx
  4326bf:	8b 5d 08             	mov    ebx,DWORD PTR [ebp+0x8]
  4326c2:	81 eb 6c 07 00 00    	sub    ebx,0x76c
  4326c8:	83 fb 46             	cmp    ebx,0x46
  4326cb:	0f 8c a3 00 00 00    	jl     0x432774
  4326d1:	81 fb 8a 00 00 00    	cmp    ebx,0x8a
  4326d7:	0f 8f 97 00 00 00    	jg     0x432774
  4326dd:	56                   	push   esi
  4326de:	57                   	push   edi
  4326df:	8b 7d 0c             	mov    edi,DWORD PTR [ebp+0xc]
  4326e2:	8b 34 bd 14 9e 44 00 	mov    esi,DWORD PTR [edi*4+0x449e14]
  4326e9:	03 75 10             	add    esi,DWORD PTR [ebp+0x10]
  4326ec:	f6 c3 03             	test   bl,0x3
  4326ef:	75 06                	jne    0x4326f7
  4326f1:	83 ff 02             	cmp    edi,0x2
  4326f4:	7e 01                	jle    0x4326f7
  4326f6:	46                   	inc    esi
  4326f7:	e8 78 40 00 00       	call   0x436774
  4326fc:	8b c3                	mov    eax,ebx
  4326fe:	8d 4b ff             	lea    ecx,[ebx-0x1]
  432701:	69 c0 6d 01 00 00    	imul   eax,eax,0x16d
  432707:	c1 f9 02             	sar    ecx,0x2
  43270a:	8b d6                	mov    edx,esi
  43270c:	89 75 f8             	mov    DWORD PTR [ebp-0x8],esi
  43270f:	03 d1                	add    edx,ecx
  432711:	89 5d f0             	mov    DWORD PTR [ebp-0x10],ebx
  432714:	03 c2                	add    eax,edx
  432716:	8b 55 1c             	mov    edx,DWORD PTR [ebp+0x1c]
  432719:	8d 0c 40             	lea    ecx,[eax+eax*2]
  43271c:	8b 45 14             	mov    eax,DWORD PTR [ebp+0x14]
  43271f:	89 45 e4             	mov    DWORD PTR [ebp-0x1c],eax
  432722:	8d 0c c8             	lea    ecx,[eax+ecx*8]
  432725:	6b c9 3c             	imul   ecx,ecx,0x3c
  432728:	03 4d 18             	add    ecx,DWORD PTR [ebp+0x18]
  43272b:	6b c9 3c             	imul   ecx,ecx,0x3c
  43272e:	03 0d 30 9d 44 00    	add    ecx,DWORD PTR ds:0x449d30
  432734:	4f                   	dec    edi
  432735:	83 7d 20 01          	cmp    DWORD PTR [ebp+0x20],0x1
  432739:	89 7d ec             	mov    DWORD PTR [ebp-0x14],edi
  43273c:	5f                   	pop    edi
  43273d:	5e                   	pop    esi
  43273e:	8d 8c 11 80 81 55 7c 	lea    ecx,[ecx+edx*1+0x7c558180]
  432745:	89 4d 08             	mov    DWORD PTR [ebp+0x8],ecx
  432748:	74 20                	je     0x43276a
  43274a:	83 7d 20 ff          	cmp    DWORD PTR [ebp+0x20],0xffffffff
  43274e:	75 20                	jne    0x432770
  432750:	83 3d 34 9d 44 00 00 	cmp    DWORD PTR ds:0x449d34,0x0
  432757:	74 17                	je     0x432770
  432759:	8d 45 dc             	lea    eax,[ebp-0x24]
  43275c:	50                   	push   eax
  43275d:	e8 85 42 00 00       	call   0x4369e7
  432762:	59                   	pop    ecx
  432763:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  432766:	85 c0                	test   eax,eax
  432768:	74 06                	je     0x432770
  43276a:	03 0d 38 9d 44 00    	add    ecx,DWORD PTR ds:0x449d38
  432770:	8b c1                	mov    eax,ecx
  432772:	eb 03                	jmp    0x432777
  432774:	83 c8 ff             	or     eax,0xffffffff
  432777:	5b                   	pop    ebx
  432778:	c9                   	leave
  432779:	c3                   	ret
