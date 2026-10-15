
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00434f43 <.text+0x33f43>:
  434f43:	83 3d 58 77 44 00 ff 	cmp    DWORD PTR ds:0x447758,0xffffffff
  434f4a:	53                   	push   ebx
  434f4b:	55                   	push   ebp
  434f4c:	56                   	push   esi
  434f4d:	57                   	push   edi
  434f4e:	75 07                	jne    0x434f57
  434f50:	be 48 77 44 00       	mov    esi,0x447748
  434f55:	eb 1d                	jmp    0x434f74
  434f57:	68 20 20 00 00       	push   0x2020
  434f5c:	6a 00                	push   0x0
  434f5e:	ff 35 40 da 51 00    	push   DWORD PTR ds:0x51da40
  434f64:	ff 15 08 b1 43 00    	call   DWORD PTR ds:0x43b108
  434f6a:	8b f0                	mov    esi,eax
  434f6c:	85 f6                	test   esi,esi
  434f6e:	0f 84 0c 01 00 00    	je     0x435080
  434f74:	8b 2d d8 b0 43 00    	mov    ebp,DWORD PTR ds:0x43b0d8
  434f7a:	6a 04                	push   0x4
  434f7c:	68 00 20 00 00       	push   0x2000
  434f81:	68 00 00 40 00       	push   0x400000
  434f86:	6a 00                	push   0x0
  434f88:	ff d5                	call   ebp
  434f8a:	8b f8                	mov    edi,eax
  434f8c:	85 ff                	test   edi,edi
  434f8e:	0f 84 d5 00 00 00    	je     0x435069
  434f94:	6a 04                	push   0x4
  434f96:	bb 00 00 01 00       	mov    ebx,0x10000
  434f9b:	68 00 10 00 00       	push   0x1000
  434fa0:	53                   	push   ebx
  434fa1:	57                   	push   edi
  434fa2:	ff d5                	call   ebp
  434fa4:	85 c0                	test   eax,eax
  434fa6:	0f 84 af 00 00 00    	je     0x43505b
  434fac:	b8 48 77 44 00       	mov    eax,0x447748
  434fb1:	3b f0                	cmp    esi,eax
  434fb3:	75 1e                	jne    0x434fd3
  434fb5:	83 3d 48 77 44 00 00 	cmp    DWORD PTR ds:0x447748,0x0
  434fbc:	75 05                	jne    0x434fc3
  434fbe:	a3 48 77 44 00       	mov    ds:0x447748,eax
  434fc3:	83 3d 4c 77 44 00 00 	cmp    DWORD PTR ds:0x44774c,0x0
  434fca:	75 1c                	jne    0x434fe8
  434fcc:	a3 4c 77 44 00       	mov    ds:0x44774c,eax
  434fd1:	eb 15                	jmp    0x434fe8
  434fd3:	89 06                	mov    DWORD PTR [esi],eax
  434fd5:	a1 4c 77 44 00       	mov    eax,ds:0x44774c
  434fda:	89 46 04             	mov    DWORD PTR [esi+0x4],eax
  434fdd:	89 35 4c 77 44 00    	mov    DWORD PTR ds:0x44774c,esi
  434fe3:	8b 46 04             	mov    eax,DWORD PTR [esi+0x4]
  434fe6:	89 30                	mov    DWORD PTR [eax],esi
  434fe8:	8d 87 00 00 40 00    	lea    eax,[edi+0x400000]
  434fee:	8d 8e 98 00 00 00    	lea    ecx,[esi+0x98]
  434ff4:	89 46 14             	mov    DWORD PTR [esi+0x14],eax
  434ff7:	8d 46 18             	lea    eax,[esi+0x18]
  434ffa:	89 4e 0c             	mov    DWORD PTR [esi+0xc],ecx
  434ffd:	89 7e 10             	mov    DWORD PTR [esi+0x10],edi
  435000:	89 46 08             	mov    DWORD PTR [esi+0x8],eax
  435003:	33 ed                	xor    ebp,ebp
  435005:	b9 f1 00 00 00       	mov    ecx,0xf1
  43500a:	33 d2                	xor    edx,edx
  43500c:	83 fd 10             	cmp    ebp,0x10
  43500f:	0f 9d c2             	setge  dl
  435012:	4a                   	dec    edx
  435013:	23 d1                	and    edx,ecx
  435015:	4a                   	dec    edx
  435016:	45                   	inc    ebp
  435017:	89 10                	mov    DWORD PTR [eax],edx
  435019:	89 48 04             	mov    DWORD PTR [eax+0x4],ecx
  43501c:	83 c0 08             	add    eax,0x8
  43501f:	81 fd 00 04 00 00    	cmp    ebp,0x400
  435025:	7c e3                	jl     0x43500a
  435027:	53                   	push   ebx
  435028:	6a 00                	push   0x0
  43502a:	57                   	push   edi
  43502b:	e8 10 15 00 00       	call   0x436540
  435030:	83 c4 0c             	add    esp,0xc
  435033:	8b 46 10             	mov    eax,DWORD PTR [esi+0x10]
  435036:	03 c3                	add    eax,ebx
  435038:	3b f8                	cmp    edi,eax
  43503a:	73 1b                	jae    0x435057
  43503c:	80 8f f8 00 00 00 ff 	or     BYTE PTR [edi+0xf8],0xff
  435043:	8d 47 08             	lea    eax,[edi+0x8]
  435046:	89 07                	mov    DWORD PTR [edi],eax
  435048:	c7 47 04 f0 00 00 00 	mov    DWORD PTR [edi+0x4],0xf0
  43504f:	81 c7 00 10 00 00    	add    edi,0x1000
  435055:	eb dc                	jmp    0x435033
  435057:	8b c6                	mov    eax,esi
  435059:	eb 27                	jmp    0x435082
  43505b:	68 00 80 00 00       	push   0x8000
  435060:	6a 00                	push   0x0
  435062:	57                   	push   edi
  435063:	ff 15 dc b0 43 00    	call   DWORD PTR ds:0x43b0dc
  435069:	81 fe 48 77 44 00    	cmp    esi,0x447748
  43506f:	74 0f                	je     0x435080
  435071:	56                   	push   esi
  435072:	6a 00                	push   0x0
  435074:	ff 35 40 da 51 00    	push   DWORD PTR ds:0x51da40
  43507a:	ff 15 0c b1 43 00    	call   DWORD PTR ds:0x43b10c
  435080:	33 c0                	xor    eax,eax
  435082:	5f                   	pop    edi
  435083:	5e                   	pop    esi
  435084:	5d                   	pop    ebp
  435085:	5b                   	pop    ebx
  435086:	c3                   	ret
