.486

.model flat, stdcall

.data
.code

filterASM proc x: qword, y: qword
	finit
	fld x
	fld y
	fadd
	ret
filterASM endp

end