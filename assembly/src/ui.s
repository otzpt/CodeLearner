# ui.s - the pieces every lesson uses to draw the screen and grade an answer.
#
# Duplicated in spirit from c/src/ui.c (same title/heading/wait_enter/rule/
# question/challenge shape every course exposes) but built from nothing but
# raw Linux syscalls -- read, write, exit. There is no libc here: this course
# is the one place in CodeLearner where "written in the language it teaches"
# is taken completely literally, the same rule c/, cpp/, python/, javascript/
# and java/ already follow, just with no standard library underneath it.
#
# System V AMD64 calling convention throughout: integer/pointer arguments in
# rdi, rsi, rdx, rcx, r8, r9 in that order; return value in rax; rax, rcx,
# rdx, rsi, rdi, r8-r11 are caller-saved (a callee may clobber them freely);
# rbx, rsp, rbp, r12-r15 are callee-saved. Any function here that needs an
# argument to survive past more than one call saves it into its own real
# stack frame (push %rbp; mov %rsp, %rbp; sub $N, %rsp) rather than
# push/pop pairs around each individual call -- the same thing a compiler's
# -O0 output already does, verified in this module's own lesson content.

.global print
.global print_cstr
.global print_num
.global strlen
.global read_line
.global rule
.global title
.global heading
.global wait_enter
.global ask_yes
.global question
.global clear_screen

.section .rodata
# \033[H\033[2J\033[3J: cursor to top-left, clear the visible screen, clear
# the scrollback -- the exact three ANSI codes c/src/ui.c's own clear_screen
# uses, and the same reason given there: no second process per screen, no
# dependency on TERM being set.
clear_screen_seq:
    .ascii "\033[H\033[2J\033[3J"
    clear_screen_seq_len = . - clear_screen_seq
rule_line:
    .asciz "  ------------------------------------------------------\n"
title_top:
    .asciz "\n  ========================================================\n  "
title_bottom:
    .asciz "\n  ========================================================\n\n"
heading_prefix:
    .asciz "  "
newline_str:
    .asciz "\n"
wait_prompt:
    .asciz "\n  Press ENTER to continue..."
yes_prompt_suffix:
    .asciz " (y/N): "
your_answer_prompt:
    .asciz "\n  Your answer: "
correct_prefix:
    .asciz "\n  CORRECT.  "
wrong_prefix:
    .asciz "\n  NOT QUITE. The answer is: "
why_prefix:
    .asciz "\n             "

.section .bss
# Shared scratch buffers. One course, one menu loop, one question on screen
# at a time -- nothing here is reentrant and nothing needs to be.
.lcomm answer_buf, 256
.lcomm discard_buf, 256

.section .text

# print(rdi=ptr, rsi=len) -> writes rsi bytes at rdi to stdout (fd 1).
print:
    mov %rsi, %rdx
    mov %rdi, %rsi
    mov $1, %rdi
    mov $1, %rax
    syscall
    ret

# strlen(rdi=ptr) -> rax=length up to the first 0 byte. Never writes rdi.
strlen:
    xor %rax, %rax
.strlen_loop:
    cmpb $0, (%rdi, %rax)
    je .strlen_done
    inc %rax
    jmp .strlen_loop
.strlen_done:
    ret

# print_cstr(rdi=ptr) -> writes the null-terminated string at ptr to stdout.
print_cstr:
    call strlen
    mov %rax, %rsi
    call print
    ret

# print_num(rdi=value) -> writes an unsigned decimal number to stdout.
# Builds the digits backwards into a stack buffer (there is no libc itoa
# here), then prints them in the right order -- the standard way to turn a
# binary value into decimal text with nothing but repeated division by 10.
print_num:
    push %rbp
    mov %rsp, %rbp
    sub $32, %rsp
    mov %rdi, %rax
    lea -1(%rbp), %rsi     # write backwards from the end of the buffer
    mov $10, %rcx
    cmp $0, %rax
    jne .print_num_loop
    movb $'0', (%rsi)
    dec %rsi
    jmp .print_num_done
.print_num_loop:
    cmp $0, %rax
    je .print_num_done
    xor %rdx, %rdx
    div %rcx                # rax / 10 -> rax=quotient, rdx=remainder
    add $'0', %dl
    movb %dl, (%rsi)
    dec %rsi
    jmp .print_num_loop
.print_num_done:
    inc %rsi                 # back up to the first digit actually written
    mov %rbp, %rdx             # one past the last digit slot (buffer end)
    sub %rsi, %rdx               # rdx = length = end - start
    mov %rsi, %rdi                # ptr -> print's 1st arg
    mov %rdx, %rsi                 # len -> print's 2nd arg
    call print
    leave
    ret

# read_line(rdi=buf, rsi=buf_size) -> rax=1 with buf null-terminated and any
# trailing \n stripped, or rax=0 at EOF. buf_size must include room for the
# terminator: the real read() count is buf_size-1, so null-terminating after
# whatever was read can never write past the buffer even with no newline in it.
read_line:
    mov %rdi, %r10
    dec %rsi
    mov %rsi, %rdx
    mov %r10, %rsi
    xor %rdi, %rdi
    xor %rax, %rax
    syscall
    cmp $0, %rax
    jle .read_line_eof
    mov %r10, %rdi
    xor %rcx, %rcx
.read_line_scan:
    cmp %rcx, %rax
    je .read_line_terminate
    cmpb $10, (%rdi, %rcx)
    je .read_line_found
    inc %rcx
    jmp .read_line_scan
.read_line_found:
    movb $0, (%rdi, %rcx)
    mov $1, %rax
    ret
.read_line_terminate:
    movb $0, (%rdi, %rax)
    mov $1, %rax
    ret
.read_line_eof:
    xor %rax, %rax
    ret

# trim_and_lower(rdi=buf) -> rax=pointer to the trimmed, lowercased start.
# Skips leading whitespace by advancing the pointer (no bytes moved),
# lowercases A-Z in place, and null-terminates right after the last
# non-whitespace byte to drop anything trailing.
trim_and_lower:
.tal_skip_leading:
    cmpb $' ', (%rdi)
    je .tal_skip_leading_inc
    cmpb $9, (%rdi)
    je .tal_skip_leading_inc
    jmp .tal_skip_leading_done
.tal_skip_leading_inc:
    inc %rdi
    jmp .tal_skip_leading
.tal_skip_leading_done:
    mov %rdi, %rax
    mov %rdi, %r10
    mov %rdi, %r11
.tal_scan:
    movb (%r10), %cl
    cmp $0, %cl
    je .tal_scan_done
    cmp $'A', %cl
    jl .tal_not_upper
    cmp $'Z', %cl
    jg .tal_not_upper
    add $32, %cl
    movb %cl, (%r10)
.tal_not_upper:
    cmp $' ', %cl
    je .tal_is_space
    cmp $9, %cl
    je .tal_is_space
    lea 1(%r10), %r11
.tal_is_space:
    inc %r10
    jmp .tal_scan
.tal_scan_done:
    movb $0, (%r11)
    ret

# streq(rdi=a, rsi=b) -> rax=1 if the two null-terminated strings are equal.
streq:
.streq_loop:
    movb (%rdi), %al
    movb (%rsi), %cl
    cmp %al, %cl
    jne .streq_no
    cmp $0, %al
    je .streq_yes
    inc %rdi
    inc %rsi
    jmp .streq_loop
.streq_yes:
    mov $1, %rax
    ret
.streq_no:
    xor %rax, %rax
    ret

rule:
    mov $rule_line, %rdi
    call print_cstr
    ret

clear_screen:
    mov $clear_screen_seq, %rdi
    mov $clear_screen_seq_len, %rsi
    call print
    ret

# title(rdi=text) -> the framed heading every module starts with.
title:
    push %rbp
    mov %rsp, %rbp
    sub $16, %rsp
    mov %rdi, -8(%rbp)
    mov $title_top, %rdi
    call print_cstr
    mov -8(%rbp), %rdi
    call print_cstr
    mov $title_bottom, %rdi
    call print_cstr
    leave
    ret

# heading(rdi=text) -> a PART/SUMMARY-style heading inside a module.
heading:
    push %rbp
    mov %rsp, %rbp
    sub $16, %rsp
    mov %rdi, -8(%rbp)
    mov $newline_str, %rdi
    call print_cstr
    mov $heading_prefix, %rdi
    call print_cstr
    mov -8(%rbp), %rdi
    call print_cstr
    mov $newline_str, %rdi
    call print_cstr
    call rule
    leave
    ret

wait_enter:
    mov $wait_prompt, %rdi
    call print_cstr
    mov $discard_buf, %rdi
    mov $256, %rsi
    call read_line
    ret

# ask_yes(rdi=question_text) -> rax=1 for yes, 0 for anything else (ENTER
# included, same as every other course's ask_yes).
ask_yes:
    push %rbp
    mov %rsp, %rbp
    sub $16, %rsp
    mov %rdi, -8(%rbp)
    mov $newline_str, %rdi
    call print_cstr
    mov -8(%rbp), %rdi
    call print_cstr
    mov $yes_prompt_suffix, %rdi
    call print_cstr

    mov $answer_buf, %rdi
    mov $256, %rsi
    call read_line
    cmp $0, %rax
    je .ask_yes_no

    movb answer_buf(%rip), %al
    cmp $'y', %al
    je .ask_yes_yes
    cmp $'Y', %al
    je .ask_yes_yes
.ask_yes_no:
    xor %rax, %rax
    jmp .ask_yes_ret
.ask_yes_yes:
    mov $1, %rax
.ask_yes_ret:
    leave
    ret

# question(rdi=text, rsi=correct, rdx=why) -> rax=1 if answered correctly.
# Always shows `why` -- somebody who guessed right still needs the reason,
# the same rule every course's question() follows. All three arguments are
# stored into this function's own stack frame immediately, since every one
# of them needs to survive several calls to functions that clobber rdi/rsi/rdx.
question:
    push %rbp
    mov %rsp, %rbp
    sub $32, %rsp
    mov %rdi, -8(%rbp)      # text
    mov %rsi, -16(%rbp)      # correct
    mov %rdx, -24(%rbp)       # why

    mov $newline_str, %rdi
    call print_cstr
    mov -8(%rbp), %rdi
    call print_cstr

    mov $your_answer_prompt, %rdi
    call print_cstr

    mov $answer_buf, %rdi
    mov $256, %rsi
    call read_line

    mov $answer_buf, %rdi
    call trim_and_lower       # rax = trimmed, lowercased typed answer

    mov %rax, %rdi
    mov -16(%rbp), %rsi
    call streq

    cmp $1, %rax
    je .question_right

    mov $wrong_prefix, %rdi
    call print_cstr
    mov -16(%rbp), %rdi
    call print_cstr
    mov $why_prefix, %rdi
    call print_cstr
    mov -24(%rbp), %rdi
    call print_cstr
    mov $newline_str, %rdi
    call print_cstr
    xor %rax, %rax
    jmp .question_ret

.question_right:
    mov $correct_prefix, %rdi
    call print_cstr
    mov -24(%rbp), %rdi
    call print_cstr
    mov $newline_str, %rdi
    call print_cstr
    mov $1, %rax

.question_ret:
    leave
    ret
