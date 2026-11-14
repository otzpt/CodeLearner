module.exports = {
  id: "first-program",
  title: "Your first program",
  titlePt: "O teu primeiro programa",
  screens: [
    [
      "The processor runs very simple instructions: move a number, add two numbers, jump somewhere else. Assembly is those instructions written as words, one per line. Every other language is eventually turned into them.",
      "NASM turns the text into machine code, ld makes it a program, and then it runs. This course is for 64-bit Linux on an x86-64 processor:",
      { code: "nasm -f elf64 -o hello.o hello.asm\nld -o hello hello.o\n./hello", run: false },
    ],
    [
      "A program has no print. To show text it asks the operating system, with an instruction called syscall. Which request it is, and its details, go in registers first: small storage slots inside the processor, with names like rax and rdi:",
      { code: 'global _start\n\nsection .data\nmessage: db "Hello!", 10\n\nsection .text\n_start:\n    mov rax, 1          ; request 1: write\n    mov rdi, 1          ; to the screen\n    mov rsi, message    ; these bytes\n    mov rdx, 7          ; this many of them\n    syscall\n\n    mov rax, 60         ; request 60: exit\n    mov rdi, 0          ; with code 0, meaning fine\n    syscall' },
    ],
    [
      "Line by line: section .data holds the program's data; message: is a label, a name for a place in memory, and db writes the bytes of \"Hello!\" and 10, the new-line character. section .text holds the instructions; _start is where the program begins, and global makes it visible to ld. ; starts a comment.",
      "mov rax, 1 puts the number 1 into register rax. Hello! plus the new line is 7 bytes, so rdx is 7.",
      { quiz: "In mov rax, 60, which way does the value go?", choices: ["from rax into 60", "60 into rax"], answer: 2, why: "The destination comes first: mov destination, source." },
    ],
    [
      "Without the exit request at the end, the processor keeps going into whatever bytes come next, and the program crashes:",
      { code: 'global _start\n\nsection .data\nmessage: db "bye", 10\n\nsection .text\n_start:\n    mov rax, 1\n    mov rdi, 1\n    mov rsi, message\n    mov rdx, 4\n    syscall', error: true },
      "Segmentation fault is the operating system stopping a program that touched memory it should not. In assembly it is the most common error.",
      { tip: "mov destination, source. Values go in registers, then syscall. Always end with exit (rax 60)." },
    ],
    [
      {
        exercise: "hello",
        task: "Write a program that prints Ana on one line and 2026 on the next, then exits. (In the file, the print_int helper sits at the top; you do not need it yet.)",
        expected: ["Ana", "2026"],
        hints: ['In section .data: text: db "Ana", 10, "2026", 10', "That is 9 bytes, so one write with rdx = 9 prints both lines."],
        solution: 'global _start\n\nsection .data\ntext: db "Ana", 10, "2026", 10\n\nsection .text\n_start:\n    mov rax, 1\n    mov rdi, 1\n    mov rsi, text\n    mov rdx, 9\n    syscall\n\n    mov rax, 60\n    xor rdi, rdi\n    syscall',
      },
    ],
  ],
  recapPt: [
    "Assembly são as instruções do processador escritas como palavras.",
    "mov destino, origem; os registos (rax, rdi, ...) são espaços dentro do processador.",
    "syscall pede algo ao sistema: rax 1 escreve, rax 60 sai.",
    "section .data para dados, section .text para instruções; _start é o início.",
  ],
  recap: [
    "Assembly is the processor's instructions written as words.",
    "mov destination, source; registers (rax, rdi, ...) are slots inside the processor.",
    "syscall asks the system for something: rax 1 writes, rax 60 exits.",
    "section .data for data, section .text for instructions; _start is where it begins.",
  ],
};
