
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0043407d <.text+0x3307d>:
  43407d:	55                   	push   ebp
  43407e:	8b ec                	mov    ebp,esp
  434080:	81 ec 14 04 00 00    	sub    esp,0x414
  434086:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  434089:	53                   	push   ebx
  43408a:	3b 0d 20 da 51 00    	cmp    ecx,DWORD PTR ds:0x51da20
  434090:	56                   	push   esi
  434091:	57                   	push   edi
  434092:	0f 83 79 01 00 00    	jae    0x434211
  434098:	8b c1                	mov    eax,ecx
  43409a:	8b f1                	mov    esi,ecx
  43409c:	c1 f8 05             	sar    eax,0x5
  43409f:	83 e6 1f             	and    esi,0x1f
  4340a2:	8d 1c 85 20 d9 51 00 	lea    ebx,[eax*4+0x51d920]
  4340a9:	c1 e6 03             	shl    esi,0x3
  4340ac:	8b 03                	mov    eax,DWORD PTR [ebx]
  4340ae:	8a 44 30 04          	mov    al,BYTE PTR [eax+esi*1+0x4]
  4340b2:	a8 01                	test   al,0x1
  4340b4:	0f 84 57 01 00 00    	je     0x434211
  4340ba:	33 ff                	xor    edi,edi
  4340bc:	39 7d 10             	cmp    DWORD PTR [ebp+0x10],edi
  4340bf:	89 7d f8             	mov    DWORD PTR [ebp-0x8],edi
  4340c2:	89 7d f0             	mov    DWORD PTR [ebp-0x10],edi
  4340c5:	75 07                	jne    0x4340ce
  4340c7:	33 c0                	xor    eax,eax
  4340c9:	e9 57 01 00 00       	jmp    0x434225
  4340ce:	a8 20                	test   al,0x20
  4340d0:	74 0c                	je     0x4340de
  4340d2:	6a 02                	push   0x2
  4340d4:	57                   	push   edi
  4340d5:	51                   	push   ecx
  4340d6:	e8 fc 31 00 00       	call   0x4372d7
  4340db:	83 c4 0c             	add    esp,0xc
  4340de:	8b 03                	mov    eax,DWORD PTR [ebx]
  4340e0:	03 c6                	add    eax,esi
  4340e2:	f6 40 04 80          	test   BYTE PTR [eax+0x4],0x80
  4340e6:	0f 84 c1 00 00 00    	je     0x4341ad
  4340ec:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  4340ef:	39 7d 10             	cmp    DWORD PTR [ebp+0x10],edi
  4340f2:	89 45 fc             	mov    DWORD PTR [ebp-0x4],eax
  4340f5:	89 7d 08             	mov    DWORD PTR [ebp+0x8],edi
  4340f8:	0f 86 e7 00 00 00    	jbe    0x4341e5
  4340fe:	8d 85 ec fb ff ff    	lea    eax,[ebp-0x414]
  434104:	8b 4d fc             	mov    ecx,DWORD PTR [ebp-0x4]
  434107:	2b 4d 0c             	sub    ecx,DWORD PTR [ebp+0xc]
  43410a:	3b 4d 10             	cmp    ecx,DWORD PTR [ebp+0x10]
  43410d:	73 29                	jae    0x434138
  43410f:	8b 4d fc             	mov    ecx,DWORD PTR [ebp-0x4]
  434112:	ff 45 fc             	inc    DWORD PTR [ebp-0x4]
  434115:	8a 09                	mov    cl,BYTE PTR [ecx]
  434117:	80 f9 0a             	cmp    cl,0xa
  43411a:	75 07                	jne    0x434123
  43411c:	ff 45 f0             	inc    DWORD PTR [ebp-0x10]
  43411f:	c6 00 0d             	mov    BYTE PTR [eax],0xd
  434122:	40                   	inc    eax
  434123:	88 08                	mov    BYTE PTR [eax],cl
  434125:	40                   	inc    eax
  434126:	8b c8                	mov    ecx,eax
  434128:	8d 95 ec fb ff ff    	lea    edx,[ebp-0x414]
  43412e:	2b ca                	sub    ecx,edx
  434130:	81 f9 00 04 00 00    	cmp    ecx,0x400
  434136:	7c cc                	jl     0x434104
  434138:	8b f8                	mov    edi,eax
  43413a:	8d 85 ec fb ff ff    	lea    eax,[ebp-0x414]
  434140:	2b f8                	sub    edi,eax
  434142:	8d 45 f4             	lea    eax,[ebp-0xc]
  434145:	6a 00                	push   0x0
  434147:	50                   	push   eax
  434148:	8d 85 ec fb ff ff    	lea    eax,[ebp-0x414]
  43414e:	57                   	push   edi
  43414f:	50                   	push   eax
  434150:	8b 03                	mov    eax,DWORD PTR [ebx]
  434152:	ff 34 30             	push   DWORD PTR [eax+esi*1]
  434155:	ff 15 f0 b0 43 00    	call   DWORD PTR ds:0x43b0f0
  43415b:	85 c0                	test   eax,eax
  43415d:	74 43                	je     0x4341a2
  43415f:	8b 45 f4             	mov    eax,DWORD PTR [ebp-0xc]
  434162:	01 45 f8             	add    DWORD PTR [ebp-0x8],eax
  434165:	3b c7                	cmp    eax,edi
  434167:	7c 0b                	jl     0x434174
  434169:	8b 45 fc             	mov    eax,DWORD PTR [ebp-0x4]
  43416c:	2b 45 0c             	sub    eax,DWORD PTR [ebp+0xc]
  43416f:	3b 45 10             	cmp    eax,DWORD PTR [ebp+0x10]
  434172:	72 8a                	jb     0x4340fe
  434174:	33 ff                	xor    edi,edi
  434176:	8b 45 f8             	mov    eax,DWORD PTR [ebp-0x8]
  434179:	3b c7                	cmp    eax,edi
  43417b:	0f 85 8b 00 00 00    	jne    0x43420c
  434181:	39 7d 08             	cmp    DWORD PTR [ebp+0x8],edi
  434184:	74 5f                	je     0x4341e5
  434186:	6a 05                	push   0x5
  434188:	58                   	pop    eax
  434189:	39 45 08             	cmp    DWORD PTR [ebp+0x8],eax
  43418c:	75 4c                	jne    0x4341da
  43418e:	c7 05 c8 c3 51 00 09 	mov    DWORD PTR ds:0x51c3c8,0x9
  434195:	00 00 00 
  434198:	a3 cc c3 51 00       	mov    ds:0x51c3cc,eax
  43419d:	e9 80 00 00 00       	jmp    0x434222
  4341a2:	ff 15 2c b1 43 00    	call   DWORD PTR ds:0x43b12c
  4341a8:	89 45 08             	mov    DWORD PTR [ebp+0x8],eax
  4341ab:	eb c7                	jmp    0x434174
  4341ad:	8d 4d f4             	lea    ecx,[ebp-0xc]
  4341b0:	57                   	push   edi
  4341b1:	51                   	push   ecx
  4341b2:	ff 75 10             	push   DWORD PTR [ebp+0x10]
  4341b5:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  4341b8:	ff 30                	push   DWORD PTR [eax]
  4341ba:	ff 15 f0 b0 43 00    	call   DWORD PTR ds:0x43b0f0
  4341c0:	85 c0                	test   eax,eax
  4341c2:	74 0b                	je     0x4341cf
  4341c4:	8b 45 f4             	mov    eax,DWORD PTR [ebp-0xc]
  4341c7:	89 7d 08             	mov    DWORD PTR [ebp+0x8],edi
  4341ca:	89 45 f8             	mov    DWORD PTR [ebp-0x8],eax
  4341cd:	eb a7                	jmp    0x434176
  4341cf:	ff 15 2c b1 43 00    	call   DWORD PTR ds:0x43b12c
  4341d5:	89 45 08             	mov    DWORD PTR [ebp+0x8],eax
  4341d8:	eb 9c                	jmp    0x434176
  4341da:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  4341dd:	e8 98 e5 ff ff       	call   0x43277a
  4341e2:	59                   	pop    ecx
  4341e3:	eb 3d                	jmp    0x434222
  4341e5:	8b 03                	mov    eax,DWORD PTR [ebx]
  4341e7:	f6 44 30 04 40       	test   BYTE PTR [eax+esi*1+0x4],0x40
  4341ec:	74 0c                	je     0x4341fa
  4341ee:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  4341f1:	80 38 1a             	cmp    BYTE PTR [eax],0x1a
  4341f4:	0f 84 cd fe ff ff    	je     0x4340c7
  4341fa:	c7 05 c8 c3 51 00 1c 	mov    DWORD PTR ds:0x51c3c8,0x1c
  434201:	00 00 00 
  434204:	89 3d cc c3 51 00    	mov    DWORD PTR ds:0x51c3cc,edi
  43420a:	eb 16                	jmp    0x434222
  43420c:	2b 45 f0             	sub    eax,DWORD PTR [ebp-0x10]
  43420f:	eb 14                	jmp    0x434225
  434211:	83 25 cc c3 51 00 00 	and    DWORD PTR ds:0x51c3cc,0x0
  434218:	c7 05 c8 c3 51 00 09 	mov    DWORD PTR ds:0x51c3c8,0x9
  43421f:	00 00 00 
  434222:	83 c8 ff             	or     eax,0xffffffff
  434225:	5f                   	pop    edi
  434226:	5e                   	pop    esi
  434227:	5b                   	pop    ebx
  434228:	c9                   	leave
  434229:	c3                   	ret
