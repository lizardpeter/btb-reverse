
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0042fcae <.text+0x2ecae>:
  42fcae:	55                   	push   ebp
  42fcaf:	8b ec                	mov    ebp,esp
  42fcb1:	83 ec 14             	sub    esp,0x14
  42fcb4:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  42fcb7:	83 65 ec 00          	and    DWORD PTR [ebp-0x14],0x0
  42fcbb:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  42fcbe:	89 45 f4             	mov    DWORD PTR [ebp-0xc],eax
  42fcc1:	8b 45 14             	mov    eax,DWORD PTR [ebp+0x14]
  42fcc4:	c7 45 f0 02 fd 42 00 	mov    DWORD PTR [ebp-0x10],0x42fd02
  42fccb:	40                   	inc    eax
  42fccc:	89 4d f8             	mov    DWORD PTR [ebp-0x8],ecx
  42fccf:	89 45 fc             	mov    DWORD PTR [ebp-0x4],eax
  42fcd2:	64 a1 00 00 00 00    	mov    eax,fs:0x0
  42fcd8:	89 45 ec             	mov    DWORD PTR [ebp-0x14],eax
  42fcdb:	8d 85 ec ff ff ff    	lea    eax,[ebp-0x14]
  42fce1:	64 a3 00 00 00 00    	mov    fs:0x0,eax
  42fce7:	ff 75 18             	push   DWORD PTR [ebp+0x18]
  42fcea:	51                   	push   ecx
  42fceb:	ff 75 10             	push   DWORD PTR [ebp+0x10]
  42fcee:	e8 cd 28 00 00       	call   0x4325c0
  42fcf3:	8b c8                	mov    ecx,eax
  42fcf5:	8b 45 ec             	mov    eax,DWORD PTR [ebp-0x14]
  42fcf8:	64 a3 00 00 00 00    	mov    fs:0x0,eax
  42fcfe:	8b c1                	mov    eax,ecx
  42fd00:	c9                   	leave
  42fd01:	c3                   	ret
