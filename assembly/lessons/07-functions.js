const { HELPER } = require("../course.js");
const LABEL = "(the print_int helper goes here)";

module.exports = {
  id: "functions",
  title: "Functions: call and ret",
  titlePt: "Funções: call e ret",
  screens: [
    [
      "A function is a label you call. call pushes the address of the next instruction and jumps; ret pops that address and jumps back. print_int has been one all along. By custom, the first argument goes in rdi, the second in rsi, and the answer comes back in rax:",
      { setup: HELPER, setupLabel: LABEL, code: "global _start\n\nsection .text\narea:                   ; rax = rdi * rsi\n    mov rax, rdi\n    imul rax, rsi\n    ret\n\n_start:\n    mov rdi, 3\n    mov rsi, 4\n    call area\n    mov rdi, rax\n    call print_int\n\n    mov rax, 60\n    xor rdi, rdi\n    syscall" },
      "This custom, called the calling convention, is the same one C uses on Linux, which is how assembly and C can call each other.",
    ],
    [
      "Because ret takes its address from the top of the stack, a function must leave the stack exactly as it found it. A push without a pop makes ret jump to a wrong address:",
      { setup: HELPER, setupLabel: LABEL, code: "global _start\n\nsection .text\nbroken:\n    push rbx\n    ret                 ; pops rbx's value, not the return address\n\n_start:\n    xor rbx, rbx\n    call broken\n\n    mov rax, 60\n    xor rdi, rdi\n    syscall", error: true },
      { quiz: "Where does a function leave its answer, by the usual custom?", choices: ["rdi", "rax", "the stack"], answer: 2, why: "Arguments go in rdi, rsi, ...; the result comes back in rax." },
    ],
    [
      { tip: "Arguments in rdi, rsi, rdx, rcx; answer in rax; the stack back as it was before ret." },
      {
        exercise: "functions",
        task: "Write a function largest that returns in rax the bigger of rdi and rsi. Use it to print the larger of 3 and 9, then of 12 and 5.",
        expected: ["9", "12"],
        hints: ["largest: mov rax, rdi / cmp rsi, rax / jle .done / mov rax, rsi / .done: ret", "mov rdi, 3 / mov rsi, 9 / call largest / mov rdi, rax / call print_int"],
        solution: HELPER + "\n\nglobal _start\n\nsection .text\nlargest:\n    mov rax, rdi\n    cmp rsi, rax\n    jle .done\n    mov rax, rsi\n.done:\n    ret\n\n_start:\n    mov rdi, 3\n    mov rsi, 9\n    call largest\n    mov rdi, rax\n    call print_int\n    mov rdi, 12\n    mov rsi, 5\n    call largest\n    mov rdi, rax\n    call print_int\n\n    mov rax, 60\n    xor rdi, rdi\n    syscall",
      },
    ],
  ],
  recapPt: [
    "call guarda o endereço de regresso na pilha e salta; ret volta.",
    "Argumentos em rdi, rsi, rdx, rcx; resultado em rax (a mesma convenção do C).",
    "Uma função tem de deixar a pilha como a encontrou antes do ret.",
  ],
  recap: [
    "call saves the return address on the stack and jumps; ret goes back.",
    "Arguments in rdi, rsi, rdx, rcx; result in rax (the same convention as C).",
    "A function must leave the stack as it found it before ret.",
  ],
};
