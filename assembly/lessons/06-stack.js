const { HELPER } = require("../course.js");
const LABEL = "(the print_int helper goes here)";

module.exports = {
  id: "stack",
  title: "The stack: push and pop",
  titlePt: "A pilha: push e pop",
  screens: [
    [
      "The stack is memory used like a pile of plates. push puts a value on top; pop takes the top one off. The last one pushed is the first one popped:",
      { setup: HELPER, setupLabel: LABEL, code: "global _start\n\nsection .text\n_start:\n    push 1\n    push 2\n    push 3\n    pop rdi             ; 3, the last pushed\n    call print_int\n    pop rdi             ; then 2\n    call print_int\n    pop rdi             ; then 1\n    call print_int\n\n    mov rax, 60\n    xor rdi, rdi\n    syscall" },
      "Register rsp always holds the address of the top of the stack. push moves it down 8 bytes and writes; pop reads and moves it back up.",
    ],
    [
      "The usual use: save a register before code that would change it, and get it back afterwards. Pushes and pops must pair up, in reverse order:",
      { setup: HELPER, setupLabel: LABEL, code: "global _start\n\nsection .text\n_start:\n    mov rcx, 7\n    push rcx            ; keep 7 safe\n    mov rdi, 100\n    call print_int      ; this changes rcx\n    pop rcx             ; 7 is back\n    mov rdi, rcx\n    call print_int\n\n    mov rax, 60\n    xor rdi, rdi\n    syscall" },
      { quiz: "push 5, push 6, then pop rax. What is in rax?", choices: ["5", "6"], answer: 2, why: "The last value pushed comes off first." },
    ],
    [
      { tip: "Every push needs a matching pop, in reverse order, or the stack is left out of step." },
      {
        exercise: "stack",
        task: "Push the numbers 10, 20 and 30, then pop and print them, so they come out in the order 30, 20, 10.",
        expected: ["30", "20", "10"],
        hints: ["push 10 / push 20 / push 30", "Three times: pop rdi / call print_int."],
        solution: HELPER + "\n\nglobal _start\n\nsection .text\n_start:\n    push 10\n    push 20\n    push 30\n    pop rdi\n    call print_int\n    pop rdi\n    call print_int\n    pop rdi\n    call print_int\n\n    mov rax, 60\n    xor rdi, rdi\n    syscall",
      },
    ],
  ],
  recapPt: [
    "A pilha é como uma pilha de pratos: o último a entrar é o primeiro a sair.",
    "push põe no topo, pop tira do topo; rsp aponta para o topo.",
    "Usa push/pop para guardar registos à volta de código que os muda.",
  ],
  recap: [
    "The stack is like a pile of plates: last in, first out.",
    "push puts on top, pop takes off the top; rsp points at the top.",
    "Use push/pop to keep registers safe around code that changes them.",
  ],
};
