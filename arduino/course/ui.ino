/*
 * ui.ino - the pieces every lesson uses to draw the screen.
 *
 * Same purpose and the same visual style as the other courses' ui files,
 * kept as a separate implementation rather than shared code. Everything here
 * goes through Serial: on a real board that is a cable to a monitor, on a PC
 * build it is your terminal. A lesson cannot tell the difference.
 */

const int WIDTH = 54; /* inside width of the frame, matching the other courses */

void clear_screen()
{
    /* ANSI codes, not a spawned process, for the same reason as every course:
     * no dependency on TERM being set. */
    Serial.print("\033[H\033[2J\033[3J");
}

void rule()
{
    Serial.print("  ");
    for (int i = 0; i < WIDTH; i++) {
        Serial.print('-');
    }
    Serial.println();
}

/* Read one line from the Serial input, without the line ending. */
String read_line()
{
    String line = Serial.readStringUntil('\n');
    if (line.length() > 0 && line[line.length() - 1] == '\r') {
        line = line.substring(0, line.length() - 1);
    }
    return line;
}

/* Throw away whatever is still waiting on Serial: the newline left behind
 * by parseInt() or read(), for instance. */
void drain_input()
{
    while (Serial.available() > 0) {
        Serial.read();
    }
}

void wait_enter()
{
    Serial.print("\n  Press ENTER to continue...");
    read_line();
}

void frame(char fill)
{
    Serial.print("  +");
    for (int i = 0; i < WIDTH - 2; i++) {
        Serial.print(fill);
    }
    Serial.println('+');
}

void padded_line(const char *text, char border)
{
    Serial.print("  ");
    Serial.print(border);
    Serial.print(' ');
    Serial.print(text);
    for (int i = (int) strlen(text); i < WIDTH - 3; i++) {
        Serial.print(' ');
    }
    Serial.println(border);
}

void title(const char *text)
{
    Serial.println();
    frame('=');
    padded_line(text, '|');
    frame('=');
    Serial.println();
}

void heading(const char *text)
{
    Serial.print("\n  ");
    Serial.println(text);
    rule();
}

bool ask_yes(const char *question_text)
{
    Serial.print("\n  ");
    Serial.print(question_text);
    Serial.print(" (y/N): ");
    String answer = read_line();
    return answer.length() > 0 && (answer[0] == 'y' || answer[0] == 'Y');
}

void exercise(int number)
{
    Serial.print("\n  >> EXERCISE - MODULE ");
    Serial.println(number);
    rule();
}

/* Compare answers ignoring case and surrounding spaces, so "  Two " and
 * "two" count as the same. */
bool question(const char *text, const char *correct, const char *why)
{
    Serial.print("\n  ");
    Serial.println(text);
    Serial.print("  Your answer: ");

    String answer = read_line();
    answer.trim();
    answer.toLowerCase();
    String expected = correct;
    expected.trim();
    expected.toLowerCase();

    bool right = answer == expected;
    if (right) {
        Serial.print("\n  CORRECT.  ");
        Serial.println(why);
    } else {
        Serial.print("\n  NOT QUITE. The answer is: ");
        Serial.println(correct);
        Serial.print("             ");
        Serial.println(why);
    }
    return right;
}

/*
 * A task to write in a real file.
 *
 * `input` is every line the sketch will read from Serial while producing
 * `expected` -- none for a task that reads nothing. Together they are the
 * actual specification: run the solution, type `input`, get `expected`, no
 * matter how the code that does it is written. `solution` is one way of
 * getting there and appears only after a confirmation.
 *
 * Rule for whoever writes a challenge: the solution may only use what the
 * course has already taught by this module.
 */
void challenge(const char *task[], int task_lines,
               const char *input[], int input_lines,
               const char *expected[], int expected_lines,
               const char *solution[], int solution_lines)
{
    Serial.println("\n  >> WRITE THIS YOURSELF, in a real file");
    rule();

    for (int i = 0; i < task_lines; i++) {
        Serial.print("  ");
        Serial.println(task[i]);
    }

    if (input_lines > 0) {
        Serial.println("\n  Try it with this input:\n");
        for (int i = 0; i < input_lines; i++) {
            Serial.print("      ");
            Serial.println(input[i]);
        }
    }

    if (expected_lines > 0) {
        Serial.println("\n  It must print:\n");
        for (int i = 0; i < expected_lines; i++) {
            Serial.print("      ");
            Serial.println(expected[i]);
        }
        Serial.println("\n  That output is the whole specification. Any code that");
        Serial.println("  produces it is correct.");
    }

    Serial.println("\n  Try it first. Save it as test.ino and, from the arduino/");
    Serial.println("  folder, run:");
    Serial.println("    make try FILE=test.ino");

    if (!ask_yes("Want to see example code?")) {
        return;
    }

    Serial.println();
    rule();
    for (int i = 0; i < solution_lines; i++) {
        Serial.print("  ");
        Serial.println(solution[i]);
    }
    rule();
    Serial.println("  This is EXAMPLE CODE, not the answer. It is one way to get");
    Serial.println("  that output; yours may look nothing like it and still be");
    Serial.println("  right -- or better. Compare the output, not the code.");
}
