/*
 * Modulos 6 a 10 - arrays, ponteiros, memoria e structs.
 *
 * Esta e a parte que separa o C das linguagens que ja sabes. Nenhuma delas
 * te obriga a saber onde os teus dados estao; o C obriga, e em troca deixa
 * ver e mexer.
 *
 * Os exemplos que sao erros de memoria (double free, use-after-free) estao
 * explicados mas nao correm. Comportamento indefinido nao se demonstra ao
 * vivo: ou rebenta o curso, ou - pior - parece funcionar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "licoes.h"
#include "ui.h"

void licao_06_arrays_strings(void)
{
    titulo("MODULO 6 - ARRAYS E STRINGS");

    seccao("PARTE 1: um array e memoria seguida");

    printf("    int numeros[5] = {10, 20, 30, 40, 50};\n\n");

    int numeros[5] = {10, 20, 30, 40, 50};

    printf("  Cinco inteiros encostados uns aos outros. Cada um ocupa\n");
    printf("  %zu bytes, por isso o array inteiro ocupa %zu.\n\n",
           sizeof(int), sizeof numeros);

    printf("  As moradas reais, agora:\n\n");
    for (int i = 0; i < 5; i++) {
        printf("    numeros[%d] = %2d   em  %p\n", i, numeros[i], (void *) &numeros[i]);
    }
    printf("\n  Repara que as moradas sobem de %zu em %zu bytes. Nao e\n",
           sizeof(int), sizeof(int));
    printf("  coincidencia: e o que 'seguidos na memoria' quer dizer.\n");

    pausa();
    limpar_ecra();
    seccao("PARTE 2: uma string e um array que acaba em \\0");

    char texto[] = "ola";

    printf("    char texto[] = \"ola\";\n\n");
    printf("  Parecem 3 letras. Sao 4 bytes:\n\n");
    printf("    indice   0     1     2     3\n");
    printf("    valor   'o'   'l'   'a'   '\\0'\n\n");

    printf("    strlen(texto)  = %zu   <- letras ate ao \\0\n", strlen(texto));
    printf("    sizeof texto   = %zu   <- bytes ocupados, \\0 incluido\n\n", sizeof texto);

    printf("  O '\\0' e o que diz onde o texto acaba. Nenhuma funcao de\n");
    printf("  string sabe o tamanho: elas andam ate encontrar o \\0.\n\n");
    printf("  Se ele desaparecer, elas continuam a andar pela memoria\n");
    printf("  fora. E por isso que um array de 10 so guarda 9 letras.\n");

    pausa();
    limpar_ecra();
    seccao("PARTE 3: == nao compara texto");

    const char *a = "ola";
    char b[] = "ola";

    printf("    const char *a = \"ola\";\n");
    printf("    char b[] = \"ola\";\n\n");
    printf("    a == b        ->  %s\n", (a == (const char *) b) ? "verdadeiro" : "falso");
    printf("    strcmp(a, b)  ->  %d\n\n", strcmp(a, b));

    printf("  As letras sao iguais mas as moradas nao. O == compara\n");
    printf("  moradas, por isso da falso.\n\n");

    printf("  Para comparar texto usa-se strcmp, que devolve 0 quando sao\n");
    printf("  iguais. Zero. E facil de trocar:\n\n");
    printf("    if (strcmp(a, b) == 0)    <- iguais\n");
    printf("    if (strcmp(a, b))         <- DIFERENTES\n\n");
    printf("  Precisa de #include <string.h>.\n");

    pausa();
    limpar_ecra();
    seccao("RESUMO");

    printf("   - array = valores seguidos na memoria\n");
    printf("   - string = array de char terminado em \\0\n");
    printf("   - strlen conta letras, sizeof conta bytes\n");
    printf("   - == compara moradas; strcmp(a,b)==0 compara texto\n");
    pausa();
}

/* Usada pelo modulo 7 para mostrar que a copia nao afeta o original. */
static void tentar_mudar(int copia)
{
    copia = 99;
    (void) copia; /* so para o compilador nao avisar que nao e usada */
}

static void mudar_mesmo(int *morada)
{
    *morada = 99;
}

void licao_07_funcoes(void)
{
    titulo("MODULO 7 - FUNCOES");

    seccao("PARTE 1: a forma");

    printf("    int somar(int a, int b)\n");
    printf("    {\n");
    printf("        return a + b;\n");
    printf("    }\n\n");
    printf("     ^     ^         ^\n");
    printf("     |     |         o que recebe, com tipos\n");
    printf("     |     nome\n");
    printf("     tipo do que devolve\n\n");

    printf("  Uma funcao que nao devolve nada declara-se void.\n");
    printf("  Uma funcao que nao recebe nada declara-se (void), nao ().\n");

    pausa();
    limpar_ecra();
    seccao("PARTE 2: o C passa copias");

    int x = 5;

    printf("  Esta e a regra que explica o & do scanf.\n\n");
    printf("    void tentar_mudar(int copia) { copia = 99; }\n\n");
    printf("    int x = 5;\n");
    printf("    tentar_mudar(x);\n\n");

    tentar_mudar(x);
    printf("    x continua %d\n\n", x);

    printf("  A funcao recebeu uma COPIA do 5. Mudou a copia. O teu x\n");
    printf("  nunca soube de nada.\n\n");

    printf("  Para uma funcao mudar a tua variavel, tens de lhe dar a\n");
    printf("  morada dela:\n\n");
    printf("    void mudar_mesmo(int *morada) { *morada = 99; }\n");
    printf("    mudar_mesmo(&x);\n\n");

    mudar_mesmo(&x);
    printf("    x agora e %d\n\n", x);

    printf("  E exatamente o que o scanf faz. Nada de magico.\n");

    pausa();
    limpar_ecra();
    seccao("PARTE 3: declarar antes de usar");

    printf("  O compilador le o ficheiro de cima para baixo. Se chamares\n");
    printf("  uma funcao antes de ele a ter visto:\n\n");
    printf("    error: implicit declaration of function 'somar'\n\n");

    printf("  Desde o GCC 14 isto e erro, nao aviso. Antes o compilador\n");
    printf("  adivinhava a assinatura e gerava codigo errado em silencio.\n\n");

    printf("  Duas solucoes: por a funcao acima de quem a usa, ou\n");
    printf("  declarar a assinatura no topo:\n\n");
    printf("    int somar(int a, int b);     <- prototipo, com ;\n\n");
    printf("  E isso que os ficheiros .h sao: uma lista de prototipos.\n");

    pausa();
    limpar_ecra();
    seccao("RESUMO");

    printf("   - tipo nome(parametros) { ... return ...; }\n");
    printf("   - os argumentos sao copias\n");
    printf("   - passa &variavel para a funcao poder mudar a tua\n");
    printf("   - declara antes de usar, ou usa um prototipo\n");
    pausa();
}

void licao_08_ponteiros(void)
{
    titulo("MODULO 8 - PONTEIROS");

    seccao("PARTE 1: um ponteiro guarda uma morada");

    int idade = 25;
    int *p = &idade;

    printf("    int idade = 25;\n");
    printf("    int *p = &idade;\n\n");

    printf("  Duas variaveis normais. A diferenca e o que guardam:\n\n");
    printf("    idade   guarda  %d\n", idade);
    printf("    p       guarda  %p     <- a morada de idade\n\n", (void *) p);

    printf("  Dois simbolos, e sao opostos:\n\n");
    printf("    &idade    a morada de idade      ->  %p\n", (void *) &idade);
    printf("    *p        o valor la dentro      ->  %d\n\n", *p);

    printf("  Le-se: & = 'morada de', * = 'o que esta em'.\n");

    pausa();
    limpar_ecra();
    seccao("PARTE 2: o * quer dizer duas coisas");

    printf("  Isto confunde toda a gente, e e so notacao:\n\n");
    printf("    int *p = &idade;    aqui * faz parte do TIPO\n");
    printf("                        'p e um ponteiro para int'\n\n");
    printf("    *p = 30;            aqui * e uma OPERACAO\n");
    printf("                        'escreve 30 onde p aponta'\n\n");

    *p = 30;
    printf("  Depois de *p = 30, a variavel idade vale %d.\n\n", idade);

    printf("  Nunca lhe toquei directamente. Mudei atraves do ponteiro,\n");
    printf("  e e a mesma memoria.\n");

    pausa();
    limpar_ecra();
    seccao("PARTE 3: NULL e o Segmentation fault");

    int *vazio = NULL;

    printf("    int *vazio = NULL;\n\n");
    printf("  NULL quer dizer 'nao aponta para nada'. Vale %p.\n\n", (void *) vazio);

    printf("  Fazer *vazio mata o programa:\n\n");
    printf("    Segmentation fault (core dumped)\n\n");

    printf("  Isso e o sistema a dizer 'pediste memoria que nao e tua'.\n");
    printf("  Por isso um ponteiro que pode nao ter nada testa-se antes:\n\n");
    printf("    if (p != NULL) {\n");
    printf("        printf(\"%%d\\n\", *p);\n");
    printf("    }\n\n");

    printf("  Um ponteiro nao inicializado e pior que NULL: tem lixo, que\n");
    printf("  pode ate ser uma morada valida. Inicializa sempre, nem que\n");
    printf("  seja a NULL.\n");

    pausa();
    limpar_ecra();
    seccao("RESUMO");

    printf("   - ponteiro = variavel que guarda uma morada\n");
    printf("   - & obtem a morada, * le/escreve o que la esta\n");
    printf("   - no tipo, * diz 'ponteiro'; no codigo, * acede\n");
    printf("   - testa NULL antes de usar\n\n");

    printf("  O modulo 9 usa isto para pedir memoria a serio.\n");
    pausa();
}

void licao_09_memoria(void)
{
    titulo("MODULO 9 - MEMORIA: MALLOC E FREE");

    seccao("PARTE 1: dois sitios onde os dados vivem");

    printf("    int numeros[5];              <- stack\n");
    printf("    int *p = malloc(5 * sizeof(int));   <- heap\n\n");

    printf("    STACK                     HEAP\n");
    separador();
    printf("    automatica                pedida por ti\n");
    printf("    morre no fim da funcao    fica ate fazeres free\n");
    printf("    tamanho fixo, no codigo   tamanho decidido a correr\n");
    printf("    pequena (~8 MB)           limitada pela RAM\n\n");

    printf("  Enquanto souberes o tamanho quando escreves o codigo, usa a\n");
    printf("  stack: e mais rapida e limpa-se sozinha.\n\n");
    printf("  A heap serve para quando o tamanho so se sabe a correr, ou\n");
    printf("  quando os dados tem de sobreviver a funcao que os criou.\n");

    pausa();
    limpar_ecra();
    seccao("PARTE 2: pedir, usar, devolver");

    printf("    int *v = malloc(5 * sizeof(int));\n");
    printf("    if (v == NULL) { return 1; }      <- pode falhar\n");
    printf("\n");
    printf("    v[0] = 10;\n");
    printf("\n");
    printf("    free(v);      <- devolve ao sistema\n");
    printf("    v = NULL;     <- para nao ficar a apontar para lixo\n\n");

    int *v = malloc(5 * sizeof(int));

    if (v == NULL) {
        printf("  O malloc falhou. Sem memoria.\n");
        pausa();
        return;
    }

    for (int i = 0; i < 5; i++) {
        v[i] = (i + 1) * 10;
    }

    printf("  A correr agora: pedi 5 inteiros (%zu bytes) e recebi a\n",
           5 * sizeof(int));
    printf("  morada %p.\n\n", (void *) v);
    printf("  Conteudo:  ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", v[i]);
    }
    printf("\n\n");

    printf("  Repara que v[i] funciona num ponteiro tal como num array.\n");
    printf("  v[i] e o mesmo que *(v + i).\n");

    free(v);
    v = NULL;

    printf("\n  Ja fiz free. A memoria voltou ao sistema.\n");

    pausa();
    limpar_ecra();
    seccao("PARTE 3: as quatro maneiras de errar");

    printf("  1. FUGA (memory leak)\n\n");
    printf("       int *p = malloc(100);\n");
    printf("       p = malloc(200);      <- perdeste a primeira morada\n\n");
    printf("     Aqueles 100 bytes ficam ocupados e ja ninguem sabe onde\n");
    printf("     estao. Num programa que corre uma vez, quase nao se nota.\n");
    printf("     Num servidor, come a RAM toda ao fim de dias.\n\n");

    printf("  2. USAR DEPOIS DE LIBERTAR (use after free)\n\n");
    printf("       free(p);\n");
    printf("       printf(\"%%d\", *p);    <- a memoria ja nao e tua\n\n");
    printf("     As vezes ainda mostra o valor antigo. E o pior caso:\n");
    printf("     parece funcionar ate o sistema reutilizar aqueles bytes.\n\n");

    pausa();
    limpar_ecra();
    seccao("PARTE 3 (continuacao)");

    printf("  3. LIBERTAR DUAS VEZES (double free)\n\n");
    printf("       free(p);\n");
    printf("       free(p);      <- estraga as contas do sistema\n\n");
    printf("     A glibc costuma apanhar isto e matar o programa:\n");
    printf("       free(): double free detected in tcache 2\n\n");
    printf("     Por isso e que se poe p = NULL depois do free: fazer\n");
    printf("     free(NULL) e legal e nao faz nada.\n\n");

    printf("  4. DEVOLVER MEMORIA DA STACK\n\n");
    printf("       char *nome(void) {\n");
    printf("           char buffer[50];      <- morre no return\n");
    printf("           return buffer;        <- morada de lixo\n");
    printf("       }\n\n");
    printf("     O array vive na stack da funcao. Quando ela acaba, aquele\n");
    printf("     espaco passa a ser de outra pessoa. Para devolver texto,\n");
    printf("     usa malloc - e quem chama fica com o dever do free.\n");

    pausa();
    limpar_ecra();
    seccao("PARTE 4: a regra e a ferramenta");

    printf("  A REGRA\n\n");
    printf("    Cada malloc tem exatamente um free.\n");
    printf("    Quem faz malloc decide quem faz free, e escreve-o.\n\n");

    printf("  Nao ha recolector de lixo. Se nao libertares tu, ninguem\n");
    printf("  liberta - ate o programa acabar.\n\n");

    printf("  AS FERRAMENTAS\n\n");
    printf("  1. Ja tens esta - vem com o gcc, nao instalas nada:\n\n");
    printf("       gcc -fsanitize=address,undefined -g programa.c -o programa\n");
    printf("       ./programa\n\n");
    printf("     Se faltar um free, o programa diz ao sair:\n\n");
    printf("       ERROR: LeakSanitizer: detected memory leaks\n");
    printf("       Direct leak of 100 byte(s) in 1 object(s)\n");
    printf("         #1 main programa.c:7\n\n");
    printf("     Da-te o ficheiro e a linha do malloc esquecido. Tambem\n");
    printf("     apanha use-after-free e escrita fora de um array.\n\n");

    printf("  2. O valgrind faz o mesmo sem recompilar:\n\n");
    printf("       valgrind ./programa\n\n");
    printf("     Tens de o instalar (sudo pacman -S valgrind no Arch).\n\n");

    printf("  Este curso foi verificado com a primeira: nao tem fugas.\n");

    pausa();
    limpar_ecra();
    seccao("RESUMO");

    printf("   - stack: automatica, morre com a funcao\n");
    printf("   - heap: malloc, vive ate ao free\n");
    printf("   - testa sempre se o malloc devolveu NULL\n");
    printf("   - um free por cada malloc, e p = NULL a seguir\n");
    printf("   - nunca devolvas a morada de um array local\n");
    printf("   - valgrind ./programa diz-te onde falhaste\n");
    pausa();
}

void licao_10_structs(void)
{
    titulo("MODULO 10 - STRUCTS");

    seccao("PARTE 1: juntar campos num tipo");

    struct Aluno {
        char nome[50];
        int nota;
    };

    printf("    struct Aluno {\n");
    printf("        char nome[50];\n");
    printf("        int  nota;\n");
    printf("    };                    <- o ; e obrigatorio\n\n");

    struct Aluno a;
    strcpy(a.nome, "Ana");
    a.nota = 15;

    printf("    struct Aluno a;\n");
    printf("    strcpy(a.nome, \"Ana\");\n");
    printf("    a.nota = 15;\n\n");

    printf("  A correr:  %s tirou %d\n\n", a.nome, a.nota);

    printf("  Repara no strcpy. Nao podes fazer a.nome = \"Ana\": um array\n");
    printf("  nao se atribui, copia-se.\n");

    pausa();
    limpar_ecra();
    seccao("PARTE 2: ponto ou seta");

    struct Aluno *ptr = &a;

    printf("    a.nota        quando tens a struct\n");
    printf("    ptr->nota     quando tens um PONTEIRO para ela\n\n");

    printf("  A correr:\n");
    printf("    a.nota     ->  %d\n", a.nota);
    printf("    ptr->nota  ->  %d\n\n", ptr->nota);

    printf("  A seta e so uma abreviatura. Isto e exactamente o mesmo:\n\n");
    printf("    ptr->nota      e      (*ptr).nota\n\n");
    printf("  Os parenteses sao precisos porque o . agarra mais forte que\n");
    printf("  o *. Como ninguem quer escrever aquilo, inventou-se o ->.\n\n");

    printf("  Se te enganares, o compilador diz:\n");
    printf("    error: member reference type 'struct Aluno *' is a pointer;\n");
    printf("           did you mean to use '->'?\n");

    pausa();
    limpar_ecra();
    seccao("PARTE 3: a ordem dos campos ocupa espaco");

    struct Mau  { char a; int b; char c; };
    struct Bom  { char a; char c; int b; };

    printf("    struct Mau  { char a; int b; char c; };  -> %zu bytes\n",
           sizeof(struct Mau));
    printf("    struct Bom  { char a; char c; int b; };  -> %zu bytes\n\n",
           sizeof(struct Bom));

    printf("  Os mesmos tres campos. Tamanhos diferentes.\n\n");
    printf("  O processador prefere ler um int numa morada multipla de 4,\n");
    printf("  por isso o compilador poe espaco vazio (padding) para\n");
    printf("  alinhar. Na primeira struct o int fica entalado entre dois\n");
    printf("  chars e desperdica-se espaco dos dois lados.\n\n");

    printf("  Regra: campos grandes primeiro, pequenos no fim. So importa\n");
    printf("  quando tens milhoes destas structs - mas e bom saber porque\n");
    printf("  o sizeof nao da a soma dos campos.\n");

    pausa();
    limpar_ecra();
    seccao("RESUMO");

    printf("   - struct junta campos num tipo so; ; no fim da declaracao\n");
    printf("   - a.campo com a struct, ptr->campo com um ponteiro\n");
    printf("   - -> e (*ptr). escritos de outra maneira\n");
    printf("   - sizeof pode ser maior que a soma dos campos (padding)\n\n");

    printf("  Chegaste ao fim do curso base. O proximo passo e escrever um\n");
    printf("  programa teu que use malloc e o liberte.\n");
    pausa();
}
