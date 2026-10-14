
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00436f12 <.text+0x35f12>:
  436f12:	55                   	push   ebp
  436f13:	8b ec                	mov    ebp,esp
  436f15:	81 ec 14 05 00 00    	sub    esp,0x514
  436f1b:	8d 45 ec             	lea    eax,[ebp-0x14]
  436f1e:	56                   	push   esi
  436f1f:	50                   	push   eax
  436f20:	ff 35 b8 c6 51 00    	push   DWORD PTR ds:0x51c6b8
  436f26:	ff 15 9c b0 43 00    	call   DWORD PTR ds:0x43b09c
  436f2c:	83 f8 01             	cmp    eax,0x1
  436f2f:	0f 85 16 01 00 00    	jne    0x43704b
  436f35:	33 c0                	xor    eax,eax
  436f37:	be 00 01 00 00       	mov    esi,0x100
  436f3c:	88 84 05 ec fe ff ff 	mov    BYTE PTR [ebp+eax*1-0x114],al
  436f43:	40                   	inc    eax
  436f44:	3b c6                	cmp    eax,esi
  436f46:	72 f4                	jb     0x436f3c
  436f48:	8a 45 f2             	mov    al,BYTE PTR [ebp-0xe]
  436f4b:	c6 85 ec fe ff ff 20 	mov    BYTE PTR [ebp-0x114],0x20
  436f52:	84 c0                	test   al,al
  436f54:	74 37                	je     0x436f8d
  436f56:	53                   	push   ebx
  436f57:	57                   	push   edi
  436f58:	8d 55 f3             	lea    edx,[ebp-0xd]
  436f5b:	0f b6 0a             	movzx  ecx,BYTE PTR [edx]
  436f5e:	0f b6 c0             	movzx  eax,al
  436f61:	3b c1                	cmp    eax,ecx
  436f63:	77 1d                	ja     0x436f82
  436f65:	2b c8                	sub    ecx,eax
  436f67:	8d bc 05 ec fe ff ff 	lea    edi,[ebp+eax*1-0x114]
  436f6e:	41                   	inc    ecx
  436f6f:	b8 20 20 20 20       	mov    eax,0x20202020
  436f74:	8b d9                	mov    ebx,ecx
  436f76:	c1 e9 02             	shr    ecx,0x2
  436f79:	f3 ab                	rep stos DWORD PTR es:[edi],eax
  436f7b:	8b cb                	mov    ecx,ebx
  436f7d:	83 e1 03             	and    ecx,0x3
  436f80:	f3 aa                	rep stos BYTE PTR es:[edi],al
  436f82:	42                   	inc    edx
  436f83:	42                   	inc    edx
  436f84:	8a 42 ff             	mov    al,BYTE PTR [edx-0x1]
  436f87:	84 c0                	test   al,al
  436f89:	75 d0                	jne    0x436f5b
  436f8b:	5f                   	pop    edi
  436f8c:	5b                   	pop    ebx
  436f8d:	6a 00                	push   0x0
  436f8f:	8d 85 ec fa ff ff    	lea    eax,[ebp-0x514]
  436f95:	ff 35 e4 c8 51 00    	push   DWORD PTR ds:0x51c8e4
  436f9b:	ff 35 b8 c6 51 00    	push   DWORD PTR ds:0x51c6b8
  436fa1:	50                   	push   eax
  436fa2:	8d 85 ec fe ff ff    	lea    eax,[ebp-0x114]
  436fa8:	56                   	push   esi
  436fa9:	50                   	push   eax
  436faa:	6a 01                	push   0x1
  436fac:	e8 53 14 00 00       	call   0x438404
  436fb1:	6a 00                	push   0x0
  436fb3:	8d 85 ec fd ff ff    	lea    eax,[ebp-0x214]
  436fb9:	ff 35 b8 c6 51 00    	push   DWORD PTR ds:0x51c6b8
  436fbf:	56                   	push   esi
  436fc0:	50                   	push   eax
  436fc1:	8d 85 ec fe ff ff    	lea    eax,[ebp-0x114]
  436fc7:	56                   	push   esi
  436fc8:	50                   	push   eax
  436fc9:	56                   	push   esi
  436fca:	ff 35 e4 c8 51 00    	push   DWORD PTR ds:0x51c8e4
  436fd0:	e8 de 00 00 00       	call   0x4370b3
  436fd5:	6a 00                	push   0x0
  436fd7:	8d 85 ec fc ff ff    	lea    eax,[ebp-0x314]
  436fdd:	ff 35 b8 c6 51 00    	push   DWORD PTR ds:0x51c6b8
  436fe3:	56                   	push   esi
  436fe4:	50                   	push   eax
  436fe5:	8d 85 ec fe ff ff    	lea    eax,[ebp-0x114]
  436feb:	56                   	push   esi
  436fec:	50                   	push   eax
  436fed:	68 00 02 00 00       	push   0x200
  436ff2:	ff 35 e4 c8 51 00    	push   DWORD PTR ds:0x51c8e4
  436ff8:	e8 b6 00 00 00       	call   0x4370b3
  436ffd:	83 c4 5c             	add    esp,0x5c
  437000:	33 c0                	xor    eax,eax
  437002:	8d 8d ec fa ff ff    	lea    ecx,[ebp-0x514]
  437008:	66 8b 11             	mov    dx,WORD PTR [ecx]
  43700b:	f6 c2 01             	test   dl,0x1
  43700e:	74 16                	je     0x437026
  437010:	80 88 e1 c7 51 00 10 	or     BYTE PTR [eax+0x51c7e1],0x10
  437017:	8a 94 05 ec fd ff ff 	mov    dl,BYTE PTR [ebp+eax*1-0x214]
  43701e:	88 90 e0 c6 51 00    	mov    BYTE PTR [eax+0x51c6e0],dl
  437024:	eb 1c                	jmp    0x437042
  437026:	f6 c2 02             	test   dl,0x2
  437029:	74 10                	je     0x43703b
  43702b:	80 88 e1 c7 51 00 20 	or     BYTE PTR [eax+0x51c7e1],0x20
  437032:	8a 94 05 ec fc ff ff 	mov    dl,BYTE PTR [ebp+eax*1-0x314]
  437039:	eb e3                	jmp    0x43701e
  43703b:	80 a0 e0 c6 51 00 00 	and    BYTE PTR [eax+0x51c6e0],0x0
  437042:	40                   	inc    eax
  437043:	41                   	inc    ecx
  437044:	41                   	inc    ecx
  437045:	3b c6                	cmp    eax,esi
  437047:	72 bf                	jb     0x437008
  437049:	eb 49                	jmp    0x437094
  43704b:	33 c0                	xor    eax,eax
  43704d:	be 00 01 00 00       	mov    esi,0x100
  437052:	83 f8 41             	cmp    eax,0x41
  437055:	72 19                	jb     0x437070
  437057:	83 f8 5a             	cmp    eax,0x5a
  43705a:	77 14                	ja     0x437070
  43705c:	80 88 e1 c7 51 00 10 	or     BYTE PTR [eax+0x51c7e1],0x10
  437063:	8a c8                	mov    cl,al
  437065:	80 c1 20             	add    cl,0x20
  437068:	88 88 e0 c6 51 00    	mov    BYTE PTR [eax+0x51c6e0],cl
  43706e:	eb 1f                	jmp    0x43708f
  437070:	83 f8 61             	cmp    eax,0x61
  437073:	72 13                	jb     0x437088
  437075:	83 f8 7a             	cmp    eax,0x7a
  437078:	77 0e                	ja     0x437088
  43707a:	80 88 e1 c7 51 00 20 	or     BYTE PTR [eax+0x51c7e1],0x20
  437081:	8a c8                	mov    cl,al
  437083:	80 e9 20             	sub    cl,0x20
  437086:	eb e0                	jmp    0x437068
  437088:	80 a0 e0 c6 51 00 00 	and    BYTE PTR [eax+0x51c6e0],0x0
  43708f:	40                   	inc    eax
  437090:	3b c6                	cmp    eax,esi
  437092:	72 be                	jb     0x437052
  437094:	5e                   	pop    esi
  437095:	c9                   	leave
  437096:	c3                   	ret
