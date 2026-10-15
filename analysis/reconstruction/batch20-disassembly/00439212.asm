
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00439212 <.text+0x38212>:
  439212:	55                   	push   ebp
  439213:	8b ec                	mov    ebp,esp
  439215:	51                   	push   ecx
  439216:	83 3d 80 c6 51 00 00 	cmp    DWORD PTR ds:0x51c680,0x0
  43921d:	53                   	push   ebx
  43921e:	75 1d                	jne    0x43923d
  439220:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  439223:	83 f8 61             	cmp    eax,0x61
  439226:	0f 8c af 00 00 00    	jl     0x4392db
  43922c:	83 f8 7a             	cmp    eax,0x7a
  43922f:	0f 8f a6 00 00 00    	jg     0x4392db
  439235:	83 e8 20             	sub    eax,0x20
  439238:	e9 9e 00 00 00       	jmp    0x4392db
  43923d:	8b 5d 08             	mov    ebx,DWORD PTR [ebp+0x8]
  439240:	81 fb 00 01 00 00    	cmp    ebx,0x100
  439246:	7d 28                	jge    0x439270
  439248:	83 3d 10 9b 44 00 01 	cmp    DWORD PTR ds:0x449b10,0x1
  43924f:	7e 0c                	jle    0x43925d
  439251:	6a 02                	push   0x2
  439253:	53                   	push   ebx
  439254:	e8 67 d3 ff ff       	call   0x4365c0
  439259:	59                   	pop    ecx
  43925a:	59                   	pop    ecx
  43925b:	eb 0b                	jmp    0x439268
  43925d:	a1 1c 9b 44 00       	mov    eax,ds:0x449b1c
  439262:	8a 04 58             	mov    al,BYTE PTR [eax+ebx*2]
  439265:	83 e0 02             	and    eax,0x2
  439268:	85 c0                	test   eax,eax
  43926a:	75 04                	jne    0x439270
  43926c:	8b c3                	mov    eax,ebx
  43926e:	eb 6b                	jmp    0x4392db
  439270:	8b 15 1c 9b 44 00    	mov    edx,DWORD PTR ds:0x449b1c
  439276:	8b c3                	mov    eax,ebx
  439278:	c1 f8 08             	sar    eax,0x8
  43927b:	0f b6 c8             	movzx  ecx,al
  43927e:	f6 44 4a 01 80       	test   BYTE PTR [edx+ecx*2+0x1],0x80
  439283:	74 0e                	je     0x439293
  439285:	80 65 0a 00          	and    BYTE PTR [ebp+0xa],0x0
  439289:	88 45 08             	mov    BYTE PTR [ebp+0x8],al
  43928c:	88 5d 09             	mov    BYTE PTR [ebp+0x9],bl
  43928f:	6a 02                	push   0x2
  439291:	eb 09                	jmp    0x43929c
  439293:	80 65 09 00          	and    BYTE PTR [ebp+0x9],0x0
  439297:	88 5d 08             	mov    BYTE PTR [ebp+0x8],bl
  43929a:	6a 01                	push   0x1
  43929c:	58                   	pop    eax
  43929d:	8d 4d fc             	lea    ecx,[ebp-0x4]
  4392a0:	6a 01                	push   0x1
  4392a2:	6a 00                	push   0x0
  4392a4:	6a 03                	push   0x3
  4392a6:	51                   	push   ecx
  4392a7:	50                   	push   eax
  4392a8:	8d 45 08             	lea    eax,[ebp+0x8]
  4392ab:	50                   	push   eax
  4392ac:	68 00 02 00 00       	push   0x200
  4392b1:	ff 35 80 c6 51 00    	push   DWORD PTR ds:0x51c680
  4392b7:	e8 f7 dd ff ff       	call   0x4370b3
  4392bc:	83 c4 20             	add    esp,0x20
  4392bf:	85 c0                	test   eax,eax
  4392c1:	74 a9                	je     0x43926c
  4392c3:	83 f8 01             	cmp    eax,0x1
  4392c6:	75 06                	jne    0x4392ce
  4392c8:	0f b6 45 fc          	movzx  eax,BYTE PTR [ebp-0x4]
  4392cc:	eb 0d                	jmp    0x4392db
  4392ce:	0f b6 45 fd          	movzx  eax,BYTE PTR [ebp-0x3]
  4392d2:	0f b6 4d fc          	movzx  ecx,BYTE PTR [ebp-0x4]
  4392d6:	c1 e0 08             	shl    eax,0x8
  4392d9:	0b c1                	or     eax,ecx
  4392db:	5b                   	pop    ebx
  4392dc:	c9                   	leave
  4392dd:	c3                   	ret
