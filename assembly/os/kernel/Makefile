# Build the C kernel image.   make   ->   os.img     make run   ->   boot it in QEMU
#
# -ffreestanding            no standard library is assumed to exist
# -fno-pie -no-pie          one fixed address: nothing fixes up a PIE at load time
# -fno-stack-protector      the protector needs a canary supplied by libc
# -mno-red-zone             an interrupt would overwrite the 128 bytes below rsp
# -mgeneral-regs-only       no SSE until the kernel has enabled it (see module 15)
CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -ffreestanding -fno-pie -fno-stack-protector \
          -mno-red-zone -mgeneral-regs-only -fno-asynchronous-unwind-tables
SECTORS = 16                    # must equal KERNEL_SECTORS in boot.asm

os.img: boot.bin stage2.bin kernel.bin
	cat boot.bin stage2.bin kernel.bin > $@

boot.bin: boot.asm
	nasm -f bin $< -o $@

stage2.bin: stage2.asm
	nasm -f bin $< -o $@

entry.o: entry.asm
	nasm -f elf64 $< -o $@

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel.elf: entry.o kernel.o linker.ld
	ld --no-warn-rwx-segments -T linker.ld -o $@ entry.o kernel.o

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
