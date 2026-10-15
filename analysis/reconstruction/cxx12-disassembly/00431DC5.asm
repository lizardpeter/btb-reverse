
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00431dc5 <.text+0x30dc5>:
  431dc5:	55                   	push   ebp
  431dc6:	8b ec                	mov    ebp,esp
  431dc8:	56                   	push   esi
  431dc9:	8b 75 18             	mov    esi,DWORD PTR [ebp+0x18]
  431dcc:	57                   	push   edi
  431dcd:	bf 20 05 93 19       	mov    edi,0x19930520
  431dd2:	39 3e                	cmp    DWORD PTR [esi],edi
  431dd4:	74 05                	je     0x431ddb
  431dd6:	e8 87 08 00 00       	call   0x432662
  431ddb:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  431dde:	f6 40 04 66          	test   BYTE PTR [eax+0x4],0x66
  431de2:	74 1f                	je     0x431e03
  431de4:	83 7e 04 00          	cmp    DWORD PTR [esi+0x4],0x0
  431de8:	74 6f                	je     0x431e59
  431dea:	83 7d 1c 00          	cmp    DWORD PTR [ebp+0x1c],0x0
  431dee:	75 69                	jne    0x431e59
  431df0:	6a ff                	push   0xffffffff
  431df2:	56                   	push   esi
  431df3:	ff 75 14             	push   DWORD PTR [ebp+0x14]
  431df6:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  431df9:	e8 0e 03 00 00       	call   0x43210c
  431dfe:	83 c4 10             	add    esp,0x10
  431e01:	eb 56                	jmp    0x431e59
  431e03:	83 7e 0c 00          	cmp    DWORD PTR [esi+0xc],0x0
  431e07:	74 50                	je     0x431e59
  431e09:	81 38 63 73 6d e0    	cmp    DWORD PTR [eax],0xe06d7363
  431e0f:	75 2c                	jne    0x431e3d
  431e11:	39 78 14             	cmp    DWORD PTR [eax+0x14],edi
  431e14:	76 27                	jbe    0x431e3d
  431e16:	8b 48 1c             	mov    ecx,DWORD PTR [eax+0x1c]
  431e19:	8b 49 08             	mov    ecx,DWORD PTR [ecx+0x8]
  431e1c:	85 c9                	test   ecx,ecx
  431e1e:	74 1d                	je     0x431e3d
  431e20:	0f b6 55 24          	movzx  edx,BYTE PTR [ebp+0x24]
  431e24:	52                   	push   edx
  431e25:	ff 75 20             	push   DWORD PTR [ebp+0x20]
  431e28:	ff 75 1c             	push   DWORD PTR [ebp+0x1c]
  431e2b:	56                   	push   esi
  431e2c:	ff 75 14             	push   DWORD PTR [ebp+0x14]
  431e2f:	ff 75 10             	push   DWORD PTR [ebp+0x10]
  431e32:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  431e35:	50                   	push   eax
  431e36:	ff d1                	call   ecx
  431e38:	83 c4 20             	add    esp,0x20
  431e3b:	eb 1f                	jmp    0x431e5c
  431e3d:	ff 75 20             	push   DWORD PTR [ebp+0x20]
  431e40:	ff 75 1c             	push   DWORD PTR [ebp+0x1c]
  431e43:	ff 75 24             	push   DWORD PTR [ebp+0x24]
  431e46:	56                   	push   esi
  431e47:	ff 75 14             	push   DWORD PTR [ebp+0x14]
  431e4a:	ff 75 10             	push   DWORD PTR [ebp+0x10]
  431e4d:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  431e50:	50                   	push   eax
  431e51:	e8 0a 00 00 00       	call   0x431e60
  431e56:	83 c4 20             	add    esp,0x20
  431e59:	6a 01                	push   0x1
  431e5b:	58                   	pop    eax
  431e5c:	5f                   	pop    edi
  431e5d:	5e                   	pop    esi
  431e5e:	5d                   	pop    ebp
  431e5f:	c3                   	ret
