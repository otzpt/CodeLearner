; max.asm - a function written in NASM, called from C.
;
;   nasm -f elf64 max.asm -o max.o
;
; int asm_max(int a, int b): the System V AMD64 ABI puts a in edi and b in
; esi, and the result goes back in eax.

default rel
global asm_max

section .text
asm_max:
    mov eax, edi            ; assume a is the larger
    cmp edi, esi
    jge .done               ; signed compare: a >= b, keep it
    mov eax, esi            ; otherwise b
.done:
    ret

; Tells the linker this object does not need an executable stack.
section .note.GNU-stack noalloc noexec nowrite progbits
