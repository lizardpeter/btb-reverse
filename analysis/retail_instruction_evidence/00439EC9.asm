
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00439ec9 <.text+0x38ec9>:
  439ec9:	b8 80 a5 43 00       	mov    eax,0x43a580
  439ece:	e8 55 03 00 00       	call   0x43a228
  439ed3:	83 ec 30             	sub    esp,0x30
  439ed6:	8a 45 f3             	mov    al,BYTE PTR [ebp-0xd]
  439ed9:	56                   	push   esi
  439eda:	6a 00                	push   0x0
  439edc:	8d 4d e0             	lea    ecx,[ebp-0x20]
  439edf:	88 45 e0             	mov    BYTE PTR [ebp-0x20],al
  439ee2:	e8 69 c4 fc ff       	call   0x406350
  439ee7:	be 60 bb 43 00       	mov    esi,0x43bb60
  439eec:	56                   	push   esi
  439eed:	e8 2e 98 ff ff       	call   0x433720
  439ef2:	59                   	pop    ecx
  439ef3:	50                   	push   eax
  439ef4:	56                   	push   esi
  439ef5:	8d 4d e0             	lea    ecx,[ebp-0x20]
  439ef8:	e8 a3 c4 fc ff       	call   0x4063a0
  439efd:	83 65 fc 00          	and    DWORD PTR [ebp-0x4],0x0
  439f01:	8d 45 e0             	lea    eax,[ebp-0x20]
  439f04:	50                   	push   eax
  439f05:	8d 4d c4             	lea    ecx,[ebp-0x3c]
  439f08:	e8 16 00 00 00       	call   0x439f23
  439f0d:	8d 45 c4             	lea    eax,[ebp-0x3c]
  439f10:	68 e8 c0 43 00       	push   0x43c0e8
  439f15:	50                   	push   eax
  439f16:	c7 45 c4 54 bb 43 00 	mov    DWORD PTR [ebp-0x3c],0x43bb54
  439f1d:	e8 73 65 ff ff       	call   0x430495
