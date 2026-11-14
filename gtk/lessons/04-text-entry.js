const { MAIN } = require("./02-widgets.js");

module.exports = {
  id: "text-entry",
  title: "Reading typed text: GtkEntry",
  titlePt: "Ler texto escrito: GtkEntry",
  screens: [
    [
      "A GtkEntry is a one-line text box. gtk_editable_get_text gives back what is typed in it. A function often needs two widgets, the entry and the label, so they go in a small struct passed as user_data:",
      { setup: "#include <gtk/gtk.h>", setupLabel: "#include <gtk/gtk.h>", code: 'typedef struct {\n    GtkWidget *entry;\n    GtkWidget *label;\n} Form;\n\nstatic Form form;\n\nstatic void on_greet(GtkButton *button, gpointer user_data)\n{\n    (void) button;\n    Form *f = user_data;\n    const char *name = gtk_editable_get_text(GTK_EDITABLE(f->entry));\n    char *text = g_strdup_printf("Hello, %s!", name);\n    gtk_label_set_text(GTK_LABEL(f->label), text);\n    g_free(text);\n}\n\nstatic void activate(GtkApplication *app, gpointer user_data)\n{\n    (void) user_data;\n    GtkWidget *window = gtk_application_window_new(app);\n    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);\n    form.entry = gtk_entry_new();\n    form.label = gtk_label_new("");\n    GtkWidget *button = gtk_button_new_with_label("Greet");\n    g_signal_connect(button, "clicked", G_CALLBACK(on_greet), &form);\n    gtk_box_append(GTK_BOX(box), form.entry);\n    gtk_box_append(GTK_BOX(box), button);\n    gtk_box_append(GTK_BOX(box), form.label);\n    gtk_window_set_child(GTK_WINDOW(window), box);\n    gtk_window_present(GTK_WINDOW(window));\n}' + MAIN },
      "form is static, so it still exists when the click comes, long after activate returned. A local struct in activate would already be gone.",
    ],
    [
      "The text is always a string. To use it as a number, convert it, and check it: atoi would turn \"hello\" into 0 without complaint. strtol says where it stopped reading, so you can tell whether the whole text was a number. The full exercise below does it.",
      { quiz: "Why is form static rather than a local variable in activate?", choices: ["static is faster", "the click happens after activate has returned, so a local would be gone"], answer: 2, why: "The callback runs later; its data must still exist." },
    ],
    [
      {
        exercise: "entry",
        task: 'Build a window with an entry, a button "Double" and a label. On click, read the entry; if it is a whole number, show twice its value, otherwise show "Not a number".',
        expected: [],
        hints: ["char *end; long n = strtol(text, &end, 10); the text was a number if end != text && *end == '\\0'.", 'g_strdup_printf("%ld", n * 2) for the label.'],
        solution: '#include <gtk/gtk.h>\n#include <stdlib.h>\n\ntypedef struct {\n    GtkWidget *entry;\n    GtkWidget *label;\n} Form;\n\nstatic Form form;\n\nstatic void on_double(GtkButton *button, gpointer user_data)\n{\n    (void) button;\n    Form *f = user_data;\n    const char *text = gtk_editable_get_text(GTK_EDITABLE(f->entry));\n    char *end;\n    long n = strtol(text, &end, 10);\n    if (end == text || *end != \'\\0\') {\n        gtk_label_set_text(GTK_LABEL(f->label), "Not a number");\n        return;\n    }\n    char *result = g_strdup_printf("%ld", n * 2);\n    gtk_label_set_text(GTK_LABEL(f->label), result);\n    g_free(result);\n}\n\nstatic void activate(GtkApplication *app, gpointer user_data)\n{\n    (void) user_data;\n    GtkWidget *window = gtk_application_window_new(app);\n    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);\n    form.entry = gtk_entry_new();\n    form.label = gtk_label_new("");\n    GtkWidget *button = gtk_button_new_with_label("Double");\n    g_signal_connect(button, "clicked", G_CALLBACK(on_double), &form);\n    gtk_box_append(GTK_BOX(box), form.entry);\n    gtk_box_append(GTK_BOX(box), button);\n    gtk_box_append(GTK_BOX(box), form.label);\n    gtk_window_set_child(GTK_WINDOW(window), box);\n    gtk_window_present(GTK_WINDOW(window));\n}' + MAIN.replace("demo", "practice"),
      },
    ],
  ],
  recapPt: [
    "GtkEntry é uma caixa de texto; gtk_editable_get_text lê o que lá está.",
    "Passa vários widgets à função numa struct, via user_data.",
    "Os dados da função têm de existir quando o clique chega: static, não local.",
    "strtol converte texto em número e diz se o texto era mesmo um número.",
  ],
  recap: [
    "GtkEntry is a text box; gtk_editable_get_text reads what is in it.",
    "Pass several widgets to the function in a struct, through user_data.",
    "The function's data must still exist when the click comes: static, not local.",
    "strtol turns text into a number and tells you whether it really was one.",
  ],
};
