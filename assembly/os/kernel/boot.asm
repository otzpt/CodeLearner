; boot.asm - stage 1 of the C kernel's loader: read stage 2 and the kernel.
;
; Disk layout:   sector 0           this sector
;                sectors 1-2        stage2.asm (two sectors)
;                sectors 3-18       the kernel, padded to KERNEL_SECTORS sectors
;
; Real mode can only reach the first megabyte, and only BIOS calls can read the
; disk, so everything has to be read NOW, before stage 2 leaves real mode.

org 0x7C00
bits 16

STAGE2_SECTORS equ 2
KERNEL_SECTORS equ 16           ; 8 KiB; the Makefile pads the kernel to this

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti
    cld
    mov [boot_drive], dl

    mov si, stage2_packet       ; stage 2 to 0x0000:0x7E00
    call read
    mov si, kernel_packet       ; the kernel to 0x1000:0x0000, which is 0x10000
    call read

    mov dl, [boot_drive]
    jmp 0x0000:0x7E00

read:                           ; read the packet at si, or stop with a message
    mov ah, 0x42
    mov dl, [boot_drive]
    int 0x13
    jnc .ok
    mov si, error_text
.print:
    lodsb
    test al, al
    jz .halt
    mov ah, 0x0E
    xor bx, bx
    int 0x10
    jmp .print
.halt:
    hlt
    jmp .halt
.ok:
    ret

boot_drive: db 0
error_text: db "Disk read error", 13, 10, 0

stage2_packet:
    db 0x10, 0
    dw STAGE2_SECTORS
    dw 0x7E00, 0x0000           ; offset, segment
    dq 1                        ; from sector 1

kernel_packet:
    db 0x10, 0
    dw KERNEL_SECTORS
    dw 0x0000, 0x1000           ; offset 0 in segment 0x1000: physical 0x10000
    dq 1 + STAGE2_SECTORS       ; from sector 3

times 510 - ($ - $$) db 0
dw 0xAA55
