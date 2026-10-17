; ui.asm - the pieces every lesson uses to draw the screen and grade an answer.
;
; Duplicated in spirit from c/src/ui.c (same title/heading/wait_enter/rule/
; question/challenge shape every course exposes) but built from nothing but
; raw Linux syscalls -- read, write, exit. There is no libc here: this course
; is the one place in CodeLearner where "written in the language it teaches"
; is taken completely literally, the same rule c/, cpp/, python/, javascript/
; and java/ already follow, just with no standard library underneath it.
;
; System V AMD64 calling convention throughout: integer/pointer arguments in
; rdi, rsi, rdx, rcx, r8, r9 in that order; return value in rax; rax, rcx,
; rdx, rsi, rdi, r8-r11 are caller-saved (a callee may clobber them freely);
; rbx, rsp, rbp, r12-r15 are callee-saved. Any function here that needs an
; argument to survive past more than one call saves it into its own real
; stack frame (push rbp; mov rbp, rsp; sub rsp, N) rather than push/pop pairs
; around each individual call -- the same thing a compiler's -O0 output
; already does, verified in this module's own lesson content.
;
; Assembled with NASM (Intel syntax):  nasm -f elf64 ui.asm -o ui.o

default rel                     ; [label] means rip-relative, as in 64-bit code generally

global print
global print_cstr
global print_paged
global print_num
global print_hex
global print_hex_fixed
global print_bin
global print_nl
global show_dec
global show_hex
global show_hexw
global show_bin
global strlen
global read_line
global rule
global title
global heading
global wait_enter
global ask_yes
global offer_solution
global question
global clear_screen

PAGE_LINES equ 26               ; print_paged stops after this many lines

section .rodata

; ESC [ H  ESC [ 2 J  ESC [ 3 J: cursor to top-left, clear the visible screen,
; clear the scrollback -- the exact three ANSI codes c/src/ui.c's own
; clear_screen uses, and the same reason given there: no second process per
; screen, no dependency on TERM being set.
clear_screen_seq:
    db 27, "[H", 27, "[2J", 27, "[3J"
clear_screen_seq_len equ $ - clear_screen_seq

rule_line:          db "  ------------------------------------------------------", 10, 0
title_top:          db 10, "  ========================================================", 10, "  ", 0
title_bottom:       db 10, "  ========================================================", 10, 10, 0
heading_prefix:     db "  ", 0
newline_str:        db 10, 0
wait_prompt:        db 10, "  Press ENTER to continue...", 0
yes_prompt_suffix:  db " (y/N): ", 0
your_answer_prompt: db 10, "  Your answer: ", 0
correct_prefix:     db 10, "  CORRECT.  ", 0
wrong_prefix:       db 10, "  NOT QUITE. The answer is: ", 0
why_prefix:         db 10, "             ", 0
example_question:   db "Want to see example code?", 0

section .bss
; Shared scratch buffers. One course, one menu loop, one question on screen
; at a time -- nothing here is reentrant and nothing needs to be.
answer_buf:  resb 256
discard_buf: resb 256

section .text

; print(rdi=ptr, rsi=len) -> writes rsi bytes at rdi to stdout (fd 1).
print:
    mov rdx, rsi
    mov rsi, rdi
    mov rdi, 1
    mov rax, 1
    syscall
    ret

; strlen(rdi=ptr) -> rax=length up to the first 0 byte. Never writes rdi.
strlen:
    xor rax, rax
.next:
    cmp byte [rdi+rax], 0
    je .done
    inc rax
    jmp .next
.done:
    ret

; print_cstr(rdi=ptr) -> writes the null-terminated string at ptr to stdout.
print_cstr:
    call strlen
    mov rsi, rax
    call print
    ret

; print_paged(rdi=text) -> like print_cstr, but for text longer than a screen:
; after every PAGE_LINES lines it waits for ENTER and clears the screen. The
; code listings in the later modules go through here.
; r12 = start of the part not yet written, r13 = lines on this page so far,
; r14 = the byte being looked at. All three are callee-saved, so print and
; wait_enter cannot disturb them (module 3, PART 3).
print_paged:
    push rbp
    mov rbp, rsp
    push r12
    push r13
    push r14
    mov r12, rdi
    mov r14, rdi
    xor r13, r13
.scan:
    movzx eax, byte [r14]
    test al, al
    jz .last
    inc r14
    cmp al, 10
    jne .scan
    inc r13
    cmp r13, PAGE_LINES
    jb .scan
    cmp byte [r14], 0           ; a full page, but the text ends here: no pause
    je .last
    mov rdi, r12                ; a full page: write it, wait, clear
    mov rsi, r14
    sub rsi, r12
    call print
    call wait_enter
    call clear_screen
    mov r12, r14
    xor r13, r13
    jmp .scan
.last:
    mov rdi, r12                ; whatever is left after the last full page
    mov rsi, r14
    sub rsi, r12
    call print
    pop r14
    pop r13
    pop r12
    leave
    ret

; print_num(rdi=value) -> writes an unsigned decimal number to stdout.
; Builds the digits backwards into a stack buffer (there is no libc itoa
; here), then prints them in the right order -- the standard way to turn a
; binary value into decimal text with nothing but repeated division by 10.
print_num:
    push rbp
    mov rbp, rsp
    sub rsp, 32
    mov rax, rdi
    lea rsi, [rbp-1]            ; write backwards from the end of the buffer
    mov rcx, 10
    cmp rax, 0
    jne .digit
    mov byte [rsi], '0'
    dec rsi
    jmp .done
.digit:
    cmp rax, 0
    je .done
    xor rdx, rdx
    div rcx                     ; rax / 10 -> rax=quotient, rdx=remainder
    add dl, '0'
    mov [rsi], dl
    dec rsi
    jmp .digit
.done:
    inc rsi                     ; back up to the first digit actually written
    mov rdx, rbp                ; one past the last digit slot (buffer end)
    sub rdx, rsi                ; rdx = length = end - start
    mov rdi, rsi                ; ptr -> print's 1st arg
    mov rsi, rdx                ; len -> print's 2nd arg
    call print
    leave
    ret

; print_nl() -> writes a newline.
print_nl:
    mov rdi, newline_str
    call print_cstr
    ret

; print_hex_fixed(rdi=value, rsi=digits) -> "0x" and exactly `digits` (1-16)
; lowercase hex digits, the way an address or a register dump is written.
; Builds the text in a stack buffer, last digit first, like print_num does.
print_hex_fixed:
    push rbp
    mov rbp, rsp
    sub rsp, 32
    mov byte [rbp-32], '0'
    mov byte [rbp-31], 'x'
    lea r8, [rbp-30]            ; where the digits go
    mov rax, rdi
    mov rdx, rsi                ; digits still to write, counting down
.next:
    test rdx, rdx
    jz .done
    mov r9, rax
    and r9, 0xF
    cmp r9, 10
    jb .decimal
    add r9, 'a' - 10
    jmp .store
.decimal:
    add r9, '0'
.store:
    mov [r8+rdx-1], r9b
    shr rax, 4
    dec rdx
    jmp .next
.done:
    lea rdi, [rbp-32]
    lea rsi, [rsi+2]            ; "0x" plus the digits
    call print
    leave
    ret

; print_hex(rdi=value) -> "0x" and as many hex digits as the value needs (at
; least one). Counts the nibbles, then prints them with print_hex_fixed.
print_hex:
    mov rsi, 1
    mov rax, rdi
    shr rax, 4
.count:
    test rax, rax
    jz .go
    inc rsi
    shr rax, 4
    jmp .count
.go:
    jmp print_hex_fixed

; print_bin(rdi=value, rsi=bits) -> the low `bits` bits as 0s and 1s, most
; significant first, with a space between each group of four.
print_bin:
    push rbp
    mov rbp, rsp
    sub rsp, 96
    mov rcx, rsi                ; bits left; also the index of the next bit
    lea r8, [rbp-96]            ; where the next character goes
    mov rax, rdi
.next:
    test rcx, rcx
    jz .done
    dec rcx
    mov rdx, rax
    shr rdx, cl
    and edx, 1
    add dl, '0'
    mov [r8], dl
    inc r8
    test rcx, 3
    jnz .next
    test rcx, rcx
    jz .next
    mov byte [r8], ' '          ; a nibble just ended, and it is not the last
    inc r8
    jmp .next
.done:
    lea rdi, [rbp-96]
    mov rsi, r8
    sub rsi, rdi
    call print
    leave
    ret

; The show_* functions print a label, a number and a newline: the lines a
; lesson computes while it runs, so the number on screen is the machine's.
;   show_dec (rdi=label, rsi=value)
;   show_hex (rdi=label, rsi=value)
;   show_hexw(rdi=label, rsi=value, rdx=digits)
;   show_bin (rdi=label, rsi=value, rdx=bits)
show_dec:
    push rbp
    mov rbp, rsp
    sub rsp, 16
    mov [rbp-8], rsi
    call print_cstr
    mov rdi, [rbp-8]
    call print_num
    call print_nl
    leave
    ret

show_hex:
    push rbp
    mov rbp, rsp
    sub rsp, 16
    mov [rbp-8], rsi
    call print_cstr
    mov rdi, [rbp-8]
    call print_hex
    call print_nl
    leave
    ret

show_hexw:
    push rbp
    mov rbp, rsp
    sub rsp, 32
    mov [rbp-8], rsi
    mov [rbp-16], rdx
    call print_cstr
    mov rdi, [rbp-8]
    mov rsi, [rbp-16]
    call print_hex_fixed
    call print_nl
    leave
    ret

show_bin:
    push rbp
    mov rbp, rsp
    sub rsp, 32
    mov [rbp-8], rsi
    mov [rbp-16], rdx
    call print_cstr
    mov rdi, [rbp-8]
    mov rsi, [rbp-16]
    call print_bin
    call print_nl
    leave
    ret

; read_line(rdi=buf, rsi=buf_size) -> rax=1 with buf null-terminated and any
; trailing \n stripped, or rax=0 at EOF. buf_size must include room for the
; terminator: the real read() count is buf_size-1, so null-terminating after
; whatever was read can never write past the buffer even with no newline in it.
read_line:
    mov r10, rdi
    dec rsi
    mov rdx, rsi
    mov rsi, r10
    xor rdi, rdi
    xor rax, rax
    syscall
    cmp rax, 0
    jle .eof
    mov rdi, r10
    xor rcx, rcx
.scan:
    cmp rcx, rax
    je .terminate
    cmp byte [rdi+rcx], 10
    je .found
    inc rcx
    jmp .scan
.found:
    mov byte [rdi+rcx], 0
    mov rax, 1
    ret
.terminate:
    mov byte [rdi+rax], 0
    mov rax, 1
    ret
.eof:
    xor rax, rax
    ret

; trim_and_lower(rdi=buf) -> rax=pointer to the trimmed, lowercased start.
; Skips leading whitespace by advancing the pointer (no bytes moved),
; lowercases A-Z in place, and null-terminates right after the last
; non-whitespace byte to drop anything trailing.
trim_and_lower:
.skip:
    cmp byte [rdi], ' '
    je .skip_one
    cmp byte [rdi], 9
    je .skip_one
    jmp .skipped
.skip_one:
    inc rdi
    jmp .skip
.skipped:
    mov rax, rdi
    mov r10, rdi
    mov r11, rdi
.scan:
    mov cl, [r10]
    cmp cl, 0
    je .end
    cmp cl, 'A'
    jl .not_upper
    cmp cl, 'Z'
    jg .not_upper
    add cl, 32
    mov [r10], cl
.not_upper:
    cmp cl, ' '
    je .is_space
    cmp cl, 9
    je .is_space
    lea r11, [r10+1]            ; one past the last non-space byte so far
.is_space:
    inc r10
    jmp .scan
.end:
    mov byte [r11], 0
    ret

; streq(rdi=a, rsi=b) -> rax=1 if the two null-terminated strings are equal.
streq:
.next:
    mov al, [rdi]
    mov cl, [rsi]
    cmp cl, al
    jne .no
    cmp al, 0
    je .yes
    inc rdi
    inc rsi
    jmp .next
.yes:
    mov rax, 1
    ret
.no:
    xor rax, rax
    ret

rule:
    mov rdi, rule_line
    call print_cstr
    ret

clear_screen:
    mov rdi, clear_screen_seq
    mov rsi, clear_screen_seq_len
    call print
    ret

; title(rdi=text) -> the framed heading every module starts with.
title:
    push rbp
    mov rbp, rsp
    sub rsp, 16
    mov [rbp-8], rdi
    mov rdi, title_top
    call print_cstr
    mov rdi, [rbp-8]
    call print_cstr
    mov rdi, title_bottom
    call print_cstr
    leave
    ret

; heading(rdi=text) -> a PART/SUMMARY-style heading inside a module.
heading:
    push rbp
    mov rbp, rsp
    sub rsp, 16
    mov [rbp-8], rdi
    mov rdi, newline_str
    call print_cstr
    mov rdi, heading_prefix
    call print_cstr
    mov rdi, [rbp-8]
    call print_cstr
    mov rdi, newline_str
    call print_cstr
    call rule
    leave
    ret

wait_enter:
    mov rdi, wait_prompt
    call print_cstr
    mov rdi, discard_buf
    mov rsi, 256
    call read_line
    ret

; ask_yes(rdi=question_text) -> rax=1 for yes, 0 for anything else (ENTER
; included, same as every other course's ask_yes).
ask_yes:
    push rbp
    mov rbp, rsp
    sub rsp, 16
    mov [rbp-8], rdi
    mov rdi, newline_str
    call print_cstr
    mov rdi, [rbp-8]
    call print_cstr
    mov rdi, yes_prompt_suffix
    call print_cstr

    mov rdi, answer_buf
    mov rsi, 256
    call read_line
    cmp rax, 0
    je .no

    mov al, [answer_buf]
    cmp al, 'y'
    je .yes
    cmp al, 'Y'
    je .yes
.no:
    xor rax, rax
    jmp .ret
.yes:
    mov rax, 1
.ret:
    leave
    ret

; offer_solution(rdi=example source) -> asks "Want to see example code?" and,
; on a yes, prints the source between two rules. The end of every challenge.
offer_solution:
    push rbp
    mov rbp, rsp
    sub rsp, 16
    mov [rbp-8], rdi
    mov rdi, example_question
    call ask_yes
    cmp rax, 1
    jne .done
    mov rdi, newline_str
    call print_cstr
    call rule
    mov rdi, [rbp-8]
    call print_cstr
    call rule
.done:
    leave
    ret

; question(rdi=text, rsi=correct, rdx=why) -> rax=1 if answered correctly.
; Always shows `why` -- somebody who guessed right still needs the reason,
; the same rule every course's question() follows. All three arguments are
; stored into this function's own stack frame immediately, since every one
; of them needs to survive several calls to functions that clobber rdi/rsi/rdx.
question:
    push rbp
    mov rbp, rsp
    sub rsp, 32
    mov [rbp-8], rdi            ; text
    mov [rbp-16], rsi           ; correct
    mov [rbp-24], rdx           ; why

    mov rdi, newline_str
    call print_cstr
    mov rdi, [rbp-8]
    call print_cstr

    mov rdi, your_answer_prompt
    call print_cstr

    mov rdi, answer_buf
    mov rsi, 256
    call read_line

    mov rdi, answer_buf
    call trim_and_lower         ; rax = trimmed, lowercased typed answer

    mov rdi, rax
    mov rsi, [rbp-16]
    call streq

    cmp rax, 1
    je .right

    mov rdi, wrong_prefix
    call print_cstr
    mov rdi, [rbp-16]
    call print_cstr
    mov rdi, why_prefix
    call print_cstr
    mov rdi, [rbp-24]
    call print_cstr
    mov rdi, newline_str
    call print_cstr
    xor rax, rax
    jmp .ret

.right:
    mov rdi, correct_prefix
    call print_cstr
    mov rdi, [rbp-24]
    call print_cstr
    mov rdi, newline_str
    call print_cstr
    mov rax, 1

.ret:
    leave
    ret

; Tells the linker this object does not need an executable stack.
section .note.GNU-stack noalloc noexec nowrite progbits
