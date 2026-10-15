
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004232b0 <.text+0x222b0>:
  4232b0:	a1 f8 5e 44 00       	mov    eax,ds:0x445ef8
  4232b5:	56                   	push   esi
  4232b6:	85 c0                	test   eax,eax
  4232b8:	7c 53                	jl     0x42330d
  4232ba:	83 f8 03             	cmp    eax,0x3
  4232bd:	0f 8c a5 01 00 00    	jl     0x423468
  4232c3:	74 36                	je     0x4232fb
  4232c5:	83 3d fc 5e 44 00 04 	cmp    DWORD PTR ds:0x445efc,0x4
  4232cc:	74 2d                	je     0x4232fb
  4232ce:	83 f8 05             	cmp    eax,0x5
  4232d1:	0f 85 91 01 00 00    	jne    0x423468
  4232d7:	a1 18 0d 51 00       	mov    eax,ds:0x510d18
  4232dc:	8b 4c 24 08          	mov    ecx,DWORD PTR [esp+0x8]
  4232e0:	5e                   	pop    esi
  4232e1:	8d 04 40             	lea    eax,[eax+eax*2]
  4232e4:	8d 04 80             	lea    eax,[eax+eax*4]
  4232e7:	8d 14 41             	lea    edx,[ecx+eax*2]
  4232ea:	8b 44 24 08          	mov    eax,DWORD PTR [esp+0x8]
  4232ee:	c1 e2 05             	shl    edx,0x5
  4232f1:	db 82 1c 0e 51 00    	fild   DWORD PTR [edx+0x510e1c]
  4232f7:	d9 58 04             	fstp   DWORD PTR [eax+0x4]
  4232fa:	c3                   	ret
  4232fb:	8b 44 24 0c          	mov    eax,DWORD PTR [esp+0xc]
  4232ff:	5e                   	pop    esi
  423300:	d9 40 04             	fld    DWORD PTR [eax+0x4]
  423303:	d8 05 e0 b4 43 00    	fadd   DWORD PTR ds:0x43b4e0
  423309:	d9 58 04             	fstp   DWORD PTR [eax+0x4]
  42330c:	c3                   	ret
  42330d:	a1 fc 5e 44 00       	mov    eax,ds:0x445efc
  423312:	85 c0                	test   eax,eax
  423314:	7c 59                	jl     0x42336f
  423316:	83 f8 03             	cmp    eax,0x3
  423319:	0f 8c 49 01 00 00    	jl     0x423468
  42331f:	8b 4c 24 0c          	mov    ecx,DWORD PTR [esp+0xc]
  423323:	8b 51 14             	mov    edx,DWORD PTR [ecx+0x14]
  423326:	85 d2                	test   edx,edx
  423328:	0f 85 3a 01 00 00    	jne    0x423468
  42332e:	83 f8 03             	cmp    eax,0x3
  423331:	74 2e                	je     0x423361
  423333:	83 f8 04             	cmp    eax,0x4
  423336:	74 29                	je     0x423361
  423338:	83 f8 05             	cmp    eax,0x5
  42333b:	0f 85 27 01 00 00    	jne    0x423468
  423341:	a1 18 0d 51 00       	mov    eax,ds:0x510d18
  423346:	5e                   	pop    esi
  423347:	8d 04 40             	lea    eax,[eax+eax*2]
  42334a:	8d 14 80             	lea    edx,[eax+eax*4]
  42334d:	8b 44 24 04          	mov    eax,DWORD PTR [esp+0x4]
  423351:	8d 14 50             	lea    edx,[eax+edx*2]
  423354:	c1 e2 05             	shl    edx,0x5
  423357:	db 82 1c 0e 51 00    	fild   DWORD PTR [edx+0x510e1c]
  42335d:	d9 59 04             	fstp   DWORD PTR [ecx+0x4]
  423360:	c3                   	ret
  423361:	d9 41 04             	fld    DWORD PTR [ecx+0x4]
  423364:	d8 25 e0 b4 43 00    	fsub   DWORD PTR ds:0x43b4e0
  42336a:	5e                   	pop    esi
  42336b:	d9 59 04             	fstp   DWORD PTR [ecx+0x4]
  42336e:	c3                   	ret
  42336f:	a1 d4 44 51 00       	mov    eax,ds:0x5144d4
  423374:	85 c0                	test   eax,eax
  423376:	0f 84 ec 00 00 00    	je     0x423468
  42337c:	8b 74 24 0c          	mov    esi,DWORD PTR [esp+0xc]
  423380:	8b 46 1c             	mov    eax,DWORD PTR [esi+0x1c]
  423383:	83 f8 0b             	cmp    eax,0xb
  423386:	0f 8c dc 00 00 00    	jl     0x423468
  42338c:	83 f8 0d             	cmp    eax,0xd
  42338f:	0f 8f d3 00 00 00    	jg     0x423468
  423395:	a1 18 0d 51 00       	mov    eax,ds:0x510d18
  42339a:	8b 4c 24 08          	mov    ecx,DWORD PTR [esp+0x8]
  42339e:	d9 46 04             	fld    DWORD PTR [esi+0x4]
  4233a1:	8d 04 40             	lea    eax,[eax+eax*2]
  4233a4:	53                   	push   ebx
  4233a5:	57                   	push   edi
  4233a6:	8d 04 80             	lea    eax,[eax+eax*4]
  4233a9:	8d 04 41             	lea    eax,[ecx+eax*2]
  4233ac:	c1 e0 05             	shl    eax,0x5
  4233af:	8b 98 1c 0e 51 00    	mov    ebx,DWORD PTR [eax+0x510e1c]
  4233b5:	8b b8 18 0e 51 00    	mov    edi,DWORD PTR [eax+0x510e18]
  4233bb:	53                   	push   ebx
  4233bc:	89 7c 24 14          	mov    DWORD PTR [esp+0x14],edi
  4233c0:	57                   	push   edi
  4233c1:	e8 0a d1 00 00       	call   0x4304d0
  4233c6:	d9 06                	fld    DWORD PTR [esi]
  4233c8:	50                   	push   eax
  4233c9:	e8 02 d1 00 00       	call   0x4304d0
  4233ce:	50                   	push   eax
  4233cf:	e8 5c 29 ff ff       	call   0x415d30
  4233d4:	dd d8                	fstp   st(0)
  4233d6:	d9 46 04             	fld    DWORD PTR [esi+0x4]
  4233d9:	53                   	push   ebx
  4233da:	57                   	push   edi
  4233db:	e8 f0 d0 00 00       	call   0x4304d0
  4233e0:	d9 06                	fld    DWORD PTR [esi]
  4233e2:	50                   	push   eax
  4233e3:	e8 e8 d0 00 00       	call   0x4304d0
  4233e8:	50                   	push   eax
  4233e9:	e8 82 29 ff ff       	call   0x415d70
  4233ee:	89 44 24 34          	mov    DWORD PTR [esp+0x34],eax
  4233f2:	8b 4e 14             	mov    ecx,DWORD PTR [esi+0x14]
  4233f5:	db 44 24 34          	fild   DWORD PTR [esp+0x34]
  4233f9:	83 c4 20             	add    esp,0x20
  4233fc:	83 f9 01             	cmp    ecx,0x1
  4233ff:	d8 0d 84 b3 43 00    	fmul   DWORD PTR ds:0x43b384
  423405:	db 44 24 18          	fild   DWORD PTR [esp+0x18]
  423409:	5f                   	pop    edi
  42340a:	5b                   	pop    ebx
  42340b:	d9 5c 24 0c          	fstp   DWORD PTR [esp+0xc]
  42340f:	d9 c0                	fld    st(0)
  423411:	d9 ff                	fcos
  423413:	d8 4c 24 0c          	fmul   DWORD PTR [esp+0xc]
  423417:	d8 0d 78 b3 43 00    	fmul   DWORD PTR ds:0x43b378
  42341d:	d8 6e 04             	fsubr  DWORD PTR [esi+0x4]
  423420:	d9 5e 04             	fstp   DWORD PTR [esi+0x4]
  423423:	d9 fe                	fsin
  423425:	d8 4c 24 0c          	fmul   DWORD PTR [esp+0xc]
  423429:	d8 0d 78 b3 43 00    	fmul   DWORD PTR ds:0x43b378
  42342f:	d8 06                	fadd   DWORD PTR [esi]
  423431:	d9 54 24 0c          	fst    DWORD PTR [esp+0xc]
  423435:	d9 1e                	fstp   DWORD PTR [esi]
  423437:	75 17                	jne    0x423450
  423439:	db 44 24 08          	fild   DWORD PTR [esp+0x8]
  42343d:	d9 44 24 0c          	fld    DWORD PTR [esp+0xc]
  423441:	d8 d9                	fcomp  st(1)
  423443:	df e0                	fnstsw ax
  423445:	f6 c4 41             	test   ah,0x41
  423448:	75 04                	jne    0x42344e
  42344a:	d9 1e                	fstp   DWORD PTR [esi]
  42344c:	eb 02                	jmp    0x423450
  42344e:	dd d8                	fstp   st(0)
  423450:	83 f9 03             	cmp    ecx,0x3
  423453:	75 13                	jne    0x423468
  423455:	db 44 24 08          	fild   DWORD PTR [esp+0x8]
  423459:	d8 16                	fcom   DWORD PTR [esi]
  42345b:	df e0                	fnstsw ax
  42345d:	f6 c4 41             	test   ah,0x41
  423460:	75 04                	jne    0x423466
  423462:	d9 1e                	fstp   DWORD PTR [esi]
  423464:	5e                   	pop    esi
  423465:	c3                   	ret
  423466:	dd d8                	fstp   st(0)
  423468:	5e                   	pop    esi
  423469:	c3                   	ret
