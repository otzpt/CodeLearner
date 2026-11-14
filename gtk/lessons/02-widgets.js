const MAIN = '\n\nint main(int argc, char **argv)\n{\n    GtkApplication *app = gtk_application_new("org.example.demo", G_APPLICATION_DEFAULT_FLAGS);\n    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);\n    int status = g_application_run(G_APPLICATION(app), argc, argv);\n    g_object_unref(app);\n    return status;\n}';

module.exports = {
  MAIN,
  id: "widgets",
  title: "Widgets and boxes",
  titlePt: "Widgets e caixas",
  screens: [
    [
      "Everything you see in a window is a widget: a label, a button, a text box. A window holds one child. To show several, put them in a GtkBox, which lines its children up vertically or horizontally:",
      { setup: "#include <gtk/gtk.h>", setupLabel: "#include <gtk/gtk.h>", code: 'static void activate(GtkApplication *app, gpointer user_data)\n{\n    (void) user_data;\n    GtkWidget *window = gtk_application_window_new(app);\n    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);\n    gtk_box_append(GTK_BOX(box), gtk_label_new("Hello!"));\n    gtk_box_append(GTK_BOX(box), gtk_button_new_with_label("Click me"));\n    gtk_window_set_child(GTK_WINDOW(window), box);\n    gtk_window_present(GTK_WINDOW(window));\n}' + MAIN },
      "8 is the gap between children, in pixels. The main is the same as lesson 1.",
    ],
    [
      "Boxes go inside boxes. A horizontal box inside a vertical one makes a row; margins add space around a widget:",
      { setup: "#include <gtk/gtk.h>", setupLabel: "#include <gtk/gtk.h>", code: 'static void activate(GtkApplication *app, gpointer user_data)\n{\n    (void) user_data;\n    GtkWidget *window = gtk_application_window_new(app);\n    GtkWidget *column = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);\n    GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);\n    gtk_widget_set_margin_start(column, 12);\n    gtk_widget_set_margin_top(column, 12);\n    gtk_box_append(GTK_BOX(column), gtk_label_new("Pick one:"));\n    gtk_box_append(GTK_BOX(row), gtk_button_new_with_label("Yes"));\n    gtk_box_append(GTK_BOX(row), gtk_button_new_with_label("No"));\n    gtk_box_append(GTK_BOX(column), row);\n    gtk_window_set_child(GTK_WINDOW(window), column);\n    gtk_window_present(GTK_WINDOW(window));\n}' + MAIN },
      { quiz: "A window needs a label and two buttons. What holds them?", choices: ["the window, directly", "a GtkBox, set as the window's child"], answer: 2, why: "A window has one child; a box holds many." },
    ],
    [
      {
        exercise: "widgets",
        task: 'Build a window with a vertical box holding a label "Shopping list" and three buttons: "Milk", "Bread" and "Eggs".',
        expected: [],
        hints: ["GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);", 'gtk_box_append(GTK_BOX(box), gtk_button_new_with_label("Milk")); and so on, then gtk_window_set_child.'],
        solution: '#include <gtk/gtk.h>\n\nstatic void activate(GtkApplication *app, gpointer user_data)\n{\n    (void) user_data;\n    GtkWidget *window = gtk_application_window_new(app);\n    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);\n    gtk_box_append(GTK_BOX(box), gtk_label_new("Shopping list"));\n    gtk_box_append(GTK_BOX(box), gtk_button_new_with_label("Milk"));\n    gtk_box_append(GTK_BOX(box), gtk_button_new_with_label("Bread"));\n    gtk_box_append(GTK_BOX(box), gtk_button_new_with_label("Eggs"));\n    gtk_window_set_child(GTK_WINDOW(window), box);\n    gtk_window_present(GTK_WINDOW(window));\n}' + MAIN.replace("demo", "practice"),
      },
    ],
  ],
  recapPt: [
    "Tudo o que aparece numa janela é um widget.",
    "Uma janela tem um filho; uma GtkBox alinha vários na vertical ou na horizontal.",
    "gtk_box_append junta um widget; gtk_window_set_child põe a caixa na janela.",
  ],
  recap: [
    "Everything shown in a window is a widget.",
    "A window has one child; a GtkBox lines up many, vertically or horizontally.",
    "gtk_box_append adds a widget; gtk_window_set_child puts the box in the window.",
  ],
};
