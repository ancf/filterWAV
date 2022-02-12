.486
.mmx
.xmm

.model flat, stdcall
.code

filterASM proc original: DWORD, copy: DWORD, min: DWORD, max: DWORD
	push edx ;pointer to original array
	push eax ;pointer to copy array which we will modify
	push ecx ;max
	push edi ;min
;	push edi ;iter
	
	
;	push esp
	mov  edx, original
	mov eax, copy
	mov ecx, min
	sal ecx, 1
;	mov 
	mov edi, max
	sub edi, min
	sar edi, 3 ;8 values per iteration

	
	movdqu xmm0, [edx+ecx]
	movdqu xmm1, [edx+ecx+2]
	movdqu xmm2, [edx+ecx+4]
	movdqu xmm3, [edx+ecx+6]
	
	movdqu xmm4, [edx+ecx+8]
	movdqu xmm5, [edx+ecx+10]
	movdqu xmm6, [edx+ecx+12]
	movdqu xmm7, [edx+ecx+14]
	

	psraw xmm0, 3
	psraw xmm1, 3
	psraw xmm2, 3
	psraw xmm3, 3

	psraw xmm4, 3
	psraw xmm5, 3
	psraw xmm6, 3
	psraw xmm7, 3

	paddw xmm0, xmm1
	paddw xmm0, xmm2
	paddw xmm0, xmm3
	paddw xmm0, xmm4
	paddw xmm0, xmm5
	paddw xmm0, xmm6
	paddw xmm0, xmm7
	
	movdqu [eax+ecx+16], xmm0

	sub edi, 1
	add ecx, 16

tests:
	mov esi, 0
	cmp edi, 0
	jne loops
	je finish
nested:
	mov esp, esi
;	add esp, 1
	
	sal esp, 1
	add esp, ecx
	movdqu xmm1, [edx+esp]
	movdqu xmm2, [edx+esp-16]
	
	psraw xmm1, 3
	psraw xmm2, 3
	
	paddw xmm0, xmm1
	psubw xmm0, xmm2
	
	add esi, 1
	jmp test_nested
test_nested:
	
	cmp esi, 8
	jne nested
	jmp loopfin
loopfin:
	movdqu [eax+ecx+16], xmm0

	sub edi, 1
	add ecx, 16

	jmp tests
loops:	
	
	
	jmp test_nested

;	movdqu xmm1, [edx+ecx+2]
;	movdqu xmm2, [edx+ecx-14]
;	sub esi, 1
;	psraw xmm1, 3
;	psraw xmm2, 3
	
;	psubw xmm0, xmm2
;	paddw xmm0, xmm1
	
;	movdqu xmm1, [edx+ecx+4]
;	movdqu xmm2, [edx+ecx-12]
	
;	psraw xmm1, 3
;	psraw xmm2, 3
	
;	psubw xmm0, xmm2
;	paddw xmm0, xmm1
	
;	movdqu xmm1, [edx+ecx+6]
;	movdqu xmm2, [edx+ecx-10]
	
;	psraw xmm1, 3
;	psraw xmm2, 3
	
;	psubw xmm0, xmm2
;	paddw xmm0, xmm1
	
;	movdqu xmm1, [edx+ecx+8]
;	movdqu xmm2, [edx+ecx-8]
	
;	psraw xmm1, 3
;	psraw xmm2, 3
	
;	psubw xmm0, xmm2
;	paddw xmm0, xmm1
	
;	movdqu xmm1, [edx+ecx+10]
;	movdqu xmm2, [edx+ecx-6]
	
;	psraw xmm1, 3
;	psraw xmm2, 3
	
;	psubw xmm0, xmm2
;	paddw xmm0, xmm1
	
;	movdqu xmm1, [edx+ecx+12]
;	movdqu xmm2, [edx+ecx-4]
	
;	psraw xmm1, 3
;	psraw xmm2, 3
	
;	psubw xmm0, xmm2
;	paddw xmm0, xmm1
	
;	movdqu xmm1, [edx+ecx+14]
;	movdqu xmm2, [edx+ecx-2]
	
;	psraw xmm1, 3
;	psraw xmm2, 3
	
;	psubw xmm0, xmm2
;	paddw xmm0, xmm1
	
;	movdqu xmm1, [edx+ecx+16]
;	movdqu xmm2, [edx+ecx]
	
;	psraw xmm1, 3
;	psraw xmm2, 3
	
;	psubw xmm0, xmm2
;	paddw xmm0, xmm1

	
	

finish:

;	pop edi ;min
;	pop ecx ;max
;	pop eax ;pointer to copy array which we will modify
;	pop edx ;pointer to original array

	ret
filterASM endp

end 