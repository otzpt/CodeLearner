; hello.asm - an assembly program that calls C's printf. gcc supplies the C
; runtime, which calls main for us.
;   nasm -f elf64 hello.asm -o hello.o && gcc hello.o -o hello

default rel
global main
extern printf

section .rodata
fmt: db "the answer is %d", 10, 0

section .text
main:
    sub rsp, 8              ; on entry rsp is 8 off a multiple of 16: fix it
    lea rdi, [fmt]          ; printf(fmt, 42)
    mov esi, 42
    xor eax, eax            ; variadic call: al = number of vector registers used
    call printf wrt ..plt   ; through the PLT, so a PIE executable can link it
    add rsp, 8
    xor eax, eax            ; return 0
    ret

; Tells the linker this object does not need an executable stack.
section .note.GNU-stack noalloc noexec nowrite progbits
