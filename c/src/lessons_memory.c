/*
 * Modules 6 to 10 - arrays, pointers, memory and structs.
 *
 * This is the part that sets C apart from the languages you already know.
 * None of them make you know where your data lives; C does, and in exchange
 * it lets you look and touch.
 *
 * The examples that are memory bugs (double free, use-after-free) are
 * explained but never run. Undefined behaviour is not something to
 * demonstrate live: it either takes the course down, or - worse - appears
 * to work.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lessons.h"
#include "ui.h"

void lesson_06_arrays_strings(void)
{
    title("MODULE 6 - ARRAYS AND STRINGS");

    heading("PART 1: an array is memory in a row");

    printf("    int numbers[5] = {10, 20, 30, 40, 50};\n\n");

    int numbers[5] = {10, 20, 30, 40, 50};

    printf("  Five integers packed against each other. Each takes %zu\n", sizeof(int));
    printf("  bytes, so the whole array takes %zu.\n\n", sizeof numbers);

    printf("  The real addresses, right now:\n\n");
    for (int i = 0; i < 5; i++) {
        printf("    numbers[%d] = %2d   at  %p\n", i, numbers[i], (void *) &numbers[i]);
    }
    printf("\n  Notice the addresses go up by %zu each time. That is not a\n",
           sizeof(int));
    printf("  coincidence: it is what 'next to each other' means.\n");

    wait_enter();
    clear_screen();
    heading("PART 2: a string is an array ending in \\0");

    char text[] = "hi";

    printf("    char text[] = \"hi\";\n\n");
    printf("  Looks like 2 letters. It is 3 bytes:\n\n");
    printf("    index    0     1     2\n");
    printf("    value   'h'   'i'   '\\0'\n\n");

    printf("    strlen(text)  = %zu   <- letters up to the \\0\n", strlen(text));
    printf("    sizeof text   = %zu   <- bytes used, \\0 included\n\n", sizeof text);

    printf("  The '\\0' is what marks where the text ends. No string\n");
    printf("  function is told the length: they walk until they hit \\0.\n\n");
    printf("  If it goes missing, they keep walking through memory. That is\n");
    printf("  why an array of 10 only holds 9 letters.\n");

    wait_enter();
    clear_screen();
    heading("PART 3: == does not compare text");

    const char *a = "hi";
    char b[] = "hi";

    printf("    const char *a = \"hi\";\n");
    printf("    char b[] = \"hi\";\n\n");
    printf("    a == b        ->  %s\n", (a == (const char *) b) ? "true" : "false");
    printf("    strcmp(a, b)  ->  %d\n\n", strcmp(a, b));

    printf("  The letters match but the addresses do not. == compares\n");
    printf("  addresses, so it is false.\n\n");

    printf("  To compare text use strcmp, which returns 0 when they are\n");
    printf("  equal. Zero. That is easy to get backwards:\n\n");
    printf("    if (strcmp(a, b) == 0)    <- equal\n");
    printf("    if (strcmp(a, b))         <- DIFFERENT\n\n");
    printf("  Needs #include <string.h>.\n");

    wait_enter();
    clear_screen();
    exercise(6);

    question("char t[] = \"hi\";   What does strlen(t) give?",
             "2",
             "It counts letters up to the \\0, without counting it.");

    question("And what does sizeof t give?",
             "3",
             "It counts the bytes used, and the \\0 takes one too.");

    question("if (strcmp(a, b)) { ... }  -- does the block run when the\n"
             "  strings are equal or different?",
             "different",
             "strcmp gives 0 for equal, and 0 is false. To test equality it\n"
             "             must be strcmp(a, b) == 0.");

    {
        const char *task[] = {
            "Read a word and print it backwards.",
            "Use strlen to know where to start.",
        };
        const char *expected[] = {
            "Word: hello",
            "olleh",
        };
        const char *solution[] = {
            "#include <stdio.h>",
            "#include <string.h>",
            "",
            "int main(void)",
            "{",
            "    char word[100];",
            "",
            "    printf(\"Word: \");",
            "    scanf(\"%99s\", word);",
            "",
            "    for (int i = (int) strlen(word) - 1; i >= 0; i--) {",
            "        printf(\"%c\", word[i]);",
            "    }",
            "    printf(\"\\n\");",
            "    return 0;",
            "}",
            "",
            "The -1 at the start is the same rule as always: the last index",
            "is the length minus one.",
        };
        const char *input[] = {"hello"};
        challenge(task, 2, input, 1, expected, 2, solution, 19);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    printf("   - array = values sitting next to each other in memory\n");
    printf("   - string = array of char ending in \\0\n");
    printf("   - strlen counts letters, sizeof counts bytes\n");
    printf("   - == compares addresses; strcmp(a,b)==0 compares text\n");
    wait_enter();
}

/* Used by module 7 to show the copy does not affect the original. */
static void try_to_change(int copy)
{
    copy = 99;
    (void) copy; /* only so the compiler does not warn it is unused */
}

static void really_change(int *address)
{
    *address = 99;
}

/* Used by module 7 to show local vs. global and the shadowing trap. */
static int counter = 0;

static void bump(void)
{
    counter++;
}

static void confusing(void)
{
    int counter = 0;
    counter++;
    (void) counter; /* only so the compiler does not warn it is unused */
}

/* Used by module 7 to show recursion with an actually-reachable base case. */
static int factorial(int n)
{
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

void lesson_07_functions(void)
{
    title("MODULE 7 - FUNCTIONS");

    heading("PART 1: the shape");

    printf("    int add(int a, int b)\n");
    printf("    {\n");
    printf("        return a + b;\n");
    printf("    }\n\n");
    printf("     ^    ^        ^\n");
    printf("     |    |        what it takes, with types\n");
    printf("     |    name\n");
    printf("     type of what it gives back\n\n");

    printf("  A function returning nothing is declared void.\n\n");

    printf("  A function taking nothing is declared (void), not (). They\n");
    printf("  are not the same thing, and this surprises everybody:\n\n");

    printf("    int total()      'I am not saying what it takes'\n");
    printf("    int total(void)  'it takes nothing'\n\n");

    printf("  With the empty (), the compiler is not allowed to check the\n");
    printf("  call. This compiles and runs, printing 42:\n\n");
    printf("    int total() { return 42; }\n");
    printf("    total(1, 2, 3);        <- three arguments. Nobody complains.\n\n");

    printf("  With (void) the same call is an error, which is what you\n");
    printf("  wanted. Verified on gcc with -std=c11.\n\n");

    printf("  C23 changed this: there, () finally means the same as\n");
    printf("  (void). Until every compiler you use is C23, write (void)\n");
    printf("  and the question never comes up.\n\n");

    printf("  For main itself both are allowed - it is the only function\n");
    printf("  where it makes no difference.\n");

    wait_enter();
    clear_screen();
    heading("PART 2: C passes copies");

    int x = 5;

    printf("  This is the rule that explains scanf's &.\n\n");
    printf("    void try_to_change(int copy) { copy = 99; }\n\n");
    printf("    int x = 5;\n");
    printf("    try_to_change(x);\n\n");

    try_to_change(x);
    printf("    x is still %d\n\n", x);

    printf("  The function got a COPY of the 5. It changed the copy. Your\n");
    printf("  x never heard about it.\n\n");

    printf("  For a function to change your variable, you have to give it\n");
    printf("  the address:\n\n");
    printf("    void really_change(int *address) { *address = 99; }\n");
    printf("    really_change(&x);\n\n");

    really_change(&x);
    printf("    x is now %d\n\n", x);

    printf("  That is exactly what scanf does. Nothing magic.\n");

    wait_enter();
    clear_screen();
    heading("PART 3: declare before use");

    printf("  The compiler reads the file top to bottom. Call a function\n");
    printf("  before it has seen it and you get:\n\n");
    printf("    error: implicit declaration of function 'add'\n\n");

    printf("  Since GCC 14 this is an error, not a warning. Before that\n");
    printf("  the compiler guessed the signature and quietly generated\n");
    printf("  wrong code.\n\n");

    printf("  Two fixes: put the function above whoever uses it, or\n");
    printf("  declare the signature at the top:\n\n");
    printf("    int add(int a, int b);     <- a prototype, with ;\n\n");
    printf("  That is all a .h file is: a list of prototypes.\n");

    wait_enter();
    clear_screen();
    heading("PART 4: local vs global -- and the name that shadows");

    printf("  A variable declared inside a function is LOCAL: it exists\n");
    printf("  only for that call, and no other function can see it. A\n");
    printf("  variable declared outside every function is GLOBAL: it\n");
    printf("  exists for the whole program and every function can see it.\n\n");

    printf("    int counter = 0;              /* global -- outside any function */\n\n");
    printf("    void bump(void)\n");
    printf("    {\n");
    printf("        counter++;                /* the global, no local of that name */\n");
    printf("    }\n\n");

    bump();
    bump();
    printf("  Running: bump() called twice, counter is now %d.\n\n", counter);

    printf("  The trap: a local variable with the SAME name as a global\n");
    printf("  does not change the global at all -- it shadows it. Inside\n");
    printf("  that function, the name refers to the local; the global\n");
    printf("  sits there untouched:\n\n");

    printf("    void confusing(void)\n");
    printf("    {\n");
    printf("        int counter = 0;          /* a NEW, local counter */\n");
    printf("        counter++;                /* changes the local, not the global */\n");
    printf("    }\n\n");

    int before = counter;
    confusing();
    printf("  Running: counter was %d before confusing(), %d after -- the\n",
           before, counter);
    printf("  global never moved. confusing()'s own local counter reached 1\n");
    printf("  and was thrown away the moment the function returned.\n\n");

    printf("  Scope is WHERE a name is visible; lifetime is HOW LONG its\n");
    printf("  storage exists. A local variable's scope ends at its\n");
    printf("  function's closing brace, and so does its lifetime -- the\n");
    printf("  moment the function returns, that storage is gone. A block\n");
    printf("  narrower than a whole function has the same rule:\n\n");

    printf("    {\n");
    printf("        int temp = 5;              /* scope starts here */\n");
    printf("    }                               /* and ends here -- gone */\n");
    printf("    printf(\"%%d\", temp);            /* error: temp is not in scope */\n\n");

    printf("  A global's scope is the whole file (or every file that\n");
    printf("  declares it extern); its lifetime is the entire run of the\n");
    printf("  program, from before main starts to after it returns.\n");

    wait_enter();
    clear_screen();
    heading("PART 5: recursion -- a function calling itself");

    printf("    int factorial(int n)\n");
    printf("    {\n");
    printf("        if (n <= 1) {\n");
    printf("            return 1;              /* the base case -- stops it */\n");
    printf("        }\n");
    printf("        return n * factorial(n - 1);  /* the recursive case */\n");
    printf("    }\n\n");

    printf("  Running: factorial(5) is %d.\n\n", factorial(5));

    printf("  Every call to factorial(n) makes its own call to\n");
    printf("  factorial(n - 1), and waits for it to return before\n");
    printf("  multiplying by n. factorial(5) waits on factorial(4), which\n");
    printf("  waits on factorial(3), and so on down to factorial(1), which\n");
    printf("  is the base case: it returns immediately, and the chain of\n");
    printf("  waiting calls multiplies its way back up.\n\n");

    printf("  The trap: every recursive function needs a base case that is\n");
    printf("  actually reachable. Without one -- or with one that never\n");
    printf("  triggers, say `if (n <= 1)` misspelled as `if (n == 1)` on an\n");
    printf("  n that starts negative -- the function calls itself forever.\n");
    printf("  Each call takes a little stack space, and stack space is\n");
    printf("  finite: real behavior, not run here on purpose, is\n\n");

    printf("    Segmentation fault (core dumped)\n\n");

    printf("  a stack overflow, the same crash module 8 explains for other\n");
    printf("  reasons -- the call stack ran out of room and hit memory\n");
    printf("  that was never allocated for it.\n");

    wait_enter();
    clear_screen();
    exercise(7);

    question("void f(int n) { n = 99; }   You call f(x) with x = 5.\n"
             "  What is x afterwards?",
             "5",
             "The function worked on a copy. Your x never heard about it.");

    question("So how does a function really change your x?\n"
             "  (answer: by passing what?)",
             "&x",
             "You pass the address. The function takes int *p and writes\n"
             "             through it with *p = 99.");

    question("A function taking nothing is declared f() or f(void)?",
             "f(void)",
             "In C, f() means 'I am not saying what it takes', not 'nothing'.");

    question("int total = 5; is global. A function declares its own\n"
             "  int total = 0; and changes it. Does the global total\n"
             "  change too?  (answer: yes or no)",
             "no",
             "The local shadows the global by name -- inside that function,\n"
             "             total means the local one. The global sits untouched.");

    question("A recursive function has no base case that can ever be\n"
             "  reached. What actually happens when you run it?",
             "stack overflow",
             "Each call uses a little stack space; with no base case the calls\n"
             "             never stop, and the stack runs out -- a real crash, not a hang.");

    {
        const char *task[] = {
            "Write a function swap(int *a, int *b) that swaps two",
            "variables. Test it from main.",
        };
        const char *expected[] = {
            "2 1",
        };
        const char *solution[] = {
            "#include <stdio.h>",
            "",
            "void swap(int *a, int *b)",
            "{",
            "    int held = *a;",
            "    *a = *b;",
            "    *b = held;",
            "}",
            "",
            "int main(void)",
            "{",
            "    int x = 1, y = 2;",
            "    swap(&x, &y);",
            "    printf(\"%d %d\\n\", x, y);   /* 2 1 */",
            "    return 0;",
            "}",
            "",
            "The held variable is necessary: without it, *a = *b would wipe",
            "a's value before you could copy it into b.",
        };
        challenge(task, 2, 0, 0, expected, 1, solution, 19);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    printf("   - type name(parameters) { ... return ...; }\n");
    printf("   - arguments are copies\n");
    printf("   - pass &variable so the function can change yours\n");
    printf("   - declare before use, or write a prototype\n");
    printf("   - a local variable shadows a global of the same name -- it does not change it\n");
    printf("   - recursion needs a reachable base case, or the stack runs out\n");
    wait_enter();
}

void lesson_08_pointers(void)
{
    title("MODULE 8 - POINTERS");

    heading("PART 1: a pointer holds an address");

    int age = 25;
    int *p = &age;

    printf("    int age = 25;\n");
    printf("    int *p = &age;\n\n");

    printf("  Two ordinary variables. The difference is what they hold:\n\n");
    printf("    age   holds  %d\n", age);
    printf("    p     holds  %p     <- the address of age\n\n", (void *) p);

    printf("  Two symbols, and they are opposites:\n\n");
    printf("    &age      the address of age      ->  %p\n", (void *) &age);
    printf("    *p        the value living there  ->  %d\n\n", *p);

    printf("  Read them as: & = 'address of', * = 'what is at'.\n");

    wait_enter();
    clear_screen();
    heading("PART 2: * means two different things");

    printf("  This confuses everyone, and it is only notation:\n\n");
    printf("    int *p = &age;    here * is part of the TYPE\n");
    printf("                      'p is a pointer to int'\n\n");
    printf("    *p = 30;          here * is an OPERATION\n");
    printf("                      'write 30 where p points'\n\n");

    *p = 30;
    printf("  After *p = 30, the variable age is %d.\n\n", age);

    printf("  I never touched it directly. I changed it through the\n");
    printf("  pointer, and it is the same memory.\n");

    wait_enter();
    clear_screen();
    heading("PART 3: NULL and the Segmentation fault");

    int *empty = NULL;

    printf("    int *empty = NULL;\n\n");
    printf("  NULL means 'points at nothing'. It is %p.\n\n", (void *) empty);

    printf("  Doing *empty kills the program:\n\n");
    printf("    Segmentation fault (core dumped)\n\n");

    printf("  That is the system saying 'you asked for memory that is not\n");
    printf("  yours'. So a pointer that might hold nothing gets tested:\n\n");
    printf("    if (p != NULL) {\n");
    printf("        printf(\"%%d\\n\", *p);\n");
    printf("    }\n\n");

    printf("  An uninitialised pointer is worse than NULL: it holds\n");
    printf("  garbage, which might even be a valid address. Always\n");
    printf("  initialise, even if only to NULL.\n");

    wait_enter();
    clear_screen();
    heading("PART 4: const with pointers -- two different promises");

    printf("  const can attach to either side of the *, and each side\n");
    printf("  makes a different promise:\n\n");

    printf("    const int *view;      view can point elsewhere, but *view\n");
    printf("                          cannot be assigned through it\n\n");
    printf("    int *const fixed;     *fixed can be assigned, but fixed\n");
    printf("                          itself cannot be pointed elsewhere\n\n");
    printf("    const int *const both;   neither -- both are locked\n\n");

    printf("  Read right to left from the name: 'const int *view' is\n");
    printf("  'view is a pointer to a const int'. 'int *const fixed' is\n");
    printf("  'fixed is a const pointer to an int'.\n\n");

    int locked = 10;
    const int *view = &locked;
    printf("  Running: const int *view = &locked; *view is %d.\n\n", *view);

    locked = 20;
    printf("  locked changed to %d through its own name -- view still\n", locked);
    printf("  sees the new value, because the const promise is only about\n");
    printf("  writing THROUGH the pointer, not about the value being\n");
    printf("  unable to change at all:\n\n");
    printf("    Running: *view is now %d.\n\n", *view);

    printf("  What the const actually blocks is this line, which does not\n");
    printf("  compile -- not run here, since a real build cannot contain a\n");
    printf("  line that fails to compile:\n\n");
    printf("    *view = 99;\n\n");
    printf("    error: assignment of read-only location '*view'\n\n");

    printf("  int *const, the other order, blocks the opposite thing --\n");
    printf("  reassigning the pointer itself, not writing through it:\n\n");
    printf("    int *const fixed = &locked;\n");
    printf("    *fixed = 99;         <- fine, changes locked\n");
    printf("    fixed = &view_target;  <- error: assignment of read-only variable\n\n");

    printf("  A function parameter is where this earns its keep: `void\n");
    printf("  print_it(const int *p)` promises callers you will not modify\n");
    printf("  what they passed you, and the compiler enforces the promise\n");
    printf("  instead of trusting a comment.\n");

    wait_enter();
    clear_screen();
    exercise(8);

    question("int n = 7; int *p = &n;   What does *p give?",
             "7",
             "* reads the value at the address p holds.");

    question("And what does p itself hold?\n"
             "  (answer: the value or the address)",
             "the address",
             "A pointer holds an address. * is what reaches the value.");

    question("What happens when you do *p with p = NULL?",
             "segmentation fault",
             "The system stops the program: that address is not yours.");

    question("const int *p = &n;   Can p itself be made to point at a\n"
             "  different int later?  (answer: yes or no)",
             "yes",
             "const int *p locks *p (writing through p), not p itself --\n"
             "             int *const p is the other way around.");

    {
        const char *task[] = {
            "Write a function that takes a pointer to int and",
            "doubles the value it points at. Show it worked.",
        };
        const char *expected[] = {
            "42",
        };
        const char *solution[] = {
            "#include <stdio.h>",
            "",
            "void double_it(int *n)",
            "{",
            "    if (n == NULL) {",
            "        return;",
            "    }",
            "    *n = *n * 2;",
            "}",
            "",
            "int main(void)",
            "{",
            "    int x = 21;",
            "    double_it(&x);",
            "    printf(\"%d\\n\", x);   /* 42 */",
            "    return 0;",
            "}",
            "",
            "The NULL check costs three lines and turns a crash into",
            "doing nothing.",
        };
        challenge(task, 2, 0, 0, expected, 1, solution, 20);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    printf("   - a pointer is a variable holding an address\n");
    printf("   - & takes the address, * reads/writes what is there\n");
    printf("   - in a type, * means 'pointer'; in code, * reaches through\n");
    printf("   - test for NULL before using\n");
    printf("   - const int *p locks writing through p; int *const p locks p itself\n\n");

    printf("  Module 9 uses this to ask for memory properly.\n");
    wait_enter();
}

void lesson_09_memory(void)
{
    title("MODULE 9 - MEMORY: MALLOC AND FREE");

    heading("PART 1: two places data lives");

    printf("    int numbers[5];                      <- stack\n");
    printf("    int *p = malloc(5 * sizeof(int));    <- heap\n\n");

    printf("    STACK                     HEAP\n");
    rule();
    printf("    automatic                 you ask for it\n");
    printf("    dies at end of function   lives until you free it\n");
    printf("    fixed size, in the code   size decided while running\n");
    printf("    small (~8 MB)             limited by RAM\n\n");

    printf("  As long as you know the size when you write the code, use\n");
    printf("  the stack: it is faster and it cleans itself up.\n\n");
    printf("  The heap is for when the size is only known at run time, or\n");
    printf("  when the data must outlive the function that made it.\n");

    wait_enter();
    clear_screen();
    heading("PART 2: ask, use, give back");

    printf("    int *v = malloc(5 * sizeof(int));\n");
    printf("    if (v == NULL) { return 1; }      <- it can fail\n");
    printf("\n");
    printf("    v[0] = 10;\n");
    printf("\n");
    printf("    free(v);      <- give it back\n");
    printf("    v = NULL;     <- so it stops pointing at dead memory\n\n");

    int *v = malloc(5 * sizeof(int));

    if (v == NULL) {
        printf("  malloc failed. Out of memory.\n");
        wait_enter();
        return;
    }

    for (int i = 0; i < 5; i++) {
        v[i] = (i + 1) * 10;
    }

    printf("  Running now: I asked for 5 ints (%zu bytes) and got back\n",
           5 * sizeof(int));
    printf("  address %p.\n\n", (void *) v);
    printf("  Contents:  ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", v[i]);
    }
    printf("\n\n");

    printf("  Note that v[i] works on a pointer just like on an array.\n");
    printf("  v[i] is the same thing as *(v + i).\n");

    free(v);
    v = NULL;

    printf("\n  I have already called free. The memory went back.\n");

    wait_enter();
    clear_screen();
    heading("PART 3: the four ways to get it wrong");

    printf("  1. LEAK\n\n");
    printf("       int *p = malloc(100);\n");
    printf("       p = malloc(200);      <- lost the first address\n\n");
    printf("     Those 100 bytes stay taken and nobody knows where they\n");
    printf("     are any more. In a program that runs once, barely\n");
    printf("     noticeable. In a server, it eats the RAM over days.\n\n");

    printf("  2. USE AFTER FREE\n\n");
    printf("       free(p);\n");
    printf("       printf(\"%%d\", *p);    <- that memory is not yours now\n\n");
    printf("     Sometimes it still shows the old value. That is the worst\n");
    printf("     case: it looks like it works until the system reuses\n");
    printf("     those bytes.\n\n");

    wait_enter();
    clear_screen();
    heading("PART 3 (continued)");

    printf("  3. DOUBLE FREE\n\n");
    printf("       free(p);\n");
    printf("       free(p);      <- corrupts the allocator's bookkeeping\n\n");
    printf("     glibc usually catches this and kills the program:\n");
    printf("       free(): double free detected in tcache 2\n\n");
    printf("     This is why you set p = NULL after free: calling\n");
    printf("     free(NULL) is legal and does nothing.\n\n");

    printf("  4. RETURNING STACK MEMORY\n\n");
    printf("       char *name(void) {\n");
    printf("           char buffer[50];      <- dies at the return\n");
    printf("           return buffer;        <- address of garbage\n");
    printf("       }\n\n");
    printf("     The array lives in the function's stack frame. When the\n");
    printf("     function ends, that space belongs to somebody else. To\n");
    printf("     return text, use malloc - and the caller inherits the\n");
    printf("     duty to free it.\n");

    wait_enter();
    clear_screen();
    heading("PART 4: the rule and the tools");

    printf("  THE RULE\n\n");
    printf("    Every malloc has exactly one free.\n");
    printf("    Whoever mallocs decides who frees, and writes it down.\n\n");

    printf("  There is no garbage collector. If you do not free it, nobody\n");
    printf("  does - until the program ends.\n\n");

    printf("  THE TOOLS\n\n");
    printf("  1. You already have this one - it ships with gcc, nothing\n");
    printf("     to install:\n\n");
    printf("       gcc -fsanitize=address,undefined -g program.c -o program\n");
    printf("       ./program\n\n");
    printf("     If a free is missing, the program says so as it exits:\n\n");
    printf("       ERROR: LeakSanitizer: detected memory leaks\n");
    printf("       Direct leak of 100 byte(s) in 1 object(s)\n");
    printf("         #1 main program.c:7\n\n");
    printf("     It gives you the file and line of the forgotten malloc.\n");
    printf("     It also catches use-after-free and writing past an array.\n\n");

    printf("  2. valgrind does the same without recompiling:\n\n");
    printf("       valgrind ./program\n\n");
    printf("     You have to install it (sudo pacman -S valgrind on Arch).\n\n");

    printf("  This course was checked with the first one: it has no leaks.\n");

    wait_enter();
    clear_screen();
    exercise(9);

    question("A function does malloc 3 times. How many frees are needed?",
             "3",
             "One free for every malloc. No more, no fewer.");

    question("int *p = malloc(100); p = malloc(200);\n"
             "  What is the name of this bug?",
             "leak",
             "The first 100 bytes are still taken and the address is gone.");

    question("Why write p = NULL right after free(p)?",
             "double free",
             "free(NULL) is legal and does nothing, so a second free becomes\n"
             "             harmless instead of corrupting the allocator.");

    question("Can you return the address of a local array from a\n"
             "  function? (answer: yes or no)",
             "no",
             "It dies at the return. Use malloc, and say who must free it.");

    {
        const char *task[] = {
            "Ask the user how many numbers they want, allocate",
            "exactly that many with malloc, fill them with 1..n,",
            "print them, and free the memory.",
            "",
            "Then compile it with -fsanitize=address and check it",
            "reports no leaks.",
        };
        const char *expected[] = {
            "How many? 5",
            "1 2 3 4 5",
        };
        const char *solution[] = {
            "#include <stdio.h>",
            "#include <stdlib.h>",
            "",
            "int main(void)",
            "{",
            "    int n;",
            "    printf(\"How many? \");",
            "",
            "    if (scanf(\"%d\", &n) != 1 || n <= 0) {",
            "        return 1;",
            "    }",
            "",
            "    int *v = malloc((size_t) n * sizeof(int));",
            "    if (v == NULL) {",
            "        return 1;",
            "    }",
            "",
            "    for (int i = 0; i < n; i++) {",
            "        v[i] = i + 1;",
            "        printf(\"%d \", v[i]);",
            "    }",
            "    printf(\"\\n\");",
            "",
            "    free(v);",
            "    return 0;",
            "}",
            "",
            "Three things worth copying: checking scanf worked, checking",
            "malloc did not return NULL, and n <= 0 being rejected before",
            "it reaches malloc.",
        };
        const char *input[] = {"5"};
        challenge(task, 6, input, 1, expected, 2, solution, 30);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    printf("   - stack: automatic, dies with the function\n");
    printf("   - heap: malloc, lives until free\n");
    printf("   - always check whether malloc returned NULL\n");
    printf("   - one free per malloc, then p = NULL\n");
    printf("   - never return the address of a local array\n");
    printf("   - gcc -fsanitize=address tells you where you slipped\n");
    wait_enter();
}

void lesson_10_structs(void)
{
    title("MODULE 10 - STRUCTS");

    heading("PART 1: grouping fields into one type");

    struct Student {
        char name[50];
        int score;
    };

    printf("    struct Student {\n");
    printf("        char name[50];\n");
    printf("        int  score;\n");
    printf("    };                    <- the ; is required\n\n");

    struct Student a;
    strcpy(a.name, "Ana");
    a.score = 15;

    printf("    struct Student a;\n");
    printf("    strcpy(a.name, \"Ana\");\n");
    printf("    a.score = 15;\n\n");

    printf("  Running:  %s scored %d\n\n", a.name, a.score);

    printf("  Note the strcpy. You cannot write a.name = \"Ana\": an array\n");
    printf("  is not assigned, it is copied into.\n");

    wait_enter();
    clear_screen();
    heading("PART 2: dot or arrow");

    struct Student *ptr = &a;

    printf("    a.score       when you have the struct\n");
    printf("    ptr->score    when you have a POINTER to it\n\n");

    printf("  Running:\n");
    printf("    a.score     ->  %d\n", a.score);
    printf("    ptr->score  ->  %d\n\n", ptr->score);

    printf("  The arrow is only shorthand. These are exactly the same:\n\n");
    printf("    ptr->score      and      (*ptr).score\n\n");
    printf("  The parentheses are needed because . binds tighter than *.\n");
    printf("  Since nobody wants to write that, -> was invented.\n\n");

    printf("  Get it wrong and the compiler tells you:\n");
    printf("    error: member reference type 'struct Student *' is a\n");
    printf("           pointer; did you mean to use '->'?\n");

    wait_enter();
    clear_screen();
    heading("PART 3: field order costs space");

    struct Bad  { char a; int b; char c; };
    struct Good { char a; char c; int b; };

    printf("    struct Bad  { char a; int b; char c; };  -> %zu bytes\n",
           sizeof(struct Bad));
    printf("    struct Good { char a; char c; int b; };  -> %zu bytes\n\n",
           sizeof(struct Good));

    printf("  The same three fields. Different sizes.\n\n");
    printf("  The processor prefers to read an int at an address that is a\n");
    printf("  multiple of 4, so the compiler inserts empty space (padding)\n");
    printf("  to line things up. In the first struct the int is wedged\n");
    printf("  between two chars and space is wasted on both sides.\n\n");

    printf("  Rule: big fields first, small ones last. It only matters\n");
    printf("  when you have millions of them - but it is worth knowing why\n");
    printf("  sizeof is not the sum of the fields.\n");

    wait_enter();
    clear_screen();
    exercise(10);

    question("struct Point { int x; int y; };  -- what is missing here?",
             ";",
             "A struct declaration ends with a semicolon after the brace.");

    question("You have struct Student *p. Do you write p.score or\n"
             "  p->score?",
             "p->score",
             "The arrow is for pointers. It is short for (*p).score.");

    question("Can sizeof(struct) be larger than the sum of its fields?\n"
             "  (answer: yes or no)",
             "yes",
             "The compiler adds padding to align fields. Order changes it.");

    {
        const char *task[] = {
            "Make a struct Book with a title and a year.",
            "Fill in two books, put them in an array, and print",
            "them with a loop.",
        };
        const char *expected[] = {
            "The C Programming Language (1978)",
            "Modern C (2019)",
        };
        const char *solution[] = {
            "#include <stdio.h>",
            "#include <string.h>",
            "",
            "struct Book {",
            "    char title[100];",
            "    int  year;",
            "};",
            "",
            "int main(void)",
            "{",
            "    struct Book shelf[2];",
            "",
            "    strcpy(shelf[0].title, \"The C Programming Language\");",
            "    shelf[0].year = 1978;",
            "",
            "    strcpy(shelf[1].title, \"Modern C\");",
            "    shelf[1].year = 2019;",
            "",
            "    for (int i = 0; i < 2; i++) {",
            "        printf(\"%s (%d)\\n\", shelf[i].title, shelf[i].year);",
            "    }",
            "    return 0;",
            "}",
            "",
            "shelf[i].title uses the dot, not the arrow: shelf[i] is a",
            "struct, not a pointer to one.",
        };
        challenge(task, 3, 0, 0, expected, 2, solution, 26);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    printf("   - struct groups fields into one type; ; after the brace\n");
    printf("   - a.field with a struct, ptr->field with a pointer\n");
    printf("   - -> is (*ptr). written another way\n");
    printf("   - sizeof can exceed the sum of the fields (padding)\n\n");

    printf("  That is the base course. The next step is writing a program\n");
    printf("  of your own that uses malloc and frees it.\n");
    wait_enter();
}
