//! Portuguese (PT-PT) text for the menus, prompts and module summaries.
//! CODELEARNER_LANG=pt-pt turns it on. The lessons themselves stay in English.

use std::env;

const TEXT: &[(&str, &str)] = &[
    ("Press ENTER to continue...", "Prime ENTER para continuar..."),
    ("Your answer:", "A tua resposta:"),
    ("CORRECT.", "CERTO."),
    ("NOT QUITE. The answer is:", "NÃO É BEM ISSO. A resposta é:"),
    (">> EXERCISE - MODULE", ">> EXERCÍCIO - MÓDULO"),
    (">> WRITE THIS YOURSELF, in a real file", ">> ESCREVE ISTO TU, num ficheiro a sério"),
    (">> TRY THIS YOURSELF, in a real directory", ">> EXPERIMENTA TU, numa pasta a sério"),
    ("Try it with this input:", "Experimenta com esta entrada:"),
    ("It must print:", "Tem de escrever:"),
    ("You should see:", "Deves ver:"),
    ("The result must be:", "O resultado tem de ser:"),
    ("That output is the whole specification. Any code that", "Esse resultado é toda a especificação. Qualquer código que"),
    ("That output is the whole specification. Any code", "Esse resultado é toda a especificação. Qualquer código"),
    ("That output is the whole specification. Any commands that", "Esse resultado é toda a especificação. Quaisquer comandos que"),
    ("That result is the whole specification. Anything that", "Esse resultado é toda a especificação. Tudo o que"),
    ("produces it is correct.", "o produza está certo."),
    ("that produces it is correct.", "que o produza está certo."),
    ("produce it are correct.", "o produzam estão certos."),
    ("produces it is correct. Open your file in whatever browser", "o produza está certo. Abre o teu ficheiro no navegador"),
    ("you normally use; this course never opens one.", "que costumas usar; este curso nunca abre nenhum."),
    ("Try it first. Compile with:", "Tenta primeiro. Compila com:"),
    ("Try it first. Run with:", "Tenta primeiro. Corre com:"),
    ("Try it first. Compile and run with:", "Tenta primeiro. Compila e corre com:"),
    ("Want to see example code?", "Queres ver código de exemplo?"),
    ("Want to see example commands?", "Queres ver comandos de exemplo?"),
    ("This is EXAMPLE CODE, not the answer. It is one way to get", "Isto é CÓDIGO DE EXEMPLO, não a resposta. É uma forma de chegar"),
    ("This is EXAMPLE CODE, not the answer. It is one way to", "Isto é CÓDIGO DE EXEMPLO, não a resposta. É uma forma de"),
    ("that output; yours may look nothing like it and still be", "a esse resultado; o teu pode ser muito diferente e estar"),
    ("that result; yours may look nothing like it and still be", "a esse resultado; o teu pode ser muito diferente e estar"),
    ("right -- or better. Compare the output, not the code.", "certo, ou melhor. Compara o resultado, não o código."),
    ("right -- or better.", "certo, ou melhor."),
    ("get that output; yours may look nothing like it and", "chegar a esse resultado; o teu pode ser muito diferente e"),
    ("still be right -- or better. Compare the output, not", "estar certo, ou melhor. Compara o resultado, não"),
    ("the code.", "o código."),
    ("This is EXAMPLE, not the answer. It is one way to reach that", "Isto é um EXEMPLO, não a resposta. É uma forma de chegar a esse"),
    ("result; a different sequence of commands landing on the same", "resultado; outra sequência de comandos que chegue ao mesmo"),
    ("state is correct too.", "estado também está certa."),
    ("Quit", "Sair"),
    ("Pick a module:", "Escolhe um módulo:"),
    ("Not a valid option.", "Opção inválida."),
    ("See you next time.", "Até à próxima."),
    ("Something went wrong:", "Algo correu mal:"),
    ("NO BROWSER -- this course is a CLI program, same as every", "SEM NAVEGADOR: este curso é um programa de terminal, como todos"),
    ("other course here. It teaches HTML; it never opens a page.", "os outros daqui. Ensina HTML; nunca abre uma página."),
    ("other course here. It teaches CSS; it never opens a page.", "os outros daqui. Ensina CSS; nunca abre uma página."),
    ("other course here. It teaches Web JavaScript; it never opens a page.", "os outros daqui. Ensina JavaScript web; nunca abre uma página."),
    ("NO GUI -- this course is a CLI program, same as every other", "SEM INTERFACE GRÁFICA: este curso é um programa de terminal,"),
    ("course here. It teaches GTK; it does not open a window itself.", "como todos os outros. Ensina GTK; ele próprio não abre janelas."),
    ("SUMMARY", "RESUMO"),
    ("BASIC", "BÁSICO"),
    ("INTERMEDIATE", "INTERMÉDIO"),
    ("ADVANCED", "AVANÇADO"),
    ("RUST COURSE - FROM println! TO MEMORY", "CURSO DE RUST"),
    ("Compiling and println!", "Compilar e println!"),
    ("Variables and types", "Variáveis e tipos"),
    ("Control flow", "Controlo de fluxo"),
    ("Functions", "Funções"),
    ("Ownership", "Ownership"),
    ("References and borrowing", "Referências e borrowing"),
    ("Memory: stack, heap, raw pointers", "Memória: pilha, heap, apontadores crus"),
    ("Structs and methods", "Structs e métodos"),
    ("Enums, match and Option", "Enums, match e Option"),
    ("Result and the ? operator", "Result e o operador ?"),
    ("Rust meets hardware", "O Rust encontra o hardware"),
    ("Inline assembly and NASM", "Assembly inline e NASM"),
];

const SUMMARIES: &[(u32, &[&str])] = &[
    (1, &[
        "   - rustc file.rs cria um programa nativo; fn main() arranca-o",
        "   - println! é uma macro; a string de formato é verificada ao compilar",
        "   - {} para mostrar, {:?} para depurar, {:>6} {:06} {:.2} {:x} {:b} para o aspeto",
        "   - {name} lê uma variável; {{ }} escreve chavetas",
        "",
        "  Módulo 2: variáveis, e o que o Rust recusa deixar-te mudar.",
    ]),
    (2, &[
        "   - let é imutável; mut permite mudar; shadowing cria uma variável nova",
        "   - os inteiros dizem o seu tamanho; overflow dá panic numa compilação de depuração",
        "   - checked_, wrapping_, saturating_ dizem o que fazer no limite",
        "   - não há conversão implícita de números; as converte e pode perder dados",
        "",
        "  Módulo 3: tomar decisões e repetir.",
    ]),
    (3, &[
        "   - if é uma expressão; a condição tem de ser bool",
        "   - for em intervalos (1..4, 1..=4), while, e loop com valor no break",
        "   - match tem de cobrir todos os casos; _ é 'o resto'",
        "",
        "  Módulo 4: funções.",
    ]),
    (4, &[
        "   - fn nome(param: Tipo) -> Tipo; a última expressão é o resultado",
        "   - um ponto e vírgula a mais nessa última linha estraga o tipo de retorno",
        "   - tuplos devolvem vários valores; () é 'nada'",
        "   - const para um valor fixo ao compilar",
        "",
        "  Módulo 5: ownership, a ideia em que assenta o resto do Rust.",
    ]),
    (5, &[
        "   - um dono; libertado ao sair de âmbito; não há free() para esquecer",
        "   - atribuir ou passar uma String move-a; o nome antigo deixa de valer",
        "   - um move copia o cabeçalho de 3 palavras, não os bytes da heap",
        "   - .clone() é a cópia profunda explícita",
        "",
        "  Módulo 6: borrowing, usar um valor sem ficar com ele.",
    ]),
    (6, &[
        "   - &valor empresta só para leitura; &mut valor empresta com licença para mudar",
        "   - muitos & OU um &mut, verificado ao compilar",
        "   - uma referência não pode durar mais do que o valor: sem apontadores pendentes",
        "   - um slice (&str, &[T]) é um apontador e um comprimento",
        "",
        "  Módulo 7: como tudo isto fica na memória.",
    ]),
    (7, &[
        "   - size_of::<T>(); String e Vec são um cabeçalho de 3 palavras sobre dados na heap",
        "   - um Vec realoca ao crescer; with_capacity evita-o",
        "   - v[i] verifica os limites e dá panic; .get(i) devolve um Option",
        "   - apontadores crus são os do C; usá-los exige unsafe",
        "",
        "  Módulo 8: structs, para agrupar valores e dar-lhes métodos.",
    ]),
    (8, &[
        "   - struct agrupa campos com nome; a mutabilidade é da variável inteira",
        "   - impl acrescenta métodos: &self lê, &mut self muda, self consome",
        "   - #[derive(Debug, Clone, Copy)] escreve o código aborrecido por ti",
        "   - #[repr(C)] fixa a disposição; uma tuple struct cria um tipo distinto",
        "",
        "  Módulo 9: enums, e um valor que pode faltar.",
    ]),
    (9, &[
        "   - enum: um conjunto fixo de variantes; match tem de as cobrir todas",
        "   - as variantes podem levar dados: Move(i32), Turn { degrees: i32 }",
        "   - Option<T> substitui null: Some(valor) ou None, tratado antes de usar",
        "   - unwrap() dá panic em None; unwrap_or, if let e match não",
        "",
        "  Módulo 10: Result, para operações que podem falhar.",
    ]),
    (10, &[
        "   - Result<T, E> é Ok(valor) ou Err(motivo); o tipo obriga a decidir",
        "   - ? devolve o Err a quem chama, ou desembrulha o Ok",
        "   - trata as falhas recuperáveis; unwrap/expect só para bugs",
        "   - parse() distingue um número inválido de um zero, ao contrário de parseInt()",
        "",
        "  Módulo 11: o Rust encontra o hardware.",
    ]),
    (11, &[
        "   - um registo é uma palavra de 32 bits num endereço; o Rust chega lá com",
        "     apontadores crus e read_volatile / write_volatile",
        "   - volatile impede o compilador de apagar ou juntar escritas em registos",
        "   - set_bit é ler, modificar, escrever: três instruções, não atómico",
        "   - aliases atómicos do RP2040: +0x1000 XOR, +0x2000 SET, +0x3000 CLR",
        "   - código #![no_std] só usa core; rp-hal embrulha este unsafe em tipos seguros",
        "",
        "  Módulo 12: escrever as próprias instruções do CPU dentro do Rust.",
    ]),
    (12, &[
        "   - asm! fica em unsafe; in / out / inout nomeiam as variáveis Rust",
        "   - sintaxe Intel por omissão em x86; options diz o que o asm evita",
        "   - nada verifica um apontador ou um clobber por declarar a não ser tu",
        "   - ficheiros NASM ligam-se através de extern \"C\" e do ABI System V",
        "",
        "  É o curso até agora. O ROADMAP lista lifetimes, traits,",
        "  genéricos e async como os módulos seguintes.",
    ]),
];

fn pt_mode() -> bool {
    env::var("CODELEARNER_LANG").is_ok_and(|value| value == "pt-pt")
}

/// The Portuguese text for `text` in that mode, otherwise `text` itself.
pub fn tr(text: &str) -> &str {
    if pt_mode() {
        if let Some((_, translated)) = TEXT.iter().find(|(english, _)| *english == text) {
            return translated;
        }
    }
    text
}

/// Prints a module's summary in Portuguese and returns true, or returns false.
pub fn print_summary_pt(module: u32) -> bool {
    if !pt_mode() {
        return false;
    }
    if let Some((_, lines)) = SUMMARIES.iter().find(|(number, _)| *number == module) {
        for line in lines.iter() {
            println!("{line}");
        }
    }
    true
}
