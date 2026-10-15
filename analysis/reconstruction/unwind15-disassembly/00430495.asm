
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00430495 <.text+0x2f495>:
  430495:	55                   	push   ebp
  430496:	8b ec                	mov    ebp,esp
  430498:	83 ec 20             	sub    esp,0x20
  43049b:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  43049e:	56                   	push   esi
  43049f:	57                   	push   edi
  4304a0:	6a 08                	push   0x8
  4304a2:	59                   	pop    ecx
  4304a3:	be e0 b5 43 00       	mov    esi,0x43b5e0
  4304a8:	8d 7d e0             	lea    edi,[ebp-0x20]
  4304ab:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  4304ad:	89 45 f8             	mov    DWORD PTR [ebp-0x8],eax
  4304b0:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  4304b3:	89 45 fc             	mov    DWORD PTR [ebp-0x4],eax
  4304b6:	8d 45 f4             	lea    eax,[ebp-0xc]
  4304b9:	50                   	push   eax
  4304ba:	ff 75 f0             	push   DWORD PTR [ebp-0x10]
  4304bd:	ff 75 e4             	push   DWORD PTR [ebp-0x1c]
  4304c0:	ff 75 e0             	push   DWORD PTR [ebp-0x20]
  4304c3:	ff 15 1c b1 43 00    	call   DWORD PTR ds:0x43b11c
  4304c9:	5f                   	pop    edi
  4304ca:	5e                   	pop    esi
  4304cb:	c9                   	leave
  4304cc:	c2 08 00             	ret    0x8
