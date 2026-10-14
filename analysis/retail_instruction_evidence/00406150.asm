# source-exe-sha256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05

verified retail PE32:     file format pei-i386


Disassembly of section .text:

00406150 <.text+0x5150>:
  406150:	53                   	push   ebx
  406151:	55                   	push   ebp
  406152:	56                   	push   esi
  406153:	8b 74 24 14          	mov    esi,DWORD PTR [esp+0x14]
  406157:	57                   	push   edi
  406158:	8b 7c 24 14          	mov    edi,DWORD PTR [esp+0x14]
  40615c:	8b e9                	mov    ebp,ecx
  40615e:	39 77 08             	cmp    DWORD PTR [edi+0x8],esi
  406161:	73 05                	jae    0x406168
  406163:	e8 8d 3f 03 00       	call   0x43a0f5
  406168:	8b 47 08             	mov    eax,DWORD PTR [edi+0x8]
  40616b:	8b 4c 24 1c          	mov    ecx,DWORD PTR [esp+0x1c]
  40616f:	8b d8                	mov    ebx,eax
  406171:	2b de                	sub    ebx,esi
  406173:	3b cb                	cmp    ecx,ebx
  406175:	73 02                	jae    0x406179
  406177:	8b d9                	mov    ebx,ecx
  406179:	3b ef                	cmp    ebp,edi
  40617b:	0f 85 b5 00 00 00    	jne    0x406236
  406181:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  406184:	8b 3d 34 b3 43 00    	mov    edi,DWORD PTR ds:0x43b334
  40618a:	03 de                	add    ebx,esi
  40618c:	3b c3                	cmp    eax,ebx
  40618e:	73 05                	jae    0x406195
  406190:	e8 60 3f 03 00       	call   0x43a0f5
  406195:	8b cd                	mov    ecx,ebp
  406197:	e8 04 07 00 00       	call   0x4068a0
  40619c:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  40619f:	2b c3                	sub    eax,ebx
  4061a1:	3b c7                	cmp    eax,edi
  4061a3:	73 02                	jae    0x4061a7
  4061a5:	8b f8                	mov    edi,eax
  4061a7:	85 ff                	test   edi,edi
  4061a9:	76 36                	jbe    0x4061e1
  4061ab:	8b 4d 04             	mov    ecx,DWORD PTR [ebp+0x4]
  4061ae:	2b c7                	sub    eax,edi
  4061b0:	03 cb                	add    ecx,ebx
  4061b2:	50                   	push   eax
  4061b3:	8d 14 39             	lea    edx,[ecx+edi*1]
  4061b6:	52                   	push   edx
  4061b7:	51                   	push   ecx
  4061b8:	e8 43 a3 02 00       	call   0x430500
  4061bd:	8b 75 08             	mov    esi,DWORD PTR [ebp+0x8]
  4061c0:	83 c4 0c             	add    esp,0xc
  4061c3:	2b f7                	sub    esi,edi
  4061c5:	8b cd                	mov    ecx,ebp
  4061c7:	6a 00                	push   0x0
  4061c9:	56                   	push   esi
  4061ca:	e8 91 02 00 00       	call   0x406460
  4061cf:	84 c0                	test   al,al
  4061d1:	74 0a                	je     0x4061dd
  4061d3:	8b 45 04             	mov    eax,DWORD PTR [ebp+0x4]
  4061d6:	89 75 08             	mov    DWORD PTR [ebp+0x8],esi
  4061d9:	c6 04 06 00          	mov    BYTE PTR [esi+eax*1],0x0
  4061dd:	8b 74 24 18          	mov    esi,DWORD PTR [esp+0x18]
  4061e1:	8b cd                	mov    ecx,ebp
  4061e3:	8b fe                	mov    edi,esi
  4061e5:	e8 b6 06 00 00       	call   0x4068a0
  4061ea:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  4061ed:	3b c6                	cmp    eax,esi
  4061ef:	73 02                	jae    0x4061f3
  4061f1:	8b f8                	mov    edi,eax
  4061f3:	85 ff                	test   edi,edi
  4061f5:	0f 86 4c 01 00 00    	jbe    0x406347
  4061fb:	8b 4d 04             	mov    ecx,DWORD PTR [ebp+0x4]
  4061fe:	2b c7                	sub    eax,edi
  406200:	50                   	push   eax
  406201:	8d 14 39             	lea    edx,[ecx+edi*1]
  406204:	52                   	push   edx
  406205:	51                   	push   ecx
  406206:	e8 f5 a2 02 00       	call   0x430500
  40620b:	8b 75 08             	mov    esi,DWORD PTR [ebp+0x8]
  40620e:	83 c4 0c             	add    esp,0xc
  406211:	2b f7                	sub    esi,edi
  406213:	8b cd                	mov    ecx,ebp
  406215:	6a 00                	push   0x0
  406217:	56                   	push   esi
  406218:	e8 43 02 00 00       	call   0x406460
  40621d:	84 c0                	test   al,al
  40621f:	0f 84 22 01 00 00    	je     0x406347
  406225:	56                   	push   esi
  406226:	8b cd                	mov    ecx,ebp
  406228:	e8 13 02 00 00       	call   0x406440
  40622d:	5f                   	pop    edi
  40622e:	8b c5                	mov    eax,ebp
  406230:	5e                   	pop    esi
  406231:	5d                   	pop    ebp
  406232:	5b                   	pop    ebx
  406233:	c2 0c 00             	ret    0xc
  406236:	33 d2                	xor    edx,edx
  406238:	3b da                	cmp    ebx,edx
  40623a:	76 71                	jbe    0x4062ad
  40623c:	3b d8                	cmp    ebx,eax
  40623e:	75 6d                	jne    0x4062ad
  406240:	8b 47 04             	mov    eax,DWORD PTR [edi+0x4]
  406243:	3b c2                	cmp    eax,edx
  406245:	75 05                	jne    0x40624c
  406247:	b8 58 b3 43 00       	mov    eax,0x43b358
  40624c:	80 78 ff fe          	cmp    BYTE PTR [eax-0x1],0xfe
  406250:	73 5b                	jae    0x4062ad
  406252:	8b 45 04             	mov    eax,DWORD PTR [ebp+0x4]
  406255:	3b c2                	cmp    eax,edx
  406257:	74 1f                	je     0x406278
  406259:	8d 48 ff             	lea    ecx,[eax-0x1]
  40625c:	8a 40 ff             	mov    al,BYTE PTR [eax-0x1]
  40625f:	84 c0                	test   al,al
  406261:	74 0a                	je     0x40626d
  406263:	3c ff                	cmp    al,0xff
  406265:	74 06                	je     0x40626d
  406267:	fe c8                	dec    al
  406269:	88 01                	mov    BYTE PTR [ecx],al
  40626b:	eb 0b                	jmp    0x406278
  40626d:	51                   	push   ecx
  40626e:	e8 69 99 02 00       	call   0x42fbdc
  406273:	83 c4 04             	add    esp,0x4
  406276:	33 d2                	xor    edx,edx
  406278:	89 55 04             	mov    DWORD PTR [ebp+0x4],edx
  40627b:	89 55 08             	mov    DWORD PTR [ebp+0x8],edx
  40627e:	89 55 0c             	mov    DWORD PTR [ebp+0xc],edx
  406281:	8b 47 04             	mov    eax,DWORD PTR [edi+0x4]
  406284:	3b c2                	cmp    eax,edx
  406286:	75 05                	jne    0x40628d
  406288:	b8 58 b3 43 00       	mov    eax,0x43b358
  40628d:	89 45 04             	mov    DWORD PTR [ebp+0x4],eax
  406290:	8b 4f 08             	mov    ecx,DWORD PTR [edi+0x8]
  406293:	89 4d 08             	mov    DWORD PTR [ebp+0x8],ecx
  406296:	8b 57 0c             	mov    edx,DWORD PTR [edi+0xc]
  406299:	89 55 0c             	mov    DWORD PTR [ebp+0xc],edx
  40629c:	8a 48 ff             	mov    cl,BYTE PTR [eax-0x1]
  40629f:	fe c1                	inc    cl
  4062a1:	5f                   	pop    edi
  4062a2:	88 48 ff             	mov    BYTE PTR [eax-0x1],cl
  4062a5:	8b c5                	mov    eax,ebp
  4062a7:	5e                   	pop    esi
  4062a8:	5d                   	pop    ebp
  4062a9:	5b                   	pop    ebx
  4062aa:	c2 0c 00             	ret    0xc
  4062ad:	8b cd                	mov    ecx,ebp
  4062af:	e8 cc 04 00 00       	call   0x406780
  4062b4:	3b c3                	cmp    eax,ebx
  4062b6:	73 05                	jae    0x4062bd
  4062b8:	e8 0c 3c 03 00       	call   0x439ec9
  4062bd:	8b 4d 04             	mov    ecx,DWORD PTR [ebp+0x4]
  4062c0:	85 c9                	test   ecx,ecx
  4062c2:	74 25                	je     0x4062e9
  4062c4:	8a 41 ff             	mov    al,BYTE PTR [ecx-0x1]
  4062c7:	84 c0                	test   al,al
  4062c9:	74 1e                	je     0x4062e9
  4062cb:	3c ff                	cmp    al,0xff
  4062cd:	74 1a                	je     0x4062e9
  4062cf:	85 db                	test   ebx,ebx
  4062d1:	75 41                	jne    0x406314
  4062d3:	fe c8                	dec    al
  4062d5:	53                   	push   ebx
  4062d6:	88 41 ff             	mov    BYTE PTR [ecx-0x1],al
  4062d9:	8b cd                	mov    ecx,ebp
  4062db:	e8 70 00 00 00       	call   0x406350
  4062e0:	5f                   	pop    edi
  4062e1:	8b c5                	mov    eax,ebp
  4062e3:	5e                   	pop    esi
  4062e4:	5d                   	pop    ebp
  4062e5:	5b                   	pop    ebx
  4062e6:	c2 0c 00             	ret    0xc
  4062e9:	85 db                	test   ebx,ebx
  4062eb:	75 12                	jne    0x4062ff
  4062ed:	6a 01                	push   0x1
  4062ef:	8b cd                	mov    ecx,ebp
  4062f1:	e8 5a 00 00 00       	call   0x406350
  4062f6:	5f                   	pop    edi
  4062f7:	8b c5                	mov    eax,ebp
  4062f9:	5e                   	pop    esi
  4062fa:	5d                   	pop    ebp
  4062fb:	5b                   	pop    ebx
  4062fc:	c2 0c 00             	ret    0xc
  4062ff:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  406302:	83 f8 1f             	cmp    eax,0x1f
  406305:	77 04                	ja     0x40630b
  406307:	3b c3                	cmp    eax,ebx
  406309:	73 11                	jae    0x40631c
  40630b:	6a 01                	push   0x1
  40630d:	8b cd                	mov    ecx,ebp
  40630f:	e8 3c 00 00 00       	call   0x406350
  406314:	53                   	push   ebx
  406315:	8b cd                	mov    ecx,ebp
  406317:	e8 74 04 00 00       	call   0x406790
  40631c:	8b 47 04             	mov    eax,DWORD PTR [edi+0x4]
  40631f:	85 c0                	test   eax,eax
  406321:	75 05                	jne    0x406328
  406323:	b8 58 b3 43 00       	mov    eax,0x43b358
  406328:	8b 7d 04             	mov    edi,DWORD PTR [ebp+0x4]
  40632b:	8b cb                	mov    ecx,ebx
  40632d:	03 f0                	add    esi,eax
  40632f:	8b c1                	mov    eax,ecx
  406331:	c1 e9 02             	shr    ecx,0x2
  406334:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  406336:	8b c8                	mov    ecx,eax
  406338:	83 e1 03             	and    ecx,0x3
  40633b:	f3 a4                	rep movs BYTE PTR es:[edi],BYTE PTR ds:[esi]
  40633d:	8b 4d 04             	mov    ecx,DWORD PTR [ebp+0x4]
  406340:	89 5d 08             	mov    DWORD PTR [ebp+0x8],ebx
  406343:	c6 04 0b 00          	mov    BYTE PTR [ebx+ecx*1],0x0
  406347:	5f                   	pop    edi
  406348:	8b c5                	mov    eax,ebp
  40634a:	5e                   	pop    esi
  40634b:	5d                   	pop    ebp
  40634c:	5b                   	pop    ebx
  40634d:	c2 0c 00             	ret    0xc
