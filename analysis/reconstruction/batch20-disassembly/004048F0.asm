
../btb-retail.exe:     file format pei-i386


Disassembly of section .text:

004048f0 <.text+0x38f0>:
  4048f0:	56                   	push   esi
  4048f1:	8b f1                	mov    esi,ecx
  4048f3:	8b 86 80 00 00 00    	mov    eax,DWORD PTR [esi+0x80]
  4048f9:	85 c0                	test   eax,eax
  4048fb:	74 10                	je     0x40490d
  4048fd:	8b 86 84 00 00 00    	mov    eax,DWORD PTR [esi+0x84]
  404903:	89 86 88 00 00 00    	mov    DWORD PTR [esi+0x88],eax
  404909:	33 c0                	xor    eax,eax
  40490b:	5e                   	pop    esi
  40490c:	c3                   	ret
  40490d:	8b 46 04             	mov    eax,DWORD PTR [esi+0x4]
  404910:	85 c0                	test   eax,eax
  404912:	75 07                	jne    0x40491b
  404914:	b8 f0 01 04 80       	mov    eax,0x800401f0
  404919:	5e                   	pop    esi
  40491a:	c3                   	ret
  40491b:	83 7e 7c 01          	cmp    DWORD PTR [esi+0x7c],0x1
  40491f:	75 41                	jne    0x404962
  404921:	8b 4e 28             	mov    ecx,DWORD PTR [esi+0x28]
  404924:	6a 00                	push   0x0
  404926:	83 c1 04             	add    ecx,0x4
  404929:	51                   	push   ecx
  40492a:	50                   	push   eax
  40492b:	ff 15 80 b2 43 00    	call   DWORD PTR ds:0x43b280
  404931:	83 f8 ff             	cmp    eax,0xffffffff
  404934:	75 07                	jne    0x40493d
  404936:	b8 05 40 00 80       	mov    eax,0x80004005
  40493b:	5e                   	pop    esi
  40493c:	c3                   	ret
  40493d:	8d 46 08             	lea    eax,[esi+0x8]
  404940:	8d 56 1c             	lea    edx,[esi+0x1c]
  404943:	6a 10                	push   0x10
  404945:	52                   	push   edx
  404946:	c7 00 64 61 74 61    	mov    DWORD PTR [eax],0x61746164
  40494c:	50                   	push   eax
  40494d:	8b 46 04             	mov    eax,DWORD PTR [esi+0x4]
  404950:	50                   	push   eax
  404951:	ff 15 74 b2 43 00    	call   DWORD PTR ds:0x43b274
  404957:	85 c0                	test   eax,eax
  404959:	74 ae                	je     0x404909
  40495b:	b8 05 40 00 80       	mov    eax,0x80004005
  404960:	5e                   	pop    esi
  404961:	c3                   	ret
  404962:	8d 4e 08             	lea    ecx,[esi+0x8]
  404965:	6a 00                	push   0x0
  404967:	51                   	push   ecx
  404968:	50                   	push   eax
  404969:	c7 01 64 61 74 61    	mov    DWORD PTR [ecx],0x61746164
  40496f:	c7 46 0c 00 00 00 00 	mov    DWORD PTR [esi+0xc],0x0
  404976:	ff 15 7c b2 43 00    	call   DWORD PTR ds:0x43b27c
  40497c:	85 c0                	test   eax,eax
  40497e:	74 07                	je     0x404987
  404980:	b8 05 40 00 80       	mov    eax,0x80004005
  404985:	5e                   	pop    esi
  404986:	c3                   	ret
  404987:	8b 56 04             	mov    edx,DWORD PTR [esi+0x4]
  40498a:	8d 4e 34             	lea    ecx,[esi+0x34]
  40498d:	6a 00                	push   0x0
  40498f:	51                   	push   ecx
  404990:	52                   	push   edx
  404991:	ff 15 78 b2 43 00    	call   DWORD PTR ds:0x43b278
  404997:	85 c0                	test   eax,eax
  404999:	0f 84 6a ff ff ff    	je     0x404909
  40499f:	b8 05 40 00 80       	mov    eax,0x80004005
  4049a4:	5e                   	pop    esi
  4049a5:	c3                   	ret
