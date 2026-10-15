
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00403ff0 <.text+0x2ff0>:
  403ff0:	53                   	push   ebx
  403ff1:	8b 5c 24 10          	mov    ebx,DWORD PTR [esp+0x10]
  403ff5:	56                   	push   esi
  403ff6:	8b f1                	mov    esi,ecx
  403ff8:	8d 04 9d 00 00 00 00 	lea    eax,[ebx*4+0x0]
  403fff:	57                   	push   edi
  404000:	50                   	push   eax
  404001:	c7 06 e4 b2 43 00    	mov    DWORD PTR [esi],0x43b2e4
  404007:	e8 c2 bb 02 00       	call   0x42fbce
  40400c:	89 46 04             	mov    DWORD PTR [esi+0x4],eax
  40400f:	83 c4 04             	add    esp,0x4
  404012:	33 c0                	xor    eax,eax
  404014:	85 db                	test   ebx,ebx
  404016:	76 12                	jbe    0x40402a
  404018:	8b 4c 24 10          	mov    ecx,DWORD PTR [esp+0x10]
  40401c:	8b 56 04             	mov    edx,DWORD PTR [esi+0x4]
  40401f:	8b 3c 81             	mov    edi,DWORD PTR [ecx+eax*4]
  404022:	89 3c 82             	mov    DWORD PTR [edx+eax*4],edi
  404025:	40                   	inc    eax
  404026:	3b c3                	cmp    eax,ebx
  404028:	72 f2                	jb     0x40401c
  40402a:	8b 44 24 14          	mov    eax,DWORD PTR [esp+0x14]
  40402e:	8b 4c 24 1c          	mov    ecx,DWORD PTR [esp+0x1c]
  404032:	8b 56 04             	mov    edx,DWORD PTR [esi+0x4]
  404035:	89 46 08             	mov    DWORD PTR [esi+0x8],eax
  404038:	89 5e 10             	mov    DWORD PTR [esi+0x10],ebx
  40403b:	89 4e 0c             	mov    DWORD PTR [esi+0xc],ecx
  40403e:	8b 02                	mov    eax,DWORD PTR [edx]
  404040:	6a 00                	push   0x0
  404042:	50                   	push   eax
  404043:	8b ce                	mov    ecx,esi
  404045:	e8 b6 00 00 00       	call   0x404100
  40404a:	33 ff                	xor    edi,edi
  40404c:	85 db                	test   ebx,ebx
  40404e:	76 13                	jbe    0x404063
  404050:	8b 4e 04             	mov    ecx,DWORD PTR [esi+0x4]
  404053:	6a 00                	push   0x0
  404055:	8b 04 b9             	mov    eax,DWORD PTR [ecx+edi*4]
  404058:	50                   	push   eax
  404059:	8b 10                	mov    edx,DWORD PTR [eax]
  40405b:	ff 52 34             	call   DWORD PTR [edx+0x34]
  40405e:	47                   	inc    edi
  40405f:	3b fb                	cmp    edi,ebx
  404061:	72 ed                	jb     0x404050
  404063:	8b c6                	mov    eax,esi
  404065:	5f                   	pop    edi
  404066:	5e                   	pop    esi
  404067:	5b                   	pop    ebx
  404068:	c2 10 00             	ret    0x10
