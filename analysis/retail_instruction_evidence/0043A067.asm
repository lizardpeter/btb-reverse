
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0043a067 <.text+0x39067>:
  43a067:	b8 d0 a5 43 00       	mov    eax,0x43a5d0
  43a06c:	e8 b7 01 00 00       	call   0x43a228
  43a071:	51                   	push   ecx
  43a072:	56                   	push   esi
  43a073:	8b f1                	mov    esi,ecx
  43a075:	89 75 f0             	mov    DWORD PTR [ebp-0x10],esi
  43a078:	c7 06 74 bb 43 00    	mov    DWORD PTR [esi],0x43bb74
  43a07e:	83 65 fc 00          	and    DWORD PTR [ebp-0x4],0x0
  43a082:	6a 01                	push   0x1
  43a084:	8d 4e 0c             	lea    ecx,[esi+0xc]
  43a087:	e8 c4 c2 fc ff       	call   0x406350
  43a08c:	83 4d fc ff          	or     DWORD PTR [ebp-0x4],0xffffffff
  43a090:	8b ce                	mov    ecx,esi
  43a092:	e8 aa 63 ff ff       	call   0x430441
  43a097:	8b 4d f4             	mov    ecx,DWORD PTR [ebp-0xc]
  43a09a:	5e                   	pop    esi
  43a09b:	64 89 0d 00 00 00 00 	mov    DWORD PTR fs:0x0,ecx
  43a0a2:	c9                   	leave
  43a0a3:	c3                   	ret
