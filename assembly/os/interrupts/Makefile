# The kernel of module 16: module 15's boot path, with interrupts.
#   make   ->   os.img      make run   ->   boot it in QEMU, serial on stdio
#
# boot.asm, stage2.asm, entry.asm and linker.ld are module 15's, unchanged.
KERNEL  = ../kernel
CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -ffreestanding -fno-pie -fno-stack-protector \
          -mno-red-zone -mgeneral-regs-only -fno-asynchronous-unwind-tables
OBJECTS = entry.o isr.o console.o idt.o kmain.o
SECTORS = 16                    # must equal KERNEL_SECTORS in boot.asm

os.img: boot.bin stage2.bin kernel.bin
	cat boot.bin stage2.bin kernel.bin > $@

boot.bin: $(KERNEL)/boot.asm
	nasm -f bin $< -o $@

stage2.bin: $(KERNEL)/stage2.asm
	nasm -f bin $< -o $@

entry.o: $(KERNEL)/entry.asm
	nasm -f elf64 $< -o $@

isr.o: isr.asm
	nasm -f elf64 $< -o $@

%.o: %.c console.h
	$(CC) $(CFLAGS) -c $< -o $@

kernel.elf: $(OBJECTS) $(KERNEL)/linker.ld
	ld --no-warn-rwx-segments -T $(KERNEL)/linker.ld -o $@ $(OBJECTS)

kernel.bin: kernel.elf
	objcopy -O binary $< $@
	@size=$$(stat -c %s $@); max=$$(( $(SECTORS) * 512 )); \
	  if [ $$size -gt $$max ]; then echo "kernel is $$size bytes, boot.asm reads $$max"; rm $@; exit 1; fi; \
	  truncate -s $$max $@

run: os.img
	qemu-system-x86_64 -drive format=raw,file=os.img -serial stdio

clean:
	rm -f *.bin *.o *.elf os.img

.PHONY: run clean
