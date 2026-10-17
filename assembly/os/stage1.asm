; stage1.asm - a boot sector whose only job is to load more code.
;
;   nasm -f bin stage1.asm -o stage1.bin
;   nasm -f bin stage2.asm -o stage2.bin
;   cat stage1.bin stage2.bin > os12.img
;   qemu-system-x86_64 -drive format=raw,file=os12.img -serial stdio
;
; 512 bytes will not hold an operating system. This sector reads the next
; STAGE2_SECTORS sectors of the disk to 0x7E00, right after itself, and
; jumps there.

org 0x7C00
bits 16

STAGE2_SECTORS equ 2            ; stage2.asm pads itself to this many sectors
STAGE2_ADDRESS equ 0x7E00

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti
    cld

    mov [boot_drive], dl        ; the BIOS passes the boot drive in dl. Keep it:
                                ; dl is about to be overwritten by int 0x13 calls.

    mov ah, 0x42                ; int 0x13 function 0x42: extended read (LBA)
    mov dl, [boot_drive]
    mov si, disk_packet         ; ds:si -> the packet describing the read
    int 0x13
    jc disk_error               ; carry set: it failed, and ah says why

    mov dl, [boot_drive]        ; hand the drive on to the next stage in dl
    jmp 0x0000:STAGE2_ADDRESS   ; a far jump: sets cs, and ip, to stage 2

disk_error:
    mov si, error_text
.next:
    lodsb
    test al, al
    jz .halt
    mov ah, 0x0E
    xor bx, bx
    int 0x10
    jmp .next
.halt:
    hlt
    jmp .halt

boot_drive: db 0
error_text: db "Disk read error", 13, 10, 0

; The disk address packet int 0x13 function 0x42 reads its orders from.
disk_packet:
    db 0x10                     ; size of this packet
    db 0                        ; reserved
    dw STAGE2_SECTORS           ; how many sectors to read
    dw STAGE2_ADDRESS           ; where to put them: offset...
    dw 0x0000                   ; ...and segment
    dq 1                        ; the first sector, counted from 0: sector 1,
                                ; because sector 0 is this one

times 510 - ($ - $$) db 0
dw 0xAA55
