const { HELPER } = require("../course.js");
const LABEL = "(the print_int helper goes here)";

module.exports = {
  id: "memory",
  title: "Memory: variables and arrays",
  titlePt: "Memória: variáveis e arrays",
  screens: [
    [
      "There are only 16 general registers, so most data lives in memory. A label names an address; square brackets mean \"the value at that address\". dq reserves 8-byte (64-bit) values:",
      { setup: HELPER, setupLabel: LABEL, code: "global _start\n\nsection .data\nscore: dq 15\n\nsection .text\n_start:\n    mov rax, [score]        ; read the value\n    add rax, 10\n    mov [score], rax        ; write it back\n    mov rdi, [score]\n    call print_int\n\n    mov rax, 60\n    xor rdi, rdi\n    syscall" },
      "mov rax, score (no brackets) would put the address itself into rax, not the value stored there.",
    ],
    [
      "Several values in a row make an array. Each dq value is 8 bytes, so element i is at address + i * 8, written [prices + rbx * 8]:",
      { setup: HELPER, setupLabel: LABEL, code: "global _start\n\nsection .data\nprices: dq 2, 15, 30, 7\n\nsection .text\n_start:\n    xor r12, r12            ; total\n    xor rbx, rbx            ; position 0\nnext:\n    add r12, [prices + rbx * 8]\n    inc rbx\n    cmp rbx, 4\n    jl next\n    mov rdi, r12\n    call print_int\n\n    mov rax, 60\n    xor rdi, rdi\n    syscall" },
      "Positions start at 0, like everywhere. Nothing checks that rbx stays below 4: reading past the end quietly reads whatever comes next.",
    ],
    [
      "Sizes matter. db is 1 byte, dw 2, dd 4, dq 8. When NASM cannot tell the size from a register, say it: byte, word, dword or qword:",
      { setup: HELPER, setupLabel: LABEL, code: "global _start\n\nsection .data\nsmall: db 200\n\nsection .text\n_start:\n    add byte [small], 100   ; 200 + 100 in one byte\n    movzx rdi, byte [small] ; widen to 64 bits\n    call print_int\n\n    mov rax, 60\n    xor rdi, rdi\n    syscall" },
      "One byte holds 0 to 255, so 300 wraps around to 44. movzx copies a small value into a big register, filling the rest with zeros.",
      { quiz: "With values: dq 10, 20, 30, what is [values + 2 * 8]?", choices: ["20", "30"], answer: 2, why: "Position 2 is the third value." },
    ],
    [
      {
        exercise: "memory",
        task: "With numbers: dq 4, 8, 15, 16, 23, 42 in section .data, print the largest value and then the total.",
        expected: ["42", "108"],
        hints: ["Start largest (r13) at [numbers]; loop rbx from 0 to 5; cmp the element with r13 and replace when bigger.", "Load the element into a register first (mov rax, [numbers + rbx * 8]), then compare and add."],
        solution: HELPER + "\n\nglobal _start\n\nsection .data\nnumbers: dq 4, 8, 15, 16, 23, 42\n\nsection .text\n_start:\n    mov r13, [numbers]\n    xor r12, r12\n    xor rbx, rbx\nnext:\n    mov rax, [numbers + rbx * 8]\n    add r12, rax\n    cmp rax, r13\n    jle .smaller\n    mov r13, rax\n.smaller:\n    inc rbx\n    cmp rbx, 6\n    jl next\n    mov rdi, r13\n    call print_int\n    mov rdi, r12\n    call print_int\n\n    mov rax, 60\n    xor rdi, rdi\n    syscall",
      },
    ],
  ],
  recapPt: [
    "[etiqueta] é o valor nesse endereço; sem parênteses retos é o próprio endereço.",
    "db 1 byte, dw 2, dd 4, dq 8. Diz o tamanho com byte/word/dword/qword quando é preciso.",
    "O elemento i de um array dq está em [array + i * 8].",
    "Nada verifica os limites: ler para lá do fim lê o que vier a seguir.",
  ],
  recap: [
    "[label] is the value at that address; without brackets it is the address.",
    "db 1 byte, dw 2, dd 4, dq 8. State the size with byte/word/dword/qword when needed.",
    "Element i of a dq array is at [array + i * 8].",
    "Nothing checks bounds: reading past the end reads whatever comes next.",
  ],
};
