
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004370b3 <.text+0x360b3>:
  4370b3:	55                   	push   ebp
  4370b4:	8b ec                	mov    ebp,esp
  4370b6:	6a ff                	push   0xffffffff
  4370b8:	68 b0 ba 43 00       	push   0x43bab0
  4370bd:	68 38 5f 43 00       	push   0x435f38
  4370c2:	64 a1 00 00 00 00    	mov    eax,fs:0x0
  4370c8:	50                   	push   eax
  4370c9:	64 89 25 00 00 00 00 	mov    DWORD PTR fs:0x0,esp
  4370d0:	83 ec 1c             	sub    esp,0x1c
  4370d3:	53                   	push   ebx
  4370d4:	56                   	push   esi
  4370d5:	57                   	push   edi
  4370d6:	89 65 e8             	mov    DWORD PTR [ebp-0x18],esp
  4370d9:	33 ff                	xor    edi,edi
  4370db:	39 3d 30 c6 51 00    	cmp    DWORD PTR ds:0x51c630,edi
  4370e1:	75 46                	jne    0x437129
  4370e3:	57                   	push   edi
  4370e4:	57                   	push   edi
  4370e5:	6a 01                	push   0x1
  4370e7:	5b                   	pop    ebx
  4370e8:	53                   	push   ebx
  4370e9:	68 a8 ba 43 00       	push   0x43baa8
  4370ee:	be 00 01 00 00       	mov    esi,0x100
  4370f3:	56                   	push   esi
  4370f4:	57                   	push   edi
  4370f5:	ff 15 80 b1 43 00    	call   DWORD PTR ds:0x43b180
  4370fb:	85 c0                	test   eax,eax
  4370fd:	74 08                	je     0x437107
  4370ff:	89 1d 30 c6 51 00    	mov    DWORD PTR ds:0x51c630,ebx
  437105:	eb 22                	jmp    0x437129
  437107:	57                   	push   edi
  437108:	57                   	push   edi
  437109:	53                   	push   ebx
  43710a:	68 a4 ba 43 00       	push   0x43baa4
  43710f:	56                   	push   esi
  437110:	57                   	push   edi
  437111:	ff 15 7c b1 43 00    	call   DWORD PTR ds:0x43b17c
  437117:	85 c0                	test   eax,eax
  437119:	0f 84 22 01 00 00    	je     0x437241
  43711f:	c7 05 30 c6 51 00 02 	mov    DWORD PTR ds:0x51c630,0x2
  437126:	00 00 00 
  437129:	39 7d 14             	cmp    DWORD PTR [ebp+0x14],edi
  43712c:	7e 10                	jle    0x43713e
  43712e:	ff 75 14             	push   DWORD PTR [ebp+0x14]
  437131:	ff 75 10             	push   DWORD PTR [ebp+0x10]
  437134:	e8 e1 27 00 00       	call   0x43991a
  437139:	59                   	pop    ecx
  43713a:	59                   	pop    ecx
  43713b:	89 45 14             	mov    DWORD PTR [ebp+0x14],eax
  43713e:	a1 30 c6 51 00       	mov    eax,ds:0x51c630
  437143:	83 f8 02             	cmp    eax,0x2
  437146:	75 1d                	jne    0x437165
  437148:	ff 75 1c             	push   DWORD PTR [ebp+0x1c]
  43714b:	ff 75 18             	push   DWORD PTR [ebp+0x18]
  43714e:	ff 75 14             	push   DWORD PTR [ebp+0x14]
  437151:	ff 75 10             	push   DWORD PTR [ebp+0x10]
  437154:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  437157:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  43715a:	ff 15 7c b1 43 00    	call   DWORD PTR ds:0x43b17c
  437160:	e9 de 00 00 00       	jmp    0x437243
  437165:	83 f8 01             	cmp    eax,0x1
  437168:	0f 85 d3 00 00 00    	jne    0x437241
  43716e:	39 7d 20             	cmp    DWORD PTR [ebp+0x20],edi
  437171:	75 08                	jne    0x43717b
  437173:	a1 90 c6 51 00       	mov    eax,ds:0x51c690
  437178:	89 45 20             	mov    DWORD PTR [ebp+0x20],eax
  43717b:	57                   	push   edi
  43717c:	57                   	push   edi
  43717d:	ff 75 14             	push   DWORD PTR [ebp+0x14]
  437180:	ff 75 10             	push   DWORD PTR [ebp+0x10]
  437183:	8b 45 24             	mov    eax,DWORD PTR [ebp+0x24]
  437186:	f7 d8                	neg    eax
  437188:	1b c0                	sbb    eax,eax
  43718a:	83 e0 08             	and    eax,0x8
  43718d:	40                   	inc    eax
  43718e:	50                   	push   eax
  43718f:	ff 75 20             	push   DWORD PTR [ebp+0x20]
  437192:	ff 15 d0 b0 43 00    	call   DWORD PTR ds:0x43b0d0
  437198:	8b d8                	mov    ebx,eax
  43719a:	89 5d e4             	mov    DWORD PTR [ebp-0x1c],ebx
  43719d:	3b df                	cmp    ebx,edi
  43719f:	0f 84 9c 00 00 00    	je     0x437241
  4371a5:	89 7d fc             	mov    DWORD PTR [ebp-0x4],edi
  4371a8:	8d 04 1b             	lea    eax,[ebx+ebx*1]
  4371ab:	83 c0 03             	add    eax,0x3
  4371ae:	24 fc                	and    al,0xfc
  4371b0:	e8 4b 9d ff ff       	call   0x430f00
  4371b5:	89 65 e8             	mov    DWORD PTR [ebp-0x18],esp
  4371b8:	8b c4                	mov    eax,esp
  4371ba:	89 45 dc             	mov    DWORD PTR [ebp-0x24],eax
  4371bd:	83 4d fc ff          	or     DWORD PTR [ebp-0x4],0xffffffff
  4371c1:	eb 13                	jmp    0x4371d6
  4371c3:	6a 01                	push   0x1
  4371c5:	58                   	pop    eax
  4371c6:	c3                   	ret
  4371c7:	8b 65 e8             	mov    esp,DWORD PTR [ebp-0x18]
  4371ca:	33 ff                	xor    edi,edi
  4371cc:	89 7d dc             	mov    DWORD PTR [ebp-0x24],edi
  4371cf:	83 4d fc ff          	or     DWORD PTR [ebp-0x4],0xffffffff
  4371d3:	8b 5d e4             	mov    ebx,DWORD PTR [ebp-0x1c]
  4371d6:	39 7d dc             	cmp    DWORD PTR [ebp-0x24],edi
  4371d9:	74 66                	je     0x437241
  4371db:	53                   	push   ebx
  4371dc:	ff 75 dc             	push   DWORD PTR [ebp-0x24]
  4371df:	ff 75 14             	push   DWORD PTR [ebp+0x14]
  4371e2:	ff 75 10             	push   DWORD PTR [ebp+0x10]
  4371e5:	6a 01                	push   0x1
  4371e7:	ff 75 20             	push   DWORD PTR [ebp+0x20]
  4371ea:	ff 15 d0 b0 43 00    	call   DWORD PTR ds:0x43b0d0
  4371f0:	85 c0                	test   eax,eax
  4371f2:	74 4d                	je     0x437241
  4371f4:	57                   	push   edi
  4371f5:	57                   	push   edi
  4371f6:	53                   	push   ebx
  4371f7:	ff 75 dc             	push   DWORD PTR [ebp-0x24]
  4371fa:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  4371fd:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  437200:	ff 15 80 b1 43 00    	call   DWORD PTR ds:0x43b180
  437206:	8b f0                	mov    esi,eax
  437208:	89 75 d8             	mov    DWORD PTR [ebp-0x28],esi
  43720b:	3b f7                	cmp    esi,edi
  43720d:	74 32                	je     0x437241
  43720f:	f6 45 0d 04          	test   BYTE PTR [ebp+0xd],0x4
  437213:	74 40                	je     0x437255
  437215:	39 7d 1c             	cmp    DWORD PTR [ebp+0x1c],edi
  437218:	0f 84 b2 00 00 00    	je     0x4372d0
  43721e:	3b 75 1c             	cmp    esi,DWORD PTR [ebp+0x1c]
  437221:	7f 1e                	jg     0x437241
  437223:	ff 75 1c             	push   DWORD PTR [ebp+0x1c]
  437226:	ff 75 18             	push   DWORD PTR [ebp+0x18]
  437229:	53                   	push   ebx
  43722a:	ff 75 dc             	push   DWORD PTR [ebp-0x24]
  43722d:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  437230:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  437233:	ff 15 80 b1 43 00    	call   DWORD PTR ds:0x43b180
  437239:	85 c0                	test   eax,eax
  43723b:	0f 85 8f 00 00 00    	jne    0x4372d0
  437241:	33 c0                	xor    eax,eax
  437243:	8d 65 c8             	lea    esp,[ebp-0x38]
  437246:	8b 4d f0             	mov    ecx,DWORD PTR [ebp-0x10]
  437249:	64 89 0d 00 00 00 00 	mov    DWORD PTR fs:0x0,ecx
  437250:	5f                   	pop    edi
  437251:	5e                   	pop    esi
  437252:	5b                   	pop    ebx
  437253:	c9                   	leave
  437254:	c3                   	ret
  437255:	c7 45 fc 01 00 00 00 	mov    DWORD PTR [ebp-0x4],0x1
  43725c:	8d 04 36             	lea    eax,[esi+esi*1]
  43725f:	83 c0 03             	add    eax,0x3
  437262:	24 fc                	and    al,0xfc
  437264:	e8 97 9c ff ff       	call   0x430f00
  437269:	89 65 e8             	mov    DWORD PTR [ebp-0x18],esp
  43726c:	8b dc                	mov    ebx,esp
  43726e:	89 5d e0             	mov    DWORD PTR [ebp-0x20],ebx
  437271:	83 4d fc ff          	or     DWORD PTR [ebp-0x4],0xffffffff
  437275:	eb 12                	jmp    0x437289
  437277:	6a 01                	push   0x1
  437279:	58                   	pop    eax
  43727a:	c3                   	ret
  43727b:	8b 65 e8             	mov    esp,DWORD PTR [ebp-0x18]
  43727e:	33 ff                	xor    edi,edi
  437280:	33 db                	xor    ebx,ebx
  437282:	83 4d fc ff          	or     DWORD PTR [ebp-0x4],0xffffffff
  437286:	8b 75 d8             	mov    esi,DWORD PTR [ebp-0x28]
  437289:	3b df                	cmp    ebx,edi
  43728b:	74 b4                	je     0x437241
  43728d:	56                   	push   esi
  43728e:	53                   	push   ebx
  43728f:	ff 75 e4             	push   DWORD PTR [ebp-0x1c]
  437292:	ff 75 dc             	push   DWORD PTR [ebp-0x24]
  437295:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  437298:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  43729b:	ff 15 80 b1 43 00    	call   DWORD PTR ds:0x43b180
  4372a1:	85 c0                	test   eax,eax
  4372a3:	74 9c                	je     0x437241
  4372a5:	39 7d 1c             	cmp    DWORD PTR [ebp+0x1c],edi
  4372a8:	57                   	push   edi
  4372a9:	57                   	push   edi
  4372aa:	75 04                	jne    0x4372b0
  4372ac:	57                   	push   edi
  4372ad:	57                   	push   edi
  4372ae:	eb 06                	jmp    0x4372b6
  4372b0:	ff                   	.byte 0xff
  4372b1:	75                   	.byte 0x75
