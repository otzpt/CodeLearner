; pm.asm - from real mode to 32-bit protected mode, and a line of text without the BIOS.
;
;   nasm -f bin pm.asm -o pm.bin
;   qemu-system-x86_64 -drive format=raw,file=pm.bin
;
; Protected mode replaces "segment * 16 + offset" with a table: each segment
; register holds a SELECTOR that picks an entry (a descriptor) in the Global
; Descriptor Table. The CPU will not run in protected mode without one.

org 0x7C00
bits 16

CODE_SEL equ gdt_code - gdt_start       ; 0x08: the second descriptor
DATA_SEL equ gdt_data - gdt_start       ; 0x10: the third

start:
    cli                         ; no interrupts: there is no table for them yet
    xor ax, ax
    mov ds, ax                  ; lgdt reads its operand through ds
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    in al, 0x92                 ; the A20 gate (module 12)
    or al, 2
    and al, 0xFE
    out 0x92, al

    lgdt [gdt_pointer]          ; tell the CPU where the table is

    mov eax, cr0
    or eax, 1                   ; PE, protection enable: bit 0 of cr0
    mov cr0, eax
    jmp protected               ; (trap variant: a near jump; cs is not reloaded)

bits 32                         ; from here on the assembler emits 32-bit code
protected:
    mov ax, DATA_SEL            ; every other segment register needs a selector too
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000            ; any free memory will do for the stack

    mov esi, message            ; no BIOS here: write the text into video memory
    mov edi, 0xB8000            ; physical address, now that we have 32 bits
    mov ah, 0x2F                ; white on green
.draw:
    lodsb
    test al, al
    jz .halt
    stosw
    jmp .draw

.halt:
    hlt
    jmp .halt

; The table. Each descriptor is 8 bytes; module 7 built these same values.
align 8
gdt_start:
    dq 0                        ; descriptor 0 must be all zeros
gdt_code:
    dq 0x00CF9A000000FFFF       ; base 0, limit 4 GiB, code, ring 0, 32-bit
gdt_data:
    dq 0x00CF92000000FFFF       ; base 0, limit 4 GiB, data, ring 0, writable
gdt_end:

gdt_pointer:                    ; what lgdt reads: 2 bytes limit, 4 bytes base
    dw gdt_end - gdt_start - 1  ; the limit is the LAST valid byte, so size - 1
    dd gdt_start

message: db "Now in 32-bit protected mode", 0

times 510 - ($ - $$) db 0
dw 0xAA55
