/*
 * Modules 5-7 - raylib and raygui: your first window, buttons and state,
 * and a todo list.
 *
 * Same rule as the GTK modules: the lesson program itself is plain C. The
 * code it teaches needs raylib and raygui, which the student builds
 * (module 5 gives the exact steps).
 *
 * Tested with raylib 6.5-dev (commit 0dedaf3066) and raygui 5.1-dev (commit
 * 2a3c4790c6), gcc 14.2.1, Linux with X11. The examples compile with
 * -std=c11 -Wall -Wextra and no warnings from the examples themselves;
 * raygui.h prints 11 warnings under -Wextra, which -isystem hides. Each one
 * was started and ran until it was stopped. The todo list's data functions
 * were driven by a test program that included the file without opening a
 * window. Clicking and typing in the raygui windows were not driven by a
 * test: the click and text-box behaviour described is raygui's documented
 * behaviour.
 *
 * Measured and quoted:
 *   frames in one second: 12134 with no SetTargetFPS, 60 with it
 *   LoadTexture of a missing file: WARNING FILEIO, texture id 0
 *   TextFormat: first result intact after 3 more calls, overwritten by the 4th
 *   strncpy into char[8]: last byte 0x78, not terminated
 *   delete inside the draw loop over a,b,d,c: visited "a b d"
 *   GuiCheckBox with a bool, GuiSetStyle with a Color: compile errors
 *
 * Explained and not run, because they are undefined behaviour: a text box
 * told its buffer is larger than it is, and printing an unterminated
 * string.
 */

#include <stdio.h>

#include "lessons.h"
#include "strings_pt.h"
#include "ui.h"

static const char *const EXR1_P0[] = {
    "// window.c : the smallest raylib program: a window that says hello.",
    "",
    "#include \"raylib.h\"   // everything raylib offers",
    "",
    "int main(void)",
    "{",
    "    // Open a 640 by 360 window with a title in its title bar.",
    "    // This must come before anything that draws or loads.",
    "    InitWindow(640, 360, \"My first raylib window\");",
    "",
    "    // Limit the loop below to 60 frames per second.",
    "    SetTargetFPS(60);",
    "",
    "    // The main loop. One pass = one frame drawn on screen.",
    "    // WindowShouldClose() turns true when the user closes the window.",
    "    while (!WindowShouldClose()) {",
    "        BeginDrawing();             // start a frame: drawing goes here",
    "        ClearBackground(RAYWHITE);  // wipe the last frame to one colour",
    "",
    "        // Text, then x and y in pixels (0,0 is the top left corner),",
    "        // then the font size and the colour.",
    "        DrawText(\"Hello, raylib!\", 190, 160, 30, DARKGRAY);",
    "",
    "        EndDrawing();               // finish the frame: show it",
    "    }",
    "",
    "    CloseWindow();   // free the window and everything raylib loaded",
    "    return 0;",
    "}",
};
static const char *const GOOD_FPS[] = {
    "SetTargetFPS(60);",
};
static const char *const BAD_FPS[] = {
    "// no SetTargetFPS: the loop runs as fast as the machine allows",
};
static const char *const BAD_TEXTURE[] = {
    "Texture2D t = LoadTexture(\"does_not_exist.png\");",
};
static const char *const EXR2_P0[] = {
    "// widgets.c : raygui widgets. Our variables hold all the state.",
    "",
    "#include \"raylib.h\"",
    "#define RAYGUI_IMPLEMENTATION   // raygui is one header: this line makes",
    "#include \"raygui.h\"             // exactly one .c file contain its code",
    "",
    "int main(void)",
    "{",
    "    InitWindow(420, 280, \"raygui widgets\");",
    "    SetTargetFPS(60);",
    "",
    "    // The state of the form lives HERE, in plain variables. raygui",
    "    // does not store it for us: we pass it in on every frame.",
    "    char name[32] = \"\";    // text typed in the text box",
    "    bool editing = false;  // is the text box taking keystrokes now?",
    "    bool shout = false;    // is the \"Shout\" box ticked?",
    "    int greets = 0;        // how many times Greet was pressed",
};
static const char *const EXR2_P1[] = {
    "    while (!WindowShouldClose()) {",
    "        BeginDrawing();",
    "        ClearBackground(RAYWHITE);",
    "",
    "        // A Rectangle is { x, y, width, height } in pixels.",
    "        GuiLabel((Rectangle){20, 20, 380, 24}, \"Your name:\");",
    "",
    "        // The text box returns nonzero when it is clicked. We flip",
    "        // `editing` ourselves: this is how it starts and stops typing.",
    "        if (GuiTextBox((Rectangle){20, 48, 380, 32}, name,",
    "                       sizeof name, editing))",
    "            editing = !editing;",
    "",
    "        // The check box writes the new value straight into `shout`.",
    "        GuiCheckBox((Rectangle){20, 96, 24, 24}, \"Shout\", &shout);",
    "",
    "        // A button returns nonzero on the frame it is clicked.",
    "        if (GuiButton((Rectangle){20, 136, 160, 36}, \"Greet\"))",
    "            greets++;",
};
static const char *const EXR2_P2[] = {
    "        // Not a widget: ordinary raylib drawing, below the form.",
    "        if (greets > 0) {",
    "            const char *message = TextFormat(\"Hello, %s.\", name);",
    "            if (shout)",
    "                message = TextToUpper(message);",
    "            DrawText(message, 20, 196, 24, DARKGRAY);",
    "            DrawText(TextFormat(\"Greeted %d time(s)\", greets),",
    "                     20, 232, 18, GRAY);",
    "        }",
    "",
    "        EndDrawing();",
    "    }",
    "",
    "    CloseWindow();",
    "    return 0;",
    "}",
};
static const char *const GOOD_CHECK[] = {
    "GuiCheckBox((Rectangle){20, 96, 24, 24}, \"Shout\", &shout);",
};
static const char *const BAD_CHECK[] = {
    "GuiCheckBox((Rectangle){20, 96, 24, 24}, \"Shout\", shout);   // no &",
};
static const char *const GOOD_TEXTBOX[] = {
    "char name[32] = \"\";",
    "if (GuiTextBox(bounds, name, sizeof name, editing))",
    "    editing = !editing;",
};
static const char *const BAD_TEXTBOX[] = {
    "char name[8] = \"\";",
    "GuiTextBox(bounds, name, 32, editing);   // says 32, buffer is 8",
};
static const char *const BAD_TEXTFORMAT[] = {
    "const char *first = TextFormat(\"first\");",
    "// ... four more TextFormat calls later:",
    "DrawText(first, 20, 20, 20, DARKGRAY);   // no longer \"first\"",
};
static const char *const GOOD_STYLE[] = {
    "GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(DARKBLUE));",
    "GuiSetStyle(DEFAULT, TEXT_SIZE, 20);",
};
static const char *const BAD_STYLE[] = {
    "GuiSetStyle(DEFAULT, BACKGROUND_COLOR, DARKBLUE);   // a Color, not an int",
};
static const char *const EXR3_P0[] = {
    "// todo.c : the todo list from module 4, built with raylib and raygui.",
    "",
    "#include <string.h>",
    "#include \"raylib.h\"",
    "#define RAYGUI_IMPLEMENTATION",
    "#include \"raygui.h\"",
    "",
    "#define MAX_TASKS 12   // as many rows as fit in the window",
    "#define TASK_LEN 48    // longest task, including the final '\\0'",
    "",
    "// ---- the data: plain C, no raylib in this part ---------------------",
    "",
    "static char text[MAX_TASKS][TASK_LEN];   // the task texts",
    "static bool done[MAX_TASKS];             // ticked or not",
    "static int count;                        // how many tasks are in use",
    "",
    "// Add a task. Returns 1 if added, 0 if empty or the list is full.",
    "static int add_task(const char *label)",
    "{",
    "    if (label[0] == '\\0' || count == MAX_TASKS)",
    "        return 0;",
    "    strncpy(text[count], label, TASK_LEN - 1);",
    "    text[count][TASK_LEN - 1] = '\\0';   // strncpy may not end it",
    "    done[count] = false;",
    "    count++;",
    "    return 1;",
    "}",
    "",
    "// Remove task i by moving every later task one place down.",
    "static void delete_task(int i)",
    "{",
    "    for (int k = i; k < count - 1; k++) {",
    "        strcpy(text[k], text[k + 1]);",
    "        done[k] = done[k + 1];",
    "    }",
    "    count--;",
    "}",
    "",
    "// Remove every ticked task, keeping the order of the rest.",
    "static void clear_done(void)",
    "{",
    "    int kept = 0;",
    "    for (int i = 0; i < count; i++) {",
    "        if (!done[i]) {",
    "            strcpy(text[kept], text[i]);",
    "            done[kept] = false;",
    "            kept++;",
    "        }",
    "    }",
    "    count = kept;",
    "}",
    "",
    "// Count the tasks that are not ticked. Asked for, never stored.",
    "static int tasks_left(void)",
    "{",
    "    int left = 0;",
    "    for (int i = 0; i < count; i++)",
    "        left += !done[i];",
    "    return left;",
    "}",
};
static const char *const EXR3_P1[] = {
    "// ---- the window ----------------------------------------------------",
    "",
    "int main(void)",
    "{",
    "    InitWindow(480, 480, \"raygui todo\");",
    "    SetTargetFPS(60);",
    "",
    "    char input[TASK_LEN] = \"\";   // text being typed",
    "    bool editing = false;        // is the text box taking keystrokes?",
    "",
    "    while (!WindowShouldClose()) {",
    "        BeginDrawing();",
    "        ClearBackground(RAYWHITE);",
    "",
    "        // Top row: text box, Add, Clear done.",
    "        if (GuiTextBox((Rectangle){10, 10, 300, 32}, input,",
    "                       sizeof input, editing))",
    "            editing = !editing;",
    "        // Enter adds too, but only while the text box is being edited.",
    "        if (editing && IsKeyPressed(KEY_ENTER))",
    "            add_task(input), input[0] = '\\0';",
    "        if (GuiButton((Rectangle){318, 10, 60, 32}, \"Add\")) {",
    "            add_task(input);",
    "            input[0] = '\\0';",
    "        }",
    "        if (GuiButton((Rectangle){386, 10, 84, 32}, \"Clear done\"))",
    "            clear_done();",
};
static const char *const EXR3_P2[] = {
    "        // One row per task. We may not delete while looping over the",
    "        // rows, so we only note which one was asked for.",
    "        int to_delete = -1;",
    "        for (int i = 0; i < count; i++) {",
    "            GuiCheckBox((Rectangle){14, 62 + i * 30, 20, 20},",
    "                        text[i], &done[i]);",
    "            if (GuiButton((Rectangle){430, 58 + i * 30, 40, 26}, \"x\"))",
    "                to_delete = i;",
    "        }",
    "        if (to_delete >= 0)    // now the loop is over: safe to delete",
    "            delete_task(to_delete);",
    "",
    "        DrawText(TextFormat(\"%d left\", tasks_left()), 10, 448, 20,",
    "                 DARKGRAY);",
    "",
    "        EndDrawing();",
    "    }",
    "",
    "    CloseWindow();",
    "    return 0;",
    "}",
};
static const char *const BAD_DELETE_LOOP[] = {
    "for (int i = 0; i < count; i++) {",
    "    draw_row(i);",
    "    if (x_was_clicked(i))",
    "        delete_task(i);   // rows shift down underneath the loop",
    "}",
};
static const char *const GOOD_DELETE_LATER[] = {
    "int to_delete = -1;",
    "for (int i = 0; i < count; i++)",
    "    if (GuiButton(x_button, \"x\"))",
    "        to_delete = i;       // only note it",
    "if (to_delete >= 0)",
    "    delete_task(to_delete);  // the loop is over: safe",
};
static const char *const BAD_STRNCPY[] = {
    "strncpy(text[count], label, TASK_LEN);   // may leave no '\\0'",
};
static const char *const GOOD_STRNCPY[] = {
    "strncpy(text[count], label, TASK_LEN - 1);",
    "text[count][TASK_LEN - 1] = '\\0';   // always end it ourselves",
};

void lesson_05_raylib(void)
{
    title("MODULE 5 - RAYLIB: YOUR FIRST WINDOW AND THE LOOP");

    heading("WHAT YOU WILL HAVE AT THE END");

    printf("  A window you drew yourself, 60 times a second. Then, in\n");
    printf("  modules 6 and 7, buttons and a todo list on top of it.\n\n");

    printf("  raylib is a small C library for windows, drawing, input\n");
    printf("  and sound. It is made for games, but it works for any\n");
    printf("  program that draws. raygui is one extra header that adds\n");
    printf("  buttons, text boxes and check boxes on top of it.\n\n");

    printf("  GTK (modules 1 to 4) and raylib solve the same problem in\n");
    printf("  opposite ways, and this module starts with that difference.\n");

    heading("THE IDEA: REDRAW EVERYTHING, EVERY FRAME");

    printf("  GTK is retained: you create widget objects once, GTK keeps\n");
    printf("  them, and calls your callbacks when something happens.\n\n");

    printf("  raylib with raygui is immediate: there are no widget objects.\n");
    printf("  Your program is a loop, and every pass through the loop\n");
    printf("  draws the whole screen again from scratch. A button is a\n");
    printf("  function call that draws the button AND tells you whether it\n");
    printf("  was clicked, in the same call. Whatever should be remembered\n");
    printf("  between frames lives in your own variables, because nothing\n");
    printf("  else remembers it.\n\n");

    printf("  Read back from the example below: with no frame limit the loop\n");
    printf("  ran 12134 times in one second on this machine. With\n");
    printf("  SetTargetFPS(60) it ran exactly 60 times.\n");

    wait_enter();
    clear_screen();
    heading("STEP 0: INSTALL raylib AND raygui");

    printf("  raylib is not in every distribution's repository (it was not\n");
    printf("  in Void's), so this builds it from source. You need gcc,\n");
    printf("  cmake, git, and the X11 and OpenGL development packages.\n");
    printf("  On Void that is:\n\n");

    printf("    sudo xbps-install cmake libXrandr-devel libXinerama-devel \\\n");
    printf("                      libXcursor-devel libXi-devel\n\n");

    printf("  Other distributions: raylib's wiki page \"Working on GNU\n");
    printf("  Linux\" lists the packages. Then:\n\n");

    printf("    git clone https://github.com/raysan5/raylib.git\n");
    printf("    cd raylib && git checkout 0dedaf3066\n");
    printf("    cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \\\n");
    printf("          -DBUILD_EXAMPLES=OFF -DBUILD_SHARED_LIBS=OFF\n");
    printf("    cmake --build build -j\n");
    printf("    cd ..\n");
    printf("    git clone https://github.com/raysan5/raygui.git\n");
    printf("    cd raygui && git checkout 2a3c4790c6 && cd ..\n\n");

    printf("  The two commits are the ones this module was tested with\n");
    printf("  (raylib 6.5-dev, raygui 5.1-dev). Both projects change their\n");
    printf("  functions between versions, so pinning them keeps the\n");
    printf("  examples working.\n\n");

    printf("  Check it worked: these two files must exist.\n\n");
    printf("    raylib/build/raylib/libraylib.a\n");
    printf("    raygui/src/raygui.h\n");

    wait_enter();
    clear_screen();
    heading("STEP 1: THE SMALLEST RAYLIB PROGRAM");

    printf("  Save this as window.c:\n\n");
    code(LINES(EXR1_P0));

    wait_enter();
    clear_screen();
    heading("STEP 1: COMPILE IT AND RUN IT");

    printf("  Set two variables to where you built things, then compile:\n\n");
    printf("    RL=$HOME/raylib\n");
    printf("    RG=$HOME/raygui\n");
    printf("    gcc -Wall -Wextra -I$RL/src -isystem $RG/src window.c -o window \\\n");
    printf("        $RL/build/raylib/libraylib.a -lm -lpthread -ldl -lrt -lGL -lX11\n");
    printf("    ./window\n\n");

    printf("  What you should see: a 640 by 360 window with the grey text\n");
    printf("  \"Hello, raylib!\". raylib also prints INFO lines in the\n");
    printf("  terminal about what it initialised. Close the window to\n");
    printf("  stop. By default the Escape key closes it too (raylib's\n");
    printf("  documentation).\n\n");

    printf("  The compile line is longer than GTK's because raylib is a\n");
    printf("  static library: -I says where the headers are, the .a file\n");
    printf("  is raylib itself, and the -l flags are what raylib needs\n");
    printf("  from the system. These were tested on Linux with X11. On\n");
    printf("  Windows and macOS the system libraries differ; see the\n");
    printf("  raylib wiki.\n");

    wait_enter();
    clear_screen();
    heading("REFERENCE 1/2: OPEN, LOOP, CLOSE");

    ref_head("void InitWindow(width, height, title)");
    ref_line("Does:", "opens the window. Call it before anything that draws or");
    ref_line("", "loads files.");

    ref_head("void SetTargetFPS(fps)");
    ref_line("Does:", "caps the loop at that many frames per second.");
    printf("\n");
    code_good(LINES(GOOD_FPS));
    code_bad(LINES(BAD_FPS));
    bad("Measured: 12134 frames in one second without it, 60 with it. An");
    more("unlimited loop keeps a CPU core busy drawing the same picture");
    more("thousands of times a second. No error, no warning.");

    ref_head("bool WindowShouldClose()");
    ref_line("Does:", "false while the window is open, true once the user");
    ref_line("", "closes it. It is the loop's condition.");

    ref_head("void CloseWindow()");
    ref_line("Does:", "closes the window and frees what raylib created.");
    ref_line("", "(Read back: IsWindowReady() was 1 before and 0 after.)");

    wait_enter();
    clear_screen();
    heading("REFERENCE 2/2: DRAWING A FRAME");

    ref_head("void BeginDrawing()  and  void EndDrawing()");
    ref_line("Does:", "bracket one frame. Draw between them. EndDrawing is");
    ref_line("", "when the frame is shown.");

    ref_head("void ClearBackground(color)");
    ref_line("Does:", "fills the whole frame with one colour, erasing the");
    ref_line("", "last frame. Colours such as RAYWHITE, DARKGRAY, RED are");
    ref_line("", "predefined.");

    ref_head("void DrawText(text, x, y, fontSize, color)");
    ref_line("Does:", "draws text with its top-left corner at x, y. The");
    ref_line("", "origin (0, 0) is the top-left of the window; y grows");
    ref_line("", "DOWNWARD, unlike a graph.");

    ref_head("Texture2D LoadTexture(path)");
    ref_line("Does:", "loads an image file onto the graphics card.");
    printf("\n");
    code_bad(LINES(BAD_TEXTURE));
    bad("The program does not stop. raylib logs");
    more("WARNING: FILEIO: [does_not_exist.png] Failed to open file");
    more("and returns an empty texture: id 0, width 0. Drawing it shows");
    more("nothing. Always read the terminal. The path is relative to where");
    more("you RUN the program, not where the source file is.");

    wait_enter();
    clear_screen();
    heading("GOOD PRACTICE");

    tip("Call SetTargetFPS in every program unless you are measuring.");
    tip("Keep what changes between frames in variables declared BEFORE the");
    more("loop. A variable declared inside the loop starts over every frame.");
    tip("Do the updating (move things, read input) first, then draw, so");
    more("the frame shows the new state.");
    tip("Read raylib's WARNING lines. Missing files and bad arguments are");
    more("reported there, and the program keeps going.");
    tip("Pass raygui's directory with -isystem, not -I. Then warnings");
    more("inside raygui.h are hidden and yours stay visible: in this");
    more("course raygui.h printed 11 warnings under -Wextra, and the");
    more("examples themselves none.");

    wait_enter();
    clear_screen();
    exercise(5);

    question("With no SetTargetFPS the example looped 12134 times in one\n"
             "  second. With SetTargetFPS(60), how many times? (a number)",
             "60",
             "SetTargetFPS caps the loop. Measured: exactly 60 frames in\n"
             "             one second, against 12134 with no cap.");

    question("Drawing calls go between BeginDrawing() and which other\n"
             "  call? (the function name)",
             "EndDrawing",
             "BeginDrawing starts the frame, EndDrawing shows it.");

    question("LoadTexture(\"does_not_exist.png\") fails. What id does the\n"
             "  returned texture have? (a number)",
             "0",
             "raylib prints a WARNING and returns an empty texture (id 0,\n"
             "             width 0). The program does not stop.");

    printf("\n  >> BUILD THIS YOURSELF, in a real file\n");
    rule();
    printf("  Make the text move. Keep its x position in a variable\n");
    printf("  declared before the loop, add 2 to it every frame, and\n");
    printf("  when it passes the right edge (640) start it again off the\n");
    printf("  left edge. Pass the variable to DrawText instead of 190.\n\n");

    printf("  There is no transcript to match, the result is a window. You\n");
    printf("  have it right when the text slides to the right, leaves, and\n");
    printf("  comes back from the left.\n\n");

    printf("  Compile with the line from step 1.\n");

    if (ask_yes("Want to see example code?")) {
        printf("\n");
        rule();
        printf("  int x = 0;   // before the loop: kept between frames\n");
        printf("  ...\n");
        printf("      x += 2;\n");
        printf("      if (x > 640)\n");
        printf("          x = -220;      // start again off the left edge\n");
        printf("  ...\n");
        printf("      DrawText(\"Hello, raylib!\", x, 160, 30, DARKGRAY);\n");
        rule();
        printf("  The update goes before BeginDrawing. This version was\n");
        printf("  compiled with no warnings and ran.\n");
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(5)) {
        printf("   - raylib redraws the whole frame in a loop; nothing is\n");
        printf("     kept for you between frames\n");
        printf("   - SetTargetFPS caps the loop: 60 frames a second against\n");
        printf("     12134 measured without it\n");
        printf("   - draw between BeginDrawing and EndDrawing; y grows\n");
        printf("     downward\n");
        printf("   - a missing file is a logged WARNING, not a crash\n");
    }
    wait_enter();
}

void lesson_06_raygui(void)
{
    title("MODULE 6 - RAYGUI: BUTTONS, TEXT AND STATE");

    heading("WHAT YOU WILL HAVE AT THE END");

    printf("  A form drawn by raygui: a label, a text box, a check box and\n");
    printf("  a Greet button. Press Greet and a greeting is drawn below.\n");

    heading("THE IDEA: A WIDGET IS A FUNCTION CALL THAT ANSWERS");

    printf("  In GTK you create a button once and connect a callback. In\n");
    printf("  raygui you call GuiButton every frame. It draws the button,\n");
    printf("  and returns nonzero on the frame the user clicked it:\n\n");

    printf("    if (GuiButton(bounds, \"Greet\"))\n");
    printf("        greets++;\n\n");

    printf("  No objects, no signals, no callbacks. The answer arrives in\n");
    printf("  the same place you asked the question. The cost is that\n");
    printf("  state is yours to keep. raygui does not remember what is\n");
    printf("  ticked or typed: you pass your variable in every frame, and\n");
    printf("  it writes the new value back.\n");

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 1 OF 3: THE STATE");

    printf("  Save this as widgets.c. All three parts are one file.\n\n");
    code(LINES(EXR2_P0));

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 2 OF 3: THE WIDGETS");

    code(LINES(EXR2_P1));

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 3 OF 3: DRAW THE ANSWER");

    code(LINES(EXR2_P2));

    printf("\n  Compile with the line from module 5, changing window.c to\n");
    printf("  widgets.c. You should see the form; type a name (click the\n");
    printf("  text box first), tick Shout, press Greet.\n");

    wait_enter();
    clear_screen();
    heading("REFERENCE 1/3: THE WIDGETS");

    printf("  A Rectangle is { x, y, width, height } in pixels, written\n");
    printf("  (Rectangle){20, 48, 380, 32}.\n");

    ref_head("int GuiButton(bounds, text)");
    ref_line("Does:", "draws a button and returns nonzero on the frame it");
    ref_line("", "was clicked, zero otherwise.");

    ref_head("int GuiLabel(bounds, text)");
    ref_line("Does:", "draws text inside a rectangle.");

    ref_head("int GuiCheckBox(bounds, text, bool *checked)");
    ref_line("Does:", "draws a tick box, and writes the new value into the");
    ref_line("", "variable whose address you pass.");
    printf("\n");
    code_good(LINES(GOOD_CHECK));
    code_bad(LINES(BAD_CHECK));
    bad("error: incompatible type for argument 3 of 'GuiCheckBox'");
    more("(gcc 14.2.1.) It needs the address, so it can write the answer");
    more("back into your variable.");

    wait_enter();
    clear_screen();
    heading("REFERENCE 2/3: THE TEXT BOX");

    ref_head("int GuiTextBox(bounds, char *text, int textSize, bool editMode)");
    ref_line("Does:", "draws a text box showing the string in text. While");
    ref_line("", "editMode is true it takes keystrokes and edits the");
    ref_line("", "string in place.");
    ref_line("Returns:", "nonzero when the box is clicked. YOU flip editMode.");
    ref_line("Takes:", "textSize  the size of YOUR buffer, in bytes.");
    printf("\n");
    code_good(LINES(GOOD_TEXTBOX));
    code_bad(LINES(BAD_TEXTBOX));
    bad("raygui believes textSize, so it can write past the end of name:");
    more("a buffer overflow, undefined behaviour. Not run. Pass sizeof of");
    more("the array, so the number can never disagree with the buffer.");

    ref_head("const char *TextFormat(format, ...)");
    ref_line("Does:", "printf into a temporary string, and returns it.");
    ref_line("Warning:", "the result lives in one of a few reused buffers.");
    printf("\n");
    code_bad(LINES(BAD_TEXTFORMAT));
    bad("Measured: after three more calls the pointer still held \"first\";");
    more("after the fourth it held \"later 3\". Use the result straight away,");
    more("in the same statement, or copy it into your own buffer.");

    wait_enter();
    clear_screen();
    heading("REFERENCE 3/3: MAKING IT LOOK DIFFERENT");

    ref_head("void GuiSetStyle(control, property, int value)");
    ref_line("Does:", "changes how raygui draws. DEFAULT is every control.");
    ref_line("Takes:", "the value is an int, and colours are packed into one.");
    ref_head("int ColorToInt(color)");
    ref_line("Does:", "packs a raylib Color into the int GuiSetStyle wants.");
    printf("\n");
    code_good(LINES(GOOD_STYLE));
    code_bad(LINES(BAD_STYLE));
    bad("error: incompatible type for argument 3 of 'GuiSetStyle'");
    more("Call it once, after InitWindow and before the loop. It changes");
    more("global state: every raygui call after it draws in that style.");

    wait_enter();
    clear_screen();
    heading("GOOD PRACTICE");

    tip("Put every value raygui edits in a variable declared before the");
    more("loop, and pass its address. Never recreate it inside the loop.");
    tip("Pass sizeof of the buffer to GuiTextBox, not a number you typed.");
    tip("Use TextFormat's result at once. Copy it if it must last.");
    tip("Call GuiSetStyle once at start-up, not every frame.");
    tip("Keep each widget's rectangle on its own line, with a comment if");
    more("its position is not obvious. A screen of bare numbers is hard");
    more("to change later.");
    tip("Update state from the answers first, draw the result after, as");
    more("in the example: the click is handled, then the greeting appears.");

    wait_enter();
    clear_screen();
    exercise(6);

    question("Does raygui remember whether a check box is ticked between\n"
             "  frames, or do you? (raygui or you)",
             "you",
             "raygui keeps nothing. You pass your variable in every frame\n"
             "             and it writes the new value back.");

    question("GuiCheckBox's last argument is a pointer to a bool. How do\n"
             "  you pass the variable shout? (write the argument)",
             "&shout",
             "The & passes its address so raygui can write the answer back.\n"
             "             Passing shout itself is a compile error.");

    question("TextFormat reuses a small set of buffers. After how many\n"
             "  MORE calls was the first result overwritten in the test?\n"
             "  (a number)",
             "4",
             "Three more calls left it intact; the fourth overwrote it.\n"
             "             Use the result immediately or copy it.");

    question("GuiSetStyle takes an int for a colour. Which raylib function\n"
             "  turns a Color into that int? (the function name)",
             "ColorToInt",
             "ColorToInt(DARKBLUE) packs the colour. Passing the Color\n"
             "             itself is a compile error.");

    printf("\n  >> BUILD THIS YOURSELF, in a real file\n");
    rule();
    printf("  Add a Reset button next to Greet. It sets the greet count\n");
    printf("  back to 0 and empties the name. Emptying a C string means\n");
    printf("  setting its first byte to '\\0'.\n\n");

    printf("  There is no transcript to match, the result is a window. You\n");
    printf("  have it right when, after greeting, Reset makes the\n");
    printf("  greeting disappear and the text box empty.\n");

    if (ask_yes("Want to see example code?")) {
        printf("\n");
        rule();
        printf("  if (GuiButton((Rectangle){196, 136, 160, 36}, \"Reset\")) {\n");
        printf("      greets = 0;\n");
        printf("      name[0] = '\\0';   // an empty string: first byte is the end\n");
        printf("  }\n");
        rule();
        printf("  Compiled with no warnings and ran.\n");
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(6)) {
        printf("   - a raygui widget is a function call that draws and\n");
        printf("     answers in one go; your variables hold the state\n");
        printf("   - GuiCheckBox takes a pointer; GuiTextBox takes the real\n");
        printf("     buffer size and YOU toggle editMode\n");
        printf("   - TextFormat's result is a reused buffer: use it at once\n");
        printf("   - GuiSetStyle takes ints; ColorToInt packs a Color\n");
    }
    wait_enter();
}

void lesson_07_raylib_todo(void)
{
    title("MODULE 7 - RAYGUI: A TODO LIST");

    heading("WHAT YOU WILL HAVE AT THE END");

    printf("  The todo list from module 4, built again with raylib and\n");
    printf("  raygui: a text box, Add, Clear done, a row per task with a\n");
    printf("  tick box and an x, and an \"N left\" line. Same program, the\n");
    printf("  other way of building it.\n");

    heading("THE IDEA: THE LIST IS AN ARRAY, NOT WIDGETS");

    printf("  In GTK the list was a box of row widgets, and adding a task\n");
    printf("  meant creating widgets. Here there are no widgets. The tasks\n");
    printf("  are an array and a count, and the window just draws what the\n");
    printf("  array holds, every frame. To add a task you add to the\n");
    printf("  array; the next frame draws it.\n\n");

    printf("  That makes one habit worth having: keep the data operations\n");
    printf("  (add, delete, clear, count) as plain C functions that know\n");
    printf("  nothing about raylib. The example does, and that is also what\n");
    printf("  let them be tested without opening a window.\n");

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 1 OF 3: THE DATA");

    printf("  Save this as todo.c. All three parts are one file.\n\n");
    code(LINES(EXR3_P0));

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 2 OF 3: THE TOP ROW");

    code(LINES(EXR3_P1));

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 3 OF 3: THE ROWS");

    code(LINES(EXR3_P2));

    printf("\n  The data functions were tested by a program that included\n");
    printf("  this file and never opened a window:\n\n");
    printf("    add \"\"                       ->  not added, 0 tasks\n");
    printf("    add a, b, c, d               ->  4 tasks, 4 left\n");
    printf("    tick b and c                 ->  2 left\n");
    printf("    delete a, then Clear done    ->  1 task left: d\n");
    printf("    add past the limit           ->  returns 0, stays at 12\n");
    printf("    a 199 character task         ->  stored as 47, terminated\n");

    wait_enter();
    clear_screen();
    heading("REFERENCE 1/2: DELETING WITHOUT SKIPPING");

    ref_head("delete_task(i)   [defined in part 1]");
    ref_line("Does:", "removes task i by moving every later task one place");
    ref_line("", "down, then reduces the count.");
    printf("\n");
    code_bad(LINES(BAD_DELETE_LOOP));
    bad("The next row slides into slot i, then the loop moves on to i + 1,");
    more("so it is never handled. Run on a, b, c, d, deleting while at b,");
    more("the loop visited:  a b d  (c was skipped).");
    printf("\n");
    code_good(LINES(GOOD_DELETE_LATER));

    ref_head("char *strncpy(dest, source, n)");
    ref_line("Does:", "copies at most n bytes. If the source is n bytes or");
    ref_line("", "longer, NO terminating '\\0' is written.");
    printf("\n");
    code_bad(LINES(BAD_STRNCPY));
    bad("Measured: copying a long string into char[8] with n = 8 left the");
    more("last byte as 0x78, not 0x00. The array is not a string any more,");
    more("and printing it reads past the end: undefined behaviour.");
    code_good(LINES(GOOD_STRNCPY));

    wait_enter();
    clear_screen();
    heading("REFERENCE 2/2: SIZE LIMITS AND DERIVED VALUES");

    ref_head("int add_task(label)   [defined in part 1]");
    ref_line("Returns:", "1 if added, 0 if the text was empty or the list is");
    ref_line("", "full. The caller can tell what happened; nothing is lost");
    ref_line("", "silently.");
    printf("\n");
    tip("A fixed array has a fixed size. MAX_TASKS is 12 because 12 rows");
    more("fit in the window. A longer list would need scrolling (raygui");
    more("has GuiScrollPanel, not covered here) or a growing array.");

    ref_head("int tasks_left()   [defined in part 1]");
    ref_line("Does:", "counts the unticked tasks every time it is asked.");
    ref_line("", "As in module 4, there is no stored counter to keep in");
    ref_line("", "step with the data.");

    wait_enter();
    clear_screen();
    heading("GOOD PRACTICE");

    tip("Keep data functions free of drawing calls. They stay testable,");
    more("and a change to the screen cannot break them.");
    tip("In a loop that draws, only record what the user asked for.");
    more("Change the data after the loop.");
    tip("Give every function that can fail a return value that says so.");
    tip("Copy strings with an explicit length and end them yourself.");
    tip("Name the limits (MAX_TASKS, TASK_LEN) once, and use the name");
    more("everywhere, including in the checks.");
    tip("Derive values like \"N left\" from the data when needed.");

    wait_enter();
    clear_screen();
    exercise(7);

    question("In the test, deleting row b inside the loop over a, b, c, d\n"
             "  visited a, b, d. Which row was skipped? (the letter)",
             "c",
             "After b is deleted, c moves into b's slot, and the loop goes\n"
             "             on to the next index. Note the delete, do it after.");

    question("strncpy(dst, src, sizeof dst) with a long src: does dst\n"
             "  always end with a '\\0'? (yes or no)",
             "no",
             "Measured: the last byte was 0x78. strncpy writes no\n"
             "             terminator when the source is as long as n. End it\n"
             "             yourself.");

    question("What does add_task return when the list is full? (a number)",
             "0",
             "0 means \"not added\". The caller can check it. 1 means\n"
             "             it was added.");

    printf("\n  >> BUILD THIS YOURSELF, in a real file\n");
    rule();
    printf("  Add an All done button that ticks every task. Write the\n");
    printf("  loop as its own small function in the data part, like\n");
    printf("  clear_done, and call it from the button.\n\n");

    printf("  There is no transcript to match, the result is a window. You\n");
    printf("  have it right when, after adding some tasks, All done ticks\n");
    printf("  every box and the line reads 0 left.\n");

    if (ask_yes("Want to see example code?")) {
        printf("\n");
        rule();
        printf("  static void tick_all(void)\n");
        printf("  {\n");
        printf("      for (int i = 0; i < count; i++)\n");
        printf("          done[i] = true;\n");
        printf("  }\n");
        rule();
        printf("  Call it from  if (GuiButton(bounds, \"All done\")) tick_all();\n");
        printf("  Compiled with no warnings and tested: two tasks, tick_all,\n");
        printf("  0 left and both tasks still present.\n");
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(7)) {
        printf("   - the list is an array; the window redraws it every frame\n");
        printf("   - keep data functions free of drawing so they can be\n");
        printf("     tested\n");
        printf("   - never delete inside the loop that draws: note it, then\n");
        printf("     delete after\n");
        printf("   - strncpy may not terminate; end the string yourself\n\n");

        printf("  Not covered: scrolling a long list, other raygui\n");
        printf("  controls (sliders, drop-downs), and loading images and\n");
        printf("  sound with raylib. raylib's own cheatsheet lists them.\n");
    }
    wait_enter();
}

