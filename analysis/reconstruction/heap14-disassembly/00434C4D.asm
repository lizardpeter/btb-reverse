
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00434c4d <.text+0x33c4d>:
  434c4d:	55                   	push   ebp
  434c4e:	8b ec                	mov    ebp,esp
  434c50:	83 ec 0c             	sub    esp,0xc
  434c53:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  434c56:	8b 45 10             	mov    eax,DWORD PTR [ebp+0x10]
  434c59:	53                   	push   ebx
  434c5a:	56                   	push   esi
  434c5b:	57                   	push   edi
  434c5c:	8b 7d 0c             	mov    edi,DWORD PTR [ebp+0xc]
  434c5f:	8b d7                	mov    edx,edi
  434c61:	8d 70 17             	lea    esi,[eax+0x17]
  434c64:	2b 51 0c             	sub    edx,DWORD PTR [ecx+0xc]
  434c67:	8b 41 10             	mov    eax,DWORD PTR [ecx+0x10]
  434c6a:	83 e6 f0             	and    esi,0xfffffff0
  434c6d:	c1 ea 0f             	shr    edx,0xf
  434c70:	8b ca                	mov    ecx,edx
  434c72:	69 c9 04 02 00 00    	imul   ecx,ecx,0x204
  434c78:	8d 8c 01 44 01 00 00 	lea    ecx,[ecx+eax*1+0x144]
  434c7f:	89 4d f4             	mov    DWORD PTR [ebp-0xc],ecx
  434c82:	8b 4f fc             	mov    ecx,DWORD PTR [edi-0x4]
  434c85:	49                   	dec    ecx
  434c86:	3b f1                	cmp    esi,ecx
  434c88:	89 4d 10             	mov    DWORD PTR [ebp+0x10],ecx
  434c8b:	8b 5c 39 fc          	mov    ebx,DWORD PTR [ecx+edi*1-0x4]
  434c8f:	8d 7c 39 fc          	lea    edi,[ecx+edi*1-0x4]
  434c93:	89 5d fc             	mov    DWORD PTR [ebp-0x4],ebx
  434c96:	0f 8e 5f 01 00 00    	jle    0x434dfb
  434c9c:	f6 c3 01             	test   bl,0x1
  434c9f:	0f 85 4f 01 00 00    	jne    0x434df4
  434ca5:	03 d9                	add    ebx,ecx
  434ca7:	3b f3                	cmp    esi,ebx
  434ca9:	0f 8f 45 01 00 00    	jg     0x434df4
  434caf:	8b 4d fc             	mov    ecx,DWORD PTR [ebp-0x4]
  434cb2:	c1 f9 04             	sar    ecx,0x4
  434cb5:	49                   	dec    ecx
  434cb6:	83 f9 3f             	cmp    ecx,0x3f
  434cb9:	89 4d f8             	mov    DWORD PTR [ebp-0x8],ecx
  434cbc:	76 06                	jbe    0x434cc4
  434cbe:	6a 3f                	push   0x3f
  434cc0:	59                   	pop    ecx
  434cc1:	89 4d f8             	mov    DWORD PTR [ebp-0x8],ecx
  434cc4:	8b 5f 04             	mov    ebx,DWORD PTR [edi+0x4]
  434cc7:	3b 5f 08             	cmp    ebx,DWORD PTR [edi+0x8]
  434cca:	75 48                	jne    0x434d14
  434ccc:	83 f9 20             	cmp    ecx,0x20
  434ccf:	73 1f                	jae    0x434cf0
  434cd1:	bb 00 00 00 80       	mov    ebx,0x80000000
  434cd6:	d3 eb                	shr    ebx,cl
  434cd8:	8b 4d f8             	mov    ecx,DWORD PTR [ebp-0x8]
  434cdb:	8d 4c 01 04          	lea    ecx,[ecx+eax*1+0x4]
  434cdf:	f7 d3                	not    ebx
  434ce1:	21 5c 90 44          	and    DWORD PTR [eax+edx*4+0x44],ebx
  434ce5:	fe 09                	dec    BYTE PTR [ecx]
  434ce7:	75 2b                	jne    0x434d14
  434ce9:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  434cec:	21 19                	and    DWORD PTR [ecx],ebx
  434cee:	eb 24                	jmp    0x434d14
  434cf0:	83 c1 e0             	add    ecx,0xffffffe0
  434cf3:	bb 00 00 00 80       	mov    ebx,0x80000000
  434cf8:	d3 eb                	shr    ebx,cl
  434cfa:	8b 4d f8             	mov    ecx,DWORD PTR [ebp-0x8]
  434cfd:	8d 4c 01 04          	lea    ecx,[ecx+eax*1+0x4]
  434d01:	f7 d3                	not    ebx
  434d03:	21 9c 90 c4 00 00 00 	and    DWORD PTR [eax+edx*4+0xc4],ebx
  434d0a:	fe 09                	dec    BYTE PTR [ecx]
  434d0c:	75 06                	jne    0x434d14
  434d0e:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  434d11:	21 59 04             	and    DWORD PTR [ecx+0x4],ebx
  434d14:	8b 4f 08             	mov    ecx,DWORD PTR [edi+0x8]
  434d17:	8b 5f 04             	mov    ebx,DWORD PTR [edi+0x4]
  434d1a:	89 59 04             	mov    DWORD PTR [ecx+0x4],ebx
  434d1d:	8b 4f 04             	mov    ecx,DWORD PTR [edi+0x4]
  434d20:	8b 7f 08             	mov    edi,DWORD PTR [edi+0x8]
  434d23:	89 79 08             	mov    DWORD PTR [ecx+0x8],edi
  434d26:	8b 4d 10             	mov    ecx,DWORD PTR [ebp+0x10]
  434d29:	2b ce                	sub    ecx,esi
  434d2b:	01 4d fc             	add    DWORD PTR [ebp-0x4],ecx
  434d2e:	83 7d fc 00          	cmp    DWORD PTR [ebp-0x4],0x0
  434d32:	0f 8e aa 00 00 00    	jle    0x434de2
  434d38:	8b 7d fc             	mov    edi,DWORD PTR [ebp-0x4]
  434d3b:	8b 4d 0c             	mov    ecx,DWORD PTR [ebp+0xc]
  434d3e:	c1 ff 04             	sar    edi,0x4
  434d41:	4f                   	dec    edi
  434d42:	8d 4c 31 fc          	lea    ecx,[ecx+esi*1-0x4]
  434d46:	83 ff 3f             	cmp    edi,0x3f
  434d49:	76 03                	jbe    0x434d4e
  434d4b:	6a 3f                	push   0x3f
  434d4d:	5f                   	pop    edi
  434d4e:	8b 5d f4             	mov    ebx,DWORD PTR [ebp-0xc]
  434d51:	8d 1c fb             	lea    ebx,[ebx+edi*8]
  434d54:	89 5d 10             	mov    DWORD PTR [ebp+0x10],ebx
  434d57:	8b 5b 04             	mov    ebx,DWORD PTR [ebx+0x4]
  434d5a:	89 59 04             	mov    DWORD PTR [ecx+0x4],ebx
  434d5d:	8b 5d 10             	mov    ebx,DWORD PTR [ebp+0x10]
  434d60:	89 59 08             	mov    DWORD PTR [ecx+0x8],ebx
  434d63:	89 4b 04             	mov    DWORD PTR [ebx+0x4],ecx
  434d66:	8b 59 04             	mov    ebx,DWORD PTR [ecx+0x4]
  434d69:	89 4b 08             	mov    DWORD PTR [ebx+0x8],ecx
  434d6c:	8b 59 04             	mov    ebx,DWORD PTR [ecx+0x4]
  434d6f:	3b 59 08             	cmp    ebx,DWORD PTR [ecx+0x8]
  434d72:	75 5c                	jne    0x434dd0
  434d74:	8a 4c 07 04          	mov    cl,BYTE PTR [edi+eax*1+0x4]
  434d78:	83 ff 20             	cmp    edi,0x20
  434d7b:	88 4d 13             	mov    BYTE PTR [ebp+0x13],cl
  434d7e:	fe c1                	inc    cl
  434d80:	88 4c 07 04          	mov    BYTE PTR [edi+eax*1+0x4],cl
  434d84:	73 21                	jae    0x434da7
  434d86:	80 7d 13 00          	cmp    BYTE PTR [ebp+0x13],0x0
  434d8a:	75 0e                	jne    0x434d9a
  434d8c:	bb 00 00 00 80       	mov    ebx,0x80000000
  434d91:	8b cf                	mov    ecx,edi
  434d93:	d3 eb                	shr    ebx,cl
  434d95:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  434d98:	09 19                	or     DWORD PTR [ecx],ebx
  434d9a:	8d 44 90 44          	lea    eax,[eax+edx*4+0x44]
  434d9e:	ba 00 00 00 80       	mov    edx,0x80000000
  434da3:	8b cf                	mov    ecx,edi
  434da5:	eb 25                	jmp    0x434dcc
  434da7:	80 7d 13 00          	cmp    BYTE PTR [ebp+0x13],0x0
  434dab:	75 10                	jne    0x434dbd
  434dad:	8d 4f e0             	lea    ecx,[edi-0x20]
  434db0:	bb 00 00 00 80       	mov    ebx,0x80000000
  434db5:	d3 eb                	shr    ebx,cl
  434db7:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  434dba:	09 59 04             	or     DWORD PTR [ecx+0x4],ebx
  434dbd:	8d 84 90 c4 00 00 00 	lea    eax,[eax+edx*4+0xc4]
  434dc4:	8d 4f e0             	lea    ecx,[edi-0x20]
  434dc7:	ba 00 00 00 80       	mov    edx,0x80000000
  434dcc:	d3 ea                	shr    edx,cl
  434dce:	09 10                	or     DWORD PTR [eax],edx
  434dd0:	8b 55 0c             	mov    edx,DWORD PTR [ebp+0xc]
  434dd3:	8b 4d fc             	mov    ecx,DWORD PTR [ebp-0x4]
  434dd6:	8d 44 32 fc          	lea    eax,[edx+esi*1-0x4]
  434dda:	89 08                	mov    DWORD PTR [eax],ecx
  434ddc:	89 4c 01 fc          	mov    DWORD PTR [ecx+eax*1-0x4],ecx
  434de0:	eb 03                	jmp    0x434de5
  434de2:	8b 55 0c             	mov    edx,DWORD PTR [ebp+0xc]
  434de5:	8d 46 01             	lea    eax,[esi+0x1]
  434de8:	89 42 fc             	mov    DWORD PTR [edx-0x4],eax
  434deb:	89 44 32 f8          	mov    DWORD PTR [edx+esi*1-0x8],eax
  434def:	e9 47 01 00 00       	jmp    0x434f3b
  434df4:	33 c0                	xor    eax,eax
  434df6:	e9 43 01 00 00       	jmp    0x434f3e
  434dfb:	0f 8d 3a 01 00 00    	jge    0x434f3b
  434e01:	8b 5d 0c             	mov    ebx,DWORD PTR [ebp+0xc]
  434e04:	29 75 10             	sub    DWORD PTR [ebp+0x10],esi
  434e07:	8d 4e 01             	lea    ecx,[esi+0x1]
  434e0a:	89 4b fc             	mov    DWORD PTR [ebx-0x4],ecx
  434e0d:	8d 5c 33 fc          	lea    ebx,[ebx+esi*1-0x4]
  434e11:	8b 75 10             	mov    esi,DWORD PTR [ebp+0x10]
  434e14:	89 5d 0c             	mov    DWORD PTR [ebp+0xc],ebx
  434e17:	c1 fe 04             	sar    esi,0x4
  434e1a:	4e                   	dec    esi
  434e1b:	89 4b fc             	mov    DWORD PTR [ebx-0x4],ecx
  434e1e:	83 fe 3f             	cmp    esi,0x3f
  434e21:	76 03                	jbe    0x434e26
  434e23:	6a 3f                	push   0x3f
  434e25:	5e                   	pop    esi
  434e26:	f6 45 fc 01          	test   BYTE PTR [ebp-0x4],0x1
  434e2a:	0f 85 85 00 00 00    	jne    0x434eb5
  434e30:	8b 75 fc             	mov    esi,DWORD PTR [ebp-0x4]
  434e33:	c1 fe 04             	sar    esi,0x4
  434e36:	4e                   	dec    esi
  434e37:	83 fe 3f             	cmp    esi,0x3f
  434e3a:	76 03                	jbe    0x434e3f
  434e3c:	6a 3f                	push   0x3f
  434e3e:	5e                   	pop    esi
  434e3f:	8b 4f 04             	mov    ecx,DWORD PTR [edi+0x4]
  434e42:	3b 4f 08             	cmp    ecx,DWORD PTR [edi+0x8]
  434e45:	75 47                	jne    0x434e8e
  434e47:	83 fe 20             	cmp    esi,0x20
  434e4a:	73 1e                	jae    0x434e6a
  434e4c:	bb 00 00 00 80       	mov    ebx,0x80000000
  434e51:	8b ce                	mov    ecx,esi
  434e53:	d3 eb                	shr    ebx,cl
  434e55:	8d 74 06 04          	lea    esi,[esi+eax*1+0x4]
  434e59:	f7 d3                	not    ebx
  434e5b:	21 5c 90 44          	and    DWORD PTR [eax+edx*4+0x44],ebx
  434e5f:	fe 0e                	dec    BYTE PTR [esi]
  434e61:	75 28                	jne    0x434e8b
  434e63:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  434e66:	21 19                	and    DWORD PTR [ecx],ebx
  434e68:	eb 21                	jmp    0x434e8b
  434e6a:	8d 4e e0             	lea    ecx,[esi-0x20]
  434e6d:	bb 00 00 00 80       	mov    ebx,0x80000000
  434e72:	d3 eb                	shr    ebx,cl
  434e74:	8d 4c 06 04          	lea    ecx,[esi+eax*1+0x4]
  434e78:	f7 d3                	not    ebx
  434e7a:	21 9c 90 c4 00 00 00 	and    DWORD PTR [eax+edx*4+0xc4],ebx
  434e81:	fe 09                	dec    BYTE PTR [ecx]
  434e83:	75 06                	jne    0x434e8b
  434e85:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  434e88:	21 59 04             	and    DWORD PTR [ecx+0x4],ebx
  434e8b:	8b 5d 0c             	mov    ebx,DWORD PTR [ebp+0xc]
  434e8e:	8b 4f 08             	mov    ecx,DWORD PTR [edi+0x8]
  434e91:	8b 77 04             	mov    esi,DWORD PTR [edi+0x4]
  434e94:	89 71 04             	mov    DWORD PTR [ecx+0x4],esi
  434e97:	8b 4f 04             	mov    ecx,DWORD PTR [edi+0x4]
  434e9a:	8b 77 08             	mov    esi,DWORD PTR [edi+0x8]
  434e9d:	89 71 08             	mov    DWORD PTR [ecx+0x8],esi
  434ea0:	8b 75 10             	mov    esi,DWORD PTR [ebp+0x10]
  434ea3:	03 75 fc             	add    esi,DWORD PTR [ebp-0x4]
  434ea6:	89 75 10             	mov    DWORD PTR [ebp+0x10],esi
  434ea9:	c1 fe 04             	sar    esi,0x4
  434eac:	4e                   	dec    esi
  434ead:	83 fe 3f             	cmp    esi,0x3f
  434eb0:	76 03                	jbe    0x434eb5
  434eb2:	6a 3f                	push   0x3f
  434eb4:	5e                   	pop    esi
  434eb5:	8b 4d f4             	mov    ecx,DWORD PTR [ebp-0xc]
  434eb8:	8b 7c f1 04          	mov    edi,DWORD PTR [ecx+esi*8+0x4]
  434ebc:	8d 0c f1             	lea    ecx,[ecx+esi*8]
  434ebf:	89 7b 04             	mov    DWORD PTR [ebx+0x4],edi
  434ec2:	89 4b 08             	mov    DWORD PTR [ebx+0x8],ecx
  434ec5:	89 59 04             	mov    DWORD PTR [ecx+0x4],ebx
  434ec8:	8b 4b 04             	mov    ecx,DWORD PTR [ebx+0x4]
  434ecb:	89 59 08             	mov    DWORD PTR [ecx+0x8],ebx
  434ece:	8b 4b 04             	mov    ecx,DWORD PTR [ebx+0x4]
  434ed1:	3b 4b 08             	cmp    ecx,DWORD PTR [ebx+0x8]
  434ed4:	75 5c                	jne    0x434f32
  434ed6:	8a 4c 06 04          	mov    cl,BYTE PTR [esi+eax*1+0x4]
  434eda:	83 fe 20             	cmp    esi,0x20
  434edd:	88 4d 0f             	mov    BYTE PTR [ebp+0xf],cl
  434ee0:	fe c1                	inc    cl
  434ee2:	88 4c 06 04          	mov    BYTE PTR [esi+eax*1+0x4],cl
  434ee6:	73 21                	jae    0x434f09
  434ee8:	80 7d 0f 00          	cmp    BYTE PTR [ebp+0xf],0x0
  434eec:	75 0e                	jne    0x434efc
  434eee:	bf 00 00 00 80       	mov    edi,0x80000000
  434ef3:	8b ce                	mov    ecx,esi
  434ef5:	d3 ef                	shr    edi,cl
  434ef7:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  434efa:	09 39                	or     DWORD PTR [ecx],edi
  434efc:	8d 44 90 44          	lea    eax,[eax+edx*4+0x44]
  434f00:	ba 00 00 00 80       	mov    edx,0x80000000
  434f05:	8b ce                	mov    ecx,esi
  434f07:	eb 25                	jmp    0x434f2e
  434f09:	80 7d 0f 00          	cmp    BYTE PTR [ebp+0xf],0x0
  434f0d:	75 10                	jne    0x434f1f
  434f0f:	8d 4e e0             	lea    ecx,[esi-0x20]
  434f12:	bf 00 00 00 80       	mov    edi,0x80000000
  434f17:	d3 ef                	shr    edi,cl
  434f19:	8b 4d 08             	mov    ecx,DWORD PTR [ebp+0x8]
  434f1c:	09 79 04             	or     DWORD PTR [ecx+0x4],edi
  434f1f:	8d 84 90 c4 00 00 00 	lea    eax,[eax+edx*4+0xc4]
  434f26:	8d 4e e0             	lea    ecx,[esi-0x20]
  434f29:	ba 00 00 00 80       	mov    edx,0x80000000
  434f2e:	d3 ea                	shr    edx,cl
  434f30:	09 10                	or     DWORD PTR [eax],edx
  434f32:	8b 45 10             	mov    eax,DWORD PTR [ebp+0x10]
  434f35:	89 03                	mov    DWORD PTR [ebx],eax
  434f37:	89 44 18 fc          	mov    DWORD PTR [eax+ebx*1-0x4],eax
  434f3b:	6a 01                	push   0x1
  434f3d:	58                   	pop    eax
  434f3e:	5f                   	pop    edi
  434f3f:	5e                   	pop    esi
  434f40:	5b                   	pop    ebx
  434f41:	c9                   	leave
  434f42:	c3                   	ret
