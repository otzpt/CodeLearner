; lessons_input.asm - module 4: reading input, and a number-guessing game.
;
; The first module whose program is interactive. Everything it needs was
; built in modules 1-3: a syscall (1), a branch (2), a jump backwards (3).
; What is genuinely new is that stdin hands you bytes, not values, and
; turning those bytes into a number is work nobody does for you here.

%include "course.inc"

default rel

global lesson_04_input

section .rodata

; ---- module 4 ----------------------------------------------------
m4_title: db "MODULE 4 - READING INPUT, AND A GUESSING GAME", 0
m4_p1_hdr: db "PART 1: the read syscall", 0
m4_p2_hdr: db "PART 2: what read does NOT do for you", 0
m4_p3_hdr: db "PART 3: text is not a number", 0
m4_p4_hdr: db "PART 4: printing a number back, and div's trap", 0
m4_p5_hdr: db "PART 5: the guessing game, as a shape", 0

m4_p1:
    ln "  Input is the same shape as output, with a different number in rax"
    ln "  and a different direction:"
    ln ""
    ln "    xor rax, rax         ; syscall 0 = read"
    ln "    xor rdi, rdi         ; fd 0 = stdin"
    ln "    mov rsi, buf         ; where to put the bytes"
    ln "    mov rdx, 32          ; how many, at most"
    ln "    syscall              ; rax = how many actually arrived"
    ln ""
    ln "  xor rax, rax instead of mov rax, 0 for the same reason"
    ln "  compilers do it: a register XORed with itself is zero, in fewer"
    ln "  bytes. Three things about the return value in rax matter, and all"
    ln "  three are bugs waiting if you ignore them:"
    ln ""
    ln "    rax > 0   that many bytes arrived -- possibly fewer than asked"
    ln "    rax = 0   end of input (Ctrl-D), and no bytes were written"
    ln "    rax < 0   an error, as a negative errno value"
    db 0

m4_p2:
    ln "  read is not a line reader. It hands you raw bytes, and:"
    ln ""
    ln "    - it does not write a terminating 0. Whatever was in the buffer"
    ln "      before is still sitting right after what it wrote."
    ln '    - the newline you pressed IS one of the bytes. "42" typed at a'
    ln "      prompt arrives as three bytes: '4', '2', and 10."
    ln "    - it can return fewer bytes than you asked for and still be"
    ln "      working perfectly."
    ln ""
    ln "  read_line in ui.asm is this course's answer to all three: it asks"
    ln "  for buf_size - 1 bytes so the 0 it writes afterwards can never"
    ln "  land past the end, scans what arrived for a newline and replaces"
    ln "  it with the 0, and reports EOF as a return of 0 rather than"
    ln "  pretending an empty line was typed."
    ln ""
    ln "  Asking for exactly buf_size and then terminating is the classic"
    ln "  one-byte overflow. Nothing crashes while you are testing it."
    db 0

m4_p3:
    ln "  The bytes '4' and '2' are 52 and 50. Nothing about them is 42."
    ln "  Converting is one loop, and it is the same one in every language"
    ln "  that has ever had an atoi:"
    ln ""
    ln "    xor r13, r13             ; the number so far"
    ln "    mov rsi, buf"
    ln "  .digit:"
    ln "    movzx rax, byte [rsi]    ; one byte, zero-extended to 64 bits"
    ln "    cmp al, '0'"
    ln "    jl .parsed               ; not a digit -- we are done"
    ln "    cmp al, '9'"
    ln "    jg .parsed"
    ln "    sub al, '0'              ; '7' -> 7"
    ln "    imul r13, 10             ; shift the running total up a place"
    ln "    add r13, rax"
    ln "    inc rsi"
    ln "    jmp .digit"
    ln "  .parsed:"
    ln ""
    ln "  The two cmp/j pairs are not validation for its own sake -- they"
    ln "  are what stops the loop, because the newline (10) fails both"
    ln "  tests. Leave them out and the loop runs off the end of the"
    ln "  buffer through whatever memory follows it."
    db 0

m4_p4:
    ln "  The other direction is repeated division by 10, digits falling"
    ln "  out backwards -- print_num in ui.asm, which every module uses:"
    ln ""
    ln "    xor rdx, rdx             ; THIS LINE IS NOT OPTIONAL"
    ln "    div rcx                  ; rdx:rax / rcx -> rax, rdx=remainder"
    ln "    add dl, '0'              ; remainder -> a character"
    ln ""
    ln "  div does not divide rax. It divides the 128-bit value held in"
    ln "  rdx:rax, and there is no way to tell it otherwise. Leaving"
    ln "  garbage in rdx means either a wrong answer or a quotient too"
    ln "  large to fit in rax, which raises #DE -- your process dies with"
    ln "  SIGFPE on a division that was never by zero."
    ln ""
    ln "  That is the single most common way a hand-written asm routine"
    ln "  crashes on its first run. The fix is one xor, above every div."
    db 0

m4_p5:
    ln "  Modules 1 to 4 add up to a real interactive program, and it is"
    ln "  the loop you already know with a compare in the middle:"
    ln ""
    ln "    .again:  print the prompt          (module 1: write)"
    ln "             read a line               (module 4: read)"
    ln "             EOF? -> stop              (module 2: cmp/jle)"
    ln "             bytes -> number           (module 4: the digit loop)"
    ln "             equal?  -> say so, exit   (module 2: je)"
    ln "             lower?  -> say so"
    ln "             else    -> say so"
    ln "             jmp .again                (module 3)"
    ln ""
    ln "  Keep the secret in r12 and the guess in r13: both are"
    ln "  callee-saved, and a syscall clobbers rcx and r11 on its way"
    ln "  through, so a counter parked in rcx would not survive the read."
    ln ""
    ln "  A fixed secret makes the game testable, which is why the example"
    ln "  code uses one. For a secret that changes, rdtsc puts the CPU's"
    ln "  own cycle counter in edx:eax -- fine for a game, not random"
    ln "  enough for anything that matters."
    db 0

m4_exercise: db `\n  >> EXERCISE - MODULE 4\n  ------------------------------------------------------\n`, 0

m4_q1_text:   db "read() returns 0 in rax. What happened? (one word)", 0
m4_q1_answer: db "eof", 0
m4_q1_why:    db `0 means end of input -- Ctrl-D, or a closed pipe. No bytes were\n             written to your buffer, so whatever is in it is stale.`, 0

m4_q2_text:   db `Which register must be cleared before div, or the process dies\n  with SIGFPE? (one register)`, 0
m4_q2_answer: db "rdx", 0
m4_q2_why:    db `div divides rdx:rax, not rax. Leftover bits in rdx make the\n             quotient too big for rax, which raises #DE.`, 0

m4_q3_text:   db `You typed 42 and pressed ENTER. How many bytes did read() give\n  you? (a number)`, 0
m4_q3_answer: db "3", 0
m4_q3_why:    db `'4', '2', and the newline (10). The ENTER you pressed is data\n             like any other byte, and the digit loop has to stop on it.`, 0

m4_challenge:
    ln ""
    ln "  >> WRITE THIS YOURSELF, in a real file"
    ln "  Write the guessing game. A fixed secret between 1 and 100, then"
    ln '  loop: prompt, read a line, turn it into a number, and say "Too'
    ln '  low." / "Too high." / "Got it." Exit on a correct guess, and'
    ln "  exit cleanly on Ctrl-D instead of looping forever."
    ln ""
    ln "  A session with the secret set to 37 must look like:"
    ln ""
    ln "      Guess (1-100): 50"
    ln "      Too high."
    ln "      Guess (1-100): 20"
    ln "      Too low."
    ln "      Guess (1-100): 37"
    ln "      Got it."
    ln ""
    ln "  Assemble and link with:"
    ln "    nasm -f elf64 prog.asm -o prog.o && ld prog.o -o prog"
    db 0

m4_summary_hdr: db "SUMMARY", 0
m4_summary:
    ln "   - read is syscall 0 on fd 0, and rax comes back as a count, 0"
    ln "     for EOF, or a negative errno"
    ln "   - read writes no terminator and keeps your newline; asking for"
    ln "     size - 1 leaves room to terminate it safely"
    ln "   - digits are bytes: subtract '0' to get a value, add '0' to get a"
    ln "     character back"
    ln "   - n = n * 10 + digit, stopping on the first byte outside '0'-'9'"
    ln "   - xor rdx, rdx before every div, or a #DE kills the process"
    ln "   - values that must outlive a syscall go in r12-r15, not rcx"
    db 0

m4_solution:
    incbin "04.asm"
    db 0

section .text

lesson_04_input:
    mov rdi, m4_title
    call title

    part m4_p1_hdr, m4_p1
    part m4_p2_hdr, m4_p2
    part m4_p3_hdr, m4_p3
    part m4_p4_hdr, m4_p4
    part m4_p5_hdr, m4_p5

    ; exercise(4)
    mov rdi, m4_exercise
    call print_cstr
    quiz m4_q1_text, m4_q1_answer, m4_q1_why
    quiz m4_q2_text, m4_q2_answer, m4_q2_why
    quiz m4_q3_text, m4_q3_answer, m4_q3_why
    challenge m4_challenge, m4_solution

    ; summary(4)
    summary m4_summary_hdr, m4_summary
    ret

; Tells the linker this object does not need an executable stack.
section .note.GNU-stack noalloc noexec nowrite progbits
