module.exports = {
  id: "shapes",
  title: "Shapes and colours",
  titlePt: "Formas e cores",
  screens: [
    [
      "raylib draws rectangles, circles and lines. Positions are in pixels from the top-left corner:",
      { code: '#include <raylib.h>\n\nint main(void)\n{\n    InitWindow(800, 450, "Shapes");\n    SetTargetFPS(60);\n    while (!WindowShouldClose()) {\n        BeginDrawing();\n        ClearBackground(RAYWHITE);\n        DrawRectangle(50, 50, 200, 100, SKYBLUE);\n        DrawCircle(400, 200, 60, RED);\n        DrawLine(0, 400, 800, 400, DARKGREEN);\n        DrawRectangleLines(550, 50, 150, 150, BLACK);\n        EndDrawing();\n    }\n    CloseWindow();\n    return 0;\n}' },
      "DrawRectangle takes x, y, width, height; DrawCircle takes the centre and the radius. The ...Lines versions draw only the outline.",
    ],
    [
      "A colour is a Color: red, green, blue and alpha (opacity), each 0 to 255. raylib has names for common ones; you can also make your own. Shapes drawn later go on top:",
      { code: '#include <raylib.h>\n\nint main(void)\n{\n    Color orange = {255, 140, 0, 255};\n    Color glass = {0, 120, 255, 100};\n    InitWindow(800, 450, "Colours");\n    SetTargetFPS(60);\n    while (!WindowShouldClose()) {\n        BeginDrawing();\n        ClearBackground(RAYWHITE);\n        DrawRectangle(100, 100, 300, 200, orange);\n        DrawRectangle(250, 180, 300, 200, glass);\n        EndDrawing();\n    }\n    CloseWindow();\n    return 0;\n}' },
      "alpha 100 out of 255 makes the second rectangle see-through, so the orange shows under it.",
      { quiz: "Two shapes overlap. Which is on top?", choices: ["the one drawn first", "the one drawn last"], answer: 2, why: "Each draw paints over what is already there." },
    ],
    [
      {
        exercise: "shapes",
        task: "Draw a simple house: a brown square body (Color {139, 69, 19, 255}) at (300, 200) sized 200 by 150, a red triangle roof with DrawTriangle using the points (400, 100), (300, 200) and (500, 200), in that order (counter-clockwise on screen), and a yellow sun circle at (650, 80) with radius 40.",
        expected: [],
        hints: ["DrawTriangle takes three Vector2 points: (Vector2){x, y}.", "raylib draws a triangle only when its points go counter-clockwise on screen: top, then left, then right."],
        solution: '#include <raylib.h>\n\nint main(void)\n{\n    Color brown = {139, 69, 19, 255};\n    InitWindow(800, 450, "House");\n    SetTargetFPS(60);\n    while (!WindowShouldClose()) {\n        BeginDrawing();\n        ClearBackground(SKYBLUE);\n        DrawRectangle(300, 200, 200, 150, brown);\n        DrawTriangle((Vector2){400, 100}, (Vector2){300, 200}, (Vector2){500, 200}, RED);\n        DrawCircle(650, 80, 40, YELLOW);\n        EndDrawing();\n    }\n    CloseWindow();\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "DrawRectangle(x, y, largura, altura, cor), DrawCircle(cx, cy, raio, cor), DrawLine.",
    "Uma cor tem vermelho, verde, azul e alfa (opacidade), de 0 a 255.",
    "O que se desenha depois fica por cima.",
    "DrawTriangle quer os pontos no sentido contrário aos ponteiros do relógio.",
  ],
  recap: [
    "DrawRectangle(x, y, width, height, colour), DrawCircle(cx, cy, radius, colour), DrawLine.",
    "A colour has red, green, blue and alpha (opacity), 0 to 255.",
    "What is drawn later goes on top.",
    "DrawTriangle wants its points counter-clockwise.",
  ],
};
