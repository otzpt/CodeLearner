; A boot sector that clears the screen and puts a red X at row 5, column 10.
org 0x7C00
bits 16

start:
    cli
    xor ax, ax
    mov ds, ax
    mov ss, ax
    mov sp, 0x7C00
    sti
    cld                         ; stosw goes upward

    mov ax, 0xB800              ; the segment of video memory (physical 0xB8000)
    mov es, ax
    xor di, di
    mov ax, 0x0720              ; a space, light grey on black
    mov cx, 80 * 25             ; one word per cell, the whole screen
    rep stosw

    mov di, (5 * 80 + 10) * 2   ; two bytes a cell: row 5, column 10
    mov ax, 0x0458              ; attribute 0x04 (red on black), character 'X' (0x58)
    stosw

.halt:
    hlt
    jmp .halt

times 510 - ($ - $$) db 0
dw 0xAA55
