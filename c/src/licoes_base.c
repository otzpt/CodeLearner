/*
 * Modulos 1 a 5 - os fundamentos.
 *
 * Regra usada em todos os modulos: nada e afirmado sem se mostrar o
 * resultado. Onde da para correr o exemplo aqui mesmo, ele corre.
 */

#include <stdio.h>
#include <string.h>

#include "licoes.h"
#include "ui.h"

void licao_01_compilar(void)
{
    titulo("MODULO 1 - COMPILAR E IMPRIMIR");

    seccao("PARTE 1: o C nao corre, compila");

    printf("  Python le o teu ficheiro e executa-o. O C nao.\n");
    printf("  O compilador traduz o teu texto para instrucoes do\n");
    printf("  processador e grava um ficheiro novo. So depois e que\n");
    printf("  ha alguma coisa para correr.\n\n");

    printf("    programa.c   ->   [ gcc ]   ->   ./programa\n");
    printf("    o que escreves    traduz        o que corre\n\n");

    printf("  Isto tem uma consequencia boa: erros de escrita aparecem\n");
    printf("  na compilacao, antes de o programa chegar a alguem.\n");

    pausa();
    limpar_ecra();
    seccao("PARTE 2: o programa mais pequeno que funciona");

    printf("    #include <stdio.h>\n");
    printf("\n");
    printf("    int main(void)\n");
    printf("    {\n");
    printf("        printf(\"Ola mundo\\n\");\n");
    printf("        return 0;\n");
    printf("    }\n\n");

    printf("  Linha a linha:\n\n");
    printf("   #include <stdio.h>   traz o printf. Sem isto o compilador\n");
    printf("                        nao sabe o que printf e.\n");
    printf("   int main(void)       onde o programa comeca. Tem de existir.\n");
    printf("   printf(...)          escreve no ecra.\n");
    printf("   \\n                   muda de linha. Sem ele fica tudo colado.\n");
    printf("   return 0             0 significa 'correu bem'.\n");

    pausa();
    limpar_ecra();
    seccao("PARTE 3: compilar e correr");

    printf("    gcc programa.c -o programa\n");
    printf("    ./programa\n\n");

    printf("   -o programa   diz o nome do ficheiro a criar. Sem isto o\n");
    printf("                 gcc grava um ficheiro chamado a.out.\n");
    printf("   ./            'nesta pasta'. Sem isto o sistema procura o\n");
    printf("                 programa nas pastas do sistema e nao o encontra.\n\n");

    printf("  Compila sempre com avisos ligados:\n\n");
    printf("    gcc -Wall -Wextra programa.c -o programa\n\n");
    printf("  Estes dois avisos apanham erros reais. Este curso e\n");
    printf("  compilado assim.\n");

    pausa();
    limpar_ecra();
    seccao("RESUMO");

    printf("   - o C compila antes de correr\n");
    printf("   - todo o programa comeca no main\n");
    printf("   - printf escreve, \\n muda de linha\n");
    printf("   - -Wall -Wextra sempre\n\n");

    printf("  No modulo 2: guardar valores em variaveis.\n");
    pausa();
}

void licao_02_variaveis(void)
{
    titulo("MODULO 2 - VARIAVEIS E TIPOS");

    seccao("PARTE 1: em C, o tipo diz-se primeiro");

    printf("  Em Python escreve-se  idade = 25.\n");
    printf("  Em C tem de se dizer que tipo de valor a caixa guarda:\n\n");

    printf("    int   idade  = 25;\n");
    printf("    float altura = 1.75f;\n");
    printf("    char  letra  = 'A';\n\n");

    printf("  Porque? Porque o compilador precisa de saber quantos bytes\n");
    printf("  reservar e como interpretar esses bytes. Um int e uma\n");
    printf("  sequencia de bits lida como numero inteiro; um float e a\n");
    printf("  mesma quantidade de bits lida de outra maneira.\n\n");

    printf("  Uma variavel so guarda o tipo que declaraste, para sempre.\n");

    pausa();
    limpar_ecra();
    seccao("PARTE 2: os tipos que vais usar");

    printf("    tipo     guarda                exemplo\n");
    separador();
    printf("    int      numeros inteiros      -3, 0, 25\n");
    printf("    float    decimais              1.75f\n");
    printf("    double   decimais, mais casas  1.75\n");
    printf("    char     UM caractere          'A'\n\n");

    printf("  Neste computador ocupam:\n\n");
    printf("    sizeof(int)    = %zu bytes\n", sizeof(int));
    printf("    sizeof(float)  = %zu bytes\n", sizeof(float));
    printf("    sizeof(double) = %zu bytes\n", sizeof(double));
    printf("    sizeof(char)   = %zu byte\n\n", sizeof(char));

    printf("  Estes numeros foram calculados agora, pelo teu compilador.\n");
    printf("  O C nao garante que sejam iguais noutra maquina.\n");

    pausa();
    limpar_ecra();
    seccao("PARTE 3: imprimir cada tipo");

    printf("  O printf nao sabe o tipo do que lhe passas. Tens de lhe\n");
    printf("  dizer com uma marca:\n\n");

    printf("    %%d   int\n");
    printf("    %%f   float ou double\n");
    printf("    %%c   char\n");
    printf("    %%s   string\n\n");

    int idade = 25;
    float altura = 1.75f;
    char inicial = 'J';

    printf("  A correr mesmo agora:\n\n");
    printf("    printf(\"%%d\", idade)      ->  %d\n", idade);
    printf("    printf(\"%%f\", altura)     ->  %f\n", altura);
    printf("    printf(\"%%.2f\", altura)   ->  %.2f\n", altura);
    printf("    printf(\"%%c\", inicial)    ->  %c\n\n", inicial);

    printf("  O %%.2f arredonda so para mostrar. A variavel nao muda.\n");

    pausa();
    limpar_ecra();
    seccao("PARTE 4: a divisao que engana toda a gente");

    int a = 5, b = 2;

    printf("    int a = 5, b = 2;\n\n");
    printf("    a / b            ->  %d\n", a / b);
    printf("    (float) a / b    ->  %.1f\n\n", (float) a / b);

    printf("  5 / 2 da 2, nao 2.5. Os dois lados sao int, por isso o C\n");
    printf("  faz divisao inteira e deita a parte decimal fora.\n\n");
    printf("  Nao e arredondamento: 9 / 10 da 0.\n\n");

    printf("  Para teres decimais, um dos lados tem de ser decimal ANTES\n");
    printf("  da divisao. Depois ja e tarde:\n\n");
    printf("    (float)(a / b)   ->  %.1f   <- ja se perdeu\n", (float)(a / b));
    printf("    (float) a / b    ->  %.1f   <- certo\n\n", (float) a / b);

    printf("  Esta e a origem de metade das medias erradas em C, e o\n");
    printf("  compilador nao avisa: a divisao inteira e legal.\n");

    pausa();
    limpar_ecra();
    seccao("RESUMO");

    printf("   - o tipo vem antes do nome e nunca muda\n");
    printf("   - cada tipo tem a sua marca no printf\n");
    printf("   - int / int da int, mesmo que guardes num float\n");
    printf("   - converte antes de dividir, nao depois\n");
    pausa();
}

void licao_03_input(void)
{
    titulo("MODULO 3 - LER DO TECLADO");

    seccao("PARTE 1: scanf e o &");

    printf("    int idade;\n");
    printf("    scanf(\"%%d\", &idade);\n\n");

    printf("  O & antes do nome nao e decoracao. Sem ele o programa\n");
    printf("  compila e depois estoira.\n\n");

    printf("  Motivo: em C, passar uma variavel a uma funcao passa uma\n");
    printf("  COPIA. O scanf tem de mudar a tua variavel, e nao consegue\n");
    printf("  mudar uma copia. O & da-lhe a MORADA da variavel, para ele\n");
    printf("  ir la escrever.\n\n");

    printf("  Sem &, o scanf recebe o valor que la esta (lixo) e usa-o\n");
    printf("  como se fosse uma morada. Escrever numa morada inventada e\n");
    printf("  o Segmentation fault.\n\n");

    printf("  O modulo 8 explica isto com calma. Por agora: com scanf,\n");
    printf("  poe sempre &.\n");

    pausa();
    limpar_ecra();
    seccao("PARTE 2: a excecao");

    printf("    char nome[50];\n");
    printf("    scanf(\"%%49s\", nome);      <- sem &\n\n");

    printf("  Um array ja e uma morada. O nome `nome` sozinho ja significa\n");
    printf("  'onde o array comeca', por isso o & seria uma morada de uma\n");
    printf("  morada.\n\n");

    printf("  Repara no 49: e o limite de caracteres que o scanf pode\n");
    printf("  escrever. O array tem 50 lugares e o ultimo fica para o\n");
    printf("  marcador de fim de texto.\n\n");

    printf("  Sem esse limite, um nome com 200 letras escreve para fora do\n");
    printf("  array, por cima do que la estiver. E a classe de bug mais\n");
    printf("  explorada da historia do C.\n");

    pausa();
    limpar_ecra();
    seccao("PARTE 3: experimenta");

    printf("  Este curso esta escrito em C. O que se segue e mesmo o\n");
    printf("  programa a ler o teu teclado.\n\n");

    char nome[50];
    printf("  Como te chamas? ");
    fflush(stdout);

    if (ler_linha(nome, sizeof nome) && nome[0] != '\0') {
        printf("\n  Ola, %s. Isso tem %zu caracteres.\n", nome, strlen(nome));
    } else {
        printf("\n  (nao escreveste nada)\n");
    }

    printf("\n  Este curso nao usa scanf para isto. Usa fgets, que recebe\n");
    printf("  o tamanho do destino e por isso nunca escreve a mais:\n\n");
    printf("    fgets(nome, sizeof nome, stdin);\n\n");
    printf("  O scanf com %%s tem outro problema: para no primeiro espaco,\n");
    printf("  por isso 'Ana Maria' le so 'Ana'.\n");

    pausa();
    limpar_ecra();
    seccao("RESUMO");

    printf("   - scanf com & para numeros, sem & para arrays\n");
    printf("   - sem & -> Segmentation fault\n");
    printf("   - limita sempre o tamanho: %%49s num array de 50\n");
    printf("   - fgets e mais seguro e le a linha toda\n");
    pausa();
}

void licao_04_condicoes(void)
{
    titulo("MODULO 4 - CONDICOES");

    seccao("PARTE 1: if, else if, else");

    printf("    int nota = 85;\n\n");
    printf("    if (nota >= 90) {\n");
    printf("        printf(\"A\\n\");\n");
    printf("    } else if (nota >= 80) {\n");
    printf("        printf(\"B\\n\");\n");
    printf("    } else {\n");
    printf("        printf(\"F\\n\");\n");
    printf("    }\n\n");

    int nota = 85;
    printf("  A correr agora com nota = %d:  ", nota);
    if (nota >= 90) {
        printf("A\n\n");
    } else if (nota >= 80) {
        printf("B\n\n");
    } else {
        printf("F\n\n");
    }

    printf("  So o primeiro ramo verdadeiro corre. Por isso a ordem\n");
    printf("  importa: se testasses >= 80 primeiro, um 95 parava ai.\n");

    pausa();
    limpar_ecra();
    seccao("PARTE 2: = nao e ==");

    printf("    if (x = 5)     atribui 5 a x, e depois pergunta se 5 e\n");
    printf("                   verdadeiro. E sempre. Sempre.\n");
    printf("    if (x == 5)    compara.\n\n");

    printf("  Isto compila. Com -Wall o gcc avisa, e e por isso que este\n");
    printf("  curso liga sempre os avisos.\n\n");

    printf("  Em C nao ha tipo booleano nas versoes antigas: 0 e falso e\n");
    printf("  tudo o resto e verdadeiro. Por isso `if (x = 5)` e um teste\n");
    printf("  perfeitamente valido para o compilador.\n");

    pausa();
    limpar_ecra();
    seccao("PARTE 3: as chavetas que faltam");

    printf("    if (chove)\n");
    printf("        printf(\"leva guarda-chuva\\n\");\n");
    printf("        printf(\"leva casaco\\n\");      <- corre SEMPRE\n\n");

    printf("  So a primeira linha pertence ao if. A indentacao engana os\n");
    printf("  olhos; o compilador nao a le.\n\n");
    printf("  Poe sempre chavetas, mesmo com uma linha so. Custa dois\n");
    printf("  caracteres e evita o bug quando alguem acrescentar a segunda.\n");

    pausa();
    limpar_ecra();
    seccao("RESUMO");

    printf("   - so o primeiro ramo verdadeiro corre; ordem importa\n");
    printf("   - == compara, = atribui\n");
    printf("   - 0 e falso, tudo o resto e verdadeiro\n");
    printf("   - chavetas sempre\n");
    pausa();
}

void licao_05_ciclos(void)
{
    titulo("MODULO 5 - CICLOS");

    seccao("PARTE 1: for");

    printf("    for (int i = 1; i <= 5; i++) {\n");
    printf("        printf(\"%%d \", i);\n");
    printf("    }\n\n");

    printf("  Tres partes separadas por ;\n");
    printf("    inicio      i = 1        corre uma vez\n");
    printf("    condicao    i <= 5       testada antes de cada volta\n");
    printf("    passo       i++          corre no fim de cada volta\n\n");

    printf("  A correr:  ");
    for (int i = 1; i <= 5; i++) {
        printf("%d ", i);
    }
    printf("\n");

    pausa();
    limpar_ecra();
    seccao("PARTE 2: o erro de um a mais");

    printf("  Um array de 5 posicoes tem indices 0,1,2,3,4. Nao ha 5.\n\n");

    printf("    for (int i = 0; i <= 5; i++)   <- toca em 6 posicoes\n");
    printf("    for (int i = 0; i <  5; i++)   <- certo\n\n");

    printf("  Com <= escreves uma posicao para fora do array. O C nao\n");
    printf("  verifica limites: nao ha erro, nao ha aviso, o programa\n");
    printf("  continua com memoria de outra coisa qualquer estragada.\n\n");

    printf("  Regra pratica: com contagem a comecar em 0, a condicao usa\n");
    printf("  < e nunca <=.\n");

    pausa();
    limpar_ecra();
    seccao("PARTE 3: while");

    printf("    int restante = 3;\n");
    printf("    while (restante > 0) {\n");
    printf("        printf(\"%%d \", restante);\n");
    printf("        restante--;\n");
    printf("    }\n\n");

    printf("  A correr:  ");
    int restante = 3;
    while (restante > 0) {
        printf("%d ", restante);
        restante--;
    }
    printf("\n\n");

    printf("  Se te esqueceres do restante--, a condicao nunca fica falsa\n");
    printf("  e o programa fica preso. Ctrl-C para o parar.\n\n");
    printf("  Usa for quando sabes quantas voltas, while quando nao sabes.\n");

    pausa();
    limpar_ecra();
    seccao("RESUMO");

    printf("   - for: inicio; condicao; passo\n");
    printf("   - indices vao de 0 a n-1: usa <, nao <=\n");
    printf("   - o C nao verifica limites de arrays\n");
    printf("   - algo dentro do while tem de mudar a condicao\n\n");

    printf("  No modulo 6 comeca a parte que separa o C do resto:\n");
    printf("  arrays, memoria e ponteiros.\n");
    pausa();
}
