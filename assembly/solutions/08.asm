default rel
global _start

section .rodata
msg: db "operating system", 0

section .text
_start:
    mov rdi, msg
    xor al, al               ; the byte to look for: the 0 that ends the string
    mov rcx, -1              ; effectively no limit
    cld                      ; scan upward
    repne scasb              ; rdi runs on until it has passed the 0
    not rcx                  ; rcx counted down from -1: this is bytes scanned
    dec rcx                  ; minus the 0 itself
    mov rdi, rcx             ; the exit status is the length
    mov rax, 60
    syscall
