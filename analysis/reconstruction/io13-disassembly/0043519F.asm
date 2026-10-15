
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0043519f <.text+0x3419f>:
  43519f:	8b 44 24 04          	mov    eax,DWORD PTR [esp+0x4]
  4351a3:	ba 48 77 44 00       	mov    edx,0x447748
  4351a8:	56                   	push   esi
  4351a9:	8b ca                	mov    ecx,edx
  4351ab:	3b 41 10             	cmp    eax,DWORD PTR [ecx+0x10]
  4351ae:	76 05                	jbe    0x4351b5
  4351b0:	3b 41 14             	cmp    eax,DWORD PTR [ecx+0x14]
  4351b3:	72 08                	jb     0x4351bd
  4351b5:	8b 09                	mov    ecx,DWORD PTR [ecx]
  4351b7:	3b ca                	cmp    ecx,edx
  4351b9:	74 37                	je     0x4351f2
  4351bb:	eb ee                	jmp    0x4351ab
  4351bd:	a8 0f                	test   al,0xf
  4351bf:	75 31                	jne    0x4351f2
  4351c1:	8b f0                	mov    esi,eax
  4351c3:	ba 00 01 00 00       	mov    edx,0x100
  4351c8:	81 e6 ff 0f 00 00    	and    esi,0xfff
  4351ce:	3b f2                	cmp    esi,edx
  4351d0:	72 20                	jb     0x4351f2
  4351d2:	8b 74 24 0c          	mov    esi,DWORD PTR [esp+0xc]
  4351d6:	89 0e                	mov    DWORD PTR [esi],ecx
  4351d8:	8b 74 24 10          	mov    esi,DWORD PTR [esp+0x10]
  4351dc:	8b c8                	mov    ecx,eax
  4351de:	66 81 e1 00 f0       	and    cx,0xf000
  4351e3:	2b c1                	sub    eax,ecx
  4351e5:	89 0e                	mov    DWORD PTR [esi],ecx
  4351e7:	2b c2                	sub    eax,edx
  4351e9:	5e                   	pop    esi
  4351ea:	c1 f8 04             	sar    eax,0x4
  4351ed:	8d 44 08 08          	lea    eax,[eax+ecx*1+0x8]
  4351f1:	c3                   	ret
  4351f2:	33 c0                	xor    eax,eax
  4351f4:	5e                   	pop    esi
  4351f5:	c3                   	ret
