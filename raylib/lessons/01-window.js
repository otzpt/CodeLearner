module.exports = {
  id: "window",
  title: "A window and the game loop",
  titlePt: "Uma janela e o ciclo do jogo",
  screens: [
    [
      "raylib is a small C library for games and graphics: open a window, draw shapes and text, read the keyboard and mouse. It assumes you know C's basics (the C course covers them).",
      "A terminal cannot show a window, so each example is built with the real raylib, and you run it yourself. To build and run one:",
      { code: "cc -o program main.c $(pkg-config --cflags --libs raylib)\n./program", run: false },
    ],
    [
      "Every raylib program has the same shape: open a window, then repeat a loop until it is closed. Each time round the loop draws one frame, from scratch:",
      { code: '#include <raylib.h>\n\nint main(void)\n{\n    InitWindow(800, 450, "Hello");\n    SetTargetFPS(60);\n    while (!WindowShouldClose()) {\n        BeginDrawing();\n        ClearBackground(RAYWHITE);\n        DrawText("Hello, raylib!", 280, 200, 30, DARKGRAY);\n        EndDrawing();\n    }\n    CloseWindow();\n    return 0;\n}' },
    ],
    [
      "InitWindow opens an 800 by 450 window. SetTargetFPS(60) makes the loop run 60 times a second. WindowShouldClose turns true when you press ESC or close the window.",
      "Drawing happens between BeginDrawing and EndDrawing. ClearBackground wipes the last frame. DrawText takes the text, x and y in pixels, a size and a colour. x grows to the right and y grows downwards: (0, 0) is the top-left corner.",
      { quiz: "Where is the point (0, 0) on the screen?", choices: ["the centre", "the top-left corner", "the bottom-left corner"], answer: 2, why: "y grows downwards from the top." },
    ],
    [
      {
        exercise: "window",
        task: 'Open a 640 by 360 window titled "My game", with a black background, and the text "Press ESC to quit" in white, size 20, at (20, 20).',
        expected: [],
        hints: ['InitWindow(640, 360, "My game");', 'ClearBackground(BLACK); DrawText("Press ESC to quit", 20, 20, 20, WHITE);'],
        solution: '#include <raylib.h>\n\nint main(void)\n{\n    InitWindow(640, 360, "My game");\n    SetTargetFPS(60);\n    while (!WindowShouldClose()) {\n        BeginDrawing();\n        ClearBackground(BLACK);\n        DrawText("Press ESC to quit", 20, 20, 20, WHITE);\n        EndDrawing();\n    }\n    CloseWindow();\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "InitWindow abre a janela; o ciclo repete até WindowShouldClose dar verdadeiro.",
    "Cada volta desenha uma imagem nova entre BeginDrawing e EndDrawing.",
    "(0, 0) é o canto de cima à esquerda; o y cresce para baixo.",
    "Compila com $(pkg-config --cflags --libs raylib).",
  ],
  recap: [
    "InitWindow opens the window; the loop repeats until WindowShouldClose is true.",
    "Each pass draws a fresh frame between BeginDrawing and EndDrawing.",
    "(0, 0) is the top-left corner; y grows downwards.",
    "Build with $(pkg-config --cflags --libs raylib).",
  ],
};
