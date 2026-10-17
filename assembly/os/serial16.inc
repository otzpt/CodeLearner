; serial16.inc - COM1 output for 16-bit real-mode code.
;
; Included by the stages that report over the serial port. Needs COM1 set
; to 0x3F8, and uses ax, cx, dx, si and the stack. Everything here is
; module 11's recipe: wait for the transmitter, out dx, al.

COM1 equ 0x3F8

serial_init:
    mov dx, COM1 + 1
    xor al, al
    out dx, al
    mov dx, COM1 + 3
    mov al, 0x80
    out dx, al
    mov dx, COM1
    mov al, 1
    out dx, al
    inc dx
    xor al, al
    out dx, al
    mov dx, COM1 + 3
    mov al, 0x03
    out dx, al
    ret

serial_putc:                    ; send al
    push ax
    push dx
    mov ah, al
.wait:
    mov dx, COM1 + 5
    in al, dx
    test al, 0x20
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

serial_newline:
    mov al, 10
    jmp serial_putc

serial_yesno:                   ; ax = 1 prints "yes", 0 prints "no"
    mov si, no_text
    test ax, ax
    jz .print
    mov si, yes_text
.print:
    call serial_print
    jmp serial_newline

; serial_hex: print the low cx hex digits of eax (cx = 1 to 8), most
; significant first. For cx = 8 it prints the whole register.
serial_hex:
    push eax
    push ecx
    mov edx, ecx
    shl edx, 2                  ; bits to print: 4 per digit
    mov cl, 32
    sub cl, dl                  ; rotate so the first digit wanted is on top
    rol eax, cl
    pop ecx
.digit:
    rol eax, 4
    push eax
    and al, 0x0F
    cmp al, 10
    jb .number
    add al, 'A' - 10 - '0'
.number:
    add al, '0'
    call serial_putc
    pop eax
    dec cx
    jnz .digit
    pop eax
    ret
