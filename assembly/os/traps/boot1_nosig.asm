; boot1.asm - the smallest useful boot sector: print a line and stop.
;
;   nasm -f bin boot1.asm -o boot1.bin
;   qemu-system-x86_64 -drive format=raw,file=boot1.bin
;
; The BIOS reads the first 512 bytes of the disk to 0x7C00, checks that the
; last two are 0x55 0xAA, and jumps to 0x7C00 in 16-bit real mode.

org 0x7C00                  ; we are loaded here, and labels must know it
bits 16                     ; the CPU starts in 16-bit real mode

start:
    cli                     ; no interrupts while the stack is half set up
    xor ax, ax
    mov ds, ax              ; the BIOS leaves these undefined
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00          ; the stack grows down from just below us
    sti

    mov si, message
.next:
    lodsb                   ; al = [ds:si], then si = si + 1
    test al, al
    jz .halt
    mov ah, 0x0E            ; BIOS teletype output: print the character in al
    mov bh, 0               ; video page 0
    int 0x10
    jmp .next

.halt:
    hlt                     ; sleep until an interrupt...
    jmp .halt               ; ...and again

message: db "Hello from a boot sector", 13, 10, 0

times 510 - ($ - $$) db 0   ; pad the sector to 510 bytes
dw 0                      ; (zeros, not the signature)
