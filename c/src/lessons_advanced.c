/*
 * Modules 11 and 12 - past the core course.
 *
 * Module 11 is the final test: two full programs, using everything from
 * modules 1-10 together. There is no PART 1/2/3 walkthrough here -- the
 * point is combining what was already taught, not learning something new.
 * The one exception is a short section on rand()/srand(), needed for the
 * game to actually be a game.
 *
 * Module 11's tasks involve randomness and live user interaction, so unlike
 * every other challenge() in this course, their output cannot be an exact
 * string to match. Both tasks are presented by hand instead of through
 * challenge(), with an explicit note that the transcript shown is one
 * possible run, not the specification.
 *
 * Module 12 goes deeper into memory: pointer-to-pointer, realloc, and
 * linked lists -- the extra requested after the final test, for closing the
 * gap between "malloc and free work" and "I trust my own ownership rules".
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lessons.h"
#include "ui.h"

void lesson_11_final_test(void)
{
    title("MODULE 11 - FINAL TEST");

    heading("WHAT THIS IS");

    printf("  Two full programs. Nothing new to learn except one small\n");
    printf("  addition below -- the rest is modules 1 through 10, combined.\n\n");

    printf("  Neither task has a single correct transcript: the game\n");
    printf("  depends on a random number, and both depend on what you\n");
    printf("  type. What follows is an example run, not a string to match.\n");

    wait_enter();
    clear_screen();
    heading("A SMALL ADDITION: RANDOM NUMBERS");

    printf("    #include <stdlib.h>\n");
    printf("    #include <time.h>\n\n");
    printf("    srand((unsigned) time(NULL));   // seed once, at the start\n");
    printf("    int n = rand() %% 100 + 1;       // 1..100\n\n");

    printf("  rand() alone gives the same sequence every time the program\n");
    printf("  runs -- useful for reproducing a bug, useless for a game.\n");
    printf("  srand() reseeds it. Seeding with the current time means a\n");
    printf("  different sequence each run, because the time is different.\n\n");

    printf("  rand() %% 100 gives 0..99. The %%1 shifts it to 1..100. This\n");
    printf("  is the same modulo trick as any other range conversion.\n\n");

    srand((unsigned) time(NULL));
    printf("  A number generated right now: %d\n", rand() % 100 + 1);

    wait_enter();
    clear_screen();
    heading("TASK 1: A GAME - GUESS THE NUMBER");

    printf("  The program picks a number from 1 to 100. The player keeps\n");
    printf("  guessing. After each guess, print one of:\n\n");
    printf("    Too low.\n");
    printf("    Too high.\n");
    printf("    Correct! You got it in N attempts.\n\n");

    printf("  Stop after a correct guess. Count every guess, including the\n");
    printf("  wrong ones.\n\n");

    printf("  Example run (the secret number and your guesses will differ):\n\n");
    rule();
    printf("  Guess a number between 1 and 100.\n");
    printf("  Your guess: 50\n");
    printf("  Too high.\n");
    printf("  Your guess: 25\n");
    printf("  Too low.\n");
    printf("  Your guess: 37\n");
    printf("  Correct! You got it in 3 attempts.\n");
    rule();

    printf("\n  Try it first. Compile with:\n");
    printf("    gcc -Wall -Wextra test.c -o test && ./test\n");

    if (ask_yes("Want to see example code?")) {
        printf("\n");
        rule();
        printf("  #include <stdio.h>\n");
        printf("  #include <stdlib.h>\n");
        printf("  #include <time.h>\n");
        printf("  \n");
        printf("  int main(void)\n");
        printf("  {\n");
        printf("      srand((unsigned) time(NULL));\n");
        printf("      int secret = rand() %% 100 + 1;\n");
        printf("      int attempts = 0;\n");
        printf("      int guess;\n");
        printf("  \n");
        printf("      printf(\"Guess a number between 1 and 100.\\n\");\n");
        printf("  \n");
        printf("      for (;;) {\n");
        printf("          printf(\"Your guess: \");\n");
        printf("          if (scanf(\"%%d\", &guess) != 1) {\n");
        printf("              printf(\"That is not a number.\\n\");\n");
        printf("              return 1;\n");
        printf("          }\n");
        printf("          attempts++;\n");
        printf("  \n");
        printf("          if (guess < secret) {\n");
        printf("              printf(\"Too low.\\n\");\n");
        printf("          } else if (guess > secret) {\n");
        printf("              printf(\"Too high.\\n\");\n");
        printf("          } else {\n");
        printf("              printf(\"Correct! You got it in %%d attempts.\\n\", attempts);\n");
        printf("              break;\n");
        printf("          }\n");
        printf("      }\n");
        printf("      return 0;\n");
        printf("  }\n");
        rule();
        printf("  This is EXAMPLE CODE. Yours may look nothing like it and\n");
        printf("  still be right.\n");
    }

    wait_enter();
    clear_screen();
    heading("TASK 2: A NORMAL PROGRAM - STUDENT GRADE MANAGER");

    printf("  Ask how many students. For each one, read a name and three\n");
    printf("  grades. Compute each student's average with a function.\n");
    printf("  Print every student with their average, then the name of the\n");
    printf("  one with the highest average.\n\n");

    printf("  The number of students is only known once the program is\n");
    printf("  running, which is exactly the case module 9 built malloc for.\n");
    printf("  Allocate the array with malloc, sized to what was typed, and\n");
    printf("  free it before the program ends.\n\n");

    printf("  Example run (with 2 students):\n\n");
    rule();
    printf("  How many students? 2\n");
    printf("  Name: Ana\n");
    printf("  Grade 1: 15\n");
    printf("  Grade 2: 18\n");
    printf("  Grade 3: 12\n");
    printf("  Name: Bea\n");
    printf("  Grade 1: 19\n");
    printf("  Grade 2: 19\n");
    printf("  Grade 3: 18\n");
    printf("  \n");
    printf("  Ana: 15.0\n");
    printf("  Bea: 18.7\n");
    printf("  Top student: Bea\n");
    rule();

    printf("\n  Try it first. Compile with:\n");
    printf("    gcc -Wall -Wextra test.c -o test && ./test\n");

    if (ask_yes("Want to see example code?")) {
        printf("\n");
        rule();
        printf("  #include <stdio.h>\n");
        printf("  #include <stdlib.h>\n");
        printf("  \n");
        printf("  struct Student {\n");
        printf("      char name[50];\n");
        printf("      float grades[3];\n");
        printf("      float average;\n");
        printf("  };\n");
        printf("  \n");
        printf("  float average_of(const struct Student *s)\n");
        printf("  {\n");
        printf("      float sum = 0;\n");
        printf("      for (int i = 0; i < 3; i++) {\n");
        printf("          sum += s->grades[i];\n");
        printf("      }\n");
        printf("      return sum / 3;\n");
        printf("  }\n");
        printf("  \n");
        printf("  int main(void)\n");
        printf("  {\n");
        printf("      int n;\n");
        printf("      printf(\"How many students? \");\n");
        printf("      if (scanf(\"%%d\", &n) != 1 || n <= 0) {\n");
        printf("          return 1;\n");
        printf("      }\n");
        printf("  \n");
        printf("      struct Student *students = malloc((size_t) n * sizeof(struct Student));\n");
        printf("      if (students == NULL) {\n");
        printf("          return 1;\n");
        printf("      }\n");
        printf("  \n");
        printf("      for (int i = 0; i < n; i++) {\n");
        printf("          printf(\"Name: \");\n");
        printf("          scanf(\"%%49s\", students[i].name);\n");
        printf("          for (int g = 0; g < 3; g++) {\n");
        printf("              printf(\"Grade %%d: \", g + 1);\n");
        printf("              scanf(\"%%f\", &students[i].grades[g]);\n");
        printf("          }\n");
        printf("          students[i].average = average_of(&students[i]);\n");
        printf("      }\n");
        printf("  \n");
        printf("      int best = 0;\n");
        printf("      for (int i = 1; i < n; i++) {\n");
        printf("          if (students[i].average > students[best].average) {\n");
        printf("              best = i;\n");
        printf("          }\n");
        printf("      }\n");
        printf("  \n");
        printf("      printf(\"\\n\");\n");
        printf("      for (int i = 0; i < n; i++) {\n");
        printf("          printf(\"%%s: %%.1f\\n\", students[i].name, students[i].average);\n");
        printf("      }\n");
        printf("      printf(\"Top student: %%s\\n\", students[best].name);\n");
        printf("  \n");
        printf("      free(students);\n");
        printf("      return 0;\n");
        printf("  }\n");
        rule();
        printf("  Note average_of takes a POINTER to a Student, not a copy.\n");
        printf("  A struct with three floats and a fifty-byte name is not\n");
        printf("  huge, but copying it on every call for no reason is the\n");
        printf("  kind of habit that costs real time once the struct grows.\n");
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    printf("   - the final test adds nothing but rand()/srand() -- the rest\n");
    printf("     is modules 1-10, combined into two full programs\n");
    printf("   - malloc a runtime-decided amount, and free it before exit\n");
    printf("   - pass structs by pointer to a function once they hold more\n");
    printf("     than a couple of fields\n\n");

    printf("  Module 12 goes further into memory, if you want it: pointers\n");
    printf("  to pointers, realloc, and linked lists.\n");
    wait_enter();
}

void lesson_12_deeper_memory(void)
{
    title("MODULE 12 - GOING DEEPER: MEMORY (EXTRA)");

    heading("PART 1: a pointer to a pointer");

    printf("  A 2D grid, sized at run time, is an array of pointers -- each\n");
    printf("  pointing at its own row:\n\n");

    printf("    int **grid = malloc(rows * sizeof(int *));\n");
    printf("    for (int r = 0; r < rows; r++) {\n");
    printf("        grid[r] = malloc(cols * sizeof(int));\n");
    printf("    }\n\n");

    printf("  `int **grid` reads the same way `int *p` did in module 8:\n");
    printf("  grid holds the address of something, and that something is\n");
    printf("  itself a pointer. grid[r] is one row's address; grid[r][c]\n");
    printf("  is one cell.\n\n");

    int rows = 3, cols = 4;
    int **grid = malloc((size_t) rows * sizeof(int *));
    for (int r = 0; r < rows; r++) {
        grid[r] = malloc((size_t) cols * sizeof(int));
    }
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            grid[r][c] = r * cols + c;
        }
    }

    printf("  Running now, a %dx%d grid:\n\n", rows, cols);
    for (int r = 0; r < rows; r++) {
        printf("    ");
        for (int c = 0; c < cols; c++) {
            printf("%2d ", grid[r][c]);
        }
        printf("\n");
    }

    printf("\n  Freeing it is two levels, inside out -- every row first,\n");
    printf("  then the array of rows:\n\n");
    printf("    for (int r = 0; r < rows; r++) {\n");
    printf("        free(grid[r]);\n");
    printf("    }\n");
    printf("    free(grid);\n\n");

    for (int r = 0; r < rows; r++) {
        free(grid[r]);
    }
    free(grid);
    printf("  Done -- freed correctly, both levels.\n\n");

    printf("  Freeing only the outer array is the mistake that makes this\n");
    printf("  worth a section. Every row is still allocated; only the list\n");
    printf("  of their addresses is gone. Real output from doing that:\n\n");
    printf("    ==12345==ERROR: LeakSanitizer: detected memory leaks\n");
    printf("    Direct leak of 48 byte(s) in 3 object(s) allocated from:\n\n");
    printf("  Forty-eight: three rows of four ints, sixteen bytes each.\n");

    wait_enter();
    clear_screen();
    heading("PART 2: realloc, and the bug in its own name");

    printf("  malloc asks for memory once. realloc resizes a block you\n");
    printf("  already have, keeping its contents:\n\n");

    printf("    int *v = malloc(2 * sizeof(int));\n");
    printf("    // ... later, needs more room ...\n");
    printf("    int *grown = realloc(v, 4 * sizeof(int));\n");
    printf("    if (grown == NULL) { free(v); return 1; }\n");
    printf("    v = grown;\n\n");

    printf("  Running a loop that doubles capacity whenever it fills up:\n\n");

    int capacity = 2, count = 0;
    int *v = malloc((size_t) capacity * sizeof(int));
    for (int i = 0; i < 5; i++) {
        if (count == capacity) {
            capacity *= 2;
            int *grown = realloc(v, (size_t) capacity * sizeof(int));
            if (grown == NULL) {
                free(v);
                printf("  realloc failed.\n");
                wait_enter();
                return;
            }
            v = grown;
        }
        v[count++] = i * 10;
    }
    printf("    ");
    for (int i = 0; i < count; i++) {
        printf("%d ", v[i]);
    }
    printf("\n\n");
    free(v);

    printf("  Notice the temporary `grown` variable. This is the one thing\n");
    printf("  to get right about realloc:\n\n");

    printf("    v = realloc(v, bigger);    <- WRONG\n\n");
    printf("  If realloc fails, it returns NULL -- and this line just\n");
    printf("  overwrote v with NULL. The original block is still out\n");
    printf("  there, allocated, but its only address just vanished. One\n");
    printf("  line, and you get a leak AND lose your data, at the exact\n");
    printf("  moment you were already low on memory.\n\n");

    printf("    int *grown = realloc(v, bigger);   <- correct\n");
    printf("    if (grown == NULL) { /* v is still valid here */ }\n");
    printf("    v = grown;\n");

    wait_enter();
    clear_screen();
    heading("PART 3: linked lists");

    printf("  An array's size is fixed once allocated. A linked list grows\n");
    printf("  one node at a time, each node holding its own next address:\n\n");

    printf("    struct Node {\n");
    printf("        int value;\n");
    printf("        struct Node *next;\n");
    printf("    };\n\n");

    printf("  A struct containing a pointer to its own type is legal --\n");
    printf("  the struct itself is not inside the pointer, only its\n");
    printf("  address is, and an address has a fixed size regardless of\n");
    printf("  what it points at.\n\n");

    printf("  Inserting at the head, five times:\n\n");
    printf("    struct Node *n = malloc(sizeof(struct Node));\n");
    printf("    n->value = i;\n");
    printf("    n->next = head;   // the new node points at the old head\n");
    printf("    head = n;         // and becomes the new head\n\n");

    struct Node {
        int value;
        struct Node *next;
    };

    struct Node *head = NULL;
    for (int i = 1; i <= 5; i++) {
        struct Node *n = malloc(sizeof(struct Node));
        n->value = i;
        n->next = head;
        head = n;
    }

    printf("  Running, then printed by walking `next` until NULL:\n\n");
    printf("    ");
    for (struct Node *cur = head; cur != NULL; cur = cur->next) {
        printf("%d ", cur->value);
    }
    printf("\n\n");

    printf("  5 4 3 2 1 -- the reverse of insertion order, because each new\n");
    printf("  node was pushed onto the front.\n\n");

    printf("  Freeing needs the same pattern as the 2D grid: walk the list,\n");
    printf("  and free() each node has to happen AFTER you have already\n");
    printf("  saved where `next` points -- freeing a node and then reading\n");
    printf("  its ->next afterwards is a use-after-free.\n\n");

    printf("    struct Node *cur = head;\n");
    printf("    while (cur != NULL) {\n");
    printf("        struct Node *next = cur->next;   // save it first\n");
    printf("        free(cur);\n");
    printf("        cur = next;                      // then move on\n");
    printf("    }\n\n");

    struct Node *cur = head;
    while (cur != NULL) {
        struct Node *next = cur->next;
        free(cur);
        cur = next;
    }
    printf("  Done -- every node freed, none leaked, none used after being\n");
    printf("  freed.\n");

    wait_enter();
    clear_screen();
    exercise(12);

    question("int **grid.  What does grid[2][3] mean?\n"
             "  (answer: a row, a pointer, or a value)",
             "a value",
             "grid[2] is a pointer to row 2. Indexing it again with [3]\n"
             "             reaches through to a single int.");

    question("You allocated 3 rows with malloc. You call free(grid)\n"
             "  without freeing the rows first. What happens to them?",
             "leak",
             "They are still allocated. Only the array holding their\n"
             "             addresses is gone -- nothing can reach them to free\n"
             "             them any more.");

    question("int *p = malloc(10); p = realloc(p, 20);\n"
             "  If realloc fails, what is p now?",
             "null",
             "realloc returned NULL, and this line just overwrote p with\n"
             "             it. The original 10-byte block still exists and is\n"
             "             now unreachable: a leak, at the worst possible time.");

    question("struct Node { int value; struct Node *next; };\n"
             "  Is a struct allowed to contain a pointer to its own type?\n"
             "  (answer: yes or no)",
             "yes",
             "A pointer has a fixed size no matter what it points at, so\n"
             "             this does not require infinite space the way an\n"
             "             actual nested Node field would.");

    {
        const char *task[] = {
            "Read integers until -1 is entered, inserting each one",
            "at the head of a linked list. Print the list, then",
            "free every node.",
        };
        const char *expected[] = {
            "Enter numbers, -1 to stop:",
            "3 2 1",
        };
        const char *solution[] = {
            "#include <stdio.h>",
            "#include <stdlib.h>",
            "",
            "struct Node {",
            "    int value;",
            "    struct Node *next;",
            "};",
            "",
            "int main(void)",
            "{",
            "    struct Node *head = NULL;",
            "    int n;",
            "",
            "    printf(\"Enter numbers, -1 to stop:\\n\");",
            "    while (scanf(\"%d\", &n) == 1 && n != -1) {",
            "        struct Node *node = malloc(sizeof(struct Node));",
            "        node->value = n;",
            "        node->next = head;",
            "        head = node;",
            "    }",
            "",
            "    for (struct Node *cur = head; cur != NULL; cur = cur->next) {",
            "        printf(\"%d \", cur->value);",
            "    }",
            "    printf(\"\\n\");",
            "",
            "    struct Node *cur = head;",
            "    while (cur != NULL) {",
            "        struct Node *next = cur->next;",
            "        free(cur);",
            "        cur = next;",
            "    }",
            "    return 0;",
            "}",
            "",
            "Input 1 2 3 -1 gives 3 2 1: each number is pushed onto the",
            "front, so the list ends up reversed from input order.",
        };
        const char *input[] = {"1", "2", "3", "-1"};
        challenge(task, 3, input, 4, expected, 2, solution, 37);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    printf("   - int **p is a pointer to a pointer; free it inside out,\n");
    printf("     every row before the array of rows\n");
    printf("   - always realloc into a temporary, never back into the\n");
    printf("     same variable you are resizing\n");
    printf("   - a struct may point at its own type; that is what a\n");
    printf("     linked list is\n");
    printf("   - save ->next before you free a node, or you cannot get to\n");
    printf("     the rest of the list\n\n");

    printf("  That is as far as this course goes. What you have now is\n");
    printf("  everything module 9 promised: you know where every byte you\n");
    printf("  asked for came from, and where it went.\n");
    wait_enter();
}
