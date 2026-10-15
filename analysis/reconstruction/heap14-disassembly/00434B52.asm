
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00434b52 <.text+0x33b52>:
  434b52:	55                   	push   ebp
  434b53:	8b ec                	mov    ebp,esp
  434b55:	51                   	push   ecx
  434b56:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  434b59:	53                   	push   ebx
  434b5a:	56                   	push   esi
  434b5b:	57                   	push   edi
  434b5c:	8b 71 10             	mov    esi,DWORD PTR [ecx+0x10]
  434b5f:	8b 41 08             	mov    eax,DWORD PTR [ecx+0x8]
  434b62:	33 db                	xor    ebx,ebx
  434b64:	85 c0                	test   eax,eax
  434b66:	7c 05                	jl     0x434b6d
  434b68:	d1 e0                	shl    eax,1
  434b6a:	43                   	inc    ebx
  434b6b:	eb f7                	jmp    0x434b64
  434b6d:	8b c3                	mov    eax,ebx
  434b6f:	6a 3f                	push   0x3f
  434b71:	69 c0 04 02 00 00    	imul   eax,eax,0x204
  434b77:	5a                   	pop    edx
  434b78:	8d 84 30 44 01 00 00 	lea    eax,[eax+esi*1+0x144]
  434b7f:	89 45 fc             	mov    DWORD PTR [ebp-0x4],eax
  434b82:	89 40 08             	mov    DWORD PTR [eax+0x8],eax
  434b85:	89 40 04             	mov    DWORD PTR [eax+0x4],eax
  434b88:	83 c0 08             	add    eax,0x8
  434b8b:	4a                   	dec    edx
  434b8c:	75 f4                	jne    0x434b82
  434b8e:	8b fb                	mov    edi,ebx
  434b90:	6a 04                	push   0x4
  434b92:	c1 e7 0f             	shl    edi,0xf
  434b95:	03 79 0c             	add    edi,DWORD PTR [ecx+0xc]
  434b98:	68 00 10 00 00       	push   0x1000
  434b9d:	68 00 80 00 00       	push   0x8000
  434ba2:	57                   	push   edi
  434ba3:	ff 15 d8 b0 43 00    	call   DWORD PTR ds:0x43b0d8
  434ba9:	85 c0                	test   eax,eax
  434bab:	75 08                	jne    0x434bb5
  434bad:	83 c8 ff             	or     eax,0xffffffff
  434bb0:	e9 93 00 00 00       	jmp    0x434c48
  434bb5:	8d 97 00 70 00 00    	lea    edx,[edi+0x7000]
  434bbb:	3b fa                	cmp    edi,edx
  434bbd:	77 3c                	ja     0x434bfb
  434bbf:	8d 47 10             	lea    eax,[edi+0x10]
  434bc2:	83 48 f8 ff          	or     DWORD PTR [eax-0x8],0xffffffff
  434bc6:	83 88 ec 0f 00 00 ff 	or     DWORD PTR [eax+0xfec],0xffffffff
  434bcd:	8d 88 fc 0f 00 00    	lea    ecx,[eax+0xffc]
  434bd3:	c7 40 fc f0 0f 00 00 	mov    DWORD PTR [eax-0x4],0xff0
  434bda:	89 08                	mov    DWORD PTR [eax],ecx
  434bdc:	8d 88 fc ef ff ff    	lea    ecx,[eax-0x1004]
  434be2:	89 48 04             	mov    DWORD PTR [eax+0x4],ecx
  434be5:	c7 80 e8 0f 00 00 f0 	mov    DWORD PTR [eax+0xfe8],0xff0
  434bec:	0f 00 00 
  434bef:	05 00 10 00 00       	add    eax,0x1000
  434bf4:	8d 48 f0             	lea    ecx,[eax-0x10]
  434bf7:	3b ca                	cmp    ecx,edx
  434bf9:	76 c7                	jbe    0x434bc2
  434bfb:	8b 45 fc             	mov    eax,DWORD PTR [ebp-0x4]
  434bfe:	8d 4f 0c             	lea    ecx,[edi+0xc]
  434c01:	05 f8 01 00 00       	add    eax,0x1f8
  434c06:	6a 01                	push   0x1
  434c08:	5f                   	pop    edi
  434c09:	89 48 04             	mov    DWORD PTR [eax+0x4],ecx
  434c0c:	89 41 08             	mov    DWORD PTR [ecx+0x8],eax
  434c0f:	8d 4a 0c             	lea    ecx,[edx+0xc]
  434c12:	89 48 08             	mov    DWORD PTR [eax+0x8],ecx
  434c15:	89 41 04             	mov    DWORD PTR [ecx+0x4],eax
  434c18:	83 64 9e 44 00       	and    DWORD PTR [esi+ebx*4+0x44],0x0
  434c1d:	89 bc 9e c4 00 00 00 	mov    DWORD PTR [esi+ebx*4+0xc4],edi
  434c24:	8a 46 43             	mov    al,BYTE PTR [esi+0x43]
  434c27:	8a c8                	mov    cl,al
  434c29:	fe c1                	inc    cl
  434c2b:	84 c0                	test   al,al
  434c2d:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  434c30:	88 4e 43             	mov    BYTE PTR [esi+0x43],cl
  434c33:	75 03                	jne    0x434c38
  434c35:	09 78 04             	or     DWORD PTR [eax+0x4],edi
  434c38:	ba 00 00 00 80       	mov    edx,0x80000000
  434c3d:	8b cb                	mov    ecx,ebx
  434c3f:	d3 ea                	shr    edx,cl
  434c41:	f7 d2                	not    edx
  434c43:	21 50 08             	and    DWORD PTR [eax+0x8],edx
  434c46:	8b c3                	mov    eax,ebx
  434c48:	5f                   	pop    edi
  434c49:	5e                   	pop    esi
  434c4a:	5b                   	pop    ebx
  434c4b:	c9                   	leave
  434c4c:	c3                   	ret
