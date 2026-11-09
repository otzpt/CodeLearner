# strings_pt.sh - Portuguese (PT-PT) text for the menus, prompts and summaries.
# CODELEARNER_LANG=pt-pt turns it on. The lessons themselves stay in English.

ui_lang_pt() {
    [[ "${CODELEARNER_LANG:-}" == "pt-pt" ]]
}

# ui_tr TEXT prints the Portuguese for TEXT in that mode, otherwise TEXT.
ui_tr() {
    if ui_lang_pt; then
        case "$1" in
            'Press ENTER to continue...') printf '%s' 'Prime ENTER para continuar...'; return ;;
            'Your answer:') printf '%s' 'A tua resposta:'; return ;;
            'CORRECT.') printf '%s' 'CERTO.'; return ;;
            'NOT QUITE. The answer is:') printf '%s' 'NÃO É BEM ISSO. A resposta é:'; return ;;
            '>> EXERCISE - MODULE') printf '%s' '>> EXERCÍCIO - MÓDULO'; return ;;
            '>> WRITE THIS YOURSELF, in a real file') printf '%s' '>> ESCREVE ISTO TU, num ficheiro a sério'; return ;;
            '>> TRY THIS YOURSELF, in a real directory') printf '%s' '>> EXPERIMENTA TU, numa pasta a sério'; return ;;
            'Try it with this input:') printf '%s' 'Experimenta com esta entrada:'; return ;;
            'It must print:') printf '%s' 'Tem de escrever:'; return ;;
            'You should see:') printf '%s' 'Deves ver:'; return ;;
            'The result must be:') printf '%s' 'O resultado tem de ser:'; return ;;
            'That output is the whole specification. Any code that') printf '%s' 'Esse resultado é toda a especificação. Qualquer código que'; return ;;
            'That output is the whole specification. Any code') printf '%s' 'Esse resultado é toda a especificação. Qualquer código'; return ;;
            'That output is the whole specification. Any commands that') printf '%s' 'Esse resultado é toda a especificação. Quaisquer comandos que'; return ;;
            'That result is the whole specification. Anything that') printf '%s' 'Esse resultado é toda a especificação. Tudo o que'; return ;;
            'produces it is correct.') printf '%s' 'o produza está certo.'; return ;;
            'that produces it is correct.') printf '%s' 'que o produza está certo.'; return ;;
            'produce it are correct.') printf '%s' 'o produzam estão certos.'; return ;;
            'produces it is correct. Open your file in whatever browser') printf '%s' 'o produza está certo. Abre o teu ficheiro no navegador'; return ;;
            'you normally use; this course never opens one.') printf '%s' 'que costumas usar; este curso nunca abre nenhum.'; return ;;
            'Try it first. Compile with:') printf '%s' 'Tenta primeiro. Compila com:'; return ;;
            'Try it first. Run with:') printf '%s' 'Tenta primeiro. Corre com:'; return ;;
            'Try it first. Compile and run with:') printf '%s' 'Tenta primeiro. Compila e corre com:'; return ;;
            'Want to see example code?') printf '%s' 'Queres ver código de exemplo?'; return ;;
            'Want to see example commands?') printf '%s' 'Queres ver comandos de exemplo?'; return ;;
            'This is EXAMPLE CODE, not the answer. It is one way to get') printf '%s' 'Isto é CÓDIGO DE EXEMPLO, não a resposta. É uma forma de chegar'; return ;;
            'This is EXAMPLE CODE, not the answer. It is one way to') printf '%s' 'Isto é CÓDIGO DE EXEMPLO, não a resposta. É uma forma de'; return ;;
            'that output; yours may look nothing like it and still be') printf '%s' 'a esse resultado; o teu pode ser muito diferente e estar'; return ;;
            'that result; yours may look nothing like it and still be') printf '%s' 'a esse resultado; o teu pode ser muito diferente e estar'; return ;;
            'right -- or better. Compare the output, not the code.') printf '%s' 'certo, ou melhor. Compara o resultado, não o código.'; return ;;
            'right -- or better.') printf '%s' 'certo, ou melhor.'; return ;;
            'get that output; yours may look nothing like it and') printf '%s' 'chegar a esse resultado; o teu pode ser muito diferente e'; return ;;
            'still be right -- or better. Compare the output, not') printf '%s' 'estar certo, ou melhor. Compara o resultado, não'; return ;;
            'the code.') printf '%s' 'o código.'; return ;;
            'This is EXAMPLE, not the answer. It is one way to reach that') printf '%s' 'Isto é um EXEMPLO, não a resposta. É uma forma de chegar a esse'; return ;;
            'result; a different sequence of commands landing on the same') printf '%s' 'resultado; outra sequência de comandos que chegue ao mesmo'; return ;;
            'state is correct too.') printf '%s' 'estado também está certa.'; return ;;
            'Quit') printf '%s' 'Sair'; return ;;
            'Pick a module:') printf '%s' 'Escolhe um módulo:'; return ;;
            'Not a valid option.') printf '%s' 'Opção inválida.'; return ;;
            'See you next time.') printf '%s' 'Até à próxima.'; return ;;
            'Something went wrong:') printf '%s' 'Algo correu mal:'; return ;;
            'NO BROWSER -- this course is a CLI program, same as every') printf '%s' 'SEM NAVEGADOR: este curso é um programa de terminal, como todos'; return ;;
            'other course here. It teaches HTML; it never opens a page.') printf '%s' 'os outros daqui. Ensina HTML; nunca abre uma página.'; return ;;
            'other course here. It teaches CSS; it never opens a page.') printf '%s' 'os outros daqui. Ensina CSS; nunca abre uma página.'; return ;;
            'other course here. It teaches Web JavaScript; it never opens a page.') printf '%s' 'os outros daqui. Ensina JavaScript web; nunca abre uma página.'; return ;;
            'NO GUI -- this course is a CLI program, same as every other') printf '%s' 'SEM INTERFACE GRÁFICA: este curso é um programa de terminal,'; return ;;
            'course here. It teaches GTK; it does not open a window itself.') printf '%s' 'como todos os outros. Ensina GTK; ele próprio não abre janelas.'; return ;;
            'SUMMARY') printf '%s' 'RESUMO'; return ;;
            'BASIC') printf '%s' 'BÁSICO'; return ;;
            'INTERMEDIATE') printf '%s' 'INTERMÉDIO'; return ;;
            'ADVANCED') printf '%s' 'AVANÇADO'; return ;;
            'GIT COURSE') printf '%s' 'CURSO DE GIT'; return ;;
            'What is Git?') printf '%s' 'O que é o Git?'; return ;;
            'git clone') printf '%s' 'git clone'; return ;;
            'git status') printf '%s' 'git status'; return ;;
            'git add') printf '%s' 'git add'; return ;;
            'git commit') printf '%s' 'git commit'; return ;;
            'git push') printf '%s' 'git push'; return ;;
            'git pull') printf '%s' 'git pull'; return ;;
            'Branches') printf '%s' 'Branches'; return ;;
            'Releases') printf '%s' 'Releases'; return ;;
            'GitHub Actions') printf '%s' 'GitHub Actions'; return ;;
        esac
    fi
    printf '%s' "$1"
}

# Prints a module's summary in Portuguese. Fails in English, so a caller can
# fall back to its own text.
ui_summary_pt() {
    ui_lang_pt || return 1
    case "$1" in
        1) cat <<'EOF'
  - Uma alteração passa de diretório de trabalho -> staging -> repositório.
  - `git init` cria .git/. Sozinho não acompanha nada.
  - `git status` diz em qual das três áreas está cada coisa.
  - Este curso fixa -b main em todo o lado, porque o predefinido real
    depende da versão e da configuração do git.
EOF
            ;;
        2) cat <<'EOF'
  - clone copia o histórico todo, não só os ficheiros mais recentes.
  - Cria sempre uma pasta nova; nunca se junta a uma existente.
  - O remoto de onde veio chama-se origin, por convenção, automaticamente.
  - `git remote -v` mostra para onde o origin aponta.
EOF
            ;;
        3) cat <<'EOF'
  - status nunca muda nada, só informa.
  - Um ficheiro não acompanhado, uma alteração em staging e uma fora de
    staging são três secções diferentes da mesma saída. Lê a secção,
    não só o nome do ficheiro.
  - O staging é uma fotografia, tirada uma vez, no momento do `git add`.
EOF
            ;;
        4) cat <<'EOF'
  - `git add <caminho>` põe em staging um ficheiro ou pasta; `git add .`
    tudo o que está abaixo da pasta atual.
  - O staging copia o conteúdo naquele instante. Editar depois não
    atualiza a fotografia.
  - `git restore --staged <ficheiro>` tira do staging sem tocar no ficheiro.
EOF
            ;;
        5) cat <<'EOF'
  - Um commit guarda para sempre o que estiver em staging.
  - O git recusa o commit quando não há nada em staging. Não é um bug.
  - `-a` põe em staging só alterações a ficheiros já acompanhados, nunca novos.
  - `git log --oneline` é a forma rápida de ver o histórico que estás a criar.
EOF
            ;;
        6) cat <<'EOF'
  - Os commits são locais até ao push.
  - O push é rejeitado, não combinado, quando o remoto tem commits que não tens.
  - A resposta segura a um push rejeitado é pull, não --force.
  - --force substitui o histórico remoto pelo teu. Pode apagar trabalho real.
EOF
            ;;
        7) cat <<'EOF'
  - pull = fetch + merge num só comando.
  - Sem sobreposição -> o merge é automático e silencioso.
  - Linhas sobrepostas -> o git deixa marcas <<<<<<< / ======= / >>>>>>>
    no ficheiro e para para tu decidires.
  - Resolve editando o ficheiro, depois add, depois commit: os mesmos dois
    comandos de qualquer outra alteração.
EOF
            ;;
        8) cat <<'EOF'
  - Um branch é um apontador móvel para um commit; criar um é instantâneo.
  - `git switch -c <nome>` cria um branch e muda para ele num só passo.
  - Sem commits novos no destino -> fast-forward, sem commit de merge.
  - Ambos os lados mudaram -> um merge a sério, e possivelmente as mesmas
    marcas de conflito e resolução com add/commit do módulo 7.
EOF
            ;;
        9) cat <<'EOF'
  - Uma tag nomeia um commit exato, para sempre; um branch continua a andar.
  - Lightweight = um simples apontador. Annotated (-a -m) = um objeto real
    com mensagem e autor; usa annotated nas versões.
  - As tags são locais até ao push, como os commits, e precisam do seu
    próprio `git push origin <tag>`.
  - Um GitHub Release é uma tag mais notas e anexos acrescentados pelo
    GitHub, não algo que o próprio git guarde.
EOF
            ;;
        10) cat <<'EOF'
  - Um workflow é só um .yml em .github/workflows/, com commit e push
    como qualquer outro ficheiro, nada de específico do git.
  - on: decide o que o dispara: um push, um pull request, uma tag que
    corresponda a um padrão.
  - jobs: -> steps:, de cima para baixo, numa máquina nova a cada execução.
  - Corre nos servidores do GitHub assim que chega um push correspondente,
    não há um comando separado para o disparar.
EOF
            ;;
    esac
}
