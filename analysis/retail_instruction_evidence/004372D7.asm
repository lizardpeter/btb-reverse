
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004372d7 <.text+0x362d7>:
  4372d7:	8b 44 24 04          	mov    eax,DWORD PTR [esp+0x4]
  4372db:	53                   	push   ebx
  4372dc:	3b 05 20 da 51 00    	cmp    eax,DWORD PTR ds:0x51da20
  4372e2:	56                   	push   esi
  4372e3:	57                   	push   edi
  4372e4:	73 73                	jae    0x437359
  4372e6:	8b c8                	mov    ecx,eax
  4372e8:	8b f0                	mov    esi,eax
  4372ea:	c1 f9 05             	sar    ecx,0x5
  4372ed:	83 e6 1f             	and    esi,0x1f
  4372f0:	8d 3c 8d 20 d9 51 00 	lea    edi,[ecx*4+0x51d920]
  4372f7:	c1 e6 03             	shl    esi,0x3
  4372fa:	8b 0f                	mov    ecx,DWORD PTR [edi]
  4372fc:	f6 44 31 04 01       	test   BYTE PTR [ecx+esi*1+0x4],0x1
  437301:	74 56                	je     0x437359
  437303:	50                   	push   eax
  437304:	e8 19 f0 ff ff       	call   0x436322
  437309:	83 f8 ff             	cmp    eax,0xffffffff
  43730c:	59                   	pop    ecx
  43730d:	75 0c                	jne    0x43731b
  43730f:	c7 05 c8 c3 51 00 09 	mov    DWORD PTR ds:0x51c3c8,0x9
  437316:	00 00 00 
  437319:	eb 4f                	jmp    0x43736a
  43731b:	ff 74 24 18          	push   DWORD PTR [esp+0x18]
  43731f:	6a 00                	push   0x0
  437321:	ff 74 24 1c          	push   DWORD PTR [esp+0x1c]
  437325:	50                   	push   eax
  437326:	ff 15 84 b1 43 00    	call   DWORD PTR ds:0x43b184
  43732c:	8b d8                	mov    ebx,eax
  43732e:	83 fb ff             	cmp    ebx,0xffffffff
  437331:	75 08                	jne    0x43733b
  437333:	ff 15 2c b1 43 00    	call   DWORD PTR ds:0x43b12c
  437339:	eb 02                	jmp    0x43733d
  43733b:	33 c0                	xor    eax,eax
  43733d:	85 c0                	test   eax,eax
  43733f:	74 09                	je     0x43734a
  437341:	50                   	push   eax
  437342:	e8 33 b4 ff ff       	call   0x43277a
  437347:	59                   	pop    ecx
  437348:	eb 20                	jmp    0x43736a
  43734a:	8b 07                	mov    eax,DWORD PTR [edi]
  43734c:	80 64 30 04 fd       	and    BYTE PTR [eax+esi*1+0x4],0xfd
  437351:	8d 44 30 04          	lea    eax,[eax+esi*1+0x4]
  437355:	8b c3                	mov    eax,ebx
  437357:	eb 14                	jmp    0x43736d
  437359:	83 25 cc c3 51 00 00 	and    DWORD PTR ds:0x51c3cc,0x0
  437360:	c7 05 c8 c3 51 00 09 	mov    DWORD PTR ds:0x51c3c8,0x9
  437367:	00 00 00 
  43736a:	83 c8 ff             	or     eax,0xffffffff
  43736d:	5f                   	pop    edi
  43736e:	5e                   	pop    esi
  43736f:	5b                   	pop    ebx
  437370:	c3                   	ret
