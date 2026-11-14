// How the Assembly course runs code: NASM assembles the file for 64-bit
// Linux, ld links it, and the program runs. Linux x86-64 only.

// print_int, used from lesson 2 on: printing a number is a whole program in
// itself, so the lessons include this ready-made one above their code.
const HELPER = "; print_int: prints the number in rdi as a decimal, then a new line.\nsection .text\nprint_int:\n    push rbx\n    sub rsp, 32\n    mov rax, rdi\n    lea rsi, [rsp + 31]\n    mov byte [rsi], 10\n    mov rcx, 1\n    mov rbx, 10\n    xor r8, r8\n    test rax, rax\n    jns .digit\n    neg rax\n    mov r8, 1\n.digit:\n    xor rdx, rdx\n    div rbx\n    add dl, '0'\n    dec rsi\n    mov [rsi], dl\n    inc rcx\n    test rax, rax\n    jnz .digit\n    test r8, r8\n    jz .write\n    dec rsi\n    mov byte [rsi], '-'\n    inc rcx\n.write:\n    mov rax, 1\n    mov rdi, 1\n    mov rdx, rcx\n    syscall\n    add rsp, 32\n    pop rbx\n    ret";

const START = `global _start

section .text
_start:
    ; your code here

    mov rax, 60         ; exit
    xor rdi, rdi        ; with code 0
    syscall`;

module.exports = {
  id: "assembly",
  title: "ASSEMBLY FROM ZERO",
  titlePt: "ASSEMBLY DO ZERO",
  HELPER,
  file: "main.asm",
  starter: HELPER + "\n\n" + START,
  steps: (file) => [
    ["nasm", "-f", "elf64", "-o", "program.o", file],
    ["ld", "-o", "program", "program.o"],
    ["./program"],
  ],
  howToRun: (name) => `nasm -f elf64 -o program.o ${name} && ld -o program program.o && ./program`,

  // nasm: "file.asm:LINE: error: message"; ld: "undefined reference" or
  // "cannot find entry symbol"; a crash ends with a signal.
  parseError(text) {
    const m = text.match(/\.asm:(\d+): (?:fatal: |error: )(.*)/);
    if (m) {
      return { errorLine: "error: " + m[2], line: Number(m[1]), compile: true };
    }
    const ld = text.split("\n").find((l) => /undefined reference|cannot find entry/.test(l));
    if (ld) {
      return { errorLine: ld.replace(/^.*?: /, ""), compile: true };
    }
    return { errorLine: text.trim().split("\n")[0] || "" };
  },

  errors: [
    [/instruction expected|label or instruction expected/, "a word NASM does not know as an instruction: a typo, or a label missing its colon.", "uma palavra que o NASM não conhece como instrução: um erro de escrita, ou uma etiqueta sem os dois pontos."],
    [/symbol .* not defined|undefined reference/, "a label that is used but never defined. Check the spelling.", "uma etiqueta usada mas nunca definida. Verifica a ortografia."],
    [/invalid combination of opcode and operands/, "that instruction cannot take those operands, often two memory operands or mismatched sizes.", "essa instrução não aceita esses operandos, muitas vezes dois operandos de memória ou tamanhos diferentes."],
    [/operation size not specified/, "NASM cannot tell how many bytes to use: add byte, word, dword or qword.", "o NASM não sabe quantos bytes usar: junta byte, word, dword ou qword."],
    [/entry symbol/, "there is no _start label marked global, so the program has nowhere to begin.", "não há a etiqueta _start marcada como global, por isso o programa não tem onde começar."],
  ],
};
