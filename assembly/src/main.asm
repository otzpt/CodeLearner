; Assembly course - main menu.
;
; Same shape as every other course's entry point: a menu, a loop, dispatch
; to a module, "0" to quit. The table of function pointers below is the
; MODULES array main.c has always had. Each entry is three quads: title,
; function, tier, and the menu prints a tier header whenever that third
; field changes, exactly like main.c's show_menu.
;
; The choice is read as a whole decimal number, the way module 4 teaches,
; so the course is not limited to nine modules.
;
; Build:  make
; Run:    ./asm-course

default rel

global _start

extern clear_screen
extern title
extern print_cstr
extern print_num
extern read_line
extern rule
extern wait_enter

extern lesson_01_registers
extern lesson_02_branching
extern lesson_03_loops
extern lesson_04_input
extern lesson_05_memory
extern lesson_06_stack
extern lesson_07_bits
extern lesson_08_strings
extern lesson_09_c
extern lesson_10_boot
extern lesson_11_hardware
extern lesson_12_loader
extern lesson_13_protected
extern lesson_14_long
extern lesson_15_kernel
extern lesson_16_interrupts

ENTRY_SIZE equ 24               ; three quads: title, function, tier

section .rodata

menu_title:        db "ASSEMBLY COURSE - REGISTERS TO AN OPERATING SYSTEM", 0

tier_basic:        db "BASIC", 0
tier_intermediate: db "INTERMEDIATE", 0
tier_advanced:     db "ADVANCED", 0

m1_name: db "Registers, syscalls, your first program", 0
m2_name: db "Comparing and branching: if/else", 0
m3_name: db "Loops", 0
m4_name: db "Reading input, and a guessing game", 0
m5_name: db "Memory and addressing", 0
m6_name: db "The stack, and calling a function", 0
m7_name: db "Bits, flags and arithmetic", 0
m8_name: db "Strings and block instructions", 0
m9_name: db "Assembly and C together", 0
m10_name: db "Real mode and the boot sector", 0
m11_name: db "Talking to the hardware", 0
m12_name: db "Loading more code: two stages, A20, the memory map", 0
m13_name: db "Protected mode", 0
m14_name: db "Long mode and paging", 0
m15_name: db "A kernel in C", 0
m16_name: db "Interrupts", 0

modules:
    dq m1_name, lesson_01_registers, tier_basic
    dq m2_name, lesson_02_branching, tier_basic
    dq m3_name, lesson_03_loops,     tier_basic
    dq m4_name, lesson_04_input,     tier_intermediate
    dq m5_name, lesson_05_memory,    tier_intermediate
    dq m6_name, lesson_06_stack,     tier_intermediate
    dq m7_name, lesson_07_bits,      tier_intermediate
    dq m8_name, lesson_08_strings,   tier_intermediate
    dq m9_name, lesson_09_c,         tier_advanced
    dq m10_name, lesson_10_boot,     tier_advanced
    dq m11_name, lesson_11_hardware, tier_advanced
    dq m12_name, lesson_12_loader,   tier_advanced
    dq m13_name, lesson_13_protected, tier_advanced
    dq m14_name, lesson_14_long,     tier_advanced
    dq m15_name, lesson_15_kernel,   tier_advanced
    dq m16_name, lesson_16_interrupts, tier_advanced
module_count equ ($ - modules) / ENTRY_SIZE

tier_prefix:     db 10, "  -- ", 0
tier_suffix:     db " --", 10, 0
item_prefix:     db "   [", 0
item_pad:        db " ", 0
item_middle:     db "]  ", 0
menu_quit:       db 10, "   [ 0]  Quit", 10, 0
prompt:          db 10, "  Pick a module: ", 0
invalid_msg:     db 10, "  Not a valid option.", 10, 0
bye_msg:         db 10, "  See you next time.", 10, 10, 0
newline:         db 10, 0

section .bss
choice_buf: resb 16

section .text

_start:
.menu_loop:
    call clear_screen
    mov rdi, menu_title
    call title

    ; r12 = current entry, r13 = 1-based number shown, r14 = the tier
    ; pointer whose header is already on screen. All three are callee-saved,
    ; so print_cstr and print_num cannot disturb them (module 3, PART 3).
    mov r12, modules
    mov r13, 1
    xor r14, r14

.item_loop:
    cmp r13, module_count
    jg .items_done

    mov rax, [r12+16]           ; this entry's tier
    cmp rax, r14
    je .no_tier_header
    mov r14, rax
    mov rdi, tier_prefix
    call print_cstr
    mov rdi, r14
    call print_cstr
    mov rdi, tier_suffix
    call print_cstr
.no_tier_header:

    mov rdi, item_prefix
    call print_cstr
    cmp r13, 10
    jge .two_digits
    mov rdi, item_pad           ; keep the "]" in one column for 1-9
    call print_cstr
.two_digits:
    mov rdi, r13
    call print_num
    mov rdi, item_middle
    call print_cstr
    mov rdi, [r12]
    call print_cstr
    mov rdi, newline
    call print_cstr

    add r12, ENTRY_SIZE
    inc r13
    jmp .item_loop
.items_done:

    mov rdi, menu_quit
    call print_cstr
    call rule

    mov rdi, prompt
    call print_cstr

    mov rdi, choice_buf
    mov rsi, 16
    call read_line
    cmp rax, 0
    je .quit                    ; EOF (Ctrl-D) -- leave the same as 0

    ; The typed digits become a number in r15: n = n * 10 + digit, stopping
    ; at the 0 that read_line put after the last byte (module 4, PART 3).
    mov rsi, choice_buf
    cmp byte [rsi], 0
    je .invalid                 ; an empty line is not a choice
    xor r15, r15
.digit:
    movzx eax, byte [rsi]
    test al, al
    jz .parsed
    sub eax, '0'
    cmp eax, 9                  ; unsigned: a byte below '0' wraps to a huge
    ja .invalid                 ; number, so one test rejects both ends
    imul r15, 10
    add r15, rax
    cmp r15, 99                 ; two digits at most
    jg .invalid
    inc rsi
    jmp .digit
.parsed:

    cmp r15, 0
    je .quit
    cmp r15, module_count
    jg .invalid

    dec r15
    imul r15, ENTRY_SIZE
    mov r12, modules
    add r12, r15
    call clear_screen           ; same as main.c: the menu does not stay under the lesson
    call [r12+8]                ; the function pointer in this entry
    jmp .menu_loop

.invalid:
    mov rdi, invalid_msg
    call print_cstr
    call wait_enter
    jmp .menu_loop

.quit:
    mov rdi, bye_msg
    call print_cstr
    mov rdi, 0
    mov rax, 60
    syscall

; Tells the linker this object does not need an executable stack.
section .note.GNU-stack noalloc noexec nowrite progbits
