// How the GTK course builds code: C with GTK 4, the toolkit of the GNOME desktop. Its programs open a window,
// which this course cannot show, so it only builds them (buildOnly) and you
// run them yourself. Build flags come from pkg-config.

const { spawnSync } = require("node:child_process");
const c = require("../c/course.js");

const WINDOWS = process.platform === "win32";

function pkg(args) {
  const r = spawnSync("pkg-config", args, { encoding: "utf8" });
  return r.status === 0 ? r.stdout.trim().split(/\s+/).filter(Boolean) : null;
}

const CFLAGS = pkg(["--cflags", "gtk4"]);
const LIBS = pkg(["--libs", "gtk4"]);

module.exports = {
  ...c,
  id: "gtk",
  title: "GTK FROM ZERO",
  titlePt: "GTK DO ZERO",
  file: "main.c",
  buildOnly: true,
  unavailable: () => (CFLAGS && LIBS ? null : "gtk4 is not installed (pkg-config cannot find it)"),
  starter: "#include <gtk/gtk.h>\n\nstatic void activate(GtkApplication *app, gpointer user_data)\n{\n    (void) user_data;\n    GtkWidget *window = gtk_application_window_new(app);\n    // your code here\n    gtk_window_present(GTK_WINDOW(window));\n}\n\nint main(int argc, char **argv)\n{\n    GtkApplication *app = gtk_application_new(\"org.example.practice\", G_APPLICATION_DEFAULT_FLAGS);\n    g_signal_connect(app, \"activate\", G_CALLBACK(activate), NULL);\n    int status = g_application_run(G_APPLICATION(app), argc, argv);\n    g_object_unref(app);\n    return status;\n}",
  steps: (file) => [
    [process.env.CC || (WINDOWS ? "gcc" : "cc"), "-std=c17", "-Wall", "-Wextra", ...(CFLAGS || []), "-o", WINDOWS ? "program.exe" : "program", file, ...(LIBS || []), "-lm"],
    [WINDOWS ? "program.exe" : "./program"],
  ],
  howToRun: (name) => `cc -o program ${name} $(pkg-config --cflags --libs gtk4) && ./program`,
};
