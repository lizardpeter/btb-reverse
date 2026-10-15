
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00436049 <.text+0x35049>:
  436049:	55                   	push   ebp
  43604a:	8b ec                	mov    ebp,esp
  43604c:	81 ec a4 01 00 00    	sub    esp,0x1a4
  436052:	8b 55 08             	mov    edx,DWORD PTR [ebp+0x8]
  436055:	33 c9                	xor    ecx,ecx
  436057:	b8 00 98 44 00       	mov    eax,0x449800
  43605c:	3b 10                	cmp    edx,DWORD PTR [eax]
  43605e:	74 0b                	je     0x43606b
  436060:	83 c0 08             	add    eax,0x8
  436063:	41                   	inc    ecx
  436064:	3d 90 98 44 00       	cmp    eax,0x449890
  436069:	72 f1                	jb     0x43605c
  43606b:	56                   	push   esi
  43606c:	8b f1                	mov    esi,ecx
  43606e:	c1 e6 03             	shl    esi,0x3
  436071:	3b 96 00 98 44 00    	cmp    edx,DWORD PTR [esi+0x449800]
  436077:	0f 85 1c 01 00 00    	jne    0x436199
  43607d:	a1 1c c4 51 00       	mov    eax,ds:0x51c41c
  436082:	83 f8 01             	cmp    eax,0x1
  436085:	0f 84 e8 00 00 00    	je     0x436173
  43608b:	85 c0                	test   eax,eax
  43608d:	75 0d                	jne    0x43609c
  43608f:	83 3d 94 75 44 00 01 	cmp    DWORD PTR ds:0x447594,0x1
  436096:	0f 84 d7 00 00 00    	je     0x436173
  43609c:	81 fa fc 00 00 00    	cmp    edx,0xfc
  4360a2:	0f 84 f1 00 00 00    	je     0x436199
  4360a8:	8d 85 5c fe ff ff    	lea    eax,[ebp-0x1a4]
  4360ae:	68 04 01 00 00       	push   0x104
  4360b3:	50                   	push   eax
  4360b4:	6a 00                	push   0x0
  4360b6:	ff 15 ec b0 43 00    	call   DWORD PTR ds:0x43b0ec
  4360bc:	85 c0                	test   eax,eax
  4360be:	75 13                	jne    0x4360d3
  4360c0:	8d 85 5c fe ff ff    	lea    eax,[ebp-0x1a4]
  4360c6:	68 48 ba 43 00       	push   0x43ba48
  4360cb:	50                   	push   eax
  4360cc:	e8 5f d5 ff ff       	call   0x433630
  4360d1:	59                   	pop    ecx
  4360d2:	59                   	pop    ecx
  4360d3:	8d 85 5c fe ff ff    	lea    eax,[ebp-0x1a4]
  4360d9:	57                   	push   edi
  4360da:	50                   	push   eax
  4360db:	8d bd 5c fe ff ff    	lea    edi,[ebp-0x1a4]
  4360e1:	e8 3a d6 ff ff       	call   0x433720
  4360e6:	40                   	inc    eax
  4360e7:	59                   	pop    ecx
  4360e8:	83 f8 3c             	cmp    eax,0x3c
  4360eb:	76 29                	jbe    0x436116
  4360ed:	8d 85 5c fe ff ff    	lea    eax,[ebp-0x1a4]
  4360f3:	50                   	push   eax
  4360f4:	e8 27 d6 ff ff       	call   0x433720
  4360f9:	8b f8                	mov    edi,eax
  4360fb:	8d 85 5c fe ff ff    	lea    eax,[ebp-0x1a4]
  436101:	83 e8 3b             	sub    eax,0x3b
  436104:	6a 03                	push   0x3
  436106:	03 f8                	add    edi,eax
  436108:	68 44 ba 43 00       	push   0x43ba44
  43610d:	57                   	push   edi
  43610e:	e8 8d a1 ff ff       	call   0x4302a0
  436113:	83 c4 10             	add    esp,0x10
  436116:	8d 85 60 ff ff ff    	lea    eax,[ebp-0xa0]
  43611c:	68 28 ba 43 00       	push   0x43ba28
  436121:	50                   	push   eax
  436122:	e8 09 d5 ff ff       	call   0x433630
  436127:	8d 85 60 ff ff ff    	lea    eax,[ebp-0xa0]
  43612d:	57                   	push   edi
  43612e:	50                   	push   eax
  43612f:	e8 0c d5 ff ff       	call   0x433640
  436134:	8d 85 60 ff ff ff    	lea    eax,[ebp-0xa0]
  43613a:	68 24 ba 43 00       	push   0x43ba24
  43613f:	50                   	push   eax
  436140:	e8 fb d4 ff ff       	call   0x433640
  436145:	ff b6 04 98 44 00    	push   DWORD PTR [esi+0x449804]
  43614b:	8d 85 60 ff ff ff    	lea    eax,[ebp-0xa0]
  436151:	50                   	push   eax
  436152:	e8 e9 d4 ff ff       	call   0x433640
  436157:	68 10 20 01 00       	push   0x12010
  43615c:	8d 85 60 ff ff ff    	lea    eax,[ebp-0xa0]
  436162:	68 fc b9 43 00       	push   0x43b9fc
  436167:	50                   	push   eax
  436168:	e8 05 21 00 00       	call   0x438272
  43616d:	83 c4 2c             	add    esp,0x2c
  436170:	5f                   	pop    edi
  436171:	eb 26                	jmp    0x436199
  436173:	8d 45 08             	lea    eax,[ebp+0x8]
  436176:	8d b6 04 98 44 00    	lea    esi,[esi+0x449804]
  43617c:	6a 00                	push   0x0
  43617e:	50                   	push   eax
  43617f:	ff 36                	push   DWORD PTR [esi]
  436181:	e8 9a d5 ff ff       	call   0x433720
  436186:	59                   	pop    ecx
  436187:	50                   	push   eax
  436188:	ff 36                	push   DWORD PTR [esi]
  43618a:	6a f4                	push   0xfffffff4
  43618c:	ff 15 b4 b0 43 00    	call   DWORD PTR ds:0x43b0b4
  436192:	50                   	push   eax
  436193:	ff 15 f0 b0 43 00    	call   DWORD PTR ds:0x43b0f0
  436199:	5e                   	pop    esi
  43619a:	c9                   	leave
  43619b:	c3                   	ret
