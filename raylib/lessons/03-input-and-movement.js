module.exports = {
  id: "input-and-movement",
  title: "Keys, mouse and movement",
  titlePt: "Teclas, rato e movimento",
  screens: [
    [
      "IsKeyDown(KEY_RIGHT) is true for every frame the key is held; IsKeyPressed is true only on the frame it went down. Move by speed times GetFrameTime(), the seconds since the last frame, so the speed is the same on fast and slow computers:",
      { code: '#include <raylib.h>\n\nint main(void)\n{\n    InitWindow(800, 450, "Move");\n    SetTargetFPS(60);\n    float x = 400, y = 225;\n    const float speed = 200;   // pixels per second\n    while (!WindowShouldClose()) {\n        float dt = GetFrameTime();\n        if (IsKeyDown(KEY_RIGHT)) x += speed * dt;\n        if (IsKeyDown(KEY_LEFT)) x -= speed * dt;\n        if (IsKeyDown(KEY_DOWN)) y += speed * dt;\n        if (IsKeyDown(KEY_UP)) y -= speed * dt;\n        BeginDrawing();\n        ClearBackground(RAYWHITE);\n        DrawCircle((int) x, (int) y, 20, MAROON);\n        EndDrawing();\n    }\n    CloseWindow();\n    return 0;\n}' },
      "Moving a fixed 3 pixels per frame instead would be twice as fast on a machine drawing twice as many frames.",
    ],
    [
      "GetMousePosition gives a Vector2 with x and y. IsMouseButtonPressed tells you about clicks. CheckCollisionPointRec tests whether a point is inside a rectangle, which is how buttons are made:",
      { code: '#include <raylib.h>\n\nint main(void)\n{\n    InitWindow(800, 450, "Click");\n    SetTargetFPS(60);\n    Rectangle button = {300, 180, 200, 80};\n    int clicks = 0;\n    while (!WindowShouldClose()) {\n        Vector2 mouse = GetMousePosition();\n        bool over = CheckCollisionPointRec(mouse, button);\n        if (over && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) clicks++;\n        BeginDrawing();\n        ClearBackground(RAYWHITE);\n        DrawRectangleRec(button, over ? GOLD : LIGHTGRAY);\n        DrawText(TextFormat("Clicks: %d", clicks), 320, 210, 20, BLACK);\n        EndDrawing();\n    }\n    CloseWindow();\n    return 0;\n}' },
      "TextFormat builds text like printf. Its result is only valid until the next call, so use it straight away.",
      { quiz: "Why multiply movement by GetFrameTime()?", choices: ["to slow everything down", "so the speed is the same whatever the frame rate"], answer: 2, why: "It turns per-second speed into per-frame movement." },
    ],
    [
      {
        exercise: "movement",
        task: "Draw a 50 by 50 square that moves left and right with the arrow keys at 300 pixels per second, and cannot leave the window (x stays between 0 and 800 - 50). Show its x with TextFormat in the top-left corner.",
        expected: [],
        hints: ["After moving: if (x < 0) x = 0; if (x > 750) x = 750;", 'DrawText(TextFormat("x = %.0f", x), 10, 10, 20, BLACK);'],
        solution: '#include <raylib.h>\n\nint main(void)\n{\n    InitWindow(800, 450, "Square");\n    SetTargetFPS(60);\n    float x = 375;\n    while (!WindowShouldClose()) {\n        float dt = GetFrameTime();\n        if (IsKeyDown(KEY_RIGHT)) x += 300 * dt;\n        if (IsKeyDown(KEY_LEFT)) x -= 300 * dt;\n        if (x < 0) x = 0;\n        if (x > 750) x = 750;\n        BeginDrawing();\n        ClearBackground(RAYWHITE);\n        DrawRectangle((int) x, 200, 50, 50, DARKBLUE);\n        DrawText(TextFormat("x = %.0f", x), 10, 10, 20, BLACK);\n        EndDrawing();\n    }\n    CloseWindow();\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "IsKeyDown enquanto a tecla está carregada; IsKeyPressed só no instante em que desce.",
    "Move com velocidade * GetFrameTime() para a velocidade não depender do computador.",
    "GetMousePosition, IsMouseButtonPressed e CheckCollisionPointRec fazem botões.",
    "TextFormat constrói texto como o printf.",
  ],
  recap: [
    "IsKeyDown while a key is held; IsKeyPressed only on the frame it goes down.",
    "Move by speed * GetFrameTime() so speed does not depend on the computer.",
    "GetMousePosition, IsMouseButtonPressed and CheckCollisionPointRec make buttons.",
    "TextFormat builds text like printf.",
  ],
};
