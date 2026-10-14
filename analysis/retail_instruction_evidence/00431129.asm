
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00431129 <.text+0x30129>:
  431129:	53                   	push   ebx
  43112a:	55                   	push   ebp
  43112b:	56                   	push   esi
  43112c:	57                   	push   edi
  43112d:	8b 7c 24 14          	mov    edi,DWORD PTR [esp+0x14]
  431131:	3b 3d 20 da 51 00    	cmp    edi,DWORD PTR ds:0x51da20
  431137:	0f 83 86 00 00 00    	jae    0x4311c3
  43113d:	8b c7                	mov    eax,edi
  43113f:	8b f7                	mov    esi,edi
  431141:	c1 f8 05             	sar    eax,0x5
  431144:	83 e6 1f             	and    esi,0x1f
  431147:	8d 1c 85 20 d9 51 00 	lea    ebx,[eax*4+0x51d920]
  43114e:	c1 e6 03             	shl    esi,0x3
  431151:	8b 03                	mov    eax,DWORD PTR [ebx]
  431153:	f6 44 30 04 01       	test   BYTE PTR [eax+esi*1+0x4],0x1
  431158:	74 69                	je     0x4311c3
  43115a:	57                   	push   edi
  43115b:	e8 c2 51 00 00       	call   0x436322
  431160:	83 f8 ff             	cmp    eax,0xffffffff
  431163:	59                   	pop    ecx
  431164:	74 3c                	je     0x4311a2
  431166:	83 ff 01             	cmp    edi,0x1
  431169:	74 05                	je     0x431170
  43116b:	83 ff 02             	cmp    edi,0x2
  43116e:	75 16                	jne    0x431186
  431170:	6a 02                	push   0x2
  431172:	e8 ab 51 00 00       	call   0x436322
  431177:	6a 01                	push   0x1
  431179:	8b e8                	mov    ebp,eax
  43117b:	e8 a2 51 00 00       	call   0x436322
  431180:	59                   	pop    ecx
  431181:	3b c5                	cmp    eax,ebp
  431183:	59                   	pop    ecx
  431184:	74 1c                	je     0x4311a2
  431186:	57                   	push   edi
  431187:	e8 96 51 00 00       	call   0x436322
  43118c:	59                   	pop    ecx
  43118d:	50                   	push   eax
  43118e:	ff 15 58 b1 43 00    	call   DWORD PTR ds:0x43b158
  431194:	85 c0                	test   eax,eax
  431196:	75 0a                	jne    0x4311a2
  431198:	ff 15 2c b1 43 00    	call   DWORD PTR ds:0x43b12c
  43119e:	8b e8                	mov    ebp,eax
  4311a0:	eb 02                	jmp    0x4311a4
  4311a2:	33 ed                	xor    ebp,ebp
  4311a4:	57                   	push   edi
  4311a5:	e8 fe 50 00 00       	call   0x4362a8
  4311aa:	8b 03                	mov    eax,DWORD PTR [ebx]
  4311ac:	59                   	pop    ecx
  4311ad:	80 64 30 04 00       	and    BYTE PTR [eax+esi*1+0x4],0x0
  4311b2:	85 ed                	test   ebp,ebp
  4311b4:	74 09                	je     0x4311bf
  4311b6:	55                   	push   ebp
  4311b7:	e8 be 15 00 00       	call   0x43277a
  4311bc:	59                   	pop    ecx
  4311bd:	eb 15                	jmp    0x4311d4
  4311bf:	33 c0                	xor    eax,eax
  4311c1:	eb 14                	jmp    0x4311d7
  4311c3:	83 25 cc c3 51 00 00 	and    DWORD PTR ds:0x51c3cc,0x0
  4311ca:	c7 05 c8 c3 51 00 09 	mov    DWORD PTR ds:0x51c3c8,0x9
  4311d1:	00 00 00 
  4311d4:	83 c8 ff             	or     eax,0xffffffff
  4311d7:	5f                   	pop    edi
  4311d8:	5e                   	pop    esi
  4311d9:	5d                   	pop    ebp
  4311da:	5b                   	pop    ebx
  4311db:	c3                   	ret
