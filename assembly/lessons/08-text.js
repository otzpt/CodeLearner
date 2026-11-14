const { HELPER } = require("../course.js");
const LABEL = "(the print_int helper goes here)";

module.exports = {
  id: "text",
  title: "Text, one byte at a time",
  titlePt: "Texto, um byte de cada vez",
  screens: [
    [
      "Text is bytes, one per character. A common custom is to end it with a 0 byte, so code can find where it stops. Counting the bytes before the 0 gives its length:",
      { setup: HELPER, setupLabel: LABEL, code: 'global _start\n\nsection .data\ngreeting: db "assembly", 0\n\nsection .text\n_start:\n    xor rbx, rbx            ; length = 0\ncount:\n    cmp byte [greeting + rbx], 0\n    je found\n    inc rbx\n    jmp count\nfound:\n    mov rdi, rbx\n    call print_int\n\n    mov rax, 60\n    xor rdi, rdi\n    syscall' },
    ],
    [
      "A character is just a number: \'a\' is 97, \'A\' is 65. Lower-case letters are 32 above their capitals, so subtracting 32 turns one into the other:",
      { code: 'global _start\n\nsection .data\ngreeting: db "hello", 10\n\nsection .text\n_start:\n    xor rbx, rbx\nnext:\n    sub byte [greeting + rbx], 32   ; to capitals\n    inc rbx\n    cmp rbx, 5\n    jl next\n\n    mov rax, 1\n    mov rdi, 1\n    mov rsi, greeting\n    mov rdx, 6\n    syscall\n\n    mov rax, 60\n    xor rdi, rdi\n    syscall' },
      "The new line (10) after the five letters is left alone, because the loop stops at 5.",
      { quiz: "What number is the character 'B'?", choices: ["66", "98"], answer: 1, why: "'A' is 65, so 'B' is 66; 'b' is 98." },
    ],
    [
      {
        exercise: "text",
        task: 'With sentence: db "how many vowels here", 0, count the letters a, e, i, o and u and print the count.',
        expected: ["6"],
        hints: ["Loop over the bytes until the 0, loading each with movzx rax, byte [sentence + rbx].", "Compare it with 'a', 'e', 'i', 'o' and 'u' in turn; jump to an inc r12 when one matches."],
        solution: HELPER + '\n\nglobal _start\n\nsection .data\nsentence: db "how many vowels here", 0\n\nsection .text\n_start:\n    xor rbx, rbx\n    xor r12, r12\nnext:\n    movzx rax, byte [sentence + rbx]\n    cmp rax, 0\n    je done\n    cmp rax, \'a\'\n    je vowel\n    cmp rax, \'e\'\n    je vowel\n    cmp rax, \'i\'\n    je vowel\n    cmp rax, \'o\'\n    je vowel\n    cmp rax, \'u\'\n    je vowel\n    jmp skip\nvowel:\n    inc r12\nskip:\n    inc rbx\n    jmp next\ndone:\n    mov rdi, r12\n    call print_int\n\n    mov rax, 60\n    xor rdi, rdi\n    syscall',
      },
    ],
  ],
  recapPt: [
    "Texto são bytes, um por carácter; muitas vezes acaba num byte 0.",
    "Um carácter é um número: 'A' é 65, 'a' é 97.",
    "Percorre texto com [texto + rbx] até encontrar o 0.",
  ],
  recap: [
    "Text is bytes, one per character; it often ends with a 0 byte.",
    "A character is a number: 'A' is 65, 'a' is 97.",
    "Walk text with [text + rbx] until you meet the 0.",
  ],
};
