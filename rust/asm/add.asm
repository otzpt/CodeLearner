; add.asm - two functions written in NASM, called from Rust.
;
;   nasm -f elf64 add.asm -o add.o
;
; NASM's syntax is Intel's: destination first. The calling convention is the
; System V AMD64 ABI that Linux uses: integer arguments arrive in rdi, rsi,
; rdx, rcx, r8, r9 in that order, and the result goes back in rax.

global add_numbers
global sum_bytes

section .text

add_numbers:                ; u64 add_numbers(u64 a, u64 b)
    mov rax, rdi            ; the first argument arrives in rdi
    add rax, rsi            ; the second in rsi
    ret                     ; the result is whatever is in rax

sum_bytes:                  ; u64 sum_bytes(const u8 *data, u64 count)
    xor eax, eax            ; total = 0
    test rsi, rsi           ; is count zero?
    jz .done
.next:
    movzx edx, byte [rdi]   ; edx = the byte rdi points at
    add rax, rdx            ; total += byte
    inc rdi                 ; point at the next byte
    dec rsi                 ; one fewer left
    jnz .next
.done:
    ret

; Tells the linker this object does not need an executable stack.
section .note.GNU-stack noalloc noexec nowrite progbits
