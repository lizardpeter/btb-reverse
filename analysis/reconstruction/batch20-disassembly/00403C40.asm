
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00403c40 <.text+0x2c40>:
  403c40:	83 ec 3c             	sub    esp,0x3c
  403c43:	8b 11                	mov    edx,DWORD PTR [ecx]
  403c45:	c7 44 24 00 00 00 00 	mov    DWORD PTR [esp+0x0],0x0
  403c4c:	00 
  403c4d:	85 d2                	test   edx,edx
  403c4f:	75 0b                	jne    0x403c5c
  403c51:	b8 f0 01 04 80       	mov    eax,0x800401f0
  403c56:	83 c4 3c             	add    esp,0x3c
  403c59:	c2 0c 00             	ret    0xc
  403c5c:	57                   	push   edi
  403c5d:	b9 09 00 00 00       	mov    ecx,0x9
  403c62:	33 c0                	xor    eax,eax
  403c64:	8d 7c 24 1c          	lea    edi,[esp+0x1c]
  403c68:	f3 ab                	rep stos DWORD PTR es:[edi],eax
  403c6a:	8d 4c 24 04          	lea    ecx,[esp+0x4]
  403c6e:	6a 00                	push   0x0
  403c70:	51                   	push   ecx
  403c71:	8d 4c 24 24          	lea    ecx,[esp+0x24]
  403c75:	c7 44 24 24 24 00 00 	mov    DWORD PTR [esp+0x24],0x24
  403c7c:	00 
  403c7d:	c7 44 24 28 81 00 00 	mov    DWORD PTR [esp+0x28],0x81
  403c84:	00 
  403c85:	89 44 24 2c          	mov    DWORD PTR [esp+0x2c],eax
  403c89:	89 44 24 34          	mov    DWORD PTR [esp+0x34],eax
  403c8d:	8b 02                	mov    eax,DWORD PTR [edx]
  403c8f:	51                   	push   ecx
  403c90:	52                   	push   edx
  403c91:	ff 50 0c             	call   DWORD PTR [eax+0xc]
  403c94:	85 c0                	test   eax,eax
  403c96:	5f                   	pop    edi
  403c97:	7c 77                	jl     0x403d10
  403c99:	8b 44 24 48          	mov    eax,DWORD PTR [esp+0x48]
  403c9d:	33 d2                	xor    edx,edx
  403c9f:	89 54 24 04          	mov    DWORD PTR [esp+0x4],edx
  403ca3:	8b 4c 24 40          	mov    ecx,DWORD PTR [esp+0x40]
  403ca7:	89 54 24 08          	mov    DWORD PTR [esp+0x8],edx
  403cab:	66 c7 44 24 04 01 00 	mov    WORD PTR [esp+0x4],0x1
  403cb2:	89 54 24 0c          	mov    DWORD PTR [esp+0xc],edx
  403cb6:	66 89 4c 24 06       	mov    WORD PTR [esp+0x6],cx
  403cbb:	89 54 24 10          	mov    DWORD PTR [esp+0x10],edx
  403cbf:	66 89 44 24 12       	mov    WORD PTR [esp+0x12],ax
  403cc4:	66 c1 e8 03          	shr    ax,0x3
  403cc8:	0f af c1             	imul   eax,ecx
  403ccb:	66 89 44 24 10       	mov    WORD PTR [esp+0x10],ax
  403cd0:	8b 44 24 10          	mov    eax,DWORD PTR [esp+0x10]
  403cd4:	66 89 54 24 14       	mov    WORD PTR [esp+0x14],dx
  403cd9:	8b 54 24 44          	mov    edx,DWORD PTR [esp+0x44]
  403cdd:	25 ff ff 00 00       	and    eax,0xffff
  403ce2:	89 54 24 08          	mov    DWORD PTR [esp+0x8],edx
  403ce6:	0f af c2             	imul   eax,edx
  403ce9:	89 44 24 0c          	mov    DWORD PTR [esp+0xc],eax
  403ced:	8b 44 24 00          	mov    eax,DWORD PTR [esp+0x0]
  403cf1:	8d 54 24 04          	lea    edx,[esp+0x4]
  403cf5:	8b 08                	mov    ecx,DWORD PTR [eax]
  403cf7:	52                   	push   edx
  403cf8:	50                   	push   eax
  403cf9:	ff 51 38             	call   DWORD PTR [ecx+0x38]
  403cfc:	85 c0                	test   eax,eax
  403cfe:	7c 10                	jl     0x403d10
  403d00:	8b 44 24 00          	mov    eax,DWORD PTR [esp+0x0]
  403d04:	85 c0                	test   eax,eax
  403d06:	74 06                	je     0x403d0e
  403d08:	8b 08                	mov    ecx,DWORD PTR [eax]
  403d0a:	50                   	push   eax
  403d0b:	ff 51 08             	call   DWORD PTR [ecx+0x8]
  403d0e:	33 c0                	xor    eax,eax
  403d10:	83 c4 3c             	add    esp,0x3c
  403d13:	c2 0c 00             	ret    0xc
