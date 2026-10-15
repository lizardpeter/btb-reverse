
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00430d2a <.text+0x2fd2a>:
  430d2a:	55                   	push   ebp
  430d2b:	8b ec                	mov    ebp,esp
  430d2d:	51                   	push   ecx
  430d2e:	56                   	push   esi
  430d2f:	8b 75 08             	mov    esi,DWORD PTR [ebp+0x8]
  430d32:	85 f6                	test   esi,esi
  430d34:	74 5a                	je     0x430d90
  430d36:	a1 44 da 51 00       	mov    eax,ds:0x51da44
  430d3b:	83 f8 03             	cmp    eax,0x3
  430d3e:	75 16                	jne    0x430d56
  430d40:	56                   	push   esi
  430d41:	e8 fe 36 00 00       	call   0x434444
  430d46:	59                   	pop    ecx
  430d47:	85 c0                	test   eax,eax
  430d49:	56                   	push   esi
  430d4a:	74 36                	je     0x430d82
  430d4c:	50                   	push   eax
  430d4d:	e8 1d 37 00 00       	call   0x43446f
  430d52:	59                   	pop    ecx
  430d53:	59                   	pop    ecx
  430d54:	eb 3a                	jmp    0x430d90
  430d56:	83 f8 02             	cmp    eax,0x2
  430d59:	75 26                	jne    0x430d81
  430d5b:	8d 45 08             	lea    eax,[ebp+0x8]
  430d5e:	50                   	push   eax
  430d5f:	8d 45 fc             	lea    eax,[ebp-0x4]
  430d62:	50                   	push   eax
  430d63:	56                   	push   esi
  430d64:	e8 36 44 00 00       	call   0x43519f
  430d69:	83 c4 0c             	add    esp,0xc
  430d6c:	85 c0                	test   eax,eax
  430d6e:	74 11                	je     0x430d81
  430d70:	50                   	push   eax
  430d71:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  430d74:	ff 75 fc             	push   DWORD PTR [ebp-0x4]
  430d77:	e8 7a 44 00 00       	call   0x4351f6
  430d7c:	83 c4 0c             	add    esp,0xc
  430d7f:	eb 0f                	jmp    0x430d90
  430d81:	56                   	push   esi
  430d82:	6a 00                	push   0x0
  430d84:	ff 35 40 da 51 00    	push   DWORD PTR ds:0x51da40
  430d8a:	ff 15 0c b1 43 00    	call   DWORD PTR ds:0x43b10c
  430d90:	5e                   	pop    esi
  430d91:	c9                   	leave
  430d92:	c3                   	ret
