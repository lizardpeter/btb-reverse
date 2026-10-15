
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00431e60 <.text+0x30e60>:
  431e60:	55                   	push   ebp
  431e61:	8b ec                	mov    ebp,esp
  431e63:	83 ec 18             	sub    esp,0x18
  431e66:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  431e69:	80 65 ec 00          	and    BYTE PTR [ebp-0x14],0x0
  431e6d:	8b 40 08             	mov    eax,DWORD PTR [eax+0x8]
  431e70:	83 f8 ff             	cmp    eax,0xffffffff
  431e73:	89 45 f0             	mov    DWORD PTR [ebp-0x10],eax
  431e76:	7c 08                	jl     0x431e80
  431e78:	8b 4d 18             	mov    ecx,DWORD PTR [ebp+0x18]
  431e7b:	3b 41 04             	cmp    eax,DWORD PTR [ecx+0x4]
  431e7e:	7c 05                	jl     0x431e85
  431e80:	e8 dd 07 00 00       	call   0x432662
  431e85:	53                   	push   ebx
  431e86:	56                   	push   esi
  431e87:	8b 75 08             	mov    esi,DWORD PTR [ebp+0x8]
  431e8a:	bb 63 73 6d e0       	mov    ebx,0xe06d7363
  431e8f:	57                   	push   edi
  431e90:	bf 20 05 93 19       	mov    edi,0x19930520
  431e95:	39 1e                	cmp    DWORD PTR [esi],ebx
  431e97:	0f 85 3f 01 00 00    	jne    0x431fdc
  431e9d:	83 7e 10 03          	cmp    DWORD PTR [esi+0x10],0x3
  431ea1:	75 56                	jne    0x431ef9
  431ea3:	39 7e 14             	cmp    DWORD PTR [esi+0x14],edi
  431ea6:	75 51                	jne    0x431ef9
  431ea8:	83 7e 1c 00          	cmp    DWORD PTR [esi+0x1c],0x0
  431eac:	75 4b                	jne    0x431ef9
  431eae:	8b 35 20 c4 51 00    	mov    esi,DWORD PTR ds:0x51c420
  431eb4:	85 f6                	test   esi,esi
  431eb6:	0f 84 1b 01 00 00    	je     0x431fd7
  431ebc:	a1 24 c4 51 00       	mov    eax,ds:0x51c424
  431ec1:	6a 01                	push   0x1
  431ec3:	56                   	push   esi
  431ec4:	89 45 10             	mov    DWORD PTR [ebp+0x10],eax
  431ec7:	c6 45 ec 01          	mov    BYTE PTR [ebp-0x14],0x1
  431ecb:	e8 3d 48 00 00       	call   0x43670d
  431ed0:	59                   	pop    ecx
  431ed1:	85 c0                	test   eax,eax
  431ed3:	59                   	pop    ecx
  431ed4:	75 05                	jne    0x431edb
  431ed6:	e8 87 07 00 00       	call   0x432662
  431edb:	39 1e                	cmp    DWORD PTR [esi],ebx
  431edd:	0f 85 f9 00 00 00    	jne    0x431fdc
  431ee3:	83 7e 10 03          	cmp    DWORD PTR [esi+0x10],0x3
  431ee7:	75 10                	jne    0x431ef9
  431ee9:	39 7e 14             	cmp    DWORD PTR [esi+0x14],edi
  431eec:	75 0b                	jne    0x431ef9
  431eee:	83 7e 1c 00          	cmp    DWORD PTR [esi+0x1c],0x0
  431ef2:	75 05                	jne    0x431ef9
  431ef4:	e8 69 07 00 00       	call   0x432662
  431ef9:	39 1e                	cmp    DWORD PTR [esi],ebx
  431efb:	0f 85 db 00 00 00    	jne    0x431fdc
  431f01:	83 7e 10 03          	cmp    DWORD PTR [esi+0x10],0x3
  431f05:	0f 85 d1 00 00 00    	jne    0x431fdc
  431f0b:	39 7e 14             	cmp    DWORD PTR [esi+0x14],edi
  431f0e:	0f 85 c8 00 00 00    	jne    0x431fdc
  431f14:	8b 7d f0             	mov    edi,DWORD PTR [ebp-0x10]
  431f17:	8d 45 e8             	lea    eax,[ebp-0x18]
  431f1a:	50                   	push   eax
  431f1b:	8d 45 fc             	lea    eax,[ebp-0x4]
  431f1e:	50                   	push   eax
  431f1f:	57                   	push   edi
  431f20:	ff 75 20             	push   DWORD PTR [ebp+0x20]
  431f23:	ff 75 18             	push   DWORD PTR [ebp+0x18]
  431f26:	e8 25 df ff ff       	call   0x42fe50
  431f2b:	83 c4 14             	add    esp,0x14
  431f2e:	8b d8                	mov    ebx,eax
  431f30:	8b 45 fc             	mov    eax,DWORD PTR [ebp-0x4]
  431f33:	3b 45 e8             	cmp    eax,DWORD PTR [ebp-0x18]
  431f36:	0f 83 8b 00 00 00    	jae    0x431fc7
  431f3c:	39 3b                	cmp    DWORD PTR [ebx],edi
  431f3e:	7f 7c                	jg     0x431fbc
  431f40:	3b 7b 04             	cmp    edi,DWORD PTR [ebx+0x4]
  431f43:	7f 77                	jg     0x431fbc
  431f45:	8b 43 10             	mov    eax,DWORD PTR [ebx+0x10]
  431f48:	89 45 08             	mov    DWORD PTR [ebp+0x8],eax
  431f4b:	8b 43 0c             	mov    eax,DWORD PTR [ebx+0xc]
  431f4e:	85 c0                	test   eax,eax
  431f50:	89 45 f4             	mov    DWORD PTR [ebp-0xc],eax
  431f53:	7e 64                	jle    0x431fb9
  431f55:	8b 46 1c             	mov    eax,DWORD PTR [esi+0x1c]
  431f58:	8b 40 0c             	mov    eax,DWORD PTR [eax+0xc]
  431f5b:	8d 78 04             	lea    edi,[eax+0x4]
  431f5e:	8b 00                	mov    eax,DWORD PTR [eax]
  431f60:	85 c0                	test   eax,eax
  431f62:	89 45 f8             	mov    DWORD PTR [ebp-0x8],eax
  431f65:	7e 1f                	jle    0x431f86
  431f67:	ff 76 1c             	push   DWORD PTR [esi+0x1c]
  431f6a:	ff 37                	push   DWORD PTR [edi]
  431f6c:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  431f6f:	e8 3b 01 00 00       	call   0x4320af
  431f74:	83 c4 0c             	add    esp,0xc
  431f77:	85 c0                	test   eax,eax
  431f79:	75 1a                	jne    0x431f95
  431f7b:	ff 4d f8             	dec    DWORD PTR [ebp-0x8]
  431f7e:	83 c7 04             	add    edi,0x4
  431f81:	39 45 f8             	cmp    DWORD PTR [ebp-0x8],eax
  431f84:	7f e1                	jg     0x431f67
  431f86:	ff 4d f4             	dec    DWORD PTR [ebp-0xc]
  431f89:	83 45 08 10          	add    DWORD PTR [ebp+0x8],0x10
  431f8d:	83 7d f4 00          	cmp    DWORD PTR [ebp-0xc],0x0
  431f91:	7f c2                	jg     0x431f55
  431f93:	eb 24                	jmp    0x431fb9
  431f95:	ff 75 ec             	push   DWORD PTR [ebp-0x14]
  431f98:	ff 75 24             	push   DWORD PTR [ebp+0x24]
  431f9b:	ff 75 20             	push   DWORD PTR [ebp+0x20]
  431f9e:	53                   	push   ebx
  431f9f:	ff 37                	push   DWORD PTR [edi]
  431fa1:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  431fa4:	ff 75 18             	push   DWORD PTR [ebp+0x18]
  431fa7:	ff 75 14             	push   DWORD PTR [ebp+0x14]
  431faa:	ff 75 10             	push   DWORD PTR [ebp+0x10]
  431fad:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  431fb0:	56                   	push   esi
  431fb1:	e8 0a 02 00 00       	call   0x4321c0
  431fb6:	83 c4 2c             	add    esp,0x2c
  431fb9:	8b 7d f0             	mov    edi,DWORD PTR [ebp-0x10]
  431fbc:	ff 45 fc             	inc    DWORD PTR [ebp-0x4]
  431fbf:	83 c3 14             	add    ebx,0x14
  431fc2:	e9 69 ff ff ff       	jmp    0x431f30
  431fc7:	80 7d 1c 00          	cmp    BYTE PTR [ebp+0x1c],0x0
  431fcb:	74 0a                	je     0x431fd7
  431fcd:	6a 01                	push   0x1
  431fcf:	56                   	push   esi
  431fd0:	e8 60 05 00 00       	call   0x432535
  431fd5:	59                   	pop    ecx
  431fd6:	59                   	pop    ecx
  431fd7:	5f                   	pop    edi
  431fd8:	5e                   	pop    esi
  431fd9:	5b                   	pop    ebx
  431fda:	c9                   	leave
  431fdb:	c3                   	ret
  431fdc:	80 7d 1c 00          	cmp    BYTE PTR [ebp+0x1c],0x0
  431fe0:	75 20                	jne    0x432002
  431fe2:	ff 75 24             	push   DWORD PTR [ebp+0x24]
  431fe5:	ff 75 20             	push   DWORD PTR [ebp+0x20]
  431fe8:	ff 75 f0             	push   DWORD PTR [ebp-0x10]
  431feb:	ff 75 18             	push   DWORD PTR [ebp+0x18]
  431fee:	ff 75 14             	push   DWORD PTR [ebp+0x14]
  431ff1:	ff 75 10             	push   DWORD PTR [ebp+0x10]
  431ff4:	ff 75 0c             	push   DWORD PTR [ebp+0xc]
  431ff7:	56                   	push   esi
  431ff8:	e8 0a 00 00 00       	call   0x432007
  431ffd:	83 c4 20             	add    esp,0x20
  432000:	eb d5                	jmp    0x431fd7
  432002:	e9 05 06 00 00       	jmp    0x43260c
