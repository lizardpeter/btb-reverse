
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00403080 <.text+0x2080>:
  403080:	81 ec 8c 00 00 00    	sub    esp,0x8c
  403086:	53                   	push   ebx
  403087:	55                   	push   ebp
  403088:	56                   	push   esi
  403089:	8b f1                	mov    esi,ecx
  40308b:	57                   	push   edi
  40308c:	8b 06                	mov    eax,DWORD PTR [esi]
  40308e:	ff 10                	call   DWORD PTR [eax]
  403090:	8d 5e 04             	lea    ebx,[esi+0x4]
  403093:	6a 00                	push   0x0
  403095:	68 68 b5 43 00       	push   0x43b568
  40309a:	53                   	push   ebx
  40309b:	6a 00                	push   0x0
  40309d:	e8 2e ba 02 00       	call   0x42ead0
  4030a2:	85 c0                	test   eax,eax
  4030a4:	7d 12                	jge    0x4030b8
  4030a6:	5f                   	pop    edi
  4030a7:	5e                   	pop    esi
  4030a8:	5d                   	pop    ebp
  4030a9:	b8 05 40 00 80       	mov    eax,0x80004005
  4030ae:	5b                   	pop    ebx
  4030af:	81 c4 8c 00 00 00    	add    esp,0x8c
  4030b5:	c2 10 00             	ret    0x10
  4030b8:	8b 03                	mov    eax,DWORD PTR [ebx]
  4030ba:	8b ac 24 a0 00 00 00 	mov    ebp,DWORD PTR [esp+0xa0]
  4030c1:	6a 11                	push   0x11
  4030c3:	55                   	push   ebp
  4030c4:	8b 08                	mov    ecx,DWORD PTR [eax]
  4030c6:	50                   	push   eax
  4030c7:	ff 51 50             	call   DWORD PTR [ecx+0x50]
  4030ca:	85 c0                	test   eax,eax
  4030cc:	7d 12                	jge    0x4030e0
  4030ce:	5f                   	pop    edi
  4030cf:	5e                   	pop    esi
  4030d0:	5d                   	pop    ebp
  4030d1:	b8 05 40 00 80       	mov    eax,0x80004005
  4030d6:	5b                   	pop    ebx
  4030d7:	81 c4 8c 00 00 00    	add    esp,0x8c
  4030dd:	c2 10 00             	ret    0x10
  4030e0:	8b 8c 24 ac 00 00 00 	mov    ecx,DWORD PTR [esp+0xac]
  4030e7:	8b 03                	mov    eax,DWORD PTR [ebx]
  4030e9:	6a 00                	push   0x0
  4030eb:	6a 00                	push   0x0
  4030ed:	8b 10                	mov    edx,DWORD PTR [eax]
  4030ef:	51                   	push   ecx
  4030f0:	8b 8c 24 b4 00 00 00 	mov    ecx,DWORD PTR [esp+0xb4]
  4030f7:	51                   	push   ecx
  4030f8:	8b 8c 24 b4 00 00 00 	mov    ecx,DWORD PTR [esp+0xb4]
  4030ff:	51                   	push   ecx
  403100:	50                   	push   eax
  403101:	ff 52 54             	call   DWORD PTR [edx+0x54]
  403104:	85 c0                	test   eax,eax
  403106:	7d 12                	jge    0x40311a
  403108:	5f                   	pop    edi
  403109:	5e                   	pop    esi
  40310a:	5d                   	pop    ebp
  40310b:	b8 05 40 00 80       	mov    eax,0x80004005
  403110:	5b                   	pop    ebx
  403111:	81 c4 8c 00 00 00    	add    esp,0x8c
  403117:	c2 10 00             	ret    0x10
  40311a:	8b 1b                	mov    ebx,DWORD PTR [ebx]
  40311c:	b9 1f 00 00 00       	mov    ecx,0x1f
  403121:	33 c0                	xor    eax,eax
  403123:	8d 7c 24 20          	lea    edi,[esp+0x20]
  403127:	f3 ab                	rep stos DWORD PTR es:[edi],eax
  403129:	8d 7e 08             	lea    edi,[esi+0x8]
  40312c:	50                   	push   eax
  40312d:	8d 44 24 24          	lea    eax,[esp+0x24]
  403131:	c7 44 24 24 7c 00 00 	mov    DWORD PTR [esp+0x24],0x7c
  403138:	00 
  403139:	c7 44 24 28 21 00 00 	mov    DWORD PTR [esp+0x28],0x21
  403140:	00 
  403141:	c7 84 24 8c 00 00 00 	mov    DWORD PTR [esp+0x8c],0x2218
  403148:	18 22 00 00 
  40314c:	c7 44 24 38 01 00 00 	mov    DWORD PTR [esp+0x38],0x1
  403153:	00 
  403154:	8b 13                	mov    edx,DWORD PTR [ebx]
  403156:	57                   	push   edi
  403157:	50                   	push   eax
  403158:	53                   	push   ebx
  403159:	ff 52 18             	call   DWORD PTR [edx+0x18]
  40315c:	85 c0                	test   eax,eax
  40315e:	7d 12                	jge    0x403172
  403160:	5f                   	pop    edi
  403161:	5e                   	pop    esi
  403162:	5d                   	pop    ebp
  403163:	b8 05 40 00 80       	mov    eax,0x80004005
  403168:	5b                   	pop    ebx
  403169:	81 c4 8c 00 00 00    	add    esp,0x8c
  40316f:	c2 10 00             	ret    0x10
  403172:	8b 3f                	mov    edi,DWORD PTR [edi]
  403174:	33 c9                	xor    ecx,ecx
  403176:	89 4c 24 10          	mov    DWORD PTR [esp+0x10],ecx
  40317a:	8d 5e 0c             	lea    ebx,[esi+0xc]
  40317d:	89 4c 24 14          	mov    DWORD PTR [esp+0x14],ecx
  403181:	8d 44 24 10          	lea    eax,[esp+0x10]
  403185:	89 4c 24 18          	mov    DWORD PTR [esp+0x18],ecx
  403189:	c7 44 24 10 04 00 00 	mov    DWORD PTR [esp+0x10],0x4
  403190:	00 
  403191:	89 4c 24 1c          	mov    DWORD PTR [esp+0x1c],ecx
  403195:	8b 17                	mov    edx,DWORD PTR [edi]
  403197:	53                   	push   ebx
  403198:	50                   	push   eax
  403199:	57                   	push   edi
  40319a:	ff 52 30             	call   DWORD PTR [edx+0x30]
  40319d:	85 c0                	test   eax,eax
  40319f:	7d 12                	jge    0x4031b3
  4031a1:	5f                   	pop    edi
  4031a2:	5e                   	pop    esi
  4031a3:	5d                   	pop    ebp
  4031a4:	b8 05 40 00 80       	mov    eax,0x80004005
  4031a9:	5b                   	pop    ebx
  4031aa:	81 c4 8c 00 00 00    	add    esp,0x8c
  4031b0:	c2 10 00             	ret    0x10
  4031b3:	8b 03                	mov    eax,DWORD PTR [ebx]
  4031b5:	50                   	push   eax
  4031b6:	8b 08                	mov    ecx,DWORD PTR [eax]
  4031b8:	ff 51 04             	call   DWORD PTR [ecx+0x4]
  4031bb:	8b ce                	mov    ecx,esi
  4031bd:	89 6e 14             	mov    DWORD PTR [esi+0x14],ebp
  4031c0:	c7 46 28 00 00 00 00 	mov    DWORD PTR [esi+0x28],0x0
  4031c7:	e8 c4 03 00 00       	call   0x403590
  4031cc:	5f                   	pop    edi
  4031cd:	5e                   	pop    esi
  4031ce:	5d                   	pop    ebp
  4031cf:	33 c0                	xor    eax,eax
  4031d1:	5b                   	pop    ebx
  4031d2:	81 c4 8c 00 00 00    	add    esp,0x8c
  4031d8:	c2 10 00             	ret    0x10
