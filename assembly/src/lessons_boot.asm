; lessons_boot.asm - modules 10 to 12: real mode, the boot sector, the hardware.
;
; The operating-system arc. The code these modules show is in os/, built
; into disk images and booted in QEMU by check-os.py; what the text quotes
; (screen contents, serial output, registers) was read back out of the
; emulator, not written from memory. The live lines in module 10 are
; computed by this program as you read.

%include "course.inc"

default rel

global lesson_10_boot
global lesson_11_hardware
global lesson_12_loader

section .rodata

; ---- module 10 ----------------------------------------------------
m10_title:   db "MODULE 10 - REAL MODE AND THE BOOT SECTOR", 0
m10_p1_hdr:  db "PART 1: what the machine does before your code runs", 0
m10_p2_hdr:  db "PART 2: os/boot1.asm, a whole operating system in 512 bytes", 0
m10_p3_hdr:  db "PART 3: real mode addressing: segment:offset", 0
m10_p4_hdr:  db "PART 4: four ways a boot sector fails, and what each looks like", 0
m10_p5_hdr:  db "PART 5: BIOS services, and why they are about to go away", 0

m10_p1:
    ln "  Power on. The CPU starts in REAL MODE: the 16-bit mode of the 8086,"
    ln "  with no operating system, no files, no memory manager and no libc."
    ln "  Firmware, the BIOS (many UEFI machines have a compatibility mode that"
    ln "  behaves the same way), runs a self test, picks a boot disk, and:"
    ln ""
    ln "    1. reads the disk's first sector, 512 bytes, to address 0x7C00"
    ln "    2. checks that its last two bytes are 0x55 and 0xAA"
    ln "    3. jumps to 0x7C00"
    ln ""
    ln "  That is the whole promise. Those 512 bytes are your operating system"
    ln "  until they load more of it. What you also inherit:"
    ln ""
    ln "    - a set of services called by software interrupt: int 0x10 is the"
    ln "      screen, 0x13 the disk, 0x15 memory and system, 0x16 the keyboard"
    ln "    - a machine in a messy state: the segment registers and the stack"
    ln "      are not set up for you, so the first job is to set them up"
    ln ""
    ln "  Everything in modules 10 to 16 runs in QEMU, an emulator of a PC. Its"
    ln "  firmware is SeaBIOS, and its banner is the first line on the screen,"
    ln "  read back below. The same files boot on real hardware, but nothing"
    ln "  here was run on any."
    db 0

m10_p2:
    ln ""
    ln "    $ nasm -f bin boot1.asm -o boot1.bin"
    ln "    $ qemu-system-x86_64 -drive format=raw,file=boot1.bin"
    ln ""
    ln "  The screen, read back out of the running emulator's video memory:"
    ln ""
    ln "    SeaBIOS (version ...)"
    ln "    Booting from Hard Disk..."
    ln "    Hello from a boot sector"
    ln ""
    ln "  And its registers a second later: CS = 0, EIP = 0x7C1E, CR0 = 0x10."
    ln "  EIP is inside our 512 bytes, in the hlt loop; bit 0 of CR0 is the"
    ln "  protection-enable bit, and it is 0: still real mode. The output"
    ln "  file is exactly 512 bytes."
    db 0

m10_p3:
    ln "  A real-mode register is 16 bits, so a plain address would reach only"
    ln "  64 KiB. The 8086 got a megabyte out of 16-bit registers by making an"
    ln "  address two of them: a segment and an offset."
    ln ""
    ln "    physical address = segment * 16 + offset"
    ln ""
    ln "  Many pairs name the same byte: 0x07C0:0x0000 and 0x0000:0x7C00 are"
    ln "  both the byte the BIOS loads us to. Computed by this lesson now:"
    ln ""
    db 0
m10_p3_a:   db "    0x07C0:0x0000  =  ", 0
m10_p3_b:   db "    0x0000:0x7C00  =  ", 0
m10_p3_c:   db "    0xB800:0x0000  =  ", 0
m10_p3_d:   db "    0xFFFF:0x000F  =  ", 0
m10_p3_e:   db "    0xFFFF:0x0010  =  ", 0
m10_p3_f:
    ln ""
    ln "  0xB800:0 is the screen (module 11). The last two are the top of the"
    ln "  megabyte: 0xFFFF:0x000F is the last byte, 0xFFFF:0x0010 is one past"
    ln "  it. On the 8086 that address wrapped round to 0; later CPUs can"
    ln "  reach it, which is what the A20 line in module 12 is about."
    ln ""
    ln "  Real mode still lets you use 32-bit registers, with a prefix byte."
    ln "  ndisasm -b 16 on the real bytes:"
    ln ""
    ln "    B80100            mov ax,0x1"
    ln "    66B801000000      mov eax,0x1"
    db 0

m10_p4:
    ln "  A boot sector either works or does nothing. Four ways to get nothing,"
    ln "  each built and booted for this module, and what the emulator showed:"
    ln ""
    ln "  1. No org 0x7C00. NASM then counts labels from 0, so mov si, message"
    ln "     loads a tiny number, not 0x7C1F. lodsb reads from the start of"
    ln "     memory, where the BIOS keeps its tables, and the screen shows a"
    ln "     couple of meaningless characters instead of the message."
    ln ""
    ln "  2. No 0x55 0xAA at the end. The BIOS does not even try:"
    ln ""
    ln "       Booting from Hard Disk..."
    ln "       Boot failed: not a bootable disk"
    ln ""
    ln "     and goes on to the floppy, the CD and the network."
    ln ""
    ln "  3. Too big. The padding line computes how many zeros are needed. With"
    ln "     69 bytes too much code NASM refuses:"
    ln ""
    ln "       error: TIMES value -69 is negative"
    ln ""
    ln "  4. Segments and stack never set up. The BIOS leaves ds, es and ss"
    ln "     undefined. A program that works on one machine and prints garbage"
    ln "     on another is usually this, so boot1.asm zeroes them, sets sp"
    ln "     just below its own code, and only then allows interrupts."
    db 0

m10_p5:
    ln "  int 0x10 with ah = 0x0E is 'teletype output': print the character in"
    ln "  al, advance the cursor, scroll if needed. It does not turn a newline"
    ln "  into a new line; the message ends in 13, 10 (carriage return, line"
    ln "  feed) for that. The BIOS has a call for most things a boot sector"
    ln "  needs:"
    ln ""
    ln "    int 0x10   screen: characters, cursor, video mode"
    ln "    int 0x13   disk: read and write sectors (module 12)"
    ln "    int 0x15   system: the memory map (module 12), the A20 line"
    ln "    int 0x16   keyboard: wait for a key, read it (module 11)"
    ln ""
    ln "  They are conveniences of real mode, and nothing more. The BIOS is"
    ln "  16-bit code living below 1 MiB; once the CPU leaves real mode for"
    ln "  protected or long mode (modules 13 and 14), calling int 0x10 means"
    ln "  executing 16-bit code in a 32- or 64-bit world, and it fails."
    ln "  From then on you talk to the hardware yourself: write characters"
    ln "  straight into video memory, read the keyboard's port, program the"
    ln "  disk controller. Module 11 does that first, while the BIOS is still"
    ln "  there to compare against."
    db 0

m10_summary_hdr: db "SUMMARY", 0
m10_summary:
    ln "   - the BIOS loads sector 0 to 0x7C00, wants 0x55 0xAA at the end, and"
    ln "     jumps there in 16-bit real mode with nothing set up"
    ln "   - address = segment * 16 + offset, one megabyte, 64 KiB at a time"
    ln "   - org 0x7C00 tells NASM where the code lives; without it every"
    ln "     label is wrong; times 510 - ($ - $$) db 0 and dw 0xAA55 finish it"
    ln "   - set ds, es, ss and sp first; hlt in a loop at the end"
    ln "   - int 0x10/0x13/0x15/0x16 are BIOS services that exist only here"
    db 0

m10_exercise:
    db `\n  >> EXERCISE - MODULE 10\n  ------------------------------------------------------\n`, 0
m10_q1_text:   db `At what address does the BIOS load the boot sector?\n  (hex, written like 0x1234)`, 0
m10_q1_answer: db "0x7c00", 0
m10_q1_why:    db `Physical address 0x7C00, by convention every PC BIOS follows.\n             org 0x7C00 tells the assembler so.`, 0
m10_q2_text:   db `Which two bytes must end a boot sector?\n  (two hex bytes with a space between, like 12 ab)`, 0
m10_q2_answer: db "55 aa", 0
m10_q2_why:    db `0x55 then 0xAA, which dw 0xAA55 produces because x86 is\n             little-endian (module 5). Without them the BIOS skips the disk.`, 0
m10_q3_text:   db `Real-mode address 0x2000:0x0123 is which physical address?\n  (hex, written like 0x1234)`, 0
m10_q3_answer: db "0x20123", 0
m10_q3_why:    db `segment * 16 + offset: 0x2000 * 16 is 0x20000, and 0x123 more\n             is 0x20123.`, 0

m10_challenge:
    ln ""
    ln "  >> WRITE THIS YOURSELF, in a real file"
    ln ""
    ln "  Write a boot sector that prints OK and then halts. Set up ds, ss"
    ln "  and sp, use int 0x10 with ah = 0x0E, and finish with the padding"
    ln "  and the signature. Two things are checked: the output file is"
    ln "  exactly 512 bytes, and OK appears on the emulated screen."
    ln ""
    ln "  Assemble and boot with:"
    ln "    nasm -f bin boot.asm -o boot.bin && ls -l boot.bin"
    ln "    qemu-system-x86_64 -drive format=raw,file=boot.bin"
    ln ""
    ln "  It must show, below the firmware's own lines:"
    ln ""
    ln "      OK"
    db 0

m10_solution:
    incbin "10.asm"
    db 0
m10_boot1:
    incbin "boot1.asm"
    db 0

; ---- module 11 ----------------------------------------------------
m11_title:   db "MODULE 11 - TALKING TO THE HARDWARE", 0
m11_p1_hdr:  db "PART 1: ports: a second address space, and the serial line", 0
m11_p2_hdr:  db "PART 2: os/boot2.asm", 0
m11_p3_hdr:  db "PART 3: video memory is just memory", 0
m11_p4_hdr:  db "PART 4: the keyboard, and what a key really is", 0
m11_p5_hdr:  db "PART 5: three ways to write to the screen and get nothing", 0

m11_p1:
    ln "  Besides memory, the x86 has a second, separate address space of 65536"
    ln "  I/O PORTS, reached by two instructions and nothing else:"
    ln ""
    ln "    out dx, al       ; write the byte in al to port number dx"
    ln "    in  al, dx       ; read a byte from port dx"
    ln ""
    ln "  Devices sit at agreed numbers. The first serial port, COM1, owns eight"
    ln "  of them from 0x3F8: the data register is 0x3F8 itself, and the line"
    ln "  status register is 0x3FD, whose bit 5 says 'the transmitter can take"
    ln "  another byte'. Sending a character is always the same two steps:"
    ln ""
    ln "    mov dx, 0x3FD / .wait: in al, dx / test al, 0x20 / jz .wait"
    ln "    mov dx, 0x3F8 / mov al, 'x' / out dx, al"
    ln ""
    ln "  Before that the port needs setting up once (a speed divisor and 8 data"
    ln "  bits, no parity, one stop bit: serial_init does it). It is worth the"
    ln "  trouble because QEMU can send the port to a file or to your terminal,"
    ln "  so serial_print is printf for an operating system that has nothing"
    ln "  else yet. It also keeps working in protected and long mode, where the"
    ln "  BIOS does not."
    db 0

m11_p2:
    ln ""
    ln "    $ nasm -f bin boot2.asm -o boot2.bin"
    ln "    $ qemu-system-x86_64 -drive format=raw,file=boot2.bin -serial stdio"
    ln "    boot2 ready"
    ln ""
    ln "  The 'boot2 ready' arrives in the terminal that started QEMU. Another"
    ln "  512-byte file; the extra length is only the serial routines."
    db 0

m11_p3:
    ln "  The text screen is 80 columns by 25 rows, and it is plain memory: 2000"
    ln "  cells starting at physical address 0xB8000, two bytes each:"
    ln ""
    ln "    byte 0   the character code"
    ln "    byte 1   the attribute: (background << 4) | foreground"
    ln ""
    ln "    colours   0 black   1 blue   2 green   4 red   7 light grey"
    ln "              0xA light green   0xF white"
    ln ""
    ln "  Cell (row, column) is at 0xB8000 + (row * 80 + column) * 2. In real"
    ln "  mode that needs a segment: es = 0xB800, then es:di. stosw stores ax"
    ln "  with al at the LOWER address, so al is the character and ah the"
    ln "  attribute: byte order from module 5 again."
    ln ""
    ln "  boot2 clears the screen with rep stosw (module 8): 0x0720 is a space"
    ln "  in light grey on black, 2000 times. It then draws a banner at row 1,"
    ln "  column 2 with attribute 0x1F. Read back out of QEMU's video memory:"
    ln ""
    ln "    row 0    empty (SeaBIOS's banner is gone)"
    ln "    row 1    'Type something:' starting at column 2, attribute 0x1F"
    ln ""
    ln "  The BIOS call of module 10 and this memory are the same screen. The"
    ln "  BIOS call just does the arithmetic for you."
    db 0

m11_p4:
    ln "  int 0x16 with ah = 0 waits for a key and returns two things:"
    ln ""
    ln "    al   the ASCII character        ah   the SCAN CODE"
    ln ""
    ln "  A scan code names a physical key, not a letter: the keyboard reports"
    ln "  which key went down and a table turns that into text, which is why"
    ln "  the same key can mean different characters in different layouts."
    ln "  boot2 sends both to the serial port. The keys h, i and a, pressed in"
    ln "  QEMU with its sendkey command, produced:"
    ln ""
    ln "    h:23"
    ln "    i:17"
    ln "    a:1E"
    ln ""
    ln "  and hia appeared on row 3 in light green (attribute 0x0A)."
    ln ""
    ln "  The BIOS is doing the real work. It handles the keyboard interrupt,"
    ln "  reads the keyboard controller's port 0x60, and looks the code up."
    ln "  When the CPU leaves real mode nothing does that for you: you install"
    ln "  an interrupt handler, read port 0x60 yourself, and own the table"
    ln "  (module 16)."
    db 0

m11_p5:
    ln "  The screen is the first place an operating system fails silently."
    ln "  Three ways to write to it and see nothing, each reproduced:"
    ln ""
    ln "  1. A linear address where a segment goes. The screen is at physical"
    ln "     0xB8000 but the segment register holds 0xB800 (module 10: times"
    ln "     16). mov ax, 0xB8000 is a 20-bit number in a 16-bit register, and"
    ln "     NASM says so, once, as a warning:"
    ln ""
    ln "       warning: word exceeds bounds [-w+number-overflow]"
    ln ""
    ln "     The warning scrolls past in the build output; the program still"
    ln "     assembles, with the wrong segment."
    ln ""
    ln "  2. Attribute 0. Black on black. boot2 with the banner's ah set to 0:"
    ln "     the characters 'Type something:' are in video memory, read back from"
    ln "     it one by one, and every one of the cells has attribute 0x00. The"
    ln "     text is there. You just cannot see it."
    ln ""
    ln "  3. A forgotten es. stosw writes at es:di, not ds:di. With es still"
    ln "     pointing at the start of memory, the screen stays empty and the"
    ln "     characters land in the BIOS's own tables."
    ln ""
    ln "  Reading the screen back from the emulator, as this module's checker"
    ln "  does, would catch the first two; a debugger would catch the third."
    db 0

m11_summary_hdr: db "SUMMARY", 0
m11_summary:
    ln "   - out/in talk to I/O ports; COM1 is 0x3F8, status at 0x3FD bit 5"
    ln "   - the serial port is the first debugging output an OS has, and QEMU"
    ln "     can send it to your terminal"
    ln "   - the text screen is memory at 0xB8000: character byte, attribute"
    ln "     byte, row * 80 + column cells in; attribute 0 is invisible"
    ln "   - in real mode a segment register holds the address divided by 16,"
    ln "     so the screen is es = 0xB800, not 0xB8000"
    ln "   - int 0x16 gives a character and a scan code; the BIOS does the"
    ln "     keyboard work that you do yourself from module 16"
    db 0

m11_exercise:
    db `\n  >> EXERCISE - MODULE 11\n  ------------------------------------------------------\n`, 0
m11_q1_text:   db `How many bytes does one text-mode screen cell take?\n  (a number)`, 0
m11_q1_answer: db "2", 0
m11_q1_why:    db `One byte for the character, one for the attribute, 2000 cells\n             for an 80 by 25 screen: 4000 bytes from 0xB8000.`, 0
m11_q2_text:   db `stosw stores ax. Which half goes to the lower address, al or\n  ah? (two letters)`, 0
m11_q2_answer: db "al", 0
m11_q2_why:    db `Little-endian: the low byte first. In a screen cell the low byte\n             is the character, so al = character and ah = attribute.`, 0
m11_q3_text:   db `What port address is COM1, the first serial port?\n  (hex, written like 0x1234)`, 0
m11_q3_answer: db "0x3f8", 0
m11_q3_why:    db `0x3F8 for the data register, with the other seven registers of the\n             port following it; 0x3FD is the line status.`, 0

m11_challenge:
    ln ""
    ln "  >> WRITE THIS YOURSELF, in a real file"
    ln ""
    ln "  Write a boot sector that clears the whole text screen with rep"
    ln "  stosw and then writes a red X (attribute 0x04) at row 5, column 10,"
    ln "  directly into video memory. No BIOS calls. Mind the segment and"
    ln "  the direction flag. Checked by reading the emulated video memory."
    ln ""
    ln "  Assemble and boot with:"
    ln "    nasm -f bin boot.asm -o boot.bin"
    ln "    qemu-system-x86_64 -drive format=raw,file=boot.bin"
    ln ""
    ln "  It must show, with nothing else on the screen:"
    ln ""
    ln "      (row 5, column 10)  X   in red"
    db 0

m11_solution:
    incbin "11.asm"
    db 0
m11_boot2:
    incbin "boot2.asm"
    db 0

; ---- module 12 ----------------------------------------------------
m12_title:   db "MODULE 12 - LOADING MORE CODE: TWO STAGES, A20, THE MEMORY MAP", 0
m12_p1_hdr:  db "PART 1: 512 bytes are not enough", 0
m12_p2_hdr:  db "PART 2: os/stage1.asm", 0
m12_p3_hdr:  db "PART 3: os/stage2.asm", 0
m12_p4_hdr:  db "PART 4: the A20 line, and a bit that resets the machine", 0
m12_p5_hdr:  db "PART 5: the memory map: ask, do not assume", 0

m12_p1:
    ln "  Module 10's boot sector used 60 of its 510 bytes. A real loader has"
    ln "  much more to do: find the kernel on a disk, check the machine, switch"
    ln "  CPU modes, build tables. The usual answer is two stages:"
    ln ""
    ln "    stage 1   the boot sector. It knows how to do exactly one thing:"
    ln "              read sectors from the disk the BIOS booted from."
    ln "    stage 2   read by stage 1 into free memory, with room to grow."
    ln ""
    ln "  The picture of low memory when stage 1 hands over:"
    ln ""
    ln "    0x00000   the BIOS's interrupt table and data"
    ln "    0x07C00   stage 1, 512 bytes, put there by the BIOS"
    ln "    0x07E00   stage 2, put there by stage 1"
    ln "    0x9FC00   the top of the memory the BIOS reports as usable here"
    ln "    0xA0000   video memory and ROMs: not RAM you can use"
    ln ""
    ln "  On the disk it is just the sectors in a row: sector 0 is stage 1,"
    ln "  sectors 1 and 2 are stage 2, and cat joins two files into that:"
    ln ""
    ln "    cat stage1.bin stage2.bin > os12.img"
    ln ""
    ln "  The BIOS tells stage 1 which disk it booted from in the dl register,"
    ln "  and a hard disk is 0x80. That number has to be handed on."
    db 0

m12_p2:
    ln ""
    ln "  int 0x13 function 0x42 reads by LBA, the plain sector number counted"
    ln "  from 0, which is all anyone wants. Its orders are a small packet in"
    ln "  memory (ds:si points at it): how many sectors, where to put them,"
    ln "  and which one to start from. The carry flag comes back set on a"
    ln "  failure, with the reason in ah."
    ln ""
    ln "  Two lines are worth reading twice. mov [boot_drive], dl saves the"
    ln "  drive number before anything else touches dl. And the jump to stage"
    ln "  2 is a far jump, jmp 0x0000:0x7E00, which loads cs as well as ip."
    db 0

m12_p3:
    ln ""
    ln "  The first two serial lines of this image: 'stage 2 loaded' and"
    ln "  'boot drive: 0x80'. Stage 1 handed over the drive in dl and it"
    ln "  survived. The rest are the next two screens; the serial routines are"
    ln "  module 11's, in os/serial16.inc. With dl forced to 0, a floppy that"
    ln "  does not exist, the read fails and stage 1 prints, on the screen:"
    ln ""
    ln "    Disk read error"
    db 0

m12_p4:
    ln "  The 8086 had 20 address lines, so address 0xFFFF:0x0010 (module 10)"
    ln "  wrapped round to the bottom of memory, and some programs counted on"
    ln "  it. When later CPUs gained a 21st line, A20, the PC put a gate on it,"
    ln "  closed by default, so those programs kept working. Code above 1 MiB"
    ln "  needs the gate open, and that includes the kernel from module 15."
    ln ""
    ln "  stage2.asm tests it by writing different values to physical 0x000500"
    ln "  and to 0xFFFF:0x0510 (physical 0x100500); if the second write changed"
    ln "  the first, the two addresses are one and the line is off. It then"
    ln "  opens the gate through port 0x92, the 'fast A20' control. Serial:"
    ln ""
    ln "    A20 usable before enabling: yes"
    ln "    A20 usable after enabling:  yes"
    ln ""
    ln "  Both yes: SeaBIOS had already opened it. To see the test work, a"
    ln "  variant switches the gate OFF first (clearing bit 1 of the port):"
    ln ""
    ln "    A20 usable before enabling: no"
    ln "    A20 usable after enabling:  yes"
    ln ""
    ln "  One more bit in that port matters more than it looks. Bit 1 is the"
    ln "  gate; bit 0 is a fast RESET of the CPU. Another variant sets it, and"
    ln "  QEMU, started with -no-reboot, simply quit the moment it ran. That is"
    ln "  why stage2.asm masks bit 0 off after setting bit 1."
    db 0

m12_p5:
    ln "  How much memory is there, and which of it may you use? The BIOS knows"
    ln "  and int 0x15 with eax = 0xE820 tells you, one range per call,"
    ln "  returning a cookie in ebx to ask for the next. Each entry is a base,"
    ln "  a length and a type: 1 usable, 2 reserved, 3 ACPI reclaimable, 4 ACPI"
    ln "  NVS, 5 bad. stage2.asm prints them. QEMU's default machine (128 MiB):"
    ln ""
    ln "    base 0x0000000000000000  length 0x000000000009FC00  type 1"
    ln "    base 0x000000000009FC00  length 0x0000000000000400  type 2"
    ln "    base 0x00000000000F0000  length 0x0000000000010000  type 2"
    ln "    base 0x0000000000100000  length 0x0000000007EE0000  type 1"
    ln "    base 0x0000000007FE0000  length 0x0000000000020000  type 2"
    ln "    base 0x00000000FFFC0000  length 0x0000000000040000  type 2"
    ln "    base 0x000000FD00000000  length 0x0000000300000000  type 2"
    ln "    usable bytes (low 32 bits): 0x07F7FC00"
    ln ""
    ln "  Two usable ranges, with holes around the screen and the ROMs: the"
    ln "  first one stops at 0x9FC00, not 0xA0000, because the BIOS keeps its"
    ln "  own data in the 1 KiB above it. 0x07F7FC00 is the two ranges added,"
    ln "  about 127.5 MiB of the 128. An operating system that assumes RAM runs"
    ln "  unbroken from 1 MiB upward writes into a ROM's address, or the video"
    ln "  buffer, and finds out later and elsewhere. It asks once, here, in"
    ln "  real mode; the BIOS call is gone after module 13."
    db 0

m12_summary_hdr: db "SUMMARY", 0
m12_summary:
    ln "   - stage 1 reads sectors with int 0x13 function 0x42 (an LBA read)"
    ln "     and jumps to stage 2; the BIOS gives the boot drive in dl, and"
    ln "     stage 1 must save it before using dl for anything"
    ln "   - cat joins the stages into a disk image; sector counts are agreed"
    ln "     by constant, and times keeps stage 2 to the size stage 1 reads"
    ln "   - A20 must be on above 1 MiB: test by aliasing, open via port 0x92,"
    ln "     and never write 1 to bit 0 of that port"
    ln "   - int 0x15 eax=0xE820 lists memory ranges and types; ask, do not"
    ln "     assume"
    db 0

m12_exercise:
    db `\n  >> EXERCISE - MODULE 12\n  ------------------------------------------------------\n`, 0
m12_q1_text:   db `Which register does the BIOS use to pass the boot drive number\n  to the boot sector? (one register)`, 0
m12_q1_answer: db "dl", 0
m12_q1_why:    db `dl, with 0x80 for the first hard disk. Anything that reuses dl\n             before saving it loses the number stage 2 will need.`, 0
m12_q2_text:   db `int 0x13 function 0x42 addresses a sector by its plain number\n  from 0. What is that kind of address called? (three letters)`, 0
m12_q2_answer: db "lba", 0
m12_q2_why:    db `Logical Block Address: sector 0 is the boot sector, 1 the next.\n             The older CHS form needs the drive's geometry as well.`, 0
m12_q3_text:   db `Which bit of port 0x92 resets the CPU when set? (a number)`, 0
m12_q3_answer: db "0", 0
m12_q3_why:    db `Bit 0 is the fast reset. Bit 1 is the A20 gate, so setting A20 is\n             or al, 2 with bit 0 kept clear.`, 0

m12_challenge:
    ln ""
    ln "  >> WRITE THIS YOURSELF, in a real file"
    ln ""
    ln "  One file, two sectors. The first is a boot sector that loads the"
    ln "  sector after it to 0x7E00 with int 0x13 function 0x42 and jumps"
    ln "  there. The second, which follows the first in the same file, writes"
    ln "  the line 'two' to the serial port, COM1, and halts. In a file with"
    ln "  org 0x7C00, the label after the signature is already at 0x7E00."
    ln ""
    ln "  Assemble and boot with:"
    ln "    nasm -f bin boot.asm -o boot.bin"
    ln "    qemu-system-x86_64 -drive format=raw,file=boot.bin -serial stdio"
    ln ""
    ln "  It must print, in the terminal that started QEMU:"
    ln ""
    ln "      two"
    db 0

m12_solution:
    incbin "12.asm"
    db 0
m12_stage1:
    incbin "stage1.asm"
    db 0
m12_stage2:
    incbin "stage2.asm"
    db 0

section .text

; m10_phys(rdi=segment, rsi=offset) -> rax = segment * 16 + offset
m10_phys:
    mov rax, rdi
    shl rax, 4
    add rax, rsi
    ret

lesson_10_boot:
    push rbx
    mov rdi, m10_title
    call title

    part m10_p1_hdr, m10_p1
    file_screen m10_p2_hdr, m10_boot1, m10_p2

    part_begin m10_p3_hdr, m10_p3
    mov rdi, 0x07C0
    mov rsi, 0x0000
    call m10_phys
    live_hex m10_p3_a, rax
    mov rdi, 0x0000
    mov rsi, 0x7C00
    call m10_phys
    live_hex m10_p3_b, rax
    mov rdi, 0xB800
    mov rsi, 0x0000
    call m10_phys
    live_hex m10_p3_c, rax
    mov rdi, 0xFFFF
    mov rsi, 0x000F
    call m10_phys
    live_hex m10_p3_d, rax
    mov rdi, 0xFFFF
    mov rsi, 0x0010
    call m10_phys
    live_hex m10_p3_e, rax
    mov rdi, m10_p3_f
    call print_cstr
    part_end

    part m10_p4_hdr, m10_p4
    part m10_p5_hdr, m10_p5

    ; exercise(10)
    mov rdi, m10_exercise
    call print_cstr
    quiz m10_q1_text, m10_q1_answer, m10_q1_why
    quiz m10_q2_text, m10_q2_answer, m10_q2_why
    quiz m10_q3_text, m10_q3_answer, m10_q3_why
    challenge m10_challenge, m10_solution

    ; summary(10)
    summary m10_summary_hdr, m10_summary
    pop rbx
    ret

lesson_11_hardware:
    mov rdi, m11_title
    call title

    part m11_p1_hdr, m11_p1
    file_screen m11_p2_hdr, m11_boot2, m11_p2
    part m11_p3_hdr, m11_p3
    part m11_p4_hdr, m11_p4
    part m11_p5_hdr, m11_p5

    ; exercise(11)
    mov rdi, m11_exercise
    call print_cstr
    quiz m11_q1_text, m11_q1_answer, m11_q1_why
    quiz m11_q2_text, m11_q2_answer, m11_q2_why
    quiz m11_q3_text, m11_q3_answer, m11_q3_why
    challenge m11_challenge, m11_solution

    ; summary(11)
    summary m11_summary_hdr, m11_summary
    ret

lesson_12_loader:
    mov rdi, m12_title
    call title

    part m12_p1_hdr, m12_p1
    file_screen m12_p2_hdr, m12_stage1, m12_p2
    file_screen m12_p3_hdr, m12_stage2, m12_p3
    part m12_p4_hdr, m12_p4
    part m12_p5_hdr, m12_p5

    ; exercise(12)
    mov rdi, m12_exercise
    call print_cstr
    quiz m12_q1_text, m12_q1_answer, m12_q1_why
    quiz m12_q2_text, m12_q2_answer, m12_q2_why
    quiz m12_q3_text, m12_q3_answer, m12_q3_why
    challenge m12_challenge, m12_solution

    ; summary(12)
    summary m12_summary_hdr, m12_summary
    ret

; Tells the linker this object does not need an executable stack.
section .note.GNU-stack noalloc noexec nowrite progbits
