
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0042ff76 <.text+0x2ef76>:
  42ff76:	33 c0                	xor    eax,eax
  42ff78:	64 8b 0d 00 00 00 00 	mov    ecx,DWORD PTR fs:0x0
  42ff7f:	81 79 04 ec fe 42 00 	cmp    DWORD PTR [ecx+0x4],0x42feec
  42ff86:	75 10                	jne    0x42ff98
  42ff88:	8b 51 0c             	mov    edx,DWORD PTR [ecx+0xc]
  42ff8b:	8b 52 0c             	mov    edx,DWORD PTR [edx+0xc]
  42ff8e:	39 51 08             	cmp    DWORD PTR [ecx+0x8],edx
  42ff91:	75 05                	jne    0x42ff98
  42ff93:	b8 01 00 00 00       	mov    eax,0x1
  42ff98:	c3                   	ret
