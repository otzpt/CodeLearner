const { MAIN } = require("./02-widgets.js");

module.exports = {
  id: "signals",
  title: "Reacting to clicks: signals",
  titlePt: "Reagir a cliques: sinais",
  screens: [
    [
      "A button sends the \"clicked\" signal when pressed. Connect a function to it, the same way main connects activate. The last argument of g_signal_connect is passed to your function as user_data, here the label to change:",
      { setup: "#include <gtk/gtk.h>", setupLabel: "#include <gtk/gtk.h>", code: 'static void on_click(GtkButton *button, gpointer user_data)\n{\n    (void) button;\n    GtkLabel *label = GTK_LABEL(user_data);\n    gtk_label_set_text(label, "Clicked!");\n}\n\nstatic void activate(GtkApplication *app, gpointer user_data)\n{\n    (void) user_data;\n    GtkWidget *window = gtk_application_window_new(app);\n    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);\n    GtkWidget *label = gtk_label_new("Not yet");\n    GtkWidget *button = gtk_button_new_with_label("Click me");\n    g_signal_connect(button, "clicked", G_CALLBACK(on_click), label);\n    gtk_box_append(GTK_BOX(box), label);\n    gtk_box_append(GTK_BOX(box), button);\n    gtk_window_set_child(GTK_WINDOW(window), box);\n    gtk_window_present(GTK_WINDOW(window));\n}' + MAIN },
      "on_click does not run when it is connected, only when the button is clicked. After activate returns, GTK waits for events and calls your functions.",
    ],
    [
      "To keep a number between clicks, make it static: a variable that lives for the whole program, not just one call. g_strdup_printf builds text like printf, and g_free gives its memory back:",
      { setup: "#include <gtk/gtk.h>", setupLabel: "#include <gtk/gtk.h>", code: 'static int clicks = 0;\n\nstatic void on_click(GtkButton *button, gpointer user_data)\n{\n    (void) button;\n    clicks++;\n    char *text = g_strdup_printf("Clicked %d times", clicks);\n    gtk_label_set_text(GTK_LABEL(user_data), text);\n    g_free(text);\n}\n\nstatic void activate(GtkApplication *app, gpointer user_data)\n{\n    (void) user_data;\n    GtkWidget *window = gtk_application_window_new(app);\n    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);\n    GtkWidget *label = gtk_label_new("Clicked 0 times");\n    GtkWidget *button = gtk_button_new_with_label("+1");\n    g_signal_connect(button, "clicked", G_CALLBACK(on_click), label);\n    gtk_box_append(GTK_BOX(box), label);\n    gtk_box_append(GTK_BOX(box), button);\n    gtk_window_set_child(GTK_WINDOW(window), box);\n    gtk_window_present(GTK_WINDOW(window));\n}' + MAIN },
      { quiz: "What does the last argument of g_signal_connect do?", choices: ["sets the signal name", "is passed to your function as user_data"], answer: 2, why: "It is how your function gets the widget it should change." },
    ],
    [
      {
        exercise: "signals",
        task: 'Build a window with a label starting at "0" and two buttons, "+1" and "-1", that change the number shown.',
        expected: [],
        hints: ["Keep static int value = 0; and two functions, one adding and one subtracting.", "Both buttons get the label as user_data; each updates it with g_strdup_printf(\"%d\", value)."],
        solution: '#include <gtk/gtk.h>\n\nstatic int value = 0;\n\nstatic void show(GtkLabel *label)\n{\n    char *text = g_strdup_printf("%d", value);\n    gtk_label_set_text(label, text);\n    g_free(text);\n}\n\nstatic void on_plus(GtkButton *button, gpointer user_data)\n{\n    (void) button;\n    value++;\n    show(GTK_LABEL(user_data));\n}\n\nstatic void on_minus(GtkButton *button, gpointer user_data)\n{\n    (void) button;\n    value--;\n    show(GTK_LABEL(user_data));\n}\n\nstatic void activate(GtkApplication *app, gpointer user_data)\n{\n    (void) user_data;\n    GtkWidget *window = gtk_application_window_new(app);\n    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);\n    GtkWidget *label = gtk_label_new("0");\n    GtkWidget *plus = gtk_button_new_with_label("+1");\n    GtkWidget *minus = gtk_button_new_with_label("-1");\n    g_signal_connect(plus, "clicked", G_CALLBACK(on_plus), label);\n    g_signal_connect(minus, "clicked", G_CALLBACK(on_minus), label);\n    gtk_box_append(GTK_BOX(box), label);\n    gtk_box_append(GTK_BOX(box), plus);\n    gtk_box_append(GTK_BOX(box), minus);\n    gtk_window_set_child(GTK_WINDOW(window), box);\n    gtk_window_present(GTK_WINDOW(window));\n}' + MAIN.replace("demo", "practice"),
      },
    ],
  ],
  recapPt: [
    "Um botão envia o sinal \"clicked\"; g_signal_connect liga-o a uma função tua.",
    "O último argumento chega à função como user_data.",
    "A função só corre no clique; depois do activate, o GTK espera por eventos.",
    "static guarda um valor entre chamadas; g_strdup_printf e g_free constroem texto.",
  ],
  recap: [
    "A button sends \"clicked\"; g_signal_connect connects it to your function.",
    "The last argument reaches the function as user_data.",
    "The function only runs on a click; after activate, GTK waits for events.",
    "static keeps a value between calls; g_strdup_printf and g_free build text.",
  ],
};
