
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004390cc <.text+0x380cc>:
  4390cc:	55                   	push   ebp
  4390cd:	8b ec                	mov    ebp,esp
  4390cf:	b8 00 10 00 00       	mov    eax,0x1000
  4390d4:	e8 27 7e ff ff       	call   0x430f00
  4390d9:	53                   	push   ebx
  4390da:	8b 5d 08             	mov    ebx,DWORD PTR [ebp+0x8]
  4390dd:	56                   	push   esi
  4390de:	33 f6                	xor    esi,esi
  4390e0:	3b 1d 20 da 51 00    	cmp    ebx,DWORD PTR ds:0x51da20
  4390e6:	0f 83 15 01 00 00    	jae    0x439201
  4390ec:	8b c3                	mov    eax,ebx
  4390ee:	8b cb                	mov    ecx,ebx
  4390f0:	c1 f8 05             	sar    eax,0x5
  4390f3:	83 e1 1f             	and    ecx,0x1f
  4390f6:	8b 04 85 20 d9 51 00 	mov    eax,DWORD PTR [eax*4+0x51d920]
  4390fd:	f6 44 c8 04 01       	test   BYTE PTR [eax+ecx*8+0x4],0x1
  439102:	0f 84 f9 00 00 00    	je     0x439201
  439108:	6a 01                	push   0x1
  43910a:	56                   	push   esi
  43910b:	53                   	push   ebx
  43910c:	e8 c6 e1 ff ff       	call   0x4372d7
  439111:	83 c4 0c             	add    esp,0xc
  439114:	83 f8 ff             	cmp    eax,0xffffffff
  439117:	89 45 08             	mov    DWORD PTR [ebp+0x8],eax
  43911a:	0f 84 eb 00 00 00    	je     0x43920b
  439120:	6a 02                	push   0x2
  439122:	56                   	push   esi
  439123:	53                   	push   ebx
  439124:	e8 ae e1 ff ff       	call   0x4372d7
  439129:	83 c4 0c             	add    esp,0xc
  43912c:	83 f8 ff             	cmp    eax,0xffffffff
  43912f:	0f 84 d6 00 00 00    	je     0x43920b
  439135:	57                   	push   edi
  439136:	8b 7d 0c             	mov    edi,DWORD PTR [ebp+0xc]
  439139:	2b f8                	sub    edi,eax
  43913b:	85 ff                	test   edi,edi
  43913d:	7e 6f                	jle    0x4391ae
  43913f:	68 00 10 00 00       	push   0x1000
  439144:	8d 85 00 f0 ff ff    	lea    eax,[ebp-0x1000]
  43914a:	56                   	push   esi
  43914b:	50                   	push   eax
  43914c:	e8 ef d3 ff ff       	call   0x436540
  439151:	68 00 80 00 00       	push   0x8000
  439156:	53                   	push   ebx
  439157:	e8 cb 04 00 00       	call   0x439627
  43915c:	83 c4 14             	add    esp,0x14
  43915f:	89 45 0c             	mov    DWORD PTR [ebp+0xc],eax
  439162:	b8 00 10 00 00       	mov    eax,0x1000
  439167:	3b f8                	cmp    edi,eax
  439169:	7d 02                	jge    0x43916d
  43916b:	8b c7                	mov    eax,edi
  43916d:	50                   	push   eax
  43916e:	8d 85 00 f0 ff ff    	lea    eax,[ebp-0x1000]
  439174:	50                   	push   eax
  439175:	53                   	push   ebx
  439176:	e8 02 af ff ff       	call   0x43407d
  43917b:	83 c4 0c             	add    esp,0xc
  43917e:	83 f8 ff             	cmp    eax,0xffffffff
  439181:	74 08                	je     0x43918b
  439183:	2b f8                	sub    edi,eax
  439185:	85 ff                	test   edi,edi
  439187:	7e 18                	jle    0x4391a1
  439189:	eb d7                	jmp    0x439162
  43918b:	83 3d cc c3 51 00 05 	cmp    DWORD PTR ds:0x51c3cc,0x5
  439192:	75 0a                	jne    0x43919e
  439194:	c7 05 c8 c3 51 00 0d 	mov    DWORD PTR ds:0x51c3c8,0xd
  43919b:	00 00 00 
  43919e:	83 ce ff             	or     esi,0xffffffff
  4391a1:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  4391a4:	53                   	push   ebx
  4391a5:	e8 7d 04 00 00       	call   0x439627
  4391aa:	59                   	pop    ecx
  4391ab:	59                   	pop    ecx
  4391ac:	eb 40                	jmp    0x4391ee
  4391ae:	7d 3e                	jge    0x4391ee
  4391b0:	6a 00                	push   0x0
  4391b2:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  4391b5:	53                   	push   ebx
  4391b6:	e8 1c e1 ff ff       	call   0x4372d7
  4391bb:	53                   	push   ebx
  4391bc:	e8 61 d1 ff ff       	call   0x436322
  4391c1:	83 c4 10             	add    esp,0x10
  4391c4:	50                   	push   eax
  4391c5:	ff 15 94 b1 43 00    	call   DWORD PTR ds:0x43b194
  4391cb:	8b f0                	mov    esi,eax
  4391cd:	f7 de                	neg    esi
  4391cf:	1b f6                	sbb    esi,esi
  4391d1:	f7 de                	neg    esi
  4391d3:	4e                   	dec    esi
  4391d4:	83 fe ff             	cmp    esi,0xffffffff
  4391d7:	75 15                	jne    0x4391ee
  4391d9:	c7 05 c8 c3 51 00 0d 	mov    DWORD PTR ds:0x51c3c8,0xd
  4391e0:	00 00 00 
  4391e3:	ff 15 2c b1 43 00    	call   DWORD PTR ds:0x43b12c
  4391e9:	a3 cc c3 51 00       	mov    ds:0x51c3cc,eax
  4391ee:	6a 00                	push   0x0
  4391f0:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  4391f3:	53                   	push   ebx
  4391f4:	e8 de e0 ff ff       	call   0x4372d7
  4391f9:	83 c4 0c             	add    esp,0xc
  4391fc:	8b c6                	mov    eax,esi
  4391fe:	5f                   	pop    edi
  4391ff:	eb 0d                	jmp    0x43920e
  439201:	c7 05 c8 c3 51 00 09 	mov    DWORD PTR ds:0x51c3c8,0x9
  439208:	00 00 00 
  43920b:	83 c8 ff             	or     eax,0xffffffff
  43920e:	5e                   	pop    esi
  43920f:	5b                   	pop    ebx
  439210:	c9                   	leave
  439211:	c3                   	ret
