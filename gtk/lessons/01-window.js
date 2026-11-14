module.exports = {
  id: "window",
  title: "A window",
  titlePt: "Uma janela",
  screens: [
    [
      "GTK is a toolkit for desktop programs with windows, buttons and text boxes: the GNOME desktop and many Linux apps are built with it. This course uses GTK 4 from C. It assumes you know C's basics: variables, functions and pointers (the C course covers them).",
      "A terminal cannot show a window, so each example here is built with the real GTK, and you run it yourself to see the window. To build and run one:",
      { code: "cc -o program main.c $(pkg-config --cflags --libs gtk4)\n./program", run: false },
    ],
    [
      "A GTK program is a GtkApplication. When it starts, it sends the \"activate\" signal, and your function connected to that signal builds the window:",
      { code: '#include <gtk/gtk.h>\n\nstatic void activate(GtkApplication *app, gpointer user_data)\n{\n    (void) user_data;\n    GtkWidget *window = gtk_application_window_new(app);\n    gtk_window_set_title(GTK_WINDOW(window), "Hello");\n    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);\n    gtk_window_present(GTK_WINDOW(window));\n}\n\nint main(int argc, char **argv)\n{\n    GtkApplication *app = gtk_application_new("org.example.hello", G_APPLICATION_DEFAULT_FLAGS);\n    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);\n    int status = g_application_run(G_APPLICATION(app), argc, argv);\n    g_object_unref(app);\n    return status;\n}' },
      "Every GTK program in this course has this same main. Only activate changes.",
    ],
    [
      "Line by line: gtk_application_new names the app (an id like a reversed web address). g_signal_connect says \"when app sends activate, call my activate function\". g_application_run starts it and only returns when the last window closes.",
      "GTK_WINDOW(window) is a cast: window is a general GtkWidget pointer, and the window functions want it as a GtkWindow. (void) user_data stops the unused-parameter warning.",
      { quiz: "When does activate run?", choices: ["when main starts", "when the application sends the activate signal"], answer: 2, why: "g_application_run sends it after starting." },
    ],
    [
      {
        exercise: "window",
        task: 'Change activate so the window is titled "Notes" and is 600 by 400.',
        expected: [],
        hints: ['gtk_window_set_title(GTK_WINDOW(window), "Notes");', "gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);"],
        solution: '#include <gtk/gtk.h>\n\nstatic void activate(GtkApplication *app, gpointer user_data)\n{\n    (void) user_data;\n    GtkWidget *window = gtk_application_window_new(app);\n    gtk_window_set_title(GTK_WINDOW(window), "Notes");\n    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);\n    gtk_window_present(GTK_WINDOW(window));\n}\n\nint main(int argc, char **argv)\n{\n    GtkApplication *app = gtk_application_new("org.example.practice", G_APPLICATION_DEFAULT_FLAGS);\n    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);\n    int status = g_application_run(G_APPLICATION(app), argc, argv);\n    g_object_unref(app);\n    return status;\n}',
      },
    ],
  ],
  recapPt: [
    "Um programa GTK é uma GtkApplication; o sinal activate constrói a janela.",
    "g_signal_connect liga um sinal a uma função tua.",
    "gtk_window_set_title e gtk_window_set_default_size configuram a janela; gtk_window_present mostra-a.",
    "Compila com $(pkg-config --cflags --libs gtk4).",
  ],
  recap: [
    "A GTK program is a GtkApplication; the activate signal builds the window.",
    "g_signal_connect connects a signal to a function of yours.",
    "gtk_window_set_title and gtk_window_set_default_size set it up; gtk_window_present shows it.",
    "Build with $(pkg-config --cflags --libs gtk4).",
  ],
};
