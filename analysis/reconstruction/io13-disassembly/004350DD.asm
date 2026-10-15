
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004350dd <.text+0x340dd>:
  4350dd:	55                   	push   ebp
  4350de:	8b ec                	mov    ebp,esp
  4350e0:	51                   	push   ecx
  4350e1:	53                   	push   ebx
  4350e2:	56                   	push   esi
  4350e3:	8b 35 4c 77 44 00    	mov    esi,DWORD PTR ds:0x44774c
  4350e9:	57                   	push   edi
  4350ea:	83 7e 10 ff          	cmp    DWORD PTR [esi+0x10],0xffffffff
  4350ee:	0f 84 94 00 00 00    	je     0x435188
  4350f4:	83 65 fc 00          	and    DWORD PTR [ebp-0x4],0x0
  4350f8:	8d be 10 20 00 00    	lea    edi,[esi+0x2010]
  4350fe:	bb 00 f0 3f 00       	mov    ebx,0x3ff000
  435103:	81 3f f0 00 00 00    	cmp    DWORD PTR [edi],0xf0
  435109:	75 39                	jne    0x435144
  43510b:	8b c3                	mov    eax,ebx
  43510d:	68 00 40 00 00       	push   0x4000
  435112:	03 46 10             	add    eax,DWORD PTR [esi+0x10]
  435115:	68 00 10 00 00       	push   0x1000
  43511a:	50                   	push   eax
  43511b:	ff 15 dc b0 43 00    	call   DWORD PTR ds:0x43b0dc
  435121:	85 c0                	test   eax,eax
  435123:	74 1f                	je     0x435144
  435125:	83 0f ff             	or     DWORD PTR [edi],0xffffffff
  435128:	ff 0d 44 c4 51 00    	dec    DWORD PTR ds:0x51c444
  43512e:	8b 46 0c             	mov    eax,DWORD PTR [esi+0xc]
  435131:	85 c0                	test   eax,eax
  435133:	74 04                	je     0x435139
  435135:	3b c7                	cmp    eax,edi
  435137:	76 03                	jbe    0x43513c
  435139:	89 7e 0c             	mov    DWORD PTR [esi+0xc],edi
  43513c:	ff 45 fc             	inc    DWORD PTR [ebp-0x4]
  43513f:	ff 4d 08             	dec    DWORD PTR [ebp+0x8]
  435142:	74 0d                	je     0x435151
  435144:	81 eb 00 10 00 00    	sub    ebx,0x1000
  43514a:	83 ef 08             	sub    edi,0x8
  43514d:	85 db                	test   ebx,ebx
  43514f:	7d b2                	jge    0x435103
  435151:	83 7d fc 00          	cmp    DWORD PTR [ebp-0x4],0x0
  435155:	8b ce                	mov    ecx,esi
  435157:	8b 76 04             	mov    esi,DWORD PTR [esi+0x4]
  43515a:	74 2c                	je     0x435188
  43515c:	83 79 18 ff          	cmp    DWORD PTR [ecx+0x18],0xffffffff
  435160:	75 26                	jne    0x435188
  435162:	6a 01                	push   0x1
  435164:	8d 41 20             	lea    eax,[ecx+0x20]
  435167:	5a                   	pop    edx
  435168:	83 38 ff             	cmp    DWORD PTR [eax],0xffffffff
  43516b:	75 0c                	jne    0x435179
  43516d:	42                   	inc    edx
  43516e:	83 c0 08             	add    eax,0x8
  435171:	81 fa 00 04 00 00    	cmp    edx,0x400
  435177:	7c ef                	jl     0x435168
  435179:	81 fa 00 04 00 00    	cmp    edx,0x400
  43517f:	75 07                	jne    0x435188
  435181:	51                   	push   ecx
  435182:	e8 00 ff ff ff       	call   0x435087
  435187:	59                   	pop    ecx
  435188:	3b 35 4c 77 44 00    	cmp    esi,DWORD PTR ds:0x44774c
  43518e:	74 0a                	je     0x43519a
  435190:	83 7d 08 00          	cmp    DWORD PTR [ebp+0x8],0x0
  435194:	0f 8f 50 ff ff ff    	jg     0x4350ea
  43519a:	5f                   	pop    edi
  43519b:	5e                   	pop    esi
  43519c:	5b                   	pop    ebx
  43519d:	c9                   	leave
  43519e:	c3                   	ret
