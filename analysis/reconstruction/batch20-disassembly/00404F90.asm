
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00404f90 <.text+0x3f90>:
  404f90:	64 a1 00 00 00 00    	mov    eax,fs:0x0
  404f96:	6a ff                	push   0xffffffff
  404f98:	68 f0 a3 43 00       	push   0x43a3f0
  404f9d:	50                   	push   eax
  404f9e:	64 89 25 00 00 00 00 	mov    DWORD PTR fs:0x0,esp
  404fa5:	83 ec 58             	sub    esp,0x58
  404fa8:	53                   	push   ebx
  404fa9:	56                   	push   esi
  404faa:	8b f1                	mov    esi,ecx
  404fac:	33 db                	xor    ebx,ebx
  404fae:	57                   	push   edi
  404faf:	39 5e 04             	cmp    DWORD PTR [esi+0x4],ebx
  404fb2:	0f 85 93 00 00 00    	jne    0x40504b
  404fb8:	8a 44 24 74          	mov    al,BYTE PTR [esp+0x74]
  404fbc:	bf c4 e1 43 00       	mov    edi,0x43e1c4
  404fc1:	88 44 24 1c          	mov    BYTE PTR [esp+0x1c],al
  404fc5:	83 c9 ff             	or     ecx,0xffffffff
  404fc8:	33 c0                	xor    eax,eax
  404fca:	89 5c 24 20          	mov    DWORD PTR [esp+0x20],ebx
  404fce:	f2 ae                	repnz scas al,BYTE PTR es:[edi]
  404fd0:	f7 d1                	not    ecx
  404fd2:	49                   	dec    ecx
  404fd3:	89 5c 24 24          	mov    DWORD PTR [esp+0x24],ebx
  404fd7:	51                   	push   ecx
  404fd8:	68 c4 e1 43 00       	push   0x43e1c4
  404fdd:	8d 4c 24 24          	lea    ecx,[esp+0x24]
  404fe1:	89 5c 24 30          	mov    DWORD PTR [esp+0x30],ebx
  404fe5:	e8 b6 13 00 00       	call   0x4063a0
  404fea:	8d 4c 24 74          	lea    ecx,[esp+0x74]
  404fee:	89 5c 24 6c          	mov    DWORD PTR [esp+0x6c],ebx
  404ff2:	51                   	push   ecx
  404ff3:	8d 4c 24 30          	lea    ecx,[esp+0x30]
  404ff7:	c7 44 24 78 74 25 48 	mov    DWORD PTR [esp+0x78],0x482574
  404ffe:	00 
  404fff:	e8 b6 b3 02 00       	call   0x4303ba
  405004:	8a 54 24 1c          	mov    dl,BYTE PTR [esp+0x1c]
  405008:	53                   	push   ebx
  405009:	8d 4c 24 3c          	lea    ecx,[esp+0x3c]
  40500d:	c6 44 24 70 01       	mov    BYTE PTR [esp+0x70],0x1
  405012:	88 54 24 3c          	mov    BYTE PTR [esp+0x3c],dl
  405016:	e8 35 13 00 00       	call   0x406350
  40501b:	a1 34 b3 43 00       	mov    eax,ds:0x43b334
  405020:	8d 4c 24 1c          	lea    ecx,[esp+0x1c]
  405024:	50                   	push   eax
  405025:	53                   	push   ebx
  405026:	51                   	push   ecx
  405027:	8d 4c 24 44          	lea    ecx,[esp+0x44]
  40502b:	e8 20 11 00 00       	call   0x406150
  405030:	8d 54 24 2c          	lea    edx,[esp+0x2c]
  405034:	68 d0 bd 43 00       	push   0x43bdd0
  405039:	52                   	push   edx
  40503a:	c7 44 24 34 28 b3 43 	mov    DWORD PTR [esp+0x34],0x43b328
  405041:	00 
  405042:	88 5c 24 74          	mov    BYTE PTR [esp+0x74],bl
  405046:	e8 4a b4 02 00       	call   0x430495
  40504b:	8b 7c 24 74          	mov    edi,DWORD PTR [esp+0x74]
  40504f:	3b fb                	cmp    edi,ebx
  405051:	0f 85 bf 00 00 00    	jne    0x405116
  405057:	8a 44 24 74          	mov    al,BYTE PTR [esp+0x74]
  40505b:	55                   	push   ebp
  40505c:	53                   	push   ebx
  40505d:	8d 4c 24 14          	lea    ecx,[esp+0x14]
  405061:	88 44 24 14          	mov    BYTE PTR [esp+0x14],al
  405065:	e8 e6 12 00 00       	call   0x406350
  40506a:	bf b0 e1 43 00       	mov    edi,0x43e1b0
  40506f:	83 c9 ff             	or     ecx,0xffffffff
  405072:	33 c0                	xor    eax,eax
  405074:	6a 01                	push   0x1
  405076:	f2 ae                	repnz scas al,BYTE PTR es:[edi]
  405078:	f7 d1                	not    ecx
  40507a:	49                   	dec    ecx
  40507b:	8b e9                	mov    ebp,ecx
  40507d:	8d 4c 24 14          	lea    ecx,[esp+0x14]
  405081:	55                   	push   ebp
  405082:	e8 d9 13 00 00       	call   0x406460
  405087:	84 c0                	test   al,al
  405089:	74 24                	je     0x4050af
  40508b:	8b 7c 24 14          	mov    edi,DWORD PTR [esp+0x14]
  40508f:	8b cd                	mov    ecx,ebp
  405091:	8b d1                	mov    edx,ecx
  405093:	be b0 e1 43 00       	mov    esi,0x43e1b0
  405098:	c1 e9 02             	shr    ecx,0x2
  40509b:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  40509d:	8b ca                	mov    ecx,edx
  40509f:	83 e1 03             	and    ecx,0x3
  4050a2:	f3 a4                	rep movs BYTE PTR es:[edi],BYTE PTR ds:[esi]
  4050a4:	8b 44 24 14          	mov    eax,DWORD PTR [esp+0x14]
  4050a8:	89 6c 24 18          	mov    DWORD PTR [esp+0x18],ebp
  4050ac:	88 1c 28             	mov    BYTE PTR [eax+ebp*1],bl
  4050af:	8d 4c 24 78          	lea    ecx,[esp+0x78]
  4050b3:	c7 44 24 70 02 00 00 	mov    DWORD PTR [esp+0x70],0x2
  4050ba:	00 
  4050bb:	51                   	push   ecx
  4050bc:	8d 4c 24 50          	lea    ecx,[esp+0x50]
  4050c0:	c7 44 24 7c 74 25 48 	mov    DWORD PTR [esp+0x7c],0x482574
  4050c7:	00 
  4050c8:	e8 ed b2 02 00       	call   0x4303ba
  4050cd:	8a 54 24 10          	mov    dl,BYTE PTR [esp+0x10]
  4050d1:	53                   	push   ebx
  4050d2:	8d 4c 24 5c          	lea    ecx,[esp+0x5c]
  4050d6:	c6 44 24 74 03       	mov    BYTE PTR [esp+0x74],0x3
  4050db:	88 54 24 5c          	mov    BYTE PTR [esp+0x5c],dl
  4050df:	e8 6c 12 00 00       	call   0x406350
  4050e4:	a1 34 b3 43 00       	mov    eax,ds:0x43b334
  4050e9:	8d 4c 24 10          	lea    ecx,[esp+0x10]
  4050ed:	50                   	push   eax
  4050ee:	53                   	push   ebx
  4050ef:	51                   	push   ecx
  4050f0:	8d 4c 24 64          	lea    ecx,[esp+0x64]
  4050f4:	e8 57 10 00 00       	call   0x406150
  4050f9:	8d 54 24 4c          	lea    edx,[esp+0x4c]
  4050fd:	68 d0 bd 43 00       	push   0x43bdd0
  405102:	52                   	push   edx
  405103:	c7 44 24 54 28 b3 43 	mov    DWORD PTR [esp+0x54],0x43b328
  40510a:	00 
  40510b:	c6 44 24 78 02       	mov    BYTE PTR [esp+0x78],0x2
  405110:	e8 80 b3 02 00       	call   0x430495
  405115:	5d                   	pop    ebp
  405116:	8b 06                	mov    eax,DWORD PTR [esi]
  405118:	57                   	push   edi
  405119:	8b ce                	mov    ecx,esi
  40511b:	ff 50 24             	call   DWORD PTR [eax+0x24]
  40511e:	8b 16                	mov    edx,DWORD PTR [esi]
  405120:	57                   	push   edi
  405121:	8b ce                	mov    ecx,esi
  405123:	ff 52 08             	call   DWORD PTR [edx+0x8]
  405126:	8b 4c 24 64          	mov    ecx,DWORD PTR [esp+0x64]
  40512a:	5f                   	pop    edi
  40512b:	5e                   	pop    esi
  40512c:	5b                   	pop    ebx
  40512d:	64 89 0d 00 00 00 00 	mov    DWORD PTR fs:0x0,ecx
  405134:	83 c4 64             	add    esp,0x64
  405137:	c2                   	.byte 0xc2
  405138:	04                   	.byte 0x4
