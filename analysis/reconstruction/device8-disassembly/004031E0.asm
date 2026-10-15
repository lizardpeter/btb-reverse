
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004031e0 <.text+0x21e0>:
  4031e0:	81 ec a0 00 00 00    	sub    esp,0xa0
  4031e6:	53                   	push   ebx
  4031e7:	8b d9                	mov    ebx,ecx
  4031e9:	55                   	push   ebp
  4031ea:	56                   	push   esi
  4031eb:	8b 03                	mov    eax,DWORD PTR [ebx]
  4031ed:	57                   	push   edi
  4031ee:	ff 10                	call   DWORD PTR [eax]
  4031f0:	8d 6b 04             	lea    ebp,[ebx+0x4]
  4031f3:	6a 00                	push   0x0
  4031f5:	68 68 b5 43 00       	push   0x43b568
  4031fa:	55                   	push   ebp
  4031fb:	6a 00                	push   0x0
  4031fd:	e8 ce b8 02 00       	call   0x42ead0
  403202:	85 c0                	test   eax,eax
  403204:	7d 12                	jge    0x403218
  403206:	5f                   	pop    edi
  403207:	5e                   	pop    esi
  403208:	5d                   	pop    ebp
  403209:	b8 05 40 00 80       	mov    eax,0x80004005
  40320e:	5b                   	pop    ebx
  40320f:	81 c4 a0 00 00 00    	add    esp,0xa0
  403215:	c2 0c 00             	ret    0xc
  403218:	8b 45 00             	mov    eax,DWORD PTR [ebp+0x0]
  40321b:	8b b4 24 b4 00 00 00 	mov    esi,DWORD PTR [esp+0xb4]
  403222:	6a 08                	push   0x8
  403224:	56                   	push   esi
  403225:	8b 08                	mov    ecx,DWORD PTR [eax]
  403227:	50                   	push   eax
  403228:	ff 51 50             	call   DWORD PTR [ecx+0x50]
  40322b:	85 c0                	test   eax,eax
  40322d:	7d 12                	jge    0x403241
  40322f:	5f                   	pop    edi
  403230:	5e                   	pop    esi
  403231:	5d                   	pop    ebp
  403232:	b8 05 40 00 80       	mov    eax,0x80004005
  403237:	5b                   	pop    ebx
  403238:	81 c4 a0 00 00 00    	add    esp,0xa0
  40323e:	c2 0c 00             	ret    0xc
  403241:	8b 3d f8 b1 43 00    	mov    edi,DWORD PTR ds:0x43b1f8
  403247:	6a f0                	push   0xfffffff0
  403249:	56                   	push   esi
  40324a:	ff d7                	call   edi
  40324c:	25 ff ff 39 7f       	and    eax,0x7f39ffff
  403251:	0d 00 00 c6 00       	or     eax,0xc60000
  403256:	50                   	push   eax
  403257:	6a f0                	push   0xfffffff0
  403259:	56                   	push   esi
  40325a:	ff 15 f4 b1 43 00    	call   DWORD PTR ds:0x43b1f4
  403260:	8b 94 24 bc 00 00 00 	mov    edx,DWORD PTR [esp+0xbc]
  403267:	8b 84 24 b8 00 00 00 	mov    eax,DWORD PTR [esp+0xb8]
  40326e:	52                   	push   edx
  40326f:	50                   	push   eax
  403270:	6a 00                	push   0x0
  403272:	8d 4c 24 20          	lea    ecx,[esp+0x20]
  403276:	6a 00                	push   0x0
  403278:	51                   	push   ecx
  403279:	ff 15 18 b2 43 00    	call   DWORD PTR ds:0x43b218
  40327f:	6a ec                	push   0xffffffec
  403281:	56                   	push   esi
  403282:	ff d7                	call   edi
  403284:	50                   	push   eax
  403285:	56                   	push   esi
  403286:	ff 15 1c b2 43 00    	call   DWORD PTR ds:0x43b21c
  40328c:	f7 d8                	neg    eax
  40328e:	1b c0                	sbb    eax,eax
  403290:	f7 d8                	neg    eax
  403292:	50                   	push   eax
  403293:	6a f0                	push   0xfffffff0
  403295:	56                   	push   esi
  403296:	ff d7                	call   edi
  403298:	8d 54 24 1c          	lea    edx,[esp+0x1c]
  40329c:	50                   	push   eax
  40329d:	52                   	push   edx
  40329e:	ff 15 20 b2 43 00    	call   DWORD PTR ds:0x43b220
  4032a4:	8b 44 24 20          	mov    eax,DWORD PTR [esp+0x20]
  4032a8:	8b 54 24 18          	mov    edx,DWORD PTR [esp+0x18]
  4032ac:	8b 4c 24 1c          	mov    ecx,DWORD PTR [esp+0x1c]
  4032b0:	8b 7c 24 14          	mov    edi,DWORD PTR [esp+0x14]
  4032b4:	2b c2                	sub    eax,edx
  4032b6:	6a 16                	push   0x16
  4032b8:	2b cf                	sub    ecx,edi
  4032ba:	8b 3d 24 b2 43 00    	mov    edi,DWORD PTR ds:0x43b224
  4032c0:	50                   	push   eax
  4032c1:	51                   	push   ecx
  4032c2:	6a 00                	push   0x0
  4032c4:	6a 00                	push   0x0
  4032c6:	6a 00                	push   0x0
  4032c8:	56                   	push   esi
  4032c9:	ff d7                	call   edi
  4032cb:	6a 13                	push   0x13
  4032cd:	6a 00                	push   0x0
  4032cf:	6a 00                	push   0x0
  4032d1:	6a 00                	push   0x0
  4032d3:	6a 00                	push   0x0
  4032d5:	6a fe                	push   0xfffffffe
  4032d7:	56                   	push   esi
  4032d8:	ff d7                	call   edi
  4032da:	8d 54 24 24          	lea    edx,[esp+0x24]
  4032de:	6a 00                	push   0x0
  4032e0:	52                   	push   edx
  4032e1:	6a 00                	push   0x0
  4032e3:	6a 30                	push   0x30
  4032e5:	ff 15 28 b2 43 00    	call   DWORD PTR ds:0x43b228
  4032eb:	8d 44 24 14          	lea    eax,[esp+0x14]
  4032ef:	50                   	push   eax
  4032f0:	56                   	push   esi
  4032f1:	ff 15 b4 b1 43 00    	call   DWORD PTR ds:0x43b1b4
  4032f7:	8b 44 24 14          	mov    eax,DWORD PTR [esp+0x14]
  4032fb:	8b 4c 24 24          	mov    ecx,DWORD PTR [esp+0x24]
  4032ff:	3b c1                	cmp    eax,ecx
  403301:	7d 06                	jge    0x403309
  403303:	8b c1                	mov    eax,ecx
  403305:	89 44 24 14          	mov    DWORD PTR [esp+0x14],eax
  403309:	8b 4c 24 18          	mov    ecx,DWORD PTR [esp+0x18]
  40330d:	8b 54 24 28          	mov    edx,DWORD PTR [esp+0x28]
  403311:	3b ca                	cmp    ecx,edx
  403313:	7d 06                	jge    0x40331b
  403315:	8b ca                	mov    ecx,edx
  403317:	89 4c 24 18          	mov    DWORD PTR [esp+0x18],ecx
  40331b:	6a 15                	push   0x15
  40331d:	6a 00                	push   0x0
  40331f:	6a 00                	push   0x0
  403321:	51                   	push   ecx
  403322:	50                   	push   eax
  403323:	6a 00                	push   0x0
  403325:	56                   	push   esi
  403326:	ff d7                	call   edi
  403328:	b9 1f 00 00 00       	mov    ecx,0x1f
  40332d:	33 c0                	xor    eax,eax
  40332f:	8d 7c 24 34          	lea    edi,[esp+0x34]
  403333:	6a 00                	push   0x0
  403335:	f3 ab                	rep stos DWORD PTR es:[edi],eax
  403337:	8b 45 00             	mov    eax,DWORD PTR [ebp+0x0]
  40333a:	8d 7b 08             	lea    edi,[ebx+0x8]
  40333d:	8d 54 24 38          	lea    edx,[esp+0x38]
  403341:	c7 44 24 38 7c 00 00 	mov    DWORD PTR [esp+0x38],0x7c
  403348:	00 
  403349:	c7 44 24 3c 01 00 00 	mov    DWORD PTR [esp+0x3c],0x1
  403350:	00 
  403351:	c7 84 24 a0 00 00 00 	mov    DWORD PTR [esp+0xa0],0x200
  403358:	00 02 00 00 
  40335c:	8b 08                	mov    ecx,DWORD PTR [eax]
  40335e:	57                   	push   edi
  40335f:	52                   	push   edx
  403360:	50                   	push   eax
  403361:	ff 51 18             	call   DWORD PTR [ecx+0x18]
  403364:	85 c0                	test   eax,eax
  403366:	7d 12                	jge    0x40337a
  403368:	5f                   	pop    edi
  403369:	5e                   	pop    esi
  40336a:	5d                   	pop    ebp
  40336b:	b8 05 40 00 80       	mov    eax,0x80004005
  403370:	5b                   	pop    ebx
  403371:	81 c4 a0 00 00 00    	add    esp,0xa0
  403377:	c2 0c 00             	ret    0xc
  40337a:	8b 8c 24 bc 00 00 00 	mov    ecx,DWORD PTR [esp+0xbc]
  403381:	8b 84 24 b8 00 00 00 	mov    eax,DWORD PTR [esp+0xb8]
  403388:	89 4c 24 3c          	mov    DWORD PTR [esp+0x3c],ecx
  40338c:	8d 4b 0c             	lea    ecx,[ebx+0xc]
  40338f:	89 44 24 40          	mov    DWORD PTR [esp+0x40],eax
  403393:	8b 45 00             	mov    eax,DWORD PTR [ebp+0x0]
  403396:	6a 00                	push   0x0
  403398:	51                   	push   ecx
  403399:	8d 4c 24 3c          	lea    ecx,[esp+0x3c]
  40339d:	c7 44 24 40 07 00 00 	mov    DWORD PTR [esp+0x40],0x7
  4033a4:	00 
  4033a5:	c7 84 24 a4 00 00 00 	mov    DWORD PTR [esp+0xa4],0x2040
  4033ac:	40 20 00 00 
  4033b0:	8b 10                	mov    edx,DWORD PTR [eax]
  4033b2:	51                   	push   ecx
  4033b3:	50                   	push   eax
  4033b4:	ff 52 18             	call   DWORD PTR [edx+0x18]
  4033b7:	85 c0                	test   eax,eax
  4033b9:	7d 12                	jge    0x4033cd
  4033bb:	5f                   	pop    edi
  4033bc:	5e                   	pop    esi
  4033bd:	5d                   	pop    ebp
  4033be:	b8 05 40 00 80       	mov    eax,0x80004005
  4033c3:	5b                   	pop    ebx
  4033c4:	81 c4 a0 00 00 00    	add    esp,0xa0
  4033ca:	c2 0c 00             	ret    0xc
  4033cd:	8b 6d 00             	mov    ebp,DWORD PTR [ebp+0x0]
  4033d0:	8d 44 24 10          	lea    eax,[esp+0x10]
  4033d4:	6a 00                	push   0x0
  4033d6:	50                   	push   eax
  4033d7:	8b 55 00             	mov    edx,DWORD PTR [ebp+0x0]
  4033da:	6a 00                	push   0x0
  4033dc:	55                   	push   ebp
  4033dd:	ff 52 10             	call   DWORD PTR [edx+0x10]
  4033e0:	85 c0                	test   eax,eax
  4033e2:	7d 12                	jge    0x4033f6
  4033e4:	5f                   	pop    edi
  4033e5:	5e                   	pop    esi
  4033e6:	5d                   	pop    ebp
  4033e7:	b8 05 40 00 80       	mov    eax,0x80004005
  4033ec:	5b                   	pop    ebx
  4033ed:	81 c4 a0 00 00 00    	add    esp,0xa0
  4033f3:	c2 0c 00             	ret    0xc
  4033f6:	8b 44 24 10          	mov    eax,DWORD PTR [esp+0x10]
  4033fa:	56                   	push   esi
  4033fb:	6a 00                	push   0x0
  4033fd:	50                   	push   eax
  4033fe:	8b 08                	mov    ecx,DWORD PTR [eax]
  403400:	ff 51 20             	call   DWORD PTR [ecx+0x20]
  403403:	85 c0                	test   eax,eax
  403405:	7d 1c                	jge    0x403423
  403407:	8b 44 24 10          	mov    eax,DWORD PTR [esp+0x10]
  40340b:	50                   	push   eax
  40340c:	8b 10                	mov    edx,DWORD PTR [eax]
  40340e:	ff 52 08             	call   DWORD PTR [edx+0x8]
  403411:	5f                   	pop    edi
  403412:	5e                   	pop    esi
  403413:	5d                   	pop    ebp
  403414:	b8 05 40 00 80       	mov    eax,0x80004005
  403419:	5b                   	pop    ebx
  40341a:	81 c4 a0 00 00 00    	add    esp,0xa0
  403420:	c2 0c 00             	ret    0xc
  403423:	8b 3f                	mov    edi,DWORD PTR [edi]
  403425:	8b 4c 24 10          	mov    ecx,DWORD PTR [esp+0x10]
  403429:	51                   	push   ecx
  40342a:	57                   	push   edi
  40342b:	8b 07                	mov    eax,DWORD PTR [edi]
  40342d:	ff 50 70             	call   DWORD PTR [eax+0x70]
  403430:	85 c0                	test   eax,eax
  403432:	8b 44 24 10          	mov    eax,DWORD PTR [esp+0x10]
  403436:	7d 18                	jge    0x403450
  403438:	8b 10                	mov    edx,DWORD PTR [eax]
  40343a:	50                   	push   eax
  40343b:	ff 52 08             	call   DWORD PTR [edx+0x8]
  40343e:	5f                   	pop    edi
  40343f:	5e                   	pop    esi
  403440:	5d                   	pop    ebp
  403441:	b8 05 40 00 80       	mov    eax,0x80004005
  403446:	5b                   	pop    ebx
  403447:	81 c4 a0 00 00 00    	add    esp,0xa0
  40344d:	c2 0c 00             	ret    0xc
  403450:	8b 08                	mov    ecx,DWORD PTR [eax]
  403452:	50                   	push   eax
  403453:	ff 51 08             	call   DWORD PTR [ecx+0x8]
  403456:	8b cb                	mov    ecx,ebx
  403458:	89 73 14             	mov    DWORD PTR [ebx+0x14],esi
  40345b:	c7 43 28 01 00 00 00 	mov    DWORD PTR [ebx+0x28],0x1
  403462:	e8 29 01 00 00       	call   0x403590
  403467:	5f                   	pop    edi
  403468:	5e                   	pop    esi
  403469:	5d                   	pop    ebp
  40346a:	33 c0                	xor    eax,eax
  40346c:	5b                   	pop    ebx
  40346d:	81 c4 a0 00 00 00    	add    esp,0xa0
  403473:	c2 0c 00             	ret    0xc
