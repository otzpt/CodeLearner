# Curso de C
#
#   make          compila para ./curso-c
#   make correr   compila e corre
#   make limpar   apaga o que foi compilado
#
# -Wall -Wextra ficam sempre ligados. Num curso de C, um aviso do compilador
# e material de aula: quase todos apanham um erro real antes de ele acontecer.

CC      = cc
CFLAGS  = -std=c11 -Wall -Wextra -g
ALVO    = curso-c
FONTES  = src/main.c src/ui.c src/licoes_base.c src/licoes_memoria.c
OBJETOS = $(FONTES:.c=.o)

$(ALVO): $(OBJETOS)
	$(CC) $(CFLAGS) -o $@ $(OBJETOS)

# Recompila um .o se o .c ou qualquer header mudar. Sem a dependencia dos
# headers, mudar ui.h nao recompilava nada e o programa ficava inconsistente.
%.o: %.c src/ui.h src/licoes.h
	$(CC) $(CFLAGS) -c $< -o $@

correr: $(ALVO)
	./$(ALVO)

limpar:
	rm -f $(OBJETOS) $(ALVO)

.PHONY: correr limpar
