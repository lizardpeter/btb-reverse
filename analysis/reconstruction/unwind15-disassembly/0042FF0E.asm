
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0042ff0e <.text+0x2ef0e>:
  42ff0e:	53                   	push   ebx
  42ff0f:	56                   	push   esi
  42ff10:	57                   	push   edi
  42ff11:	8b 44 24 10          	mov    eax,DWORD PTR [esp+0x10]
  42ff15:	50                   	push   eax
  42ff16:	6a fe                	push   0xfffffffe
  42ff18:	68 ec fe 42 00       	push   0x42feec
  42ff1d:	64 ff 35 00 00 00 00 	push   DWORD PTR fs:0x0
  42ff24:	64 89 25 00 00 00 00 	mov    DWORD PTR fs:0x0,esp
  42ff2b:	8b 44 24 20          	mov    eax,DWORD PTR [esp+0x20]
  42ff2f:	8b 58 08             	mov    ebx,DWORD PTR [eax+0x8]
  42ff32:	8b 70 0c             	mov    esi,DWORD PTR [eax+0xc]
  42ff35:	83 fe ff             	cmp    esi,0xffffffff
  42ff38:	74 2e                	je     0x42ff68
  42ff3a:	3b 74 24 24          	cmp    esi,DWORD PTR [esp+0x24]
  42ff3e:	74 28                	je     0x42ff68
  42ff40:	8d 34 76             	lea    esi,[esi+esi*2]
  42ff43:	8b 0c b3             	mov    ecx,DWORD PTR [ebx+esi*4]
  42ff46:	89 4c 24 08          	mov    DWORD PTR [esp+0x8],ecx
  42ff4a:	89 48 0c             	mov    DWORD PTR [eax+0xc],ecx
  42ff4d:	83 7c b3 04 00       	cmp    DWORD PTR [ebx+esi*4+0x4],0x0
  42ff52:	75 12                	jne    0x42ff66
  42ff54:	68 01 01 00 00       	push   0x101
  42ff59:	8b 44 b3 08          	mov    eax,DWORD PTR [ebx+esi*4+0x8]
  42ff5d:	e8 40 00 00 00       	call   0x42ffa2
  42ff62:	ff 54 b3 08          	call   DWORD PTR [ebx+esi*4+0x8]
  42ff66:	eb c3                	jmp    0x42ff2b
  42ff68:	64 8f 05 00 00 00 00 	pop    DWORD PTR fs:0x0
  42ff6f:	83 c4 0c             	add    esp,0xc
  42ff72:	5f                   	pop    edi
  42ff73:	5e                   	pop    esi
  42ff74:	5b                   	pop    ebx
  42ff75:	c3                   	ret
