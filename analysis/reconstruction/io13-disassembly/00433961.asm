
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

00433961 <.text+0x32961>:
  433961:	55                   	push   ebp
  433962:	8b ec                	mov    ebp,esp
  433964:	83 ec 0c             	sub    esp,0xc
  433967:	53                   	push   ebx
  433968:	56                   	push   esi
  433969:	8b 75 08             	mov    esi,DWORD PTR [ebp+0x8]
  43396c:	57                   	push   edi
  43396d:	3b 35 20 da 51 00    	cmp    esi,DWORD PTR ds:0x51da20
  433973:	0f 83 c5 01 00 00    	jae    0x433b3e
  433979:	8b c6                	mov    eax,esi
  43397b:	83 e6 1f             	and    esi,0x1f
  43397e:	c1 f8 05             	sar    eax,0x5
  433981:	c1 e6 03             	shl    esi,0x3
  433984:	8d 1c 85 20 d9 51 00 	lea    ebx,[eax*4+0x51d920]
  43398b:	8b 04 85 20 d9 51 00 	mov    eax,DWORD PTR [eax*4+0x51d920]
  433992:	03 c6                	add    eax,esi
  433994:	8a 50 04             	mov    dl,BYTE PTR [eax+0x4]
  433997:	f6 c2 01             	test   dl,0x1
  43399a:	0f 84 9e 01 00 00    	je     0x433b3e
  4339a0:	83 65 f8 00          	and    DWORD PTR [ebp-0x8],0x0
  4339a4:	8b 7d 0c             	mov    edi,DWORD PTR [ebp+0xc]
  4339a7:	83 7d 10 00          	cmp    DWORD PTR [ebp+0x10],0x0
  4339ab:	8b cf                	mov    ecx,edi
  4339ad:	74 67                	je     0x433a16
  4339af:	f6 c2 02             	test   dl,0x2
  4339b2:	75 62                	jne    0x433a16
  4339b4:	f6 c2 48             	test   dl,0x48
  4339b7:	74 1d                	je     0x4339d6
  4339b9:	8a 40 05             	mov    al,BYTE PTR [eax+0x5]
  4339bc:	3c 0a                	cmp    al,0xa
  4339be:	74 16                	je     0x4339d6
  4339c0:	ff 4d 10             	dec    DWORD PTR [ebp+0x10]
  4339c3:	88 07                	mov    BYTE PTR [edi],al
  4339c5:	8b 03                	mov    eax,DWORD PTR [ebx]
  4339c7:	8d 4f 01             	lea    ecx,[edi+0x1]
  4339ca:	c7 45 f8 01 00 00 00 	mov    DWORD PTR [ebp-0x8],0x1
  4339d1:	c6 44 30 05 0a       	mov    BYTE PTR [eax+esi*1+0x5],0xa
  4339d6:	8d 45 f4             	lea    eax,[ebp-0xc]
  4339d9:	6a 00                	push   0x0
  4339db:	50                   	push   eax
  4339dc:	8b 03                	mov    eax,DWORD PTR [ebx]
  4339de:	ff 75 10             	push   DWORD PTR [ebp+0x10]
  4339e1:	51                   	push   ecx
  4339e2:	ff 34 30             	push   DWORD PTR [eax+esi*1]
  4339e5:	ff 15 54 b1 43 00    	call   DWORD PTR ds:0x43b154
  4339eb:	85 c0                	test   eax,eax
  4339ed:	75 3a                	jne    0x433a29
  4339ef:	ff 15 2c b1 43 00    	call   DWORD PTR ds:0x43b12c
  4339f5:	6a 05                	push   0x5
  4339f7:	59                   	pop    ecx
  4339f8:	3b c1                	cmp    eax,ecx
  4339fa:	75 15                	jne    0x433a11
  4339fc:	c7 05 c8 c3 51 00 09 	mov    DWORD PTR ds:0x51c3c8,0x9
  433a03:	00 00 00 
  433a06:	89 0d cc c3 51 00    	mov    DWORD PTR ds:0x51c3cc,ecx
  433a0c:	e9 3e 01 00 00       	jmp    0x433b4f
  433a11:	83 f8 6d             	cmp    eax,0x6d
  433a14:	75 07                	jne    0x433a1d
  433a16:	33 c0                	xor    eax,eax
  433a18:	e9 35 01 00 00       	jmp    0x433b52
  433a1d:	50                   	push   eax
  433a1e:	e8 57 ed ff ff       	call   0x43277a
  433a23:	59                   	pop    ecx
  433a24:	e9 26 01 00 00       	jmp    0x433b4f
  433a29:	8b 03                	mov    eax,DWORD PTR [ebx]
  433a2b:	8b 55 f4             	mov    edx,DWORD PTR [ebp-0xc]
  433a2e:	01 55 f8             	add    DWORD PTR [ebp-0x8],edx
  433a31:	8d 4c 30 04          	lea    ecx,[eax+esi*1+0x4]
  433a35:	8a 44 30 04          	mov    al,BYTE PTR [eax+esi*1+0x4]
  433a39:	a8 80                	test   al,0x80
  433a3b:	0f 84 f8 00 00 00    	je     0x433b39
  433a41:	85 d2                	test   edx,edx
  433a43:	74 09                	je     0x433a4e
  433a45:	80 3f 0a             	cmp    BYTE PTR [edi],0xa
  433a48:	75 04                	jne    0x433a4e
  433a4a:	0c 04                	or     al,0x4
  433a4c:	eb 02                	jmp    0x433a50
  433a4e:	24 fb                	and    al,0xfb
  433a50:	88 01                	mov    BYTE PTR [ecx],al
  433a52:	8b 45 0c             	mov    eax,DWORD PTR [ebp+0xc]
  433a55:	8b 4d f8             	mov    ecx,DWORD PTR [ebp-0x8]
  433a58:	89 45 10             	mov    DWORD PTR [ebp+0x10],eax
  433a5b:	03 c8                	add    ecx,eax
  433a5d:	3b c1                	cmp    eax,ecx
  433a5f:	89 4d f8             	mov    DWORD PTR [ebp-0x8],ecx
  433a62:	0f 83 cb 00 00 00    	jae    0x433b33
  433a68:	8b 45 10             	mov    eax,DWORD PTR [ebp+0x10]
  433a6b:	8a 00                	mov    al,BYTE PTR [eax]
  433a6d:	3c 1a                	cmp    al,0x1a
  433a6f:	0f 84 ae 00 00 00    	je     0x433b23
  433a75:	3c 0d                	cmp    al,0xd
  433a77:	74 0b                	je     0x433a84
  433a79:	88 07                	mov    BYTE PTR [edi],al
  433a7b:	47                   	inc    edi
  433a7c:	ff 45 10             	inc    DWORD PTR [ebp+0x10]
  433a7f:	e9 91 00 00 00       	jmp    0x433b15
  433a84:	49                   	dec    ecx
  433a85:	39 4d 10             	cmp    DWORD PTR [ebp+0x10],ecx
  433a88:	73 18                	jae    0x433aa2
  433a8a:	8b 45 10             	mov    eax,DWORD PTR [ebp+0x10]
  433a8d:	40                   	inc    eax
  433a8e:	80 38 0a             	cmp    BYTE PTR [eax],0xa
  433a91:	75 06                	jne    0x433a99
  433a93:	83 45 10 02          	add    DWORD PTR [ebp+0x10],0x2
  433a97:	eb 5e                	jmp    0x433af7
  433a99:	c6 07 0d             	mov    BYTE PTR [edi],0xd
  433a9c:	47                   	inc    edi
  433a9d:	89 45 10             	mov    DWORD PTR [ebp+0x10],eax
  433aa0:	eb 73                	jmp    0x433b15
  433aa2:	8d 45 f4             	lea    eax,[ebp-0xc]
  433aa5:	6a 00                	push   0x0
  433aa7:	50                   	push   eax
  433aa8:	ff 45 10             	inc    DWORD PTR [ebp+0x10]
  433aab:	8d 45 ff             	lea    eax,[ebp-0x1]
  433aae:	6a 01                	push   0x1
  433ab0:	50                   	push   eax
  433ab1:	8b 03                	mov    eax,DWORD PTR [ebx]
  433ab3:	ff 34 30             	push   DWORD PTR [eax+esi*1]
  433ab6:	ff 15 54 b1 43 00    	call   DWORD PTR ds:0x43b154
  433abc:	85 c0                	test   eax,eax
  433abe:	75 0a                	jne    0x433aca
  433ac0:	ff 15 2c b1 43 00    	call   DWORD PTR ds:0x43b12c
  433ac6:	85 c0                	test   eax,eax
  433ac8:	75 47                	jne    0x433b11
  433aca:	83 7d f4 00          	cmp    DWORD PTR [ebp-0xc],0x0
  433ace:	74 41                	je     0x433b11
  433ad0:	8b 03                	mov    eax,DWORD PTR [ebx]
  433ad2:	f6 44 30 04 48       	test   BYTE PTR [eax+esi*1+0x4],0x48
  433ad7:	74 13                	je     0x433aec
  433ad9:	8a 45 ff             	mov    al,BYTE PTR [ebp-0x1]
  433adc:	3c 0a                	cmp    al,0xa
  433ade:	74 17                	je     0x433af7
  433ae0:	c6 07 0d             	mov    BYTE PTR [edi],0xd
  433ae3:	8b 0b                	mov    ecx,DWORD PTR [ebx]
  433ae5:	47                   	inc    edi
  433ae6:	88 44 31 05          	mov    BYTE PTR [ecx+esi*1+0x5],al
  433aea:	eb 29                	jmp    0x433b15
  433aec:	3b 7d 0c             	cmp    edi,DWORD PTR [ebp+0xc]
  433aef:	75 0b                	jne    0x433afc
  433af1:	80 7d ff 0a          	cmp    BYTE PTR [ebp-0x1],0xa
  433af5:	75 05                	jne    0x433afc
  433af7:	c6 07 0a             	mov    BYTE PTR [edi],0xa
  433afa:	eb 18                	jmp    0x433b14
  433afc:	6a 01                	push   0x1
  433afe:	6a ff                	push   0xffffffff
  433b00:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  433b03:	e8 cf 37 00 00       	call   0x4372d7
  433b08:	83 c4 0c             	add    esp,0xc
  433b0b:	80 7d ff 0a          	cmp    BYTE PTR [ebp-0x1],0xa
  433b0f:	74 04                	je     0x433b15
  433b11:	c6 07 0d             	mov    BYTE PTR [edi],0xd
  433b14:	47                   	inc    edi
  433b15:	8b 4d f8             	mov    ecx,DWORD PTR [ebp-0x8]
  433b18:	39 4d 10             	cmp    DWORD PTR [ebp+0x10],ecx
  433b1b:	0f 82 47 ff ff ff    	jb     0x433a68
  433b21:	eb 10                	jmp    0x433b33
  433b23:	8b 03                	mov    eax,DWORD PTR [ebx]
  433b25:	8d 74 30 04          	lea    esi,[eax+esi*1+0x4]
  433b29:	8a 06                	mov    al,BYTE PTR [esi]
  433b2b:	a8 40                	test   al,0x40
  433b2d:	75 04                	jne    0x433b33
  433b2f:	0c 02                	or     al,0x2
  433b31:	88 06                	mov    BYTE PTR [esi],al
  433b33:	2b 7d 0c             	sub    edi,DWORD PTR [ebp+0xc]
  433b36:	89 7d f8             	mov    DWORD PTR [ebp-0x8],edi
  433b39:	8b 45 f8             	mov    eax,DWORD PTR [ebp-0x8]
  433b3c:	eb 14                	jmp    0x433b52
  433b3e:	83 25 cc c3 51 00 00 	and    DWORD PTR ds:0x51c3cc,0x0
  433b45:	c7 05 c8 c3 51 00 09 	mov    DWORD PTR ds:0x51c3c8,0x9
  433b4c:	00 00 00 
  433b4f:	83 c8 ff             	or     eax,0xffffffff
  433b52:	5f                   	pop    edi
  433b53:	5e                   	pop    esi
  433b54:	5b                   	pop    ebx
  433b55:	c9                   	leave
  433b56:	c3                   	ret
