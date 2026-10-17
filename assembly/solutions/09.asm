default rel
global asm_clamp

section .text
; int asm_clamp(int x, int lo, int hi): x in edi, lo in esi, hi in edx
asm_clamp:
    mov eax, edi            ; start from x
    cmp eax, esi
    jge .not_low
    mov eax, esi            ; x < lo: use lo
.not_low:
    cmp eax, edx
    jle .done
    mov eax, edx            ; x > hi: use hi
.done:
    ret

; Tells the linker this object does not need an executable stack.
section .note.GNU-stack noalloc noexec nowrite progbits
