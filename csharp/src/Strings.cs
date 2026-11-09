// Portuguese (PT-PT) text for the menus, prompts and module summaries.
// CODELEARNER_LANG=pt-pt turns it on. The lessons themselves stay in English.

static class Strings
{
    static readonly bool Pt = Environment.GetEnvironmentVariable("CODELEARNER_LANG") == "pt-pt";

    static readonly Dictionary<string, string> Text = new()
    {
        ["Press ENTER to continue..."] = "Prime ENTER para continuar...",
        ["Your answer:"] = "A tua resposta:",
        ["CORRECT."] = "CERTO.",
        ["NOT QUITE. The answer is:"] = "NÃO É BEM ISSO. A resposta é:",
        [">> EXERCISE - MODULE"] = ">> EXERCÍCIO - MÓDULO",
        [">> WRITE THIS YOURSELF, in a real file"] = ">> ESCREVE ISTO TU, num ficheiro a sério",
        [">> TRY THIS YOURSELF, in a real directory"] = ">> EXPERIMENTA TU, numa pasta a sério",
        ["Try it with this input:"] = "Experimenta com esta entrada:",
        ["It must print:"] = "Tem de escrever:",
        ["You should see:"] = "Deves ver:",
        ["The result must be:"] = "O resultado tem de ser:",
        ["That output is the whole specification. Any code that"] = "Esse resultado é toda a especificação. Qualquer código que",
        ["That output is the whole specification. Any code"] = "Esse resultado é toda a especificação. Qualquer código",
        ["That output is the whole specification. Any commands that"] = "Esse resultado é toda a especificação. Quaisquer comandos que",
        ["That result is the whole specification. Anything that"] = "Esse resultado é toda a especificação. Tudo o que",
        ["produces it is correct."] = "o produza está certo.",
        ["that produces it is correct."] = "que o produza está certo.",
        ["produce it are correct."] = "o produzam estão certos.",
        ["produces it is correct. Open your file in whatever browser"] = "o produza está certo. Abre o teu ficheiro no navegador",
        ["you normally use; this course never opens one."] = "que costumas usar; este curso nunca abre nenhum.",
        ["Try it first. Compile with:"] = "Tenta primeiro. Compila com:",
        ["Try it first. Run with:"] = "Tenta primeiro. Corre com:",
        ["Try it first. Compile and run with:"] = "Tenta primeiro. Compila e corre com:",
        ["Want to see example code?"] = "Queres ver código de exemplo?",
        ["Want to see example commands?"] = "Queres ver comandos de exemplo?",
        ["This is EXAMPLE CODE, not the answer. It is one way to get"] = "Isto é CÓDIGO DE EXEMPLO, não a resposta. É uma forma de chegar",
        ["This is EXAMPLE CODE, not the answer. It is one way to"] = "Isto é CÓDIGO DE EXEMPLO, não a resposta. É uma forma de",
        ["that output; yours may look nothing like it and still be"] = "a esse resultado; o teu pode ser muito diferente e estar",
        ["that result; yours may look nothing like it and still be"] = "a esse resultado; o teu pode ser muito diferente e estar",
        ["right -- or better. Compare the output, not the code."] = "certo, ou melhor. Compara o resultado, não o código.",
        ["right -- or better."] = "certo, ou melhor.",
        ["get that output; yours may look nothing like it and"] = "chegar a esse resultado; o teu pode ser muito diferente e",
        ["still be right -- or better. Compare the output, not"] = "estar certo, ou melhor. Compara o resultado, não",
        ["the code."] = "o código.",
        ["This is EXAMPLE, not the answer. It is one way to reach that"] = "Isto é um EXEMPLO, não a resposta. É uma forma de chegar a esse",
        ["result; a different sequence of commands landing on the same"] = "resultado; outra sequência de comandos que chegue ao mesmo",
        ["state is correct too."] = "estado também está certa.",
        ["Quit"] = "Sair",
        ["Pick a module:"] = "Escolhe um módulo:",
        ["Not a valid option."] = "Opção inválida.",
        ["See you next time."] = "Até à próxima.",
        ["Something went wrong:"] = "Algo correu mal:",
        ["NO BROWSER -- this course is a CLI program, same as every"] = "SEM NAVEGADOR: este curso é um programa de terminal, como todos",
        ["other course here. It teaches HTML; it never opens a page."] = "os outros daqui. Ensina HTML; nunca abre uma página.",
        ["other course here. It teaches CSS; it never opens a page."] = "os outros daqui. Ensina CSS; nunca abre uma página.",
        ["other course here. It teaches Web JavaScript; it never opens a page."] = "os outros daqui. Ensina JavaScript web; nunca abre uma página.",
        ["NO GUI -- this course is a CLI program, same as every other"] = "SEM INTERFACE GRÁFICA: este curso é um programa de terminal,",
        ["course here. It teaches GTK; it does not open a window itself."] = "como todos os outros. Ensina GTK; ele próprio não abre janelas.",
        ["SUMMARY"] = "RESUMO",
        ["BASIC"] = "BÁSICO",
        ["INTERMEDIATE"] = "INTERMÉDIO",
        ["ADVANCED"] = "AVANÇADO",
        ["C# COURSE - FROM TOP-LEVEL STATEMENTS TO IDISPOSABLE"] = "CURSO DE C#",
        ["Compiling and printing"] = "Compilar e escrever",
        ["Variables and types"] = "Variáveis e tipos",
        ["Reading input"] = "Ler entrada",
        ["Conditions and pattern matching"] = "Condições e pattern matching",
        ["Loops"] = "Ciclos",
        ["Arrays and List<T>"] = "Arrays e List<T>",
        ["Methods and default parameters"] = "Métodos e parâmetros por omissão",
        ["Classes, structs, and records"] = "Classes, structs e records",
        ["Nullable reference types"] = "Tipos de referência anuláveis",
        ["Exceptions and IDisposable"] = "Exceções e IDisposable",
        ["LINQ"] = "LINQ",
        ["Async/await"] = "Async/await",
        ["Generics and interfaces in depth"] = "Genéricos e interfaces a fundo",
        ["Delegates and events"] = "Delegates e eventos",
    };

    static readonly Dictionary<int, string[]> Summaries = new()
    {
        [1] = new[]
        {
            "  - As instruções de topo são açúcar sobre class Program { static",
            "    void Main(...) }, não um modelo de execução diferente.",
            "  - dotnet run = dotnet build (código -> IL numa .dll) + arranque",
            "    pelo runtime, num só comando.",
            "  - Write não acrescenta fim de linha; WriteLine acrescenta.",
        },
        [2] = new[]
        {
            "  - var infere um tipo real e fixo: não é tipagem dinâmica.",
            "  - int / int trunca; faz primeiro de um dos lados um double.",
            "  - Tipos de valor (int, struct) SÃO os seus dados; tipos de referência",
            "    (string, class) guardam um apontador para eles. O módulo 8 mostra",
            "    porque é que essa diferença importa.",
        },
        [3] = new[]
        {
            "  - Console.ReadLine() devolve string?, não string: pode ser null",
            "    no fim da entrada.",
            "  - ?? dá uma alternativa para o caso null.",
            "  - int.TryParse devolve bool e um parâmetro out: sem exceção para",
            "    entrada que simplesmente não é um número.",
        },
        [4] = new[]
        {
            "  - Uma condição tem de ser bool: if (x = 5) não compila.",
            "  - As expressões switch têm um valor; _ apanha o resto.",
            "  - `is` verifica um tipo e liga uma variável num só passo, sem",
            "    cast que lance exceção.",
        },
        [5] = new[]
        {
            "  - <= corre mais uma iteração do que <: lê a condição, não",
            "    contes a olho.",
            "  - foreach lê uma sequência; o operador de intervalo (1..4) corta,",
            "    não é ele próprio percorrível com foreach.",
            "  - a variável de iteração do foreach é só de leitura; usa um for",
            "    com índice para mudar elementos.",
        },
        [6] = new[]
        {
            "  - Os arrays têm tamanho fixo; List<T> cresce. Usa List<T> por",
            "    omissão.",
            "  - .Length é sempre uma propriedade aqui, em arrays e strings:",
            "    sem parênteses, ao contrário do String.length() do Java.",
            "  - Os arrays comparam por referência (== é falso com conteúdo",
            "    igual); string sobrecarrega == para comparar conteúdo:",
            "    comportamentos opostos do mesmo operador.",
        },
        [7] = new[]
        {
            "  - C# tem valores por omissão; o módulo 7 de Java usa sobrecarga",
            "    para o mesmo porque não tem alternativa.",
            "  - A sobrecarga é resolvida pelos tipos dos parâmetros ao compilar.",
            "  - nome: numa chamada deixa os argumentos virem fora de ordem.",
        },
        [8] = new[]
        {
            "  - Atribuir uma struct copia o valor; atribuir uma class copia",
            "    a referência. É a divisão do módulo 2 a tornar-se real.",
            "  - record é um tipo de referência com igualdade por valor gerada",
            "    pelo compilador; uma class simples precisa disso à mão.",
            "  - private é imposto pelo compilador, como em Java, ao contrário",
            "    da convenção de nomes do Python.",
        },
        [9] = new[]
        {
            "  - string significa não anulável por convenção; string? significa",
            "    que pode mesmo ser null, imposto só ao compilar.",
            "  - ?. curto-circuita para null em vez de lançar; ?? dá uma",
            "    alternativa para esse null.",
            "  - ! cala o aviso sem verificar nada: ainda pode lançar exceção",
            "    em execução se estiveres enganado.",
        },
        [10] = new[]
        {
            "  - C# não tem exceções checked: todas se comportam como a",
            "    RuntimeException unchecked do Java.",
            "  - Parse lança com entrada inválida; TryParse devolve bool.",
            "    Escolhe conforme essa entrada seja excecional ou de rotina.",
            "  - using garante que Dispose() corre de forma determinística à",
            "    saída do bloco, não quando o GC calhar.",
        },
        [11] = new[]
        {
            "  - Where filtra, Select transforma: encadeia-os em vez de aninhar ciclos",
            "  - Sum, Count, OrderBy(Descending) substituem os ciclos que reduzem uma",
            "    lista a um valor ou a uma cópia ordenada",
            "  - uma consulta LINQ é preguiçosa: corre ao ser percorrida, não ao ser",
            "    criada; .ToList() força-a a correr já, uma vez, se é isso que queres",
            "",
            "  O módulo 12 é async/await, comparado diretamente com a versão",
            "  da mesma ideia em JavaScript, com Promises.",
        },
        [12] = new[]
        {
            "  - uma Task está pendente, e depois conclui ou falha uma vez: a mesma",
            "    forma de uma Promise de JavaScript, com outro nome",
            "  - Task.Delay não bloqueia a thread como Thread.Sleep; só o await",
            "    espera de facto",
            "  - await é contagioso: um método com await tem de ser async, e",
            "    também todos os que o chamam, até ao ponto de entrada",
            "  - Task.WhenAll corre awaits independentes em simultâneo em vez de",
            "    um a seguir ao outro",
            "  - uma Task falhada com await lança exceção, apanhada com o mesmo",
            "    try/catch do módulo 10, sem mecanismo novo",
            "",
            "  O módulo 13 são genéricos e interfaces a fundo: um método genérico",
            "  com uma restrição real e métodos de interface por omissão, além",
            "  do que os records e structs do módulo 8 já mostraram.",
        },
        [13] = new[]
        {
            "  - um T sem restrições só tem o que todos os tipos têm (os membros",
            "    de Object): CompareTo precisa primeiro de where T : IComparable<T>",
            "  - essa restrição aceita qualquer tipo que implemente IComparable<T>,",
            "    de valor ou de referência: int e string servem ambos",
            "  - desde o C# 8, um método de interface pode ter corpo: uma",
            "    implementação por omissão, herdada se a classe não a substituir",
            "  - um corpo por omissão só se resolve através de uma referência do",
            "    tipo da interface, não pela classe concreta, mesmo que a implemente",
            "  - o Java real também tem métodos por omissão desde o Java 8; o",
            "    módulo 9 de Java deste curso só mostra a metade do contrato puro",
            "",
            "  O módulo 14 são delegates e eventos: passar comportamento como",
            "  valor, e a armadilha que faz de event uma palavra-chave própria",
            "  em vez de um simples campo delegate.",
        },
        [14] = new[]
        {
            "  - Action<T> e Func<T,R> passam comportamento como valor: um",
            "    método ou lambda guardado por referência, chamado quando o recetor decidir",
            "  - delegate dá nome a uma assinatura que Action/Func descreveriam de",
            "    forma genérica, e é o tipo que um event tem de ter",
            "  - um campo delegate simples pode ser reatribuído ou anulado por qualquer",
            "    código com acesso a ele; event limita o exterior a += e -=",
            "  - ?.Invoke salta a chamada quando ainda ninguém subscreveu, em vez de",
            "    lançar NullReferenceException num delegate null",
        },
    };

    public static string Tr(string text) => Pt && Text.TryGetValue(text, out string? pt) ? pt : text;

    public static bool PrintSummaryPt(int module)
    {
        if (!Pt)
        {
            return false;
        }
        foreach (string line in Summaries[module])
        {
            Console.WriteLine(line);
        }
        return true;
    }
}
