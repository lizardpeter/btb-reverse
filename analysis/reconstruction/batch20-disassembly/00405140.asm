
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00405140 <.text+0x4140>:
  405140:	55                   	push   ebp
  405141:	8b ec                	mov    ebp,esp
  405143:	6a ff                	push   0xffffffff
  405145:	68 20 a4 43 00       	push   0x43a420
  40514a:	64 a1 00 00 00 00    	mov    eax,fs:0x0
  405150:	50                   	push   eax
  405151:	64 89 25 00 00 00 00 	mov    DWORD PTR fs:0x0,esp
  405158:	83 ec 70             	sub    esp,0x70
  40515b:	53                   	push   ebx
  40515c:	8b d9                	mov    ebx,ecx
  40515e:	56                   	push   esi
  40515f:	33 f6                	xor    esi,esi
  405161:	8b 43 04             	mov    eax,DWORD PTR [ebx+0x4]
  405164:	57                   	push   edi
  405165:	3b c6                	cmp    eax,esi
  405167:	89 65 f0             	mov    DWORD PTR [ebp-0x10],esp
  40516a:	0f 85 81 00 00 00    	jne    0x4051f1
  405170:	8a 45 0b             	mov    al,BYTE PTR [ebp+0xb]
  405173:	bf c4 e1 43 00       	mov    edi,0x43e1c4
  405178:	88 45 d0             	mov    BYTE PTR [ebp-0x30],al
  40517b:	83 c9 ff             	or     ecx,0xffffffff
  40517e:	33 c0                	xor    eax,eax
  405180:	89 75 d4             	mov    DWORD PTR [ebp-0x2c],esi
  405183:	f2 ae                	repnz scas al,BYTE PTR es:[edi]
  405185:	f7 d1                	not    ecx
  405187:	49                   	dec    ecx
  405188:	89 75 d8             	mov    DWORD PTR [ebp-0x28],esi
  40518b:	51                   	push   ecx
  40518c:	68 c4 e1 43 00       	push   0x43e1c4
  405191:	8d 4d d0             	lea    ecx,[ebp-0x30]
  405194:	89 75 dc             	mov    DWORD PTR [ebp-0x24],esi
  405197:	e8 04 12 00 00       	call   0x4063a0
  40519c:	8d 4d 08             	lea    ecx,[ebp+0x8]
  40519f:	89 75 fc             	mov    DWORD PTR [ebp-0x4],esi
  4051a2:	51                   	push   ecx
  4051a3:	8d 4d a0             	lea    ecx,[ebp-0x60]
  4051a6:	c7 45 08 74 25 48 00 	mov    DWORD PTR [ebp+0x8],0x482574
  4051ad:	e8 08 b2 02 00       	call   0x4303ba
  4051b2:	8a 55 d0             	mov    dl,BYTE PTR [ebp-0x30]
  4051b5:	56                   	push   esi
  4051b6:	8d 4d ac             	lea    ecx,[ebp-0x54]
  4051b9:	c6 45 fc 01          	mov    BYTE PTR [ebp-0x4],0x1
  4051bd:	88 55 ac             	mov    BYTE PTR [ebp-0x54],dl
  4051c0:	e8 8b 11 00 00       	call   0x406350
  4051c5:	a1 34 b3 43 00       	mov    eax,ds:0x43b334
  4051ca:	8d 4d d0             	lea    ecx,[ebp-0x30]
  4051cd:	50                   	push   eax
  4051ce:	56                   	push   esi
  4051cf:	51                   	push   ecx
  4051d0:	8d 4d ac             	lea    ecx,[ebp-0x54]
  4051d3:	e8 78 0f 00 00       	call   0x406150
  4051d8:	8d 55 a0             	lea    edx,[ebp-0x60]
  4051db:	68 d0 bd 43 00       	push   0x43bdd0
  4051e0:	52                   	push   edx
  4051e1:	c7 45 a0 28 b3 43 00 	mov    DWORD PTR [ebp-0x60],0x43b328
  4051e8:	c6 45 fc 00          	mov    BYTE PTR [ebp-0x4],0x0
  4051ec:	e8 a4 b2 02 00       	call   0x430495
  4051f1:	8b 7d 08             	mov    edi,DWORD PTR [ebp+0x8]
  4051f4:	3b fe                	cmp    edi,esi
  4051f6:	0f 85 ac 00 00 00    	jne    0x4052a8
  4051fc:	8a 45 0b             	mov    al,BYTE PTR [ebp+0xb]
  4051ff:	56                   	push   esi
  405200:	8d 4d e0             	lea    ecx,[ebp-0x20]
  405203:	88 45 e0             	mov    BYTE PTR [ebp-0x20],al
  405206:	e8 45 11 00 00       	call   0x406350
  40520b:	bf b0 e1 43 00       	mov    edi,0x43e1b0
  405210:	83 c9 ff             	or     ecx,0xffffffff
  405213:	33 c0                	xor    eax,eax
  405215:	6a 01                	push   0x1
  405217:	f2 ae                	repnz scas al,BYTE PTR es:[edi]
  405219:	f7 d1                	not    ecx
  40521b:	49                   	dec    ecx
  40521c:	8b d9                	mov    ebx,ecx
  40521e:	8d 4d e0             	lea    ecx,[ebp-0x20]
  405221:	53                   	push   ebx
  405222:	e8 39 12 00 00       	call   0x406460
  405227:	84 c0                	test   al,al
  405229:	74 24                	je     0x40524f
  40522b:	8b 7d e4             	mov    edi,DWORD PTR [ebp-0x1c]
  40522e:	8b cb                	mov    ecx,ebx
  405230:	8b d1                	mov    edx,ecx
  405232:	be b0 e1 43 00       	mov    esi,0x43e1b0
  405237:	c1 e9 02             	shr    ecx,0x2
  40523a:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  40523c:	8b ca                	mov    ecx,edx
  40523e:	83 e1 03             	and    ecx,0x3
  405241:	f3 a4                	rep movs BYTE PTR es:[edi],BYTE PTR ds:[esi]
  405243:	8b 45 e4             	mov    eax,DWORD PTR [ebp-0x1c]
  405246:	89 5d e8             	mov    DWORD PTR [ebp-0x18],ebx
  405249:	33 f6                	xor    esi,esi
  40524b:	c6 04 18 00          	mov    BYTE PTR [eax+ebx*1],0x0
  40524f:	8d 4d 08             	lea    ecx,[ebp+0x8]
  405252:	bb 02 00 00 00       	mov    ebx,0x2
  405257:	51                   	push   ecx
  405258:	8d 4d 84             	lea    ecx,[ebp-0x7c]
  40525b:	89 5d fc             	mov    DWORD PTR [ebp-0x4],ebx
  40525e:	c7 45 08 74 25 48 00 	mov    DWORD PTR [ebp+0x8],0x482574
  405265:	e8 50 b1 02 00       	call   0x4303ba
  40526a:	8a 55 e0             	mov    dl,BYTE PTR [ebp-0x20]
  40526d:	56                   	push   esi
  40526e:	8d 4d 90             	lea    ecx,[ebp-0x70]
  405271:	c6 45 fc 03          	mov    BYTE PTR [ebp-0x4],0x3
  405275:	88 55 90             	mov    BYTE PTR [ebp-0x70],dl
  405278:	e8 d3 10 00 00       	call   0x406350
  40527d:	a1 34 b3 43 00       	mov    eax,ds:0x43b334
  405282:	8d 4d e0             	lea    ecx,[ebp-0x20]
  405285:	50                   	push   eax
  405286:	56                   	push   esi
  405287:	51                   	push   ecx
  405288:	8d 4d 90             	lea    ecx,[ebp-0x70]
  40528b:	e8 c0 0e 00 00       	call   0x406150
  405290:	8d 55 84             	lea    edx,[ebp-0x7c]
  405293:	68 d0 bd 43 00       	push   0x43bdd0
  405298:	52                   	push   edx
  405299:	c7 45 84 28 b3 43 00 	mov    DWORD PTR [ebp-0x7c],0x43b328
  4052a0:	88 5d fc             	mov    BYTE PTR [ebp-0x4],bl
  4052a3:	e8 ed b1 02 00       	call   0x430495
  4052a8:	33 c9                	xor    ecx,ecx
  4052aa:	8d 55 bc             	lea    edx,[ebp-0x44]
  4052ad:	89 4d c4             	mov    DWORD PTR [ebp-0x3c],ecx
  4052b0:	8d 43 0c             	lea    eax,[ebx+0xc]
  4052b3:	89 4d c8             	mov    DWORD PTR [ebp-0x38],ecx
  4052b6:	52                   	push   edx
  4052b7:	57                   	push   edi
  4052b8:	c7 45 bc 14 00 00 00 	mov    DWORD PTR [ebp-0x44],0x14
  4052bf:	89 45 c0             	mov    DWORD PTR [ebp-0x40],eax
  4052c2:	89 4d cc             	mov    DWORD PTR [ebp-0x34],ecx
  4052c5:	ff 15 68 b0 43 00    	call   DWORD PTR ds:0x43b068
  4052cb:	85 c0                	test   eax,eax
  4052cd:	7e 27                	jle    0x4052f6
  4052cf:	57                   	push   edi
  4052d0:	ff 15 3c b0 43 00    	call   DWORD PTR ds:0x43b03c
  4052d6:	85 c0                	test   eax,eax
  4052d8:	7e 15                	jle    0x4052ef
  4052da:	8b 03                	mov    eax,DWORD PTR [ebx]
  4052dc:	57                   	push   edi
  4052dd:	8b cb                	mov    ecx,ebx
  4052df:	c7 45 fc 04 00 00 00 	mov    DWORD PTR [ebp-0x4],0x4
  4052e6:	ff 10                	call   DWORD PTR [eax]
  4052e8:	57                   	push   edi
  4052e9:	ff 15 38 b0 43 00    	call   DWORD PTR ds:0x43b038
  4052ef:	57                   	push   edi
  4052f0:	ff 15 34 b0 43 00    	call   DWORD PTR ds:0x43b034
  4052f6:	8b 4d f4             	mov    ecx,DWORD PTR [ebp-0xc]
  4052f9:	5f                   	pop    edi
  4052fa:	5e                   	pop    esi
  4052fb:	64 89 0d 00 00 00 00 	mov    DWORD PTR fs:0x0,ecx
  405302:	5b                   	pop    ebx
  405303:	8b e5                	mov    esp,ebp
  405305:	5d                   	pop    ebp
  405306:	c2 04 00             	ret    0x4
