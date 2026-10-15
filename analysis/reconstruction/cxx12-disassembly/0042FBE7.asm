
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0042fbe7 <.text+0x2ebe7>:
  42fbe7:	55                   	push   ebp
  42fbe8:	8b ec                	mov    ebp,esp
  42fbea:	51                   	push   ecx
  42fbeb:	53                   	push   ebx
  42fbec:	56                   	push   esi
  42fbed:	57                   	push   edi
  42fbee:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  42fbf1:	83 c0 0c             	add    eax,0xc
  42fbf4:	89 45 fc             	mov    DWORD PTR [ebp-0x4],eax
  42fbf7:	64 8b 1d 00 00 00 00 	mov    ebx,DWORD PTR fs:0x0
  42fbfe:	8b 03                	mov    eax,DWORD PTR [ebx]
  42fc00:	64 a3 00 00 00 00    	mov    fs:0x0,eax
  42fc06:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  42fc09:	8b 5d 0c             	mov    ebx,DWORD PTR [ebp+0xc]
  42fc0c:	8b 63 fc             	mov    esp,DWORD PTR [ebx-0x4]
  42fc0f:	8b 6d fc             	mov    ebp,DWORD PTR [ebp-0x4]
  42fc12:	ff e0                	jmp    eax
