; boot2.asm - talking to the hardware: the serial port, video memory, the keyboard.
;
;   nasm -f bin boot2.asm -o boot2.bin
;   qemu-system-x86_64 -drive format=raw,file=boot2.bin -serial stdio
;
; Everything the screen and serial port do here is port I/O and plain memory
; writes. Only the key read goes through the BIOS (int 0x16).

org 0x7C00
bits 16

COM1 equ 0x3F8                  ; first serial port: data at +0, line status at +5

start:
    cli
    xor ax, ax
    mov ds, ax
    mov ss, ax
    mov sp, 0x7C00
    sti
    cld                         ; string instructions go upward

    call serial_init
    mov si, ready
    call serial_print

    mov ax, 0xB800              ; video memory is the segment 0xB800
    mov es, ax                  ; (physical 0xB8000); stosw writes at es:di
    xor di, di
    mov ax, 0x0720              ; a space (0x20) in light grey on black (0x07)
    mov cx, 80 * 25             ; the whole text screen, one word per cell
    rep stosw

    mov si, banner              ; a string straight into video memory
    mov di, (1 * 80 + 2) * 2    ; row 1, column 2; two bytes per cell
    xor ah, ah                  ; (attribute 0: black on black)
.draw:
    lodsb                       ; al = next character
    test al, al
    jz .keys
    stosw                       ; store ax: al (character) first, then ah (attribute)
    jmp .draw

.keys:
    mov di, (3 * 80 + 2) * 2    ; typed keys go on row 3
.key:
    xor ah, ah
    int 0x16                    ; BIOS: wait for a key; al = ASCII, ah = scan code
    mov bx, ax                  ; keep both
    mov al, bl
    call serial_putc            ; the character to the serial port...
    mov al, ':'
    call serial_putc
    mov al, bh
    call serial_hex             ; ...then its scan code in hex
    mov al, 10
    call serial_putc
    mov al, bl                  ; and the character to the screen
    mov ah, 0x0A                ; light green on black
    stosw
    jmp .key

; ---- serial port ------------------------------------------------------

serial_init:
    mov dx, COM1 + 1
    xor al, al
    out dx, al                  ; no interrupts from the port
    mov dx, COM1 + 3
    mov al, 0x80
    out dx, al                  ; DLAB on: the first two registers become the divisor
    mov dx, COM1
    mov al, 1
    out dx, al                  ; divisor 1 = 115200 baud (low byte)
    inc dx
    xor al, al
    out dx, al                  ; (high byte)
    mov dx, COM1 + 3
    mov al, 0x03
    out dx, al                  ; DLAB off, 8 data bits, no parity, 1 stop bit
    ret

serial_putc:                    ; send the byte in al; keeps al
    push ax
    push dx
    mov ah, al
.wait:
    mov dx, COM1 + 5
    in al, dx                   ; line status
    test al, 0x20               ; bit 5: transmitter holding register empty
    jz .wait
    mov dx, COM1
    mov al, ah
    out dx, al
    pop dx
    pop ax
    ret

serial_print:                   ; send the zero-terminated string at si
    lodsb
    test al, al
    jz .done
    call serial_putc
    jmp serial_print
.done:
    ret

serial_hex:                     ; send al as two hex digits
    push ax
    shr al, 4
    call .digit
    pop ax
    and al, 0x0F
.digit:
    cmp al, 10
    jb .num
    add al, 'A' - 10 - '0'
.num:
    add al, '0'
    jmp serial_putc

ready:  db "boot2 ready", 13, 10, 0
banner: db "Type something:", 0

times 510 - ($ - $$) db 0
dw 0xAA55
