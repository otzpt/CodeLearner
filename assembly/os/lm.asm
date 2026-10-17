; lm.asm - from real mode, through protected mode, to 64-bit long mode.
;
;   nasm -f bin lm.asm -o lm.bin
;   qemu-system-x86_64 -drive format=raw,file=lm.bin
;
; Long mode needs paging, so besides the GDT this builds three page tables
; that map the first 2 MiB onto itself ("identity mapping"): the code that
; turns paging on keeps running at the address it was running at.

org 0x7C00
bits 16

CODE32 equ 0x08                 ; GDT selectors: 32-bit code, data, 64-bit code
DATA   equ 0x10
CODE64 equ 0x18

PML4 equ 0x1000                 ; the three tables, each 4096 bytes, 4 KiB aligned
PDPT equ 0x2000
PD   equ 0x3000

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    in al, 0x92                 ; A20 on (module 12)
    or al, 2
    and al, 0xFE
    out 0x92, al
    lgdt [gdt_pointer]
    mov eax, cr0
    or eax, 1                   ; PE: protected mode, as in module 13
    mov cr0, eax
    jmp CODE32:protected

bits 32
protected:
    mov ax, DATA
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, 0x90000

    ; Does this CPU have long mode? CPUID leaf 0x80000001, edx bit 29.
    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    jb .no_long_mode            ; the CPU has no extended leaves at all
    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29
    jz .no_long_mode

    ; The page tables: zero all three pages, then fill in one entry each.
    mov edi, PML4
    xor eax, eax
    mov ecx, 3 * 4096 / 4
    rep stosd
    mov dword [PML4], PDPT | 0x03      ; present + writable: points at the PDPT
    mov dword [PDPT], PD | 0x03        ; ...which points at the PD
    mov dword [PD], 0x00 | 0x83        ; ...whose first entry is a 2 MiB page at 0:
                                       ; present, writable, and bit 7: "big page"

    mov eax, PML4
    mov cr3, eax                ; where the top-level table is

    mov eax, cr4
    or eax, 1 << 5              ; PAE: the page-table format long mode uses
    mov cr4, eax

    mov ecx, 0xC0000080         ; EFER, a model-specific register
    rdmsr
    or eax, 1 << 8              ; LME: long mode enable
    wrmsr

    mov eax, cr0
    or eax, 1 << 31             ; PG: paging on. With LME set, this activates long mode
    mov cr0, eax

    jmp CODE64:long_entry            ; a far jump into the 64-bit code segment

.no_long_mode:
    mov dword [0xB8000], 0x4F4F4F4E     ; "N", "O" in white on red
    mov dword [0xB8004], 0x4F344F36     ; "6", "4"
.stop:
    hlt
    jmp .stop

bits 64
long_entry:
    mov ax, DATA                ; most segment registers are ignored now, but
    mov ds, ax                  ; they still have to be valid
    mov es, ax
    mov ss, ax
    mov rsp, 0x90000

    mov rsi, message
    mov rdi, 0xB8000
    mov ah, 0x1F                ; white on blue
.draw:
    lodsb
    test al, al
    jz .halt
    stosw
    jmp .draw
.halt:
    hlt
    jmp .halt

align 8
gdt_start:
    dq 0
    dq 0x00CF9A000000FFFF       ; 0x08: 32-bit code
    dq 0x00CF92000000FFFF       ; 0x10: data
    dq 0x00AF9A000000FFFF       ; 0x18: 64-bit code (flag nibble 0xA: L set, D clear)
gdt_end:

gdt_pointer:
    dw gdt_end - gdt_start - 1
    dd gdt_start

message: db "Now in 64-bit long mode", 0

times 510 - ($ - $$) db 0
dw 0xAA55
