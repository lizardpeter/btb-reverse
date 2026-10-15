
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00404c60 <.text+0x3c60>:
  404c60:	83 ec 18             	sub    esp,0x18
  404c63:	53                   	push   ebx
  404c64:	55                   	push   ebp
  404c65:	56                   	push   esi
  404c66:	8b f1                	mov    esi,ecx
  404c68:	57                   	push   edi
  404c69:	8b 3d 7c b2 43 00    	mov    edi,DWORD PTR ds:0x43b27c
  404c6f:	8b 4e 04             	mov    ecx,DWORD PTR [esi+0x4]
  404c72:	8d 46 1c             	lea    eax,[esi+0x1c]
  404c75:	6a 20                	push   0x20
  404c77:	50                   	push   eax
  404c78:	51                   	push   ecx
  404c79:	c7 44 24 1c ff ff ff 	mov    DWORD PTR [esp+0x1c],0xffffffff
  404c80:	ff 
  404c81:	c7 46 24 57 41 56 45 	mov    DWORD PTR [esi+0x24],0x45564157
  404c88:	c7 46 20 00 00 00 00 	mov    DWORD PTR [esi+0x20],0x0
  404c8f:	ff d7                	call   edi
  404c91:	85 c0                	test   eax,eax
  404c93:	74 0f                	je     0x404ca4
  404c95:	5f                   	pop    edi
  404c96:	5e                   	pop    esi
  404c97:	5d                   	pop    ebp
  404c98:	b8 05 40 00 80       	mov    eax,0x80004005
  404c9d:	5b                   	pop    ebx
  404c9e:	83 c4 18             	add    esp,0x18
  404ca1:	c2 04 00             	ret    0x4
  404ca4:	8b 56 04             	mov    edx,DWORD PTR [esi+0x4]
  404ca7:	8d 5e 08             	lea    ebx,[esi+0x8]
  404caa:	6a 00                	push   0x0
  404cac:	53                   	push   ebx
  404cad:	52                   	push   edx
  404cae:	c7 03 66 6d 74 20    	mov    DWORD PTR [ebx],0x20746d66
  404cb4:	c7 46 0c 10 00 00 00 	mov    DWORD PTR [esi+0xc],0x10
  404cbb:	ff d7                	call   edi
  404cbd:	85 c0                	test   eax,eax
  404cbf:	74 0f                	je     0x404cd0
  404cc1:	5f                   	pop    edi
  404cc2:	5e                   	pop    esi
  404cc3:	5d                   	pop    ebp
  404cc4:	b8 05 40 00 80       	mov    eax,0x80004005
  404cc9:	5b                   	pop    ebx
  404cca:	83 c4 18             	add    esp,0x18
  404ccd:	c2 04 00             	ret    0x4
  404cd0:	8b 7c 24 2c          	mov    edi,DWORD PTR [esp+0x2c]
  404cd4:	66 83 3f 01          	cmp    WORD PTR [edi],0x1
  404cd8:	75 23                	jne    0x404cfd
  404cda:	8b 46 04             	mov    eax,DWORD PTR [esi+0x4]
  404cdd:	8b 2d 8c b2 43 00    	mov    ebp,DWORD PTR ds:0x43b28c
  404ce3:	6a 10                	push   0x10
  404ce5:	57                   	push   edi
  404ce6:	50                   	push   eax
  404ce7:	ff d5                	call   ebp
  404ce9:	83 f8 10             	cmp    eax,0x10
  404cec:	74 42                	je     0x404d30
  404cee:	5f                   	pop    edi
  404cef:	5e                   	pop    esi
  404cf0:	5d                   	pop    ebp
  404cf1:	b8 05 40 00 80       	mov    eax,0x80004005
  404cf6:	5b                   	pop    ebx
  404cf7:	83 c4 18             	add    esp,0x18
  404cfa:	c2 04 00             	ret    0x4
  404cfd:	8b 56 04             	mov    edx,DWORD PTR [esi+0x4]
  404d00:	33 c9                	xor    ecx,ecx
  404d02:	66 8b 4f 10          	mov    cx,WORD PTR [edi+0x10]
  404d06:	8b 2d 8c b2 43 00    	mov    ebp,DWORD PTR ds:0x43b28c
  404d0c:	83 c1 12             	add    ecx,0x12
  404d0f:	51                   	push   ecx
  404d10:	57                   	push   edi
  404d11:	52                   	push   edx
  404d12:	ff d5                	call   ebp
  404d14:	33 c9                	xor    ecx,ecx
  404d16:	66 8b 4f 10          	mov    cx,WORD PTR [edi+0x10]
  404d1a:	83 c1 12             	add    ecx,0x12
  404d1d:	3b c1                	cmp    eax,ecx
  404d1f:	74 0f                	je     0x404d30
  404d21:	5f                   	pop    edi
  404d22:	5e                   	pop    esi
  404d23:	5d                   	pop    ebp
  404d24:	b8 05 40 00 80       	mov    eax,0x80004005
  404d29:	5b                   	pop    ebx
  404d2a:	83 c4 18             	add    esp,0x18
  404d2d:	c2 04 00             	ret    0x4
  404d30:	8b 56 04             	mov    edx,DWORD PTR [esi+0x4]
  404d33:	8b 3d 6c b2 43 00    	mov    edi,DWORD PTR ds:0x43b26c
  404d39:	6a 00                	push   0x0
  404d3b:	53                   	push   ebx
  404d3c:	52                   	push   edx
  404d3d:	ff d7                	call   edi
  404d3f:	85 c0                	test   eax,eax
  404d41:	74 0f                	je     0x404d52
  404d43:	5f                   	pop    edi
  404d44:	5e                   	pop    esi
  404d45:	5d                   	pop    ebp
  404d46:	b8 05 40 00 80       	mov    eax,0x80004005
  404d4b:	5b                   	pop    ebx
  404d4c:	83 c4 18             	add    esp,0x18
  404d4f:	c2 04 00             	ret    0x4
  404d52:	8b 4e 04             	mov    ecx,DWORD PTR [esi+0x4]
  404d55:	8d 44 24 14          	lea    eax,[esp+0x14]
  404d59:	6a 00                	push   0x0
  404d5b:	50                   	push   eax
  404d5c:	51                   	push   ecx
  404d5d:	c7 44 24 20 66 61 63 	mov    DWORD PTR [esp+0x20],0x74636166
  404d64:	74 
  404d65:	c7 44 24 24 00 00 00 	mov    DWORD PTR [esp+0x24],0x0
  404d6c:	00 
  404d6d:	ff 15 7c b2 43 00    	call   DWORD PTR ds:0x43b27c
  404d73:	85 c0                	test   eax,eax
  404d75:	74 0f                	je     0x404d86
  404d77:	5f                   	pop    edi
  404d78:	5e                   	pop    esi
  404d79:	5d                   	pop    ebp
  404d7a:	b8 05 40 00 80       	mov    eax,0x80004005
  404d7f:	5b                   	pop    ebx
  404d80:	83 c4 18             	add    esp,0x18
  404d83:	c2 04 00             	ret    0x4
  404d86:	8b 46 04             	mov    eax,DWORD PTR [esi+0x4]
  404d89:	8d 54 24 10          	lea    edx,[esp+0x10]
  404d8d:	6a 04                	push   0x4
  404d8f:	52                   	push   edx
  404d90:	50                   	push   eax
  404d91:	ff d5                	call   ebp
  404d93:	83 f8 04             	cmp    eax,0x4
  404d96:	74 0f                	je     0x404da7
  404d98:	5f                   	pop    edi
  404d99:	5e                   	pop    esi
  404d9a:	5d                   	pop    ebp
  404d9b:	b8 05 40 00 80       	mov    eax,0x80004005
  404da0:	5b                   	pop    ebx
  404da1:	83 c4 18             	add    esp,0x18
  404da4:	c2 04 00             	ret    0x4
  404da7:	8b 56 04             	mov    edx,DWORD PTR [esi+0x4]
  404daa:	8d 4c 24 14          	lea    ecx,[esp+0x14]
  404dae:	6a 00                	push   0x0
  404db0:	51                   	push   ecx
  404db1:	52                   	push   edx
  404db2:	ff d7                	call   edi
  404db4:	f7 d8                	neg    eax
  404db6:	5f                   	pop    edi
  404db7:	5e                   	pop    esi
  404db8:	1b c0                	sbb    eax,eax
  404dba:	5d                   	pop    ebp
  404dbb:	25 05 40 00 80       	and    eax,0x80004005
  404dc0:	5b                   	pop    ebx
  404dc1:	83 c4 18             	add    esp,0x18
  404dc4:	c2 04 00             	ret    0x4
