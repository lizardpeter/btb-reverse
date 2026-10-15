
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004304d0 <.text+0x2f4d0>:
  4304d0:	55                   	push   ebp
  4304d1:	8b ec                	mov    ebp,esp
  4304d3:	83 c4 f4             	add    esp,0xfffffff4
  4304d6:	9b d9 7d fe          	fstcw  WORD PTR [ebp-0x2]
  4304da:	9b                   	fwait
  4304db:	66 8b 45 fe          	mov    ax,WORD PTR [ebp-0x2]
  4304df:	80 cc 0c             	or     ah,0xc
  4304e2:	66 89 45 fc          	mov    WORD PTR [ebp-0x4],ax
  4304e6:	d9 6d fc             	fldcw  WORD PTR [ebp-0x4]
  4304e9:	df 7d f4             	fistp  QWORD PTR [ebp-0xc]
  4304ec:	d9 6d fe             	fldcw  WORD PTR [ebp-0x2]
  4304ef:	8b 45 f4             	mov    eax,DWORD PTR [ebp-0xc]
  4304f2:	8b 55 f8             	mov    edx,DWORD PTR [ebp-0x8]
  4304f5:	c9                   	leave
  4304f6:	c3                   	ret
