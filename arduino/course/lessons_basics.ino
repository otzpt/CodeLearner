/*
 * Modules 1 to 6 - the basics.
 *
 * Rule used throughout: nothing is claimed without being shown. Where an
 * example can run here, it runs. Where it cannot -- because the PC is not
 * the chip -- the lesson says so instead of pretending.
 */

void lesson_01_sketch()
{
    title("MODULE 1 - YOUR FIRST SKETCH");

    heading("PART 1: no main(), two functions instead");

    Serial.println("  A C program starts at main(). A sketch has no main() that");
    Serial.println("  you write. It has two functions, and the board's startup");
    Serial.println("  code calls them for you:");
    Serial.println();
    Serial.println("    void setup() { }   // runs ONCE, at power-on or reset");
    Serial.println("    void loop()  { }   // runs again and again, forever");
    Serial.println();
    Serial.println("  The hidden main() is, in shape, just this:");
    Serial.println();
    Serial.println("    setup();");
    Serial.println("    for (;;) { loop(); }");
    Serial.println();
    Serial.println("  This course is a sketch too. setup() at the bottom of");
    Serial.println("  course.ino ran once when you started it; loop() is what");
    Serial.println("  draws the menu each time you come back to it.");
    Serial.println();
    Serial.println("  One honest limit: there is no board here. The sketch runs");
    Serial.println("  on this PC against shim/Arduino.h, a stand-in for the");
    Serial.println("  Arduino core. Serial goes to your terminal, and pins,");
    Serial.println("  sensors and time are simulated. The code you write is the");
    Serial.println("  same code a board would run.");

    wait_enter();
    clear_screen();
    heading("PART 2: Serial.print and Serial.println");

    Serial.println("    Serial.begin(9600);   // open the serial line, 9600 baud");
    Serial.println("    Serial.print(\"a\");");
    Serial.println("    Serial.print(\"b\");");
    Serial.println("    Serial.println(\"c\");");
    Serial.println();
    Serial.print("  Running:  ");
    Serial.print("a");
    Serial.print("b");
    Serial.println("c");
    Serial.println();
    Serial.println("  print() leaves the cursor where it is; println() adds a");
    Serial.println("  line break after. Serial.begin(9600) sets the speed in");
    Serial.println("  bits per second, and the other end -- the Serial Monitor on");
    Serial.println("  a real board -- must be set to the same number or it shows");
    Serial.println("  garbage. Here it changes nothing; on a board it matters.");

    wait_enter();
    clear_screen();
    heading("PART 3: what Serial prints for numbers");

    Serial.println("    Serial.println(3.14159);");
    Serial.print("  Running:  ");
    Serial.println(3.14159);
    Serial.println("    Serial.println(3.14159, 4);");
    Serial.print("  Running:  ");
    Serial.println(3.14159, 4);
    Serial.println();
    Serial.println("  Two decimals by default. The value is not rounded, only");
    Serial.println("  what is printed. A second argument chooses the decimals.");
    Serial.println();
    Serial.println("    Serial.println(78);          // decimal");
    Serial.println("    Serial.println(78, BIN);     // binary");
    Serial.println("    Serial.println(78, HEX);     // hexadecimal");
    Serial.print("  Running:  ");
    Serial.println(78);
    Serial.print("            ");
    Serial.println(78, BIN);
    Serial.print("            ");
    Serial.println(78, HEX);
    Serial.println();
    Serial.println("  And one trap: a character is not a number.");
    Serial.println();
    Serial.println("    Serial.println('A');         // the character");
    Serial.println("    Serial.println(65);          // the number");
    Serial.print("  Running:  ");
    Serial.println('A');
    Serial.print("            ");
    Serial.println(65);

    wait_enter();
    clear_screen();
    exercise(1);

    question("How many times does the board run setup() between power-on\n"
             "  and the next reset?",
             "1", "setup() runs once. loop() is the one that repeats.");
    question("Serial.println(2.71828); -- what appears on the screen?",
             "2.72", "Two decimals by default; the second argument changes that.");

    {
        const char *task[] = {
            "Print the word start, then the numbers 1, 2 and 3, each on its own",
            "line. Do it in setup(); loop() can stay empty.",
        };
        const char *expected[] = {"start", "1", "2", "3"};
        const char *solution[] = {
            "void setup() {",
            "  Serial.begin(9600);",
            "  Serial.println(\"start\");",
            "  Serial.println(1);",
            "  Serial.println(2);",
            "  Serial.println(3);",
            "}",
            "",
            "void loop() { }",
        };
        challenge(task, 2, NULL, 0, expected, 4, solution, 9);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    Serial.println("   - a sketch has setup() (once) and loop() (forever), no main()");
    Serial.println("   - Serial.print stays on the line, Serial.println ends it");
    Serial.println("   - Serial.println(x, 4) sets the decimals; (x, BIN) the base");
    Serial.println("   - 'A' and 65 are different things to Serial");
    Serial.println();
    Serial.println("  Module 2: how big a number is on a chip this small.");
    wait_enter();
}

void lesson_02_types()
{
    title("MODULE 2 - VARIABLES AND TYPES");

    heading("PART 1: a number has a size, and the size depends on the chip");

    Serial.println("  An Arduino Uno has a 2 KB working memory, so the size of");
    Serial.println("  every variable matters. On the Uno (Arduino reference):");
    Serial.println();
    Serial.println("    byte / uint8_t     1 byte    0 to 255");
    Serial.println("    int / int16_t      2 bytes   -32768 to 32767");
    Serial.println("    long / int32_t     4 bytes");
    Serial.println("    float              4 bytes");
    Serial.println("    double             4 bytes   (same as float on the Uno)");
    Serial.println();
    Serial.println("  Now the PC this course is running on:");
    Serial.println();
    Serial.print("    sizeof(int)       = ");
    Serial.println(sizeof(int));
    Serial.print("    sizeof(double)    = ");
    Serial.println(sizeof(double));
    Serial.println();
    Serial.println("  Different. An int is 4 bytes here and 2 on an Uno, so a");
    Serial.println("  program that works on one can break on the other. The fix");
    Serial.println("  is the fixed-width types, which mean the same everywhere:");
    Serial.println();
    Serial.print("    sizeof(uint8_t)   = ");
    Serial.println(sizeof(uint8_t));
    Serial.print("    sizeof(int16_t)   = ");
    Serial.println(sizeof(int16_t));
    Serial.print("    sizeof(uint32_t)  = ");
    Serial.println(sizeof(uint32_t));
    Serial.println();
    Serial.println("  When a size matters, name it: uint8_t, int16_t, uint32_t.");

    wait_enter();
    clear_screen();
    heading("PART 2: wrapping around");

    uint8_t small = 255;
    small = small + 1;
    uint16_t medium = 65535;
    medium = medium + 1;

    Serial.println("    uint8_t small = 255;");
    Serial.println("    small = small + 1;");
    Serial.print("  Running:  small is now ");
    Serial.println(small);
    Serial.println();
    Serial.println("    uint16_t medium = 65535;");
    Serial.println("    medium = medium + 1;");
    Serial.print("  Running:  medium is now ");
    Serial.println(medium);
    Serial.println();
    Serial.println("  An unsigned number past its top goes back to 0, with no");
    Serial.println("  error. A counter that wraps silently is a real bug source:");
    Serial.println("  a uint8_t cannot count to 300.");
    Serial.println();
    Serial.println("  A SIGNED number past its top is worse: the language calls it");
    Serial.println("  undefined behaviour. On an Uno, int x = 32767; x = x + 1; is");
    Serial.println("  exactly that, so it is not run here. Keep signed values");
    Serial.println("  well inside their range.");

    wait_enter();
    clear_screen();
    heading("PART 3: division, and the cast that comes too late");

    Serial.println("    Serial.println(7 / 2);");
    Serial.print("  Running:  ");
    Serial.println(7 / 2);
    Serial.println("    Serial.println(7 % 2);");
    Serial.print("  Running:  ");
    Serial.println(7 % 2);
    Serial.println("    Serial.println(7 / 2.0);");
    Serial.print("  Running:  ");
    Serial.println(7 / 2.0);
    Serial.println("    Serial.println((float)(7 / 2));    // cast too late");
    Serial.print("  Running:  ");
    Serial.println((float) (7 / 2));
    Serial.println("    Serial.println((float)7 / 2);      // cast first");
    Serial.print("  Running:  ");
    Serial.println((float) 7 / 2);
    Serial.println();
    Serial.println("  Integer divided by integer is an integer: 7 / 2 is 3, and");
    Serial.println("  % gives the remainder. Making one side a float first gives");
    Serial.println("  3.50. Casting the RESULT of 7 / 2 changes nothing: the");
    Serial.println("  fraction was already thrown away.");
    Serial.println();
    Serial.println("  A float holds about 7 significant digits, not more:");
    Serial.println();

    float big = 16777216.0f;
    big = big + 1.0f;
    Serial.println("    float big = 16777216.0;");
    Serial.println("    big = big + 1.0;");
    Serial.print("  Running:  big is ");
    Serial.println(big, 0);
    Serial.println("  The +1 vanished. Past 2^24 a float cannot count by one.");

    wait_enter();
    clear_screen();
    exercise(2);

    question("uint8_t b = 255;  b = b + 1;   What is b?", "0",
             "An unsigned number wraps back to 0 past its top.");
    question("Serial.println(5 / 2);   What appears?", "2",
             "int divided by int stays an int; the .5 is dropped.");

    {
        const char *task[] = {
            "Put 7 and 2 in two variables. Print their integer division, their",
            "remainder, and their division as a decimal, each on its own line.",
        };
        const char *expected[] = {"3", "1", "3.50"};
        const char *solution[] = {
            "void setup() {",
            "  Serial.begin(9600);",
            "  int a = 7;",
            "  int b = 2;",
            "  Serial.println(a / b);",
            "  Serial.println(a % b);",
            "  Serial.println((float)a / b);",
            "}",
            "",
            "void loop() { }",
        };
        challenge(task, 2, NULL, 0, expected, 3, solution, 10);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    Serial.println("   - an int is 2 bytes on an Uno, 4 on this PC: name the size");
    Serial.println("     with uint8_t, int16_t, uint32_t when it matters");
    Serial.println("   - unsigned numbers wrap to 0; signed overflow is undefined");
    Serial.println("   - int / int drops the fraction; cast one side FIRST");
    Serial.println("   - a float keeps about 7 digits");
    Serial.println();
    Serial.println("  Module 3: making decisions, and repeating.");
    wait_enter();
}

void lesson_03_decisions()
{
    title("MODULE 3 - DECISIONS AND LOOPS");

    heading("PART 1: if / else, and the = that should be ==");

    int sensor = 620;
    Serial.println("    int sensor = 620;");
    Serial.println("    if (sensor > 500) { Serial.println(\"bright\"); }");
    Serial.println("    else              { Serial.println(\"dark\");   }");
    Serial.print("  Running:  ");
    if (sensor > 500) {
        Serial.println("bright");
    } else {
        Serial.println("dark");
    }
    Serial.println();
    Serial.println("  Comparison uses ==, !=, <, >, <=, >=. The classic slip is");
    Serial.println("  writing = (assign) where == (compare) was meant:");
    Serial.println();
    Serial.println("    if (sensor = 500) { ... }");
    Serial.println();
    Serial.println("  That sets sensor to 500, and 500 is not zero, so the test");
    Serial.println("  is always true. The compiler warns, when asked to:");
    Serial.println();
    Serial.println("    warning: suggest parentheses around assignment used as");
    Serial.println("    truth value [-Wparentheses]");
    Serial.println();
    Serial.println("  Warnings are on in this course's Makefile for that reason.");

    wait_enter();
    clear_screen();
    heading("PART 2: for and while");

    Serial.println("    for (int i = 0; i < 4; i++) {");
    Serial.println("      Serial.print(i);");
    Serial.println("      Serial.print(\" \");");
    Serial.println("    }");
    Serial.print("  Running:  ");
    for (int i = 0; i < 4; i++) {
        Serial.print(i);
        Serial.print(" ");
    }
    Serial.println();
    Serial.println();
    Serial.println("  Three parts: where to start, when to keep going, what to do");
    Serial.println("  after each pass. i < 4 runs for 0, 1, 2, 3: FOUR passes.");
    Serial.println("  Counting from 0 is why an array of 4 has no index 4.");
    Serial.println();
    Serial.println("    int n = 3;");
    Serial.println("    while (n > 0) { Serial.print(n); n--; }");
    Serial.print("  Running:  ");
    int n = 3;
    while (n > 0) {
        Serial.print(n);
        n--;
    }
    Serial.println();
    Serial.println();
    Serial.println("  while checks first, then runs. And remember: loop() itself");
    Serial.println("  is a loop that never ends -- a for or while INSIDE loop()");
    Serial.println("  that never ends would freeze the whole board.");

    wait_enter();
    clear_screen();
    heading("PART 3: && || ! and break");

    int distance = 12;
    bool left_clear = true;
    Serial.println("    int distance = 12;  bool left_clear = true;");
    Serial.println("    if (distance < 20 && left_clear) { turn left }");
    Serial.print("  Running:  ");
    if (distance < 20 && left_clear) {
        Serial.println("turn left");
    }
    Serial.println();
    Serial.println("  && is AND, || is OR, ! is NOT. && stops at the first false");
    Serial.println("  and || at the first true, so the right side may never run.");
    Serial.println();
    Serial.println("    for (int i = 0; i < 10; i++) {");
    Serial.println("      if (i == 3) { break; }       // leave the loop now");
    Serial.println("      Serial.print(i);");
    Serial.println("    }");
    Serial.print("  Running:  ");
    for (int i = 0; i < 10; i++) {
        if (i == 3) {
            break;
        }
        Serial.print(i);
    }
    Serial.println();
    Serial.println();
    Serial.println("  A bool prints as 1 or 0: Serial.println(true) shows 1.");

    wait_enter();
    clear_screen();
    exercise(3);

    question("for (int i = 0; i < 3; i++) -- how many times does the\n"
             "  body run?",
             "3", "i takes 0, 1 and 2; at 3 the test i < 3 fails.");
    question("for (int i = 1; i <= 4; i++) Serial.print(i);\n"
             "  What appears? (digits only, no spaces)",
             "1234", "Starts at 1, keeps going while i <= 4, so 1 2 3 4.");

    {
        const char *task[] = {
            "Print the multiples of 3 from 3 up to 12, each on its own line.",
            "Use a loop.",
        };
        const char *expected[] = {"3", "6", "9", "12"};
        const char *solution[] = {
            "void setup() {",
            "  Serial.begin(9600);",
            "  for (int i = 1; i <= 4; i++) {",
            "    Serial.println(i * 3);",
            "  }",
            "}",
            "",
            "void loop() { }",
        };
        challenge(task, 2, NULL, 0, expected, 4, solution, 8);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    Serial.println("   - if / else decides; == compares, = assigns");
    Serial.println("   - for(start; keep going?; step) and while(keep going?)");
    Serial.println("   - i < 4 gives four passes, 0 to 3");
    Serial.println("   - && || !  and  break");
    Serial.println();
    Serial.println("  Module 4: functions and arrays.");
    wait_enter();
}

/* Lesson 4's demonstration functions. */
int add_one_copy(int value)
{
    value = value + 1;
    return value;
}

/* The compiler warns about exactly this, and the lesson quotes the warning.
 * It is silenced here only so the course itself builds clean. */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsizeof-array-argument"
int array_size_inside(int values[])
{
    return (int) sizeof(values);
}
#pragma GCC diagnostic pop

int average_of(int values[], int count)
{
    int total = 0;
    for (int i = 0; i < count; i++) {
        total = total + values[i];
    }
    return total / count;
}

void lesson_04_functions()
{
    title("MODULE 4 - FUNCTIONS AND ARRAYS");

    heading("PART 1: functions, and what they receive");

    Serial.println("    int add_one_copy(int value) {");
    Serial.println("      value = value + 1;");
    Serial.println("      return value;");
    Serial.println("    }");
    Serial.println();

    int original = 5;
    int result = add_one_copy(original);
    Serial.println("    int original = 5;");
    Serial.println("    int result = add_one_copy(original);");
    Serial.print("  Running:  result = ");
    Serial.print(result);
    Serial.print(", original = ");
    Serial.println(original);
    Serial.println();
    Serial.println("  The function got a COPY of original. Changing the copy left");
    Serial.println("  the original alone. A function that returns nothing is");
    Serial.println("  declared void; setup() and loop() are both void functions.");

    wait_enter();
    clear_screen();
    heading("PART 2: arrays");

    int readings[4] = {512, 530, 498, 505};
    Serial.println("    int readings[4] = {512, 530, 498, 505};");
    Serial.println("    readings[0] is the first, readings[3] the last.");
    Serial.println();
    Serial.print("  Running:  readings[0] = ");
    Serial.print(readings[0]);
    Serial.print(", readings[3] = ");
    Serial.println(readings[3]);
    Serial.println();
    Serial.println("  An array of 4 has indexes 0, 1, 2, 3. There is no index 4,");
    Serial.println("  and nothing checks. Reading readings[4] returns whatever");
    Serial.println("  sits next to the array; WRITING there overwrites some other");
    Serial.println("  variable. A board has no operating system to stop you, so");
    Serial.println("  this is not run here. Loop to count, never past it.");
    Serial.println();
    Serial.println("  The number of elements is the size of the whole array");
    Serial.println("  divided by the size of one:");
    Serial.println();
    Serial.println("    sizeof(readings) / sizeof(readings[0])");
    Serial.print("  Running:  ");
    Serial.println(sizeof(readings) / sizeof(readings[0]));

    wait_enter();
    clear_screen();
    heading("PART 3: arrays and functions");

    Serial.println("    int average_of(int values[], int count) { ... }");
    Serial.println("    average_of(readings, 4)");
    Serial.print("  Running:  ");
    Serial.println(average_of(readings, 4));
    Serial.println();
    Serial.println("  (512 + 530 + 498 + 505) / 4 = 511.25, but an int keeps");
    Serial.println("  only 511: module 2's integer division, on real data.");
    Serial.println();
    Serial.println("  Why the function takes `count` as well: inside it, the");
    Serial.println("  array arrives as just an address, and its size is gone.");
    Serial.println("  sizeof(values) inside the function reports:");
    Serial.print("  Running:  ");
    Serial.println(array_size_inside(readings));
    Serial.println();
    Serial.println("  That is the size of an address -- 8 bytes on this PC, 2 on");
    Serial.println("  an Uno -- and not 4 numbers. The compiler says so itself:");
    Serial.println();
    Serial.println("    warning: 'sizeof' on array function parameter 'values'");
    Serial.println("    will return size of 'int*' [-Wsizeof-array-argument]");
    Serial.println();
    Serial.println("  Always pass the count.");

    wait_enter();
    clear_screen();
    exercise(4);

    question("int a[5];   What is the highest valid index?", "4",
             "Indexes run from 0 to 4; a[5] is one past the end.");
    question("Inside a function taking int values[], sizeof(values) is\n"
             "  the size of the array. true or false?",
             "false", "It is only the size of an address; pass the count too.");

    {
        const char *task[] = {
            "Write a function int largest(int values[], int count) that returns",
            "the biggest value. In setup(), call it on {4, 9, 2, 7} and print",
            "the result.",
        };
        const char *expected[] = {"9"};
        const char *solution[] = {
            "int largest(int values[], int count) {",
            "  int best = values[0];",
            "  for (int i = 1; i < count; i++) {",
            "    if (values[i] > best) { best = values[i]; }",
            "  }",
            "  return best;",
            "}",
            "",
            "void setup() {",
            "  Serial.begin(9600);",
            "  int data[4] = {4, 9, 2, 7};",
            "  Serial.println(largest(data, 4));",
            "}",
            "",
            "void loop() { }",
        };
        challenge(task, 3, NULL, 0, expected, 1, solution, 15);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    Serial.println("   - a function gets copies of its arguments");
    Serial.println("   - arrays count from 0; nothing stops an index past the end");
    Serial.println("   - sizeof(a) / sizeof(a[0]) counts elements, but only where");
    Serial.println("     the array is declared");
    Serial.println("   - pass the count along with the array");
    Serial.println();
    Serial.println("  Module 5: the first real hardware idea -- pins.");
    wait_enter();
}

/* One line per pin: its mode and the level it reads as. The mode is worked
 * out from the DDR register, which is where the chip keeps it. */
void show_pin(uint8_t pin)
{
    bool is_output;
    if (pin <= 7) {
        is_output = bitRead(DDRD, pin);
    } else {
        is_output = bitRead(DDRB, pin - 8);
    }
    Serial.print("      pin ");
    if (pin < 10) {
        Serial.print(' ');
    }
    Serial.print(pin);
    Serial.print("   ");
    Serial.print(is_output ? "OUTPUT" : "INPUT ");
    Serial.print("   reads ");
    Serial.println(digitalRead(pin) ? "HIGH" : "LOW");
}

void lesson_05_pins()
{
    title("MODULE 5 - DIGITAL PINS");

    heading("PART 1: pinMode and digitalWrite");

    Serial.println("  A pin is a wire the chip can drive to 5 V (HIGH) or 0 V");
    Serial.println("  (LOW), or read. First say which way it points:");
    Serial.println();
    Serial.println("    pinMode(13, OUTPUT);");
    Serial.println("    digitalWrite(13, HIGH);");
    Serial.println();

    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    Serial.println("  Running, then looking at the pin:");
    show_pin(13);
    Serial.println();
    Serial.println("  LED_BUILTIN is 13 on an Uno: the small LED on the board.");
    Serial.print("  Running:  LED_BUILTIN = ");
    Serial.print(LED_BUILTIN);
    Serial.print(", HIGH = ");
    Serial.print(HIGH);
    Serial.print(", LOW = ");
    Serial.println(LOW);
    Serial.println();
    Serial.println("  HIGH and LOW are just 1 and 0 with names.");

    wait_enter();
    clear_screen();
    heading("PART 2: time, and the board that cannot do two things");

    uint32_t before = millis();
    delay(1000);
    uint32_t after = millis();
    Serial.println("    delay(1000);");
    Serial.print("  Running:  millis() went from ");
    Serial.print(before);
    Serial.print(" to ");
    Serial.println(after);
    Serial.println();
    Serial.println("  Honest note: on this PC the clock is simulated, so that");
    Serial.println("  second cost nothing. On a board it takes a real second,");
    Serial.println("  and for that second the chip does NOTHING else: no button");
    Serial.println("  read, no sensor check. Module 8 shows how to wait without");
    Serial.println("  freezing.");

    wait_enter();
    clear_screen();
    heading("PART 3: reading a button, and why it reads backwards");

    Serial.println("  A button wired from the pin to ground, with the chip's");
    Serial.println("  built-in pull-up resistor switched on:");
    Serial.println();
    Serial.println("    pinMode(2, INPUT_PULLUP);");
    pinMode(2, INPUT_PULLUP);
    Serial.println("    digitalRead(2);                // nobody pressing");
    Serial.print("  Running:  ");
    Serial.println(digitalRead(2) ? "HIGH" : "LOW");
    sim_set_external(2, LOW);
    Serial.println("    (the button is pressed -- it connects the pin to ground)");
    Serial.print("  Running:  ");
    Serial.println(digitalRead(2) ? "HIGH" : "LOW");
    sim_set_external(2, HIGH);
    Serial.println();
    Serial.println("  With INPUT_PULLUP the logic is INVERTED: released is HIGH,");
    Serial.println("  PRESSED is LOW. It is what most wiring uses, because it");
    Serial.println("  needs no extra resistor, and the single most common source");
    Serial.println("  of 'my button does the opposite'.");
    Serial.println();
    Serial.println("  (sim_set_external is not Arduino. It plays the part of the");
    Serial.println("  finger; on a board the world does that itself.)");
    Serial.println();
    Serial.println("  One more trap, shown running. Writing HIGH to a pin set as");
    Serial.println("  INPUT does not drive it. It switches its pull-up ON:");
    Serial.println();
    Serial.println("    pinMode(4, INPUT);");
    pinMode(4, INPUT);
    Serial.print("    digitalRead(4)  ->  ");
    Serial.println(digitalRead(4) ? "HIGH" : "LOW");
    Serial.println("    digitalWrite(4, HIGH);");
    digitalWrite(4, HIGH);
    Serial.print("    digitalRead(4)  ->  ");
    Serial.println(digitalRead(4) ? "HIGH" : "LOW");
    Serial.println();
    Serial.println("  Forgetting pinMode(pin, OUTPUT) gives exactly this: a dim");
    Serial.println("  LED that never lights properly, with no error anywhere.");

    wait_enter();
    clear_screen();
    exercise(5);

    question("A button between a pin and GND, with INPUT_PULLUP.\n"
             "  What does digitalRead give while it is PRESSED?",
             "LOW", "The pull-up makes released HIGH; pressing grounds the pin.");
    question("While delay(500) runs, can the board read a button?\n"
             "  (yes or no)",
             "no", "delay() blocks everything; nothing else runs until it ends.");

    {
        const char *task[] = {
            "Set pin 8 as an OUTPUT. Three times: turn it HIGH, print ON and the",
            "pin's level on one line, wait 500 ms, turn it LOW, print OFF and the",
            "pin's level on one line, wait 500 ms.",
        };
        const char *expected[] = {"ON 1", "OFF 0", "ON 1", "OFF 0", "ON 1", "OFF 0"};
        const char *solution[] = {
            "void setup() {",
            "  Serial.begin(9600);",
            "  pinMode(8, OUTPUT);",
            "  for (int i = 0; i < 3; i++) {",
            "    digitalWrite(8, HIGH);",
            "    Serial.print(\"ON \");",
            "    Serial.println(digitalRead(8));",
            "    delay(500);",
            "    digitalWrite(8, LOW);",
            "    Serial.print(\"OFF \");",
            "    Serial.println(digitalRead(8));",
            "    delay(500);",
            "  }",
            "}",
            "",
            "void loop() { }",
        };
        challenge(task, 3, NULL, 0, expected, 6, solution, 16);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    Serial.println("   - pinMode first (OUTPUT, INPUT or INPUT_PULLUP), then use it");
    Serial.println("   - HIGH is 1 and LOW is 0; LED_BUILTIN is pin 13 on an Uno");
    Serial.println("   - INPUT_PULLUP inverts a button: pressed reads LOW");
    Serial.println("   - digitalWrite on an INPUT pin sets its pull-up");
    Serial.println("   - delay() freezes everything else");
    Serial.println();
    Serial.println("  Module 6: talking back -- reading Serial input.");
    wait_enter();
}

void lesson_06_serial_input()
{
    title("MODULE 6 - READING SERIAL INPUT");

    heading("PART 1: input arrives one character at a time");

    Serial.println("  Serial.available() says how many characters are waiting.");
    Serial.println("  Serial.read() takes one, or returns -1 if there is none.");
    Serial.println();
    Serial.print("  Type a short word and press ENTER: ");

    while (Serial.available() == 0) {
        /* wait for the first character */
    }
    Serial.println();
    while (Serial.available() > 0) {
        int c = Serial.read();
        Serial.print("      character ");
        Serial.print(c);
        if (c == '\n') {
            Serial.println("   <- ENTER itself");
        } else {
            Serial.print("   is '");
            Serial.print((char) c);
            Serial.println("'");
        }
    }
    Serial.println();
    Serial.println("  Every key is a number, and ENTER is one too: 10, the");
    Serial.println("  newline. A Serial Monitor can be set to append a newline,");
    Serial.println("  a carriage return (13), both, or nothing. A sketch that");
    Serial.println("  waits for a newline hangs when the monitor sends none.");

    wait_enter();
    clear_screen();
    heading("PART 2: reading a whole number");

    Serial.println("    int n = Serial.parseInt();");
    Serial.println();
    Serial.print("  Type a whole number and press ENTER: ");
    while (Serial.available() == 0) {
    }
    int typed = Serial.parseInt();
    drain_input();
    Serial.print("  parseInt returned ");
    Serial.println(typed);
    Serial.println();
    Serial.println("  parseInt skips anything that is not a digit, then reads");
    Serial.println("  digits. Now type something that is NOT a number, like abc:");
    Serial.println();
    Serial.print("  Type abc and press ENTER: ");
    while (Serial.available() == 0) {
    }
    typed = Serial.parseInt();
    drain_input();
    Serial.print("  parseInt returned ");
    Serial.println(typed);
    Serial.println();
    Serial.println("  The trap: finding no number does not fail, it returns 0.");
    Serial.println("  A typed 0 and a typed abc look identical to the sketch.");
    Serial.println("  (On a real board parseInt also gives up after a timeout,");
    Serial.println("  1 second by default, and returns 0 then too.)");

    wait_enter();
    clear_screen();
    heading("PART 3: one-letter commands");

    Serial.println("  The simplest robot protocol: one character, one action.");
    Serial.println();
    Serial.println("    char command = Serial.read();");
    Serial.println("    if (command == 'f')      { forward }");
    Serial.println("    else if (command == 's') { stop }");
    Serial.println("    else                     { unknown }");
    Serial.println();
    Serial.print("  Type f, s or anything else, then ENTER: ");
    while (Serial.available() == 0) {
    }
    char command = (char) Serial.read();
    drain_input();
    Serial.print("  Running:  ");
    if (command == 'f') {
        Serial.println("forward");
    } else if (command == 's') {
        Serial.println("stop");
    } else {
        Serial.println("unknown command");
    }
    Serial.println();
    Serial.println("  Always have the else. A byte of line noise will arrive");
    Serial.println("  someday, and a robot should not act on it.");

    wait_enter();
    clear_screen();
    exercise(6);

    question("Which character code does the ENTER key send on this\n"
             "  terminal? (a number)",
             "10", "It sends a newline, which is code 10.");
    question("Serial.parseInt() reads the text abc. What does it return?",
             "0", "No digits found returns 0, the same as a typed 0.");

    {
        const char *task[] = {
            "Read one whole number from Serial and print it doubled.",
        };
        const char *input[] = {"21"};
        const char *expected[] = {"42"};
        const char *solution[] = {
            "void setup() {",
            "  Serial.begin(9600);",
            "  while (Serial.available() == 0) { }",
            "  int n = Serial.parseInt();",
            "  Serial.println(n * 2);",
            "}",
            "",
            "void loop() { }",
        };
        challenge(task, 1, input, 1, expected, 1, solution, 8);
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    Serial.println("   - Serial.available() then Serial.read(): one character, -1");
    Serial.println("     when there is none");
    Serial.println("   - ENTER is a character (10); a monitor may add 13 as well");
    Serial.println("   - parseInt() returns 0 for 'no number', same as a real 0");
    Serial.println("   - always handle the command you did not expect");
    Serial.println();
    Serial.println("  Module 7: analog values, and turning one range into another.");
    wait_enter();
}
