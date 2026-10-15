
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

0043a2f5 <.text+0x392f5>:
  43a2f5:	55                   	push   ebp
  43a2f6:	8b ec                	mov    ebp,esp
  43a2f8:	51                   	push   ecx
  43a2f9:	51                   	push   ecx
  43a2fa:	a1 44 da 51 00       	mov    eax,ds:0x51da44
  43a2ff:	56                   	push   esi
  43a300:	83 f8 03             	cmp    eax,0x3
  43a303:	75 19                	jne    0x43a31e
  43a305:	8b 75 08             	mov    esi,DWORD PTR [ebp+0x8]
  43a308:	56                   	push   esi
  43a309:	e8 36 a1 ff ff       	call   0x434444
  43a30e:	85 c0                	test   eax,eax
  43a310:	59                   	pop    ecx
  43a311:	74 08                	je     0x43a31b
  43a313:	8b 46 fc             	mov    eax,DWORD PTR [esi-0x4]
  43a316:	83 e8 09             	sub    eax,0x9
  43a319:	eb 38                	jmp    0x43a353
  43a31b:	56                   	push   esi
  43a31c:	eb 27                	jmp    0x43a345
  43a31e:	83 f8 02             	cmp    eax,0x2
  43a321:	75 1f                	jne    0x43a342
  43a323:	8d 45 fc             	lea    eax,[ebp-0x4]
  43a326:	50                   	push   eax
  43a327:	8d 45 f8             	lea    eax,[ebp-0x8]
  43a32a:	50                   	push   eax
  43a32b:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  43a32e:	e8 6c ae ff ff       	call   0x43519f
  43a333:	83 c4 0c             	add    esp,0xc
  43a336:	85 c0                	test   eax,eax
  43a338:	74 08                	je     0x43a342
  43a33a:	0f b6 00             	movzx  eax,BYTE PTR [eax]
  43a33d:	c1 e0 04             	shl    eax,0x4
  43a340:	eb 11                	jmp    0x43a353
  43a342:	ff 75 08             	push   DWORD PTR [ebp+0x8]
  43a345:	6a 00                	push   0x0
  43a347:	ff 35 40 da 51 00    	push   DWORD PTR ds:0x51da40
  43a34d:	ff 15 5c b1 43 00    	call   DWORD PTR ds:0x43b15c
  43a353:	5e                   	pop    esi
  43a354:	c9                   	leave
  43a355:	c3                   	ret
