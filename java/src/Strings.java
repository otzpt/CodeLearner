// Portuguese (PT-PT) text for the menus, prompts and module summaries.
// CODELEARNER_LANG=pt-pt turns it on. The lessons themselves stay in English.

import java.util.Map;

final class Strings {
    private static final boolean PT = "pt-pt".equals(System.getenv("CODELEARNER_LANG"));

    private static final Map<String, String> TEXT = Map.ofEntries(
            Map.entry("Press ENTER to continue...", "Prime ENTER para continuar..."),
            Map.entry("Your answer:", "A tua resposta:"),
            Map.entry("CORRECT.", "CERTO."),
            Map.entry("NOT QUITE. The answer is:", "NÃO É BEM ISSO. A resposta é:"),
            Map.entry(">> EXERCISE - MODULE", ">> EXERCÍCIO - MÓDULO"),
            Map.entry(">> WRITE THIS YOURSELF, in a real file", ">> ESCREVE ISTO TU, num ficheiro a sério"),
            Map.entry(">> TRY THIS YOURSELF, in a real directory", ">> EXPERIMENTA TU, numa pasta a sério"),
            Map.entry("Try it with this input:", "Experimenta com esta entrada:"),
            Map.entry("It must print:", "Tem de escrever:"),
            Map.entry("You should see:", "Deves ver:"),
            Map.entry("The result must be:", "O resultado tem de ser:"),
            Map.entry("That output is the whole specification. Any code that", "Esse resultado é toda a especificação. Qualquer código que"),
            Map.entry("That output is the whole specification. Any code", "Esse resultado é toda a especificação. Qualquer código"),
            Map.entry("That output is the whole specification. Any commands that", "Esse resultado é toda a especificação. Quaisquer comandos que"),
            Map.entry("That result is the whole specification. Anything that", "Esse resultado é toda a especificação. Tudo o que"),
            Map.entry("produces it is correct.", "o produza está certo."),
            Map.entry("that produces it is correct.", "que o produza está certo."),
            Map.entry("produce it are correct.", "o produzam estão certos."),
            Map.entry("produces it is correct. Open your file in whatever browser", "o produza está certo. Abre o teu ficheiro no navegador"),
            Map.entry("you normally use; this course never opens one.", "que costumas usar; este curso nunca abre nenhum."),
            Map.entry("Try it first. Compile with:", "Tenta primeiro. Compila com:"),
            Map.entry("Try it first. Run with:", "Tenta primeiro. Corre com:"),
            Map.entry("Try it first. Compile and run with:", "Tenta primeiro. Compila e corre com:"),
            Map.entry("Want to see example code?", "Queres ver código de exemplo?"),
            Map.entry("Want to see example commands?", "Queres ver comandos de exemplo?"),
            Map.entry("This is EXAMPLE CODE, not the answer. It is one way to get", "Isto é CÓDIGO DE EXEMPLO, não a resposta. É uma forma de chegar"),
            Map.entry("This is EXAMPLE CODE, not the answer. It is one way to", "Isto é CÓDIGO DE EXEMPLO, não a resposta. É uma forma de"),
            Map.entry("that output; yours may look nothing like it and still be", "a esse resultado; o teu pode ser muito diferente e estar"),
            Map.entry("that result; yours may look nothing like it and still be", "a esse resultado; o teu pode ser muito diferente e estar"),
            Map.entry("right -- or better. Compare the output, not the code.", "certo, ou melhor. Compara o resultado, não o código."),
            Map.entry("right -- or better.", "certo, ou melhor."),
            Map.entry("get that output; yours may look nothing like it and", "chegar a esse resultado; o teu pode ser muito diferente e"),
            Map.entry("still be right -- or better. Compare the output, not", "estar certo, ou melhor. Compara o resultado, não"),
            Map.entry("the code.", "o código."),
            Map.entry("This is EXAMPLE, not the answer. It is one way to reach that", "Isto é um EXEMPLO, não a resposta. É uma forma de chegar a esse"),
            Map.entry("result; a different sequence of commands landing on the same", "resultado; outra sequência de comandos que chegue ao mesmo"),
            Map.entry("state is correct too.", "estado também está certa."),
            Map.entry("Quit", "Sair"),
            Map.entry("Pick a module:", "Escolhe um módulo:"),
            Map.entry("Not a valid option.", "Opção inválida."),
            Map.entry("See you next time.", "Até à próxima."),
            Map.entry("Something went wrong:", "Algo correu mal:"),
            Map.entry("NO BROWSER -- this course is a CLI program, same as every", "SEM NAVEGADOR: este curso é um programa de terminal, como todos"),
            Map.entry("other course here. It teaches HTML; it never opens a page.", "os outros daqui. Ensina HTML; nunca abre uma página."),
            Map.entry("other course here. It teaches CSS; it never opens a page.", "os outros daqui. Ensina CSS; nunca abre uma página."),
            Map.entry("other course here. It teaches Web JavaScript; it never opens a page.", "os outros daqui. Ensina JavaScript web; nunca abre uma página."),
            Map.entry("NO GUI -- this course is a CLI program, same as every other", "SEM INTERFACE GRÁFICA: este curso é um programa de terminal,"),
            Map.entry("course here. It teaches GTK; it does not open a window itself.", "como todos os outros. Ensina GTK; ele próprio não abre janelas."),
            Map.entry("SUMMARY", "RESUMO"),
            Map.entry("BASIC", "BÁSICO"),
            Map.entry("INTERMEDIATE", "INTERMÉDIO"),
            Map.entry("ADVANCED", "AVANÇADO"),
            Map.entry("JAVA COURSE - FROM CLASS Main TO CHECKED EXCEPTIONS", "CURSO DE JAVA"),
            Map.entry("Compiling and printing", "Compilar e escrever"),
            Map.entry("Variables and types", "Variáveis e tipos"),
            Map.entry("Reading input", "Ler entrada"),
            Map.entry("Conditions", "Condições"),
            Map.entry("Loops", "Ciclos"),
            Map.entry("Arrays and Strings", "Arrays e Strings"),
            Map.entry("Methods and overloading", "Métodos e sobrecarga"),
            Map.entry("ArrayList and boxing", "ArrayList e boxing"),
            Map.entry("Classes, interfaces, and real encapsulation", "Classes, interfaces e encapsulamento real"),
            Map.entry("Exceptions: checked vs unchecked", "Exceções: checked e unchecked"),
            Map.entry("The Collections Framework: Map and Set", "A Collections Framework: Map e Set"),
            Map.entry("Generics", "Genéricos"),
            Map.entry("Concurrency", "Concorrência"));

    private static final String[][] SUMMARIES = {
            {},
            {
                "   - javac compila para bytecode; java corre-o na JVM: dois passos",
                "   - o nome de uma classe public tem de ser igual ao do ficheiro, exatamente",
                "   - print não escreve nada a mais; println acrescenta a mudança de linha",
            },
            {
                "   - os primitivos (int, double, boolean, ...) guardam o valor diretamente",
                "   - int / int trunca em direção a zero: converte antes de dividir, não depois",
                "   - == em Integer compara identidade, não valor; em cache só de -128 a 127",
            },
            {
                "   - Scanner lê a entrada; nextLine() lê uma linha, nextInt() um número",
                "   - nextInt() deixa o fim de linha para trás, e o nextLine() seguinte lê vazio",
                "   - a solução é um sc.nextLine() extra para consumir esse fim de linha",
            },
            {
                "   - if / else if / else, a forma de toda a família do C",
                "   - uma condição tem de ser boolean: nada de inteiros verdadeiros, sem exceções",
                "   - if (x = 5) é erro de compilação aqui, não uma gralha em execução como em C/C++",
            },
            {
                "   - while e for funcionam como em toda a família do C aqui",
                "   - o contador de um for vive no ciclo, não se usa depois dele",
                "   - for (T x : colecao) dá valores, sem índice a gerir",
            },
            {
                "   - os arrays têm tamanho fixo; .length é um campo, sem parênteses",
                "   - String é imutável; .length() é um método, com parênteses",
                "   - == compara identidade, não conteúdo: .equals() para o conteúdo, sempre",
            },
            {
                "   - não há sintaxe de argumentos por omissão: a sobrecarga resolve o mesmo",
                "   - as sobrecargas diferem na lista de parâmetros, escolhidas ao compilar",
                "   - cada sobrecarga é um método separado, nada partilhado entre chamadas",
            },
            {
                "   - ArrayList cresce; add/get/size substituem []/.length",
                "   - um tipo genérico tem de ser um objeto: Integer, não int",
                "   - autoboxing/unboxing convertem sozinhos; o tipo continua a ter de ser boxed",
            },
            {
                "   - private é imposto pelo compilador, não uma convenção como o _grade do Python",
                "   - implementar uma interface obriga a implementar todos os métodos, ou não compila",
                "   - coletor de lixo como no Python; try-with-resources dá a limpeza determinística do RAII, opcional",
            },
            {
                "   - try/catch/finally: a forma de todos os outros cursos aqui",
                "   - exceções checked têm de ser apanhadas ou declaradas, imposto ao compilar",
                "   - unchecked (RuntimeException) não exigem nada, como todas em C++/Python/JS",
                "",
                "  Fica assim o curso principal. Coletor de lixo como Python e",
                "  JavaScript, tipagem estática e compilado como C e C++, e o único",
                "  curso aqui em que o próprio compilador impõe tanto o controlo de",
                "  acesso como o tratamento de exceções antes de o programa poder",
                "  correr.",
                "",
                "  O módulo 11 completa a Collections Framework, de que o",
                "  ArrayList do módulo 8 só tocou uma parte: Map e Set.",
            },
            {
                "   - List, Map e Set são interfaces; ArrayList/HashMap/HashSet são",
                "     implementações delas",
                "   - Map.get() numa chave em falta devolve null, não exceção nem um",
                "     valor inserido; getOrDefault() evita o NullPointerException",
                "     que esse null causa depois",
                "   - Set.add() devolve false, sem exceção, quando o valor já lá estava",
                "",
                "  O módulo 12 são genéricos: o mecanismo que deixa existir",
                "  List<String> e List<Integer> a partir de uma classe, a do próprio",
                "  ArrayList, sem escrever uma por tipo.",
            },
            {
                "   - um contentor do tipo Object precisa de um cast em cada leitura, e o cast",
                "     errado é ClassCastException em execução, não erro de compilação",
                "   - uma classe genérica (Box<T>) ou método (<T> firstOf(List<T>)) devolve ao",
                "     compilador essa informação: tipos errados são apanhados antes de o",
                "     programa correr, não três chamadas abaixo em execução",
                "   - um parâmetro limitado (<T extends Comparable<T>>) é como um método",
                "     genérico promete ao compilador que T tem um certo método",
                "   - type erasure significa que os genéricos só existem ao compilar:",
                "     ArrayList<String> e ArrayList<Integer> são a mesma classe em execução",
            },
            {
                "   - new Thread(runnable).start() corre em paralelo, numa thread nova;",
                "     chamar .run() diretamente é uma chamada normal na thread atual",
                "   - duas threads a partilhar um campo simples sem proteção podem ler",
                "     o mesmo valor antes de qualquer uma escrever: uma atualização",
                "     perdida, o exemplo clássico de data race",
                "   - synchronized resolve deixando só uma thread executar a secção",
                "     crítica de cada vez: garante exclusão mútua, não qual vai primeiro",
                "   - uma data race em Java num campo simples não é comportamento",
                "     indefinido como em C, sem corrupção nem crash, mas é um bug real:",
                "     atualizações perdidas e leituras antigas, outro tipo de bug,",
                "     não um seguro",
                "   - join() bloqueia a thread que chama até o alvo terminar; sem ele,",
                "     um resultado lido depois de start() pode ainda não estar pronto",
            },
    };

    private Strings() {
    }

    static String tr(String text) {
        return PT ? TEXT.getOrDefault(text, text) : text;
    }

    static boolean printSummaryPt(int module) {
        if (!PT) {
            return false;
        }
        for (String line : SUMMARIES[module]) {
            System.out.println(line);
        }
        return true;
    }
}
