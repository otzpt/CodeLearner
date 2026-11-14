const { HELPER } = require("../course.js");
const LABEL = "(the print_int helper goes here)";

module.exports = {
  id: "arithmetic",
  title: "Registers and arithmetic",
  titlePt: "Registos e contas",
  screens: [
    [
      "Showing a number means turning it into digit characters first, which is a small program of its own. From here on, a ready-made routine called print_int sits above each example. Put a number in rdi and call print_int to print it:",
      { setup: HELPER, setupLabel: LABEL, code: "global _start\n\nsection .text\n_start:\n    mov rdi, 42\n    call print_int\n\n    mov rax, 60\n    xor rdi, rdi\n    syscall" },
      "xor rdi, rdi sets rdi to 0; it is the usual short way to write mov rdi, 0.",
    ],
    [
      "add and sub change the first register using the second. inc adds 1, dec takes 1 away:",
      { setup: HELPER, setupLabel: LABEL, code: "global _start\n\nsection .text\n_start:\n    mov rax, 10\n    add rax, 5          ; rax = 15\n    sub rax, 3          ; rax = 12\n    inc rax             ; rax = 13\n    mov rdi, rax\n    call print_int\n\n    mov rax, 60\n    xor rdi, rdi\n    syscall" },
      "General registers to use: rax, rbx, rcx, rdx, rsi, rdi, r8 to r15. Each holds 64 bits.",
    ],
    [
      "imul multiplies. Division is fussier: div divides the number in rdx and rax together by its operand, puts the result in rax and the remainder in rdx. So set rdx to 0 first:",
      { setup: HELPER, setupLabel: LABEL, code: "global _start\n\nsection .text\n_start:\n    mov rax, 6\n    imul rax, 7         ; rax = 42\n    mov rdi, rax\n    call print_int\n\n    mov rax, 17\n    xor rdx, rdx\n    mov rbx, 5\n    div rbx             ; rax = 3, rdx = 2\n    mov rdi, rdx\n    call print_int\n\n    mov rax, 60\n    xor rdi, rdi\n    syscall" },
      { quiz: "After div rbx, where is the remainder?", choices: ["rax", "rdx", "rbx"], answer: 2, why: "div puts the result in rax and the remainder in rdx." },
    ],
    [
      { tip: "Results land in the first operand. Clear rdx before div." },
      {
        exercise: "arithmetic",
        task: "Print (8 + 4) * 5, then 100 divided by 7 (whole number), then the remainder of 100 divided by 7.",
        expected: ["60", "14", "2"],
        hints: ["mov rax, 8 / add rax, 4 / imul rax, 5 / mov rdi, rax / call print_int", "For the division: mov rax, 100 / xor rdx, rdx / mov rbx, 7 / div rbx. Save rdx before printing rax, since print_int changes registers."],
        solution: HELPER + "\n\nglobal _start\n\nsection .text\n_start:\n    mov rax, 8\n    add rax, 4\n    imul rax, 5\n    mov rdi, rax\n    call print_int\n\n    mov rax, 100\n    xor rdx, rdx\n    mov rbx, 7\n    div rbx\n    mov r12, rdx\n    mov rdi, rax\n    call print_int\n    mov rdi, r12\n    call print_int\n\n    mov rax, 60\n    xor rdi, rdi\n    syscall",
      },
    ],
  ],
  recapPt: [
    "Põe um número em rdi e faz call print_int para o mostrar.",
    "add, sub, inc, dec e imul mudam o primeiro operando.",
    "div divide rdx:rax: resultado em rax, resto em rdx; põe rdx a 0 antes.",
    "xor rdi, rdi é a forma curta de pôr um registo a 0.",
  ],
  recap: [
    "Put a number in rdi and call print_int to show it.",
    "add, sub, inc, dec and imul change the first operand.",
    "div divides rdx:rax: result in rax, remainder in rdx; zero rdx first.",
    "xor rdi, rdi is the short way to set a register to 0.",
  ],
};
