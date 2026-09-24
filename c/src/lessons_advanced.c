/*
 * Modules 11 through 15 - past the core course.
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
 * Module 12 goes deeper into memory: pointer-to-pointer for a 2D grid, and
 * realloc for a growing array -- the extra requested after the final test,
 * for closing the gap between "malloc and free work" and "I trust my own
 * ownership rules".
 *
 * Module 13 is a second, unrelated way to grow a collection: a linked
 * list, one node at a time, instead of realloc's doubling blocks. Kept out
 * of module 12 on purpose -- a self-referential struct is a different idea
 * from either of module 12's, not a variation on them.
 *
 * Module 14 is the first data-structures module built on top of both:
 * stacks and queues, array-based -- a third underlying representation for
 * "a collection that grows", with its own tradeoff (fixed capacity instead
 * of one malloc per item). The trap for each is real and checked, not
 * simulated: a capacity check that actually runs and actually refuses the
 * operation, the same way module 12's realloc section never runs the broken
 * version of anything, only the guarded one.
 *
 * Module 15 steps back from data structures to name three things the
 * course has used informally since module 10: enum for a named integer
 * constant instead of a magic number or a #define, union for one block of
 * storage where only whichever member was written last is meaningfully
 * valid, and typedef for giving any of those -- or a plain type -- a
 * shorter name at the point of use. It comes after struct rather than as a
 * part inside it because none of the three is really about grouping
 * fields: enum replaces a number, union replaces a struct's assumption
 * that every field holds something at once, and typedef is a naming
 * convenience layered on top of all three. Module 10's struct padding
 * returns here too, extended to an enum field. Module 10 states the
 * alignment fact flatly; Part 4 here is explicit that it is a property of
 * this compiler and this machine, not a guarantee the language makes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lessons.h"
#include "ui.h"

/* Module 13's demo capacities. File scope, not inside the function, so
 * they read like the ordinary top-of-file constant every ".c" outside this
 * course would use -- a #define does not observe C's block scope anyway. */
#define STACK_CAP 5
#define QUEUE_CAP 3

/* Module 15's #define, kept at file scope for the same reason as the two
 * above: a #define does not observe C's block scope, so writing it inside
 * a function would be misleading about what it actually is. */
#define MAX_RETRIES 3

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

    printf("  Modules 12 and 13 go further into memory, if you want it:\n");
    printf("  pointers to pointers and realloc in one, linked lists in the\n");
    printf("  other.\n");
    wait_enter();
}

void lesson_12_growing_memory(void)
{
    title("MODULE 12 - GROWING MEMORY: GRIDS AND REALLOC");

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

    {
        const char *task[] = {
            "Read integers until -1 is entered, storing each one in an",
            "array that starts at capacity 2 and doubles (realloc)",
            "whenever it fills up. Print them all, space separated,",
            "then free the array.",
        };
        const char *input[] = {"1", "2", "3", "4", "-1"};
        const char *expected[] = {
            "Enter numbers, -1 to stop:",
            "1 2 3 4 ",
        };
        const char *solution[] = {
            "#include <stdio.h>",
            "#include <stdlib.h>",
            "",
            "int main(void)",
            "{",
            "    int capacity = 2, count = 0;",
            "    int *v = malloc((size_t) capacity * sizeof(int));",
            "    int n;",
            "",
            "    printf(\"Enter numbers, -1 to stop:\\n\");",
            "    while (scanf(\"%d\", &n) == 1 && n != -1) {",
            "        if (count == capacity) {",
            "            capacity *= 2;",
            "            int *grown = realloc(v, (size_t) capacity * sizeof(int));",
            "            if (grown == NULL) {",
            "                free(v);",
            "                return 1;",
            "            }",
            "            v = grown;",
            "        }",
            "        v[count++] = n;",
            "    }",
            "",
            "    for (int i = 0; i < count; i++) {",
            "        printf(\"%d \", v[i]);",
            "    }",
            "    printf(\"\\n\");",
            "",
            "    free(v);",
            "    return 0;",
            "}",
            "",
            "Input 1 2 3 4 -1 gives 1 2 3 4, in the order entered --",
            "appending to an array keeps insertion order for free, unlike",
            "module 13's linked list, which reverses it.",
        };
        challenge(task, 4, input, 5, expected, 2, solution,
                  (int) (sizeof solution / sizeof solution[0]));
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    printf("   - int **p is a pointer to a pointer; free it inside out,\n");
    printf("     every row before the array of rows\n");
    printf("   - always realloc into a temporary, never back into the\n");
    printf("     same variable you are resizing\n");
    printf("   - doubling capacity when full keeps the number of reallocs\n");
    printf("     small, even for an array that grows a lot\n\n");

    printf("  What you have now is everything module 9 promised: you know\n");
    printf("  where every byte you asked for came from, and where it went.\n\n");

    printf("  Module 13 is a second way to grow a collection: a linked\n");
    printf("  list, one node at a time instead of realloc's doubling\n");
    printf("  blocks.\n");
    wait_enter();
}

void lesson_13_linked_lists(void)
{
    title("MODULE 13 - LINKED LISTS");

    heading("PART 1: a struct pointing at its own type");

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

    printf("  Freeing needs the same pattern as module 12's 2D grid: walk\n");
    printf("  the list, and free() each node has to happen AFTER you have\n");
    printf("  already saved where `next` points -- freeing a node and\n");
    printf("  then reading its ->next afterwards is a use-after-free.\n\n");

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
    exercise(13);

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

    printf("   - a struct may point at its own type; that is what a\n");
    printf("     linked list is\n");
    printf("   - inserting at the head is O(1), but reverses the order\n");
    printf("     items came in\n");
    printf("   - save ->next before you free a node, or you cannot get to\n");
    printf("     the rest of the list\n\n");

    printf("  Module 14 builds on this: two data structures, stacks and\n");
    printf("  queues, using a plain array instead of another linked list.\n");
    wait_enter();
}

void lesson_14_stacks_queues(void)
{
    title("MODULE 14 - STACKS AND QUEUES");

    heading("PART 1: a stack, and what makes it a stack");

    printf("  A stack is last in, first out (LIFO): the most recent item\n");
    printf("  pushed is the first one popped. An array holds the items;\n");
    printf("  one index, `top`, tracks how many are in it:\n\n");

    printf("    #define STACK_CAP 5\n\n");
    printf("    struct Stack {\n");
    printf("        int items[STACK_CAP];\n");
    printf("        int top;   // how many items are in it right now\n");
    printf("    };\n\n");

    printf("    void stack_push(struct Stack *s, int value)\n");
    printf("    {\n");
    printf("        if (s->top == STACK_CAP) {\n");
    printf("            printf(\"Stack is full -- cannot push %%d\\n\", value);\n");
    printf("            return;\n");
    printf("        }\n");
    printf("        s->items[s->top] = value;\n");
    printf("        s->top++;\n");
    printf("    }\n\n");

    printf("    int stack_pop(struct Stack *s)\n");
    printf("    {\n");
    printf("        s->top--;\n");
    printf("        return s->items[s->top];\n");
    printf("    }\n\n");

    printf("  `top` is not an index into a full array -- it is a count.\n");
    printf("  `s->items[s->top]` after a push is always the next FREE\n");
    printf("  slot, not the one just written. This is the same pattern as\n");
    printf("  module 13's linked list count, applied to an index instead\n");
    printf("  of a pointer.\n\n");

    printf("  Running: five pushes fill a 5-slot stack exactly. A sixth\n");
    printf("  push has nowhere to go -- this is where the phrase \"stack\n");
    printf("  overflow\" comes from, and why a real stack checks `top`\n");
    printf("  against its capacity before writing:\n\n");

    struct Stack {
        int items[STACK_CAP];
        int top;
    };
    struct Stack demo_stack = { .top = 0 };
    for (int i = 1; i <= 5; i++) {
        int value = i * 10;
        if (demo_stack.top == STACK_CAP) {
            printf("    Stack is full -- cannot push %d\n", value);
        } else {
            demo_stack.items[demo_stack.top] = value;
            demo_stack.top++;
        }
    }
    {
        int value = 60;
        if (demo_stack.top == STACK_CAP) {
            printf("    Stack is full -- cannot push %d\n", value);
        }
    }

    printf("\n  Popping until empty prints them back in reverse -- the\n");
    printf("  last one pushed (50) is the first one out:\n\n");
    printf("    ");
    while (demo_stack.top > 0) {
        demo_stack.top--;
        printf("%d ", demo_stack.items[demo_stack.top]);
    }
    printf("\n");

    wait_enter();
    clear_screen();
    heading("PART 2: a queue, done the way that looks obvious first");

    printf("  A queue is first in, first out (FIFO): oldest item out\n");
    printf("  first. The obvious array version tracks two indices, `front`\n");
    printf("  (the oldest item) and `rear` (the next free slot), and just\n");
    printf("  moves them forward:\n\n");

    printf("    #define QUEUE_CAP 3\n\n");
    printf("    struct QueueNaive {\n");
    printf("        int items[QUEUE_CAP];\n");
    printf("        int front;\n");
    printf("        int rear;\n");
    printf("    };\n\n");

    printf("  Enqueue writes at `rear` and moves it forward. Dequeue reads\n");
    printf("  at `front` and moves IT forward. Neither ever goes back down.\n\n");

    printf("  Running: fill a 3-slot queue, then dequeue two of the three\n");
    printf("  items. Only one item is left -- there should be room for two\n");
    printf("  more:\n\n");

    struct QueueNaive {
        int items[QUEUE_CAP];
        int front;
        int rear;
    };
    struct QueueNaive naive = { .front = 0, .rear = 0 };
    for (int i = 1; i <= 3; i++) {
        naive.items[naive.rear] = i;
        naive.rear++;
    }
    printf("    dequeued: %d\n", naive.items[naive.front++]);
    printf("    dequeued: %d\n", naive.items[naive.front++]);
    printf("    1 item left (the %d), 2 free slots logically -- but:\n\n", naive.items[naive.front]);

    if (naive.rear == QUEUE_CAP) {
        printf("    Queue full -- cannot enqueue 4\n");
    }

    printf("\n  `rear` reached QUEUE_CAP and never comes back down, so the\n");
    printf("  queue reports full with only one real item in it. The two\n");
    printf("  slots dequeue freed up (index 0 and 1) are just sitting\n");
    printf("  there, unreachable, exactly like the leaked rows in module\n");
    printf("  12's grid -- nothing is wrong with the memory, the bookkeeping\n");
    printf("  just never looks back.\n");

    wait_enter();
    clear_screen();
    heading("PART 3: the fix -- wrap the indices around");

    printf("  The slots freed by dequeue are real and reusable. `front`\n");
    printf("  and `rear` just need to wrap back to 0 after the last index\n");
    printf("  instead of growing forever. That is what %% (modulo) is for:\n\n");

    printf("    struct Queue {\n");
    printf("        int items[QUEUE_CAP];\n");
    printf("        int front;\n");
    printf("        int count;   // how many items, not an index\n");
    printf("    };\n\n");

    printf("    void queue_enqueue(struct Queue *q, int value)\n");
    printf("    {\n");
    printf("        if (q->count == QUEUE_CAP) {\n");
    printf("            printf(\"Queue full -- cannot enqueue %%d\\n\", value);\n");
    printf("            return;\n");
    printf("        }\n");
    printf("        int rear = (q->front + q->count) %% QUEUE_CAP;\n");
    printf("        q->items[rear] = value;\n");
    printf("        q->count++;\n");
    printf("    }\n\n");

    printf("    int queue_dequeue(struct Queue *q)\n");
    printf("    {\n");
    printf("        int value = q->items[q->front];\n");
    printf("        q->front = (q->front + 1) %% QUEUE_CAP;\n");
    printf("        q->count--;\n");
    printf("        return value;\n");
    printf("    }\n\n");

    printf("  `count` replaced `rear` as a real number of items, and\n");
    printf("  `rear` is now computed each time instead of stored. Running\n");
    printf("  the same sequence as PART 2, then enqueuing two more and\n");
    printf("  draining it completely:\n\n");

    struct Queue {
        int items[QUEUE_CAP];
        int front;
        int count;
    };
    struct Queue q = { .front = 0, .count = 0 };

    /* enqueue 1, 2, 3 */
    for (int i = 1; i <= 3; i++) {
        int rear = (q.front + q.count) % QUEUE_CAP;
        q.items[rear] = i;
        q.count++;
    }
    /* dequeue two */
    for (int i = 0; i < 2; i++) {
        printf("    dequeued: %d\n", q.items[q.front]);
        q.front = (q.front + 1) % QUEUE_CAP;
        q.count--;
    }
    /* enqueue 4, 5 -- succeeds now, unlike the naive version */
    for (int i = 4; i <= 5; i++) {
        int rear = (q.front + q.count) % QUEUE_CAP;
        q.items[rear] = i;
        q.count++;
    }
    printf("    dequeued: ");
    while (q.count > 0) {
        printf("%d ", q.items[q.front]);
        q.front = (q.front + 1) % QUEUE_CAP;
        q.count--;
    }
    printf("\n\n");

    printf("  3 4 5 -- the two slots the naive version considered gone\n");
    printf("  for good got reused, and every item still came out in the\n");
    printf("  order it went in.\n");

    wait_enter();
    clear_screen();
    exercise(14);

    question("Array-based stack, capacity 5, already holds 5 items.\n"
             "  You push one more without checking capacity first.\n"
             "  What is this called? (one word)",
             "overflow",
             "Writing past items[4] into memory the stack does not own --\n"
             "             \"stack overflow\" is a description of exactly this,\n"
             "             not a separate phenomenon.");

    question("Naive array queue: enqueue 3 items (fills capacity 3),\n"
             "  dequeue 2. One item remains. Can you enqueue a new one\n"
             "  without changing how `rear` works? (yes or no)",
             "no",
             "`rear` already reached QUEUE_CAP and only ever moves\n"
             "             forward, so the queue reports full with two\n"
             "             genuinely free slots sitting behind `front`.");

    question("What operator turns an index that would run off the end\n"
             "  of the array back into a valid one? (symbol)",
             "%",
             "index = (index + 1) % QUEUE_CAP wraps CAP back to 0 --\n"
             "             the same modulo used for `rand() % 100` in module\n"
             "             11, applied to an index instead of a random range.");

    {
        const char *task[] = {
            "Read one line: a string of ( ) [ ] { } characters.",
            "Using a stack, check whether every bracket is closed",
            "by the same type, in the right order. Print Balanced",
            "or Not balanced.",
        };
        const char *input[] = { "([{}])" };
        const char *expected[] = { "Line: ([{}])", "Balanced" };
        const char *solution[] = {
            "#include <stdio.h>",
            "",
            "#define CAP 100",
            "",
            "struct Stack {",
            "    char items[CAP];",
            "    int top;",
            "};",
            "",
            "void push(struct Stack *s, char c)",
            "{",
            "    s->items[s->top] = c;",
            "    s->top++;",
            "}",
            "",
            "char pop(struct Stack *s)",
            "{",
            "    s->top--;",
            "    return s->items[s->top];",
            "}",
            "",
            "int matches(char open, char close)",
            "{",
            "    return (open == '(' && close == ')') ||",
            "           (open == '[' && close == ']') ||",
            "           (open == '{' && close == '}');",
            "}",
            "",
            "int main(void)",
            "{",
            "    char line[200];",
            "    printf(\"Line: \");",
            "    if (!fgets(line, sizeof line, stdin)) {",
            "        return 1;",
            "    }",
            "",
            "    struct Stack s = { .top = 0 };",
            "    int ok = 1;",
            "",
            "    for (int i = 0; line[i] != '\\0' && line[i] != '\\n'; i++) {",
            "        char c = line[i];",
            "        if (c == '(' || c == '[' || c == '{') {",
            "            push(&s, c);",
            "        } else if (c == ')' || c == ']' || c == '}') {",
            "            if (s.top == 0 || !matches(pop(&s), c)) {",
            "                ok = 0;",
            "                break;",
            "            }",
            "        }",
            "    }",
            "    if (s.top != 0) {",
            "        ok = 0;",
            "    }",
            "",
            "    printf(\"%s\\n\", ok ? \"Balanced\" : \"Not balanced\");",
            "    return 0;",
            "}",
            "",
            "An opening bracket is pushed. A closing one pops and must",
            "match what comes back -- a ']' popping a '(' is wrong even",
            "though both are \"open\" brackets. s.top != 0 at the end",
            "means something opened was never closed.",
        };
        challenge(task, 4, input, 1, expected, 2, solution, 60);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    printf("   - a stack is LIFO: push and pop both happen at `top`\n");
    printf("   - a queue is FIFO: enqueue at the back, dequeue from the\n");
    printf("     front\n");
    printf("   - an array-based stack or queue needs a real capacity\n");
    printf("     check before writing -- \"stack overflow\" is that check\n");
    printf("     missing, not a metaphor\n");
    printf("   - a naive array queue reports full long before it actually\n");
    printf("     is, because `front` and `rear` only ever move forward\n");
    printf("   - %% (modulo) wraps an index back to 0, which is what turns\n");
    printf("     a naive queue into a correct, reusable one\n\n");

    printf("  Both used a fixed-size array on purpose. A linked list (like\n");
    printf("  module 13's) removes the capacity limit entirely, at the\n");
    printf("  cost of one malloc per item instead of one for the whole\n");
    printf("  block -- the same tradeoff module 9 first raised between\n");
    printf("  an array and a linked structure.\n");
    wait_enter();
}

void lesson_15_enums_unions(void)
{
    title("MODULE 15 - ENUMS, UNIONS, AND TYPEDEF");

    heading("PART 1: enum -- named constants instead of magic numbers");

    printf("    enum Color { RED, GREEN, BLUE };\n\n");

    enum Color { RED, GREEN, BLUE };
    enum Color c = GREEN;

    printf("  Running -- these are real int values, not just labels:\n\n");
    printf("    RED   = %d\n", RED);
    printf("    GREEN = %d\n", GREEN);
    printf("    BLUE  = %d\n", BLUE);
    printf("    sizeof(enum Color) = %zu bytes\n\n", sizeof(enum Color));

    printf("  With no = given, the first name is 0 and each one after is\n");
    printf("  one more than the last. c above holds GREEN, which prints\n");
    printf("  as an ordinary int: %d.\n\n", c);

    printf("  A number can be assigned instead, and counting picks up\n");
    printf("  from there:\n\n");

    enum Status { OK = 0, FAIL = 1 };

    printf("    enum Status { OK = 0, FAIL = 1 };\n\n");
    printf("  Running:\n");
    printf("    OK   = %d\n", OK);
    printf("    FAIL = %d\n\n", FAIL);

    printf("  Compare that to the way earlier modules flagged a limit:\n\n");
    printf("    #define MAX_RETRIES 3\n\n");
    printf("  Running: MAX_RETRIES expands to %d -- the same kind of\n",
           MAX_RETRIES);
    printf("  plain int RED is.\n\n");

    printf("  The difference does not show up in that printf. It shows up\n");
    printf("  everywhere else: RED itself is still a plain int, but enum\n");
    printf("  Color is a real declared type name -- a variable declared\n");
    printf("  as one keeps that name for a debugger to show, and\n");
    printf("  documents intent in a function's parameter list. C does\n");
    printf("  NOT check that only RED, GREEN, or BLUE ever end up in\n");
    printf("  such a variable; enforcing that is what C++'s enum class\n");
    printf("  adds, not plain C. MAX_RETRIES has none of this -- it is\n");
    printf("  gone before the compiler even sees it, replaced by the\n");
    printf("  digit 3, with no type, no scope of its own, and no name\n");
    printf("  left anywhere for a debugger to print.\n");

    wait_enter();
    clear_screen();
    heading("PART 2: union -- one block of storage, one member at a time");

    printf("    union Data { int i; double d; };\n\n");

    union Data { int i; double d; };
    union Data data;
    struct Pair { int i; double d; };

    printf("  A struct with these same two fields needs room for both. A\n");
    printf("  union needs room for only the LARGER one, because every\n");
    printf("  member starts at the same address:\n\n");

    printf("    sizeof(int)          = %zu bytes\n", sizeof(int));
    printf("    sizeof(double)       = %zu bytes\n", sizeof(double));
    printf("    sizeof(union Data)   = %zu bytes  <- the larger member\n",
           sizeof(union Data));
    printf("    sizeof(struct Pair)  = %zu bytes  <- both fields, plus\n",
           sizeof(struct Pair));
    printf("                                          padding (Part 4)\n\n");

    data.i = 42;
    printf("    data.i = 42;\n");
    printf("    printf(\"%%d\", data.i);   ->  %d\n\n", data.i);

    printf("  data.d shares the exact same bytes data.i just used, and\n");
    printf("  those bytes were never written as a double. Reading data.d\n");
    printf("  right now is undefined behaviour by the C standard's own\n");
    printf("  definition, not \"some garbage value\" -- which is exactly\n");
    printf("  why this course will not print one. Rule 3 of this course\n");
    printf("  applies to a union the same way it applies to a freed\n");
    printf("  pointer: explain the trap, do not run it.\n\n");

    printf("  Only whichever member was written last is meaningfully\n");
    printf("  valid. A union is not a way to hold two values at once --\n");
    printf("  it is a way to spend the space for one, when only one of\n");
    printf("  several types is ever needed at a time. Code that needs to\n");
    printf("  know WHICH member is currently valid stores that\n");
    printf("  separately, often as an enum sitting next to the union.\n");

    wait_enter();
    clear_screen();
    heading("PART 3: typedef -- naming a type");

    printf("    typedef int count_t;\n");
    printf("    count_t n = 5;\n\n");

    typedef int count_t;
    count_t n = 5;

    printf("  Running: n = %d, sizeof(count_t) = %zu, sizeof(int) = %zu\n",
           n, sizeof(count_t), sizeof(int));
    printf("  -- count_t IS int, just spelled differently. typedef does\n");
    printf("  not create a new type, only a new name for an existing one.\n\n");

    printf("  The real payoff is dropping struct/enum/union at the point\n");
    printf("  of use:\n\n");

    printf("    typedef struct { int x; int y; } Point;\n");
    printf("    Point p = { 3, 4 };        // no \"struct\" needed here\n\n");

    typedef struct { int x; int y; } Point;
    Point p = { 3, 4 };

    printf("  Running: p.x = %d, p.y = %d\n\n", p.x, p.y);

    printf("  The same works for an enum:\n\n");
    printf("    typedef enum { MODE_ON, MODE_OFF } Mode;\n");
    printf("    Mode m = MODE_ON;          // no \"enum\" needed here\n\n");

    typedef enum { MODE_ON, MODE_OFF } Mode;
    Mode m = MODE_ON;

    printf("  Running: m = %d\n\n", m);

    printf("  One trap: `typedef struct Foo { ... } Foo;` names Foo\n");
    printf("  twice on purpose, not by accident. `struct Foo` is a tag --\n");
    printf("  its own namespace, separate from ordinary identifiers -- and\n");
    printf("  the typedef name Foo is a different identifier that happens\n");
    printf("  to reuse the same spelling. The tag is what module 13's\n");
    printf("  linked list used inside its own struct (`struct Node\n");
    printf("  *next;`): the typedef name is not defined until the\n");
    printf("  semicolon closing the whole declaration, so it cannot be\n");
    printf("  used to refer to the type from inside its own braces.\n");

    wait_enter();
    clear_screen();
    heading("PART 4: padding, revisited with an enum in the mix");

    printf("  An enum constant is really just an int (Part 1), so a\n");
    printf("  variable of an enum type costs as much space as one and\n");
    printf("  aligns the same way. The field-order trap module 10 showed\n");
    printf("  for int applies to it too:\n\n");

    struct Bad2  { char a; enum Color color; char c; };
    struct Good2 { char a; char c; enum Color color; };

    printf("    struct Bad2  { char a; enum Color color; char c; };\n");
    printf("    struct Good2 { char a; char c; enum Color color; };\n\n");

    printf("  Running, on this machine, compiled with gcc:\n\n");
    printf("    sizeof(struct Bad2)  = %zu bytes\n", sizeof(struct Bad2));
    printf("    sizeof(struct Good2) = %zu bytes\n\n", sizeof(struct Good2));

    printf("  Same three fields, same types, different order, different\n");
    printf("  size -- module 10's Bad/Good result again, with an enum\n");
    printf("  standing in for the int. This is what THIS compiler, on\n");
    printf("  THIS machine, actually produced just now. The C standard\n");
    printf("  guarantees none of it: not enum's size, not its alignment,\n");
    printf("  not how much padding a compiler chooses to insert. A\n");
    printf("  different compiler, a different target, or a packing\n");
    printf("  pragma could all legally print different numbers for the\n");
    printf("  exact same two structs.\n");

    wait_enter();
    clear_screen();
    exercise(15);

    question("In `enum Color { RED, GREEN, BLUE };`, what is the real\n"
             "  type of the constant RED? (one word)",
             "int",
             "An enum introduces a set of named int constants. The enum\n"
             "             type itself types a VARIABLE that holds one of them;\n"
             "             the constants themselves are plain ints.");

    question("union Data { int i; double d; }; data.i = 5; -- how many\n"
             "  of its members hold a value the program actually put\n"
             "  there right now? (a number)",
             "1",
             "Every member shares the same storage. Only i was written,\n"
             "             so only i is meaningfully valid. Reading d now\n"
             "             would be undefined behaviour, not a second value.");

    question("typedef struct Foo { int x; } Foo; -- is `struct Foo` and\n"
             "  the typedef name `Foo` the same identifier written twice,\n"
             "  or two different ones? (answer: same or different)",
             "different",
             "The tag Foo lives in its own namespace, separate from the\n"
             "             typedef name Foo. Naming the tag is optional; giving\n"
             "             the typedef a target type on its left is not.");

    question("struct S { char a; enum Color color; }; -- is sizeof(S)\n"
             "  guaranteed to be the same on every C compiler? (yes or no)",
             "no",
             "The standard fixes neither enum's size nor its alignment,\n"
             "             so how much padding a compiler inserts around it is\n"
             "             a property of the compiler and platform, not the\n"
             "             language.");

    {
        const char *task[] = {
            "Read one integer, the day number typed after the prompt",
            "below. Using a typedef'd enum Day (SUNDAY = 0 through",
            "SATURDAY = 6), print the day's name by indexing an array",
            "of strings with it. If the number is not 0-6, print",
            "\"Invalid day.\" instead.",
        };
        const char *input[] = { "3" };
        const char *expected[] = {
            "Day number: 3",
            "Wednesday",
        };
        const char *solution[] = {
            "#include <stdio.h>",
            "",
            "typedef enum {",
            "    SUNDAY, MONDAY, TUESDAY, WEDNESDAY,",
            "    THURSDAY, FRIDAY, SATURDAY",
            "} Day;",
            "",
            "int main(void)",
            "{",
            "    const char *names[] = {",
            "        \"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\",",
            "        \"Thursday\", \"Friday\", \"Saturday\",",
            "    };",
            "    int n;",
            "    Day d;",
            "",
            "    printf(\"Day number: \");",
            "    if (scanf(\"%d\", &n) != 1 || n < SUNDAY || n > SATURDAY) {",
            "        printf(\"Invalid day.\\n\");",
            "        return 1;",
            "    }",
            "",
            "    d = n;",
            "    printf(\"%s\\n\", names[d]);",
            "    return 0;",
            "}",
            "",
            "n is checked against SUNDAY and SATURDAY, the enum's own",
            "first and last constants, before it is trusted as an index --",
            "the same bounds check module 6 required for any array.",
        };
        challenge(task, 5, input, 1, expected, 2, solution,
                  (int) (sizeof solution / sizeof solution[0]));
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    printf("   - enum names a set of int constants and gives the enum\n");
    printf("     itself a real type name; unlike #define, a variable of\n");
    printf("     that type stays visible to a debugger (C does not check\n");
    printf("     that only a valid member ever ends up in it)\n");
    printf("   - union members share one block of storage; sizeof is the\n");
    printf("     largest member, not the sum, and only the last-written\n");
    printf("     member is safe to read back\n");
    printf("   - typedef names an existing type; it earns its keep on\n");
    printf("     struct/enum/union, dropping the keyword at every use\n");
    printf("   - padding is a property of the compiler and the machine,\n");
    printf("     not the language -- true for an int (module 10) and\n");
    printf("     just as true for an enum\n\n");

    printf("  That covers every kind of type this course builds by hand.\n");
    printf("  What is left from here is practice: bigger programs that\n");
    printf("  combine all of it, the way module 11's final test combined\n");
    printf("  modules 1 through 10.\n");
    wait_enter();
}
