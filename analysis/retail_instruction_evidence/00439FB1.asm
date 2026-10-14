
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00439fb1 <.text+0x38fb1>:
  439fb1:	b8 a8 a5 43 00       	mov    eax,0x43a5a8
  439fb6:	e8 6d 02 00 00       	call   0x43a228
  439fbb:	51                   	push   ecx
  439fbc:	56                   	push   esi
  439fbd:	8b f1                	mov    esi,ecx
  439fbf:	89 75 f0             	mov    DWORD PTR [ebp-0x10],esi
  439fc2:	c7 06 74 bb 43 00    	mov    DWORD PTR [esi],0x43bb74
  439fc8:	83 65 fc 00          	and    DWORD PTR [ebp-0x4],0x0
  439fcc:	6a 01                	push   0x1
  439fce:	8d 4e 0c             	lea    ecx,[esi+0xc]
  439fd1:	e8 7a c3 fc ff       	call   0x406350
  439fd6:	83 4d fc ff          	or     DWORD PTR [ebp-0x4],0xffffffff
  439fda:	8b ce                	mov    ecx,esi
  439fdc:	e8 60 64 ff ff       	call   0x430441
  439fe1:	8b 4d f4             	mov    ecx,DWORD PTR [ebp-0xc]
  439fe4:	5e                   	pop    esi
  439fe5:	64 89 0d 00 00 00 00 	mov    DWORD PTR fs:0x0,ecx
  439fec:	c9                   	leave
  439fed:	c3                   	ret
