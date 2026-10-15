
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00405ee0 <.text+0x4ee0>:
  405ee0:	64 a1 00 00 00 00    	mov    eax,fs:0x0
  405ee6:	6a ff                	push   0xffffffff
  405ee8:	68 03 a5 43 00       	push   0x43a503
  405eed:	50                   	push   eax
  405eee:	64 89 25 00 00 00 00 	mov    DWORD PTR fs:0x0,esp
  405ef5:	81 ec 8c 00 00 00    	sub    esp,0x8c
  405efb:	8d 44 24 38          	lea    eax,[esp+0x38]
  405eff:	53                   	push   ebx
  405f00:	55                   	push   ebp
  405f01:	56                   	push   esi
  405f02:	57                   	push   edi
  405f03:	8b bc 24 ac 00 00 00 	mov    edi,DWORD PTR [esp+0xac]
  405f0a:	50                   	push   eax
  405f0b:	6a 54                	push   0x54
  405f0d:	8b e9                	mov    ebp,ecx
  405f0f:	57                   	push   edi
  405f10:	ff 15 4c b0 43 00    	call   DWORD PTR ds:0x43b04c
  405f16:	83 f8 54             	cmp    eax,0x54
  405f19:	0f 85 24 01 00 00    	jne    0x406043
  405f1f:	8b 74 24 5a          	mov    esi,DWORD PTR [esp+0x5a]
  405f23:	8b 94 24 80 00 00 00 	mov    edx,DWORD PTR [esp+0x80]
  405f2a:	8b ce                	mov    ecx,esi
  405f2c:	89 54 24 10          	mov    DWORD PTR [esp+0x10],edx
  405f30:	81 e1 ff ff 00 00    	and    ecx,0xffff
  405f36:	8b c1                	mov    eax,ecx
  405f38:	0f af 44 24 4c       	imul   eax,DWORD PTR [esp+0x4c]
  405f3d:	83 c0 1f             	add    eax,0x1f
  405f40:	c1 f8 03             	sar    eax,0x3
  405f43:	24 fc                	and    al,0xfc
  405f45:	66 85 d2             	test   dx,dx
  405f48:	0f 85 c3 00 00 00    	jne    0x406011
  405f4e:	66 83 fe 08          	cmp    si,0x8
  405f52:	0f 87 b9 00 00 00    	ja     0x406011
  405f58:	ba 01 00 00 00       	mov    edx,0x1
  405f5d:	d3 e2                	shl    edx,cl
  405f5f:	89 54 24 10          	mov    DWORD PTR [esp+0x10],edx
  405f63:	8b 5c 24 50          	mov    ebx,DWORD PTR [esp+0x50]
  405f67:	0f bf 74 24 10       	movsx  esi,WORD PTR [esp+0x10]
  405f6c:	0f af d8             	imul   ebx,eax
  405f6f:	8b 45 04             	mov    eax,DWORD PTR [ebp+0x4]
  405f72:	89 74 24 28          	mov    DWORD PTR [esp+0x28],esi
  405f76:	50                   	push   eax
  405f77:	e8 60 9c 02 00       	call   0x42fbdc
  405f7c:	8d 4c b3 28          	lea    ecx,[ebx+esi*4+0x28]
  405f80:	51                   	push   ecx
  405f81:	e8 48 9c 02 00       	call   0x42fbce
  405f86:	8d 54 b0 28          	lea    edx,[eax+esi*4+0x28]
  405f8a:	83 c4 08             	add    esp,0x8
  405f8d:	66 83 7c 24 10 00    	cmp    WORD PTR [esp+0x10],0x0
  405f93:	b9 0a 00 00 00       	mov    ecx,0xa
  405f98:	8d 74 24 60          	lea    esi,[esp+0x60]
  405f9c:	8b f8                	mov    edi,eax
  405f9e:	89 45 04             	mov    DWORD PTR [ebp+0x4],eax
  405fa1:	89 55 08             	mov    DWORD PTR [ebp+0x8],edx
  405fa4:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  405fa6:	7e 50                	jle    0x405ff8
  405fa8:	83 7c 24 70 03       	cmp    DWORD PTR [esp+0x70],0x3
  405fad:	74 49                	je     0x405ff8
  405faf:	6a 00                	push   0x0
  405fb1:	ff 15 40 b0 43 00    	call   DWORD PTR ds:0x43b040
  405fb7:	8b 3d 48 b0 43 00    	mov    edi,DWORD PTR ds:0x43b048
  405fbd:	8b f0                	mov    esi,eax
  405fbf:	8b 84 24 ac 00 00 00 	mov    eax,DWORD PTR [esp+0xac]
  405fc6:	50                   	push   eax
  405fc7:	56                   	push   esi
  405fc8:	ff d7                	call   edi
  405fca:	8b 4d 04             	mov    ecx,DWORD PTR [ebp+0x4]
  405fcd:	8b 54 24 28          	mov    edx,DWORD PTR [esp+0x28]
  405fd1:	83 c1 28             	add    ecx,0x28
  405fd4:	89 84 24 ac 00 00 00 	mov    DWORD PTR [esp+0xac],eax
  405fdb:	51                   	push   ecx
  405fdc:	52                   	push   edx
  405fdd:	6a 00                	push   0x0
  405fdf:	56                   	push   esi
  405fe0:	ff 15 60 b0 43 00    	call   DWORD PTR ds:0x43b060
  405fe6:	8b 84 24 ac 00 00 00 	mov    eax,DWORD PTR [esp+0xac]
  405fed:	50                   	push   eax
  405fee:	56                   	push   esi
  405fef:	ff d7                	call   edi
  405ff1:	56                   	push   esi
  405ff2:	ff 15 44 b0 43 00    	call   DWORD PTR ds:0x43b044
  405ff8:	8b 74 24 5c          	mov    esi,DWORD PTR [esp+0x5c]
  405ffc:	8b 7d 08             	mov    edi,DWORD PTR [ebp+0x8]
  405fff:	8b cb                	mov    ecx,ebx
  406001:	8b d1                	mov    edx,ecx
  406003:	c1 e9 02             	shr    ecx,0x2
  406006:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  406008:	8b ca                	mov    ecx,edx
  40600a:	83 e1 03             	and    ecx,0x3
  40600d:	f3 a4                	rep movs BYTE PTR es:[edi],BYTE PTR ds:[esi]
  40600f:	eb 17                	jmp    0x406028
  406011:	8b 4c 24 70          	mov    ecx,DWORD PTR [esp+0x70]
  406015:	85 c9                	test   ecx,ecx
  406017:	0f 84 46 ff ff ff    	je     0x405f63
  40601d:	8b 55 00             	mov    edx,DWORD PTR [ebp+0x0]
  406020:	6a 00                	push   0x0
  406022:	57                   	push   edi
  406023:	8b cd                	mov    ecx,ebp
  406025:	ff 52 2c             	call   DWORD PTR [edx+0x2c]
  406028:	8b 8c 24 9c 00 00 00 	mov    ecx,DWORD PTR [esp+0x9c]
  40602f:	5f                   	pop    edi
  406030:	5e                   	pop    esi
  406031:	5d                   	pop    ebp
  406032:	5b                   	pop    ebx
  406033:	64 89 0d 00 00 00 00 	mov    DWORD PTR fs:0x0,ecx
  40603a:	81 c4 98 00 00 00    	add    esp,0x98
  406040:	c2 04 00             	ret    0x4
  406043:	8a 44 24 17          	mov    al,BYTE PTR [esp+0x17]
  406047:	bf 78 e2 43 00       	mov    edi,0x43e278
  40604c:	88 44 24 18          	mov    BYTE PTR [esp+0x18],al
  406050:	83 c9 ff             	or     ecx,0xffffffff
  406053:	33 c0                	xor    eax,eax
  406055:	33 ed                	xor    ebp,ebp
  406057:	f2 ae                	repnz scas al,BYTE PTR es:[edi]
  406059:	f7 d1                	not    ecx
  40605b:	49                   	dec    ecx
  40605c:	6a 01                	push   0x1
  40605e:	8b d9                	mov    ebx,ecx
  406060:	8d 4c 24 1c          	lea    ecx,[esp+0x1c]
  406064:	53                   	push   ebx
  406065:	89 6c 24 24          	mov    DWORD PTR [esp+0x24],ebp
  406069:	89 6c 24 28          	mov    DWORD PTR [esp+0x28],ebp
  40606d:	89 6c 24 2c          	mov    DWORD PTR [esp+0x2c],ebp
  406071:	e8 ea 03 00 00       	call   0x406460
  406076:	84 c0                	test   al,al
  406078:	74 25                	je     0x40609f
  40607a:	8b 7c 24 1c          	mov    edi,DWORD PTR [esp+0x1c]
  40607e:	8b cb                	mov    ecx,ebx
  406080:	8b d1                	mov    edx,ecx
  406082:	be 78 e2 43 00       	mov    esi,0x43e278
  406087:	c1 e9 02             	shr    ecx,0x2
  40608a:	f3 a5                	rep movs DWORD PTR es:[edi],DWORD PTR ds:[esi]
  40608c:	8b ca                	mov    ecx,edx
  40608e:	83 e1 03             	and    ecx,0x3
  406091:	f3 a4                	rep movs BYTE PTR es:[edi],BYTE PTR ds:[esi]
  406093:	8b 44 24 1c          	mov    eax,DWORD PTR [esp+0x1c]
  406097:	89 5c 24 20          	mov    DWORD PTR [esp+0x20],ebx
  40609b:	c6 04 18 00          	mov    BYTE PTR [eax+ebx*1],0x0
  40609f:	8d 4c 24 10          	lea    ecx,[esp+0x10]
  4060a3:	89 ac 24 a4 00 00 00 	mov    DWORD PTR [esp+0xa4],ebp
  4060aa:	51                   	push   ecx
  4060ab:	8d 4c 24 30          	lea    ecx,[esp+0x30]
  4060af:	c7 44 24 14 74 25 48 	mov    DWORD PTR [esp+0x14],0x482574
  4060b6:	00 
  4060b7:	e8 fe a2 02 00       	call   0x4303ba
  4060bc:	a1 34 b3 43 00       	mov    eax,ds:0x43b334
  4060c1:	8a 54 24 18          	mov    dl,BYTE PTR [esp+0x18]
  4060c5:	50                   	push   eax
  4060c6:	8d 4c 24 1c          	lea    ecx,[esp+0x1c]
  4060ca:	55                   	push   ebp
  4060cb:	51                   	push   ecx
  4060cc:	8d 4c 24 44          	lea    ecx,[esp+0x44]
  4060d0:	c6 84 24 b0 00 00 00 	mov    BYTE PTR [esp+0xb0],0x1
  4060d7:	01 
  4060d8:	88 54 24 44          	mov    BYTE PTR [esp+0x44],dl
  4060dc:	89 6c 24 48          	mov    DWORD PTR [esp+0x48],ebp
  4060e0:	89 6c 24 4c          	mov    DWORD PTR [esp+0x4c],ebp
  4060e4:	89 6c 24 50          	mov    DWORD PTR [esp+0x50],ebp
  4060e8:	e8 63 00 00 00       	call   0x406150
  4060ed:	8d 54 24 2c          	lea    edx,[esp+0x2c]
  4060f1:	68 d0 bd 43 00       	push   0x43bdd0
  4060f6:	52                   	push   edx
  4060f7:	c7 44 24 34 28 b3 43 	mov    DWORD PTR [esp+0x34],0x43b328
  4060fe:	00 
  4060ff:	c6 84 24 ac 00 00 00 	mov    BYTE PTR [esp+0xac],0x0
  406106:	00 
  406107:	e8 89 a3 02 00       	call   0x430495
