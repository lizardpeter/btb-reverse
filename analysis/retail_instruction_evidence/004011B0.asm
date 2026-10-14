
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004011b0 <.text+0x1b0>:
  4011b0:	81 ec 28 02 00 00    	sub    esp,0x228
  4011b6:	8b 15 08 de 44 00    	mov    edx,DWORD PTR ds:0x44de08
  4011bc:	a1 98 c2 51 00       	mov    eax,ds:0x51c298
  4011c1:	56                   	push   esi
  4011c2:	33 f6                	xor    esi,esi
  4011c4:	8b 52 0c             	mov    edx,DWORD PTR [edx+0xc]
  4011c7:	8b 08                	mov    ecx,DWORD PTR [eax]
  4011c9:	57                   	push   edi
  4011ca:	56                   	push   esi
  4011cb:	56                   	push   esi
  4011cc:	52                   	push   edx
  4011cd:	56                   	push   esi
  4011ce:	56                   	push   esi
  4011cf:	50                   	push   eax
  4011d0:	ff 51 1c             	call   DWORD PTR [ecx+0x1c]
  4011d3:	bf 01 00 00 00       	mov    edi,0x1
  4011d8:	89 35 a0 a2 44 00    	mov    DWORD PTR ds:0x44a2a0,esi
  4011de:	57                   	push   edi
  4011df:	68 dc 01 00 00       	push   0x1dc
  4011e4:	89 3d a0 dd 44 00    	mov    DWORD PTR ds:0x44dda0,edi
  4011ea:	e8 11 fe ff ff       	call   0x401000
  4011ef:	89 7c 24 18          	mov    DWORD PTR [esp+0x18],edi
  4011f3:	b9 02 00 00 00       	mov    ecx,0x2
  4011f8:	bf 11 00 00 00       	mov    edi,0x11
  4011fd:	83 c4 08             	add    esp,0x8
  401200:	89 4c 24 18          	mov    DWORD PTR [esp+0x18],ecx
  401204:	89 4c 24 34          	mov    DWORD PTR [esp+0x34],ecx
  401208:	89 bc 24 84 00 00 00 	mov    DWORD PTR [esp+0x84],edi
  40120f:	89 bc 24 90 00 00 00 	mov    DWORD PTR [esp+0x90],edi
  401216:	83 c8 ff             	or     eax,0xffffffff
  401219:	ba 07 00 00 00       	mov    edx,0x7
  40121e:	b9 08 00 00 00       	mov    ecx,0x8
  401223:	bf 12 00 00 00       	mov    edi,0x12
  401228:	89 35 a8 dd 44 00    	mov    DWORD PTR ds:0x44dda8,esi
  40122e:	89 74 24 08          	mov    DWORD PTR [esp+0x8],esi
  401232:	89 44 24 0c          	mov    DWORD PTR [esp+0xc],eax
  401236:	c7 44 24 14 9d ff ff 	mov    DWORD PTR [esp+0x14],0xffffff9d
  40123d:	ff 
  40123e:	89 44 24 1c          	mov    DWORD PTR [esp+0x1c],eax
  401242:	c7 44 24 20 03 00 00 	mov    DWORD PTR [esp+0x20],0x3
  401249:	00 
  40124a:	89 44 24 24          	mov    DWORD PTR [esp+0x24],eax
  40124e:	c7 44 24 28 04 00 00 	mov    DWORD PTR [esp+0x28],0x4
  401255:	00 
  401256:	89 44 24 2c          	mov    DWORD PTR [esp+0x2c],eax
  40125a:	c7 44 24 30 05 00 00 	mov    DWORD PTR [esp+0x30],0x5
  401261:	00 
  401262:	c7 44 24 38 06 00 00 	mov    DWORD PTR [esp+0x38],0x6
  401269:	00 
  40126a:	89 44 24 3c          	mov    DWORD PTR [esp+0x3c],eax
  40126e:	89 54 24 40          	mov    DWORD PTR [esp+0x40],edx
  401272:	89 44 24 44          	mov    DWORD PTR [esp+0x44],eax
  401276:	89 4c 24 48          	mov    DWORD PTR [esp+0x48],ecx
  40127a:	89 44 24 4c          	mov    DWORD PTR [esp+0x4c],eax
  40127e:	c7 44 24 50 09 00 00 	mov    DWORD PTR [esp+0x50],0x9
  401285:	00 
  401286:	89 44 24 54          	mov    DWORD PTR [esp+0x54],eax
  40128a:	c7 44 24 58 0a 00 00 	mov    DWORD PTR [esp+0x58],0xa
  401291:	00 
  401292:	89 44 24 5c          	mov    DWORD PTR [esp+0x5c],eax
  401296:	c7 44 24 60 0b 00 00 	mov    DWORD PTR [esp+0x60],0xb
  40129d:	00 
  40129e:	89 44 24 64          	mov    DWORD PTR [esp+0x64],eax
  4012a2:	c7 44 24 68 0c 00 00 	mov    DWORD PTR [esp+0x68],0xc
  4012a9:	00 
  4012aa:	89 44 24 6c          	mov    DWORD PTR [esp+0x6c],eax
  4012ae:	c7 44 24 70 0d 00 00 	mov    DWORD PTR [esp+0x70],0xd
  4012b5:	00 
  4012b6:	89 4c 24 74          	mov    DWORD PTR [esp+0x74],ecx
  4012ba:	c7 44 24 78 0e 00 00 	mov    DWORD PTR [esp+0x78],0xe
  4012c1:	00 
  4012c2:	89 44 24 7c          	mov    DWORD PTR [esp+0x7c],eax
  4012c6:	c7 84 24 80 00 00 00 	mov    DWORD PTR [esp+0x80],0xf
  4012cd:	0f 00 00 00 
  4012d1:	c7 84 24 88 00 00 00 	mov    DWORD PTR [esp+0x88],0x10
  4012d8:	10 00 00 00 
  4012dc:	89 84 24 8c 00 00 00 	mov    DWORD PTR [esp+0x8c],eax
  4012e3:	c7 84 24 94 00 00 00 	mov    DWORD PTR [esp+0x94],0x4
  4012ea:	04 00 00 00 
  4012ee:	89 bc 24 98 00 00 00 	mov    DWORD PTR [esp+0x98],edi
  4012f5:	89 84 24 9c 00 00 00 	mov    DWORD PTR [esp+0x9c],eax
  4012fc:	c7 84 24 a0 00 00 00 	mov    DWORD PTR [esp+0xa0],0x13
  401303:	13 00 00 00 
  401307:	89 8c 24 a4 00 00 00 	mov    DWORD PTR [esp+0xa4],ecx
  40130e:	c7 84 24 a8 00 00 00 	mov    DWORD PTR [esp+0xa8],0x14
  401315:	14 00 00 00 
  401319:	89 84 24 ac 00 00 00 	mov    DWORD PTR [esp+0xac],eax
  401320:	c7 84 24 b0 00 00 00 	mov    DWORD PTR [esp+0xb0],0x15
  401327:	15 00 00 00 
  40132b:	c7 84 24 b4 00 00 00 	mov    DWORD PTR [esp+0xb4],0xc
  401332:	0c 00 00 00 
  401336:	c7 84 24 b8 00 00 00 	mov    DWORD PTR [esp+0xb8],0x16
  40133d:	16 00 00 00 
  401341:	89 84 24 bc 00 00 00 	mov    DWORD PTR [esp+0xbc],eax
  401348:	c7 84 24 c0 00 00 00 	mov    DWORD PTR [esp+0xc0],0x17
  40134f:	17 00 00 00 
  401353:	c7 84 24 c4 00 00 00 	mov    DWORD PTR [esp+0xc4],0x3
  40135a:	03 00 00 00 
  40135e:	c7 84 24 c8 00 00 00 	mov    DWORD PTR [esp+0xc8],0x18
  401365:	18 00 00 00 
  401369:	89 84 24 cc 00 00 00 	mov    DWORD PTR [esp+0xcc],eax
  401370:	c7 84 24 d0 00 00 00 	mov    DWORD PTR [esp+0xd0],0x19
  401377:	19 00 00 00 
  40137b:	89 94 24 d4 00 00 00 	mov    DWORD PTR [esp+0xd4],edx
  401382:	c7 84 24 d8 00 00 00 	mov    DWORD PTR [esp+0xd8],0x1a
  401389:	1a 00 00 00 
  40138d:	89 84 24 dc 00 00 00 	mov    DWORD PTR [esp+0xdc],eax
  401394:	c7 84 24 e0 00 00 00 	mov    DWORD PTR [esp+0xe0],0x1b
  40139b:	1b 00 00 00 
  40139f:	c7 84 24 e4 00 00 00 	mov    DWORD PTR [esp+0xe4],0xb
  4013a6:	0b 00 00 00 
  4013aa:	c7 84 24 e8 00 00 00 	mov    DWORD PTR [esp+0xe8],0x1c
  4013b1:	1c 00 00 00 
  4013b5:	89 84 24 ec 00 00 00 	mov    DWORD PTR [esp+0xec],eax
  4013bc:	c7 84 24 f0 00 00 00 	mov    DWORD PTR [esp+0xf0],0x1d
  4013c3:	1d 00 00 00 
  4013c7:	c7 84 24 f4 00 00 00 	mov    DWORD PTR [esp+0xf4],0x5
  4013ce:	05 00 00 00 
  4013d2:	c7 84 24 f8 00 00 00 	mov    DWORD PTR [esp+0xf8],0x1e
  4013d9:	1e 00 00 00 
  4013dd:	89 84 24 fc 00 00 00 	mov    DWORD PTR [esp+0xfc],eax
  4013e4:	c7 84 24 00 01 00 00 	mov    DWORD PTR [esp+0x100],0x1f
  4013eb:	1f 00 00 00 
  4013ef:	89 8c 24 04 01 00 00 	mov    DWORD PTR [esp+0x104],ecx
  4013f6:	c7 84 24 08 01 00 00 	mov    DWORD PTR [esp+0x108],0x20
  4013fd:	20 00 00 00 
  401401:	89 84 24 0c 01 00 00 	mov    DWORD PTR [esp+0x10c],eax
  401408:	c7 84 24 10 01 00 00 	mov    DWORD PTR [esp+0x110],0x21
  40140f:	21 00 00 00 
  401413:	c7 84 24 14 01 00 00 	mov    DWORD PTR [esp+0x114],0xe
  40141a:	0e 00 00 00 
  40141e:	c7 84 24 18 01 00 00 	mov    DWORD PTR [esp+0x118],0x22
  401425:	22 00 00 00 
  401429:	89 84 24 1c 01 00 00 	mov    DWORD PTR [esp+0x11c],eax
  401430:	c7 84 24 20 01 00 00 	mov    DWORD PTR [esp+0x120],0x23
  401437:	23 00 00 00 
  40143b:	89 94 24 24 01 00 00 	mov    DWORD PTR [esp+0x124],edx
  401442:	c7 84 24 28 01 00 00 	mov    DWORD PTR [esp+0x128],0x24
  401449:	24 00 00 00 
  40144d:	89 84 24 2c 01 00 00 	mov    DWORD PTR [esp+0x12c],eax
  401454:	c7 84 24 30 01 00 00 	mov    DWORD PTR [esp+0x130],0x25
  40145b:	25 00 00 00 
  40145f:	89 bc 24 34 01 00 00 	mov    DWORD PTR [esp+0x134],edi
  401466:	c7 84 24 38 01 00 00 	mov    DWORD PTR [esp+0x138],0x26
  40146d:	26 00 00 00 
  401471:	89 84 24 3c 01 00 00 	mov    DWORD PTR [esp+0x13c],eax
  401478:	c7 84 24 40 01 00 00 	mov    DWORD PTR [esp+0x140],0x27
  40147f:	27 00 00 00 
  401483:	89 8c 24 44 01 00 00 	mov    DWORD PTR [esp+0x144],ecx
  40148a:	c7 84 24 48 01 00 00 	mov    DWORD PTR [esp+0x148],0x28
  401491:	28 00 00 00 
  401495:	89 84 24 4c 01 00 00 	mov    DWORD PTR [esp+0x14c],eax
  40149c:	c7 84 24 50 01 00 00 	mov    DWORD PTR [esp+0x150],0x29
  4014a3:	29 00 00 00 
  4014a7:	c7 84 24 54 01 00 00 	mov    DWORD PTR [esp+0x154],0xd
  4014ae:	0d 00 00 00 
  4014b2:	c7 84 24 58 01 00 00 	mov    DWORD PTR [esp+0x158],0x2a
  4014b9:	2a 00 00 00 
  4014bd:	89 84 24 5c 01 00 00 	mov    DWORD PTR [esp+0x15c],eax
  4014c4:	c7 84 24 60 01 00 00 	mov    DWORD PTR [esp+0x160],0x2b
  4014cb:	2b 00 00 00 
  4014cf:	c7 84 24 64 01 00 00 	mov    DWORD PTR [esp+0x164],0x6
  4014d6:	06 00 00 00 
  4014da:	c7 84 24 68 01 00 00 	mov    DWORD PTR [esp+0x168],0x2c
  4014e1:	2c 00 00 00 
  4014e5:	89 84 24 6c 01 00 00 	mov    DWORD PTR [esp+0x16c],eax
  4014ec:	c7 84 24 70 01 00 00 	mov    DWORD PTR [esp+0x170],0x2d
  4014f3:	2d 00 00 00 
  4014f7:	89 94 24 74 01 00 00 	mov    DWORD PTR [esp+0x174],edx
  4014fe:	c7 84 24 78 01 00 00 	mov    DWORD PTR [esp+0x178],0x2e
  401505:	2e 00 00 00 
  401509:	89 84 24 7c 01 00 00 	mov    DWORD PTR [esp+0x17c],eax
  401510:	c7 84 24 80 01 00 00 	mov    DWORD PTR [esp+0x180],0x2f
  401517:	2f 00 00 00 
  40151b:	c7 84 24 84 01 00 00 	mov    DWORD PTR [esp+0x184],0x10
  401522:	10 00 00 00 
  401526:	c7 84 24 88 01 00 00 	mov    DWORD PTR [esp+0x188],0x30
  40152d:	30 00 00 00 
  401531:	89 84 24 8c 01 00 00 	mov    DWORD PTR [esp+0x18c],eax
  401538:	c7 84 24 90 01 00 00 	mov    DWORD PTR [esp+0x190],0x31
  40153f:	31 00 00 00 
  401543:	89 94 24 94 01 00 00 	mov    DWORD PTR [esp+0x194],edx
  40154a:	c7 84 24 98 01 00 00 	mov    DWORD PTR [esp+0x198],0x32
  401551:	32 00 00 00 
  401555:	89 84 24 9c 01 00 00 	mov    DWORD PTR [esp+0x19c],eax
  40155c:	c7 84 24 a0 01 00 00 	mov    DWORD PTR [esp+0x1a0],0x33
  401563:	33 00 00 00 
  401567:	c7 84 24 a4 01 00 00 	mov    DWORD PTR [esp+0x1a4],0x9
  40156e:	09 00 00 00 
  401572:	c7 84 24 a8 01 00 00 	mov    DWORD PTR [esp+0x1a8],0x34
  401579:	34 00 00 00 
  40157d:	89 84 24 ac 01 00 00 	mov    DWORD PTR [esp+0x1ac],eax
  401584:	c7 84 24 b0 01 00 00 	mov    DWORD PTR [esp+0x1b0],0x35
  40158b:	35 00 00 00 
  40158f:	89 8c 24 b4 01 00 00 	mov    DWORD PTR [esp+0x1b4],ecx
  401596:	c7 84 24 b8 01 00 00 	mov    DWORD PTR [esp+0x1b8],0x36
  40159d:	36 00 00 00 
  4015a1:	89 84 24 bc 01 00 00 	mov    DWORD PTR [esp+0x1bc],eax
  4015a8:	c7 84 24 c0 01 00 00 	mov    DWORD PTR [esp+0x1c0],0x37
  4015af:	37 00 00 00 
  4015b3:	c7 84 24 c4 01 00 00 	mov    DWORD PTR [esp+0x1c4],0xf
  4015ba:	0f 00 00 00 
  4015be:	c7 84 24 c8 01 00 00 	mov    DWORD PTR [esp+0x1c8],0x38
  4015c5:	38 00 00 00 
  4015c9:	89 84 24 cc 01 00 00 	mov    DWORD PTR [esp+0x1cc],eax
  4015d0:	c7 84 24 d0 01 00 00 	mov    DWORD PTR [esp+0x1d0],0x39
  4015d7:	39 00 00 00 
  4015db:	89 8c 24 d4 01 00 00 	mov    DWORD PTR [esp+0x1d4],ecx
  4015e2:	c7 84 24 d8 01 00 00 	mov    DWORD PTR [esp+0x1d8],0x3a
  4015e9:	3a 00 00 00 
  4015ed:	89 84 24 dc 01 00 00 	mov    DWORD PTR [esp+0x1dc],eax
  4015f4:	c7 84 24 e0 01 00 00 	mov    DWORD PTR [esp+0x1e0],0x3b
  4015fb:	3b 00 00 00 
  4015ff:	c7 84 24 e4 01 00 00 	mov    DWORD PTR [esp+0x1e4],0xa
  401606:	0a 00 00 00 
  40160a:	c7 84 24 e8 01 00 00 	mov    DWORD PTR [esp+0x1e8],0x3c
  401611:	3c 00 00 00 
  401615:	89 84 24 ec 01 00 00 	mov    DWORD PTR [esp+0x1ec],eax
  40161c:	c7 84 24 f0 01 00 00 	mov    DWORD PTR [esp+0x1f0],0x3d
  401623:	3d 00 00 00 
  401627:	89 84 24 f4 01 00 00 	mov    DWORD PTR [esp+0x1f4],eax
  40162e:	c7 84 24 f8 01 00 00 	mov    DWORD PTR [esp+0x1f8],0x3e
  401635:	3e 00 00 00 
  401639:	89 84 24 fc 01 00 00 	mov    DWORD PTR [esp+0x1fc],eax
  401640:	c7 84 24 00 02 00 00 	mov    DWORD PTR [esp+0x200],0x3f
  401647:	3f 00 00 00 
  40164b:	89 84 24 04 02 00 00 	mov    DWORD PTR [esp+0x204],eax
  401652:	c7 84 24 08 02 00 00 	mov    DWORD PTR [esp+0x208],0x40
  401659:	40 00 00 00 
  40165d:	89 84 24 0c 02 00 00 	mov    DWORD PTR [esp+0x20c],eax
  401664:	c7 84 24 10 02 00 00 	mov    DWORD PTR [esp+0x210],0x41
  40166b:	41 00 00 00 
  40166f:	89 84 24 14 02 00 00 	mov    DWORD PTR [esp+0x214],eax
  401676:	c7 84 24 18 02 00 00 	mov    DWORD PTR [esp+0x218],0x44
  40167d:	44 00 00 00 
  401681:	89 84 24 1c 02 00 00 	mov    DWORD PTR [esp+0x21c],eax
  401688:	8b 15 14 de 44 00    	mov    edx,DWORD PTR ds:0x44de14
  40168e:	33 c9                	xor    ecx,ecx
  401690:	89 8c 24 20 02 00 00 	mov    DWORD PTR [esp+0x220],ecx
  401697:	89 8c 24 24 02 00 00 	mov    DWORD PTR [esp+0x224],ecx
  40169e:	89 8c 24 28 02 00 00 	mov    DWORD PTR [esp+0x228],ecx
  4016a5:	89 8c 24 2c 02 00 00 	mov    DWORD PTR [esp+0x22c],ecx
  4016ac:	8b 4c d4 0c          	mov    ecx,DWORD PTR [esp+edx*8+0xc]
  4016b0:	83 f9 9d             	cmp    ecx,0xffffff9d
  4016b3:	89 0d ac dd 44 00    	mov    DWORD PTR ds:0x44ddac,ecx
  4016b9:	75 14                	jne    0x4016cf
  4016bb:	8b 0d f4 c2 51 00    	mov    ecx,DWORD PTR ds:0x51c2f4
  4016c1:	33 c0                	xor    eax,eax
  4016c3:	3b ce                	cmp    ecx,esi
  4016c5:	0f 95 c0             	setne  al
  4016c8:	a3 ac dd 44 00       	mov    ds:0x44ddac,eax
  4016cd:	eb 0a                	jmp    0x4016d9
  4016cf:	3b c8                	cmp    ecx,eax
  4016d1:	75 06                	jne    0x4016d9
  4016d3:	89 35 a0 dd 44 00    	mov    DWORD PTR ds:0x44dda0,esi
  4016d9:	a1 24 c3 51 00       	mov    eax,ds:0x51c324
  4016de:	5f                   	pop    edi
  4016df:	3b c6                	cmp    eax,esi
  4016e1:	5e                   	pop    esi
  4016e2:	74 0a                	je     0x4016ee
  4016e4:	c7 05 ac dd 44 00 13 	mov    DWORD PTR ds:0x44ddac,0x13
  4016eb:	00 00 00 
  4016ee:	81 c4 28 02 00 00    	add    esp,0x228
  4016f4:	c3                   	ret
