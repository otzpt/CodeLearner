; start.asm - the entry point of a program with no C runtime.
;
;   nasm -f elf64 start.asm -o start.o
;   gcc -ffreestanding -nostdlib -static -fno-pie -no-pie -O2 -c free.c -o free.o
;   ld start.o free.o -o free
;
; The kernel starts the process at _start. There is no libc to set anything
; up, so the first job is to hand control to C and, when it returns, exit.

default rel
global _start
extern c_main

section .text
_start:
    and rsp, -16            ; the ABI wants a multiple of 16 at a call
    call c_main             ; the C function's return value arrives in eax
    mov edi, eax            ; exit status = what c_main returned
    mov eax, 60             ; exit
    syscall

; Tells the linker this object does not need an executable stack.
section .note.GNU-stack noalloc noexec nowrite progbits
