/*
 * Arduino course - main menu.
 *
 * Written as a sketch on purpose, the same way the other courses are written
 * in the language they teach: this is setup() and loop() like any sketch, and
 * the module table below uses a struct, an array and function pointers --
 * things the later modules explain.
 *
 * Build:  make
 * Run:    ./arduino-course
 */

/* One menu entry: what is shown, the function that runs, and which of the
 * three menu bands ("BASIC", "INTERMEDIATE", "ADVANCED") it is filed under.
 * Tiers are a display grouping only -- the module functions don't know it
 * exists, and adding one to a module is this one field, nothing else. */
struct Module {
    const char *title;
    void (*run)();
    const char *tier;
};

const Module MODULES[] = {
    {"Your first sketch",                    lesson_01_sketch,         "BASIC"},
    {"Variables and types",                  lesson_02_types,          "BASIC"},
    {"Decisions and loops",                  lesson_03_decisions,      "BASIC"},
    {"Functions and arrays",                 lesson_04_functions,      "BASIC"},
    {"Digital pins",                         lesson_05_pins,           "BASIC"},
    {"Reading Serial input",                 lesson_06_serial_input,   "BASIC"},
    {"Analog values and map()",              lesson_07_analog,         "INTERMEDIATE"},
    {"Time without freezing: millis()",      lesson_08_time,           "INTERMEDIATE"},
    {"Text: char arrays and String",         lesson_09_text,           "INTERMEDIATE"},
    {"Memory, bits and ports",               lesson_10_memory,         "ADVANCED"},
    {"Interrupts and volatile",              lesson_11_interrupts,     "ADVANCED"},
    {"Inline assembly on the AVR",           lesson_12_assembly,       "ADVANCED"},
    {"Final project: an obstacle robot",     lesson_13_robot,          "ADVANCED"},
};

/* Number of elements: the size of the whole array divided by the size of one
 * element. Module 4 explains why this works here and not inside a function. */
#define MODULE_COUNT (int)(sizeof MODULES / sizeof MODULES[0])

void show_menu()
{
    clear_screen();
    title("ARDUINO COURSE - FROM setup() TO REGISTERS");

    const char *shown_tier = "";
    for (int i = 0; i < MODULE_COUNT; i++) {
        if (strcmp(MODULES[i].tier, shown_tier) != 0) {
            shown_tier = MODULES[i].tier;
            Serial.print("\n  -- ");
            Serial.print(shown_tier);
            Serial.println(" --");
        }
        Serial.print("   [");
        if (i + 1 < 10) {
            Serial.print(' ');
        }
        Serial.print(i + 1);
        Serial.print("]  ");
        Serial.println(MODULES[i].title);
    }

    Serial.println("\n   [ 0]  Quit");
    rule();
}

void setup()
{
    Serial.begin(9600);
}

/* loop() draws the menu once per call and runs one module; the board's
 * startup code calls it again for as long as the program lives. */
void loop()
{
    show_menu();
    Serial.print("\n  Pick a module: ");

    String choice = read_line();
    choice.trim();

    if (choice == "0") {
        Serial.println("\n  See you next time.\n");
        sim_quit();
    }

    int n = (int) choice.toInt();
    if (n >= 1 && n <= MODULE_COUNT) {
        clear_screen();
        MODULES[n - 1].run();
    } else {
        Serial.println("\n  Not a valid option.");
        wait_enter();
    }
}
