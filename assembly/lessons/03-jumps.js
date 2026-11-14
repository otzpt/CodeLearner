const { HELPER } = require("../course.js");
const LABEL = "(the print_int helper goes here)";

module.exports = {
  id: "jumps",
  title: "Decisions: compare and jump",
  titlePt: "Decisões: comparar e saltar",
  screens: [
    [
      "There is no if. Instead, cmp compares two values, and a jump instruction goes to a label only when the comparison came out a certain way. je jumps if equal, jne if not equal, jl if less, jg if greater, jle and jge with equal included. jmp always jumps:",
      { setup: HELPER, setupLabel: LABEL, code: "global _start\n\nsection .text\n_start:\n    mov rax, 72         ; the score\n    cmp rax, 50\n    jl fail             ; less than 50: go to fail\n    mov rdi, 1          ; passed\n    jmp done\nfail:\n    mov rdi, 0          ; failed\ndone:\n    call print_int\n\n    mov rax, 60\n    xor rdi, rdi\n    syscall" },
    ],
    [
      "Read it as: if rax < 50, go to fail; otherwise carry on, set rdi to 1, and jump over the fail part to done. Without that jmp done, the program would fall straight into fail and set rdi to 0 as well. Instructions always run one after another unless something jumps.",
      { quiz: "After cmp rax, 50 with rax = 50, does jl jump?", choices: ["yes", "no"], answer: 2, why: "50 is not less than 50. jle would jump." },
    ],
    [
      "Labels starting with a dot, like .skip, belong to the last plain label above them, so the same short name can be reused in different routines:",
      { setup: HELPER, setupLabel: LABEL, code: "global _start\n\nsection .text\n_start:\n    mov rax, -7\n    cmp rax, 0\n    jge .positive\n    neg rax             ; make it positive\n.positive:\n    mov rdi, rax\n    call print_int\n\n    mov rax, 60\n    xor rdi, rdi\n    syscall" },
      "neg flips the sign. This prints the distance of -7 from zero.",
      { tip: "cmp then a conditional jump. Remember to jmp over the other branch." },
    ],
    [
      {
        exercise: "jumps",
        task: "Put 64 in rax. Print 2 if it is 80 or more, 1 if it is 50 or more, and 0 otherwise.",
        expected: ["1"],
        hints: ["cmp rax, 80 / jge great, then cmp rax, 50 / jge good, then the 0 case.", "Each branch sets rdi and jumps to a shared done: label that calls print_int."],
        solution: HELPER + "\n\nglobal _start\n\nsection .text\n_start:\n    mov rax, 64\n    cmp rax, 80\n    jge great\n    cmp rax, 50\n    jge good\n    mov rdi, 0\n    jmp done\ngreat:\n    mov rdi, 2\n    jmp done\ngood:\n    mov rdi, 1\ndone:\n    call print_int\n\n    mov rax, 60\n    xor rdi, rdi\n    syscall",
      },
    ],
  ],
  recapPt: [
    "cmp compara; je, jne, jl, jg, jle e jge saltam conforme o resultado.",
    "jmp salta sempre; sem ele as instruções seguem pela ordem.",
    "Etiquetas com ponto (.nome) pertencem à última etiqueta normal.",
  ],
  recap: [
    "cmp compares; je, jne, jl, jg, jle and jge jump depending on the result.",
    "jmp always jumps; otherwise instructions run in order.",
    "Labels starting with a dot (.name) belong to the last plain label.",
  ],
};
