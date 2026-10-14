
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00430884 <.text+0x2f884>:
  430884:	57                   	push   edi
  430885:	6a 01                	push   0x1
  430887:	5f                   	pop    edi
  430888:	39 3d 10 c4 51 00    	cmp    DWORD PTR ds:0x51c410,edi
  43088e:	75 11                	jne    0x4308a1
  430890:	ff 74 24 08          	push   DWORD PTR [esp+0x8]
  430894:	ff 15 10 b1 43 00    	call   DWORD PTR ds:0x43b110
  43089a:	50                   	push   eax
  43089b:	ff 15 14 b1 43 00    	call   DWORD PTR ds:0x43b114
  4308a1:	83 7c 24 0c 00       	cmp    DWORD PTR [esp+0xc],0x0
  4308a6:	53                   	push   ebx
  4308a7:	8b 5c 24 14          	mov    ebx,DWORD PTR [esp+0x14]
  4308ab:	89 3d 0c c4 51 00    	mov    DWORD PTR ds:0x51c40c,edi
  4308b1:	88 1d 08 c4 51 00    	mov    BYTE PTR ds:0x51c408,bl
  4308b7:	75 3c                	jne    0x4308f5
  4308b9:	a1 58 da 51 00       	mov    eax,ds:0x51da58
  4308be:	85 c0                	test   eax,eax
  4308c0:	74 22                	je     0x4308e4
  4308c2:	8b 0d 54 da 51 00    	mov    ecx,DWORD PTR ds:0x51da54
  4308c8:	56                   	push   esi
  4308c9:	8d 71 fc             	lea    esi,[ecx-0x4]
  4308cc:	3b f0                	cmp    esi,eax
  4308ce:	72 13                	jb     0x4308e3
  4308d0:	8b 06                	mov    eax,DWORD PTR [esi]
  4308d2:	85 c0                	test   eax,eax
  4308d4:	74 02                	je     0x4308d8
  4308d6:	ff d0                	call   eax
  4308d8:	83 ee 04             	sub    esi,0x4
  4308db:	3b 35 58 da 51 00    	cmp    esi,DWORD PTR ds:0x51da58
  4308e1:	73 ed                	jae    0x4308d0
  4308e3:	5e                   	pop    esi
  4308e4:	68 2c e0 43 00       	push   0x43e02c
  4308e9:	68 24 e0 43 00       	push   0x43e024
  4308ee:	e8 2a 00 00 00       	call   0x43091d
  4308f3:	59                   	pop    ecx
  4308f4:	59                   	pop    ecx
  4308f5:	68 38 e0 43 00       	push   0x43e038
  4308fa:	68 30 e0 43 00       	push   0x43e030
  4308ff:	e8 19 00 00 00       	call   0x43091d
  430904:	59                   	pop    ecx
  430905:	59                   	pop    ecx
  430906:	85 db                	test   ebx,ebx
  430908:	5b                   	pop    ebx
  430909:	75 10                	jne    0x43091b
  43090b:	ff 74 24 08          	push   DWORD PTR [esp+0x8]
  43090f:	89 3d 10 c4 51 00    	mov    DWORD PTR ds:0x51c410,edi
  430915:	ff 15 18 b1 43 00    	call   DWORD PTR ds:0x43b118
  43091b:	5f                   	pop    edi
  43091c:	c3                   	ret
