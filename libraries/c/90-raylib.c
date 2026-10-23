/*
 * TITLE: raylib: a window, drawing and the keyboard (games)
 * GROUP: Games
 * USES: #include <raylib.h>   (Arch: pacman -S raylib)
 * SUMMARY: raylib opens a window and draws shapes, text and images in it with
 *   one call each. A game is one loop: read the keys, move things, then draw
 *   the frame between BeginDrawing() and EndDrawing().
 * NOTES:
 *   - Not part of libc: install it first. Arch has it in extra (raylib).
 *     Debian has it only in testing and unstable. Elsewhere, build it from
 *     github.com/raysan5/raylib. Without it this page does not compile.
 *   - Coordinates are pixels, (x, y), with (0, 0) at the top-left and y
 *     growing downward. For a grid game, multiply cells by a cell size.
 *   - EndDrawing() shows the frame, waits to keep SetTargetFPS(), and reads
 *     new input. Keys only change once per frame.
 *   - IsKeyPressed() is true on the one frame a key goes down. IsKeyDown() is
 *     true while it is held. GetKeyPressed() returns queued keys, 0 when empty.
 *   - The FPS sets how often the loop runs. To move a game piece slower than
 *     that, move it every N frames or use GetFrameTime() (seconds per frame).
 *   - ESC closes the window by default (WindowShouldClose() turns true).
 *     SetExitKey(KEY_NULL) turns that off.
 *   - InitWindow() prints a page of INFO lines. SetTraceLogLevel(LOG_WARNING)
 *     before it keeps only warnings and errors.
 *   - With no display (ssh, a CI runner) the window fails to open:
 *     IsWindowReady() is false and nothing else may be called.
 *   - The Arch package draws through X11 (GLFW). Under Wayland it runs on
 *     XWayland, which works without any change.
 *   - This page hides its window (FLAG_WINDOW_HIDDEN) and stops after 30
 *     frames so the browser can run it. A game leaves both out.
 * SEE: /usr/include/raylib.h, https://www.raylib.com/cheatsheet/cheatsheet.html
 * LIBS: -lraylib -lm
 * PROVIDES:
 *   InitWindow(w, h, title)       open the window, once
 *   WindowShouldClose()           true after ESC or the close button
 *   SetTargetFPS(fps)             frames per second of the loop
 *   BeginDrawing(), EndDrawing()  start and show one frame
 *   ClearBackground(colour)       fill the frame
 *   DrawRectangle(x, y, w, h, c)  a filled rectangle
 *   DrawText(text, x, y, size, c) text with the default font
 *   IsKeyPressed(KEY_UP)          the key went down this frame
 *   CloseWindow()                 close it and free everything
 */

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#define CELL 20
#define COLUMNS 30
#define ROWS 20

int main(void)
{
    printf("raylib version: %s\n", RAYLIB_VERSION);

    if (getenv("DISPLAY") == NULL && getenv("WAYLAND_DISPLAY") == NULL) {
        printf("window: not available (no display)\n");
        return 0;
    }

    SetTraceLogLevel(LOG_WARNING);
    SetConfigFlags(FLAG_WINDOW_HIDDEN);
    InitWindow(COLUMNS * CELL, ROWS * CELL, "raylib page");
    if (!IsWindowReady()) {
        printf("window: not available (it did not open)\n");
        return 0;
    }
    SetTargetFPS(60);

    int x = 0, y = ROWS / 2; // a square on the grid, in cells
    int frames = 0;

    while (!WindowShouldClose() && frames < 30) {
        if (IsKeyPressed(KEY_RIGHT)) x++;
        if (IsKeyPressed(KEY_LEFT))  x--;
        if (frames % 5 == 0) x++;    // also move one cell every 5 frames

        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangle(x * CELL, y * CELL, CELL, CELL, GREEN);
        DrawText("arrows to move", 10, 10, 20, RAYWHITE);
        EndDrawing();
        frames++;
    }

    int monitor = GetCurrentMonitor();
    printf("window: %d x %d pixels (%d x %d cells of %d)\n",
           GetScreenWidth(), GetScreenHeight(), COLUMNS, ROWS, CELL);
    printf("monitor: %s at %d Hz\n", GetMonitorName(monitor), GetMonitorRefreshRate(monitor));
    printf("frames drawn: %d, %.2f s since InitWindow (target 60 per second)\n", frames, GetTime());
    printf("square ended at cell (%d, %d), pixel (%d, %d)\n", x, y, x * CELL, y * CELL);

    CloseWindow();
    return 0;
}
