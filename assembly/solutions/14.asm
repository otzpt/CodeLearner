; Long mode with the first 4 MiB identity-mapped (two 2 MiB pages). The 64-bit
; code stores a byte at 3 MiB, reads it back, and shows it at the top left.
org 0x7C00
default abs                     ; [0xB8000] below is an absolute address, not rip-relative
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
    mov dword [PD], 0x000000 | 0x83    ; ...whose first entry is a 2 MiB page at 0
    mov dword [PD + 8], 0x200000 | 0x83 ; and the second one, 2 MiB on, covers 2-4 MiB

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

    mov byte [0x300000], 'M'    ; 3 MiB: inside the second page, or a page fault
    mov al, [0x300000]          ; read it back
    mov ah, 0x1F                ; white on blue
    mov [0xB8000], ax           ; top-left screen cell
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

times 510 - ($ - $$) db 0
dw 0xAA55
