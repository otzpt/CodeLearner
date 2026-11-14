module.exports = {
  id: "next-steps",
  title: "Next: below the operating system",
  titlePt: "A seguir: abaixo do sistema operativo",
  screens: [
    [
      "Every program so far asked Linux to print and to exit. Below Linux there is no syscall: the code that starts a computer talks to the hardware itself. The first piece is a boot sector, 512 bytes the computer loads from disk and runs, in an older 16-bit mode:",
      { code: "bits 16\norg 0x7c00\n\n    mov si, message\nprint:\n    lodsb               ; next byte of message into al\n    cmp al, 0\n    je stop\n    mov ah, 0x0e        ; firmware: show one character\n    int 0x10\n    jmp print\nstop:\n    hlt\n    jmp stop\n\nmessage: db \"Hello from the boot sector\", 0\n\ntimes 510 - ($ - $$) db 0\ndw 0xaa55", run: false },
      "int 0x10 asks the firmware, not an operating system, to show a character. The last two lines pad the file to 510 bytes and add the two bytes that mark it as bootable.",
    ],
    [
      "This course cannot run that here: it needs a whole computer, or an emulator of one. To try it with QEMU:",
      { code: "nasm -f bin -o boot.bin boot.asm\nqemu-system-x86_64 -drive format=raw,file=boot.bin", run: false },
      "The folder assembly/os in CodeLearner continues from here: a two-stage loader, protected mode, 64-bit long mode, a kernel in C and interrupts, each checked in QEMU by assembly/check-os.py.",
      { quiz: "What does a boot sector use to show text, with no operating system?", choices: ["syscall", "the firmware, through int 0x10"], answer: 2, why: "There is no operating system yet to take a syscall." },
    ],
  ],
  recapPt: [
    "Abaixo do sistema operativo não há syscall: o código fala com o hardware e o firmware.",
    "Um setor de arranque tem 512 bytes, acaba em 0xaa55 e corre em modo de 16 bits.",
    "Experimenta com nasm -f bin e qemu-system-x86_64; continua em assembly/os.",
  ],
  recap: [
    "Below the operating system there is no syscall: code talks to the hardware and firmware.",
    "A boot sector is 512 bytes ending in 0xaa55, running in 16-bit mode.",
    "Try it with nasm -f bin and qemu-system-x86_64; carry on in assembly/os.",
  ],
};
