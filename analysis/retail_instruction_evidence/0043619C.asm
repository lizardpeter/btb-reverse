
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0043619c <.text+0x3519c>:
  43619c:	53                   	push   ebx
  43619d:	56                   	push   esi
  43619e:	57                   	push   edi
  43619f:	83 cb ff             	or     ebx,0xffffffff
  4361a2:	33 ff                	xor    edi,edi
  4361a4:	33 f6                	xor    esi,esi
  4361a6:	b9 20 d9 51 00       	mov    ecx,0x51d920
  4361ab:	8b 01                	mov    eax,DWORD PTR [ecx]
  4361ad:	85 c0                	test   eax,eax
  4361af:	74 37                	je     0x4361e8
  4361b1:	8d 90 00 01 00 00    	lea    edx,[eax+0x100]
  4361b7:	3b c2                	cmp    eax,edx
  4361b9:	73 1c                	jae    0x4361d7
  4361bb:	f6 40 04 01          	test   BYTE PTR [eax+0x4],0x1
  4361bf:	74 05                	je     0x4361c6
  4361c1:	83 c0 08             	add    eax,0x8
  4361c4:	eb f1                	jmp    0x4361b7
  4361c6:	83 08 ff             	or     DWORD PTR [eax],0xffffffff
  4361c9:	2b 01                	sub    eax,DWORD PTR [ecx]
  4361cb:	c1 f8 03             	sar    eax,0x3
  4361ce:	03 c6                	add    eax,esi
  4361d0:	8b d8                	mov    ebx,eax
  4361d2:	83 fb ff             	cmp    ebx,0xffffffff
  4361d5:	75 54                	jne    0x43622b
  4361d7:	83 c1 04             	add    ecx,0x4
  4361da:	47                   	inc    edi
  4361db:	83 c6 20             	add    esi,0x20
  4361de:	81 f9 20 da 51 00    	cmp    ecx,0x51da20
  4361e4:	7c c5                	jl     0x4361ab
  4361e6:	eb 43                	jmp    0x43622b
  4361e8:	be 00 01 00 00       	mov    esi,0x100
  4361ed:	56                   	push   esi
  4361ee:	e8 a0 ab ff ff       	call   0x430d93
  4361f3:	85 c0                	test   eax,eax
  4361f5:	59                   	pop    ecx
  4361f6:	74 33                	je     0x43622b
  4361f8:	83 05 20 da 51 00 20 	add    DWORD PTR ds:0x51da20,0x20
  4361ff:	8d 0c bd 20 d9 51 00 	lea    ecx,[edi*4+0x51d920]
  436206:	8d 90 00 01 00 00    	lea    edx,[eax+0x100]
  43620c:	89 01                	mov    DWORD PTR [ecx],eax
  43620e:	3b c2                	cmp    eax,edx
  436210:	73 14                	jae    0x436226
  436212:	80 60 04 00          	and    BYTE PTR [eax+0x4],0x0
  436216:	83 08 ff             	or     DWORD PTR [eax],0xffffffff
  436219:	c6 40 05 0a          	mov    BYTE PTR [eax+0x5],0xa
  43621d:	8b 11                	mov    edx,DWORD PTR [ecx]
  43621f:	83 c0 08             	add    eax,0x8
  436222:	03 d6                	add    edx,esi
  436224:	eb e8                	jmp    0x43620e
  436226:	c1 e7 05             	shl    edi,0x5
  436229:	8b df                	mov    ebx,edi
  43622b:	5f                   	pop    edi
  43622c:	8b c3                	mov    eax,ebx
  43622e:	5e                   	pop    esi
  43622f:	5b                   	pop    ebx
  436230:	c3                   	ret
