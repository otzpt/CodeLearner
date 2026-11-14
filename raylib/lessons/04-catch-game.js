module.exports = {
  id: "catch-game",
  title: "Project: a catching game",
  titlePt: "Projeto: um jogo de apanhar",
  screens: [
    [
      "Time to put it together: a game where a ball falls from the top, and you move a paddle along the bottom to catch it. Each catch scores a point and drops a new ball; a miss ends the game.",
      "Plan it before writing it. Every frame:\n1. Move the paddle with the arrow keys (lesson 3).\n2. Move the ball down by its speed times the frame time.\n3. If the ball touches the paddle: add a point, start a new ball at a random x.\n4. If the ball falls past the bottom: game over.\n5. Draw everything, and the score.",
    ],
    [
      "Two raylib pieces are new. GetRandomValue(min, max) gives a random whole number, both ends included. CheckCollisionCircleRec tests whether a circle touches a rectangle:",
      { code: '#include <raylib.h>\n\nint main(void)\n{\n    InitWindow(400, 300, "Random");\n    SetTargetFPS(60);\n    Rectangle paddle = {150, 250, 100, 20};\n    Vector2 ball = {(float) GetRandomValue(20, 380), 240};\n    while (!WindowShouldClose()) {\n        bool hit = CheckCollisionCircleRec(ball, 10, paddle);\n        BeginDrawing();\n        ClearBackground(RAYWHITE);\n        DrawRectangleRec(paddle, DARKGRAY);\n        DrawCircleV(ball, 10, hit ? GREEN : RED);\n        EndDrawing();\n    }\n    CloseWindow();\n    return 0;\n}' },
      "The ball turns green when it touches the paddle. DrawCircleV takes the centre as a Vector2.",
      { quiz: "In which order should a frame do its work?", choices: ["draw, then move", "read input and move, then check collisions, then draw"], answer: 2, why: "Draw what the frame's updates produced." },
    ],
    [
      {
        exercise: "game",
        task: "Write the catching game: an 800 by 450 window, a 120 by 20 paddle near the bottom moving at 400 px/s, a ball of radius 12 falling at 200 px/s that speeds up by 20 px/s on each catch, a score in the corner, and a \"Game over\" message (with the score) when a ball is missed.",
        expected: [],
        hints: ["Keep bool over = false; when over, skip the updates and draw the message instead.", "On a catch: score++, ballSpeed += 20, ball.y = 0, ball.x = GetRandomValue(12, 788)."],
        solution: '#include <raylib.h>\n\nint main(void)\n{\n    InitWindow(800, 450, "Catch");\n    SetTargetFPS(60);\n    Rectangle paddle = {340, 410, 120, 20};\n    Vector2 ball = {(float) GetRandomValue(12, 788), 0};\n    float ballSpeed = 200;\n    int score = 0;\n    bool over = false;\n    while (!WindowShouldClose()) {\n        float dt = GetFrameTime();\n        if (!over) {\n            if (IsKeyDown(KEY_RIGHT)) paddle.x += 400 * dt;\n            if (IsKeyDown(KEY_LEFT)) paddle.x -= 400 * dt;\n            if (paddle.x < 0) paddle.x = 0;\n            if (paddle.x > 800 - paddle.width) paddle.x = 800 - paddle.width;\n            ball.y += ballSpeed * dt;\n            if (CheckCollisionCircleRec(ball, 12, paddle)) {\n                score++;\n                ballSpeed += 20;\n                ball.y = 0;\n                ball.x = (float) GetRandomValue(12, 788);\n            } else if (ball.y > 450) {\n                over = true;\n            }\n        }\n        BeginDrawing();\n        ClearBackground(RAYWHITE);\n        DrawRectangleRec(paddle, DARKBLUE);\n        DrawCircleV(ball, 12, MAROON);\n        DrawText(TextFormat("Score: %d", score), 10, 10, 20, BLACK);\n        if (over) {\n            DrawText(TextFormat("Game over: %d", score), 280, 200, 40, RED);\n        }\n        EndDrawing();\n    }\n    CloseWindow();\n    return 0;\n}',
      },
    ],
  ],
  recapPt: [
    "Planeia cada imagem: ler teclas, mexer, verificar colisões, desenhar.",
    "GetRandomValue(min, max) inclui os dois extremos.",
    "CheckCollisionCircleRec testa se um círculo toca num retângulo.",
    "Um bool como over separa o jogo a decorrer do fim do jogo.",
  ],
  recap: [
    "Plan each frame: read keys, move, check collisions, draw.",
    "GetRandomValue(min, max) includes both ends.",
    "CheckCollisionCircleRec tests whether a circle touches a rectangle.",
    "A bool like over separates the running game from the end.",
  ],
};
