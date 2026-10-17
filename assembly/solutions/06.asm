default rel
global _start

section .text
_start:
    mov rdi, 10
    call fib
    mov rdi, rax            ; the exit status is fib(10)
    mov rax, 60             ; exit
    syscall

; fib(rdi = n) -> rax. fib(0) = 0, fib(1) = 1.
fib:
    cmp rdi, 2
    jb .small
    push rbx                ; callee-saved: will hold fib(n-1)
    push rdi                ; n, needed again after the first call
    dec rdi
    call fib                ; fib(n-1)
    mov rbx, rax
    pop rdi
    sub rdi, 2
    call fib                ; fib(n-2)
    add rax, rbx
    pop rbx
    ret
.small:
    mov rax, rdi
    ret
