default rel
global _start

section .text
_start:
    mov rax, 0xB6            ; the value, 1011 0110
    xor rcx, rcx             ; the count of 1 bits
.next:
    test rax, rax
    jz .done                 ; nothing left to count
    mov rdx, rax
    and rdx, 1               ; the lowest bit: 0 or 1
    add rcx, rdx
    shr rax, 1               ; move the next bit down
    jmp .next
.done:
    mov rdi, rcx             ; the exit status is the count
    mov rax, 60
    syscall
