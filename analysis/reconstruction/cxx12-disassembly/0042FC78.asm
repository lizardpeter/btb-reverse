
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0042fc78 <.text+0x2ec78>:
  42fc78:	55                   	push   ebp
  42fc79:	8b ec                	mov    ebp,esp
  42fc7b:	83 ec 04             	sub    esp,0x4
  42fc7e:	53                   	push   ebx
  42fc7f:	56                   	push   esi
  42fc80:	57                   	push   edi
  42fc81:	fc                   	cld
  42fc82:	89 45 fc             	mov    DWORD PTR [ebp-0x4],eax
  42fc85:	33 c0                	xor    eax,eax
  42fc87:	50                   	push   eax
  42fc88:	50                   	push   eax
  42fc89:	50                   	push   eax
  42fc8a:	ff 75 fc             	push   DWORD PTR [ebp-0x4]
  42fc8d:	ff 75 14             	push   DWORD PTR [ebp+0x14]
  42fc90:	ff 75 10             	push   DWORD PTR [ebp+0x10]
  42fc93:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  42fc96:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  42fc99:	e8 27 21 00 00       	call   0x431dc5
  42fc9e:	83 c4 20             	add    esp,0x20
  42fca1:	89 45 14             	mov    DWORD PTR [ebp+0x14],eax
  42fca4:	5f                   	pop    edi
  42fca5:	5e                   	pop    esi
  42fca6:	5b                   	pop    ebx
  42fca7:	8b 45 14             	mov    eax,DWORD PTR [ebp+0x14]
  42fcaa:	8b e5                	mov    esp,ebp
  42fcac:	5d                   	pop    ebp
  42fcad:	c3                   	ret
