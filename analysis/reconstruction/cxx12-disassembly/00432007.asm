
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00432007 <.text+0x31007>:
  432007:	55                   	push   ebp
  432008:	8b ec                	mov    ebp,esp
  43200a:	51                   	push   ecx
  43200b:	51                   	push   ecx
  43200c:	83 3d 28 c4 51 00 00 	cmp    DWORD PTR ds:0x51c428,0x0
  432013:	56                   	push   esi
  432014:	57                   	push   edi
  432015:	74 21                	je     0x432038
  432017:	ff 75 24             	push   DWORD PTR [ebp+0x24]
  43201a:	ff 75 20             	push   DWORD PTR [ebp+0x20]
  43201d:	ff 75 18             	push   DWORD PTR [ebp+0x18]
  432020:	ff 75 14             	push   DWORD PTR [ebp+0x14]
  432023:	ff 75 10             	push   DWORD PTR [ebp+0x10]
  432026:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  432029:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  43202c:	e8 f6 dc ff ff       	call   0x42fd27
  432031:	83 c4 1c             	add    esp,0x1c
  432034:	85 c0                	test   eax,eax
  432036:	75 73                	jne    0x4320ab
  432038:	8b 7d 1c             	mov    edi,DWORD PTR [ebp+0x1c]
  43203b:	8d 45 f8             	lea    eax,[ebp-0x8]
  43203e:	50                   	push   eax
  43203f:	8d 45 fc             	lea    eax,[ebp-0x4]
  432042:	50                   	push   eax
  432043:	57                   	push   edi
  432044:	ff 75 20             	push   DWORD PTR [ebp+0x20]
  432047:	ff 75 18             	push   DWORD PTR [ebp+0x18]
  43204a:	e8 01 de ff ff       	call   0x42fe50
  43204f:	83 c4 14             	add    esp,0x14
  432052:	8b f0                	mov    esi,eax
  432054:	8b 45 fc             	mov    eax,DWORD PTR [ebp-0x4]
  432057:	3b 45 f8             	cmp    eax,DWORD PTR [ebp-0x8]
  43205a:	73 4f                	jae    0x4320ab
  43205c:	3b 3e                	cmp    edi,DWORD PTR [esi]
  43205e:	7c 43                	jl     0x4320a3
  432060:	3b 7e 04             	cmp    edi,DWORD PTR [esi+0x4]
  432063:	7f 3e                	jg     0x4320a3
  432065:	8b 46 0c             	mov    eax,DWORD PTR [esi+0xc]
  432068:	8b 4e 10             	mov    ecx,DWORD PTR [esi+0x10]
  43206b:	c1 e0 04             	shl    eax,0x4
  43206e:	03 c1                	add    eax,ecx
  432070:	8b 48 f4             	mov    ecx,DWORD PTR [eax-0xc]
  432073:	85 c9                	test   ecx,ecx
  432075:	74 06                	je     0x43207d
  432077:	80 79 08 00          	cmp    BYTE PTR [ecx+0x8],0x0
  43207b:	75 26                	jne    0x4320a3
  43207d:	6a 01                	push   0x1
  43207f:	83 c0 f0             	add    eax,0xfffffff0
  432082:	ff 75 24             	push   DWORD PTR [ebp+0x24]
  432085:	ff 75 20             	push   DWORD PTR [ebp+0x20]
  432088:	56                   	push   esi
  432089:	6a 00                	push   0x0
  43208b:	50                   	push   eax
  43208c:	ff 75 18             	push   DWORD PTR [ebp+0x18]
  43208f:	ff 75 14             	push   DWORD PTR [ebp+0x14]
  432092:	ff 75 10             	push   DWORD PTR [ebp+0x10]
  432095:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  432098:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  43209b:	e8 20 01 00 00       	call   0x4321c0
  4320a0:	83 c4 2c             	add    esp,0x2c
  4320a3:	ff 45 fc             	inc    DWORD PTR [ebp-0x4]
  4320a6:	83 c6 14             	add    esi,0x14
  4320a9:	eb a9                	jmp    0x432054
  4320ab:	5f                   	pop    edi
  4320ac:	5e                   	pop    esi
  4320ad:	c9                   	leave
  4320ae:	c3                   	ret
