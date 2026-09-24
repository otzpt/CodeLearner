# Assembly course - main menu.
#
# Same shape as every other course's entry point: a menu, a loop, dispatch
# to a module, "0" to quit. The straight compare that dispatched a single
# module has become the table of function pointers main.c's MODULES array
# has always been -- the growth the earlier version of this file said would
# happen the moment a second module existed. Each entry is three quads:
# title, function, tier, and the menu prints a tier header whenever that
# third field changes, exactly like main.c's show_menu.
#
# ponytail: one-digit choices only, since the parse is a single byte minus
# '0'. Fine to 9 modules; past that, read the whole line as a number the
# way module 4 teaches.
#
# Build:  make
# Run:    ./asm-course

.global _start

.section .rodata
menu_title:      .asciz "ASSEMBLY COURSE - REGISTERS TO A GUESSING GAME"

tier_basic:        .asciz "BASIC"
tier_intermediate: .asciz "INTERMEDIATE"

m1_name: .asciz "Registers, syscalls, your first program"
m2_name: .asciz "Comparing and branching: if/else"
m3_name: .asciz "Loops"
m4_name: .asciz "Reading input, and a guessing game"

# title, function, tier -- 24 bytes an entry.
modules:
    .quad m1_name, lesson_01_registers, tier_basic
    .quad m2_name, lesson_02_branching, tier_basic
    .quad m3_name, lesson_03_loops,     tier_basic
    .quad m4_name, lesson_04_input,     tier_intermediate
module_count = (. - modules) / 24

tier_prefix:     .asciz "\n  -- "
tier_suffix:     .asciz " --\n"
item_prefix:     .asciz "   [ "
item_middle:     .asciz "]  "
tier_advanced:   .asciz "\n  -- ADVANCED -- (coming soon)\n"
menu_quit:       .asciz "\n   [ 0]  Quit\n"
prompt:          .asciz "\n  Pick a module: "
invalid_msg:     .asciz "\n  Not a valid option.\n"
bye_msg:         .asciz "\n  See you next time.\n\n"
nl_for_main:     .asciz "\n"

.section .bss
.lcomm choice_buf, 16

.section .text

_start:
.menu_loop:
    call clear_screen
    mov $menu_title, %rdi
    call title

    # r12 = current entry, r13 = 1-based number shown, r14 = the tier
    # pointer whose header is already on screen. All three are callee-saved,
    # so print_cstr and print_num cannot disturb them (module 3, PART 3).
    mov $modules, %r12
    mov $1, %r13
    xor %r14, %r14

.item_loop:
    cmp $module_count, %r13
    jg .items_done

    mov 16(%r12), %rax           # this entry's tier
    cmp %r14, %rax
    je .no_tier_header
    mov %rax, %r14
    mov $tier_prefix, %rdi
    call print_cstr
    mov %r14, %rdi
    call print_cstr
    mov $tier_suffix, %rdi
    call print_cstr
.no_tier_header:

    mov $item_prefix, %rdi
    call print_cstr
    mov %r13, %rdi
    call print_num
    mov $item_middle, %rdi
    call print_cstr
    mov (%r12), %rdi
    call print_cstr
    mov $nl_for_main, %rdi
    call print_cstr

    add $24, %r12
    inc %r13
    jmp .item_loop
.items_done:

    mov $tier_advanced, %rdi
    call print_cstr
    mov $menu_quit, %rdi
    call print_cstr
    call rule

    mov $prompt, %rdi
    call print_cstr

    mov $choice_buf, %rdi
    mov $16, %rsi
    call read_line
    cmp $0, %rax
    je .quit                     # EOF (Ctrl-D) -- leave the same as 0

    movzbq choice_buf(%rip), %rax
    cmp $'0', %al
    je .quit

    # require the choice to be exactly one digit, not "1x" or similar
    cmpb $0, choice_buf+1(%rip)
    jne .invalid

    sub $'0', %al
    cmp $1, %al
    jl .invalid
    cmp $module_count, %al
    jg .invalid

    movzbq %al, %rax
    dec %rax
    imul $24, %rax               # entry size
    mov $modules, %r12
    add %rax, %r12
    call *8(%r12)                # the function pointer in this entry
    jmp .menu_loop

.invalid:
    mov $invalid_msg, %rdi
    call print_cstr
    call wait_enter
    jmp .menu_loop

.quit:
    mov $bye_msg, %rdi
    call print_cstr
    mov $0, %rdi
    mov $60, %rax
    syscall
