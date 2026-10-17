; entry.asm - the first instruction of the kernel, and the only assembly in it.
;
; stage2.asm jumps here in 64-bit mode with the page tables of module 14 and
; nothing else set up. A C function needs three things its caller always
; provided: a stack, zeroed static data, and a return address worth having.

default rel
bits 64

extern kmain
extern __bss_start
extern __bss_end

section .text.entry             ; the linker script puts this section first
global _start
_start:
    mov rsp, 0x90000            ; a stack: any free memory below the kernel's roof

    mov rdi, __bss_start        ; zero .bss. The file does not contain it, so
    mov rcx, __bss_end          ; nothing has written zeros there: rep stosb does
    sub rcx, rdi
    xor eax, eax
    cld
    rep stosb

    call kmain                  ; rsp is a multiple of 16 here: the ABI wants that
.halt:                          ; kmain should not return; if it does, stop
    cli
    hlt
    jmp .halt
