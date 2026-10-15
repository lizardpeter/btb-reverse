
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00404100 <.text+0x3100>:
  404100:	83 ec 0c             	sub    esp,0xc
  404103:	55                   	push   ebp
  404104:	8b 6c 24 14          	mov    ebp,DWORD PTR [esp+0x14]
  404108:	56                   	push   esi
  404109:	57                   	push   edi
  40410a:	85 ed                	test   ebp,ebp
  40410c:	8b f1                	mov    esi,ecx
  40410e:	c7 44 24 10 00 00 00 	mov    DWORD PTR [esp+0x10],0x0
  404115:	00 
  404116:	c7 44 24 0c 00 00 00 	mov    DWORD PTR [esp+0xc],0x0
  40411d:	00 
  40411e:	c7 44 24 14 00 00 00 	mov    DWORD PTR [esp+0x14],0x0
  404125:	00 
  404126:	75 0e                	jne    0x404136
  404128:	5f                   	pop    edi
  404129:	5e                   	pop    esi
  40412a:	b8 f0 01 04 80       	mov    eax,0x800401f0
  40412f:	5d                   	pop    ebp
  404130:	83 c4 0c             	add    esp,0xc
  404133:	c2 08 00             	ret    0x8
  404136:	6a 00                	push   0x0
  404138:	55                   	push   ebp
  404139:	8b ce                	mov    ecx,esi
  40413b:	e8 30 01 00 00       	call   0x404270
  404140:	85 c0                	test   eax,eax
  404142:	0f 8c 17 01 00 00    	jl     0x40425f
  404148:	8b 45 00             	mov    eax,DWORD PTR [ebp+0x0]
  40414b:	6a 00                	push   0x0
  40414d:	6a 00                	push   0x0
  40414f:	8d 4c 24 14          	lea    ecx,[esp+0x14]
  404153:	6a 00                	push   0x0
  404155:	51                   	push   ecx
  404156:	8b 4e 08             	mov    ecx,DWORD PTR [esi+0x8]
  404159:	8d 54 24 20          	lea    edx,[esp+0x20]
  40415d:	52                   	push   edx
  40415e:	51                   	push   ecx
  40415f:	6a 00                	push   0x0
  404161:	55                   	push   ebp
  404162:	ff 50 2c             	call   DWORD PTR [eax+0x2c]
  404165:	85 c0                	test   eax,eax
  404167:	0f 8c f2 00 00 00    	jl     0x40425f
  40416d:	8b 4e 0c             	mov    ecx,DWORD PTR [esi+0xc]
  404170:	e8 7b 07 00 00       	call   0x4048f0
  404175:	8b 44 24 0c          	mov    eax,DWORD PTR [esp+0xc]
  404179:	8b 4c 24 10          	mov    ecx,DWORD PTR [esp+0x10]
  40417d:	8d 54 24 14          	lea    edx,[esp+0x14]
  404181:	52                   	push   edx
  404182:	50                   	push   eax
  404183:	51                   	push   ecx
  404184:	8b 4e 0c             	mov    ecx,DWORD PTR [esi+0xc]
  404187:	e8 24 08 00 00       	call   0x4049b0
  40418c:	85 c0                	test   eax,eax
  40418e:	0f 8c cb 00 00 00    	jl     0x40425f
  404194:	8b 7c 24 14          	mov    edi,DWORD PTR [esp+0x14]
  404198:	8b 4c 24 0c          	mov    ecx,DWORD PTR [esp+0xc]
  40419c:	85 ff                	test   edi,edi
  40419e:	75 1e                	jne    0x4041be
  4041a0:	8b 56 0c             	mov    edx,DWORD PTR [esi+0xc]
  4041a3:	8b 7c 24 10          	mov    edi,DWORD PTR [esp+0x10]
  4041a7:	8b 02                	mov    eax,DWORD PTR [edx]
  4041a9:	66 83 78 0e 08       	cmp    WORD PTR [eax+0xe],0x8
  4041ae:	0f 95 c0             	setne  al
  4041b1:	48                   	dec    eax
  4041b2:	25 80 00 00 00       	and    eax,0x80
  4041b7:	25 ff 00 00 00       	and    eax,0xff
  4041bc:	eb 70                	jmp    0x40422e
  4041be:	3b f9                	cmp    edi,ecx
  4041c0:	0f 83 82 00 00 00    	jae    0x404248
  4041c6:	8b 44 24 20          	mov    eax,DWORD PTR [esp+0x20]
  4041ca:	85 c0                	test   eax,eax
  4041cc:	74 40                	je     0x40420e
  4041ce:	8b 4e 0c             	mov    ecx,DWORD PTR [esi+0xc]
  4041d1:	e8 1a 07 00 00       	call   0x4048f0
  4041d6:	85 c0                	test   eax,eax
  4041d8:	0f 8c 81 00 00 00    	jl     0x40425f
  4041de:	8b 4c 24 0c          	mov    ecx,DWORD PTR [esp+0xc]
  4041e2:	8b 54 24 10          	mov    edx,DWORD PTR [esp+0x10]
  4041e6:	8d 44 24 14          	lea    eax,[esp+0x14]
  4041ea:	2b cf                	sub    ecx,edi
  4041ec:	50                   	push   eax
  4041ed:	8d 04 17             	lea    eax,[edi+edx*1]
  4041f0:	51                   	push   ecx
  4041f1:	8b 4e 0c             	mov    ecx,DWORD PTR [esi+0xc]
  4041f4:	50                   	push   eax
  4041f5:	e8 b6 07 00 00       	call   0x4049b0
  4041fa:	85 c0                	test   eax,eax
  4041fc:	7c 61                	jl     0x40425f
  4041fe:	8b 4c 24 14          	mov    ecx,DWORD PTR [esp+0x14]
  404202:	8b 44 24 0c          	mov    eax,DWORD PTR [esp+0xc]
  404206:	03 f9                	add    edi,ecx
  404208:	3b f8                	cmp    edi,eax
  40420a:	72 c2                	jb     0x4041ce
  40420c:	eb 3a                	jmp    0x404248
  40420e:	8b 56 0c             	mov    edx,DWORD PTR [esi+0xc]
  404211:	2b cf                	sub    ecx,edi
  404213:	8b 02                	mov    eax,DWORD PTR [edx]
  404215:	8b 54 24 10          	mov    edx,DWORD PTR [esp+0x10]
  404219:	66 83 78 0e 08       	cmp    WORD PTR [eax+0xe],0x8
  40421e:	0f 95 c0             	setne  al
  404221:	48                   	dec    eax
  404222:	25 80 00 00 00       	and    eax,0x80
  404227:	25 ff 00 00 00       	and    eax,0xff
  40422c:	03 fa                	add    edi,edx
  40422e:	8a d0                	mov    dl,al
  404230:	8b f1                	mov    esi,ecx
  404232:	8a f2                	mov    dh,dl
  404234:	8b c2                	mov    eax,edx
  404236:	c1 e0 10             	shl    eax,0x10
  404239:	66 8b c2             	mov    ax,dx
  40423c:	c1 e9 02             	shr    ecx,0x2
  40423f:	f3 ab                	rep stos DWORD PTR es:[edi],eax
  404241:	8b ce                	mov    ecx,esi
  404243:	83 e1 03             	and    ecx,0x3
  404246:	f3 aa                	rep stos BYTE PTR es:[edi],al
  404248:	8b 4c 24 0c          	mov    ecx,DWORD PTR [esp+0xc]
  40424c:	8b 54 24 10          	mov    edx,DWORD PTR [esp+0x10]
  404250:	8b 45 00             	mov    eax,DWORD PTR [ebp+0x0]
  404253:	6a 00                	push   0x0
  404255:	6a 00                	push   0x0
  404257:	51                   	push   ecx
  404258:	52                   	push   edx
  404259:	55                   	push   ebp
  40425a:	ff 50 4c             	call   DWORD PTR [eax+0x4c]
  40425d:	33 c0                	xor    eax,eax
  40425f:	5f                   	pop    edi
  404260:	5e                   	pop    esi
  404261:	5d                   	pop    ebp
  404262:	83 c4 0c             	add    esp,0xc
  404265:	c2 08 00             	ret    0x8
