
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00435a04 <.text+0x34a04>:
  435a04:	55                   	push   ebp
  435a05:	8b ec                	mov    ebp,esp
  435a07:	51                   	push   ecx
  435a08:	51                   	push   ecx
  435a09:	53                   	push   ebx
  435a0a:	33 db                	xor    ebx,ebx
  435a0c:	39 1d 50 da 51 00    	cmp    DWORD PTR ds:0x51da50,ebx
  435a12:	56                   	push   esi
  435a13:	57                   	push   edi
  435a14:	75 05                	jne    0x435a1b
  435a16:	e8 7c 16 00 00       	call   0x437097
  435a1b:	be 5c c4 51 00       	mov    esi,0x51c45c
  435a20:	68 04 01 00 00       	push   0x104
  435a25:	56                   	push   esi
  435a26:	53                   	push   ebx
  435a27:	ff 15 ec b0 43 00    	call   DWORD PTR ds:0x43b0ec
  435a2d:	a1 48 da 51 00       	mov    eax,ds:0x51da48
  435a32:	89 35 00 c4 51 00    	mov    DWORD PTR ds:0x51c400,esi
  435a38:	8b fe                	mov    edi,esi
  435a3a:	38 18                	cmp    BYTE PTR [eax],bl
  435a3c:	74 02                	je     0x435a40
  435a3e:	8b f8                	mov    edi,eax
  435a40:	8d 45 f8             	lea    eax,[ebp-0x8]
  435a43:	50                   	push   eax
  435a44:	8d 45 fc             	lea    eax,[ebp-0x4]
  435a47:	50                   	push   eax
  435a48:	53                   	push   ebx
  435a49:	53                   	push   ebx
  435a4a:	57                   	push   edi
  435a4b:	e8 4d 00 00 00       	call   0x435a9d
  435a50:	8b 45 f8             	mov    eax,DWORD PTR [ebp-0x8]
  435a53:	8b 4d fc             	mov    ecx,DWORD PTR [ebp-0x4]
  435a56:	8d 04 88             	lea    eax,[eax+ecx*4]
  435a59:	50                   	push   eax
  435a5a:	e8 34 b3 ff ff       	call   0x430d93
  435a5f:	8b f0                	mov    esi,eax
  435a61:	83 c4 18             	add    esp,0x18
  435a64:	3b f3                	cmp    esi,ebx
  435a66:	75 08                	jne    0x435a70
  435a68:	6a 08                	push   0x8
  435a6a:	e8 71 b6 ff ff       	call   0x4310e0
  435a6f:	59                   	pop    ecx
  435a70:	8d 45 f8             	lea    eax,[ebp-0x8]
  435a73:	50                   	push   eax
  435a74:	8d 45 fc             	lea    eax,[ebp-0x4]
  435a77:	50                   	push   eax
  435a78:	8b 45 fc             	mov    eax,DWORD PTR [ebp-0x4]
  435a7b:	8d 04 86             	lea    eax,[esi+eax*4]
  435a7e:	50                   	push   eax
  435a7f:	56                   	push   esi
  435a80:	57                   	push   edi
  435a81:	e8 17 00 00 00       	call   0x435a9d
  435a86:	8b 45 fc             	mov    eax,DWORD PTR [ebp-0x4]
  435a89:	83 c4 14             	add    esp,0x14
  435a8c:	48                   	dec    eax
  435a8d:	89 35 e8 c3 51 00    	mov    DWORD PTR ds:0x51c3e8,esi
  435a93:	5f                   	pop    edi
  435a94:	5e                   	pop    esi
  435a95:	a3 e4 c3 51 00       	mov    ds:0x51c3e4,eax
  435a9a:	5b                   	pop    ebx
  435a9b:	c9                   	leave
  435a9c:	c3                   	ret
