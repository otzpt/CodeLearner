; lessons_bits.asm - modules 7 and 8: bits, flags and arithmetic, and strings.
;
; Like lessons_memory.asm, these run things while the lesson is on screen:
; every number after a label like "x = " is computed by this program as you
; read it. Text screens are labels followed by ln lines (course.inc).

%include "course.inc"

default rel

global lesson_07_bits
global lesson_08_strings

section .rodata

; ---- module 7 -----------------------------------------------------
m7_title:   db "MODULE 7 - BITS, FLAGS AND ARITHMETIC", 0
m7_p1_hdr:  db "PART 1: and, or, xor, not: masks", 0
m7_p2_hdr:  db "PART 2: shifts, and the shift that looks the same", 0
m7_p3_hdr:  db "PART 3: flags: one add, two kinds of overflow", 0
m7_p4_hdr:  db "PART 4: dividing, and widening", 0
m7_p5_hdr:  db "PART 5: packing fields, the way hardware wants them", 0

m7_p1:
    ln "  A register is also 64 separate bits, and four instructions work on"
    ln "  them one position at a time. A MASK is a constant with 1s where you"
    ln "  care and 0s where you do not:"
    ln ""
    ln "    and rax, 0x0F      ; keep only the low four bits, clear the rest"
    ln "    or  rax, 0x01      ; set bit 0, leave the rest alone"
    ln "    xor rax, 0xFF      ; flip the low eight bits"
    ln "    not rax            ; flip all 64"
    ln "    test rax, 0x80     ; is bit 7 set?  and, but only the flags kept"
    ln ""
    ln "  Hardware registers are bit fields, and this is how every one of"
    ln "  them is read and written: read, mask, change, write back. Starting"
    ln "  from the byte 1011 0110 (0xB6), computed by this lesson now:"
    ln ""
    db 0
m7_p1_a:   db "    value          = ", 0
m7_p1_b:   db "    and 0x0F       = ", 0
m7_p1_c:   db "    or  0x01       = ", 0
m7_p1_d:   db "    xor 0xFF       = ", 0
m7_p1_e:   db "    not (low byte) = ", 0
m7_p1_f:
    ln ""
    ln "  xor against 0xFF and not give the same byte here; they differ only"
    ln "  in what happens to the other 56 bits. One more use of xor you have"
    ln "  already met: xor rax, rax is zero in fewer bytes than mov rax, 0."
    db 0

m7_p2:
    ln "  A shift moves every bit sideways and fills the gap:"
    ln ""
    ln "    shl rax, n     ; left,  fill with 0     rax * 2^n"
    ln "    shr rax, n     ; right, fill with 0     rax / 2^n, UNSIGNED"
    ln "    sar rax, n     ; right, copy the sign   rax / 2^n, SIGNED"
    ln ""
    ln "  shr and sar are one letter apart and agree on every positive number."
    ln "  On a negative one they are different instructions. rax holds -16,"
    ln "  every bit above bit 3 set; each result below is computed now:"
    ln ""
    db 0
m7_p2_a:   db "    rax = -16        = ", 0
m7_p2_b:   db "    shr rax, 2       = ", 0
m7_p2_c:   db "    sar rax, 2       = ", 0
m7_p2_d:   db "    1 shl 63         = ", 0
m7_p2_e:
    ln ""
    ln "  sar gives -4, which is -16 / 4. shr gives a huge positive number:"
    ln "  it read the sign bit as the biggest digit. Choose by what the value"
    ln "  MEANS, the same decision as jl against jb in module 2. The last line"
    ln "  is the other edge: shift a 1 into bit 63 and the number is negative"
    ln "  to anything reading it as signed."
    ln ""
    ln "  Also: rol and ror rotate, bits leaving one end enter the other."
    db 0

m7_p3:
    ln "  Most arithmetic leaves a trail in the FLAGS register. Four of the"
    ln "  bits matter for now:"
    ln ""
    ln "    ZF  the result was zero              SF  the result's top bit is 1"
    ln "    CF  an UNSIGNED carry or borrow      OF  a SIGNED overflow"
    ln ""
    ln "  CF and OF are the two meanings of 'it did not fit', and one add"
    ln "  can set either, both or neither. The 8-bit register al, with the"
    ln "  flags read straight after the instruction (pushfq), by this lesson:"
    ln ""
    db 0
m7_p3_a:   db "    255 + 1  ->  al = ", 0
m7_p3_b:   db "    127 + 1  ->  al = ", 0
m7_p3_c:   db "      0 - 1  ->  al = ", 0
m7_p3_d:
    ln ""
    ln "  255 + 1 is 256 as an unsigned number: it carried out of the top, so"
    ln "  CF, and the byte is 0, so ZF. As a signed number it is -1 + 1 = 0,"
    ln "  which is right, so no OF. 127 + 1 is the opposite: fine unsigned,"
    ln "  but as a signed byte it went from the largest positive to the"
    ln "  largest negative, so OF and SF. 0 - 1 borrows, so CF. The CPU sets"
    ln "  both flags every time; reading the right one, with the right jump"
    ln "  (jc/jnc for unsigned, jo for signed), is your decision."
    db 0

m7_p4:
    ln "  Multiplying and dividing have more rules than adding:"
    ln ""
    ln "    mul rcx        rdx:rax = rax * rcx       (unsigned, a 128-bit result)"
    ln "    imul rax, rcx  rax = rax * rcx           (signed, 64 bits kept)"
    ln "    div rcx        rax, rdx = rdx:rax / rcx   (quotient, remainder)"
    ln "    idiv rcx       the same, signed"
    ln ""
    ln "  div and idiv divide the 128-bit pair rdx:rax. For div you zero rdx"
    ln "  (module 4). For idiv you must instead SIGN-extend rax into rdx, which"
    ln "  is what the one-byte instruction cqo does. -7 / 2, right and wrong:"
    ln ""
    db 0
m7_p4_a:   db "    cqo, then idiv       quotient = ", 0
m7_p4_b:   db "                         remainder = ", 0
m7_p4_c:   db "    xor rdx, then idiv   quotient = ", 0
m7_p4_d:
    ln ""
    ln "  Right: -3 remainder -1. idiv truncates toward zero, like C and unlike"
    ln "  Python, which would answer -4 remainder 1. Wrong: with rdx = 0 the"
    ln "  CPU divides the huge positive number 2^64 - 7, and returns a"
    ln "  perfectly confident wrong answer. No fault, no warning."
    ln ""
    ln "  The same split exists when a small value is widened into a big"
    ln "  register. The byte 0x80 widened, both ways, computed now:"
    ln ""
    db 0
m7_p4_e:   db "    movzx rax, al   (zero-extend)  = ", 0
m7_p4_f:   db "    movsx rax, al   (sign-extend)  = ", 0
m7_p4_g:
    ln ""
    ln "  Unsigned data widens with movzx, signed with movsx (movsxd from 32"
    ln "  bits). The wrong one turns -128 into 128 or the reverse."
    db 0

m7_p5:
    ln "  The reason for all of it: the CPU's own tables are bit fields."
    ln "  A segment descriptor in the GDT, which module 13 builds, is 8 bytes"
    ln "  with its pieces scattered:"
    ln ""
    ln "    bits  0-15   limit 15:0          bits 40-47   access byte"
    ln "    bits 16-39   base 23:0           bits 48-51   limit 19:16"
    ln "    bits 56-63   base 31:24          bits 52-55   flags"
    ln ""
    ln "  Assembling one is mask, shift, or, six times over:"
    ln ""
    ln "    mov r8, rdi / and r8, 0xFFFFFF / shl r8, 16 / or rax, r8"
    ln ""
    ln "  That is the first field (base 23:0, moved up to bit 16). A function"
    ln "  that does all six, run now, for the three descriptors the next"
    ln "  modules use (base 0, limit 0xFFFFF):"
    ln ""
    db 0
m7_p5_a:   db "    32-bit code, access 0x9A, flags 0xC  = ", 0
m7_p5_b:   db "    32-bit data, access 0x92, flags 0xC  = ", 0
m7_p5_c:   db "    64-bit code, access 0x9A, flags 0xA  = ", 0
m7_p5_d:
    ln ""
    ln "  The first is the famous 0x00CF9A000000FFFF; you will meet all three"
    ln "  again, as constants, in a boot sector. Writing them by hand as"
    ln "  hex is how most people do it. Knowing which bits those digits are"
    ln "  is how you fix it when the machine resets itself on boot."
    db 0

m7_summary_hdr: db "SUMMARY", 0
m7_summary:
    ln "   - and clears, or sets, xor flips, test only reads: all with a mask"
    ln "   - shl multiplies by 2^n; shr divides unsigned, sar divides signed;"
    ln "     the wrong right shift turns -16 / 4 into a huge number"
    ln "   - CF is an unsigned overflow, OF a signed one; the CPU sets both"
    ln "     and your jump picks which one counts"
    ln "   - idiv needs cqo, div needs xor rdx; idiv truncates toward zero"
    ln "   - movzx widens unsigned data, movsx signed"
    ln "   - hardware tables are bit fields: mask, shift, or to build them"
    db 0

m7_exercise:
    db `\n  >> EXERCISE - MODULE 7\n  ------------------------------------------------------\n`, 0
m7_q1_text:   db `Which instruction shifts right and copies the sign bit into\n  the vacated positions? (three letters)`, 0
m7_q1_answer: db "sar", 0
m7_q1_why:    db `shr fills with zeros, so a negative number turns into a huge\n             positive one. sar keeps the sign.`, 0
m7_q2_text:   db `al holds 255 and you run add al, 1. Which flag shows the\n  unsigned overflow? (two letters)`, 0
m7_q2_answer: db "cf", 0
m7_q2_why:    db `The carry flag is set when an unsigned result does not fit. OF\n             is the signed one, and here it stays clear: 255 is -1 as a signed byte.`, 0
m7_q3_text:   db `Which instruction sign-extends rax into rdx before idiv?\n  (three letters)`, 0
m7_q3_answer: db "cqo", 0
m7_q3_why:    db `cqo copies the sign bit of rax across all of rdx. xor rdx, rdx\n             is for div, and gives idiv a wrong answer on a negative.`, 0

m7_challenge:
    ln ""
    ln "  >> WRITE THIS YOURSELF, in a real file"
    ln ""
    ln "  Count the 1 bits in the byte 0xB6 (1011 0110) with a loop: test"
    ln "  the lowest bit, add it to a counter, shift right, stop when the"
    ln "  value is zero. Exit with the count as the exit status."
    ln ""
    ln "  Run it and ask the shell for the exit status. It must print:"
    ln ""
    ln "      $ ./prog; echo $?"
    ln "      5"
    ln ""
    ln "  Assemble and link with:"
    ln "    nasm -f elf64 prog.asm -o prog.o && ld prog.o -o prog"
    db 0

m7_solution:
    incbin "07.asm"
    db 0

m7_val:      equ 0xB6

; ---- module 8 -----------------------------------------------------
m8_title:   db "MODULE 8 - STRINGS AND BLOCK INSTRUCTIONS", 0
m8_p1_hdr:  db "PART 1: rep movsb is memcpy", 0
m8_p2_hdr:  db "PART 2: stos fills, scas searches", 0
m8_p3_hdr:  db "PART 3: cmps compares", 0
m8_p4_hdr:  db "PART 4: the direction flag, and overlapping copies", 0
m8_p5_hdr:  db "PART 5: writing memcpy, and the register it must return", 0

m8_p1:
    ln "  x86 has a family of instructions that work on a block of memory in"
    ln "  one go. They take their operands from fixed registers, so they have"
    ln "  none of their own:"
    ln ""
    ln "    movs   copy a unit from [rsi] to [rdi]"
    ln "    stos   store al/ax/eax/rax at [rdi]"
    ln "    lods   load al/ax/eax/rax from [rsi]"
    ln "    scas   compare al/ax/eax/rax with [rdi]"
    ln "    cmps   compare [rsi] with [rdi]"
    ln ""
    ln "  Each comes in sizes b, w, d, q (movsb, movsw, movsd, movsq). Each"
    ln "  advances rsi and/or rdi by the size when it finishes. Put a rep"
    ln "  in front and it runs rcx times, counting rcx down to zero. That is"
    ln "  memcpy:"
    ln ""
    ln "    mov rsi, source        ; from"
    ln "    mov rdi, dest          ; to"
    ln "    mov rcx, 18            ; how many bytes"
    ln "    rep movsb"
    ln ""
    ln "  Run now, on 'Hello, rep movsb!' and its 0, into a zeroed buffer:"
    ln ""
    db 0
m8_p1_a:   db "    destination = ", 0
m8_p1_b:
    db 0

m8_p2:
    ln "  stos is memset. al holds the byte, rdi the start, rcx the count:"
    ln ""
    ln "    mov al, '*'  / mov rcx, 10  / rep stosb       ; ten stars"
    ln "    mov rax, 0x2323232323232323  / mov rcx, 2  / rep stosq"
    ln ""
    ln "  stosq stores eight bytes per round, so two rounds fill sixteen."
    ln "  scas compares al with the byte at rdi, which makes a search. The"
    ln "  classic is strlen: look for the 0 byte, with no limit, and read the"
    ln "  length off the counter:"
    ln ""
    ln "    xor al, al             ; the byte we want: 0"
    ln "    mov rcx, -1            ; effectively unlimited"
    ln "    repne scasb            ; repeat while NOT equal"
    ln "    not rcx                ; rcx counted down from -1"
    ln "    dec rcx                ; do not count the 0 itself"
    ln ""
    ln "  Run now:"
    ln ""
    db 0
m8_p2_a:   db "    rep stosb of '*'          : ", 0
m8_p2_b:   db "    rep stosq of eight '#'    : ", 0
m8_p2_c:   db "    repne scasb on 'assembly' : length = ", 0
m8_p2_d:
    ln ""
    ln "  repne means repeat while not equal; repe, repeat while equal. rcx"
    ln "  runs out first if the byte is never found, which is why a real"
    ln "  strlen of a string with no 0 reads on through memory."
    db 0

m8_p3:
    ln "  cmpsb compares [rsi] with [rdi] and sets the flags like cmp does."
    ln "  With repe it keeps going while the bytes are equal, and stops at"
    ln "  the first difference or when rcx runs out, so ZF afterwards tells"
    ln "  you which:"
    ln ""
    ln "    mov rsi, a / mov rdi, b / mov rcx, 7        ; 'kernel' and its 0"
    ln "    repe cmpsb"
    ln "    jne .different         ; stopped on a mismatch"
    ln ""
    ln "  On a mismatch rsi and rdi have already stepped PAST the differing"
    ln "  byte, so its index is rsi - a - 1. Comparing 'kernel' with 'kernal'"
    ln "  and with 'kernel', run now:"
    ln ""
    db 0
m8_p3_a:   db "    kernel vs kernal : first difference at index ", 0
m8_p3_b:   db "    kernel vs kernel : ", 0
m8_p3_equal: db "equal", 10, 0
m8_p3_c:
    ln ""
    ln "  The off-by-one is the trap: read rsi right after the repe and you"
    ln "  are one byte past where the strings parted."
    db 0

m8_p4:
    ln "  Which way do rsi and rdi move? The DIRECTION FLAG decides. DF = 0"
    ln "  (cld): forward, upward. DF = 1 (std): backward. It exists for one"
    ln "  reason: copying a block onto an overlapping part of itself."
    ln ""
    ln "  The buffer holds 'abcdefgh' and six bytes are copied from the start"
    ln "  to two places on. Going forward overwrites bytes before they are"
    ln "  read. Going backward from the end does not:"
    ln ""
    ln "    cld / rsi = buf    / rdi = buf+2 / rcx = 6 / rep movsb"
    ln "    std / rsi = buf+5  / rdi = buf+7 / rcx = 6 / rep movsb  (then cld)"
    ln ""
    ln "  Both run now, on the same starting buffer:"
    ln ""
    db 0
m8_p4_a:   db "    forward  (cld) : ", 0
m8_p4_b:   db "    backward (std) : ", 0
m8_p4_c:
    ln ""
    ln "  The forward copy is the wrong answer: abababab, the first two bytes"
    ln "  repeated. memmove is this decision made for you. The System V ABI"
    ln "  says DF must be 0 whenever a function is entered or left, so a std"
    ln "  has to be undone by a cld before anything else runs. Leave it set"
    ln "  and every later rep movsb, in your code or in libc, walks backward."
    db 0

m8_p5:
    ln "  memcpy returns its destination, which is rdi as it was when the"
    ln "  function started. But rep movsb advances rdi, so by the end rdi is"
    ln "  one past the last byte copied. The function has to save it first:"
    ln ""
    ln "    bad:   mov rcx, rdx / rep movsb / mov rax, rdi   ; the END"
    ln "    good:  mov rax, rdi / mov rcx, rdx / rep movsb   ; the start"
    ln ""
    ln "  Called with a five-byte copy, how far the returned pointer is from"
    ln "  the destination, run now:"
    ln ""
    db 0
m8_p5_a:   db "    bad  : returned - dest = ", 0
m8_p5_b:   db "    good : returned - dest = ", 0
m8_p5_c:
    ln ""
    ln "  Both copy the bytes correctly, which is why this survives testing"
    ln "  until a caller uses the return value, as in p = memcpy(buf, src,"
    ln "  n). A kernel has no libc, so memcpy, memset and memcmp are yours"
    ln "  to write, out of exactly these instructions; module 9 shows the"
    ln "  compiler asking for them in code that never mentions them."
    db 0

m8_summary_hdr: db "SUMMARY", 0
m8_summary:
    ln "   - movs/stos/lods/scas/cmps take rsi, rdi, al/rax and rcx as fixed"
    ln "     operands; rep repeats rcx times, repe/repne also watch ZF"
    ln "   - rep movsb is memcpy, rep stosb is memset, repne scasb is strlen"
    ln "   - after a repe cmpsb mismatch rsi/rdi are one past the difference"
    ln "   - DF picks the direction; std for an overlapping copy to a higher"
    ln "     address, and always cld before anything else runs"
    ln "   - rep movsb advances rdi: a memcpy that returns dest saves it first"
    db 0

m8_exercise:
    db `\n  >> EXERCISE - MODULE 8\n  ------------------------------------------------------\n`, 0
m8_q1_text:   db `Which register holds the repeat count for rep?\n  (three letters)`, 0
m8_q1_answer: db "rcx", 0
m8_q1_why:    db `rep runs the instruction rcx times, decrementing rcx each round.\n             rcx = 0 on entry means it does nothing.`, 0
m8_q2_text:   db `Which flag makes movsb walk backward through memory?\n  (two letters)`, 0
m8_q2_answer: db "df", 0
m8_q2_why:    db `The direction flag: std sets it, cld clears it. It must be clear\n             whenever a function is entered or left.`, 0
m8_q3_text:   db `In rep movsb, which register is the destination?\n  (three letters)`, 0
m8_q3_answer: db "rdi", 0
m8_q3_why:    db `rsi is the source and rdi the destination: 's' for source, 'd'\n             for destination, the same letters as the registers' names.`, 0

m8_challenge:
    ln ""
    ln "  >> WRITE THIS YOURSELF, in a real file"
    ln ""
    ln "  Find the length of the string 'operating system' (put it in"
    ln "  .rodata with a 0 after it) using repne scasb, and exit with the"
    ln "  length as the exit status. No loop of your own: one rep"
    ln "  instruction does the scanning."
    ln ""
    ln "  Run it and ask the shell for the exit status. It must print:"
    ln ""
    ln "      $ ./prog; echo $?"
    ln "      16"
    ln ""
    ln "  Assemble and link with:"
    ln "    nasm -f elf64 prog.asm -o prog.o && ld prog.o -o prog"
    db 0

m8_solution:
    incbin "08.asm"
    db 0

m8_hello:    db "Hello, rep movsb!", 0
m8_assembly: db "assembly", 0
m8_kernel:   db "kernel", 0
m8_kernal:   db "kernal", 0
m8_equal:    db "equal", 10, 0
m8_nl:       db 10, 0

section .data
; The flag line is edited in place by m7_report: the four digits after the
; equals signs are overwritten with 0 or 1.
m7_flagline: db "CF=0 ZF=0 SF=0 OF=0", 10, 0
m7_flagbits: db 0, 6, 7, 11          ; CF, ZF, SF, OF: the bit in rflags
m7_flagpos:  db 3, 8, 13, 18         ; where its digit sits in the line
m8_overlap:  db "abcdefgh", 0

section .bss
m8_dst:      resb 32

section .data

section .text

; m7_report(rdi=label, rsi=byte, rdx=rflags) -> label, the byte in hex, then
; the four flags. Prints "<label>0x..  CF=0 ZF=1 SF=0 OF=0".
m7_report:
    push rbp
    mov rbp, rsp
    push rbx
    push r12
    mov rbx, rsi
    mov r12, rdx
    call print_cstr
    mov rdi, rbx
    mov rsi, 2
    call print_hex_fixed
    xor r8, r8                  ; which flag: 0 to 3
.flag:
    movzx ecx, byte [m7_flagbits + r8]
    mov rdx, r12
    shr rdx, cl                 ; the flag's bit is now bit 0
    and edx, 1
    add dl, '0'
    movzx eax, byte [m7_flagpos + r8]
    mov [m7_flagline + rax], dl
    inc r8
    cmp r8, 4
    jb .flag
    mov rdi, m7_two_spaces
    call print_cstr
    mov rdi, m7_flagline
    call print_cstr
    pop r12
    pop rbx
    pop rbp
    ret

; m7_descriptor(base=rdi, limit=rsi, access=rdx, flags=rcx) -> rax, the
; 8-byte segment descriptor those fields make.
m7_descriptor:
    mov rax, rsi
    and rax, 0xFFFF             ; limit 15:0 stays at bit 0
    mov r8, rdi
    and r8, 0xFFFFFF
    shl r8, 16                  ; base 23:0 at bit 16
    or rax, r8
    mov r8, rdx
    and r8, 0xFF
    shl r8, 40                  ; access byte at bit 40
    or rax, r8
    mov r8, rsi
    shr r8, 16
    and r8, 0xF
    shl r8, 48                  ; limit 19:16 at bit 48
    or rax, r8
    mov r8, rcx
    and r8, 0xF
    shl r8, 52                  ; flags at bit 52
    or rax, r8
    mov r8, rdi
    shr r8, 24
    and r8, 0xFF
    shl r8, 56                  ; base 31:24 at bit 56
    or rax, r8
    ret

section .rodata
m7_two_spaces: db "  ", 0
section .text

lesson_07_bits:
    push rbx
    push r12
    sub rsp, 8                  ; two pushes left rsp 8 off a multiple of 16
    mov rdi, m7_title
    call title

    part_begin m7_p1_hdr, m7_p1
    mov rbx, m7_val
    live_bin m7_p1_a, rbx, 8
    mov rax, rbx
    and rax, 0x0F
    live_bin m7_p1_b, rax, 8
    mov rax, rbx
    or rax, 0x01
    live_bin m7_p1_c, rax, 8
    mov rax, rbx
    xor rax, 0xFF
    live_bin m7_p1_d, rax, 8
    mov rax, rbx
    not rax
    live_bin m7_p1_e, rax, 8
    mov rdi, m7_p1_f
    call print_cstr
    part_end

    part_begin m7_p2_hdr, m7_p2
    mov rbx, -16
    live_hexw m7_p2_a, rbx, 16
    mov rax, rbx
    shr rax, 2
    live_hexw m7_p2_b, rax, 16
    mov rax, rbx
    sar rax, 2
    live_hexw m7_p2_c, rax, 16
    mov rax, 1
    shl rax, 63
    live_hexw m7_p2_d, rax, 16
    mov rdi, m7_p2_e
    call print_cstr
    part_end

    part_begin m7_p3_hdr, m7_p3
    mov al, 255
    add al, 1
    pushfq
    pop rdx
    movzx esi, al
    mov rdi, m7_p3_a
    call m7_report
    mov al, 127
    add al, 1
    pushfq
    pop rdx
    movzx esi, al
    mov rdi, m7_p3_b
    call m7_report
    mov al, 0
    sub al, 1
    pushfq
    pop rdx
    movzx esi, al
    mov rdi, m7_p3_c
    call m7_report
    mov rdi, m7_p3_d
    call print_cstr
    part_end

    part_begin m7_p4_hdr, m7_p4
    mov rax, -7
    cqo
    mov rcx, 2
    idiv rcx
    mov rbx, rax
    mov r12, rdx
    live_hexw m7_p4_a, rbx, 16
    live_hexw m7_p4_b, r12, 16
    mov rax, -7
    xor edx, edx
    mov rcx, 2
    idiv rcx
    live_hexw m7_p4_c, rax, 16
    mov rdi, m7_p4_d
    call print_cstr
    mov al, 0x80
    movzx rbx, al
    movsx r12, al
    live_hexw m7_p4_e, rbx, 16
    live_hexw m7_p4_f, r12, 16
    mov rdi, m7_p4_g
    call print_cstr
    part_end

    part_begin m7_p5_hdr, m7_p5
    mov rdi, 0
    mov rsi, 0xFFFFF
    mov rdx, 0x9A
    mov rcx, 0xC
    call m7_descriptor
    live_hexw m7_p5_a, rax, 16
    mov rdi, 0
    mov rsi, 0xFFFFF
    mov rdx, 0x92
    mov rcx, 0xC
    call m7_descriptor
    live_hexw m7_p5_b, rax, 16
    mov rdi, 0
    mov rsi, 0xFFFFF
    mov rdx, 0x9A
    mov rcx, 0xA
    call m7_descriptor
    live_hexw m7_p5_c, rax, 16
    mov rdi, m7_p5_d
    call print_cstr
    part_end

    ; exercise(7)
    mov rdi, m7_exercise
    call print_cstr
    quiz m7_q1_text, m7_q1_answer, m7_q1_why
    quiz m7_q2_text, m7_q2_answer, m7_q2_why
    quiz m7_q3_text, m7_q3_answer, m7_q3_why
    challenge m7_challenge, m7_solution

    ; summary(7)
    summary m7_summary_hdr, m7_summary
    add rsp, 8
    pop r12
    pop rbx
    ret

; ---- module 8 code --------------------------------------------------

; The two memcpys of PART 5: copy rdx bytes from rsi to rdi.
m8_copy_bad:
    mov rcx, rdx
    rep movsb
    mov rax, rdi                ; BUG: rdi has moved to the end of the copy
    ret

m8_copy_good:
    mov rax, rdi                ; save the destination before it moves
    mov rcx, rdx
    rep movsb
    ret

; m8_zero_dst() clears the first 32 bytes of m8_dst.
m8_zero_dst:
    mov rdi, m8_dst
    xor eax, eax
    mov rcx, 32
    rep stosb
    ret

lesson_08_strings:
    push rbx
    push r12
    sub rsp, 8                  ; two pushes left rsp 8 off a multiple of 16
    cld                         ; the ABI says it is clear; make it so
    mov rdi, m8_title
    call title

    part_begin m8_p1_hdr, m8_p1
    call m8_zero_dst
    mov rsi, m8_hello
    mov rdi, m8_dst
    mov rcx, 18
    rep movsb
    mov rdi, m8_p1_a
    call print_cstr
    mov rdi, m8_dst
    call print_cstr
    call print_nl
    mov rdi, m8_p1_b
    call print_cstr
    part_end

    part_begin m8_p2_hdr, m8_p2
    call m8_zero_dst
    mov rdi, m8_dst
    mov al, '*'
    mov rcx, 10
    rep stosb
    mov rdi, m8_p2_a
    call print_cstr
    mov rdi, m8_dst
    call print_cstr
    call print_nl
    call m8_zero_dst
    mov rdi, m8_dst
    mov rax, 0x2323232323232323
    mov rcx, 2
    rep stosq
    mov rdi, m8_p2_b
    call print_cstr
    mov rdi, m8_dst
    call print_cstr
    call print_nl
    mov rdi, m8_assembly
    xor al, al
    mov rcx, -1
    repne scasb
    not rcx
    dec rcx
    live_dec m8_p2_c, rcx
    mov rdi, m8_p2_d
    call print_cstr
    part_end

    part_begin m8_p3_hdr, m8_p3
    mov rsi, m8_kernel
    mov rdi, m8_kernal
    mov rcx, 7
    repe cmpsb
    jne .differ
    mov rdi, m8_p3_a
    call print_cstr
    jmp .second
.differ:
    mov rbx, rsi
    sub rbx, m8_kernel
    dec rbx                     ; rsi is one past the byte that differed
    live_dec m8_p3_a, rbx
.second:
    mov rsi, m8_kernel
    mov rdi, m8_kernel
    mov rcx, 7
    repe cmpsb
    mov rdi, m8_p3_b
    call print_cstr
    mov rdi, m8_equal
    call print_cstr
    mov rdi, m8_p3_c
    call print_cstr
    part_end

    part_begin m8_p4_hdr, m8_p4
    mov rsi, m8_overlap
    mov rdi, m8_overlap + 2
    mov rcx, 6
    cld
    rep movsb
    mov rdi, m8_p4_a
    call print_cstr
    mov rdi, m8_overlap
    call print_cstr
    call print_nl
    mov rax, "abcdefgh"         ; put the original eight bytes back
    mov [m8_overlap], rax
    mov rsi, m8_overlap + 5
    mov rdi, m8_overlap + 7
    mov rcx, 6
    std
    rep movsb
    cld                         ; always put it back
    mov rdi, m8_p4_b
    call print_cstr
    mov rdi, m8_overlap
    call print_cstr
    call print_nl
    mov rdi, m8_p4_c
    call print_cstr
    part_end

    part_begin m8_p5_hdr, m8_p5
    mov rdi, m8_dst
    mov rsi, m8_hello
    mov rdx, 5
    call m8_copy_bad
    sub rax, m8_dst
    mov rbx, rax
    mov rdi, m8_dst
    mov rsi, m8_hello
    mov rdx, 5
    call m8_copy_good
    sub rax, m8_dst
    mov r12, rax
    live_dec m8_p5_a, rbx
    live_dec m8_p5_b, r12
    mov rdi, m8_p5_c
    call print_cstr
    part_end

    ; exercise(8)
    mov rdi, m8_exercise
    call print_cstr
    quiz m8_q1_text, m8_q1_answer, m8_q1_why
    quiz m8_q2_text, m8_q2_answer, m8_q2_why
    quiz m8_q3_text, m8_q3_answer, m8_q3_why
    challenge m8_challenge, m8_solution

    ; summary(8)
    summary m8_summary_hdr, m8_summary
    add rsp, 8
    pop r12
    pop rbx
    ret

; Tells the linker this object does not need an executable stack.
section .note.GNU-stack noalloc noexec nowrite progbits
