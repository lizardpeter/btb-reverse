
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00437b3b <.text+0x36b3b>:
  437b3b:	55                   	push   ebp
  437b3c:	8b ec                	mov    ebp,esp
  437b3e:	51                   	push   ecx
  437b3f:	8b 55 0c             	mov    edx,DWORD PTR [ebp+0xc]
  437b42:	53                   	push   ebx
  437b43:	56                   	push   esi
  437b44:	57                   	push   edi
  437b45:	66 8b 42 06          	mov    ax,WORD PTR [edx+0x6]
  437b49:	bf ff 07 00 00       	mov    edi,0x7ff
  437b4e:	8b c8                	mov    ecx,eax
  437b50:	25 00 80 00 00       	and    eax,0x8000
  437b55:	c1 e9 04             	shr    ecx,0x4
  437b58:	23 cf                	and    ecx,edi
  437b5a:	89 45 0c             	mov    DWORD PTR [ebp+0xc],eax
  437b5d:	8b 42 04             	mov    eax,DWORD PTR [edx+0x4]
  437b60:	8b 12                	mov    edx,DWORD PTR [edx]
  437b62:	0f b7 d9             	movzx  ebx,cx
  437b65:	be 00 00 00 80       	mov    esi,0x80000000
  437b6a:	25 ff ff 0f 00       	and    eax,0xfffff
  437b6f:	85 db                	test   ebx,ebx
  437b71:	89 75 fc             	mov    DWORD PTR [ebp-0x4],esi
  437b74:	74 13                	je     0x437b89
  437b76:	3b df                	cmp    ebx,edi
  437b78:	74 08                	je     0x437b82
  437b7a:	8d b9 00 3c 00 00    	lea    edi,[ecx+0x3c00]
  437b80:	eb 28                	jmp    0x437baa
  437b82:	bf ff 7f 00 00       	mov    edi,0x7fff
  437b87:	eb 21                	jmp    0x437baa
  437b89:	33 db                	xor    ebx,ebx
  437b8b:	3b c3                	cmp    eax,ebx
  437b8d:	75 12                	jne    0x437ba1
  437b8f:	3b d3                	cmp    edx,ebx
  437b91:	75 0e                	jne    0x437ba1
  437b93:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  437b96:	89 58 04             	mov    DWORD PTR [eax+0x4],ebx
  437b99:	89 18                	mov    DWORD PTR [eax],ebx
  437b9b:	66 89 58 08          	mov    WORD PTR [eax+0x8],bx
  437b9f:	eb 4b                	jmp    0x437bec
  437ba1:	8d b9 01 3c 00 00    	lea    edi,[ecx+0x3c01]
  437ba7:	89 5d fc             	mov    DWORD PTR [ebp-0x4],ebx
  437baa:	8b ca                	mov    ecx,edx
  437bac:	c1 e9 15             	shr    ecx,0x15
  437baf:	c1 e0 0b             	shl    eax,0xb
  437bb2:	0b c8                	or     ecx,eax
  437bb4:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  437bb7:	0b 4d fc             	or     ecx,DWORD PTR [ebp-0x4]
  437bba:	c1 e2 0b             	shl    edx,0xb
  437bbd:	89 48 04             	mov    DWORD PTR [eax+0x4],ecx
  437bc0:	89 10                	mov    DWORD PTR [eax],edx
  437bc2:	85 ce                	test   esi,ecx
  437bc4:	75 1d                	jne    0x437be3
  437bc6:	8b 10                	mov    edx,DWORD PTR [eax]
  437bc8:	03 c9                	add    ecx,ecx
  437bca:	8b da                	mov    ebx,edx
  437bcc:	c1 eb 1f             	shr    ebx,0x1f
  437bcf:	0b d9                	or     ebx,ecx
  437bd1:	8d 0c 12             	lea    ecx,[edx+edx*1]
  437bd4:	89 08                	mov    DWORD PTR [eax],ecx
  437bd6:	89 58 04             	mov    DWORD PTR [eax+0x4],ebx
  437bd9:	81 c7 ff ff 00 00    	add    edi,0xffff
  437bdf:	8b cb                	mov    ecx,ebx
  437be1:	eb df                	jmp    0x437bc2
  437be3:	8b 4d 0c             	mov    ecx,DWORD PTR [ebp+0xc]
  437be6:	0b cf                	or     ecx,edi
  437be8:	66 89 48 08          	mov    WORD PTR [eax+0x8],cx
  437bec:	5f                   	pop    edi
  437bed:	5e                   	pop    esi
  437bee:	5b                   	pop    ebx
  437bef:	c9                   	leave
  437bf0:	c3                   	ret
