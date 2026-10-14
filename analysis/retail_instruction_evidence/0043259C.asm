
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0043259c <.text+0x3159c>:
  43259c:	8b 4c 24 08          	mov    ecx,DWORD PTR [esp+0x8]
  4325a0:	56                   	push   esi
  4325a1:	8b 74 24 08          	mov    esi,DWORD PTR [esp+0x8]
  4325a5:	8b 01                	mov    eax,DWORD PTR [ecx]
  4325a7:	8b 51 04             	mov    edx,DWORD PTR [ecx+0x4]
  4325aa:	03 c6                	add    eax,esi
  4325ac:	85 d2                	test   edx,edx
  4325ae:	7c 0d                	jl     0x4325bd
  4325b0:	8b 34 32             	mov    esi,DWORD PTR [edx+esi*1]
  4325b3:	8b 49 08             	mov    ecx,DWORD PTR [ecx+0x8]
  4325b6:	8b 0c 0e             	mov    ecx,DWORD PTR [esi+ecx*1]
  4325b9:	03 ca                	add    ecx,edx
  4325bb:	03 c1                	add    eax,ecx
  4325bd:	5e                   	pop    esi
  4325be:	c3                   	ret
