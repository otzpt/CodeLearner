; strings_pt.asm - Portuguese (PT-PT) text for the menus, prompts and summaries.
; CODELEARNER_LANG=pt-pt turns it on. The lessons themselves stay in English.
;
; translate compares strings rather than addresses, so nothing outside this
; file has to export a label for its text.

default rel

global detect_language
global translate
global print_tr
global summary_text

extern print_cstr

section .rodata

language_setting: db "CODELEARNER_LANG=pt-pt", 0

text_table:
    dq en_0, pt_0
    dq en_1, pt_1
    dq en_2, pt_2
    dq en_3, pt_3
    dq en_4, pt_4
    dq en_5, pt_5
    dq en_6, pt_6
    dq en_7, pt_7
    dq en_8, pt_8
    dq en_9, pt_9
    dq en_10, pt_10
    dq en_11, pt_11
    dq en_12, pt_12
    dq en_13, pt_13
    dq en_14, pt_14
    dq en_15, pt_15
    dq en_16, pt_16
    dq en_17, pt_17
    dq en_18, pt_18
    dq en_19, pt_19
    dq en_20, pt_20
    dq en_21, pt_21
    dq en_22, pt_22
    dq en_23, pt_23
    dq en_24, pt_24
    dq en_25, pt_25
    dq en_26, pt_26
    dq en_27, pt_27
    dq en_28, pt_28
    dq en_29, pt_29
    dq 0

en_0: db "Registers, syscalls, your first program", 0
pt_0: db "Registos, syscalls, o teu primeiro programa", 0
en_1: db "Comparing and branching: if/else", 0
pt_1: db "Comparar e saltar: if/else", 0
en_2: db "Loops", 0
pt_2: db "Ciclos", 0
en_3: db "Reading input, and a guessing game", 0
pt_3: db "Ler entrada, e um jogo de adivinhar", 0
en_4: db "Memory and addressing", 0
pt_4: db "Memória e endereçamento", 0
en_5: db "The stack, and calling a function", 0
pt_5: db "A pilha, e chamar uma função", 0
en_6: db "Bits, flags and arithmetic", 0
pt_6: db "Bits, flags e aritmética", 0
en_7: db "Strings and block instructions", 0
pt_7: db "Strings e instruções de bloco", 0
en_8: db "Assembly and C together", 0
pt_8: db "Assembly e C juntos", 0
en_9: db "Real mode and the boot sector", 0
pt_9: db "Modo real e o setor de arranque", 0
en_10: db "Talking to the hardware", 0
pt_10: db "Falar com o hardware", 0
en_11: db "Loading more code: two stages, A20, the memory map", 0
pt_11: db "Carregar mais código: duas fases, A20, o mapa de memória", 0
en_12: db "Protected mode", 0
pt_12: db "Modo protegido", 0
en_13: db "Long mode and paging", 0
pt_13: db "Modo longo e paginação", 0
en_14: db "A kernel in C", 0
pt_14: db "Um kernel em C", 0
en_15: db "Interrupts", 0
pt_15: db "Interrupções", 0
en_16: db "SUMMARY", 0
pt_16: db "RESUMO", 0
en_17: db "BASIC", 0
pt_17: db "BÁSICO", 0
en_18: db "INTERMEDIATE", 0
pt_18: db "INTERMÉDIO", 0
en_19: db "ADVANCED", 0
pt_19: db "AVANÇADO", 0
en_20: db "ASSEMBLY COURSE - REGISTERS TO AN OPERATING SYSTEM", 0
pt_20: db "CURSO DE ASSEMBLY", 0
en_21: db 10, "  Press ENTER to continue...", 0
pt_21: db 10, "  Prime ENTER para continuar...", 0
en_22: db 10, "  Your answer: ", 0
pt_22: db 10, "  A tua resposta: ", 0
en_23: db 10, "  CORRECT.  ", 0
pt_23: db 10, "  CERTO.  ", 0
en_24: db 10, "  NOT QUITE. The answer is: ", 0
pt_24: db 10, "  NÃO É BEM ISSO. A resposta é: ", 0
en_25: db "Want to see example code?", 0
pt_25: db "Queres ver código de exemplo?", 0
en_26: db 10, "   [ 0]  Quit", 10, 0
pt_26: db 10, "   [ 0]  Sair", 10, 0
en_27: db 10, "  Pick a module: ", 0
pt_27: db 10, "  Escolhe um módulo: ", 0
en_28: db 10, "  Not a valid option.", 10, 0
pt_28: db 10, "  Opção inválida.", 10, 0
en_29: db 10, "  See you next time.", 10, 10, 0
pt_29: db 10, "  Até à próxima.", 10, 10, 0

summaries:
    dq 0
    dq summary_1
    dq summary_2
    dq summary_3
    dq summary_4
    dq summary_5
    dq summary_6
    dq summary_7
    dq summary_8
    dq summary_9
    dq summary_10
    dq summary_11
    dq summary_12
    dq summary_13
    dq summary_14
    dq summary_15
    dq summary_16

summary_1:
    db db "   - sintaxe Intel: destino primeiro, depois origem: mov dst, src;", 10
    db db "     a ordem AT&T que gcc -S mostra por omissão é a inversa", 10
    db db "   - os seis primeiros argumentos inteiros/apontadores vão em rdi,", 10
    db db "     rsi, rdx, rcx, r8, r9: não na pilha, só a partir do 7.o", 10
    db db "   - um syscall é um número em rax mais os argumentos nos mesmos", 10
    db db "     registos de uma chamada de função, depois a instrução", 10
    db db "     syscall: sem precisar de libc para nada disto", 10
    db db "   - a saída -O0 de um compilador não é uma caixa negra: push rbp /", 10
    db db "     mov rbp, rsp é o mesmo stack frame que o ui.asm deste curso", 10
    db db "     escreve à mão", 10
    db db 10
    db db "  Não é fluência: é o suficiente para reconhecer no que o teu C", 10
    db db "  compilado se transforma, e para que objdump -d num programa", 10
    db db "  teu seja legível em vez de ruído.", 10
    db 0

summary_2:
    db db "   - cmp calcula uma subtração e guarda só as flags; o salto a", 10
    db db "     seguir é que as lê", 10
    db db "   - cmp rax, 10 é rax - 10, não 10 - rax: o primeiro operando é o comparado", 10
    db db "   - jl/jg têm sinal, jb/ja não têm; o assembler não te diz qual", 10
    db db "     delas o valor no registo queria", 10
    db db "   - um if é um salto na condição INVERTIDA, mais um jmp no fim", 10
    db db "     do corpo do if para não cair no do else", 10
    db db "   - test rax, rax é a forma idiomática de perguntar ", 34, "é zero?", 34, 10
    db 0

summary_3:
    db db "   - um ciclo é uma etiqueta, um corpo e um salto condicional de volta", 10
    db db "   - testado em cima corre o corpo zero vezes com contagem zero;", 10
    db db "     testado em baixo corre sempre pelo menos uma, com um salto a menos", 10
    db db "   - dec define ZF, por isso contar até zero não precisa de cmp", 10
    db db "   - a instrução loop é mais lenta do que dec/jnz e fixa rcx, que", 10
    db db "     qualquer call no corpo pode destruir", 10
    db db "   - um contador que sobrevive a um call vai em rbx ou r12-r15", 10
    db db "   - (base, índice, escala) indexa um array num só operando", 10
    db 0

summary_4:
    db db "   - read é o syscall 0 no fd 0, e rax volta com uma contagem, 0", 10
    db db "     em EOF, ou um errno negativo", 10
    db db "   - read não escreve terminador e mantém o fim de linha; pedir", 10
    db db "     tamanho - 1 deixa espaço para terminar com segurança", 10
    db db "   - os dígitos são bytes: subtrai '0' para ter o valor, soma '0'", 10
    db db "     para voltar ao carácter", 10
    db db "   - n = n * 10 + dígito, parando no primeiro byte fora de '0'-'9'", 10
    db db "   - xor rdx, rdx antes de cada div, ou um #DE mata o processo", 10
    db db "   - valores que sobrevivem a um syscall vão em r12-r15, não em rcx", 10
    db 0

summary_5:
    db db "   - uma etiqueta é um endereço: mov rax, etiqueta carrega-o, mov rax,", 10
    db db "     [etiqueta] carrega o que lá está", 10
    db db "   - .text corre, .rodata é só de leitura, .data e .bss escrevem-se;", 10
    db db "     .bss vem a zeros e não ocupa espaço no ficheiro", 10
    db db "   - guardar um número em memória precisa de indicar o tamanho: sem", 10
    db db "     isso o NASM 3.01 guarda um único byte", 10
    db db "   - escrever em eax limpa a metade de cima de rax; escrever em ax", 10
    db db "     ou al não toca no resto", 10
    db db "   - [base + índice*escala + deslocamento] com escala 1, 2, 4 ou 8;", 10
    db db "     lea calcula o endereço e não carrega nada", 10
    db db "   - x86 é little-endian: o byte baixo fica no endereço mais baixo", 10
    db 0

summary_6:
    db db "   - push subtrai 8 a rsp e guarda; pop carrega e soma 8; a pilha", 10
    db db "     cresce para endereços mais baixos e pop não apaga nada", 10
    db db "   - call empilha o endereço de retorno e salta; ret tira-o para rip;", 10
    db db "     um push ou pop a mais faz o ret ir para onde não devia", 10
    db db "   - rsp é múltiplo de 16 num call, por isso fica desfasado 8 lá dentro", 10
    db db "   - um frame é push rbp / mov rbp, rsp / sub rsp, N ... leave / ret;", 10
    db db "     os locais são [rbp-8].., o 7.o argumento é [rbp+16]", 10
    db db "   - rbx, rbp, r12-r15 são callee-saved: repõe-nos. Tudo o resto um", 10
    db db "     call pode destruir, e syscall destrói rcx e r11", 10
    db db "   - a pilha tem tamanho fixo; a recursão precisa de um caso base", 10
    db 0

summary_7:
    db db "   - and desliga, or liga, xor inverte, test só lê: tudo com uma máscara", 10
    db db "   - shl multiplica por 2^n; shr divide sem sinal, sar com sinal;", 10
    db db "     o shift errado para a direita faz de -16 / 4 um número enorme", 10
    db db "   - CF é overflow sem sinal, OF com sinal; o CPU define ambos e o", 10
    db db "     teu salto escolhe qual conta", 10
    db db "   - idiv precisa de cqo, div de xor rdx; idiv trunca para zero", 10
    db db "   - movzx alarga dados sem sinal, movsx com sinal", 10
    db db "   - as tabelas do hardware são campos de bits: mascarar, deslocar, or", 10
    db 0

summary_8:
    db db "   - movs/stos/lods/scas/cmps usam rsi, rdi, al/rax e rcx como", 10
    db db "     operandos fixos; rep repete rcx vezes, repe/repne vigiam ZF", 10
    db db "   - rep movsb é memcpy, rep stosb é memset, repne scasb é strlen", 10
    db db "   - depois de uma diferença em repe cmpsb, rsi/rdi ficam um à frente", 10
    db db "   - DF escolhe o sentido; std numa cópia sobreposta para um endereço", 10
    db db "     mais alto, e sempre cld antes de correr mais alguma coisa", 10
    db db "   - rep movsb avança rdi: um memcpy que devolve dest guarda-o primeiro", 10
    db 0

summary_9:
    db db "   - a convenção System V é a interface toda: registos à entrada, rax", 10
    db db "     à saída, rbx/rbp/r12-r15 mantidos, rsp múltiplo de 16 num call", 10
    db db "   - asm chamado de C precisa de global e de uma declaração igual;", 10
    db db "     C chamado de asm precisa de alinhamento, al definido em varargs,", 10
    db db "     e wrt ..plt (ou -no-pie) no PIE predefinido do gcc", 10
    db db "   - C freestanding precisa de um ponto de entrada em assembly e do", 10
    db db "     seu memcpy/memset: o compilador chama-os sem lhe pedirem", 10
    db db "   - o código do kernel compila com -mno-red-zone porque as", 10
    db db "     interrupções usam a pilha abaixo de rsp", 10
    db db "   - as disposições de hardware precisam de structs packed e de um", 10
    db db "     assert do tamanho; a memória de ecrã precisa de volatile", 10
    db 0

summary_10:
    db db "   - a BIOS carrega o setor 0 para 0x7C00, quer 0x55 0xAA no fim, e", 10
    db db "     salta para lá em modo real de 16 bits sem nada preparado", 10
    db db "   - endereço = segmento * 16 + offset, um megabyte, 64 KiB de cada vez", 10
    db db "   - org 0x7C00 diz ao NASM onde o código fica; sem isso todas as", 10
    db db "     etiquetas erram; times 510 - ($ - $$) db 0 e dw 0xAA55 terminam", 10
    db db "   - define primeiro ds, es, ss e sp; hlt num ciclo no fim", 10
    db db "   - int 0x10/0x13/0x15/0x16 são serviços da BIOS que só existem aqui", 10
    db 0

summary_11:
    db db "   - out/in falam com portas de E/S; COM1 é 0x3F8, estado em 0x3FD bit 5", 10
    db db "   - a porta série é a primeira saída de depuração de um SO, e o QEMU", 10
    db db "     pode enviá-la para o teu terminal", 10
    db db "   - o ecrã de texto é memória em 0xB8000: byte de carácter, byte de", 10
    db db "     atributo, linha * 80 + coluna células; o atributo 0 é invisível", 10
    db db "   - em modo real um registo de segmento guarda o endereço a dividir", 10
    db db "     por 16, por isso o ecrã é es = 0xB800, não 0xB8000", 10
    db db "   - int 0x16 dá um carácter e um scan code; a BIOS faz o trabalho do", 10
    db db "     teclado que farás tu a partir do módulo 16", 10
    db 0

summary_12:
    db db "   - a fase 1 lê setores com int 0x13 função 0x42 (leitura LBA) e", 10
    db db "     salta para a fase 2; a BIOS dá o disco de arranque em dl, e a", 10
    db db "     fase 1 tem de o guardar antes de usar dl para outra coisa", 10
    db db "   - cat junta as fases numa imagem de disco; as contagens de setores", 10
    db db "     combinam-se por constante, e times mantém a fase 2 no tamanho lido", 10
    db db "   - A20 tem de estar ligado acima de 1 MiB: testa por aliasing, abre", 10
    db db "     pela porta 0x92, e nunca escrevas 1 no bit 0 dessa porta", 10
    db db "   - int 0x15 eax=0xE820 lista intervalos e tipos de memória; pergunta,", 10
    db db "     não assumas", 10
    db 0

summary_13:
    db db "   - modo protegido: 32 bits, 4 GiB, anéis de privilégio, e sem BIOS", 10
    db db "   - um registo de segmento guarda um seletor: a posição de um descritor", 10
    db db "     na GDT; código 0x08, dados 0x10 depois do descritor nulo", 10
    db db "   - o apontador que lgdt lê é limite = tamanho - 1, e depois uma base de 32 bits", 10
    db db "   - cli, lgdt, liga o bit 0 de cr0, salto FAR, depois recarrega os outros segmentos", 10
    db db "   - um passo errado costuma ser um triple fault: a máquina reinicia, e o", 10
    db db "     registo -d int do QEMU mostra a cadeia que lá levou", 10
    db 0

summary_14:
    db db "   - o modo longo precisa de paginação; dos segmentos só fica o bit L do de código", 10
    db db "   - verifica primeiro CPUID 0x80000001 edx bit 29", 10
    db db "   - um endereço virtual são quatro índices de 9 bits e um offset de 12;", 10
    db db "     uma entrada PD com o bit 7 ligado mapeia 2 MiB diretamente", 10
    db db "   - ordem: tabelas, cr3, PAE, EFER.LME, cr0.PG, salto far para um segmento", 10
    db db "     de código de 64 bits; o código a correr tem de estar em identity map", 10
    db db "   - LME é o pedido, LMA o resultado; EFER 0x500 significa ambos", 10
    db db "   - um emulador pode perdoar o que o hardware real não perdoaria", 10
    db 0

summary_15:
    db db "   - lê o kernel com a BIOS em modo real; depois o disco fica fora de alcance", 10
    db db "   - o papel do assembly à entrada: uma pilha, .bss a zeros, call kmain", 10
    db db "   - um script de linker define o endereço e a ordem; objcopy -O binary", 10
    db db "     faz uma imagem sem cabeçalho cujo primeiro byte é _start", 10
    db db "   - memória de ecrã e portas passam por volatile, ou as escritas desaparecem", 10
    db db "   - -mgeneral-regs-only até o SSE estar ligado; -mno-red-zone; as outras", 10
    db db "     opções existem porque faltam a libc e um carregador", 10
    db 0

summary_16:
    db db "   - uma interrupção: procura o vetor na IDT, empilha cinco registos,", 10
    db db "     corre o handler, iretq; a gate tem 16 bytes e lidt recebe 10", 10
    db db "   - um stub por vetor, um código de erro falso para o frame ficar", 10
    db db "     uniforme, registos guardados, um handler em C", 10
    db db "   - remapeia o PIC para longe dos vetores de exceção; envia EOI a", 10
    db db "     cada IRQ que tratares", 10
    db db "   - o seletor da gate tem de ser o segmento de código de 64 bits", 10
    db db "   - os handlers correm na pilha interrompida: -mno-red-zone, e curtos", 10
    db 0

section .bss
lang_pt: resb 1

section .text

; detect_language(rdi=envp): sets lang_pt if the setting is in the environment.
detect_language:
.next_variable:
    mov rsi, [rdi]
    test rsi, rsi
    jz .done
    lea rdx, [language_setting]
.compare:
    mov al, [rsi]
    cmp al, [rdx]
    jne .no_match
    test al, al
    jz .found
    inc rsi
    inc rdx
    jmp .compare
.no_match:
    add rdi, 8
    jmp .next_variable
.found:
    mov byte [lang_pt], 1
.done:
    ret

; translate(rdi=text) -> rax=the Portuguese for it in that mode, otherwise text.
translate:
    mov rax, rdi
    cmp byte [lang_pt], 0
    je .done
    lea r8, [text_table]
.next_entry:
    mov rsi, [r8]
    test rsi, rsi
    jz .done
    mov rcx, rdi
.compare:
    mov dl, [rcx]
    cmp dl, [rsi]
    jne .skip
    test dl, dl
    jz .found
    inc rcx
    inc rsi
    jmp .compare
.skip:
    add r8, 16
    jmp .next_entry
.found:
    mov rax, [r8+8]
.done:
    ret

; print_tr(rdi=text): print_cstr, through translate.
print_tr:
    call translate
    mov rdi, rax
    jmp print_cstr

; summary_text(rdi=english, rsi=module) -> rax=the text to print.
summary_text:
    mov rax, rdi
    cmp byte [lang_pt], 0
    je .done
    lea rcx, [summaries]
    mov rax, [rcx+rsi*8]
.done:
    ret

; Tells the linker this object does not need an executable stack.
section .note.GNU-stack noalloc noexec nowrite progbits
