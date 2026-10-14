
/tmp/btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00426f30 <.text+0x25f30>:
  426f30:	81 ec 88 00 00 00    	sub    esp,0x88
  426f36:	a1 cc 50 51 00       	mov    eax,ds:0x5150cc
  426f3b:	53                   	push   ebx
  426f3c:	55                   	push   ebp
  426f3d:	33 db                	xor    ebx,ebx
  426f3f:	56                   	push   esi
  426f40:	83 cd ff             	or     ebp,0xffffffff
  426f43:	57                   	push   edi
  426f44:	3b c3                	cmp    eax,ebx
  426f46:	89 6c 24 14          	mov    DWORD PTR [esp+0x14],ebp
  426f4a:	bf 02 00 00 00       	mov    edi,0x2
  426f4f:	75 0e                	jne    0x426f5f
  426f51:	c7 44 24 1c 03 00 00 	mov    DWORD PTR [esp+0x1c],0x3
  426f58:	00 
  426f59:	89 5c 24 18          	mov    DWORD PTR [esp+0x18],ebx
  426f5d:	eb 1f                	jmp    0x426f7e
  426f5f:	83 f8 01             	cmp    eax,0x1
  426f62:	75 0e                	jne    0x426f72
  426f64:	c7 44 24 1c 03 00 00 	mov    DWORD PTR [esp+0x1c],0x3
  426f6b:	00 
  426f6c:	89 44 24 18          	mov    DWORD PTR [esp+0x18],eax
  426f70:	eb 0c                	jmp    0x426f7e
  426f72:	3b c7                	cmp    eax,edi
  426f74:	75 08                	jne    0x426f7e
  426f76:	89 7c 24 1c          	mov    DWORD PTR [esp+0x1c],edi
  426f7a:	89 7c 24 18          	mov    DWORD PTR [esp+0x18],edi
  426f7e:	8b 0d d0 50 51 00    	mov    ecx,DWORD PTR ds:0x5150d0
  426f84:	8b 34 8d 78 50 51 00 	mov    esi,DWORD PTR [ecx*4+0x515078]
  426f8b:	83 fe 01             	cmp    esi,0x1
  426f8e:	75 1c                	jne    0x426fac
  426f90:	8b d1                	mov    edx,ecx
  426f92:	c1 e2 04             	shl    edx,0x4
  426f95:	8b 82 10 4f 51 00    	mov    eax,DWORD PTR [edx+0x514f10]
  426f9b:	8b aa 14 4f 51 00    	mov    ebp,DWORD PTR [edx+0x514f14]
  426fa1:	a3 00 4f 51 00       	mov    ds:0x514f00,eax
  426fa6:	89 6c 24 14          	mov    DWORD PTR [esp+0x14],ebp
  426faa:	eb 05                	jmp    0x426fb1
  426fac:	a1 00 4f 51 00       	mov    eax,ds:0x514f00
  426fb1:	3b f3                	cmp    esi,ebx
  426fb3:	75 16                	jne    0x426fcb
  426fb5:	c1 e1 04             	shl    ecx,0x4
  426fb8:	8b 81 0c 4f 51 00    	mov    eax,DWORD PTR [ecx+0x514f0c]
  426fbe:	8b a9 10 4f 51 00    	mov    ebp,DWORD PTR [ecx+0x514f10]
  426fc4:	a3 00 4f 51 00       	mov    ds:0x514f00,eax
  426fc9:	eb 18                	jmp    0x426fe3
  426fcb:	3b f7                	cmp    esi,edi
  426fcd:	75 18                	jne    0x426fe7
  426fcf:	c1 e1 04             	shl    ecx,0x4
  426fd2:	8b 81 14 4f 51 00    	mov    eax,DWORD PTR [ecx+0x514f14]
  426fd8:	8b a9 18 4f 51 00    	mov    ebp,DWORD PTR [ecx+0x514f18]
  426fde:	a3 00 4f 51 00       	mov    ds:0x514f00,eax
  426fe3:	89 6c 24 14          	mov    DWORD PTR [esp+0x14],ebp
  426fe7:	8b 4c 24 1c          	mov    ecx,DWORD PTR [esp+0x1c]
  426feb:	3b cb                	cmp    ecx,ebx
  426fed:	7e 69                	jle    0x427058
  426fef:	8d 5c 24 48          	lea    ebx,[esp+0x48]
  426ff3:	8d 7c 24 5c          	lea    edi,[esp+0x5c]
  426ff7:	8d 74 24 58          	lea    esi,[esp+0x58]
  426ffb:	89 4c 24 10          	mov    DWORD PTR [esp+0x10],ecx
  426fff:	83 fd ff             	cmp    ebp,0xffffffff
  427002:	75 2a                	jne    0x42702e
  427004:	e8 bb 8f 00 00       	call   0x42ffc4
  427009:	99                   	cdq
  42700a:	b9 03 00 00 00       	mov    ecx,0x3
  42700f:	83 c6 08             	add    esi,0x8
  427012:	f7 f9                	idiv   ecx
  427014:	a1 00 4f 51 00       	mov    eax,ds:0x514f00
  427019:	8b c8                	mov    ecx,eax
  42701b:	89 46 f8             	mov    DWORD PTR [esi-0x8],eax
  42701e:	89 17                	mov    DWORD PTR [edi],edx
  427020:	8d 2c 42             	lea    ebp,[edx+eax*2]
  427023:	03 cd                	add    ecx,ebp
  427025:	8b 6c 24 14          	mov    ebp,DWORD PTR [esp+0x14]
  427029:	83 c7 08             	add    edi,0x8
  42702c:	eb 18                	jmp    0x427046
  42702e:	8d 54 45 00          	lea    edx,[ebp+eax*2+0x0]
  427032:	8b c8                	mov    ecx,eax
  427034:	89 06                	mov    DWORD PTR [esi],eax
  427036:	03 ca                	add    ecx,edx
  427038:	89 2f                	mov    DWORD PTR [edi],ebp
  42703a:	83 c6 08             	add    esi,0x8
  42703d:	83 c7 08             	add    edi,0x8
  427040:	89 2d 54 4f 51 00    	mov    DWORD PTR ds:0x514f54,ebp
  427046:	89 0b                	mov    DWORD PTR [ebx],ecx
  427048:	8b 4c 24 10          	mov    ecx,DWORD PTR [esp+0x10]
  42704c:	83 c3 04             	add    ebx,0x4
  42704f:	49                   	dec    ecx
  427050:	89 4c 24 10          	mov    DWORD PTR [esp+0x10],ecx
  427054:	75 a9                	jne    0x426fff
  427056:	33 db                	xor    ebx,ebx
  427058:	8b 4c 24 18          	mov    ecx,DWORD PTR [esp+0x18]
  42705c:	89 5c 24 10          	mov    DWORD PTR [esp+0x10],ebx
  427060:	3b cb                	cmp    ecx,ebx
  427062:	7e 74                	jle    0x4270d8
  427064:	8d 4c 24 38          	lea    ecx,[esp+0x38]
  427068:	8d 7c 24 7c          	lea    edi,[esp+0x7c]
  42706c:	89 4c 24 20          	mov    DWORD PTR [esp+0x20],ecx
  427070:	eb 05                	jmp    0x427077
  427072:	a1 00 4f 51 00       	mov    eax,ds:0x514f00
  427077:	85 db                	test   ebx,ebx
  427079:	8b f0                	mov    esi,eax
  42707b:	0f 85 3c 01 00 00    	jne    0x4271bd
  427081:	3b f0                	cmp    esi,eax
  427083:	75 18                	jne    0x42709d
  427085:	e8 3a 8f 00 00       	call   0x42ffc4
  42708a:	99                   	cdq
  42708b:	b9 03 00 00 00       	mov    ecx,0x3
  427090:	f7 f9                	idiv   ecx
  427092:	a1 00 4f 51 00       	mov    eax,ds:0x514f00
  427097:	8b f2                	mov    esi,edx
  427099:	3b f0                	cmp    esi,eax
  42709b:	74 e8                	je     0x427085
  42709d:	89 74 24 10          	mov    DWORD PTR [esp+0x10],esi
  4270a1:	e8 1e 8f 00 00       	call   0x42ffc4
  4270a6:	99                   	cdq
  4270a7:	b9 03 00 00 00       	mov    ecx,0x3
  4270ac:	f7 f9                	idiv   ecx
  4270ae:	3b d5                	cmp    edx,ebp
  4270b0:	74 ef                	je     0x4270a1
  4270b2:	8d 04 72             	lea    eax,[edx+esi*2]
  4270b5:	83 c7 08             	add    edi,0x8
  4270b8:	03 f0                	add    esi,eax
  4270ba:	8b 44 24 20          	mov    eax,DWORD PTR [esp+0x20]
  4270be:	89 77 f4             	mov    DWORD PTR [edi-0xc],esi
  4270c1:	89 57 f8             	mov    DWORD PTR [edi-0x8],edx
  4270c4:	89 30                	mov    DWORD PTR [eax],esi
  4270c6:	83 c0 04             	add    eax,0x4
  4270c9:	89 44 24 20          	mov    DWORD PTR [esp+0x20],eax
  4270cd:	8b 44 24 18          	mov    eax,DWORD PTR [esp+0x18]
  4270d1:	43                   	inc    ebx
  4270d2:	3b d8                	cmp    ebx,eax
  4270d4:	7c 9c                	jl     0x427072
  4270d6:	33 db                	xor    ebx,ebx
  4270d8:	e8 e7 8e 00 00       	call   0x42ffc4
  4270dd:	99                   	cdq
  4270de:	b9 07 00 00 00       	mov    ecx,0x7
  4270e3:	33 f6                	xor    esi,esi
  4270e5:	f7 f9                	idiv   ecx
  4270e7:	8b 44 24 18          	mov    eax,DWORD PTR [esp+0x18]
  4270eb:	89 5c 24 24          	mov    DWORD PTR [esp+0x24],ebx
  4270ef:	89 5c 24 20          	mov    DWORD PTR [esp+0x20],ebx
  4270f3:	89 15 98 50 51 00    	mov    DWORD PTR ds:0x515098,edx
  4270f9:	8b 54 24 1c          	mov    edx,DWORD PTR [esp+0x1c]
  4270fd:	03 c2                	add    eax,edx
  4270ff:	3b c3                	cmp    eax,ebx
  427101:	89 44 24 34          	mov    DWORD PTR [esp+0x34],eax
  427105:	0f 8e 4d 01 00 00    	jle    0x427258
  42710b:	8d 4c 24 7c          	lea    ecx,[esp+0x7c]
  42710f:	8d 54 24 38          	lea    edx,[esp+0x38]
  427113:	89 4c 24 30          	mov    DWORD PTR [esp+0x30],ecx
  427117:	8d 44 24 5c          	lea    eax,[esp+0x5c]
  42711b:	8d 4c 24 48          	lea    ecx,[esp+0x48]
  42711f:	c7 44 24 14 bc 4e 51 	mov    DWORD PTR [esp+0x14],0x514ebc
  427126:	00 
  427127:	bd b8 4e 51 00       	mov    ebp,0x514eb8
  42712c:	bb f0 4f 51 00       	mov    ebx,0x514ff0
  427131:	bf 88 50 51 00       	mov    edi,0x515088
  427136:	89 54 24 10          	mov    DWORD PTR [esp+0x10],edx
  42713a:	89 44 24 2c          	mov    DWORD PTR [esp+0x2c],eax
  42713e:	89 4c 24 28          	mov    DWORD PTR [esp+0x28],ecx
  427142:	e8 7d 8e 00 00       	call   0x42ffc4
  427147:	25 01 00 00 80       	and    eax,0x80000001
  42714c:	79 05                	jns    0x427153
  42714e:	48                   	dec    eax
  42714f:	83 c8 fe             	or     eax,0xfffffffe
  427152:	40                   	inc    eax
  427153:	0f 85 88 00 00 00    	jne    0x4271e1
  427159:	8b 54 24 24          	mov    edx,DWORD PTR [esp+0x24]
  42715d:	8b 44 24 1c          	mov    eax,DWORD PTR [esp+0x1c]
  427161:	3b d0                	cmp    edx,eax
  427163:	0f 8d e5 00 00 00    	jge    0x42724e
  427169:	a1 98 50 51 00       	mov    eax,ds:0x515098
  42716e:	c7 03 01 00 00 00    	mov    DWORD PTR [ebx],0x1
  427174:	8d 0c 86             	lea    ecx,[esi+eax*4]
  427177:	8b 04 8d ac 69 44 00 	mov    eax,DWORD PTR [ecx*4+0x4469ac]
  42717e:	8d 14 c0             	lea    edx,[eax+eax*8]
  427181:	8b 44 24 28          	mov    eax,DWORD PTR [esp+0x28]
  427185:	8b 08                	mov    ecx,DWORD PTR [eax]
  427187:	8b 44 24 2c          	mov    eax,DWORD PTR [esp+0x2c]
  42718b:	03 d1                	add    edx,ecx
  42718d:	8b 48 fc             	mov    ecx,DWORD PTR [eax-0x4]
  427190:	89 17                	mov    DWORD PTR [edi],edx
  427192:	8b 10                	mov    edx,DWORD PTR [eax]
  427194:	89 4d 00             	mov    DWORD PTR [ebp+0x0],ecx
  427197:	8b 4c 24 14          	mov    ecx,DWORD PTR [esp+0x14]
  42719b:	89 11                	mov    DWORD PTR [ecx],edx
  42719d:	8b 54 24 24          	mov    edx,DWORD PTR [esp+0x24]
  4271a1:	42                   	inc    edx
  4271a2:	89 54 24 24          	mov    DWORD PTR [esp+0x24],edx
  4271a6:	8b 54 24 28          	mov    edx,DWORD PTR [esp+0x28]
  4271aa:	83 c2 04             	add    edx,0x4
  4271ad:	83 c0 08             	add    eax,0x8
  4271b0:	89 54 24 28          	mov    DWORD PTR [esp+0x28],edx
  4271b4:	89 44 24 2c          	mov    DWORD PTR [esp+0x2c],eax
  4271b8:	e9 80 00 00 00       	jmp    0x42723d
  4271bd:	3b f0                	cmp    esi,eax
  4271bf:	74 0a                	je     0x4271cb
  4271c1:	3b 74 24 10          	cmp    esi,DWORD PTR [esp+0x10]
  4271c5:	0f 85 d6 fe ff ff    	jne    0x4270a1
  4271cb:	e8 f4 8d 00 00       	call   0x42ffc4
  4271d0:	99                   	cdq
  4271d1:	b9 03 00 00 00       	mov    ecx,0x3
  4271d6:	f7 f9                	idiv   ecx
  4271d8:	a1 00 4f 51 00       	mov    eax,ds:0x514f00
  4271dd:	8b f2                	mov    esi,edx
  4271df:	eb dc                	jmp    0x4271bd
  4271e1:	8b 44 24 20          	mov    eax,DWORD PTR [esp+0x20]
  4271e5:	8b 4c 24 18          	mov    ecx,DWORD PTR [esp+0x18]
  4271e9:	3b c1                	cmp    eax,ecx
  4271eb:	7d 61                	jge    0x42724e
  4271ed:	8b 0d 98 50 51 00    	mov    ecx,DWORD PTR ds:0x515098
  4271f3:	c7 03 00 00 00 00    	mov    DWORD PTR [ebx],0x0
  4271f9:	8d 14 8e             	lea    edx,[esi+ecx*4]
  4271fc:	8b 4c 24 10          	mov    ecx,DWORD PTR [esp+0x10]
  427200:	8b 04 95 ac 69 44 00 	mov    eax,DWORD PTR [edx*4+0x4469ac]
  427207:	8b 11                	mov    edx,DWORD PTR [ecx]
  427209:	8b 4c 24 14          	mov    ecx,DWORD PTR [esp+0x14]
  42720d:	8d 04 c0             	lea    eax,[eax+eax*8]
  427210:	03 c2                	add    eax,edx
  427212:	89 07                	mov    DWORD PTR [edi],eax
  427214:	8b 44 24 30          	mov    eax,DWORD PTR [esp+0x30]
  427218:	8b 50 fc             	mov    edx,DWORD PTR [eax-0x4]
  42721b:	89 55 00             	mov    DWORD PTR [ebp+0x0],edx
  42721e:	8b 10                	mov    edx,DWORD PTR [eax]
  427220:	89 11                	mov    DWORD PTR [ecx],edx
  427222:	8b 54 24 20          	mov    edx,DWORD PTR [esp+0x20]
  427226:	42                   	inc    edx
  427227:	89 54 24 20          	mov    DWORD PTR [esp+0x20],edx
  42722b:	8b 54 24 10          	mov    edx,DWORD PTR [esp+0x10]
  42722f:	83 c2 04             	add    edx,0x4
  427232:	83 c0 08             	add    eax,0x8
  427235:	89 54 24 10          	mov    DWORD PTR [esp+0x10],edx
  427239:	89 44 24 30          	mov    DWORD PTR [esp+0x30],eax
  42723d:	46                   	inc    esi
  42723e:	83 c7 04             	add    edi,0x4
  427241:	83 c3 04             	add    ebx,0x4
  427244:	83 c5 08             	add    ebp,0x8
  427247:	83 c1 08             	add    ecx,0x8
  42724a:	89 4c 24 14          	mov    DWORD PTR [esp+0x14],ecx
  42724e:	3b 74 24 34          	cmp    esi,DWORD PTR [esp+0x34]
  427252:	0f 8c ea fe ff ff    	jl     0x427142
  427258:	5f                   	pop    edi
  427259:	5e                   	pop    esi
  42725a:	5d                   	pop    ebp
  42725b:	5b                   	pop    ebx
  42725c:	81 c4 88 00 00 00    	add    esp,0x88
  427262:	c3                   	ret
