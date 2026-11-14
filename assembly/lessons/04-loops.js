const { HELPER } = require("../course.js");
const LABEL = "(the print_int helper goes here)";

module.exports = {
  id: "loops",
  title: "Loops",
  titlePt: "Ciclos",
  screens: [
    [
      "A loop is a jump backwards. Here a counter goes from 1 to 4 and is added to a total each time:",
      { setup: HELPER, setupLabel: LABEL, code: "global _start\n\nsection .text\n_start:\n    xor rbx, rbx        ; total = 0\n    mov rcx, 1          ; counter = 1\nnext:\n    add rbx, rcx\n    inc rcx\n    cmp rcx, 4\n    jle next            ; repeat while counter <= 4\n    mov rdi, rbx\n    call print_int\n\n    mov rax, 60\n    xor rdi, rdi\n    syscall" },
      "jle next sends the processor back to next: while the counter is 4 or less.",
    ],
    [
      "Calling a routine inside a loop needs care: the routine may change registers. print_int changes rax, rcx, rdx, rsi, rdi, r8 and r11, but leaves rbx and r12 to r15 alone. So keep loop counters in those:",
      { setup: HELPER, setupLabel: LABEL, code: "global _start\n\nsection .text\n_start:\n    mov rbx, 3          ; count down from 3\nagain:\n    mov rdi, rbx\n    call print_int\n    dec rbx\n    jnz again           ; jump while rbx is not zero\n\n    mov rax, 60\n    xor rdi, rdi\n    syscall" },
      "dec sets the zero flag when the result is 0, so jnz can test it without a cmp.",
      { quiz: "Why use rbx, not rcx, as the counter around call print_int?", choices: ["rbx is faster", "print_int may change rcx, but keeps rbx"], answer: 2, why: "Routines are expected to keep rbx and r12-r15; rcx may be overwritten." },
    ],
    [
      { tip: "A loop is a cmp and a jump back. Keep values you need across a call in rbx or r12 to r15." },
      {
        exercise: "loops",
        task: "Print the numbers 1 to 5, one per line, then the sum of 1 to 100.",
        expected: ["1", "2", "3", "4", "5", "5050"],
        hints: ["A loop with rbx going 1 to 5: mov rdi, rbx / call print_int / inc rbx / cmp rbx, 5 / jle ...", "A second loop adding r12 into r13 for 1 to 100, then print r13."],
        solution: HELPER + "\n\nglobal _start\n\nsection .text\n_start:\n    mov rbx, 1\nshow:\n    mov rdi, rbx\n    call print_int\n    inc rbx\n    cmp rbx, 5\n    jle show\n\n    xor r13, r13\n    mov r12, 1\nsum:\n    add r13, r12\n    inc r12\n    cmp r12, 100\n    jle sum\n    mov rdi, r13\n    call print_int\n\n    mov rax, 60\n    xor rdi, rdi\n    syscall",
      },
    ],
  ],
  recapPt: [
    "Um ciclo é um salto para trás com uma comparação.",
    "dec e jnz contam até zero sem cmp.",
    "Uma rotina pode mudar rax, rcx, rdx, rsi, rdi, r8-r11; rbx e r12-r15 ficam.",
  ],
  recap: [
    "A loop is a jump backwards with a comparison.",
    "dec with jnz counts down to zero without a cmp.",
    "A routine may change rax, rcx, rdx, rsi, rdi, r8-r11; rbx and r12-r15 are kept.",
  ],
};
