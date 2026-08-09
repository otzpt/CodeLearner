# Assembly course - main menu.
#
# Same shape as every other course's entry point: a menu, a loop, dispatch
# to a module, "0" to quit. No table of function pointers the way main.c's
# MODULES array has one -- with a single module, a straight compare is the
# whole dispatch, and a jump table would be solving a problem this course
# does not have yet. Grows into one the moment a second module does.
#
# Build:  make
# Run:    ./asm-course

.global _start

.section .rodata
menu_title:  .asciz "ASSEMBLY COURSE - REGISTERS TO SYSCALLS"
menu_item1:  .asciz "   [ 1]  Registers, syscalls, your first program"
menu_quit:   .asciz "\n   [ 0]  Quit"
prompt:      .asciz "\n  Pick a module: "
invalid_msg: .asciz "\n  Not a valid option.\n"
bye_msg:     .asciz "\n  See you next time.\n\n"

.section .bss
.lcomm choice_buf, 16

.section .text

_start:
.menu_loop:
    call clear_screen
    mov $menu_title, %rdi
    call title
    mov $menu_item1, %rdi
    call print_cstr
    mov $nl_for_main, %rdi
    call print_cstr
    mov $menu_quit, %rdi
    call print_cstr
    mov $nl_for_main, %rdi
    call print_cstr
    call rule

    mov $prompt, %rdi
    call print_cstr

    mov $choice_buf, %rdi
    mov $16, %rsi
    call read_line
    cmp $0, %rax
    je .quit               # EOF (Ctrl-D) -- leave the same as choosing 0

    movb choice_buf(%rip), %al
    cmp $'0', %al
    je .quit

    cmp $'1', %al
    jne .invalid
    # require the choice to be exactly "1", not "1x" or similar
    cmpb $0, choice_buf+1(%rip)
    jne .invalid

    call lesson_01_registers
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

.section .rodata
nl_for_main: .asciz "\n"
