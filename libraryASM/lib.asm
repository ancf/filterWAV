;	Temat projektu: Filtr uœredniaj¹cy WAV (œrednia krocz¹ca)
;	Semestr: 5
;   Rok akademicki: 2021/2022
;	Opis algorytmu: 
;		Prosta œrednia ruchoma - œrednia arytmetyczna z ostatnich n pomiarów 
;	Autor: Krystian Stebel



.486
.mmx
.xmm

.model flat, stdcall
.code


;	Parametry wejœciowe:
;		- wskaŸniki na tablice z próbkami - oryginaln¹ (tylko do odczytu) i kopiê (do zapisywania wyników)
;		- indeksy pocz¹tkowy i koñcowy przedzia³u do przetworzenia
;	W wyniku wykonania procedury ulegaj¹ zmianie rejestry xmm0 - xmm7

filterASM proc original: DWORD, copy: DWORD, min: DWORD, max: DWORD
	;save values to stack before overwriting them with:
	push eax ;current pointer
	push ebx ;number of iterations
	push ecx ;iterations in nested loop
	push edx ;tmp for nested loop
	
	;current pointer is pointer to original table + 2 * lower bound
	mov eax, min 
	sal eax, 1 ;2 * lower bound as each value in array is 2 bytes long
	add eax, original

	;number of iterations is (max-min) / 2^3 as each iteration will process and write 8 values
	mov ebx, max
	sub ebx, min
	sar ebx, 3 

	;move values of indexes n, n+1, ... n+7 to registers
	movdqu xmm0, [eax]
	movdqu xmm1, [eax+2]
	movdqu xmm2, [eax+4]
	movdqu xmm3, [eax+6]	
	movdqu xmm4, [eax+8]
	movdqu xmm5, [eax+10]
	movdqu xmm6, [eax+12]
	movdqu xmm7, [eax+14]
	
	;divide each value in each register by 2^3 (parallel arithmetic shift to the right)
	psraw xmm0, 3
	psraw xmm1, 3
	psraw xmm2, 3
	psraw xmm3, 3
	psraw xmm4, 3
	psraw xmm5, 3
	psraw xmm6, 3
	psraw xmm7, 3

	;add corresponding values from xmm0-xmm7 to each other, store in xmm0
	paddw xmm0, xmm1
	paddw xmm0, xmm2
	paddw xmm0, xmm3
	paddw xmm0, xmm4
	paddw xmm0, xmm5
	paddw xmm0, xmm6
	paddw xmm0, xmm7
	
	;replace pointer to nth index of original with pointer to nth index of copy, write xmm0 values to indexes starting with n+8
	push eax
	sub eax, original
	add eax, copy
	movdqu [eax+16], xmm0
	pop eax 
	
	; decrement iterator, move to first unprocessed value
	sub ebx, 1
	add eax, 16

tests:
	mov ecx, 0 ;reset nested loop iterator
	cmp ebx, 0 ;jump to finish if main iterator is 0, otherwise repeat main loop
	jne loops
	je finish
nested:
	;load iterator and multiply by number of bytes per value
	mov edx, ecx 
	sal edx, 1 
	;add pointer to current index (n) to modified iterator (i) so it points to original[n+i]
	add edx, eax
	;load n+i and n+i-8
	movdqu xmm1, [edx]
	movdqu xmm2, [edx-16]
	
	;parallel arithemic shift to divide by 8 again
	psraw xmm1, 3
	psraw xmm2, 3
	
	;add n+i and substract n+i-8 from current average
	paddw xmm0, xmm1
	psubw xmm0, xmm2
	
	
	add ecx, 1 ;increment iterator
	jmp test_nested ;check if loop ended
test_nested:
	
	cmp ecx, 8 ;if loop repreated 8 times then none of the values in xmm0 were written already and we can write them in loopfin 
	jne nested
	jmp loopfin
loopfin:
	;once again swap original[n] for copy[n] and write 8 values, then restore original[n] as eax
	push eax
	sub eax, original
	add eax, copy
	movdqu [eax+16], xmm0
	pop eax

	
	sub ebx, 1 ;decrement iterator
	add eax, 16 ;move pointer to the start of next chunk of 8 unprocessed values

	jmp tests
loops:	
	jmp test_nested	

finish:
	;restore original values from stack
	pop edx 
	pop ecx 
	pop ebx 
	pop eax

	ret
filterASM endp

;	Parametry wejœciowe:
;		- wskaŸniki na tablice z próbkami - oryginaln¹ (tylko do odczytu) i kopiê (do zapisywania wyników)
;		- indeksy pocz¹tkowy i koñcowy przedzia³u do przetworzenia
;	W wyniku wykonania procedury ulegaj¹ zmianie rejestry xmm0 - xmm7

altFilterASM proc original: DWORD, copy: DWORD, min: DWORD, max: DWORD
;save values to stack before overwriting them with:
	push eax ;pointer to original array
	push ebx ;pointer to copy array which we will modify
	push ecx ;min
	push edx ;max
	
	mov eax, original
	mov ebx, copy

	mov ecx, min
	sal ecx, 1  ;2 bytes per value - Nth index is at pointer + 2*n

	;number of iterations is (max-min) / 2^3 as each iteration will process and write 8 values
	mov edx, max
	sub edx, min
	sar edx, 3  


tests:
	cmp edx, 0 ;jump to finish if iterator is 0, otherwise repeat loop
	jne loops
	je finish
loops:
	
	;move values of indexes n, n+1, ... n+7 to registers
	movdqu xmm0, [eax+ecx]
	movdqu xmm1, [eax+ecx+2]
	movdqu xmm2, [eax+ecx+4]
	movdqu xmm3, [eax+ecx+6]	
	movdqu xmm4, [eax+ecx+8]
	movdqu xmm5, [eax+ecx+10]
	movdqu xmm6, [eax+ecx+12]
	movdqu xmm7, [eax+ecx+14]
	
	;divide each value in each register by 2^3 (parallel arithmetic shift to the right)
	psraw xmm0, 3
	psraw xmm1, 3
	psraw xmm2, 3
	psraw xmm3, 3
	psraw xmm4, 3
	psraw xmm5, 3
	psraw xmm6, 3
	psraw xmm7, 3

	;add corresponding values from xmm0-xmm7 to each other, store in xmm0
	paddw xmm0, xmm1
	paddw xmm0, xmm2
	paddw xmm0, xmm3
	paddw xmm0, xmm4
	paddw xmm0, xmm5
	paddw xmm0, xmm6
	paddw xmm0, xmm7
	
	;save xmm0 values to copy array
	movdqu [eax+ecx+16], xmm0


	sub edx, 1 ;decrement iterator
	add ecx, 16 ;increase index to start of next 8-value chunk

	jmp tests
finish:
	;restore original values
	pop edx ;max
	pop ecx ;min
	pop ebx ;pointer to copy array which we will modify
	pop eax ;pointer to original array

	ret
altFilterASM endp


end 