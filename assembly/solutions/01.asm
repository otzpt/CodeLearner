default rel
global _start

section .rodata
msg:     db "done", 10
msg_len  equ $ - msg

section .text
_start:
    mov rax, 1          ; write
    mov rdi, 1          ; stdout
    mov rsi, msg
    mov rdx, msg_len
    syscall

    mov rdi, 0          ; exit code
    mov rax, 60         ; exit
    syscall
