; stage2.asm - the second stage: it runs from 0x7E00, with room to grow.
;
; Loaded by stage1.asm, which leaves the boot drive number in dl. This stage
; reports over the serial port what it found out about the machine: the boot
; drive, the A20 line, and the BIOS's memory map.

org 0x7E00
bits 16

start:
    mov [boot_drive], dl        ; keep the value stage 1 handed over
    call serial_init

    mov si, hello
    call serial_print
    mov si, drive_label
    call serial_print
    movzx eax, byte [boot_drive]
    mov cx, 2
    call serial_hex             ; "0x" has already been printed; two digits
    call serial_newline

    ; --- A20: is address bit 20 usable? --------------------------------
    in al, 0x92                 ; (trap variant: switch A20 OFF first)
    and al, 0xFC
    out 0x92, al
    mov si, a20_before
    call serial_print
    call a20_test
    call serial_yesno

    in al, 0x92                 ; the "fast A20" gate on the system control port
    or al, 2                    ; bit 1: enable A20
    and al, 0xFE                ; bit 0 resets the CPU: keep it clear
    out 0x92, al

    mov si, a20_after
    call serial_print
    call a20_test
    call serial_yesno

    ; --- the memory map: int 0x15, eax = 0xE820 ------------------------
    mov si, map_head
    call serial_print
    xor ebx, ebx                ; 0 asks for the first entry, then the BIOS's cookie
    xor ebp, ebp                ; running total of the usable bytes (type 1)
.entry:
    mov di, entry_buffer
    mov eax, 0xE820
    mov edx, 0x534D4150         ; 'SMAP'
    mov ecx, 24                 ; room for 24 bytes
    int 0x15
    jc .done                    ; carry: the list ended (or the call is unsupported)
    cmp eax, 0x534D4150
    jne .done                   ; the BIOS echoes 'SMAP' if it understood
    mov si, base_label
    call serial_print
    mov eax, [entry_buffer + 4]
    mov cx, 8
    call serial_hex
    mov eax, [entry_buffer]
    mov cx, 8
    call serial_hex
    mov si, length_label
    call serial_print
    mov eax, [entry_buffer + 12]
    mov cx, 8
    call serial_hex
    mov eax, [entry_buffer + 8]
    mov cx, 8
    call serial_hex
    mov si, type_label
    call serial_print
    mov eax, [entry_buffer + 16]
    mov cx, 1
    call serial_hex
    call serial_newline
    cmp dword [entry_buffer + 16], 1
    jne .next
    add ebp, [entry_buffer + 8] ; the low 32 bits are enough for a small machine
.next:
    test ebx, ebx
    jnz .entry                  ; ebx = 0 after the last entry
.done:
    mov si, usable_label
    call serial_print
    mov eax, ebp
    mov cx, 8
    call serial_hex
    call serial_newline

.halt:
    hlt
    jmp .halt

; a20_test: ax = 1 if address 0x100500 is distinct from 0x000500, else 0.
; With A20 off, 0xFFFF:0x0510 wraps to physical 0x000500.
a20_test:
    push ds
    push es
    xor ax, ax
    mov ds, ax                  ; ds:0x500 = physical 0x000500
    mov ax, 0xFFFF
    mov es, ax                  ; es:0x510 = physical 0x100500
    mov word [ds:0x500], 0x1111
    mov word [es:0x510], 0x2222
    xor ax, ax
    cmp word [ds:0x500], 0x2222 ; did the second write change the first?
    setne al                    ; ax = 1: they are different bytes, A20 is on
    pop es
    pop ds
    ret

%include "serial16.inc"

boot_drive:    db 0
hello:         db "stage 2 loaded", 10, 0
drive_label:   db "boot drive: 0x", 0
a20_before:    db "A20 usable before enabling: ", 0
a20_after:     db "A20 usable after enabling:  ", 0
yes_text:      db "yes", 0
no_text:       db "no", 0
map_head:      db "memory map (E820):", 10, 0
base_label:    db "  base 0x", 0
length_label:  db "  length 0x", 0
type_label:    db "  type ", 0
usable_label:  db "usable bytes (low 32 bits): 0x", 0
entry_buffer:  times 24 db 0

times 1024 - ($ - $$) db 0      ; two sectors, which is what stage 1 reads
