; A boot sector that enters 32-bit protected mode and writes a white-on-red P
; into the top-left screen cell, directly, with no BIOS.
org 0x7C00
bits 16

start:
    cli
    xor ax, ax
    mov ds, ax                  ; lgdt reads its operand through ds
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    lgdt [gdt_pointer]
    mov eax, cr0
    or eax, 1                   ; PE
    mov cr0, eax
    jmp 0x08:protected          ; far jump: cs = descriptor 1

bits 32
protected:
    mov ax, 0x10                ; descriptor 2: data
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, 0x90000
    mov word [0xB8000], 0x4F50  ; 'P' = 0x50, attribute 0x4F: white on red

.halt:
    hlt
    jmp .halt

align 8
gdt:
    dq 0
    dq 0x00CF9A000000FFFF       ; code
    dq 0x00CF92000000FFFF       ; data
gdt_end:

gdt_pointer:
    dw gdt_end - gdt - 1
    dd gdt

times 510 - ($ - $$) db 0
dw 0xAA55
