; stage2.asm - real mode to long mode, then into the kernel at 0x10000.
;
; The same journey as os/lm.asm (module 14), minus the message: this stage
; has no output of its own. When it is done it jumps to the kernel's entry.

org 0x7E00
bits 16

KERNEL equ 0x10000              ; where boot.asm put it, and where the linker script puts it

CODE32 equ 0x08
DATA   equ 0x10
CODE64 equ 0x18

PML4 equ 0x1000
PDPT equ 0x2000
PD   equ 0x3000

start:
    cli
    in al, 0x92                 ; A20 on
    or al, 2
    and al, 0xFE
    out 0x92, al
    lgdt [gdt_pointer]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE32:protected

bits 32
protected:
    mov ax, DATA
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov esp, 0x90000

    mov eax, 0x80000001         ; long mode available? (module 14)
    cpuid
    test edx, 1 << 29
    jz .no_long_mode

    mov edi, PML4               ; the page tables: identity-map the first 2 MiB
    xor eax, eax
    mov ecx, 3 * 4096 / 4
    rep stosd
    mov dword [PML4], PDPT | 0x03
    mov dword [PDPT], PD | 0x03
    mov dword [PD], 0x83

    mov eax, PML4
    mov cr3, eax
    mov eax, cr4
    or eax, 1 << 5              ; PAE
    mov cr4, eax
    mov ecx, 0xC0000080         ; EFER.LME
    rdmsr
    or eax, 1 << 8
    wrmsr
    mov eax, cr0
    or eax, 1 << 31             ; PG
    mov cr0, eax
    jmp CODE64:long_entry

.no_long_mode:
    mov dword [0xB8000], 0x4F4F4F4E     ; "NO" then "64", white on red
    mov dword [0xB8004], 0x4F344F36
.stop:
    hlt
    jmp .stop

bits 64
long_entry:
    mov ax, DATA
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov rax, KERNEL
    jmp rax                     ; into the C kernel's entry point

align 8
gdt_start:
    dq 0
    dq 0x00CF9A000000FFFF
    dq 0x00CF92000000FFFF
    dq 0x00AF9A000000FFFF
gdt_end:

gdt_pointer:
    dw gdt_end - gdt_start - 1
    dd gdt_start

times 1024 - ($ - $$) db 0      ; two sectors, what boot.asm reads
