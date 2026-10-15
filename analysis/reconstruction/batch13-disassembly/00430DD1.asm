
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00430dd1 <.text+0x2fdd1>:
  430dd1:	a1 44 da 51 00       	mov    eax,ds:0x51da44
  430dd6:	56                   	push   esi
  430dd7:	8b 74 24 08          	mov    esi,DWORD PTR [esp+0x8]
  430ddb:	83 f8 03             	cmp    eax,0x3
  430dde:	75 15                	jne    0x430df5
  430de0:	3b 35 3c da 51 00    	cmp    esi,DWORD PTR ds:0x51da3c
  430de6:	77 3f                	ja     0x430e27
  430de8:	56                   	push   esi
  430de9:	e8 aa 39 00 00       	call   0x434798
  430dee:	85 c0                	test   eax,eax
  430df0:	59                   	pop    ecx
  430df1:	74 34                	je     0x430e27
  430df3:	5e                   	pop    esi
  430df4:	c3                   	ret
  430df5:	83 f8 02             	cmp    eax,0x2
  430df8:	75 2d                	jne    0x430e27
  430dfa:	8b 44 24 08          	mov    eax,DWORD PTR [esp+0x8]
  430dfe:	85 c0                	test   eax,eax
  430e00:	74 08                	je     0x430e0a
  430e02:	8d 70 0f             	lea    esi,[eax+0xf]
  430e05:	83 e6 f0             	and    esi,0xfffffff0
  430e08:	eb 03                	jmp    0x430e0d
  430e0a:	6a 10                	push   0x10
  430e0c:	5e                   	pop    esi
  430e0d:	3b 35 6c 97 44 00    	cmp    esi,DWORD PTR ds:0x44976c
  430e13:	77 1f                	ja     0x430e34
  430e15:	8b c6                	mov    eax,esi
  430e17:	c1 e8 04             	shr    eax,0x4
  430e1a:	50                   	push   eax
  430e1b:	e8 1b 44 00 00       	call   0x43523b
  430e20:	85 c0                	test   eax,eax
  430e22:	59                   	pop    ecx
  430e23:	75 1e                	jne    0x430e43
  430e25:	eb 0d                	jmp    0x430e34
  430e27:	85 f6                	test   esi,esi
  430e29:	75 03                	jne    0x430e2e
  430e2b:	6a 01                	push   0x1
  430e2d:	5e                   	pop    esi
  430e2e:	83 c6 0f             	add    esi,0xf
  430e31:	83 e6 f0             	and    esi,0xfffffff0
  430e34:	56                   	push   esi
  430e35:	6a 00                	push   0x0
  430e37:	ff 35 40 da 51 00    	push   DWORD PTR ds:0x51da40
  430e3d:	ff 15 08 b1 43 00    	call   DWORD PTR ds:0x43b108
  430e43:	5e                   	pop    esi
  430e44:	c3                   	ret
