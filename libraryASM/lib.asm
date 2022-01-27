

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
	sal ecx, 2
;	mov 
	mov edi, max
	sub edi, min
	sar edi, 3

	

tests:
	cmp edi, 0
	jne loops
	je finish
loops:
	
	movdqu xmm0, [edx+ecx]
	movdqu xmm1, [edx+ecx+4]
	movdqu xmm2, [edx+ecx+8]
	movdqu xmm3, [edx+ecx+12]
	
	movdqu xmm4, [edx+ecx+16]
	movdqu xmm5, [edx+ecx+20]
	movdqu xmm6, [edx+ecx+24]
	movdqu xmm7, [edx+ecx+28]
	

	paddd xmm0, xmm1
	paddd xmm0, xmm2
	paddd xmm0, xmm3
	
	paddd xmm4, xmm5
	paddd xmm4, xmm6
	paddd xmm4, xmm7

	psrad xmm0, 2
	psrad xmm4, 2

	movdqu [eax+ecx+16], xmm0
	movdqu [eax+ecx+32], xmm4


;	movdqu xmm5, [eax+ecx+16]

	sub edi, 1

	add ecx, 32

	jmp tests
finish:

;	pop edi ;min
;	pop ecx ;max
;	pop eax ;pointer to copy array which we will modify
;	pop edx ;pointer to original array
	
	
	
	ret
filterASM endp

end