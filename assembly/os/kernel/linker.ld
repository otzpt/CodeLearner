/* linker.ld - lay the kernel out as a flat binary that starts at 0x10000. */

ENTRY(_start)

SECTIONS
{
    . = 0x10000;

    .text   : { *(.text.entry) *(.text .text.*) }
    .rodata : { *(.rodata .rodata.*) }
    .data   : { *(.data .data.*) }

    __bss_start = .;            /* entry.asm zeroes everything from here... */
    .bss    : { *(COMMON) *(.bss .bss.*) }
    __bss_end = .;              /* ...to here */

    /DISCARD/ : { *(.note*) *(.comment) *(.eh_frame*) }
}
