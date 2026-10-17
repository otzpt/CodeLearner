; lessons_kernel.asm - modules 13 to 16: protected mode, long mode, a kernel in C,
; interrupts.
;
; The code these modules show is in os/, built into disk images and booted
; in QEMU by check-os.py. Register dumps, exception logs and screen contents
; quoted in the text were read back out of the emulator. The live lines in
; module 13 are computed by this program while you read.

%include "course.inc"

default rel

global lesson_13_protected
global lesson_14_long
global lesson_15_kernel
global lesson_16_interrupts

section .rodata

; ---- module 13 ----------------------------------------------------
m13_title:   db "MODULE 13 - PROTECTED MODE", 0
m13_p1_hdr:  db "PART 1: what protected mode gives, and takes away", 0
m13_p2_hdr:  db "PART 2: the GDT: a table of 8-byte descriptors", 0
m13_p3_hdr:  db "PART 3: os/pm.asm", 0
m13_p4_hdr:  db "PART 4: what the CPU looks like afterwards", 0
m13_p5_hdr:  db "PART 5: three ways to reset the machine", 0

m13_p1:
    ln "  Protected mode is the 32-bit mode of the 386. It gives an operating"
    ln "  system what real mode never could:"
    ln ""
    ln "    - 32-bit registers and addresses: 4 GiB, not 1 MiB"
    ln "    - checking: every memory access is tested against a descriptor, and"
    ln "      code runs at a privilege level, ring 0 for the kernel and ring 3"
    ln "      for programs, which is the basis of memory protection"
    ln ""
    ln "  What it takes away is the BIOS. Its services are real-mode code, 16-bit"
    ln "  instructions that assume segment * 16 + offset, and the CPU no longer"
    ln "  works that way. By now the module has already read everything it needs"
    ln "  from the BIOS: the boot drive and the memory map (module 12). From"
    ln "  here on you talk to the hardware yourself."
    ln ""
    ln "  What stays the same: I/O ports, the serial port, and video memory at"
    ln "  0xB8000. Module 11 carries over unchanged, with 32-bit registers."
    ln ""
    ln "  What is different is how a segment register works. In real mode it"
    ln "  held an address divided by 16. In protected mode it holds a SELECTOR:"
    ln "  an index into a table the CPU reads, the Global Descriptor Table, and"
    ln "  nothing runs until that table exists."
    db 0

m13_p2:
    ln "  A descriptor is 8 bytes with its fields scattered, as module 7 built"
    ln "  them. The three the CPU needs to start, as constants:"
    ln ""
    ln "    descriptor 0   0x0000000000000000   must be all zeros"
    ln "    descriptor 1   0x00CF9A000000FFFF   code, 4 GiB, ring 0"
    ln "    descriptor 2   0x00CF92000000FFFF   data, 4 GiB, ring 0"
    ln ""
    ln "  The first constant taken apart, by this lesson, now:"
    ln ""
    db 0
m13_p2_a:   db "    limit  (bits 0-15, 48-51)  = ", 0
m13_p2_b:   db "    base   (bits 16-39, 56-63) = ", 0
m13_p2_c:   db "    access (bits 40-47)        = ", 0
m13_p2_d:   db "    flags  (bits 52-55)        = ", 0
m13_p2_e:
    ln ""
    ln "  Base 0 and limit 0xFFFFF would be a megabyte, but flag bit G (the top"
    ln "  one of 0xC) counts the limit in 4 KiB pages: 4 GiB, the whole space."
    ln "  The flag D means 32-bit. The access byte 0x9A is: present, ring 0,"
    ln "  a code/data descriptor, executable, readable; 0x92 is the same but a"
    ln "  writable data segment. Base 0 means every segment starts at address"
    ln "  0: the 'flat model', and the reason segmentation then disappears."
    ln ""
    ln "  A selector is the descriptor's byte offset in the table, with two"
    ln "  low bits for the privilege level it asks for: descriptor 1 is"
    ln "  selector 0x08, descriptor 2 is 0x10. lgdt takes 6 bytes: the table's"
    ln "  limit, which is its size MINUS ONE (the last valid byte), and base."
    db 0

m13_p3:
    ln ""
    ln "  The order matters. cli, because an interrupt arriving mid-switch"
    ln "  finds no handler table. lgdt. Set bit 0 of cr0. Then a far jump:"
    ln "  it loads cs from the new table, and until it runs the CPU is in"
    ln "  protected mode with a real-mode code segment. After it, bits 32 tells"
    ln "  NASM to emit 32-bit code, and every other segment register is loaded"
    ln "  with descriptor 2."
    db 0

m13_p4:
    ln "  QEMU's monitor command info registers, run a moment after pm.bin"
    ln "  booted, shows the flat model as the CPU holds it:"
    ln ""
    ln "    CS =0008 00000000 ffffffff 00cf9a00 DPL=0 CS32 [-R-]"
    ln "    DS =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]"
    ln "    GDT=     00007c58 00000017"
    ln "    CR0=00000011"
    ln ""
    ln "  Each segment line is: the selector, then the base, limit and flags the"
    ln "  CPU copied out of the table into the register's hidden part. CS is"
    ln "  selector 8: base 0, limit 0xFFFFFFFF, a 32-bit code segment (CS32),"
    ln "  readable. DS, SS, ES, FS and GS are selector 0x10: writable data."
    ln "  GDT is the table's address and limit: 0x17 is 24 bytes minus one."
    ln "  CR0 is 0x11: bit 0, PE, is the switch; bit 4 was already set."
    ln ""
    ln "  On the screen, with no BIOS call involved, in white on green (0x2F)"
    ln "  at the very top left:"
    ln ""
    ln "    Now in 32-bit protected mode"
    db 0

m13_p5:
    ln "  Get any of this wrong and the usual result is not an error message."
    ln "  The CPU faults, finds no handler, faults again, and resets: a TRIPLE"
    ln "  FAULT. The machine just restarts. Three variants of pm.asm, built and"
    ln "  booted for this module, with QEMU's own log of what happened:"
    ln ""
    ln "  1. A near jump instead of the far one. The machine does not reset: it"
    ln "     hangs with CR0 = 0x11 and CS still 0, the 32-bit code being decoded"
    ln "     as 16-bit. The message never appears; the SeaBIOS banner stays."
    ln ""
    ln "  2. A far jump with the DATA selector (0x10) in cs. A code segment"
    ln "     register must hold a code descriptor. QEMU logged, in order:"
    ln ""
    ln "       check_exception old: 0xffffffff new 0xd     (a general protection fault)"
    ln "       check_exception old: 0xd new 0xd            (and again, while handling it)"
    ln "       check_exception old: 0x8 new 0xd            (a double fault, and again)"
    ln "       Triple fault"
    ln ""
    ln "  3. int 0x10, a BIOS call, from protected mode. The CPU now looks up"
    ln "     vector 16 in an interrupt table that is empty. The same four"
    ln "     lines, the same triple fault."
    ln ""
    ln "  Module 16 builds the table that turns those into messages instead."
    db 0

m13_summary_hdr: db "SUMMARY", 0
m13_summary:
    ln "   - protected mode: 32-bit, 4 GiB, privilege rings, and no BIOS"
    ln "   - a segment register holds a selector: a descriptor's offset in the"
    ln "     GDT; code 0x08, data 0x10 after the null descriptor"
    ln "   - the pointer lgdt reads is limit = size - 1, then a 32-bit base"
    ln "   - cli, lgdt, set cr0 bit 0, FAR jump, then reload the other segments"
    ln "   - a wrong step is usually a triple fault: the machine resets, and"
    ln "     QEMU's -d int log shows the chain that led to it"
    db 0

m13_exercise:
    db `\n  >> EXERCISE - MODULE 13\n  ------------------------------------------------------\n`, 0
m13_q1_text:   db `The selector for GDT descriptor 1 (the second entry, after the\n  null one) is what? (hex, written like 0x1234)`, 0
m13_q1_answer: db "0x08", 0
m13_q1_why:    db `A selector's index sits in bits 3 and up, so descriptor 1 is\n             1 * 8 = 0x08, and descriptor 2 is 0x10.`, 0
m13_q2_text:   db `The limit in the 6-byte pointer lgdt reads is the size of the\n  table minus what? (a number)`, 0
m13_q2_answer: db "1", 0
m13_q2_why:    db `The limit is the offset of the last valid byte, so a 24-byte\n             table has limit 23, which is 0x17 in the register dump.`, 0
m13_q3_text:   db `Which bit of cr0 switches protected mode on? (a number)`, 0
m13_q3_answer: db "0", 0
m13_q3_why:    db `Bit 0, PE. It takes effect at once; the far jump that follows\n             is what finishes the switch by loading cs.`, 0

m13_challenge:
    ln ""
    ln "  >> WRITE THIS YOURSELF, in a real file"
    ln ""
    ln "  Write a boot sector that switches to 32-bit protected mode with a"
    ln "  GDT of your own (null, code, data), then writes the letter P in"
    ln "  white on red (attribute 0x4F) into the top-left screen cell, and"
    ln "  halts. No BIOS after the switch. It is checked by reading the"
    ln "  emulated video memory and the CPU's registers."
    ln ""
    ln "  Assemble and boot with:"
    ln "    nasm -f bin boot.asm -o boot.bin"
    ln "    qemu-system-x86_64 -drive format=raw,file=boot.bin"
    ln ""
    ln "  The top-left cell must hold P, white on red, and CS must be 0x08."
    db 0

m13_solution:
    incbin "13.asm"
    db 0
m13_pm:
    incbin "pm.asm"
    db 0

; ---- module 14 ----------------------------------------------------
m14_title:   db "MODULE 14 - LONG MODE AND PAGING", 0
m14_p1_hdr:  db "PART 1: 64 bits, and the price: paging", 0
m14_p2_hdr:  db "PART 2: a virtual address, taken apart", 0
m14_p3_hdr:  db "PART 3: os/lm.asm", 0
m14_p4_hdr:  db "PART 4: what the CPU looks like afterwards", 0
m14_p5_hdr:  db "PART 5: ways it fails, and one where QEMU is kinder than the hardware", 0

m14_p1:
    ln "  Long mode is the 64-bit mode of the AMD64 / x86-64 architecture: the"
    ln "  16 registers of module 1, 64-bit addresses, and the instruction set"
    ln "  this course has used throughout. It is also how nearly all 64-bit"
    ln "  kernels start. It comes with conditions:"
    ln ""
    ln "    - paging must be on. There is no 64-bit mode without page tables."
    ln "    - segmentation is mostly switched off. Code and data segments have"
    ln "      base 0 and no limit; a code descriptor with its L bit set says"
    ln "      'this segment is 64-bit', and that is nearly all it still does."
    ln "    - not every CPU has it. The kernel asks with CPUID before it tries."
    ln ""
    ln "  The path from where module 13 stopped:"
    ln ""
    ln "    1. check CPUID extended leaf 0x80000001, bit 29 of edx (LM)"
    ln "    2. build the page tables, and put the top one's address in cr3"
    ln "    3. set PAE, bit 5 of cr4, the page-table format long mode uses"
    ln "    4. set LME, bit 8 of the EFER register (a model-specific register,"
    ln "       read and written with rdmsr and wrmsr, number 0xC0000080)"
    ln "    5. set PG, bit 31 of cr0. With LME set, this turns long mode on"
    ln "    6. a far jump into a 64-bit code segment, which starts 64-bit code"
    db 0

m14_p2:
    ln "  With paging every address a program uses is VIRTUAL, and the CPU"
    ln "  translates it through tables kept in memory. A 64-bit address uses"
    ln "  48 bits of it, in five pieces:"
    ln ""
    ln "    bits 39-47   index into the PML4, the top-level table"
    ln "    bits 30-38   index into the PDPT"
    ln "    bits 21-29   index into the PD"
    ln "    bits 12-20   index into the PT"
    ln "    bits  0-11   the byte within a 4 KiB page"
    ln ""
    ln "  Each table is one 4096-byte page: 512 entries of 8 bytes, which is"
    ln "  why each index is 9 bits. An entry holds the physical address of the"
    ln "  next table and flags in its low bits: bit 0 present, bit 1 writable,"
    ln "  bit 2 user-accessible, bit 7 'big page'. If bit 7 is set in a PD"
    ln "  entry, that entry maps a 2 MiB page itself and the PT level is"
    ln "  skipped: bits 0-20 are all offset. Two addresses taken apart now:"
    ln ""
    db 0
m14_p2_a:   db "    0xB8000, the screen:", 10, 0
m14_p2_b:   db "    0xFFFFFFFF80000000:", 10, 0
m14_p2_pml4: db "      PML4 index = ", 0
m14_p2_pdpt: db "      PDPT index = ", 0
m14_p2_pd:   db "      PD index   = ", 0
m14_p2_pt:   db "      PT index   = ", 0
m14_p2_off:  db "      offset     = ", 0
m14_p2_c:
    db 0

m14_p3:
    ln ""
    ln "  The screen is in the first entry of every table, so one 2 MiB page at"
    ln "  0 covers it. The other address of PART 2, PML4 entry 511, is the"
    ln "  'higher half' where kernels often live: the top 512 GiB."
    ln ""
    ln "  The three tables this builds sit at 0x1000, 0x2000 and 0x3000. Three"
    ln "  entries link them: PML4[0] = 0x2003 (the PDPT, present + writable),"
    ln "  PDPT[0] = 0x3003 (the PD), PD[0] = 0x83 (a 2 MiB page at physical 0:"
    ln "  present, writable, big). That maps virtual 0 to 2 MiB onto physical"
    ln "  0 to 2 MiB: an identity map. It has to include the code that turns"
    ln "  paging on, which is running at 0x7Cxx when it does."
    db 0

m14_p4:
    ln "  info registers a moment after lm.bin booted:"
    ln ""
    ln "    CS =0018 0000000000000000 ffffffff 00af9a00 DPL=0 CS64 [-R-]"
    ln "    CR0=80000011 CR2=0000000000000000 CR3=0000000000001000 CR4=00000020"
    ln "    EFER=0000000000000500"
    ln ""
    ln "  CS is selector 0x18, the fourth descriptor, and the monitor calls it"
    ln "  CS64: flag nibble 0xA, G and L. CR0 has PG (bit 31) as well as PE;"
    ln "  CR3 is the PML4's address; CR4 is 0x20, PAE. EFER is 0x500: bit 8,"
    ln "  LME, which the code set, and bit 10, LMA, which the CPU set itself"
    ln "  the instant paging came on: 'long mode active'. LME is a request;"
    ln "  LMA is the fact. And the screen, written by 64-bit code through a"
    ln "  64-bit register, white on blue:"
    ln ""
    ln "    Now in 64-bit long mode"
    db 0

m14_p5:
    ln "  1. A CPU without long mode. QEMU can pretend to be one: -cpu qemu32."
    ln "     lm.asm's CPUID test fails and it writes NO64 on the screen in"
    ln "     white on red, instead of ever touching cr3. Checked."
    ln ""
    ln "  2. The code is not mapped. The variant with the PD entry marked not"
    ln "     present (0x82, not 0x83): the next instruction after mov cr0 needs"
    ln "     a page that does not exist, and QEMU logged:"
    ln ""
    ln "       check_exception old: 0xffffffff new 0xe     (a page fault)"
    ln "       check_exception old: 0xe new 0xe"
    ln "       check_exception old: 0x8 new 0xe"
    ln "       Triple fault"
    ln ""
    ln "  3. PAE left off, with LME set. Intel's manual says that turning on"
    ln "     paging then is a general protection fault, and on real hardware"
    ln "     it would be. QEMU is kinder: it silently ignores the write to cr0."
    ln "     Observed here: CR0 stays 0x11, EFER shows 0x100 (LME, but not LMA),"
    ln "     CS holds the 64-bit descriptor in a CPU that is not in long mode,"
    ln "     and the screen never changes. Nothing reset, nothing printed. An"
    ln "     emulator accepting a mistake is not a pass: run on real hardware,"
    ln "     that same file would fault."
    ln ""
    ln "  The rest of the order is forced by the CPU, not by taste: PAE before"
    ln "  PG, tables built before cr3 is loaded, and the code you are running"
    ln "  identity-mapped."
    db 0

m14_summary_hdr: db "SUMMARY", 0
m14_summary:
    ln "   - long mode needs paging; segments keep only the L bit of the code one"
    ln "   - check CPUID 0x80000001 edx bit 29 first"
    ln "   - a virtual address is four 9-bit indices and a 12-bit offset; a PD"
    ln "     entry with bit 7 set maps 2 MiB directly"
    ln "   - order: tables, cr3, PAE, EFER.LME, cr0.PG, far jump to a 64-bit code"
    ln "     segment; the running code must be identity-mapped"
    ln "   - LME is the request, LMA the result; EFER 0x500 means both"
    ln "   - an emulator may forgive what real hardware would fault on"
    db 0

m14_exercise:
    db `\n  >> EXERCISE - MODULE 14\n  ------------------------------------------------------\n`, 0
m14_q1_text:   db `How many bytes is one page-table entry? (a number)`, 0
m14_q1_answer: db "8", 0
m14_q1_why:    db `Eight: 64 bits, holding an address and the flag bits. 4096 / 8\n             is 512 entries per table, which is why an index is 9 bits.`, 0
m14_q2_text:   db `How many entries does one 4096-byte page table hold?\n  (a number)`, 0
m14_q2_answer: db "512", 0
m14_q2_why:    db `4096 bytes of 8-byte entries. 512 is 2 to the 9th, so one table\n             level translates 9 bits of the virtual address.`, 0
m14_q3_text:   db `Which control register holds the physical address of the\n  top-level page table? (cr and a number)`, 0
m14_q3_answer: db "cr3", 0
m14_q3_why:    db `cr3. Writing it also flushes the translation cache. cr0 holds\n             PG, cr4 holds PAE, and cr2 the address of the last page fault.`, 0

m14_challenge:
    ln ""
    ln "  >> WRITE THIS YOURSELF, in a real file"
    ln ""
    ln "  Start from os/lm.asm. Make it map the first FOUR MiB (two 2 MiB pages"
    ln "  in the PD), then have the 64-bit code store the byte 'M' at physical"
    ln "  address 0x300000 (3 MiB), read it back, and write it, white on blue"
    ln "  (attribute 0x1F), into the top-left screen cell. If the second page"
    ln "  is missing the machine must fault, so the screen cell proves it."
    ln ""
    ln "  Assemble and boot with:"
    ln "    nasm -f bin boot.asm -o boot.bin"
    ln "    qemu-system-x86_64 -drive format=raw,file=boot.bin"
    ln ""
    ln "  The top-left cell must hold M, white on blue, with EFER = 0x500."
    db 0

m14_solution:
    incbin "14.asm"
    db 0
m14_lm:
    incbin "lm.asm"
    db 0


; ---- module 15 ----------------------------------------------------
m15_title:   db "MODULE 15 - A KERNEL IN C", 0
m15_p1_hdr:  db "PART 1: the plan: three programs and one disk", 0
m15_p2_hdr:  db "PART 2: os/kernel/boot.asm", 0
m15_p3_hdr:  db "PART 3: os/kernel/entry.asm: the way into C", 0
m15_p3b_hdr: db "PART 3 (continued): os/kernel/linker.ld", 0
m15_p4_hdr:  db "PART 4: os/kernel/kernel.c", 0
m15_p4b_hdr: db "PART 4 (continued): running it", 0
m15_p5_hdr:  db "PART 5: every flag is a bug that was already found", 0

m15_p1:
    ln "  Modules 10 to 14 each ended in one 512-byte file. A kernel in C"
    ln "  needs the same journey and a place to put the C. The pieces:"
    ln ""
    ln "    boot.asm     stage 1 (module 12): reads stage 2 AND the kernel,"
    ln "                 because only real mode can use the BIOS to read"
    ln "    stage2.asm   module 14's real mode -> long mode, then jumps to the"
    ln "                 kernel. It has no output of its own."
    ln "    entry.asm    the kernel's first instructions: a stack, zeroed .bss"
    ln "    kernel.c     everything else"
    ln ""
    ln "  The disk and then memory, as built:"
    ln ""
    ln "    sector 0      boot.asm        -> 0x07C00"
    ln "    sectors 1-2   stage2.asm      -> 0x07E00"
    ln "    sectors 3-18  kernel (8 KiB)  -> 0x10000"
    ln ""
    ln "    0x1000-0x3FFF  page tables      0x10000  the kernel      0x90000  stack"
    ln ""
    ln "  make in os/kernel builds all of it and joins the pieces with cat into"
    ln "  os.img. The kernel is compiled to an ELF file, linked to run at"
    ln "  0x10000, and objcopy -O binary strips it to the raw bytes. There"
    ln "  is no ELF loader here: the bytes are the program, loaded to the"
    ln "  address the linker assumed."
    db 0

m15_p2:
    ln ""
    ln "  The new part is the second read: the same int 0x13 call as module"
    ln "  12, with the destination written as segment 0x1000, offset 0, which"
    ln "  is physical 0x10000. Both reads happen here, in real mode, because"
    ln "  once stage 2 has left it there is no way to read the disk."
    db 0

m15_p3:
    ln ""
    ln "  Four things have to happen between 'a 64-bit CPU is running' and"
    ln "  'a C function can run', and this is all of them: a stack (rsp), the"
    ln "  .bss zeroed, a call, and somewhere to stop if the function returns."
    ln "  The .bss one is the easy one to forget, and PART 5 shows it."
    ln ""
    ln "  Everything between __bss_start and __bss_end is defined by the"
    ln "  linker script on the next screen. The section name .text.entry is"
    ln "  how entry.asm gets itself put first in the file."
    db 0

m15_p3b:
    ln ""
    ln "  . = 0x10000 sets the address the code is linked to run at. The file"
    ln "  that comes out has no headers: the first byte is the first byte of"
    ln "  .text.entry, so address 0x10000 is _start, which stage 2 jumps to."
    ln "  nm confirms it:  0000000000010000 T _start"
    db 0

m15_p4:
    ln ""
    ln "  The three output helpers are module 11 and 13's, in C; kmain is the"
    ln "  last twenty lines."
    db 0

m15_p4b:
    ln "  Booted, the screen and the serial port both carry:"
    ln ""
    ln "    Hello from a kernel written in C"
    ln "    kmain is at 0x0000000000010180"
    ln "    .bss counter = 0x0000000000000000"
    ln "    .data value  = 0x0000000000001234"
    ln ""
    ln "  with CS = 0x18 and EFER = 0x500, module 14's registers. size: 724"
    ln "  bytes of code and constants, 4 of .data, 12 of .bss. The two variables"
    ln "  are volatile: nothing writes them, so gcc would print the constants it"
    ln "  can see instead of reading memory (the next screen)."
    db 0

m15_p5:
    ln "  The compile line looks like superstition. Each flag is a bug that was"
    ln "  hit, and each of these was reproduced for this module:"
    ln ""
    ln "  volatile. Two stores to the same screen cell, gcc -O2:"
    ln ""
    ln "     plain:    mov eax, 3906 / mov [753664], ax             one store"
    ln "     volatile: mov [753664], ax / mov [753664], dx          both"
    ln ""
    ln "  Without volatile the first store is thrown away as dead. For memory"
    ln "  it is, but a device register is not memory: the write is the event."
    ln ""
    ln "  -mgeneral-regs-only. A kernel with `volatile struct {...} r = {0}` and"
    ln "  gcc's default flags compiled to pxor xmm0 and movaps. SSE is off until"
    ln "  the kernel switches it on, so QEMU logged:"
    ln ""
    ln "     check_exception old: 0xffffffff new 0x6     (invalid opcode, #UD)"
    ln "     then the general-protection and double-fault chain of module 13,"
    ln "     and: Triple fault"
    ln ""
    ln "  The zeroing loop in entry.asm. Padding the kernel with 0xAA bytes"
    ln "  instead of 0 (what an old buffer might hold) and removing the loop,"
    ln "  .bss counter printed 0xffffffffaaaaaaaa. With the loop, 0. Real"
    ln "  images pad with zeros, which is why this passes quietly until it"
    ln "  does not."
    ln ""
    ln "  The rest: -ffreestanding (no libc), -fno-pie (linked to a fixed"
    ln "  address), -fno-stack-protector (it wants a canary libc provides) and"
    ln "  -mno-red-zone (module 9, and module 16)."
    db 0

m15_summary_hdr: db "SUMMARY", 0
m15_summary:
    ln "   - read the kernel with the BIOS in real mode; the disk is out of"
    ln "     reach afterwards"
    ln "   - assembly's job at the entry: a stack, zeroed .bss, call kmain"
    ln "   - a linker script sets the address and the order; objcopy -O binary"
    ln "     makes a headerless image whose first byte is _start"
    ln "   - screen memory and ports go through volatile, or stores vanish"
    ln "   - -mgeneral-regs-only until SSE is enabled; -mno-red-zone; the rest"
    ln "     of the flags exist because libc and a loader are missing"
    db 0

m15_exercise:
    db `\n  >> EXERCISE - MODULE 15\n  ------------------------------------------------------\n`, 0
m15_q1_text:   db `Which section is NOT stored in the kernel's file, but is zeroed\n  by entry.asm at boot? (the name, with its dot)`, 0
m15_q1_answer: db ".bss", 0
m15_q1_why:    db `The file only says how large .bss is. Nothing in a flat image\n             loader writes zeros there, so the entry code has to.`, 0
m15_q2_text:   db `Which C keyword forces the compiler to perform every store to\n  a hardware address, even ones it thinks are useless? (one word)`, 0
m15_q2_answer: db "volatile", 0
m15_q2_why:    db `Without it the first of two stores to one address is removed as\n             dead. volatile says the access itself is the effect.`, 0
m15_q3_text:   db `Which gcc flag stops it emitting SSE instructions, which would\n  fault in this kernel? (the whole flag)`, 0
m15_q3_answer: db "-mgeneral-regs-only", 0
m15_q3_why:    db `SSE is not enabled until the kernel sets up cr4 and cr0 for it, so\n             an xmm instruction raises #UD (invalid opcode) with no handler.`, 0

m15_challenge:
    ln ""
    ln "  >> WRITE THIS YOURSELF, in a real file"
    ln ""
    ln "  In os/kernel/kernel.c, add print_dec(uint64_t value, uint8_t attr),"
    ln "  which prints a number in decimal the way print_hex does in hex (the"
    ln "  digit loop of module 4: value % 10, then value / 10, then turn the"
    ln "  digits around). Call it from kmain, before the final loop, so the"
    ln "  screen gets a line:"
    ln ""
    ln "      dec = 123456789"
    ln ""
    ln "  Build and boot with:"
    ln "    cd os/kernel && make run"
    ln ""
    ln "  The line must be on the emulated screen, with the other four."
    db 0

m15_solution:
    incbin "15.c"
    db 0
m15_boot:
    incbin "kernel/boot.asm"
    db 0
m15_entry:
    incbin "kernel/entry.asm"
    db 0
m15_linker:
    incbin "kernel/linker.ld"
    db 0
m15_kernel:
    incbin "kernel/kernel.c"
    db 0

; ---- module 16 ----------------------------------------------------
m16_title:   db "MODULE 16 - INTERRUPTS", 0
m16_p1_hdr:  db "PART 1: what an interrupt is, and the table behind it", 0
m16_p2_hdr:  db "PART 2: os/interrupts/isr.asm", 0
m16_p3_hdr:  db "PART 3: os/interrupts/idt.c", 0
m16_p4_hdr:  db "PART 4: os/interrupts/kmain.c, and running it", 0
m16_p5_hdr:  db "PART 5: four ways interrupts go wrong", 0

m16_p1:
    ln "  An interrupt is the CPU being told to stop, handle something, and carry"
    ln "  on exactly where it was. There are three sources:"
    ln ""
    ln "    exceptions   the CPU itself, vectors 0 to 31: divide error (0),"
    ln "                 invalid opcode (6), double fault (8), general"
    ln "                 protection fault (13), page fault (14) ..."
    ln "    hardware     devices, through the interrupt controller: the timer,"
    ln "                 the keyboard, the disk"
    ln "    software     the int n instruction"
    ln ""
    ln "  Each has a vector number. The CPU finishes the current instruction,"
    ln "  looks the vector up in the INTERRUPT DESCRIPTOR TABLE, pushes rip, cs,"
    ln "  rflags, rsp and ss (plus an error code for some exceptions), and jumps"
    ln "  to the handler that entry names. The handler ends with iretq, which"
    ln "  pops those five and resumes."
    ln ""
    ln "  The table is an array of gates, 16 bytes each in long mode, loaded"
    ln "  with lidt through a 10-byte pointer exactly like lgdt's. Module 9's"
    ln "  rule applies: idt.c declares both structs packed and asserts their"
    ln "  sizes. Until a table is loaded, an interrupt means nothing to the CPU,"
    ln "  which is why module 13's mistakes were triple faults. Now there is a"
    ln "  table, and the same mistakes can produce a message instead."
    db 0

m16_p2:
    ln ""
    ln "  The CPU does not tell a handler which vector it was called for, so"
    ln "  there is one tiny stub per vector, generated by a macro, pushing it."
    ln "  Some exceptions also push an error code and most do not: the stubs"
    ln "  push a fake zero for those, so one C function sees one frame shape."
    ln "  isr_common saves every register, hands C a pointer to the frame, and"
    ln "  undoes it all. The stack is aligned for the call (module 6) and the"
    ln "  C is compiled with -mno-red-zone, because the CPU has just pushed"
    ln "  straight through whatever sat below rsp (module 9)."
    db 0

m16_p3:
    ln ""
    ln "  Three jobs, and the comments on each say why it is needed:"
    ln ""
    ln "  - set_gate: split a handler's address across the gate, name the"
    ln "    64-bit code selector 0x18, and type 0x8E (present, ring 0,"
    ln "    interrupt gate)."
    ln "  - pic_remap: the 8259 interrupt controllers deliver IRQ 0-7 to"
    ln "    vectors 8-15, on top of the CPU's own exceptions. Move them to 32."
    ln "  - interrupt_handler: the timer counts a tick; the keyboard reads"
    ln "    port 0x60 for the scancode; and both send EOI (end of interrupt) to"
    ln "    the controller, or it never delivers another."
    db 0

m16_p4:
    ln ""
    ln "  Booted, the screen and the serial port carry, with a key pressed in"
    ln "  QEMU for each of a, s and d:"
    ln ""
    ln "    Interrupts: IDT, PIC and PIT set up"
    ln "    exception 0 at 0x00000000000106bd"
    ln "    returned from the handler"
    ln "    timer: 100 ticks"
    ln "    key: 0x000000000000001e"
    ln "    key: 0x000000000000001f"
    ln "    key: 0x0000000000000020"
    ln ""
    ln "  The divide error is deliberate. The handler printed where it happened"
    ln "  and moved the saved rip past the two-byte div, or iretq would have run"
    ln "  the same instruction again, forever. 'timer: 100 ticks' appears one"
    ln "  second after boot: the timer is programmed to 100 Hz. The scancodes"
    ln "  are the ones module 11 read through the BIOS (1E, 1F, 20); the"
    ln "  interrupt reads them from port 0x60 itself. info registers shows the"
    ln "  table: IDT = 000002ff, which is 48 gates of 16 bytes, minus one."
    db 0

m16_p5:
    ln "  Four variants, each built and booted for this module:"
    ln ""
    ln "  1. No end-of-interrupt. The timer fires once; the controller waits"
    ln "     for the acknowledgement that never comes. 'timer: 100 ticks' never"
    ln "     prints, and ticks read from the kernel's memory is 1."
    ln ""
    ln "  2. No remap. The first timer interrupt arrives as vector 8, the"
    ln "     double fault, and the kernel prints:"
    ln ""
    ln "       exception 8 at 0x0000000000000018"
    ln ""
    ln "     0x18 is not an address in the kernel. It is the code selector:"
    ln "     a real double fault pushes an error code, an interrupt does not,"
    ln "     so the stub read the frame one slot off."
    ln ""
    ln "  3. A gate pointing at selector 0x08, the 32-bit code segment. Long"
    ln "     mode will not run a 64-bit interrupt handler there, so even the"
    ln "     divide error can not be delivered: a general protection fault,"
    ln "     then a double fault, then a triple fault. QEMU's log is the four"
    ln "     lines of module 13 again."
    ln ""
    ln "  4. Without -mno-red-zone (module 9) an interrupt can overwrite the"
    ln "     locals of the function it interrupted. It is the one variant that"
    ln "     was not reproduced here; it is documented, and intermittent."
    db 0

m16_summary_hdr: db "SUMMARY", 0
m16_summary:
    ln "   - an interrupt: look up the vector in the IDT, push five registers,"
    ln "     run the handler, iretq; the gate is 16 bytes and lidt takes 10"
    ln "   - one stub per vector, a fake error code to keep the frame uniform,"
    ln "     registers saved, one C handler"
    ln "   - remap the PIC away from the exception vectors; send EOI to every"
    ln "     IRQ you handle"
    ln "   - the gate's selector must be the 64-bit code segment"
    ln "   - handlers run on the interrupted stack: -mno-red-zone, and keep"
    ln "     them short"
    db 0

m16_exercise:
    db `\n  >> EXERCISE - MODULE 16\n  ------------------------------------------------------\n`, 0
m16_q1_text:   db `How many bytes is one gate in the 64-bit interrupt descriptor\n  table? (a number)`, 0
m16_q1_answer: db "16", 0
m16_q1_why:    db `Sixteen: the handler address is split in three pieces, plus the\n             selector, the type byte, the stack index and a reserved field.`, 0
m16_q2_text:   db `Which instruction returns from an interrupt handler in 64-bit\n  mode? (one word)`, 0
m16_q2_answer: db "iretq", 0
m16_q2_why:    db `iretq pops rip, cs, rflags, rsp and ss. A plain ret would pop only\n             the return address and leave the other four on the stack.`, 0
m16_q3_text:   db `A hardware interrupt handler must tell the controller it is\n  done, or no more arrive. What is that signal called? (three letters)`, 0
m16_q3_answer: db "eoi", 0
m16_q3_why:    db `End of interrupt: out 0x20, 0x20 for the master controller. Without\n             it the timer ran exactly once.`, 0

m16_challenge:
    ln ""
    ln "  >> WRITE THIS YOURSELF, in a real file"
    ln ""
    ln "  In os/interrupts/idt.c, make the timer handler print a dot to the"
    ln "  screen and the serial port once a second. The timer is programmed"
    ln "  to 100 Hz and ticks counts the interrupts, so that is every 100th."
    ln ""
    ln "  Build and boot with:"
    ln "    cd os/interrupts && make run"
    ln ""
    ln "  After the line 'timer: 100 ticks', dots must keep arriving, one a"
    ln "  second, until you close the window."
    db 0

m16_solution:
    incbin "16.c"
    db 0
m16_isr:
    incbin "interrupts/isr.asm"
    db 0
m16_idt:
    incbin "interrupts/idt.c"
    db 0
m16_kmain:
    incbin "interrupts/kmain.c"
    db 0

section .text

lesson_13_protected:
    push rbx
    push r12
    sub rsp, 8                  ; two pushes left rsp 8 off a multiple of 16
    mov rdi, m13_title
    call title

    part m13_p1_hdr, m13_p1

    part_begin m13_p2_hdr, m13_p2
    mov rbx, 0x00CF9A000000FFFF
    mov rax, rbx                ; limit = bits 0-15, then bits 48-51 as bits 16-19
    and eax, 0xFFFF
    mov rcx, rbx
    shr rcx, 48
    and ecx, 0xF
    shl ecx, 16
    or rax, rcx
    live_hex m13_p2_a, rax
    mov rax, rbx                ; base = bits 16-39, then bits 56-63 as bits 24-31
    shr rax, 16
    and eax, 0xFFFFFF
    mov rcx, rbx
    shr rcx, 56
    shl ecx, 24
    or rax, rcx
    live_hex m13_p2_b, rax
    mov rax, rbx
    shr rax, 40
    and eax, 0xFF
    live_hex m13_p2_c, rax
    mov rax, rbx
    shr rax, 52
    and eax, 0xF
    live_hex m13_p2_d, rax
    mov rdi, m13_p2_e
    call print_cstr
    part_end

    file_screen m13_p3_hdr, m13_pm, m13_p3
    part m13_p4_hdr, m13_p4
    part m13_p5_hdr, m13_p5

    ; exercise(13)
    mov rdi, m13_exercise
    call print_cstr
    quiz m13_q1_text, m13_q1_answer, m13_q1_why
    quiz m13_q2_text, m13_q2_answer, m13_q2_why
    quiz m13_q3_text, m13_q3_answer, m13_q3_why
    challenge m13_challenge, m13_solution

    ; summary(13)
    summary m13_summary_hdr, m13_summary
    add rsp, 8
    pop r12
    pop rbx
    ret

section .text

; m14_split(rdi = virtual address) prints its five pieces.
m14_split:
    push rbx
    mov rbx, rdi
    mov rax, rbx
    shr rax, 39
    and eax, 0x1FF
    live_dec m14_p2_pml4, rax
    mov rax, rbx
    shr rax, 30
    and eax, 0x1FF
    live_dec m14_p2_pdpt, rax
    mov rax, rbx
    shr rax, 21
    and eax, 0x1FF
    live_dec m14_p2_pd, rax
    mov rax, rbx
    shr rax, 12
    and eax, 0x1FF
    live_dec m14_p2_pt, rax
    mov rax, rbx
    and eax, 0xFFF
    live_hex m14_p2_off, rax
    pop rbx
    ret

lesson_14_long:
    push rbx
    mov rdi, m14_title
    call title

    part m14_p1_hdr, m14_p1

    part_begin m14_p2_hdr, m14_p2
    mov rdi, m14_p2_a
    call print_cstr
    mov rdi, 0xB8000
    call m14_split
    mov rdi, m14_p2_b
    call print_cstr
    mov rdi, 0xFFFFFFFF80000000
    call m14_split
    mov rdi, m14_p2_c
    call print_cstr
    part_end

    file_screen m14_p3_hdr, m14_lm, m14_p3
    part m14_p4_hdr, m14_p4
    part m14_p5_hdr, m14_p5

    ; exercise(14)
    mov rdi, m14_exercise
    call print_cstr
    quiz m14_q1_text, m14_q1_answer, m14_q1_why
    quiz m14_q2_text, m14_q2_answer, m14_q2_why
    quiz m14_q3_text, m14_q3_answer, m14_q3_why
    challenge m14_challenge, m14_solution

    ; summary(14)
    summary m14_summary_hdr, m14_summary
    pop rbx
    ret

lesson_15_kernel:
    mov rdi, m15_title
    call title

    part m15_p1_hdr, m15_p1
    file_screen m15_p2_hdr, m15_boot, m15_p2
    file_screen m15_p3_hdr, m15_entry, m15_p3
    file_screen m15_p3b_hdr, m15_linker, m15_p3b
    file_screen m15_p4_hdr, m15_kernel, m15_p4
    part m15_p4b_hdr, m15_p4b
    part m15_p5_hdr, m15_p5

    ; exercise(15)
    mov rdi, m15_exercise
    call print_cstr
    quiz m15_q1_text, m15_q1_answer, m15_q1_why
    quiz m15_q2_text, m15_q2_answer, m15_q2_why
    quiz m15_q3_text, m15_q3_answer, m15_q3_why
    challenge m15_challenge, m15_solution

    ; summary(15)
    summary m15_summary_hdr, m15_summary
    ret

lesson_16_interrupts:
    mov rdi, m16_title
    call title

    part m16_p1_hdr, m16_p1
    file_screen m16_p2_hdr, m16_isr, m16_p2
    file_screen m16_p3_hdr, m16_idt, m16_p3
    file_screen m16_p4_hdr, m16_kmain, m16_p4
    part m16_p5_hdr, m16_p5

    ; exercise(16)
    mov rdi, m16_exercise
    call print_cstr
    quiz m16_q1_text, m16_q1_answer, m16_q1_why
    quiz m16_q2_text, m16_q2_answer, m16_q2_why
    quiz m16_q3_text, m16_q3_answer, m16_q3_why
    challenge m16_challenge, m16_solution

    ; summary(16)
    summary m16_summary_hdr, m16_summary
    ret

; Tells the linker this object does not need an executable stack.
section .note.GNU-stack noalloc noexec nowrite progbits
