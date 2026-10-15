
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00434aa1 <.text+0x33aa1>:
  434aa1:	a1 34 da 51 00       	mov    eax,ds:0x51da34
  434aa6:	8b 0d 24 da 51 00    	mov    ecx,DWORD PTR ds:0x51da24
  434aac:	56                   	push   esi
  434aad:	57                   	push   edi
  434aae:	33 ff                	xor    edi,edi
  434ab0:	3b c1                	cmp    eax,ecx
  434ab2:	75 30                	jne    0x434ae4
  434ab4:	8d 44 89 50          	lea    eax,[ecx+ecx*4+0x50]
  434ab8:	c1 e0 02             	shl    eax,0x2
  434abb:	50                   	push   eax
  434abc:	ff 35 38 da 51 00    	push   DWORD PTR ds:0x51da38
  434ac2:	57                   	push   edi
  434ac3:	ff 35 40 da 51 00    	push   DWORD PTR ds:0x51da40
  434ac9:	ff 15 d4 b0 43 00    	call   DWORD PTR ds:0x43b0d4
  434acf:	3b c7                	cmp    eax,edi
  434ad1:	74 61                	je     0x434b34
  434ad3:	83 05 24 da 51 00 10 	add    DWORD PTR ds:0x51da24,0x10
  434ada:	a3 38 da 51 00       	mov    ds:0x51da38,eax
  434adf:	a1 34 da 51 00       	mov    eax,ds:0x51da34
  434ae4:	8b 0d 38 da 51 00    	mov    ecx,DWORD PTR ds:0x51da38
  434aea:	68 c4 41 00 00       	push   0x41c4
  434aef:	6a 08                	push   0x8
  434af1:	8d 04 80             	lea    eax,[eax+eax*4]
  434af4:	ff 35 40 da 51 00    	push   DWORD PTR ds:0x51da40
  434afa:	8d 34 81             	lea    esi,[ecx+eax*4]
  434afd:	ff 15 08 b1 43 00    	call   DWORD PTR ds:0x43b108
  434b03:	3b c7                	cmp    eax,edi
  434b05:	89 46 10             	mov    DWORD PTR [esi+0x10],eax
  434b08:	74 2a                	je     0x434b34
  434b0a:	6a 04                	push   0x4
  434b0c:	68 00 20 00 00       	push   0x2000
  434b11:	68 00 00 10 00       	push   0x100000
  434b16:	57                   	push   edi
  434b17:	ff 15 d8 b0 43 00    	call   DWORD PTR ds:0x43b0d8
  434b1d:	3b c7                	cmp    eax,edi
  434b1f:	89 46 0c             	mov    DWORD PTR [esi+0xc],eax
  434b22:	75 14                	jne    0x434b38
  434b24:	ff 76 10             	push   DWORD PTR [esi+0x10]
  434b27:	57                   	push   edi
  434b28:	ff 35 40 da 51 00    	push   DWORD PTR ds:0x51da40
  434b2e:	ff 15 0c b1 43 00    	call   DWORD PTR ds:0x43b10c
  434b34:	33 c0                	xor    eax,eax
  434b36:	eb 17                	jmp    0x434b4f
  434b38:	83 4e 08 ff          	or     DWORD PTR [esi+0x8],0xffffffff
  434b3c:	89 3e                	mov    DWORD PTR [esi],edi
  434b3e:	89 7e 04             	mov    DWORD PTR [esi+0x4],edi
  434b41:	ff 05 34 da 51 00    	inc    DWORD PTR ds:0x51da34
  434b47:	8b 46 10             	mov    eax,DWORD PTR [esi+0x10]
  434b4a:	83 08 ff             	or     DWORD PTR [eax],0xffffffff
  434b4d:	8b c6                	mov    eax,esi
  434b4f:	5f                   	pop    edi
  434b50:	5e                   	pop    esi
  434b51:	c3                   	ret
