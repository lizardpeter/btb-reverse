
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0043a00a <.text+0x3900a>:
  43a00a:	b8 bc a5 43 00       	mov    eax,0x43a5bc
  43a00f:	e8 14 02 00 00       	call   0x43a228
  43a014:	51                   	push   ecx
  43a015:	53                   	push   ebx
  43a016:	8b 5d 08             	mov    ebx,DWORD PTR [ebp+0x8]
  43a019:	56                   	push   esi
  43a01a:	57                   	push   edi
  43a01b:	8b f1                	mov    esi,ecx
  43a01d:	53                   	push   ebx
  43a01e:	89 75 f0             	mov    DWORD PTR [ebp-0x10],esi
  43a021:	e8 d1 63 ff ff       	call   0x4303f7
  43a026:	8a 43 0c             	mov    al,BYTE PTR [ebx+0xc]
  43a029:	83 65 fc 00          	and    DWORD PTR [ebp-0x4],0x0
  43a02d:	83 c3 0c             	add    ebx,0xc
  43a030:	8d 7e 0c             	lea    edi,[esi+0xc]
  43a033:	6a 00                	push   0x0
  43a035:	8b cf                	mov    ecx,edi
  43a037:	88 07                	mov    BYTE PTR [edi],al
  43a039:	e8 12 c3 fc ff       	call   0x406350
  43a03e:	ff 35 34 b3 43 00    	push   DWORD PTR ds:0x43b334
  43a044:	8b cf                	mov    ecx,edi
  43a046:	6a 00                	push   0x0
  43a048:	53                   	push   ebx
  43a049:	e8 02 c1 fc ff       	call   0x406150
  43a04e:	8b 4d f4             	mov    ecx,DWORD PTR [ebp-0xc]
  43a051:	c7 06 74 bb 43 00    	mov    DWORD PTR [esi],0x43bb74
  43a057:	8b c6                	mov    eax,esi
  43a059:	5f                   	pop    edi
  43a05a:	5e                   	pop    esi
  43a05b:	5b                   	pop    ebx
  43a05c:	64 89 0d 00 00 00 00 	mov    DWORD PTR fs:0x0,ecx
  43a063:	c9                   	leave
  43a064:	c2 04 00             	ret    0x4
