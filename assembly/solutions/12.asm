; A boot sector that loads the sector after it, and jumps to it.
; The file is two sectors long: the second one prints "two" on COM1.
org 0x7C00
bits 16

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    ; dl still holds the boot drive the BIOS passed in: nothing has touched
    ; it yet. A real stage 1 saves it first.
    mov ah, 0x42                ; extended read
    mov si, packet
    int 0x13
    jc .fail
    jmp 0x0000:second           ; second is at 0x7E00, the next sector

.fail:
    hlt
    jmp .fail

packet:
    db 0x10, 0                  ; packet size, reserved
    dw 1                        ; one sector
    dw 0x7E00, 0x0000           ; to 0x0000:0x7E00
    dq 1                        ; starting at sector 1

times 510 - ($ - $$) db 0
dw 0xAA55

; ---- sector 1: in the file right after sector 0, so at 0x7E00 ----------
second:
    mov si, text
.next:
    lodsb
    test al, al
    jz .done
    mov ah, al
.wait:
    mov dx, 0x3FD               ; line status
    in al, dx
    test al, 0x20               ; transmitter empty?
    jz .wait
    mov dx, 0x3F8               ; data
    mov al, ah
    out dx, al
    jmp .next
.done:
    hlt
    jmp .done

text: db "two", 10, 0

times 1024 - ($ - $$) db 0
