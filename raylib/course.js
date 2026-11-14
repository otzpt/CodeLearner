// How the raylib course builds code: C with raylib, a small library for games. Its programs open a window,
// which this course cannot show, so it only builds them (buildOnly) and you
// run them yourself. Build flags come from pkg-config.

const { spawnSync } = require("node:child_process");
const c = require("../c/course.js");

const WINDOWS = process.platform === "win32";

function pkg(args) {
  const r = spawnSync("pkg-config", args, { encoding: "utf8" });
  return r.status === 0 ? r.stdout.trim().split(/\s+/).filter(Boolean) : null;
}

const CFLAGS = pkg(["--cflags", "raylib"]);
const LIBS = pkg(["--libs", "raylib"]);

module.exports = {
  ...c,
  id: "raylib",
  title: "RAYLIB FROM ZERO",
  titlePt: "RAYLIB DO ZERO",
  file: "main.c",
  buildOnly: true,
  unavailable: () => (CFLAGS && LIBS ? null : "raylib is not installed (pkg-config cannot find it)"),
  starter: "#include <raylib.h>\n\nint main(void)\n{\n    InitWindow(800, 450, \"practice\");\n    SetTargetFPS(60);\n    while (!WindowShouldClose()) {\n        BeginDrawing();\n        ClearBackground(RAYWHITE);\n        // your code here\n        EndDrawing();\n    }\n    CloseWindow();\n    return 0;\n}",
  steps: (file) => [
    [process.env.CC || (WINDOWS ? "gcc" : "cc"), "-std=c17", "-Wall", "-Wextra", ...(CFLAGS || []), "-o", WINDOWS ? "program.exe" : "program", file, ...(LIBS || []), "-lm"],
    [WINDOWS ? "program.exe" : "./program"],
  ],
  howToRun: (name) => `cc -o program ${name} $(pkg-config --cflags --libs raylib) && ./program`,
};
