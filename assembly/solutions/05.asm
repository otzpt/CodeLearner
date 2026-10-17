default rel
global _start

section .data
source:  db "stressed"
src_len  equ $ - source

section .bss
reversed: resb src_len + 1

section .text
_start:
    xor rcx, rcx                  ; i = 0
.next:
    cmp rcx, src_len
    jge .print
    mov rax, src_len - 1
    sub rax, rcx                  ; the index counted from the end
    mov dl, [source + rax]        ; one load...
    mov [reversed + rcx], dl      ; ...one store
    inc rcx
    jmp .next

.print:
    mov byte [reversed + src_len], 10
    mov rax, 1                    ; write
    mov rdi, 1                    ; stdout
    mov rsi, reversed
    mov rdx, src_len + 1
    syscall

    mov rdi, 0
    mov rax, 60                   ; exit
    syscall
