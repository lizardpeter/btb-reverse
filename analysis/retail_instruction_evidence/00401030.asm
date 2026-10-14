
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00401030 <.text+0x30>:
  401030:	81 ec cc 00 00 00    	sub    esp,0xcc
  401036:	53                   	push   ebx
  401037:	55                   	push   ebp
  401038:	56                   	push   esi
  401039:	57                   	push   edi
  40103a:	b9 19 00 00 00       	mov    ecx,0x19
  40103f:	b8 01 00 00 00       	mov    eax,0x1
  401044:	bf a4 a2 44 00       	mov    edi,0x44a2a4
  401049:	33 ed                	xor    ebp,ebp
  40104b:	68 70 e0 43 00       	push   0x43e070
  401050:	68 58 e0 43 00       	push   0x43e058
  401055:	f3 ab                	rep stos DWORD PTR es:[edi],eax
  401057:	89 2d a0 a2 44 00    	mov    DWORD PTR ds:0x44a2a0,ebp
  40105d:	89 2d a0 dd 44 00    	mov    DWORD PTR ds:0x44dda0,ebp
  401063:	e8 a8 5d 00 00       	call   0x406e10
  401068:	83 c4 08             	add    esp,0x8
  40106b:	8b d8                	mov    ebx,eax
  40106d:	c7 44 24 10 01 00 00 	mov    DWORD PTR [esp+0x10],0x1
  401074:	00 
  401075:	83 fd 01             	cmp    ebp,0x1
  401078:	75 34                	jne    0x4010ae
  40107a:	c7 05 60 a5 44 00 5c 	mov    DWORD PTR ds:0x44a560,0x5c
  401081:	00 00 00 
  401084:	c7 05 64 a5 44 00 31 	mov    DWORD PTR ds:0x44a564,0x131
  40108b:	01 00 00 
  40108e:	c7 05 68 a5 44 00 8f 	mov    DWORD PTR ds:0x44a568,0x8f
  401095:	00 00 00 
  401098:	c7 05 6c a5 44 00 65 	mov    DWORD PTR ds:0x44a56c,0x165
  40109f:	01 00 00 
  4010a2:	c7 05 70 a5 44 00 dd 	mov    DWORD PTR ds:0x44a570,0x1dd
  4010a9:	01 00 00 
  4010ac:	eb 3f                	jmp    0x4010ed
  4010ae:	8d 44 6d 00          	lea    eax,[ebp+ebp*2+0x0]
  4010b2:	8d 04 80             	lea    eax,[eax+eax*4]
  4010b5:	8d 04 80             	lea    eax,[eax+eax*4]
  4010b8:	c1 e0 03             	shl    eax,0x3
  4010bb:	c7 80 08 a3 44 00 14 	mov    DWORD PTR [eax+0x44a308],0x14
  4010c2:	00 00 00 
  4010c5:	c7 80 0c a3 44 00 a4 	mov    DWORD PTR [eax+0x44a30c],0x1a4
  4010cc:	01 00 00 
  4010cf:	c7 80 10 a3 44 00 3f 	mov    DWORD PTR [eax+0x44a310],0x3f
  4010d6:	00 00 00 
  4010d9:	c7 80 14 a3 44 00 ce 	mov    DWORD PTR [eax+0x44a314],0x1ce
  4010e0:	01 00 00 
  4010e3:	c7 80 18 a3 44 00 dd 	mov    DWORD PTR [eax+0x44a318],0x1dd
  4010ea:	01 00 00 
  4010ed:	8d 44 24 14          	lea    eax,[esp+0x14]
  4010f1:	50                   	push   eax
  4010f2:	68 54 e0 43 00       	push   0x43e054
  4010f7:	53                   	push   ebx
  4010f8:	e8 bb ea 02 00       	call   0x42fbb8
  4010fd:	83 c4 0c             	add    esp,0xc
  401100:	8d 74 24 14          	lea    esi,[esp+0x14]
  401104:	b8 50 e0 43 00       	mov    eax,0x43e050
  401109:	8a 10                	mov    dl,BYTE PTR [eax]
  40110b:	8a ca                	mov    cl,dl
  40110d:	3a 16                	cmp    dl,BYTE PTR [esi]
  40110f:	75 1c                	jne    0x40112d
  401111:	84 c9                	test   cl,cl
  401113:	74 14                	je     0x401129
  401115:	8a 50 01             	mov    dl,BYTE PTR [eax+0x1]
  401118:	8a ca                	mov    cl,dl
  40111a:	3a 56 01             	cmp    dl,BYTE PTR [esi+0x1]
  40111d:	75 0e                	jne    0x40112d
  40111f:	83 c0 02             	add    eax,0x2
  401122:	83 c6 02             	add    esi,0x2
  401125:	84 c9                	test   cl,cl
  401127:	75 e0                	jne    0x401109
  401129:	33 c0                	xor    eax,eax
  40112b:	eb 05                	jmp    0x401132
  40112d:	1b c0                	sbb    eax,eax
  40112f:	83 d8 ff             	sbb    eax,0xffffffff
  401132:	85 c0                	test   eax,eax
  401134:	74 5e                	je     0x401194
  401136:	8d 44 6d 00          	lea    eax,[ebp+ebp*2+0x0]
  40113a:	8d 04 80             	lea    eax,[eax+eax*4]
  40113d:	8d 04 80             	lea    eax,[eax+eax*4]
  401140:	8d 34 c5 28 a3 44 00 	lea    esi,[eax*8+0x44a328]
  401147:	8d 4e 04             	lea    ecx,[esi+0x4]
  40114a:	8d 56 fc             	lea    edx,[esi-0x4]
  40114d:	51                   	push   ecx
  40114e:	56                   	push   esi
  40114f:	8d 46 f8             	lea    eax,[esi-0x8]
  401152:	8d 7e f4             	lea    edi,[esi-0xc]
  401155:	52                   	push   edx
  401156:	50                   	push   eax
  401157:	57                   	push   edi
  401158:	68 40 e0 43 00       	push   0x43e040
  40115d:	53                   	push   ebx
  40115e:	e8 55 ea 02 00       	call   0x42fbb8
  401163:	8b 07                	mov    eax,DWORD PTR [edi]
  401165:	83 c4 1c             	add    esp,0x1c
  401168:	83 f8 ff             	cmp    eax,0xffffffff
  40116b:	74 14                	je     0x401181
  40116d:	8b 0c ad a4 a2 44 00 	mov    ecx,DWORD PTR [ebp*4+0x44a2a4]
  401174:	41                   	inc    ecx
  401175:	83 c6 14             	add    esi,0x14
  401178:	89 0c ad a4 a2 44 00 	mov    DWORD PTR [ebp*4+0x44a2a4],ecx
  40117f:	eb c6                	jmp    0x401147
  401181:	8b 44 24 10          	mov    eax,DWORD PTR [esp+0x10]
  401185:	45                   	inc    ebp
  401186:	40                   	inc    eax
  401187:	83 f8 19             	cmp    eax,0x19
  40118a:	89 44 24 10          	mov    DWORD PTR [esp+0x10],eax
  40118e:	0f 8c e1 fe ff ff    	jl     0x401075
  401194:	53                   	push   ebx
  401195:	e8 c8 e9 02 00       	call   0x42fb62
  40119a:	83 c4 04             	add    esp,0x4
  40119d:	5f                   	pop    edi
  40119e:	5e                   	pop    esi
  40119f:	5d                   	pop    ebp
  4011a0:	5b                   	pop    ebx
  4011a1:	81 c4 cc 00 00 00    	add    esp,0xcc
  4011a7:	c3                   	ret
