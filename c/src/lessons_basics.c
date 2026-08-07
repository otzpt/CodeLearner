/*
 * Modules 1 to 5 - the basics.
 *
 * Rule used throughout: nothing is claimed without being shown. Where an
 * example can run here, it runs.
 */

#include <stdio.h>
#include <string.h>

#include "lessons.h"
#include "ui.h"

void lesson_01_compiling(void)
{
    title("MODULE 1 - COMPILING AND PRINTING");

    heading("PART 1: C does not run, it compiles");

    printf("  Python reads your file and executes it. C does not.\n");
    printf("  The compiler translates your text into processor\n");
    printf("  instructions and writes a new file. Only then is there\n");
    printf("  something to run.\n\n");

    printf("    program.c    ->   [ gcc ]   ->   ./program\n");
    printf("    what you write    translates     what runs\n\n");

    printf("  This has one good consequence: mistakes show up at compile\n");
    printf("  time, before the program reaches anybody.\n");

    wait_enter();
    clear_screen();
    heading("PART 2: the smallest program that works");

    printf("    #include <stdio.h>\n");
    printf("\n");
    printf("    int main(void)\n");
    printf("    {\n");
    printf("        printf(\"Hello world\\n\");\n");
    printf("        return 0;\n");
    printf("    }\n\n");

    printf("  Line by line:\n\n");
    printf("   #include <stdio.h>   brings in printf. Without it the\n");
    printf("                        compiler does not know what printf is.\n");
    printf("   int main(void)       where the program starts. Required.\n");
    printf("                        You will also see int main(). Both are\n");
    printf("                        legal here; module 7 shows why (void)\n");
    printf("                        is the better habit everywhere else.\n");
    printf("   printf(...)          writes to the screen.\n");
    printf("   \\n                   newline. Without it everything runs on.\n");
    printf("   return 0             0 means it went fine.\n");

    wait_enter();
    clear_screen();
    heading("PART 3: compiling and running");

    printf("    gcc program.c -o program\n");
    printf("    ./program\n\n");

    printf("   -o program    names the file to create. Without it gcc\n");
    printf("                 writes a file called a.out.\n");
    printf("   ./            'in this folder'. Without it the system looks\n");
    printf("                 in the system folders and does not find it.\n\n");

    printf("  Always compile with warnings on:\n\n");
    printf("    gcc -Wall -Wextra program.c -o program\n\n");
    printf("  These two catch real bugs. This course is built that way.\n");

    wait_enter();
    clear_screen();
    exercise(1);

    question("printf(\"Hello\"); printf(\"world\");  -- does that print on\n"
             "  one line or two? (answer: one or two)",
             "one",
             "printf does not add a newline. You get Helloworld, joined up.");

    question("Which gcc option names the file it creates?",
             "-o",
             "Without -o, gcc writes a file called a.out.");

    {
        const char *task[] = {
            "Write a program that prints your name and the year,",
            "each on its own line.",
        };
        const char *expected[] = {
            "Jose",
            "2026",
        };
        const char *solution[] = {
            "#include <stdio.h>",
            "",
            "int main(void)",
            "{",
            "    printf(\"Jose\\n\");",
            "    printf(\"2026\\n\");",
            "    return 0;",
            "}",
        };
        challenge(task, 2, expected, 2, solution, 8);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    printf("   - C compiles before it runs\n");
    printf("   - every program starts at main\n");
    printf("   - printf writes, \\n breaks the line\n");
    printf("   - -Wall -Wextra, always\n\n");

    printf("  Module 2: storing values in variables.\n");
    wait_enter();
}

void lesson_02_variables(void)
{
    title("MODULE 2 - VARIABLES AND TYPES");

    heading("PART 1: in C, the type comes first");

    printf("  In Python you write  age = 25.\n");
    printf("  In C you must say what kind of value the box holds:\n\n");

    printf("    int   age    = 25;\n");
    printf("    float height = 1.75f;\n");
    printf("    char  letter = 'A';\n\n");

    printf("  Why? Because the compiler needs to know how many bytes to\n");
    printf("  reserve and how to read them back. An int is a pattern of\n");
    printf("  bits read as a whole number; a float is the same amount of\n");
    printf("  bits read a different way.\n\n");

    printf("  A variable holds the type you declared, forever.\n");

    wait_enter();
    clear_screen();
    heading("PART 2: the types you will use");

    printf("    type     holds                  example\n");
    rule();
    printf("    int      whole numbers          -3, 0, 25\n");
    printf("    float    decimals               1.75f\n");
    printf("    double   decimals, more digits  1.75\n");
    printf("    char     ONE character          'A'\n\n");

    printf("  On this machine they take up:\n\n");
    printf("    sizeof(int)    = %zu bytes\n", sizeof(int));
    printf("    sizeof(float)  = %zu bytes\n", sizeof(float));
    printf("    sizeof(double) = %zu bytes\n", sizeof(double));
    printf("    sizeof(char)   = %zu byte\n\n", sizeof(char));

    printf("  Those numbers were just calculated by your own compiler.\n");
    printf("  C does not guarantee they are the same elsewhere.\n");

    wait_enter();
    clear_screen();
    heading("PART 3: printing each type");

    printf("  printf does not know the type of what you hand it. You have\n");
    printf("  to say, with a marker:\n\n");

    printf("    %%d   int\n");
    printf("    %%f   float or double\n");
    printf("    %%c   char\n");
    printf("    %%s   string\n\n");

    int age = 25;
    float height = 1.75f;
    char initial = 'J';

    printf("  Running right now:\n\n");
    printf("    printf(\"%%d\", age)      ->  %d\n", age);
    printf("    printf(\"%%f\", height)   ->  %f\n", height);
    printf("    printf(\"%%.2f\", height) ->  %.2f\n", height);
    printf("    printf(\"%%c\", initial)  ->  %c\n\n", initial);

    printf("  %%.2f rounds for display only. The variable is unchanged.\n");

    wait_enter();
    clear_screen();
    heading("PART 4: the division that catches everyone");

    int a = 5, b = 2;

    printf("    int a = 5, b = 2;\n\n");
    printf("    a / b            ->  %d\n", a / b);
    printf("    (float) a / b    ->  %.1f\n\n", (float) a / b);

    printf("  5 / 2 gives 2, not 2.5. Both sides are int, so C does\n");
    printf("  integer division and throws the fraction away.\n\n");
    printf("  It is not rounding: 9 / 10 gives 0.\n\n");

    printf("  To get decimals, one side must be decimal BEFORE the\n");
    printf("  division. Afterwards is too late:\n\n");
    printf("    (float)(a / b)   ->  %.1f   <- already lost\n", (float)(a / b));
    printf("    (float) a / b    ->  %.1f   <- right\n\n", (float) a / b);

    printf("  This is where half of all wrong averages in C come from,\n");
    printf("  and the compiler says nothing: integer division is legal.\n");

    wait_enter();
    clear_screen();
    exercise(2);

    question("int x = 7 / 2;   What is x?",
             "3",
             "Integer division: 3.5 loses its fraction. It does not round.");

    question("float m = 7 / 2;   What is m?",
             "3",
             "7/2 is worked out BEFORE it is stored. Storing into a float\n"
             "             does not undo integer division: you get 3.000000.");

    question("Which printf marker prints an int? (e.g. %x)",
             "%d",
             "%f is for decimals, %c for one character, %s for text.");

    {
        const char *task[] = {
            "Declare two ints, 7 and 2, and print their division",
            "with one decimal place (it should show 3.5).",
        };
        const char *expected[] = {
            "3.5",
        };
        const char *solution[] = {
            "#include <stdio.h>",
            "",
            "int main(void)",
            "{",
            "    int a = 7, b = 2;",
            "    printf(\"%.1f\\n\", (float) a / b);",
            "    return 0;",
            "}",
            "",
            "The (float) must come BEFORE the division, on one side.",
        };
        challenge(task, 2, expected, 1, solution, 10);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    printf("   - the type comes before the name and never changes\n");
    printf("   - each type has its own printf marker\n");
    printf("   - int / int gives int, even stored into a float\n");
    printf("   - convert before dividing, not after\n");
    wait_enter();
}

void lesson_03_input(void)
{
    title("MODULE 3 - READING INPUT");

    heading("PART 1: scanf and the &");

    printf("    int age;\n");
    printf("    scanf(\"%%d\", &age);\n\n");

    printf("  The & before the name is not decoration. Without it the\n");
    printf("  program compiles and then crashes.\n\n");

    printf("  Reason: in C, passing a variable to a function passes a\n");
    printf("  COPY. scanf has to change your variable, and it cannot\n");
    printf("  change a copy. The & gives it the ADDRESS of the variable,\n");
    printf("  so it can go and write there.\n\n");

    printf("  Without &, scanf gets whatever value happens to be in the\n");
    printf("  variable (garbage) and uses it as an address. Writing to a\n");
    printf("  made-up address is the Segmentation fault.\n\n");

    printf("  Module 8 covers this properly. For now: with scanf, always\n");
    printf("  write &.\n");

    wait_enter();
    clear_screen();
    heading("PART 2: the exception");

    printf("    char name[50];\n");
    printf("    scanf(\"%%49s\", name);      <- no &\n\n");

    printf("  An array is already an address. The name `name` on its own\n");
    printf("  already means 'where the array starts', so & would be the\n");
    printf("  address of an address.\n\n");

    printf("  Note the 49: it caps how many characters scanf may write.\n");
    printf("  The array has 50 slots and the last one is reserved for the\n");
    printf("  end-of-text marker.\n\n");

    printf("  Without that cap, a 200-character name writes past the end\n");
    printf("  of the array, over whatever is there. It is the most\n");
    printf("  exploited class of bug in the history of C.\n");

    wait_enter();
    clear_screen();
    heading("PART 3: try it");

    printf("  This course is written in C. What follows is really the\n");
    printf("  program reading your keyboard.\n\n");

    char name[50];
    printf("  What is your name? ");
    fflush(stdout);

    if (read_line(name, sizeof name) && name[0] != '\0') {
        printf("\n  Hello, %s. That is %zu characters.\n", name, strlen(name));
    } else {
        printf("\n  (you typed nothing)\n");
    }

    printf("\n  This course does not use scanf for that. It uses fgets,\n");
    printf("  which is told the size of the destination and so can never\n");
    printf("  overrun it:\n\n");
    printf("    fgets(name, sizeof name, stdin);\n\n");
    printf("  scanf with %%s has another problem: it stops at the first\n");
    printf("  space, so 'Ana Maria' reads only 'Ana'.\n");

    wait_enter();
    clear_screen();
    exercise(3);

    question("int n; scanf(\"%d\", n);  -- is anything missing?\n"
             "  (answer: yes or no)",
             "yes",
             "The & is missing. Without it scanf writes to a made-up\n"
             "             address: Segmentation fault.");

    question("char name[50]; scanf(\"%49s\", name);  -- does it need &?\n"
             "  (answer: yes or no)",
             "no",
             "An array's name is already the address where it starts.");

    question("With scanf(\"%s\") and the user typing 'Ana Maria',\n"
             "  what ends up stored?",
             "ana",
             "%s stops at the first space. That is why this course uses fgets.");

    {
        const char *task[] = {
            "Ask the user for their name and greet them.",
        };
        const char *expected[] = {
            "What is your name? Jose",
            "Hello, Jose",
        };
        const char *solution[] = {
            "#include <stdio.h>",
            "",
            "int main(void)",
            "{",
            "    char name[50];",
            "",
            "    printf(\"What is your name? \");",
            "    fgets(name, sizeof name, stdin);",
            "",
            "    printf(\"Hello, %s\", name);",
            "    return 0;",
            "}",
            "",
            "No \\n on the last printf: fgets keeps the newline you typed,",
            "so it is already at the end of name.",
        };
        challenge(task, 1, expected, 2, solution, 15);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    printf("   - scanf takes & for numbers, no & for arrays\n");
    printf("   - no & -> Segmentation fault\n");
    printf("   - always cap the size: %%49s into an array of 50\n");
    printf("   - fgets is safer and reads the whole line\n");
    wait_enter();
}

void lesson_04_conditions(void)
{
    title("MODULE 4 - CONDITIONS");

    heading("PART 1: if, else if, else");

    printf("    int score = 85;\n\n");
    printf("    if (score >= 90) {\n");
    printf("        printf(\"A\\n\");\n");
    printf("    } else if (score >= 80) {\n");
    printf("        printf(\"B\\n\");\n");
    printf("    } else {\n");
    printf("        printf(\"F\\n\");\n");
    printf("    }\n\n");

    int score = 85;
    printf("  Running now with score = %d:  ", score);
    if (score >= 90) {
        printf("A\n\n");
    } else if (score >= 80) {
        printf("B\n\n");
    } else {
        printf("F\n\n");
    }

    printf("  Only the first true branch runs. That is why order matters:\n");
    printf("  if you tested >= 80 first, a 95 would stop there.\n");

    wait_enter();
    clear_screen();
    heading("PART 2: = is not ==");

    printf("    if (x = 5)     assigns 5 to x, then asks whether 5 is\n");
    printf("                   true. It always is. Always.\n");
    printf("    if (x == 5)    compares.\n\n");

    printf("  This compiles. With -Wall gcc warns about it, which is why\n");
    printf("  this course keeps warnings on.\n\n");

    printf("  C has no boolean type in the older standards: 0 is false and\n");
    printf("  everything else is true. So `if (x = 5)` is a perfectly\n");
    printf("  valid test as far as the compiler is concerned.\n");

    wait_enter();
    clear_screen();
    heading("PART 3: the missing braces");

    printf("    if (raining)\n");
    printf("        printf(\"take an umbrella\\n\");\n");
    printf("        printf(\"take a coat\\n\");      <- runs ALWAYS\n\n");

    printf("  Only the first line belongs to the if. The indentation fools\n");
    printf("  your eyes; the compiler does not read it.\n\n");
    printf("  Always use braces, even for one line. It costs two\n");
    printf("  characters and prevents the bug when somebody adds a second.\n");

    wait_enter();
    clear_screen();
    exercise(4);

    question("int x = 3;  if (x = 5) { ... }   Does the block run?\n"
             "  (answer: yes, no or depends)",
             "yes",
             "= assigns 5 and evaluates to 5. 5 is not zero, so it is true.\n"
             "             It runs every time, and x became 5 on the way.");

    question("In C, which is the only value that counts as false?",
             "0",
             "Everything else is true, including negative numbers.");

    question("score = 95. You test (score >= 80) first, then\n"
             "  (score >= 90). Which branch runs?",
             "80",
             "The first one that is true. Always test highest to lowest.");

    {
        const char *task[] = {
            "Read a number and say whether it is positive,",
            "negative or zero.",
        };
        const char *expected[] = {
            "Number: -5",
            "negative",
        };
        const char *solution[] = {
            "#include <stdio.h>",
            "",
            "int main(void)",
            "{",
            "    int n;",
            "    printf(\"Number: \");",
            "",
            "    if (scanf(\"%d\", &n) != 1) {",
            "        return 1;",
            "    }",
            "",
            "    if (n > 0) {",
            "        printf(\"positive\\n\");",
            "    } else if (n < 0) {",
            "        printf(\"negative\\n\");",
            "    } else {",
            "        printf(\"zero\\n\");",
            "    }",
            "    return 0;",
            "}",
        };
        challenge(task, 2, expected, 2, solution, 21);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    printf("   - only the first true branch runs; order matters\n");
    printf("   - == compares, = assigns\n");
    printf("   - 0 is false, everything else is true\n");
    printf("   - braces always\n");
    wait_enter();
}

void lesson_05_loops(void)
{
    title("MODULE 5 - LOOPS");

    heading("PART 1: for");

    printf("    for (int i = 1; i <= 5; i++) {\n");
    printf("        printf(\"%%d \", i);\n");
    printf("    }\n\n");

    printf("  Three parts separated by ;\n");
    printf("    start       i = 1        runs once\n");
    printf("    condition   i <= 5       tested before each pass\n");
    printf("    step        i++          runs at the end of each pass\n\n");

    printf("  Running:  ");
    for (int i = 1; i <= 5; i++) {
        printf("%d ", i);
    }
    printf("\n");

    wait_enter();
    clear_screen();
    heading("PART 2: the off-by-one");

    printf("  An array of 5 has indices 0,1,2,3,4. There is no 5.\n\n");

    printf("    for (int i = 0; i <= 5; i++)   <- touches 6 slots\n");
    printf("    for (int i = 0; i <  5; i++)   <- right\n\n");

    printf("  With <= you write one slot past the end. C does not check\n");
    printf("  bounds: no error, no warning, the program carries on with\n");
    printf("  somebody else's memory corrupted.\n\n");

    printf("  Rule of thumb: counting from 0, the condition uses < and\n");
    printf("  never <=.\n");

    wait_enter();
    clear_screen();
    heading("PART 3: while");

    printf("    int left = 3;\n");
    printf("    while (left > 0) {\n");
    printf("        printf(\"%%d \", left);\n");
    printf("        left--;\n");
    printf("    }\n\n");

    printf("  Running:  ");
    int left = 3;
    while (left > 0) {
        printf("%d ", left);
        left--;
    }
    printf("\n\n");

    printf("  Forget the left--, and the condition never becomes false and\n");
    printf("  the program hangs. Ctrl-C stops it.\n\n");
    printf("  Use for when you know how many passes, while when you do not.\n");

    wait_enter();
    clear_screen();
    exercise(5);

    question("int v[5];  What is the last valid index?",
             "4",
             "Five slots numbered 0 to 4. v[5] is already somebody else's\n"
             "             memory.");

    question("for (int i = 0; i < 5; i++)  -- how many passes?",
             "5",
             "0 through 4. Starting at 0 with <, the number is the count.");

    question("What happens if you write to v[5] in an array of 5?\n"
             "  (answer: compile error, warning, or nothing)",
             "nothing",
             "C does not check bounds. It writes over whatever is there and\n"
             "             the program carries on, corrupted.");

    {
        const char *task[] = {
            "Print the 7 times table, from 7x1 to 7x10,",
            "one per line: '7 x 1 = 7'.",
        };
        const char *expected[] = {
            "7 x 1 = 7",
            "7 x 2 = 14",
            "...",
            "7 x 10 = 70",
        };
        const char *solution[] = {
            "#include <stdio.h>",
            "",
            "int main(void)",
            "{",
            "    for (int i = 1; i <= 10; i++) {",
            "        printf(\"7 x %d = %d\\n\", i, 7 * i);",
            "    }",
            "    return 0;",
            "}",
            "",
            "Here <= is correct: a times table starts at 1, not 0.",
            "The < rule is for array indices.",
        };
        challenge(task, 2, expected, 4, solution, 12);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    printf("   - for: start; condition; step\n");
    printf("   - indices run 0 to n-1: use <, not <=\n");
    printf("   - C does not check array bounds\n");
    printf("   - something inside a while must change the condition\n\n");

    printf("  Module 6 starts the part that sets C apart: arrays,\n");
    printf("  memory and pointers.\n");
    wait_enter();
}
