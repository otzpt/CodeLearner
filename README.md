# Aprender a programar

Cursos de linha de comandos, um por linguagem. Sem browser, sem conta, sem
ligacao a internet — abres o terminal, corres o programa, aprendes.

## Estado

| Linguagem | Estado | Onde |
| --- | --- | --- |
| C | 10 modulos, do primeiro programa ate malloc/free | [`c/`](c/) |
| C++ | por fazer | |
| Python | por fazer | |
| JavaScript | por fazer | |
| Menu universal | por fazer — escolhe a linguagem e abre o curso | |

Cada curso e um programa independente. O menu universal sera apenas um
lancador: nada do que os cursos fazem depende dele.

## Correr o curso de C

```bash
cd c
make
./curso-c
```

Precisas do `gcc` (ou `clang`) e do `make`. No Arch:

```bash
sudo pacman -S base-devel
```

## Como os cursos sao escritos

**Escritos na propria linguagem que ensinam.** O curso de C esta escrito em C.
Quem esta a aprender pode abrir `c/src/` e ler codigo real que faz o que o
curso acabou de explicar — o menu usa arrays, structs e ponteiros para funcao,
todos ensinados la dentro.

**Nada e afirmado sem se mostrar.** Onde da para correr o exemplo, ele corre
mesmo. O modulo das variaveis imprime o `sizeof` calculado pelo teu
compilador, o dos ponteiros mostra moradas reais da tua maquina, e o da
memoria faz um `malloc` a serio e mostra o que recebeu.

**Erros de memoria sao explicados, nao demonstrados.** Um `double free` ou um
`use-after-free` sao comportamento indefinido: ou rebentavam o curso, ou —
pior — pareciam funcionar e ensinavam a coisa errada.

## Linux e Windows

O curso de C compila e corre nos dois. A unica diferenca no codigo sao tres
linhas em `c/src/ui.c`, a limpar o ecra.

Testado em Linux. O caminho Windows esta escrito mas ainda nao foi corrido.

## Verificacao

O curso de C compila com `-Wall -Wextra` sem um unico aviso, e foi corrido com
o AddressSanitizer e o UndefinedBehaviorSanitizer sem fugas nem erros. Um
curso que ensina `free` nao pode ter fugas.

```bash
cd c
cc -std=c11 -Wall -Wextra -g -fsanitize=address,undefined -o /tmp/curso src/*.c
/tmp/curso
```
