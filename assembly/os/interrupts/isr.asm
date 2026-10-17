; isr.asm - the entry points the CPU jumps to on an interrupt or exception.
;
; The CPU saves rip, cs, rflags, rsp and ss on the stack, and for SOME
; exceptions an error code as well, then jumps through the IDT to one of
; these stubs. Each stub pushes what is missing so every vector leaves the
; same frame, saves the registers, and calls one C function with a pointer
; to it. Frame, from the top of the stack down:
;
;     r15 .. r8, rbp, rdi, rsi, rdx, rcx, rbx, rax    (pushed here)
;     vector, error code                              (pushed here / by the CPU)
;     rip, cs, rflags, rsp, ss                        (pushed by the CPU)

bits 64

extern interrupt_handler
global isr_table

; Exceptions 8, 10-14 and 17 push an error code; the rest do not.
%macro isr_stub 2               ; vector, 1 if the CPU pushes an error code
isr%1:
%if %2 == 0
    push 0                      ; a fake error code, to keep the frame the same
%endif
    push %1
    jmp isr_common
%endmacro

section .text

%assign i 0
%rep 48                         ; 32 CPU exceptions, then 16 PIC interrupts
%if i == 8 || (i >= 10 && i <= 14) || i == 17
    isr_stub i, 1
%else
    isr_stub i, 0
%endif
%assign i i + 1
%endrep

isr_common:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov rdi, rsp                ; argument: a pointer to the frame
    mov rbp, rsp                ; remember where the frame is...
    and rsp, -16                ; ...and align the stack for the call
    call interrupt_handler
    mov rsp, rbp

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    add rsp, 16                 ; drop the vector and the error code
    iretq                       ; pops rip, cs, rflags, rsp, ss and resumes

section .data
isr_table:                      ; the C side reads the stubs' addresses from here
%assign i 0
%rep 48
    dq isr %+ i
%assign i i + 1
%endrep
