
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0043210c <.text+0x3110c>:
  43210c:	55                   	push   ebp
  43210d:	8b ec                	mov    ebp,esp
  43210f:	6a ff                	push   0xffffffff
  432111:	68 10 b6 43 00       	push   0x43b610
  432116:	68 38 5f 43 00       	push   0x435f38
  43211b:	64 a1 00 00 00 00    	mov    eax,fs:0x0
  432121:	50                   	push   eax
  432122:	64 89 25 00 00 00 00 	mov    DWORD PTR fs:0x0,esp
  432129:	83 ec 0c             	sub    esp,0xc
  43212c:	53                   	push   ebx
  43212d:	56                   	push   esi
  43212e:	57                   	push   edi
  43212f:	89 65 e8             	mov    DWORD PTR [ebp-0x18],esp
  432132:	8b 5d 08             	mov    ebx,DWORD PTR [ebp+0x8]
  432135:	8b 73 08             	mov    esi,DWORD PTR [ebx+0x8]
  432138:	89 75 e4             	mov    DWORD PTR [ebp-0x1c],esi
  43213b:	8b 7d 10             	mov    edi,DWORD PTR [ebp+0x10]
  43213e:	3b 75 14             	cmp    esi,DWORD PTR [ebp+0x14]
  432141:	74 55                	je     0x432198
  432143:	83 fe ff             	cmp    esi,0xffffffff
  432146:	7e 05                	jle    0x43214d
  432148:	3b 77 04             	cmp    esi,DWORD PTR [edi+0x4]
  43214b:	7c 05                	jl     0x432152
  43214d:	e8 10 05 00 00       	call   0x432662
  432152:	83 65 fc 00          	and    DWORD PTR [ebp-0x4],0x0
  432156:	8b 47 08             	mov    eax,DWORD PTR [edi+0x8]
  432159:	8b 44 f0 04          	mov    eax,DWORD PTR [eax+esi*8+0x4]
  43215d:	85 c0                	test   eax,eax
  43215f:	74 0c                	je     0x43216d
  432161:	68 03 01 00 00       	push   0x103
  432166:	53                   	push   ebx
  432167:	50                   	push   eax
  432168:	e8 53 04 00 00       	call   0x4325c0
  43216d:	83 4d fc ff          	or     DWORD PTR [ebp-0x4],0xffffffff
  432171:	eb 1a                	jmp    0x43218d
  432173:	ff 75 ec             	push   DWORD PTR [ebp-0x14]
  432176:	e8 2f 00 00 00       	call   0x4321aa
  43217b:	59                   	pop    ecx
  43217c:	c3                   	ret
  43217d:	8b 65 e8             	mov    esp,DWORD PTR [ebp-0x18]
  432180:	83 4d fc ff          	or     DWORD PTR [ebp-0x4],0xffffffff
  432184:	8b 7d 10             	mov    edi,DWORD PTR [ebp+0x10]
  432187:	8b 5d 08             	mov    ebx,DWORD PTR [ebp+0x8]
  43218a:	8b 75 e4             	mov    esi,DWORD PTR [ebp-0x1c]
  43218d:	8b 47 08             	mov    eax,DWORD PTR [edi+0x8]
