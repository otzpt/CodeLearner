; course.inc - the macros every lesson file uses.
;
; A lesson is a title, five screens ("parts"), three questions, a challenge
; and a summary. Writing that out as call/mov by hand in every module would
; bury the content, so each piece is one macro here. Nothing in them is
; clever: every macro is the exact register setup and call you would write
; yourself, and `nasm -E file.asm` shows the expansion.
;
; The functions they call all live in ui.asm.

extern title
extern heading
extern print_cstr
extern print_paged
extern wait_enter
extern clear_screen
extern question
extern offer_solution
extern print_nl
extern print_num
extern print_hex
extern print_hex_fixed
extern print_bin
extern show_dec
extern show_hex
extern show_hexw
extern show_bin

; ln <string>   one line of lesson text: the string, then a newline. A screen
; of text is a label, a run of ln lines, and one db 0 to end it.
%macro ln 1+
    db %1, 10
%endmacro

; part <heading>, <text>   one screen: heading, text, wait, clear
%macro part 2
    mov rdi, %1
    call heading
    mov rdi, %2
    call print_cstr
    call wait_enter
    call clear_screen
%endmacro

; part_begin <heading>, <text>   the first half of a screen that also shows live
; output: the heading and the text. The lines the lesson computes while it
; runs go between this and part_end.
%macro part_begin 2
    mov rdi, %1
    call heading
    mov rdi, %2
    call print_cstr
%endmacro

%macro part_end 0
    call wait_enter
    call clear_screen
%endmacro

; live_dec <label>, <value>            label, then the value in decimal
; live_hex <label>, <value>            ... in hex, as few digits as it needs
; live_hexw <label>, <value>, <digits> ... in hex, exactly that many digits
; live_bin <label>, <value>, <bits>    ... in binary, in groups of four
; The value is read before anything is printed; it must not be rdi, rsi or rdx.
%macro live_dec 2
    mov rsi, %2
    mov rdi, %1
    call show_dec
%endmacro

%macro live_hex 2
    mov rsi, %2
    mov rdi, %1
    call show_hex
%endmacro

%macro live_hexw 3
    mov rdx, %3
    mov rsi, %2
    mov rdi, %1
    call show_hexw
%endmacro

%macro live_bin 3
    mov rdx, %3
    mov rsi, %2
    mov rdi, %1
    call show_bin
%endmacro

; file_screen <heading>, <file>, <text>   a real source file from examples/ (print_paged
; handles one longer than a screen), then some text, then wait and clear
%macro file_screen 3
    mov rdi, %1
    call heading
    mov rdi, %2
    call print_paged
    mov rdi, %3
    call print_cstr
    call wait_enter
    call clear_screen
%endmacro

; listing <heading>, <text>   like part, for text longer than a screen
%macro listing 2
    mov rdi, %1
    call heading
    mov rdi, %2
    call print_paged
    call wait_enter
    call clear_screen
%endmacro

; quiz <question>, <expected answer>, <why>   one graded question
%macro quiz 3
    mov rdi, %1
    mov rsi, %2
    mov rdx, %3
    call question
%endmacro

; challenge <task text>, <example solution>   the write-it-yourself task, and
; the example solution if the student asks for it
%macro challenge 2
    mov rdi, %1
    call print_cstr
    mov rdi, %2
    call offer_solution
%endmacro

; summary <heading>, <text>   the last screen of a module
%macro summary 2
    call wait_enter
    call clear_screen
    mov rdi, %1
    call heading
    mov rdi, %2
    call print_cstr
    call wait_enter
%endmacro
