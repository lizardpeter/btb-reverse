
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0042fe50 <.text+0x2ee50>:
  42fe50:	55                   	push   ebp
  42fe51:	8b ec                	mov    ebp,esp
  42fe53:	51                   	push   ecx
  42fe54:	53                   	push   ebx
  42fe55:	56                   	push   esi
  42fe56:	83 7d 0c 00          	cmp    DWORD PTR [ebp+0xc],0x0
  42fe5a:	57                   	push   edi
  42fe5b:	8b 7d 08             	mov    edi,DWORD PTR [ebp+0x8]
  42fe5e:	8b 77 0c             	mov    esi,DWORD PTR [edi+0xc]
  42fe61:	8b 5f 10             	mov    ebx,DWORD PTR [edi+0x10]
  42fe64:	8b c6                	mov    eax,esi
  42fe66:	89 75 08             	mov    DWORD PTR [ebp+0x8],esi
  42fe69:	89 45 fc             	mov    DWORD PTR [ebp-0x4],eax
  42fe6c:	7c 39                	jl     0x42fea7
  42fe6e:	83 fe ff             	cmp    esi,0xffffffff
  42fe71:	75 05                	jne    0x42fe78
  42fe73:	e8 ea 27 00 00       	call   0x432662
  42fe78:	8b 4d 10             	mov    ecx,DWORD PTR [ebp+0x10]
  42fe7b:	4e                   	dec    esi
  42fe7c:	8d 04 b6             	lea    eax,[esi+esi*4]
  42fe7f:	39 4c 83 04          	cmp    DWORD PTR [ebx+eax*4+0x4],ecx
  42fe83:	8d 04 83             	lea    eax,[ebx+eax*4]
  42fe86:	7d 05                	jge    0x42fe8d
  42fe88:	3b 48 08             	cmp    ecx,DWORD PTR [eax+0x8]
  42fe8b:	7e 05                	jle    0x42fe92
  42fe8d:	83 fe ff             	cmp    esi,0xffffffff
  42fe90:	75 0c                	jne    0x42fe9e
  42fe92:	8b 45 08             	mov    eax,DWORD PTR [ebp+0x8]
  42fe95:	ff 4d 0c             	dec    DWORD PTR [ebp+0xc]
  42fe98:	89 45 fc             	mov    DWORD PTR [ebp-0x4],eax
  42fe9b:	89 75 08             	mov    DWORD PTR [ebp+0x8],esi
  42fe9e:	83 7d 0c 00          	cmp    DWORD PTR [ebp+0xc],0x0
  42fea2:	7d ca                	jge    0x42fe6e
  42fea4:	8b 45 fc             	mov    eax,DWORD PTR [ebp-0x4]
  42fea7:	8b 4d 14             	mov    ecx,DWORD PTR [ebp+0x14]
  42feaa:	46                   	inc    esi
  42feab:	89 31                	mov    DWORD PTR [ecx],esi
  42fead:	8b 4d 18             	mov    ecx,DWORD PTR [ebp+0x18]
  42feb0:	89 01                	mov    DWORD PTR [ecx],eax
  42feb2:	3b 47 0c             	cmp    eax,DWORD PTR [edi+0xc]
  42feb5:	77 04                	ja     0x42febb
  42feb7:	3b f0                	cmp    esi,eax
  42feb9:	76 05                	jbe    0x42fec0
  42febb:	e8 a2 27 00 00       	call   0x432662
  42fec0:	8d 04 b6             	lea    eax,[esi+esi*4]
  42fec3:	5f                   	pop    edi
  42fec4:	5e                   	pop    esi
  42fec5:	8d 04 83             	lea    eax,[ebx+eax*4]
  42fec8:	5b                   	pop    ebx
  42fec9:	c9                   	leave
  42feca:	c3                   	ret
