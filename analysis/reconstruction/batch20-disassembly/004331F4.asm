
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004331f4 <.text+0x321f4>:
  4331f4:	56                   	push   esi
  4331f5:	8b 74 24 08          	mov    esi,DWORD PTR [esp+0x8]
  4331f9:	0f be 06             	movsx  eax,BYTE PTR [esi]
  4331fc:	50                   	push   eax
  4331fd:	e8 a7 43 00 00       	call   0x4375a9
  433202:	83 f8 65             	cmp    eax,0x65
  433205:	59                   	pop    ecx
  433206:	74 2c                	je     0x433234
  433208:	46                   	inc    esi
  433209:	83 3d 10 9b 44 00 01 	cmp    DWORD PTR ds:0x449b10,0x1
  433210:	7e 0f                	jle    0x433221
  433212:	0f be 06             	movsx  eax,BYTE PTR [esi]
  433215:	6a 04                	push   0x4
  433217:	50                   	push   eax
  433218:	e8 a3 33 00 00       	call   0x4365c0
  43321d:	59                   	pop    ecx
  43321e:	59                   	pop    ecx
  43321f:	eb 0f                	jmp    0x433230
  433221:	0f be 06             	movsx  eax,BYTE PTR [esi]
  433224:	8b 0d 1c 9b 44 00    	mov    ecx,DWORD PTR ds:0x449b1c
  43322a:	8a 04 41             	mov    al,BYTE PTR [ecx+eax*2]
  43322d:	83 e0 04             	and    eax,0x4
  433230:	85 c0                	test   eax,eax
  433232:	75 d4                	jne    0x433208
  433234:	8a 0d 14 9b 44 00    	mov    cl,BYTE PTR ds:0x449b14
  43323a:	8a 06                	mov    al,BYTE PTR [esi]
  43323c:	88 0e                	mov    BYTE PTR [esi],cl
  43323e:	46                   	inc    esi
  43323f:	8a 0e                	mov    cl,BYTE PTR [esi]
  433241:	88 06                	mov    BYTE PTR [esi],al
  433243:	8a c1                	mov    al,cl
  433245:	8a 0e                	mov    cl,BYTE PTR [esi]
  433247:	46                   	inc    esi
  433248:	84 c9                	test   cl,cl
  43324a:	75 f3                	jne    0x43323f
  43324c:	5e                   	pop    esi
  43324d:	c3                   	ret
