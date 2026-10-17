default rel
global _start

section .bss
digit_buf: resb 2        ; one digit plus the newline

section .text
_start:
    mov r12, 1           ; callee-saved: write() will not touch it

.loop:
    cmp r12, 5
    jg .done

    mov rax, r12
    add al, '0'          ; 1 -> '1'
    mov [digit_buf], al
    mov byte [digit_buf+1], 10   ; '\n'

    mov rax, 1           ; write
    mov rdi, 1           ; stdout
    mov rsi, digit_buf
    mov rdx, 2
    syscall

    inc r12
    jmp .loop

.done:
    mov rdi, 0
    mov rax, 60          ; exit
    syscall
