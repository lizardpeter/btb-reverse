
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0042fd02 <.text+0x2ed02>:
  42fd02:	55                   	push   ebp
  42fd03:	8b ec                	mov    ebp,esp
  42fd05:	fc                   	cld
  42fd06:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  42fd09:	6a 00                	push   0x0
  42fd0b:	50                   	push   eax
  42fd0c:	ff 70 10             	push   DWORD PTR [eax+0x10]
  42fd0f:	ff 70 08             	push   DWORD PTR [eax+0x8]
  42fd12:	6a 00                	push   0x0
  42fd14:	ff 75 10             	push   DWORD PTR [ebp+0x10]
  42fd17:	ff 70 0c             	push   DWORD PTR [eax+0xc]
  42fd1a:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  42fd1d:	e8 a3 20 00 00       	call   0x431dc5
  42fd22:	83 c4 20             	add    esp,0x20
  42fd25:	5d                   	pop    ebp
  42fd26:	c3                   	ret
