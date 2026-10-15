
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0042fd27 <.text+0x2ed27>:
  42fd27:	55                   	push   ebp
  42fd28:	8b ec                	mov    ebp,esp
  42fd2a:	83 ec 34             	sub    esp,0x34
  42fd2d:	53                   	push   ebx
  42fd2e:	56                   	push   esi
  42fd2f:	57                   	push   edi
  42fd30:	83 65 d8 00          	and    DWORD PTR [ebp-0x28],0x0
  42fd34:	c7 45 dc db fd 42 00 	mov    DWORD PTR [ebp-0x24],0x42fddb
  42fd3b:	8b 45 18             	mov    eax,DWORD PTR [ebp+0x18]
  42fd3e:	89 45 e0             	mov    DWORD PTR [ebp-0x20],eax
  42fd41:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  42fd44:	89 45 e4             	mov    DWORD PTR [ebp-0x1c],eax
  42fd47:	8b 45 1c             	mov    eax,DWORD PTR [ebp+0x1c]
  42fd4a:	89 45 e8             	mov    DWORD PTR [ebp-0x18],eax
  42fd4d:	8b 45 20             	mov    eax,DWORD PTR [ebp+0x20]
  42fd50:	89 45 ec             	mov    DWORD PTR [ebp-0x14],eax
  42fd53:	83 65 f0 00          	and    DWORD PTR [ebp-0x10],0x0
  42fd57:	83 65 f4 00          	and    DWORD PTR [ebp-0xc],0x0
  42fd5b:	83 65 f8 00          	and    DWORD PTR [ebp-0x8],0x0
  42fd5f:	83 65 fc 00          	and    DWORD PTR [ebp-0x4],0x0
  42fd63:	c7 45 f0 ad fd 42 00 	mov    DWORD PTR [ebp-0x10],0x42fdad
  42fd6a:	89 65 f4             	mov    DWORD PTR [ebp-0xc],esp
  42fd6d:	89 6d f8             	mov    DWORD PTR [ebp-0x8],ebp
  42fd70:	64 a1 00 00 00 00    	mov    eax,fs:0x0
  42fd76:	89 45 d8             	mov    DWORD PTR [ebp-0x28],eax
  42fd79:	8d 85 d8 ff ff ff    	lea    eax,[ebp-0x28]
  42fd7f:	64 a3 00 00 00 00    	mov    fs:0x0,eax
  42fd85:	c7 45 cc 01 00 00 00 	mov    DWORD PTR [ebp-0x34],0x1
  42fd8c:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  42fd8f:	89 45 d0             	mov    DWORD PTR [ebp-0x30],eax
  42fd92:	8b 45 10             	mov    eax,DWORD PTR [ebp+0x10]
  42fd95:	89 45 d4             	mov    DWORD PTR [ebp-0x2c],eax
  42fd98:	8d 45 d0             	lea    eax,[ebp-0x30]
  42fd9b:	50                   	push   eax
  42fd9c:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  42fd9f:	ff 30                	push   DWORD PTR [eax]
  42fda1:	ff 15 28 c4 51 00    	call   DWORD PTR ds:0x51c428
  42fda7:	59                   	pop    ecx
  42fda8:	59                   	pop    ecx
  42fda9:	83 65 cc 00          	and    DWORD PTR [ebp-0x34],0x0
  42fdad:	83 7d fc 00          	cmp    DWORD PTR [ebp-0x4],0x0
  42fdb1:	74 17                	je     0x42fdca
  42fdb3:	64 8b 1d 00 00 00 00 	mov    ebx,DWORD PTR fs:0x0
  42fdba:	8b 03                	mov    eax,DWORD PTR [ebx]
  42fdbc:	8b 5d d8             	mov    ebx,DWORD PTR [ebp-0x28]
  42fdbf:	89 03                	mov    DWORD PTR [ebx],eax
  42fdc1:	64 89 1d 00 00 00 00 	mov    DWORD PTR fs:0x0,ebx
  42fdc8:	eb 09                	jmp    0x42fdd3
  42fdca:	8b 45 d8             	mov    eax,DWORD PTR [ebp-0x28]
  42fdcd:	64 a3 00 00 00 00    	mov    fs:0x0,eax
  42fdd3:	8b 45 cc             	mov    eax,DWORD PTR [ebp-0x34]
  42fdd6:	5f                   	pop    edi
  42fdd7:	5e                   	pop    esi
  42fdd8:	5b                   	pop    ebx
  42fdd9:	c9                   	leave
  42fdda:	c3                   	ret
