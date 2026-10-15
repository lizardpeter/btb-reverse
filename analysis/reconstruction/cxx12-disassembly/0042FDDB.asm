
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0042fddb <.text+0x2eddb>:
  42fddb:	55                   	push   ebp
  42fddc:	8b ec                	mov    ebp,esp
  42fdde:	53                   	push   ebx
  42fddf:	56                   	push   esi
  42fde0:	57                   	push   edi
  42fde1:	fc                   	cld
  42fde2:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  42fde5:	8b 40 04             	mov    eax,DWORD PTR [eax+0x4]
  42fde8:	83 e0 66             	and    eax,0x66
  42fdeb:	85 c0                	test   eax,eax
  42fded:	74 0f                	je     0x42fdfe
  42fdef:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  42fdf2:	c7 40 24 01 00 00 00 	mov    DWORD PTR [eax+0x24],0x1
  42fdf9:	6a 01                	push   0x1
  42fdfb:	58                   	pop    eax
  42fdfc:	eb 4d                	jmp    0x42fe4b
  42fdfe:	6a 01                	push   0x1
  42fe00:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  42fe03:	ff 70 14             	push   DWORD PTR [eax+0x14]
  42fe06:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  42fe09:	ff 70 10             	push   DWORD PTR [eax+0x10]
  42fe0c:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  42fe0f:	ff 70 08             	push   DWORD PTR [eax+0x8]
  42fe12:	6a 00                	push   0x0
  42fe14:	ff 75 10             	push   DWORD PTR [ebp+0x10]
  42fe17:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  42fe1a:	ff 70 0c             	push   DWORD PTR [eax+0xc]
  42fe1d:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  42fe20:	e8 a0 1f 00 00       	call   0x431dc5
  42fe25:	83 c4 20             	add    esp,0x20
  42fe28:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  42fe2b:	83 78 24 00          	cmp    DWORD PTR [eax+0x24],0x0
  42fe2f:	75 0b                	jne    0x42fe3c
  42fe31:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  42fe34:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  42fe37:	e8 ed fd ff ff       	call   0x42fc29
  42fe3c:	8b 5d 0c             	mov    ebx,DWORD PTR [ebp+0xc]
  42fe3f:	8b 63 1c             	mov    esp,DWORD PTR [ebx+0x1c]
  42fe42:	8b 6b 20             	mov    ebp,DWORD PTR [ebx+0x20]
  42fe45:	ff 63 18             	jmp    DWORD PTR [ebx+0x18]
  42fe48:	6a 01                	push   0x1
  42fe4a:	58                   	pop    eax
  42fe4b:	5f                   	pop    edi
  42fe4c:	5e                   	pop    esi
