; A boot sector that prints OK and halts.
org 0x7C00                  ; the BIOS loads us here
bits 16

start:
    cli
    xor ax, ax
    mov ds, ax              ; the BIOS leaves these undefined
    mov ss, ax
    mov sp, 0x7C00          ; stack just below the code
    sti

    mov ah, 0x0E            ; teletype output
    mov bh, 0               ; page 0
    mov al, 'O'
    int 0x10
    mov al, 'K'
    int 0x10

.halt:
    hlt
    jmp .halt

times 510 - ($ - $$) db 0
dw 0xAA55
