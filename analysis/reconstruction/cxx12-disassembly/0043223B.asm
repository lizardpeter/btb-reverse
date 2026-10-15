
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0043223b <.text+0x3123b>:
  43223b:	55                   	push   ebp
  43223c:	8b ec                	mov    ebp,esp
  43223e:	6a ff                	push   0xffffffff
  432240:	68 20 b6 43 00       	push   0x43b620
  432245:	68 38 5f 43 00       	push   0x435f38
  43224a:	64 a1 00 00 00 00    	mov    eax,fs:0x0
  432250:	50                   	push   eax
  432251:	64 89 25 00 00 00 00 	mov    DWORD PTR fs:0x0,esp
  432258:	83 ec 1c             	sub    esp,0x1c
  43225b:	53                   	push   ebx
  43225c:	56                   	push   esi
  43225d:	57                   	push   edi
  43225e:	89 65 e8             	mov    DWORD PTR [ebp-0x18],esp
  432261:	8b 45 18             	mov    eax,DWORD PTR [ebp+0x18]
  432264:	89 45 d4             	mov    DWORD PTR [ebp-0x2c],eax
  432267:	33 db                	xor    ebx,ebx
  432269:	89 5d dc             	mov    DWORD PTR [ebp-0x24],ebx
  43226c:	8b 75 0c             	mov    esi,DWORD PTR [ebp+0xc]
  43226f:	8b 4e fc             	mov    ecx,DWORD PTR [esi-0x4]
  432272:	89 4d d8             	mov    DWORD PTR [ebp-0x28],ecx
  432275:	8b 0d 20 c4 51 00    	mov    ecx,DWORD PTR ds:0x51c420
  43227b:	89 4d e4             	mov    DWORD PTR [ebp-0x1c],ecx
  43227e:	8b 0d 24 c4 51 00    	mov    ecx,DWORD PTR ds:0x51c424
  432284:	89 4d e0             	mov    DWORD PTR [ebp-0x20],ecx
  432287:	8b 7d 08             	mov    edi,DWORD PTR [ebp+0x8]
  43228a:	89 3d 20 c4 51 00    	mov    DWORD PTR ds:0x51c420,edi
  432290:	8b 4d 10             	mov    ecx,DWORD PTR [ebp+0x10]
  432293:	89 0d 24 c4 51 00    	mov    DWORD PTR ds:0x51c424,ecx
  432299:	89 5d fc             	mov    DWORD PTR [ebp-0x4],ebx
  43229c:	c7 45 fc 01 00 00 00 	mov    DWORD PTR [ebp-0x4],0x1
  4322a3:	ff 75 20             	push   DWORD PTR [ebp+0x20]
  4322a6:	ff 75 1c             	push   DWORD PTR [ebp+0x1c]
  4322a9:	50                   	push   eax
  4322aa:	ff 75 14             	push   DWORD PTR [ebp+0x14]
  4322ad:	56                   	push   esi
  4322ae:	e8 fb d9 ff ff       	call   0x42fcae
  4322b3:	83 c4 14             	add    esp,0x14
  4322b6:	89 45 d4             	mov    DWORD PTR [ebp-0x2c],eax
  4322b9:	89 5d fc             	mov    DWORD PTR [ebp-0x4],ebx
  4322bc:	83 4d fc ff          	or     DWORD PTR [ebp-0x4],0xffffffff
  4322c0:	e8 3c 00 00 00       	call   0x432301
  4322c5:	8b 45 d4             	mov    eax,DWORD PTR [ebp-0x2c]
  4322c8:	8b 4d f0             	mov    ecx,DWORD PTR [ebp-0x10]
  4322cb:	64 89 0d 00 00 00 00 	mov    DWORD PTR fs:0x0,ecx
  4322d2:	5f                   	pop    edi
  4322d3:	5e                   	pop    esi
  4322d4:	5b                   	pop    ebx
  4322d5:	c9                   	leave
  4322d6:	c3                   	ret
