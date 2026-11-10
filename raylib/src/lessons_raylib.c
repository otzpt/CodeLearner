/*
 * Modules 1-6 - raylib and raygui: the loop, shapes and text, input,
 * time and movement, raygui widgets, and a todo list.
 *
 * The lesson program itself is plain C. The code it teaches needs raylib and
 * raygui, which the student builds (module 1 gives the exact steps).
 *
 * Function signatures and one-line descriptions in the reference screens
 * are raylib's own: they were read from raylib.h and raymath.h (zlib
 * license, (c) Ramon Santamaria and contributors) at the commit below, not
 * retyped.
 *
 * Tested with raylib 6.5-dev (commit 0dedaf3066) and raygui 5.1-dev (commit
 * 2a3c4790c6), gcc 14.2.1, Linux with X11. The examples compile with
 * -std=c11 -Wall -Wextra and no warnings of their own; raygui.h prints 11
 * warnings under -Wextra, which -isystem hides. Each one was started and ran
 * until stopped.
 *
 * Measured by small programs and quoted as real results:
 *   drawing, read back from an off-screen texture: origin top-left, later
 *     shapes on top, DrawRectangle covers x..x+w-1, (Color){255,0,0,0.5f}
 *     draws nothing, Fade(RED,0.5f) over white = (243,148,155), a triangle
 *     with clockwise vertices is not drawn, DrawLine at y=60 coloured row
 *     59, circle radius extent, MeasureText widths (186 at 30, 124 at 20)
 *   collision: CheckCollisionPointRec on a {100,100,50,30} box (edges),
 *     touching rectangles do not collide, GetCollisionRec, Clamp,
 *     Vector2Length / Vector2Normalize of (1,1)
 *   timing: x += 3 per frame gave 93 px at 30 FPS and 183 px at 60 FPS,
 *     x += 200 * GetFrameTime() gave 203 px at both; 12134 frames a second
 *     with no SetTargetFPS, 60 with it
 *   pong rules (update_ball), the todo list's data functions: driven by test
 *     programs that included the files without opening a window
 *   other: LoadTexture of a missing file, TextFormat's reused buffer,
 *     strncpy, delete inside a draw loop, GuiCheckBox and GuiSetStyle
 *     argument errors
 *
 * NOT driven by a test: how real key presses, mouse clicks, the wheel and
 * raygui widgets behave under a person's hands. The lessons say so where
 * they describe it; it is raylib's and raygui's documented behaviour.
 *
 * Explained and not run, because they are undefined behaviour: a text box
 * told its buffer is larger than it is, and printing an unterminated string.
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
    "// shapes.c : rectangles, circles, lines, a triangle and centred text.",
    "",
    "#include \"raylib.h\"",
    "",
    "int main(void)",
    "{",
    "    InitWindow(640, 360, \"Shapes\");",
    "    SetTargetFPS(60);",
    "",
    "    // A colour is four numbers from 0 to 255: red, green, blue, alpha.",
    "    // Alpha is opacity: 255 is solid, 0 is invisible.",
    "    Color teal = {0, 150, 150, 255};",
    "",
    "    // Fade(colour, 0.0 to 1.0) makes a see-through copy. Use it rather",
    "    // than writing alpha by hand: alpha is a whole number, so 0.5",
    "    // typed there would be cut down to 0 and draw nothing.",
    "    Color glass = Fade(teal, 0.4f);",
};
static const char *const EXR2_P1[] = {
    "    while (!WindowShouldClose()) {",
    "        BeginDrawing();",
    "        ClearBackground(RAYWHITE);",
    "",
    "        // Rectangles: x, y, width, height, colour. (x, y) is the",
    "        // top-left corner, and y grows downward.",
    "        DrawRectangle(40, 40, 160, 100, teal);",
    "        DrawRectangle(120, 90, 160, 100, glass);   // drawn on top",
    "        DrawRectangleLines(40, 40, 160, 100, DARKGRAY);   // outline",
    "",
    "        // Circles: centre x, centre y, radius, colour.",
    "        DrawCircle(400, 100, 50, ORANGE);",
    "        DrawCircleLines(400, 100, 60, DARKGRAY);",
    "",
    "        // A line from (x1, y1) to (x2, y2).",
    "        DrawLine(320, 200, 600, 200, GRAY);",
    "",
    "        // A triangle from three points. The order matters: top, then",
    "        // bottom-left, then bottom-right (counter-clockwise on screen).",
    "        DrawTriangle((Vector2){480, 210}, (Vector2){440, 290},",
    "                     (Vector2){520, 290}, MAROON);",
};
static const char *const EXR2_P2[] = {
    "        // Centre text: measure its width, then start half a width left",
    "        // of the middle of the window.",
    "        const char *title = \"Centred with MeasureText\";",
    "        int width = MeasureText(title, 24);",
    "        int x = (GetScreenWidth() - width) / 2;",
    "        DrawText(title, x, 310, 24, DARKGRAY);",
    "",
    "        DrawFPS(10, 10);   // raylib's own frame counter, top left",
    "",
    "        EndDrawing();",
    "    }",
    "",
    "    CloseWindow();",
    "    return 0;",
    "}",
};
static const char *const GOOD_COLOR[] = {
    "Color teal = {0, 150, 150, 255};",
    "Color glass = Fade(teal, 0.4f);   // 40% opaque",
};
static const char *const BAD_ALPHA[] = {
    "Color glass = (Color){255, 0, 0, 0.5f};   // alpha is whole",
};
static const char *const GOOD_RECT[] = {
    "DrawRectangle(10, 10, 20, 20, RED);   // covers x 10 to 29",
};
static const char *const GOOD_TRI[] = {
    "Vector2 top = {50, 10}, left = {10, 90}, right = {90, 90};",
    "DrawTriangle(top, left, right, MAROON);",
};
static const char *const BAD_TRI[] = {
    "DrawTriangle(top, right, left, MAROON);   // other order",
};
static const char *const GOOD_CENTRE[] = {
    "int w = MeasureText(text, 24);",
    "DrawText(text, (GetScreenWidth() - w) / 2, y, 24, DARKGRAY);",
};
static const char *const BAD_CENTRE[] = {
    "DrawText(text, GetScreenWidth() / 2, y, 24, DARKGRAY);",
};
static const char *const EXR3_P0[] = {
    "// input.c : keyboard, mouse position, clicks and the wheel.",
    "",
    "#include \"raylib.h\"",
    "",
    "int main(void)",
    "{",
    "    InitWindow(640, 360, \"Input\");",
    "    SetTargetFPS(60);",
    "",
    "    Rectangle box = {290, 150, 60, 60};   // the square we control",
    "    bool orange = false;                  // flipped by a click",
    "    int pressed = 0;                      // times SPACE went down",
    "    int held = 0;                         // frames SPACE was held",
};
static const char *const EXR3_P1[] = {
    "    while (!WindowShouldClose()) {",
    "        // ---- 1. read the input and change the state ----",
    "",
    "        // IsKeyDown is true on EVERY frame the key is held. Use it for",
    "        // movement that should continue while the key is down.",
    "        if (IsKeyDown(KEY_RIGHT)) box.x += 4;",
    "        if (IsKeyDown(KEY_LEFT))  box.x -= 4;",
    "        if (IsKeyDown(KEY_DOWN))  box.y += 4;",
    "        if (IsKeyDown(KEY_UP))    box.y -= 4;",
    "",
    "        // IsKeyPressed is true only on the one frame the key goes down.",
    "        // Use it for one-shot actions. Compare the counters on screen.",
    "        if (IsKeyPressed(KEY_SPACE)) pressed++;",
    "        if (IsKeyDown(KEY_SPACE)) held++;",
    "",
    "        // The mouse position is a Vector2: .x and .y in window pixels.",
    "        Vector2 mouse = GetMousePosition();",
    "",
    "        // Is the pointer inside the square? A point and a rectangle.",
    "        bool over = CheckCollisionPointRec(mouse, box);",
    "",
    "        // A click is \"the button went down this frame\" AND \"the pointer",
    "        // is over the square\".",
    "        if (over && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))",
    "            orange = !orange;",
    "",
    "        // The wheel gives how far it turned this frame: positive is up.",
    "        box.width += GetMouseWheelMove() * 4;",
    "        box.height = box.width;   // keep it square",
    "        if (box.width < 20) box.width = box.height = 20;",
    "",
    "        // Keep the square inside the window.",
    "        if (box.x < 0) box.x = 0;",
    "        if (box.y < 0) box.y = 0;",
    "        if (box.x + box.width > GetScreenWidth())",
    "            box.x = GetScreenWidth() - box.width;",
    "        if (box.y + box.height > GetScreenHeight())",
    "            box.y = GetScreenHeight() - box.height;",
};
static const char *const EXR3_P2[] = {
    "        // ---- 2. draw the result ----",
    "        BeginDrawing();",
    "        ClearBackground(RAYWHITE);",
    "",
    "        // DrawRectangleRec takes a whole Rectangle.",
    "        DrawRectangleRec(box, orange ? ORANGE : SKYBLUE);",
    "        if (over)   // a thick outline while the pointer is over it",
    "            DrawRectangleLinesEx(box, 3, DARKGRAY);",
    "",
    "        DrawText(TextFormat(\"mouse: %.0f, %.0f\", mouse.x, mouse.y),",
    "                 10, 10, 20, DARKGRAY);",
    "        DrawText(TextFormat(\"SPACE pressed %d, held %d frames\",",
    "                            pressed, held), 10, 36, 20, DARKGRAY);",
    "        DrawText(\"arrows move, click the square, wheel resizes\",",
    "                 10, 330, 20, GRAY);",
    "",
    "        EndDrawing();",
    "    }",
    "",
    "    CloseWindow();",
    "    return 0;",
    "}",
};
static const char *const GOOD_KEYS[] = {
    "if (IsKeyDown(KEY_RIGHT)) x += 4;      // move while held",
    "if (IsKeyPressed(KEY_SPACE)) fire();    // once per press",
};
static const char *const BAD_KEYS[] = {
    "if (IsKeyDown(KEY_SPACE)) fire();   // fires every frame it is held",
};
static const char *const GOOD_CHAR[] = {
    "int c;",
    "while ((c = GetCharPressed()) > 0)",
    "    add_to_text(c);   // one call per typed character",
};
static const char *const BAD_CLICK[] = {
    "if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))",
    "    orange = !orange;   // flips even when you click elsewhere",
};
static const char *const GOOD_CLICK[] = {
    "if (over && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))",
    "    orange = !orange;",
};
static const char *const BAD_FLOAT[] = {
    "if (mouse.x == box.x)   // exact match on floats is almost never true",
};
static const char *const EXR4_P0[] = {
    "// pong.c : a ball and a paddle. Movement uses time, not frames.",
    "",
    "#include \"raylib.h\"",
    "#include \"raymath.h\"   // Vector2 maths: Vector2Add, Clamp, and so on",
    "",
    "#define W 640",
    "#define H 360",
    "",
    "typedef struct {",
    "    Vector2 pos;      // centre of the ball, in pixels",
    "    Vector2 vel;      // pixels per SECOND, not per frame",
    "    float radius;",
    "} Ball;",
    "",
    "// ---- the rules: no drawing in here, so it can be tested ------------",
    "",
    "// Move the ball for dt seconds, bounce it off the walls and the paddle.",
    "// Returns 1 if it hit the paddle, -1 if it fell off the bottom, else 0.",
    "static int update_ball(Ball *b, Rectangle paddle, float dt)",
    "{",
    "    // Distance = speed * time. Because dt is the real frame time, the",
    "    // ball covers the same distance per second at any frame rate.",
    "    b->pos = Vector2Add(b->pos, Vector2Scale(b->vel, dt));",
    "",
    "    if (b->pos.x < b->radius || b->pos.x > W - b->radius)",
    "        b->vel.x = -b->vel.x;   // side walls: flip the horizontal speed",
    "    if (b->pos.y < b->radius)",
    "        b->vel.y = -b->vel.y;   // top wall",
    "",
    "    // Only bounce while moving DOWN, or it could stick in the paddle.",
    "    if (b->vel.y > 0",
    "        && CheckCollisionCircleRec(b->pos, b->radius, paddle)) {",
    "        b->vel.y = -b->vel.y;",
    "        return 1;",
    "    }",
    "    if (b->pos.y > H + b->radius)",
    "        return -1;",
    "    return 0;",
    "}",
};
static const char *const EXR4_P1[] = {
    "int main(void)",
    "{",
    "    InitWindow(W, H, \"Pong\");",
    "    SetTargetFPS(60);",
    "",
    "    Ball ball = {{W / 2.0f, 60}, {180, 220}, 10};",
    "    Rectangle paddle = {W / 2.0f - 50, H - 30, 100, 12};",
    "    int score = 0;",
    "",
    "    while (!WindowShouldClose()) {",
    "        // GetFrameTime() is how many seconds the last frame took.",
    "        float dt = GetFrameTime();",
    "",
    "        // The paddle follows the mouse, kept inside the window.",
    "        paddle.x = Clamp(GetMouseX() - paddle.width / 2,",
    "                         0, W - paddle.width);",
    "        if (IsKeyDown(KEY_LEFT))  paddle.x -= 300 * dt;",
    "        if (IsKeyDown(KEY_RIGHT)) paddle.x += 300 * dt;",
    "",
    "        int result = update_ball(&ball, paddle, dt);",
    "        if (result == 1) score++;",
    "        if (result == -1) {   // missed: start again from the top",
    "            ball.pos = (Vector2){W / 2.0f, 60};",
    "            ball.vel = (Vector2){180, 220};",
    "            score = 0;",
    "        }",
    "",
    "        BeginDrawing();",
    "        ClearBackground(RAYWHITE);",
    "        DrawRectangleRec(paddle, DARKGRAY);",
    "        DrawCircleV(ball.pos, ball.radius, MAROON);   // takes a Vector2",
    "        DrawText(TextFormat(\"score %d\", score), 10, 10, 20, DARKGRAY);",
    "        EndDrawing();",
    "    }",
    "",
    "    CloseWindow();",
    "    return 0;",
    "}",
};
static const char *const GOOD_DT[] = {
    "x += speed * GetFrameTime();   // speed in pixels per second",
};
static const char *const BAD_PERFRAME[] = {
    "x += 3;   // pixels per frame: faster on a faster computer",
};
static const char *const BAD_DIAG[] = {
    "if (right) x += s; if (down) y += s;   // diagonal = 1.4142 * s",
};
static const char *const BAD_STICK[] = {
    "if (CheckCollisionCircleRec(ball, r, paddle))",
    "    vel.y = -vel.y;   // also true on the next frame, still inside",
};
static const char *const EXR5_P0[] = {
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
static const char *const EXR5_P1[] = {
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
static const char *const EXR5_P2[] = {
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
static const char *const EXR6_P0[] = {
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
static const char *const EXR6_P1[] = {
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
static const char *const EXR6_P2[] = {
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

void lesson_01_raylib(void)
{
    title("MODULE 1 - RAYLIB: YOUR FIRST WINDOW AND THE LOOP");

    heading("WHAT YOU WILL HAVE AT THE END");

    printf("  A window you drew yourself, 60 times a second. Then, in\n");
    printf("  modules 2 to 6: shapes, input, movement, then buttons and\n");
    printf("  a todo list on top of it.\n\n");

    printf("  raylib is a small C library for windows, drawing, input\n");
    printf("  and sound. It is made for games, but it works for any\n");
    printf("  program that draws. raygui is one extra header that adds\n");
    printf("  buttons, text boxes and check boxes on top of it.\n\n");

    printf("  GTK (the other course in the GUI group) and raylib solve\n");
    printf("  the same problem in opposite ways, and this module starts\n");
    printf("  with that difference.\n");

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
    exercise(1);

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

    if (!print_summary_pt(1)) {
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

void lesson_02_shapes(void)
{
    title("MODULE 2 - RAYLIB: SHAPES, COLOURS AND TEXT");

    heading("WHAT YOU WILL HAVE AT THE END");

    printf("  A window with rectangles, a circle, a line, a triangle and\n");
    printf("  text centred on the screen, drawn the way raylib draws: in\n");
    printf("  pixels, from the top-left corner, later shapes on top.\n");

    heading("THE IDEA: PIXELS, A CORNER AND AN ORDER");

    printf("  Three rules explain almost everything you draw. Each was\n");
    printf("  checked by drawing into an off-screen image and reading the\n");
    printf("  pixels back.\n\n");

    printf("  1. The origin (0, 0) is the TOP-LEFT corner, x grows to the\n");
    printf("     right and y grows DOWN. A rectangle drawn at 10, 10 had\n");
    printf("     its first pixel at 10, 10.\n\n");

    printf("  2. Shapes are painted in the order you call them, and a\n");
    printf("     later shape covers an earlier one. A red rectangle and a\n");
    printf("     blue one that overlapped gave blue at the overlap.\n\n");

    printf("  3. A colour is four numbers from 0 to 255: red, green, blue\n");
    printf("     and alpha (opacity, 255 solid, 0 invisible). raylib's RED\n");
    printf("     is (230, 41, 55), BLUE is (0, 121, 241), RAYWHITE is\n");
    printf("     (245, 245, 245).\n");

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 1 OF 3: COLOURS");

    printf("  Save this as shapes.c. All three parts are one file.\n\n");
    code(LINES(EXR2_P0));

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 2 OF 3: THE SHAPES");

    code(LINES(EXR2_P1));

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 3 OF 3: TEXT");

    code(LINES(EXR2_P2));

    printf("\n  Compile it with the line from module 1. You should see a\n");
    printf("  teal rectangle overlapped by a see-through one, an orange\n");
    printf("  circle, a grey line, a red-brown triangle and a centred title.\n");

    wait_enter();
    clear_screen();
    heading("REFERENCE 1/4: COLOURS");

    printf("  Color is a struct of four unsigned chars: r, g, b, a.\n\n");
    code_good(LINES(GOOD_COLOR));
    code_bad(LINES(BAD_ALPHA));
    bad("It compiles. 0.5 is cut down to 0, so alpha is 0: fully invisible.");
    more("Measured: a rectangle drawn with it left the white background pixel");
    more("at (255, 255, 255). With Fade(RED, 0.5f) the same pixel became");
    more("(243, 148, 155), a pale red.");

        ref_head("Color Fade(Color color, float alpha)");
    ref_line("Does:", "Get color with alpha applied, alpha goes from 0.0f to");
    ref_line("", "1.0f");
    ref_head("int ColorToInt(Color color)");
    ref_line("Does:", "Get hexadecimal value for a Color (0xRRGGBBAA)");
    ref_head("Color GetColor(unsigned int hexValue)");
    ref_line("Does:", "Get Color structure from hexadecimal value");
    ref_head("Color ColorFromHSV(float hue, float saturation, float value)");
    ref_line("Does:", "Get a Color from HSV values, hue [0..360],");
    ref_line("", "saturation/value [0..1]");
    ref_head("Color ColorAlpha(Color color, float alpha)");
    ref_line("Does:", "Get color with alpha applied, alpha goes from 0.0f to");
    ref_line("", "1.0f");
    ref_head("Color ColorBrightness(Color color, float factor)");
    ref_line("Does:", "Get color with brightness correction, brightness factor");
    ref_line("", "goes from -1.0f to 1.0f");

    wait_enter();
    clear_screen();
    heading("REFERENCE 2/4: RECTANGLES, CIRCLES, LINES, TRIANGLES");

        ref_head("void DrawRectangle(int posX, int posY, int width, int height,\n    Color color)");
    ref_line("Does:", "Draw a color-filled rectangle");
    ref_head("void DrawRectangleRec(Rectangle rec, Color color)");
    ref_line("Does:", "Draw a color-filled rectangle");
    ref_head("void DrawRectangleLines(int posX, int posY, int width, int height,\n    Color color)");
    ref_line("Does:", "Draw rectangle outline");
    printf("\n");
    code_good(LINES(GOOD_RECT));
    tip("Measured: pixels (10,10) and (29,29) were red; (30,30) and (9,9)");
    more("were not. A rectangle covers x to x + width - 1, like an array.");
    tip("DrawRectangleLines drew only the border: the edge pixel was black,");
    more("a pixel inside stayed white.");

        ref_head("void DrawCircle(int centerX, int centerY, float radius, Color color)");
    ref_line("Does:", "Draw a color-filled circle");
    ref_head("void DrawCircleLines(int centerX, int centerY, float radius,\n    Color color)");
    ref_line("Does:", "Draw circle outline");
    tip("Measured with a circle at (50, 150), radius 20: pixel (69, 150) was");
    more("inside, (71, 150) was outside.");

        ref_head("void DrawLine(int startPosX, int startPosY, int endPosX, int endPosY,\n    Color color)");
    ref_line("Does:", "Draw a line");
    tip("Measured: DrawLine from (0, 60) to (99, 60) coloured row 59, one");
    more("above the y you gave. For exact pixels use a rectangle.");

    wait_enter();
    clear_screen();
    heading("REFERENCE 2/4: TRIANGLES (continued)");

        ref_head("void DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color)");
    ref_line("Does:", "Draw a color-filled triangle, counter-clockwise vertex");
    ref_line("", "order");
    printf("\n");
    code_good(LINES(GOOD_TRI));
    code_bad(LINES(BAD_TRI));
    bad("Same three points, other order: nothing is drawn, and there is no");
    more("error. Measured: the centre pixel stayed white. Give the points in");
    more("counter-clockwise order on screen: top, bottom-left, bottom-right.");

    wait_enter();
    clear_screen();
    heading("REFERENCE 3/4: TEXT");

        ref_head("void DrawText(const char *text, int posX, int posY, int fontSize,\n    Color color)");
    ref_line("Does:", "Draw text (using default font)");
    ref_head("int MeasureText(const char *text, int fontSize)");
    ref_line("Does:", "Measure string width for default font");
    ref_head("void DrawFPS(int posX, int posY)");
    ref_line("Does:", "Draw current FPS");
    ref_head("int GetScreenWidth(void)");
    ref_line("Does:", "Get current screen width");
    ref_head("int GetScreenHeight(void)");
    ref_line("Does:", "Get current screen height");
    printf("\n");
    tip("Measured with the default font, \"Hello, raylib!\": 186 px wide at");
    more("size 30, 124 px at size 20. The width scales with the size.");
    code_good(LINES(GOOD_CENTRE));
    code_bad(LINES(BAD_CENTRE));
    bad("That starts the text at the middle, so its centre is half a text");
    more("width too far right. For the size-30 example, 93 px.");

    wait_enter();
    clear_screen();
    heading("REFERENCE 4/4: EVERYTHING raylib CAN DRAW (shapes)");

    printf("  The rest of raylib's basic shapes, with raylib's own one-line\n");
    printf("  descriptions (from raylib.h). Same pattern as above.\n\n");
        api_line("void DrawPixel(int posX, int posY, Color color)", "Draw a pixel using geometry [Can be slow, use with care]");
    api_line("void DrawPixelV(Vector2 position, Color color)", "Draw a pixel using geometry (Vector version) [Can be slow, use with\n      care]");
    api_line("void DrawLineV(Vector2 startPos, Vector2 endPos, Color color)", "Draw a line (using gl lines)");
    api_line("void DrawLineEx(Vector2 startPos, Vector2 endPos, float thick,\n    Color color)", "Draw a line (using triangles/quads)");
    api_line("void DrawLineStrip(const Vector2 *points, int pointCount, Color color)", "Draw lines sequence (using gl lines)");
    api_line("void DrawLineBezier(Vector2 startPos, Vector2 endPos, float thick,\n    Color color)", "Draw line segment cubic-bezier in-out interpolation");
    api_line("void DrawLineDashed(Vector2 startPos, Vector2 endPos, int dashSize,\n    int spaceSize, Color color)", "Draw a dashed line");
    api_line("void DrawTriangleGradient(Vector2 v1, Vector2 v2, Vector2 v3, Color c1,\n    Color c2, Color c3)", "Draw triangle with interpolated colors, counter-clockwise\n      vertex/color order");
    api_line("void DrawTriangleLines(Vector2 v1, Vector2 v2, Vector2 v3, Color color)", "Draw triangle outline, counter-clockwise vertex order");
    wait_enter();
    clear_screen();
    heading("ALL THE SHAPES FUNCTIONS (continued)");

    api_line("void DrawTriangleLinesEx(Vector2 v1, Vector2 v2, Vector2 v3, float thick,\n    Color color)", "Draw triangle outline with line thickness, counter-clockwise vertex\n      order");
    api_line("void DrawTriangleFan(const Vector2 *points, int pointCount, Color color)", "Draw a triangle fan defined by points (first vertex is the center)");
    api_line("void DrawTriangleStrip(const Vector2 *points, int pointCount, Color color)", "Draw a triangle strip defined by points");
    api_line("void DrawRectangleV(Vector2 position, Vector2 size, Color color)", "Draw a color-filled rectangle (Vector version)");
    api_line("void DrawRectanglePro(Rectangle rec, Vector2 origin, float rotation,\n    Color color)", "Draw a color-filled rectangle with pro parameters");
    api_line("void DrawRectangleGradientV(int posX, int posY, int width, int height,\n    Color top, Color bottom)", "Draw a vertical-gradient-filled rectangle");
    api_line("void DrawRectangleGradientH(int posX, int posY, int width, int height,\n    Color left, Color right)", "Draw a horizontal-gradient-filled rectangle");
    api_line("void DrawRectangleGradientEx(Rectangle rec, Color col1, Color col2,\n    Color col3, Color col4)", "Draw a gradient-filled rectangle with custom vertex colors,\n      counter-clockwise color order");
    api_line("void DrawRectangleLinesEx(Rectangle rec, float thick, Color color)", "Draw rectangle outline with line thickness");
    wait_enter();
    clear_screen();
    heading("ALL THE SHAPES FUNCTIONS (continued)");

    api_line("void DrawRectangleRounded(Rectangle rec, float roundness, int segments,\n    Color color)", "Draw rectangle with rounded edges");
    api_line("void DrawRectangleRoundedLines(Rectangle rec, float roundness,\n    int segments, Color color)", "Draw rectangle lines with rounded edges");
    api_line("void DrawRectangleRoundedLinesEx(Rectangle rec, float roundness,\n    int segments, float thick, Color color)", "Draw rectangle lines with rounded edges outline and line thickness");
    api_line("void DrawPoly(Vector2 center, int sides, float radius, float rotation,\n    Color color)", "Draw a polygon of n sides");
    api_line("void DrawPolyLines(Vector2 center, int sides, float radius,\n    float rotation, Color color)", "Draw a polygon outline of n sides");
    api_line("void DrawPolyLinesEx(Vector2 center, int sides, float radius,\n    float rotation, float thick, Color color)", "Draw a polygon outline of n sides with line thickness");
    api_line("void DrawCircleV(Vector2 center, float radius, Color color)", "Draw a color-filled circle (Vector version)");
    api_line("void DrawCircleGradient(Vector2 center, float radius, Color inner,\n    Color outer)", "Draw a gradient-filled circle");
    api_line("void DrawCircleSector(Vector2 center, float radius, float startAngle,\n    float endAngle, int segments, Color color)", "Draw a piece of a circle");
    wait_enter();
    clear_screen();
    heading("ALL THE SHAPES FUNCTIONS (continued)");

    api_line("void DrawCircleSectorLines(Vector2 center, float radius,\n    float startAngle, float endAngle, int segments, Color color)", "Draw circle sector outline");
    api_line("void DrawCircleSectorLinesEx(Vector2 center, float radius,\n    float startAngle, float endAngle, int segments, float thick,\n    Color color)", "Draw circle sector outline with thickness");
    api_line("void DrawCircleLinesV(Vector2 center, float radius, Color color)", "Draw circle outline (Vector version)");
    api_line("void DrawCircleLinesEx(Vector2 center, float radius, float thick,\n    Color color)", "Draw circle outline with line thickness");
    api_line("void DrawEllipse(int centerX, int centerY, float radiusH, float radiusV,\n    Color color)", "Draw ellipse");
    api_line("void DrawEllipseV(Vector2 center, float radiusH, float radiusV,\n    Color color)", "Draw ellipse (Vector version)");
    api_line("void DrawEllipseLines(int centerX, int centerY, float radiusH,\n    float radiusV, Color color)", "Draw ellipse outline");
    api_line("void DrawEllipseLinesV(Vector2 center, float radiusH, float radiusV,\n    Color color)", "Draw ellipse outline (Vector version)");
    api_line("void DrawEllipseLinesEx(Vector2 center, float radiusH, float radiusV,\n    float thick, Color color)", "Draw ellipse outline with line thickness");
    wait_enter();
    clear_screen();
    heading("ALL THE SHAPES FUNCTIONS (continued)");

    api_line("void DrawRing(Vector2 center, float innerRadius, float outerRadius,\n    float startAngle, float endAngle, int segments, Color color)", "Draw ring");
    api_line("void DrawRingLines(Vector2 center, float innerRadius, float outerRadius,\n    float startAngle, float endAngle, int segments, Color color)", "Draw ring outline");
    api_line("void DrawRingLinesEx(Vector2 center, float innerRadius,\n    float outerRadius, float startAngle, float endAngle, int segments,\n    float thick, Color color)", "Draw ring outline with line thickness");

    wait_enter();
    clear_screen();
    heading("EVERYTHING raylib CAN DO WITH COLOURS AND TEXT");

    printf("  Colours (raylib.h):\n\n");
        api_line("bool ColorIsEqual(Color col1, Color col2)", "Check if two colors are equal");
    api_line("Vector4 ColorNormalize(Color color)", "Get Color normalized as float [0..1]");
    api_line("Color ColorFromNormalized(Vector4 normalized)", "Get Color from normalized values [0..1]");
    api_line("Vector3 ColorToHSV(Color color)", "Get HSV values for a Color, hue [0..360], saturation/value [0..1]");
    api_line("Color ColorTint(Color color, Color tint)", "Get color multiplied with another color");
    api_line("Color ColorContrast(Color color, float contrast)", "Get color with contrast correction, contrast values between -1.0f\n      and 1.0f");
    api_line("Color ColorAlphaBlend(Color dst, Color src, Color tint)", "Get src alpha-blended into dst color with tint");
    api_line("Color ColorLerp(Color color1, Color color2, float factor)", "Get color lerp interpolation between two colors, factor [0.0f..1.0f]");
    api_line("Color GetPixelColor(const void *srcPtr, int format)", "Get Color from a source pixel pointer of certain format");
    wait_enter();
    clear_screen();
    heading("ALL THE COLOUR FUNCTIONS (continued)");

    api_line("void SetPixelColor(void *dstPtr, Color color, int format)", "Set color formatted into destination pixel pointer");
    api_line("int GetPixelDataSize(int width, int height, int format)", "Get pixel data size in bytes for certain format");

    wait_enter();
    clear_screen();
    heading("TEXT FUNCTIONS (raylib.h)");

    printf("  Fonts, measuring and string helpers. Fonts and extra text\n");
    printf("  functions need a loaded Font: see the textures and fonts\n");
    printf("  module when it is added.\n\n");
        api_line("void DrawFPS(int posX, int posY)", "Draw current FPS");
    api_line("void DrawTextEx(Font font, const char *text, Vector2 position,\n    float fontSize, float spacing, Color tint)", "Draw text using font and additional parameters");
    api_line("void DrawTextPro(Font font, const char *text, Vector2 position,\n    Vector2 origin, float rotation, float fontSize, float spacing,\n    Color tint)", "Draw text using Font and pro parameters (rotation)");
    api_line("void DrawTextCodepoint(Font font, int codepoint, Vector2 position,\n    float fontSize, Color tint)", "Draw one character (codepoint)");
    api_line("void DrawTextCodepoints(Font font, const int *codepoints,\n    int codepointCount, Vector2 position, float fontSize, float spacing,\n    Color tint)", "Draw multiple characters (codepoint)");

    wait_enter();
    clear_screen();
    heading("GOOD PRACTICE");

    tip("Draw back to front: background first, then what sits on top.");
    tip("Use Fade() for see-through colours; never type alpha as a fraction.");
    tip("Put the numbers you will change (positions, sizes) in variables or");
    more("constants with names, not scattered literals.");
    tip("Centre with MeasureText and GetScreenWidth, not by guessing x.");
    tip("Check winding order when a triangle does not appear.");
    tip("Do not rely on DrawLine for pixel-exact placement.");
    tip("Draw only between BeginDrawing and EndDrawing.");

    wait_enter();
    clear_screen();
    exercise(2);

    question("DrawRectangle(10, 10, 20, 20, RED) was drawn. Is the pixel at\n"
             "  (30, 30) red? (yes or no)",
             "no",
             "A rectangle covers x to x + width - 1. Measured: (29, 29) was\n"
             "             red, (30, 30) was not.");

    question("A triangle drawn with vertices top, bottom-right, bottom-left\n"
             "  was drawn or not drawn? (write: drawn or not drawn)",
             "not drawn",
             "The order was clockwise on screen. Only counter-clockwise is\n"
             "             drawn: top, bottom-left, bottom-right.");

    question("(Color){255, 0, 0, 0.5f} ends up with what alpha? (a number)",
             "0",
             "Alpha is a whole number, so 0.5 is cut down to 0: invisible.\n"
             "             Use Fade(RED, 0.5f).");

    printf("\n  >> BUILD THIS YOURSELF, in a real file\n");
    rule();
    printf("  Draw a traffic light: a dark rectangle with three circles\n");
    printf("  inside it, red on top, yellow in the middle, green at the\n");
    printf("  bottom, and the word \"STOP\" centred under it.\n\n");

    printf("  There is no transcript to match, the result is a window. You\n");
    printf("  have it right when the three circles are stacked inside the\n");
    printf("  rectangle and the text is centred under it.\n");

    if (ask_yes("Want to see example code?")) {
        printf("\n");
        rule();
        printf("  DrawRectangle(270, 40, 100, 240, DARKGRAY);\n");
        printf("  DrawCircle(320, 90, 30, RED);\n");
        printf("  DrawCircle(320, 160, 30, YELLOW);\n");
        printf("  DrawCircle(320, 230, 30, GREEN);\n");
        printf("  int w = MeasureText(\"STOP\", 30);\n");
        printf("  DrawText(\"STOP\", (GetScreenWidth() - w) / 2, 300, 30, BLACK);\n");
        rule();
        printf("  Compiled with no warnings and ran, inside the usual loop.\n");
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(2)) {
        printf("   - the origin is the top-left, y grows down; later shapes\n");
        printf("     cover earlier ones\n");
        printf("   - a rectangle covers x to x + width - 1\n");
        printf("   - Fade() for transparency: a fractional alpha becomes 0\n");
        printf("   - triangles need counter-clockwise points or vanish\n");
        printf("   - centre text with MeasureText\n");
    }
    wait_enter();
}

void lesson_03_input(void)
{
    title("MODULE 3 - RAYLIB: KEYBOARD, MOUSE AND CLICKS");

    heading("WHAT YOU WILL HAVE AT THE END");

    printf("  A square you move with the arrow keys, recolour by clicking\n");
    printf("  it, and resize with the mouse wheel, with the mouse position\n");
    printf("  and key counters drawn on screen. This is the module that\n");
    printf("  makes a raylib window respond to a person.\n");

    heading("THE IDEA: ASK EVERY FRAME");

    printf("  There are no callbacks in raylib. Each frame, your loop asks\n");
    printf("  questions about the input, and the answers are valid for\n");
    printf("  that frame only. For every key and mouse button there are\n");
    printf("  two kinds of question:\n\n");

    ref_line("Down", "is it held at this moment? True on EVERY frame while held.");
    ref_line("Pressed", "did it go down THIS frame? True on ONE frame only.");
    ref_line("Released", "did it come up this frame? True on one frame.");

    printf("\n  Pick by what you want. Moving while a key is held is Down.\n");
    printf("  Firing a shot, toggling a setting or clicking a button is\n");
    printf("  Pressed. Mixing them up is the commonest input bug.\n\n");

    printf("  Note on testing: the collision results quoted below were run.\n");
    printf("  How a real key or mouse button behaves while a person holds\n");
    printf("  it is raylib's documented behaviour; it was not driven by a\n");
    printf("  test, because injecting fake input into a desktop session is\n");
    printf("  not something these examples do.\n");

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 1 OF 3: THE STATE");

    printf("  Save this as input.c. All three parts are one file.\n\n");
    code(LINES(EXR3_P0));

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 2 OF 3: READ INPUT, CHANGE STATE");

    code(LINES(EXR3_P1));

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 3 OF 3: DRAW THE RESULT");

    code(LINES(EXR3_P2));

    printf("\n  Compile it with the line from module 1. Move with the arrows,\n");
    printf("  hover the square (it gets an outline), click it, turn the wheel.\n");
    printf("  Hold SPACE for a second and compare the two counters.\n");

    wait_enter();
    clear_screen();
    heading("REFERENCE 1/4: KEYBOARD");

        ref_head("bool IsKeyDown(int key)");
    ref_line("Does:", "Check if key is being pressed");
    ref_head("bool IsKeyPressed(int key)");
    ref_line("Does:", "Check if key has been pressed once");
    ref_head("bool IsKeyReleased(int key)");
    ref_line("Does:", "Check if key has been released once");
    ref_head("bool IsKeyUp(int key)");
    ref_line("Does:", "Check if key is NOT being pressed");
    printf("\n");
    code_good(LINES(GOOD_KEYS));
    code_bad(LINES(BAD_KEYS));
    bad("Down is true on every frame the key is held, so at 60 frames a");
    more("second a held key fires about 60 times a second. (raylib's");
    more("documented behaviour, not driven by a test.)");

    printf("\n  Key names are constants. Read back from the header:\n");
    printf("    KEY_A=65  KEY_SPACE=32  KEY_ENTER=257  KEY_ESCAPE=256\n");
    printf("    KEY_RIGHT=262  KEY_LEFT=263  KEY_DOWN=264  KEY_UP=265\n");

    wait_enter();
    clear_screen();
    heading("REFERENCE 1/4: KEYBOARD (continued)");

        ref_head("int GetKeyPressed(void)");
    ref_line("Does:", "Get key pressed (keycode), call it multiple times for");
    ref_line("", "keys queued, returns 0 when the queue is empty");
    ref_head("int GetCharPressed(void)");
    ref_line("Does:", "Get char pressed (unicode), call it multiple times for");
    ref_line("", "chars queued, returns 0 when the queue is empty");
    ref_head("bool IsKeyPressedRepeat(int key)");
    ref_line("Does:", "Check if key has been pressed again");
    ref_head("const char *GetKeyName(int key)");
    ref_line("Does:", "Get name of a QWERTY key on the current keyboard layout");
    ref_line("", "(eg returns string 'q' for KEY_A on an AZERTY keyboard)");
    ref_head("void SetExitKey(int key)");
    ref_line("Does:", "Set a custom key to exit program (default is ESC)");
    printf("\n");
    tip("To read TYPED TEXT use GetCharPressed, not GetKeyPressed. It gives");
    more("the actual character (including unicode and shifted symbols), and");
    more("returns 0 when nothing is waiting. Call it in a loop until 0.");
    code_good(LINES(GOOD_CHAR));

    wait_enter();
    clear_screen();
    heading("REFERENCE 2/4: MOUSE");

        ref_head("Vector2 GetMousePosition(void)");
    ref_line("Does:", "Get mouse position XY");
    ref_head("int GetMouseX(void)");
    ref_line("Does:", "Get mouse position X");
    ref_head("int GetMouseY(void)");
    ref_line("Does:", "Get mouse position Y");
    ref_head("Vector2 GetMouseDelta(void)");
    ref_line("Does:", "Get mouse delta between frames");
        ref_head("bool IsMouseButtonPressed(int button)");
    ref_line("Does:", "Check if mouse button has been pressed once");
    ref_head("bool IsMouseButtonDown(int button)");
    ref_line("Does:", "Check if mouse button is being pressed");
    ref_head("bool IsMouseButtonReleased(int button)");
    ref_line("Does:", "Check if mouse button has been released once");

    printf("\n  Buttons: MOUSE_BUTTON_LEFT=0, MOUSE_BUTTON_RIGHT=1,\n");
    printf("  MOUSE_BUTTON_MIDDLE=2 (read back from the header).\n");

    wait_enter();
    clear_screen();
    heading("REFERENCE 2/4: MOUSE (continued)");

        ref_head("float GetMouseWheelMove(void)");
    ref_line("Does:", "Get mouse wheel movement for X or Y, whichever is larger");
    ref_head("Vector2 GetMouseWheelMoveV(void)");
    ref_line("Does:", "Get mouse wheel movement for both X and Y");
    ref_head("void SetMouseCursor(int cursor)");
    ref_line("Does:", "Set mouse cursor");
    ref_head("void SetMousePosition(int x, int y)");
    ref_line("Does:", "Set mouse position XY");
    printf("\n");
    code_bad(LINES(BAD_CLICK));
    bad("A click anywhere flips it. A click on a thing is two questions:");
    more("was the button pressed, AND is the pointer over the thing.");
    code_good(LINES(GOOD_CLICK));

    wait_enter();
    clear_screen();
    heading("REFERENCE 3/4: IS THE POINTER OVER IT? (hit testing)");

        ref_head("bool CheckCollisionPointRec(Vector2 point, Rectangle rec)");
    ref_line("Does:", "Check if point is inside rectangle");
    ref_head("bool CheckCollisionPointCircle(Vector2 point, Vector2 center,\n    float radius)");
    ref_line("Does:", "Check if point is inside circle");
    printf("\n  Measured, for Rectangle {100, 100, 50, 30} (x 100..149, y 100..129):\n\n");
    printf("    point (100, 100), the top-left       -> inside\n");
    printf("    point (149, 129), the last pixel     -> inside\n");
    printf("    point (150, 130), x + w and y + h    -> OUTSIDE\n");
    printf("    point (99.5, 110)                    -> outside\n");
    printf("    point (149.9, 110)                   -> inside\n\n");
    printf("  The box includes its top and left edges and excludes its\n");
    printf("  bottom and right edges, like the pixels in module 2.\n");
    printf("  A point (3, 4) is inside a circle at (0, 0) of radius 5 and\n");
    printf("  outside one of radius 4.\n\n");
    code_bad(LINES(BAD_FLOAT));
    good("Test a range: CheckCollisionPointRec(mouse, box).");

    wait_enter();
    clear_screen();
    heading("REFERENCE 4/4: EVERYTHING ELSE raylib CAN READ");

    printf("  Gamepads, touch and the rest of the mouse and keyboard,\n");
    printf("  with raylib's own one-line descriptions (raylib.h).\n\n");
        api_line("bool IsGamepadAvailable(int gamepad)", "Check if gamepad is available");
    api_line("const char *GetGamepadName(int gamepad)", "Get gamepad internal name id");
    api_line("bool IsGamepadButtonPressed(int gamepad, int button)", "Check if gamepad button has been pressed once");
    api_line("bool IsGamepadButtonDown(int gamepad, int button)", "Check if gamepad button is being pressed");
    api_line("bool IsGamepadButtonReleased(int gamepad, int button)", "Check if gamepad button has been released once");
    api_line("bool IsGamepadButtonUp(int gamepad, int button)", "Check if gamepad button is NOT being pressed");
    api_line("int GetGamepadButtonPressed(void)", "Get the last gamepad button pressed");
    api_line("int GetGamepadAxisCount(int gamepad)", "Get axis count for a gamepad");
    api_line("float GetGamepadAxisMovement(int gamepad, int axis)", "Get movement value for a gamepad axis");
    wait_enter();
    clear_screen();
    heading("GAMEPAD FUNCTIONS (continued)");

    api_line("int SetGamepadMappings(const char *mappings)", "Set internal gamepad mappings (SDL_GameControllerDB)");
    api_line("void SetGamepadVibration(int gamepad, float leftMotor, float rightMotor,\n    float duration)", "Set gamepad vibration for both motors (duration in seconds)");

    wait_enter();
    clear_screen();
    heading("TOUCH AND THE FULL KEYBOARD/MOUSE LIST");

        api_line("int GetTouchX(void)", "Get touch position X for touch point 0 (relative to screen size)");
    api_line("int GetTouchY(void)", "Get touch position Y for touch point 0 (relative to screen size)");
    api_line("Vector2 GetTouchPosition(int index)", "Get touch position XY for a touch point index (relative to screen\n      size)");
    api_line("int GetTouchPointId(int index)", "Get touch point identifier for provided index");
    api_line("int GetTouchPointCount(void)", "Get number of touch points");
        api_line("void SetMouseOffset(int offsetX, int offsetY)", "Set mouse offset");
    api_line("void SetMouseScale(float scaleX, float scaleY)", "Set mouse scaling");

    wait_enter();
    clear_screen();
    heading("GOOD PRACTICE");

    tip("Pick Down or Pressed on purpose, per action, and write which in a");
    more("comment if it is not obvious.");
    tip("Read all input at the top of the frame, change the state, then");
    more("draw. Do not read input in the middle of drawing.");
    tip("A click is Pressed plus a hit test. Write it as one condition.");
    tip("Use GetCharPressed for text, key constants for controls.");
    tip("Keep the thing the user controls inside the window: clamp it.");
    tip("Never compare floats with ==; use ranges and the collision calls.");

    wait_enter();
    clear_screen();
    exercise(3);

    question("Holding a key for one second at 60 frames a second, about how\n"
             "  many frames is IsKeyDown true? (a number)",
             "60",
             "Down is true on every frame the key is held. IsKeyPressed\n"
             "             would be true on only one of them.");

    question("Is the point (150, 130) inside Rectangle {100, 100, 50, 30}?\n"
             "  (yes or no)",
             "no",
             "Measured: the box excludes x + width and y + height. (149, 129)\n"
             "             was the last pixel inside.");

    question("Which function reads typed characters, including unicode,\n"
             "  rather than key codes? (the function name)",
             "GetCharPressed",
             "GetKeyPressed returns key codes. GetCharPressed returns the\n"
             "             characters typed, and 0 when none are waiting.");

    printf("\n  >> BUILD THIS YOURSELF, in a real file\n");
    rule();
    printf("  Starting from the example, make a RIGHT click anywhere move\n");
    printf("  the square back to the middle of the window. Use\n");
    printf("  MOUSE_BUTTON_RIGHT and the right kind of question.\n\n");

    printf("  There is no transcript to match, the result is a window. You\n");
    printf("  have it right when moving the square away and right-clicking\n");
    printf("  puts it back in the centre every time, not just while held.\n");

    if (ask_yes("Want to see example code?")) {
        printf("\n");
        rule();
        printf("  if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {\n");
        printf("      box.x = (GetScreenWidth() - box.width) / 2;\n");
        printf("      box.y = (GetScreenHeight() - box.height) / 2;\n");
        printf("  }\n");
        rule();
        printf("  Pressed, because it should happen once per click. Compiled\n");
        printf("  with no warnings and ran. Put it before the clamping code.\n");
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(3)) {
        printf("   - ask every frame: Down is every frame held, Pressed is\n");
        printf("     one frame, Released is one frame\n");
        printf("   - a click is Pressed plus a hit test\n");
        printf("   - a box includes its top-left edges and excludes x + w\n");
        printf("     and y + h\n");
        printf("   - GetCharPressed for typed text, key constants for controls\n");
    }
    wait_enter();
}

void lesson_04_move(void)
{
    title("MODULE 4 - RAYLIB: TIME, MOVEMENT AND COLLISION");

    heading("WHAT YOU WILL HAVE AT THE END");

    printf("  A small pong: a ball that bounces off the walls and a paddle\n");
    printf("  you move with the mouse or the arrow keys, with a score. It\n");
    printf("  moves at the same real speed on a slow or a fast computer.\n");

    heading("THE IDEA: MOVE BY TIME, NOT BY FRAMES");

    printf("  The obvious way to move something is x += 3 every frame. It\n");
    printf("  is wrong, because frames per second differ between computers.\n");
    printf("  Measured by moving a square for one second:\n\n");

    printf("    x += 3 per frame             30 FPS ->  93 px   60 FPS -> 183 px\n");
    printf("    x += 200 * GetFrameTime()    30 FPS -> 203 px   60 FPS -> 203 px\n\n");

    printf("  The same code moved twice as far at 60 FPS. The fix is the\n");
    printf("  real-world formula, distance = speed * time: keep speed in\n");
    printf("  pixels PER SECOND and multiply by the seconds the last frame\n");
    printf("  took, which raylib gives you as GetFrameTime().\n");

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 1 OF 2: THE RULES");

    printf("  Save this as pong.c. Note that the rules are a function with\n");
    printf("  no drawing in it. That is what let them be tested without a\n");
    printf("  window.\n\n");
    code(LINES(EXR4_P0));

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 2 OF 2: THE LOOP");

    code(LINES(EXR4_P1));

    printf("\n  The rules were tested by a program that included this file:\n\n");
    printf("    ball at the left wall       -> horizontal speed flipped\n");
    printf("    ball at the top wall        -> vertical speed flipped\n");
    printf("    falling onto the paddle     -> returns 1, ball goes up\n");
    printf("    moving up inside the paddle -> no bounce, no sticking\n");
    printf("    off the bottom              -> returns -1\n");
    printf("    1 s of flight at 100 px/s   -> x = 200.0 with 10, 30, 60 and\n");
    printf("                                   144 steps per second\n");

    wait_enter();
    clear_screen();
    heading("REFERENCE 1/3: TIME");

        ref_head("float GetFrameTime(void)");
    ref_line("Does:", "Get time in seconds for last frame drawn (delta time)");
    ref_head("double GetTime(void)");
    ref_line("Does:", "Get elapsed time in seconds since InitWindow()");
    ref_head("int GetFPS(void)");
    ref_line("Does:", "Get current FPS");
    ref_head("void WaitTime(double seconds)");
    ref_line("Does:", "Wait for some time (halt program execution)");
    ref_head("void SetTargetFPS(int fps)");
    ref_line("Does:", "Set target FPS (maximum)");
    printf("\n");
    code_good(LINES(GOOD_DT));
    code_bad(LINES(BAD_PERFRAME));
    bad("Measured: 93 px at 30 FPS but 183 px at 60 FPS for the same code.");
    more("The program is only right on the machine it was written on.");

    wait_enter();
    clear_screen();
    heading("REFERENCE 2/3: VECTOR MATHS (raymath.h)");

    printf("  raymath.h comes with raylib. Add #include \"raymath.h\".\n");
    printf("  A Vector2 is a struct with x and y: a position or a velocity.\n\n");
        ref_head("Vector2 Vector2Add(Vector2 v1, Vector2 v2)");
    ref_line("Does:", "Add two vectors (v1 + v2)");
    ref_head("Vector2 Vector2Scale(Vector2 v, float scale)");
    ref_line("Does:", "Scale vector (multiply by value)");
    ref_head("float Vector2Length(Vector2 v)");
    ref_line("Does:", "Calculate vector length");
    ref_head("Vector2 Vector2Normalize(Vector2 v)");
    ref_line("Does:", "Normalize provided vector");
    ref_head("float Vector2Distance(Vector2 v1, Vector2 v2)");
    ref_line("Does:", "Calculate distance between two vectors");
    ref_head("float Clamp(float value, float min, float max)");
    ref_line("Does:", "Clamp float value");
    ref_head("float Lerp(float start, float end, float amount)");
    ref_line("Does:", "Calculate linear interpolation between two floats");
    printf("\n  Measured: Clamp(150, 0, 100) = 100 and Clamp(-5, 0, 100) = 0.\n");
    printf("  A velocity of (1, 1) has length 1.4142 and, normalised, is\n");
    printf("  (0.7071, 0.7071).\n\n");
    code_bad(LINES(BAD_DIAG));
    bad("Moving right and down together adds (s, s): 1.4142 times faster");
    more("than either alone. Build a Vector2 from the keys, then");
    more("Vector2Normalize it and Vector2Scale by the speed.");

    wait_enter();
    clear_screen();
    heading("REFERENCE 3/3: COLLISION");

        ref_head("bool CheckCollisionRecs(Rectangle rec1, Rectangle rec2)");
    ref_line("Does:", "Check collision between two rectangles");
    ref_head("bool CheckCollisionCircles(Vector2 center1, float radius1,\n    Vector2 center2, float radius2)");
    ref_line("Does:", "Check collision between two circles");
    ref_head("bool CheckCollisionCircleRec(Vector2 center, float radius,\n    Rectangle rec)");
    ref_line("Does:", "Check collision between circle and rectangle");
    ref_head("Rectangle GetCollisionRec(Rectangle rec1, Rectangle rec2)");
    ref_line("Does:", "Get collision rectangle for two rectangles collision");
    printf("\n  Measured: rectangles {0,0,10,10} and {10,0,10,10}, which only\n");
    printf("  touch at an edge, do NOT collide. Overlapping by one pixel\n");
    printf("  ({9,0,10,10}) they do. GetCollisionRec of {0,0,10,10} and\n");
    printf("  {6,4,10,10} is {6,4,4,6}, the overlap.\n\n");
    code_bad(LINES(BAD_STICK));
    bad("If the ball is still inside the paddle next frame, it flips again");
    more("and sticks. The example only bounces while moving DOWN. The test");
    more("moved the ball up while inside the paddle: no bounce.");

    wait_enter();
    clear_screen();
    heading("EVERY COLLISION FUNCTION (raylib.h)");

        api_line("bool CheckCollisionCircleLine(Vector2 center, float radius, Vector2 p1,\n    Vector2 p2)", "Check if circle collides with a line created between two points [p1]\n      and [p2]");
    api_line("bool CheckCollisionPointTriangle(Vector2 point, Vector2 p1, Vector2 p2,\n    Vector2 p3)", "Check if point is inside a triangle");
    api_line("bool CheckCollisionPointLine(Vector2 point, Vector2 p1, Vector2 p2,\n    int threshold)", "Check if point belongs to line created between two points [p1] and\n      [p2] with defined margin in pixels [threshold]");
    api_line("bool CheckCollisionPointPoly(Vector2 point, const Vector2 *points,\n    int pointCount)", "Check if point is within a polygon described by array of vertices");
    api_line("bool CheckCollisionLines(Vector2 startPos1, Vector2 endPos1,\n    Vector2 startPos2, Vector2 endPos2, Vector2 *collisionPoint)", "Check the collision between two lines defined by two points each,\n      returns collision point by reference");

    wait_enter();
    clear_screen();
    heading("GOOD PRACTICE");

    tip("Speeds are in units per second. Multiply by GetFrameTime().");
    tip("Put the rules in a function with no drawing, so they can be");
    more("tested and so the screen cannot break them.");
    tip("Normalise a direction before scaling it by a speed.");
    tip("Clamp positions instead of if-chains.");
    tip("Test collisions with the ready-made functions, not by hand.");
    tip("Let a bounce change only the direction that hit something.");

    wait_enter();
    clear_screen();
    exercise(4);

    question("Movement should be speed multiplied by what? (one word)",
             "time",
             "distance = speed * time. In raylib the time is\n"
             "             GetFrameTime(), the seconds the last frame took.");

    question("The vector (1, 1) has what length? (two decimals)",
             "1.41",
             "Measured: 1.4142. Diagonal movement is that much faster\n"
             "             unless the direction is normalised first.");

    question("What does update_ball return when the ball falls off the\n"
             "  bottom? (a number)",
             "-1",
             "1 means it hit the paddle, -1 means it was missed, 0 means\n"
             "             nothing special. This was tested without a window.");

    printf("\n  >> BUILD THIS YOURSELF, in a real file\n");
    rule();
    printf("  Make the ball 5 percent faster every time it hits the\n");
    printf("  paddle. A velocity is a Vector2, so one call scales it.\n\n");

    printf("  There is no transcript to match, the result is a window. You\n");
    printf("  have it right when the ball visibly speeds up as the score\n");
    printf("  rises, and resets when you miss.\n");

    if (ask_yes("Want to see example code?")) {
        printf("\n");
        rule();
        printf("  if (result == 1) {\n");
        printf("      score++;\n");
        printf("      ball.vel = Vector2Scale(ball.vel, 1.05f);\n");
        printf("  }\n");
        rule();
        printf("  Vector2Scale multiplies both parts of the velocity, so the\n");
        printf("  direction is kept. Compiled with no warnings and ran.\n");
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(4)) {
        printf("   - move by speed * GetFrameTime(), never by a fixed\n");
        printf("     amount per frame (93 px against 183 px measured)\n");
        printf("   - normalise a direction before scaling it by a speed\n");
        printf("   - touching edges do not collide; overlapping does\n");
        printf("   - keep the rules in a function with no drawing\n");
    }
    wait_enter();
}

void lesson_05_raygui(void)
{
    title("MODULE 5 - RAYGUI: BUTTONS, TEXT AND STATE");

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
    code(LINES(EXR5_P0));

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 2 OF 3: THE WIDGETS");

    code(LINES(EXR5_P1));

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 3 OF 3: DRAW THE ANSWER");

    code(LINES(EXR5_P2));

    printf("\n  Compile with the line from module 1, changing window.c to\n");
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
    exercise(5);

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

    if (!print_summary_pt(5)) {
        printf("   - a raygui widget is a function call that draws and\n");
        printf("     answers in one go; your variables hold the state\n");
        printf("   - GuiCheckBox takes a pointer; GuiTextBox takes the real\n");
        printf("     buffer size and YOU toggle editMode\n");
        printf("   - TextFormat's result is a reused buffer: use it at once\n");
        printf("   - GuiSetStyle takes ints; ColorToInt packs a Color\n");
    }
    wait_enter();
}

void lesson_06_todo(void)
{
    title("MODULE 6 - RAYGUI: A TODO LIST");

    heading("WHAT YOU WILL HAVE AT THE END");

    printf("  The todo list from the GTK course's module 4, built again\n");
    printf("  with raylib and raygui: a text box, Add, Clear done, a row\n");
    printf("  per task with a tick box and an x, and an \"N left\" line.\n");
    printf("  Same program, the other way of building it.\n");

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
    code(LINES(EXR6_P0));

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 2 OF 3: THE TOP ROW");

    code(LINES(EXR6_P1));

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 3 OF 3: THE ROWS");

    code(LINES(EXR6_P2));

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
    ref_line("", "As in the GTK course, there is no stored counter to keep in");
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
    exercise(6);

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

    if (!print_summary_pt(6)) {
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

