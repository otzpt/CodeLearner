default rel
global _start

section .rodata
big_msg:    db "big", 10
big_len     equ $ - big_msg
small_msg:  db "small", 10
small_len   equ $ - small_msg

section .text
_start:
    mov rax, 25

    cmp rax, 10
    jl .small            ; inverted: jump away when the if is false

    mov rsi, big_msg
    mov rdx, big_len
    jmp .print

.small:
    mov rsi, small_msg
    mov rdx, small_len

.print:
    mov rax, 1           ; write
    mov rdi, 1           ; stdout
    syscall

    mov rdi, 0
    mov rax, 60          ; exit
    syscall
