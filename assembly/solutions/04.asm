default rel
global _start

section .rodata
ask_msg:   db "Guess (1-100): "
ask_len    equ $ - ask_msg
low_msg:   db "Too low.", 10
low_len    equ $ - low_msg
high_msg:  db "Too high.", 10
high_len   equ $ - high_msg
win_msg:   db "Got it.", 10
win_len    equ $ - win_msg

section .bss
buf:       resb 32

section .text
_start:
    mov r12, 37              ; the secret; rdtsc here for a varying one

.again:
    mov rax, 1               ; write the prompt
    mov rdi, 1
    mov rsi, ask_msg
    mov rdx, ask_len
    syscall

    xor rax, rax             ; read a line
    xor rdi, rdi
    mov rsi, buf
    mov rdx, 32
    syscall
    cmp rax, 0
    jle .done                ; 0 = EOF, negative = error: stop either way

    xor r13, r13             ; bytes -> number
    mov rsi, buf
.digit:
    movzx rax, byte [rsi]
    cmp al, '0'
    jl .parsed
    cmp al, '9'
    jg .parsed
    sub al, '0'
    imul r13, 10
    add r13, rax
    inc rsi
    jmp .digit

.parsed:
    cmp r13, r12             ; guess against secret
    je .correct
    jl .too_low

    mov rsi, high_msg
    mov rdx, high_len
    jmp .say

.too_low:
    mov rsi, low_msg
    mov rdx, low_len

.say:
    mov rax, 1
    mov rdi, 1
    syscall
    jmp .again

.correct:
    mov rax, 1
    mov rdi, 1
    mov rsi, win_msg
    mov rdx, win_len
    syscall

.done:
    mov rdi, 0
    mov rax, 60
    syscall
