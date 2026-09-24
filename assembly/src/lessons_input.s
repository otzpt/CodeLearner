# Module 4 - reading input, and a number-guessing game.
#
# The first module whose program is interactive. Everything it needs was
# built in modules 1-3: a syscall (1), a branch (2), a jump backwards (3).
# What is genuinely new is that stdin hands you bytes, not values, and
# turning those bytes into a number is work nobody does for you here.

.global lesson_04_input

.section .rodata

m4_title:      .asciz "MODULE 4 - READING INPUT, AND A GUESSING GAME"

m4_part1_hdr:  .asciz "PART 1: the read syscall"
m4_part1_a:    .asciz "  Input is the same shape as output, with a different number in rax"
m4_part1_b:    .asciz "  and a different direction:"
m4_part1_c:    .asciz ""
m4_part1_d:    .asciz "    xor %rax, %rax       # syscall 0 = read"
m4_part1_e:    .asciz "    xor %rdi, %rdi       # fd 0 = stdin"
m4_part1_f:    .asciz "    mov $buf, %rsi       # where to put the bytes"
m4_part1_g:    .asciz "    mov $32, %rdx        # how many, at most"
m4_part1_h:    .asciz "    syscall              # rax = how many actually arrived"
m4_part1_i:    .asciz ""
m4_part1_j:    .asciz "  xor %rax, %rax instead of mov $0, %rax for the same reason"
m4_part1_k:    .asciz "  compilers do it: a register XORed with itself is zero, in fewer"
m4_part1_l:    .asciz "  bytes. Three things about the return value in rax matter, and all"
m4_part1_m:    .asciz "  three are bugs waiting if you ignore them:"
m4_part1_n:    .asciz ""
m4_part1_o:    .asciz "    rax > 0   that many bytes arrived -- possibly fewer than asked"
m4_part1_p:    .asciz "    rax = 0   end of input (Ctrl-D), and no bytes were written"
m4_part1_q:    .asciz "    rax < 0   an error, as a negative errno value"

m4_part2_hdr:  .asciz "PART 2: what read does NOT do for you"
m4_part2_a:    .asciz "  read is not a line reader. It hands you raw bytes, and:"
m4_part2_b:    .asciz ""
m4_part2_c:    .asciz "    - it does not write a terminating 0. Whatever was in the buffer"
m4_part2_d:    .asciz "      before is still sitting right after what it wrote."
m4_part2_e:    .asciz "    - the newline you pressed IS one of the bytes. \"42\" typed at a"
m4_part2_f:    .asciz "      prompt arrives as three bytes: '4', '2', and 10."
m4_part2_g:    .asciz "    - it can return fewer bytes than you asked for and still be"
m4_part2_h:    .asciz "      working perfectly."
m4_part2_i:    .asciz ""
m4_part2_j:    .asciz "  read_line in ui.s is this course's answer to all three: it asks"
m4_part2_k:    .asciz "  for buf_size - 1 bytes so the 0 it writes afterwards can never"
m4_part2_l:    .asciz "  land past the end, scans what arrived for a newline and replaces"
m4_part2_m:    .asciz "  it with the 0, and reports EOF as a return of 0 rather than"
m4_part2_n:    .asciz "  pretending an empty line was typed."
m4_part2_o:    .asciz ""
m4_part2_p:    .asciz "  Asking for exactly buf_size and then terminating is the classic"
m4_part2_q:    .asciz "  one-byte overflow. Nothing crashes while you are testing it."

m4_part3_hdr:  .asciz "PART 3: text is not a number"
m4_part3_a:    .asciz "  The bytes '4' and '2' are 52 and 50. Nothing about them is 42."
m4_part3_b:    .asciz "  Converting is one loop, and it is the same one in every language"
m4_part3_c:    .asciz "  that has ever had an atoi:"
m4_part3_d:    .asciz ""
m4_part3_e:    .asciz "    xor %r13, %r13           # the number so far"
m4_part3_f:    .asciz "    mov $buf, %rsi"
m4_part3_g:    .asciz "  .digit:"
m4_part3_h:    .asciz "    movzbq (%rsi), %rax      # one byte, zero-extended to 64 bits"
m4_part3_i:    .asciz "    cmp $'0', %al"
m4_part3_j:    .asciz "    jl .parsed               # not a digit -- we are done"
m4_part3_k:    .asciz "    cmp $'9', %al"
m4_part3_l:    .asciz "    jg .parsed"
m4_part3_m:    .asciz "    sub $'0', %al            # '7' -> 7"
m4_part3_n:    .asciz "    imul $10, %r13           # shift the running total up a place"
m4_part3_o:    .asciz "    add %rax, %r13"
m4_part3_p:    .asciz "    inc %rsi"
m4_part3_q:    .asciz "    jmp .digit"
m4_part3_r:    .asciz "  .parsed:"
m4_part3_s:    .asciz ""
m4_part3_t:    .asciz "  The two cmp/j pairs are not validation for its own sake -- they"
m4_part3_u:    .asciz "  are what stops the loop, because the newline (10) fails both"
m4_part3_v:    .asciz "  tests. Leave them out and the loop runs off the end of the"
m4_part3_w:    .asciz "  buffer through whatever memory follows it."

m4_part4_hdr:  .asciz "PART 4: printing a number back, and div's trap"
m4_part4_a:    .asciz "  The other direction is repeated division by 10, digits falling"
m4_part4_b:    .asciz "  out backwards -- print_num in ui.s, which every module uses:"
m4_part4_c:    .asciz ""
m4_part4_d:    .asciz "    xor %rdx, %rdx           # THIS LINE IS NOT OPTIONAL"
m4_part4_e:    .asciz "    div %rcx                 # rdx:rax / rcx -> rax, rdx=remainder"
m4_part4_f:    .asciz "    add $'0', %dl            # remainder -> a character"
m4_part4_g:    .asciz ""
m4_part4_h:    .asciz "  div does not divide rax. It divides the 128-bit value held in"
m4_part4_i:    .asciz "  rdx:rax, and there is no way to tell it otherwise. Leaving"
m4_part4_j:    .asciz "  garbage in rdx means either a wrong answer or a quotient too"
m4_part4_k:    .asciz "  large to fit in rax, which raises #DE -- your process dies with"
m4_part4_l:    .asciz "  SIGFPE on a division that was never by zero."
m4_part4_m:    .asciz ""
m4_part4_n:    .asciz "  That is the single most common way a hand-written asm routine"
m4_part4_o:    .asciz "  crashes on its first run. The fix is one xor, above every div."

m4_part5_hdr:  .asciz "PART 5: the guessing game, as a shape"
m4_part5_a:    .asciz "  Modules 1 to 4 add up to a real interactive program, and it is"
m4_part5_b:    .asciz "  the loop you already know with a compare in the middle:"
m4_part5_c:    .asciz ""
m4_part5_d:    .asciz "    .again:  print the prompt          (module 1: write)"
m4_part5_e:    .asciz "             read a line               (module 4: read)"
m4_part5_f:    .asciz "             EOF? -> stop              (module 2: cmp/jle)"
m4_part5_g:    .asciz "             bytes -> number           (module 4: the digit loop)"
m4_part5_h:    .asciz "             equal?  -> say so, exit   (module 2: je)"
m4_part5_i:    .asciz "             lower?  -> say so"
m4_part5_j:    .asciz "             else    -> say so"
m4_part5_k:    .asciz "             jmp .again                (module 3)"
m4_part5_l:    .asciz ""
m4_part5_m:    .asciz "  Keep the secret in r12 and the guess in r13: both are"
m4_part5_n:    .asciz "  callee-saved, and a syscall clobbers rcx and r11 on its way"
m4_part5_o:    .asciz "  through, so a counter parked in rcx would not survive the read."
m4_part5_p:    .asciz ""
m4_part5_q:    .asciz "  A fixed secret makes the game testable, which is why the example"
m4_part5_r:    .asciz "  code uses one. For a secret that changes, rdtsc puts the CPU's"
m4_part5_s:    .asciz "  own cycle counter in edx:eax -- fine for a game, not random"
m4_part5_t:    .asciz "  enough for anything that matters."

m4_summary_hdr: .asciz "SUMMARY"
m4_sum_a:      .asciz "   - read is syscall 0 on fd 0, and rax comes back as a count, 0"
m4_sum_b:      .asciz "     for EOF, or a negative errno"
m4_sum_c:      .asciz "   - read writes no terminator and keeps your newline; asking for"
m4_sum_d:      .asciz "     size - 1 leaves room to terminate it safely"
m4_sum_e:      .asciz "   - digits are bytes: sub $'0' to get a value, add $'0' to get a"
m4_sum_f:      .asciz "     character back"
m4_sum_g:      .asciz "   - n = n * 10 + digit, stopping on the first byte outside '0'-'9'"
m4_sum_h:      .asciz "   - xor %rdx, %rdx before every div, or a #DE kills the process"
m4_sum_i:      .asciz "   - values that must outlive a syscall go in r12-r15, not rcx"

m4_q1_text:    .asciz "read() returns 0 in rax. What happened? (one word)"
m4_q1_correct: .asciz "eof"
m4_q1_why:     .asciz "0 means end of input -- Ctrl-D, or a closed pipe. No bytes were\n             written to your buffer, so whatever is in it is stale."
m4_q2_text:    .asciz "Which register must be cleared before div, or the process dies\n  with SIGFPE? (one register)"
m4_q2_correct: .asciz "rdx"
m4_q2_why:     .asciz "div divides rdx:rax, not rax. Leftover bits in rdx make the\n             quotient too big for rax, which raises #DE."
m4_q3_text:    .asciz "You typed 42 and pressed ENTER. How many bytes did read() give\n  you? (a number)"
m4_q3_correct: .asciz "3"
m4_q3_why:     .asciz "'4', '2', and the newline (10). The ENTER you pressed is data\n             like any other byte, and the digit loop has to stop on it."

m4_chal_hdr:   .asciz "\n  >> WRITE THIS YOURSELF, in a real file"
m4_chal_a:     .asciz "  Write the guessing game. A fixed secret between 1 and 100, then"
m4_chal_b:     .asciz "  loop: prompt, read a line, turn it into a number, and say \"Too"
m4_chal_c:     .asciz "  low.\" / \"Too high.\" / \"Got it.\" Exit on a correct guess, and"
m4_chal_d:     .asciz "  exit cleanly on Ctrl-D instead of looping forever."
m4_chal_out:   .asciz "\n  A session with the secret set to 37 must look like:\n\n      Guess (1-100): 50\n      Too high.\n      Guess (1-100): 20\n      Too low.\n      Guess (1-100): 37\n      Got it.\n"
m4_chal_build: .asciz "\n  Assemble and link with:\n    as prog.s -o prog.o && ld prog.o -o prog\n"
m4_chal_ask:   .asciz "Want to see example code?"
m4_chal_sol:   .asciz ".global _start\n\n.section .rodata\nask:   .ascii \"Guess (1-100): \"\n       ask_len = . - ask\nlow:   .ascii \"Too low.\\n\"\n       low_len = . - low\nhigh:  .ascii \"Too high.\\n\"\n       high_len = . - high\nwin:   .ascii \"Got it.\\n\"\n       win_len = . - win\n\n.section .bss\n    .lcomm buf, 32\n\n.section .text\n_start:\n    mov $37, %r12            # the secret; rdtsc here for a varying one\n\n.again:\n    mov $1, %rax             # write the prompt\n    mov $1, %rdi\n    mov $ask, %rsi\n    mov $ask_len, %rdx\n    syscall\n\n    xor %rax, %rax           # read a line\n    xor %rdi, %rdi\n    mov $buf, %rsi\n    mov $32, %rdx\n    syscall\n    cmp $0, %rax\n    jle .done                # 0 = EOF, negative = error: stop either way\n\n    xor %r13, %r13           # bytes -> number\n    mov $buf, %rsi\n.digit:\n    movzbq (%rsi), %rax\n    cmp $'0', %al\n    jl .parsed\n    cmp $'9', %al\n    jg .parsed\n    sub $'0', %al\n    imul $10, %r13\n    add %rax, %r13\n    inc %rsi\n    jmp .digit\n\n.parsed:\n    cmp %r12, %r13           # r13 - r12: guess against secret\n    je .correct\n    jl .too_low\n\n    mov $high, %rsi\n    mov $high_len, %rdx\n    jmp .say\n\n.too_low:\n    mov $low, %rsi\n    mov $low_len, %rdx\n\n.say:\n    mov $1, %rax\n    mov $1, %rdi\n    syscall\n    jmp .again\n\n.correct:\n    mov $1, %rax\n    mov $1, %rdi\n    mov $win, %rsi\n    mov $win_len, %rdx\n    syscall\n\n.done:\n    mov $0, %rdi\n    mov $60, %rax\n    syscall\n"

nl4: .asciz "\n"

m4_p1_lines:
    .quad m4_part1_a, m4_part1_b, m4_part1_c, m4_part1_d, m4_part1_e
    .quad m4_part1_f, m4_part1_g, m4_part1_h, m4_part1_i, m4_part1_j
    .quad m4_part1_k, m4_part1_l, m4_part1_m, m4_part1_n, m4_part1_o
    .quad m4_part1_p, m4_part1_q
m4_p1_count = (. - m4_p1_lines) / 8

m4_p2_lines:
    .quad m4_part2_a, m4_part2_b, m4_part2_c, m4_part2_d, m4_part2_e
    .quad m4_part2_f, m4_part2_g, m4_part2_h, m4_part2_i, m4_part2_j
    .quad m4_part2_k, m4_part2_l, m4_part2_m, m4_part2_n, m4_part2_o
    .quad m4_part2_p, m4_part2_q
m4_p2_count = (. - m4_p2_lines) / 8

m4_p3_lines:
    .quad m4_part3_a, m4_part3_b, m4_part3_c, m4_part3_d, m4_part3_e
    .quad m4_part3_f, m4_part3_g, m4_part3_h, m4_part3_i, m4_part3_j
    .quad m4_part3_k, m4_part3_l, m4_part3_m, m4_part3_n, m4_part3_o
    .quad m4_part3_p, m4_part3_q, m4_part3_r, m4_part3_s, m4_part3_t
    .quad m4_part3_u, m4_part3_v, m4_part3_w
m4_p3_count = (. - m4_p3_lines) / 8

m4_p4_lines:
    .quad m4_part4_a, m4_part4_b, m4_part4_c, m4_part4_d, m4_part4_e
    .quad m4_part4_f, m4_part4_g, m4_part4_h, m4_part4_i, m4_part4_j
    .quad m4_part4_k, m4_part4_l, m4_part4_m, m4_part4_n, m4_part4_o
m4_p4_count = (. - m4_p4_lines) / 8

m4_p5_lines:
    .quad m4_part5_a, m4_part5_b, m4_part5_c, m4_part5_d, m4_part5_e
    .quad m4_part5_f, m4_part5_g, m4_part5_h, m4_part5_i, m4_part5_j
    .quad m4_part5_k, m4_part5_l, m4_part5_m, m4_part5_n, m4_part5_o
    .quad m4_part5_p, m4_part5_q, m4_part5_r, m4_part5_s, m4_part5_t
m4_p5_count = (. - m4_p5_lines) / 8

m4_sum_lines:
    .quad m4_sum_a, m4_sum_b, m4_sum_c, m4_sum_d, m4_sum_e, m4_sum_f
    .quad m4_sum_g, m4_sum_h, m4_sum_i
m4_sum_count = (. - m4_sum_lines) / 8

m4_chal_lines:
    .quad m4_chal_a, m4_chal_b, m4_chal_c, m4_chal_d
m4_chal_count = (. - m4_chal_lines) / 8

m4_ex_marker: .asciz "\n  >> EXERCISE - MODULE 4\n  ------------------------------------------------------\n"

.section .text

lesson_04_input:
    push %rbp
    mov %rsp, %rbp

    mov $m4_title, %rdi
    call title

    mov $m4_part1_hdr, %rdi
    call heading
    mov $m4_p1_lines, %rdi
    mov $m4_p1_count, %rsi
    call print_lines
    call wait_enter
    call clear_screen

    mov $m4_part2_hdr, %rdi
    call heading
    mov $m4_p2_lines, %rdi
    mov $m4_p2_count, %rsi
    call print_lines
    call wait_enter
    call clear_screen

    mov $m4_part3_hdr, %rdi
    call heading
    mov $m4_p3_lines, %rdi
    mov $m4_p3_count, %rsi
    call print_lines
    call wait_enter
    call clear_screen

    mov $m4_part4_hdr, %rdi
    call heading
    mov $m4_p4_lines, %rdi
    mov $m4_p4_count, %rsi
    call print_lines
    call wait_enter
    call clear_screen

    mov $m4_part5_hdr, %rdi
    call heading
    mov $m4_p5_lines, %rdi
    mov $m4_p5_count, %rsi
    call print_lines
    call wait_enter
    call clear_screen

    # exercise(4)
    mov $m4_ex_marker, %rdi
    call print_cstr

    mov $m4_q1_text, %rdi
    mov $m4_q1_correct, %rsi
    mov $m4_q1_why, %rdx
    call question

    mov $m4_q2_text, %rdi
    mov $m4_q2_correct, %rsi
    mov $m4_q2_why, %rdx
    call question

    mov $m4_q3_text, %rdi
    mov $m4_q3_correct, %rsi
    mov $m4_q3_why, %rdx
    call question

    mov $m4_chal_hdr, %rdi
    call print_cstr
    mov $nl4, %rdi
    call print_cstr
    mov $m4_chal_lines, %rdi
    mov $m4_chal_count, %rsi
    call print_lines
    mov $m4_chal_out, %rdi
    call print_cstr
    mov $m4_chal_build, %rdi
    call print_cstr

    mov $m4_chal_ask, %rdi
    call ask_yes
    cmp $1, %rax
    jne .m4_no_solution
    mov $nl4, %rdi
    call print_cstr
    call rule
    mov $m4_chal_sol, %rdi
    call print_cstr
    call rule
.m4_no_solution:

    call wait_enter
    call clear_screen
    # summary(4)

    mov $m4_summary_hdr, %rdi
    call heading
    mov $m4_sum_lines, %rdi
    mov $m4_sum_count, %rsi
    call print_lines
    call wait_enter

    leave
    ret
