/*
 * Modules 2-4 - GTK: layout and widgets, signals and state, and widgets
 * that come and go.
 *
 * Same rule as module 1 (lessons_gtk.c): the lesson program itself is plain
 * C, and the GTK code it teaches is compiled separately by the student.
 * Every example printed here is a file that was compiled and run, not typed
 * from memory:
 *
 *   gcc -std=c11 -Wall -Wextra $(pkg-config --cflags gtk4) x.c -o x $(pkg-config --libs gtk4)
 *
 * gcc 14.2.1, GTK 4.22.5. All of them compiled with no warnings. Beyond
 * "it compiled", each one was driven by a small test program that included
 * the example, opened the real window, set entry text, ticked check
 * buttons, emitted "clicked" and "activate", and asserted on the labels and
 * the number of rows. The two layout numbers in module 2 and the scroller
 * height in module 4 are widths and heights read back from the real
 * widgets on the author's machine, so they will differ with another theme
 * or font; the lessons say so where they quote them.
 *
 * Not demonstrated, on purpose, because they are undefined behaviour: a
 * callback reading a dead stack variable (module 3) and reading a
 * widget's sibling after the widget was removed (module 4).
 */

#include <stdio.h>

#include "lessons.h"
#include "strings_pt.h"
#include "ui.h"

void lesson_02_layout(void)
{
    title("MODULE 2 - GTK: LAYOUT AND THE COMMON WIDGETS");

    heading("A WINDOW HAS ONE CHILD");

    printf("  Module 1 ended on a limit: gtk_window_set_child() takes one\n");
    printf("  widget. A real window has a label, a text field, a few\n");
    printf("  buttons. The answer is a container that is itself one\n");
    printf("  widget and holds many: GtkBox lays its children out in a\n");
    printf("  line, vertical or horizontal.\n\n");

    printf("    GtkWidget *column = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);\n");
    printf("    gtk_box_append(GTK_BOX(column), label);\n");
    printf("    gtk_box_append(GTK_BOX(column), entry);\n");
    printf("    gtk_window_set_child(GTK_WINDOW(window), column);\n\n");

    printf("  The 8 is the gap in pixels between children. Children appear\n");
    printf("  in the order they were appended. A row of buttons inside a\n");
    printf("  column is a horizontal box appended to the vertical one --\n");
    printf("  boxes nest, and that is how every layout in this course is\n");
    printf("  built.\n\n");

    printf("  The window itself has no padding setting. Space around the\n");
    printf("  edge is a margin on the outermost box, one call per side:\n\n");
    printf("    gtk_widget_set_margin_top(column, 12);\n");
    printf("    gtk_widget_set_margin_bottom(column, 12);\n");
    printf("    gtk_widget_set_margin_start(column, 12);\n");
    printf("    gtk_widget_set_margin_end(column, 12);\n");

    wait_enter();
    clear_screen();
    heading("PART 1: GTK3 tutorials do not compile");

    printf("  Search for \"GTK box example\" and most results are GTK3. Their\n");
    printf("  calls were removed in GTK4. This file, written the GTK3 way:\n\n");

    rule();
    printf("  #include <gtk/gtk.h>\n");
    printf("  \n");
    printf("  static void activate(GtkApplication *app, gpointer user_data)\n");
    printf("  {\n");
    printf("      (void) user_data;\n");
    printf("      GtkWidget *window = gtk_application_window_new(app);\n");
    printf("      GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);\n");
    printf("      GtkWidget *button = gtk_button_new_with_label(\"Hi\");\n");
    printf("      gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);\n");
    printf("      gtk_container_add(GTK_CONTAINER(window), box);\n");
    printf("      gtk_widget_show_all(window);\n");
    printf("  }\n");
    rule();

    printf("\n  gives, against GTK 4.22.5 with gcc 14.2.1 (-Wall -Wextra):\n\n");

    printf("    error: implicit declaration of function 'gtk_box_pack_start'\n");
    printf("    error: implicit declaration of function 'gtk_container_add'\n");
    printf("    error: implicit declaration of function 'GTK_CONTAINER'\n");
    printf("    error: implicit declaration of function 'gtk_widget_show_all'\n\n");

    printf("  \"Implicit declaration\" means the function does not exist in the\n");
    printf("  headers you included -- not that you misspelled it. GCC 14\n");
    printf("  made that an error; older GCC versions only warned. The GTK4\n");
    printf("  replacements:\n\n");

    printf("    GTK3                            GTK4\n");
    printf("    gtk_box_pack_start(box, w,...)  gtk_box_append(box, w)\n");
    printf("    gtk_container_add(window, w)    gtk_window_set_child(window, w)\n");
    printf("    gtk_widget_show_all(window)     gtk_window_present(window)\n");

    wait_enter();
    clear_screen();
    heading("PART 2: the four widgets most forms are made of");

    printf("    gtk_label_new(\"text\")                  text the user reads\n");
    printf("    gtk_entry_new()                        one line of typed text\n");
    printf("    gtk_check_button_new_with_label(\"x\")   a tick box with a label\n");
    printf("    gtk_button_new_with_label(\"OK\")        a button\n\n");

    printf("  A GtkLabel placed in a vertical box does not sit at the left\n");
    printf("  by default: it stretches across the whole box. Measured on the\n");
    printf("  example below, the \"Your name:\" label was 336 pixels wide (the\n");
    printf("  full width of its column) with the default alignment, and 79\n");
    printf("  once gtk_widget_set_halign(prompt, GTK_ALIGN_START) was set.\n");
    printf("  START means \"take only what the text needs, and stay at the\n");
    printf("  start edge\".\n");

    wait_enter();
    clear_screen();
    heading("PART 3: children hug their content unless told otherwise");

    printf("  In the horizontal row, OK and Cancel are only as wide as their\n");
    printf("  text. gtk_widget_set_hexpand(button, TRUE) lets a child grow\n");
    printf("  into spare room. Measured on this author's machine, with the\n");
    printf("  window 360 pixels wide:\n\n");

    printf("    no hexpand                     OK  22 px   Cancel  49 px\n");
    printf("    hexpand on both                OK 117 px   Cancel 143 px\n");
    printf("    hexpand + gtk_box_set_homogeneous(box, TRUE)\n");
    printf("                                   OK 130 px   Cancel 130 px\n\n");

    printf("  The middle row is the trap. Both buttons grew, but they are\n");
    printf("  still not equal: each one starts from its own text width, and\n");
    printf("  the 27 px difference between \"OK\" and \"Cancel\" (22 against 49)\n");
    printf("  shows up again as 26 px (117 against 143). To get equal\n");
    printf("  widths, make the box homogeneous. The exact pixel counts\n");
    printf("  depend on your theme and font; which row is equal and which\n");
    printf("  is not does not.\n");

    wait_enter();
    clear_screen();
    heading("THE FULL EXAMPLE");

    printf("  A small form: a left-aligned label, a text field, a check\n");
    printf("  button, and two buttons sharing a row. It does nothing yet --\n");
    printf("  wiring widgets to behaviour is module 3.\n\n");

    rule();
    printf("  #include <gtk/gtk.h>\n");
    printf("  \n");
    printf("  static void activate(GtkApplication *app, gpointer user_data)\n");
    printf("  {\n");
    printf("      (void) user_data;\n");
    printf("      GtkWidget *window = gtk_application_window_new(app);\n");
    printf("      gtk_window_set_title(GTK_WINDOW(window), \"Layout\");\n");
    printf("      gtk_window_set_default_size(GTK_WINDOW(window), 360, 160);\n");
    printf("  \n");
    printf("      GtkWidget *column = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);\n");
    printf("      gtk_widget_set_margin_top(column, 12);\n");
    printf("      gtk_widget_set_margin_bottom(column, 12);\n");
    printf("      gtk_widget_set_margin_start(column, 12);\n");
    printf("      gtk_widget_set_margin_end(column, 12);\n");
    printf("  \n");
    printf("      GtkWidget *prompt = gtk_label_new(\"Your name:\");\n");
    printf("      gtk_widget_set_halign(prompt, GTK_ALIGN_START);\n");
    printf("      GtkWidget *entry = gtk_entry_new();\n");
    printf("      GtkWidget *check = gtk_check_button_new_with_label(\"Subscribe\");\n");
    printf("  \n");
    printf("      GtkWidget *buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);\n");
    printf("      GtkWidget *ok = gtk_button_new_with_label(\"OK\");\n");
    printf("      GtkWidget *cancel = gtk_button_new_with_label(\"Cancel\");\n");
    printf("      gtk_widget_set_hexpand(ok, TRUE);\n");
    printf("      gtk_widget_set_hexpand(cancel, TRUE);\n");
    printf("      gtk_box_append(GTK_BOX(buttons), ok);\n");
    printf("      gtk_box_append(GTK_BOX(buttons), cancel);\n");
    printf("  \n");
    printf("      gtk_box_append(GTK_BOX(column), prompt);\n");
    printf("      gtk_box_append(GTK_BOX(column), entry);\n");
    printf("      gtk_box_append(GTK_BOX(column), check);\n");
    printf("      gtk_box_append(GTK_BOX(column), buttons);\n");
    printf("      gtk_window_set_child(GTK_WINDOW(window), column);\n");
    printf("      gtk_window_present(GTK_WINDOW(window));\n");
    printf("  }\n");
    printf("  \n");
    printf("  int main(int argc, char **argv)\n");
    printf("  {\n");
    printf("      GtkApplication *app = gtk_application_new(\n");
    printf("          \"com.example.layout\", G_APPLICATION_DEFAULT_FLAGS);\n");
    printf("      g_signal_connect(app, \"activate\", G_CALLBACK(activate), NULL);\n");
    printf("      int status = g_application_run(G_APPLICATION(app), argc, argv);\n");
    printf("      g_object_unref(app);\n");
    printf("      return status;\n");
    printf("  }\n");
    rule();

    printf("\n  Build and run it yourself:\n\n");
    printf("    gcc -std=c11 -Wall -Wextra $(pkg-config --cflags gtk4) main.c -o app $(pkg-config --libs gtk4)\n");
    printf("    ./app\n");

    wait_enter();
    clear_screen();
    exercise(2);

    question("Which GTK4 function adds a widget to the end of a GtkBox?\n"
             "  (the function name)",
             "gtk_box_append",
             "gtk_box_pack_start and gtk_container_add are GTK3. In GTK4\n"
             "             the box has gtk_box_append, and a window takes a single\n"
             "             child through gtk_window_set_child.");

    question("A button inside a horizontal box is only as wide as its text.\n"
             "  Which property makes it grow into the spare room?\n"
             "  (one word, the part of the function name after set_)",
             "hexpand",
             "gtk_widget_set_hexpand(button, TRUE). Its vertical twin is\n"
             "             vexpand. Without it a child asks only for its own size.");

    question("gcc says \"implicit declaration of function\n"
             "  'gtk_container_add'\". In which GTK major version was that\n"
             "  function removed? (a number)",
             "4",
             "It exists in GTK3 and not in GTK4. The header you included\n"
             "             really does not declare it, so this is not a typo.");

    printf("\n  >> BUILD THIS YOURSELF, in a real file\n");
    rule();
    printf("  Starting from the full example, make OK and Cancel exactly the\n");
    printf("  same width, filling the row. Each of them still needs\n");
    printf("  gtk_widget_set_hexpand; the box holding them needs one more\n");
    printf("  call, from Part 3.\n\n");

    printf("  There is no transcript to match -- the result is a window. You\n");
    printf("  have it right when the two buttons are the same width and\n");
    printf("  together fill the row, and widening the window keeps them\n");
    printf("  equal.\n\n");

    printf("  Compile with:\n");
    printf("    gcc -std=c11 -Wall -Wextra $(pkg-config --cflags gtk4) main.c -o app $(pkg-config --libs gtk4)\n");

    if (ask_yes("Want to see example code?")) {
        printf("\n");
        printf("  One added line, right after the two buttons are created\n");
        printf("  (this version was compiled and measured: 130 px and 130 px):\n\n");
        printf("      gtk_box_set_homogeneous(GTK_BOX(buttons), TRUE);\n");
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(2)) {
        printf("   - a window has one child; GtkBox holds many, and boxes nest\n");
        printf("   - gtk_box_append and gtk_window_set_child are GTK4; GTK3's\n");
        printf("     pack_start and container_add are \"implicit declaration\"\n");
        printf("     errors here\n");
        printf("   - margins are set per side on the outer box; the box's\n");
        printf("     number is the gap between children\n");
        printf("   - a child takes only the size it needs; halign START keeps a\n");
        printf("     label from stretching, hexpand lets a child grow, and\n");
        printf("     homogeneous is what makes the grown children equal\n");
    }
    wait_enter();
}

void lesson_03_signals(void)
{
    title("MODULE 3 - GTK: SIGNALS AND KEEPING STATE");

    heading("A CALLBACK CAN SEE ONLY WHAT IT IS HANDED");

    printf("  A form that does something has to read one widget when\n");
    printf("  another is clicked. But a callback is a plain C function:\n");
    printf("  its parameters are the widget that emitted the signal and\n");
    printf("  the user_data pointer given to g_signal_connect. Nothing\n");
    printf("  else is in scope. There are three ways to reach the widgets\n");
    printf("  it needs: a global variable per widget, one widget through\n");
    printf("  user_data (module 1's label), or -- the way used here -- a\n");
    printf("  struct holding every widget the handlers share, passed\n");
    printf("  through user_data:\n\n");

    printf("    struct Ui {\n");
    printf("        GtkWidget *entry;\n");
    printf("        GtkWidget *shout;\n");
    printf("        GtkWidget *result;\n");
    printf("    };\n\n");
    printf("    g_signal_connect(button, \"clicked\", G_CALLBACK(greet), &ui);\n\n");

    printf("  and inside the handler, user_data is that pointer again:\n\n");
    printf("    struct Ui *u = user_data;\n");

    wait_enter();
    clear_screen();
    heading("PART 1: where the struct lives");

    printf("  The obvious place to put it is where the widgets are made,\n");
    printf("  inside activate():\n\n");
    printf("    static void activate(GtkApplication *app, gpointer user_data)\n");
    printf("    {\n");
    printf("        struct Ui ui;                      /* <-- local variable */\n");
    printf("        ...\n");
    printf("        g_signal_connect(button, \"clicked\", G_CALLBACK(greet), &ui);\n");
    printf("    }\n\n");

    printf("  gcc -std=c11 -Wall -Wextra compiled that file with no output at\n");
    printf("  all -- not even a warning. It is still a bug. activate() builds\n");
    printf("  the window and returns straight away, and a local variable ends\n");
    printf("  when its function does. The button is clicked later, from GTK's\n");
    printf("  event loop; by then &ui points at stack space that something\n");
    printf("  else is using. That is undefined behaviour, so this course does\n");
    printf("  not run it. Same family as returning a pointer to a local\n");
    printf("  array in plain C.\n\n");

    printf("  The state has to outlive activate(). The simplest fix is a\n");
    printf("  file-level static, which is what every example here uses:\n\n");
    printf("    static struct Ui ui;\n\n");
    printf("  A heap block from g_new0(struct Ui, 1) also works, but then\n");
    printf("  something has to g_free it. A static struct for one window is\n");
    printf("  enough until a program opens many.\n");

    wait_enter();
    clear_screen();
    heading("PART 2: who owns which string");

    printf("  Two calls return text, and they follow opposite rules:\n\n");

    printf("    const char *name = gtk_editable_get_text(GTK_EDITABLE(entry));\n");
    printf("    char *message = g_strdup_printf(\"Hello, %%s.\", name);\n\n");

    printf("  gtk_editable_get_text returns a pointer into the entry. GTK\n");
    printf("  owns it: you do not free it, and you must not change it. It\n");
    printf("  is only good until the entry's text changes, so to keep the\n");
    printf("  name past that, copy it with g_strdup.\n\n");

    printf("  g_strdup_printf builds a new string and gives you ownership.\n");
    printf("  You free it with g_free (not free -- GLib's allocator is the\n");
    printf("  one that handed it out). The example calls g_free right after\n");
    printf("  gtk_label_set_text, which copies the text, so the label does\n");
    printf("  not depend on your copy surviving.\n\n");

    printf("  Setting an entry's text goes through the same GtkEditable\n");
    printf("  interface: gtk_editable_set_text(GTK_EDITABLE(entry), \"\")\n");
    printf("  empties it.\n");

    wait_enter();
    clear_screen();
    heading("PART 3: one handler, two signals");

    printf("  Pressing Enter in a GtkEntry emits \"activate\". Clicking a\n");
    printf("  button emits \"clicked\". If both should do the same thing,\n");
    printf("  connect both to one function instead of copying it:\n\n");

    printf("    g_signal_connect(button, \"clicked\", G_CALLBACK(greet), &ui);\n");
    printf("    g_signal_connect(ui.entry, \"activate\", G_CALLBACK(greet), &ui);\n\n");

    printf("  Module 1's warning still applies: G_CALLBACK checks nothing.\n");
    printf("  greet() is written as (gpointer source, gpointer user_data):\n");
    printf("  both of these signals pass an object pointer first and the\n");
    printf("  user_data last, and the handler ignores the object. A signal\n");
    printf("  with extra parameters in the middle would need a different\n");
    printf("  signature, which is why docs.gtk.org is worth opening for\n");
    printf("  each new signal.\n");

    wait_enter();
    clear_screen();
    heading("THE FULL EXAMPLE");

    printf("  A greeter. Clicking Greet or pressing Enter in the field\n");
    printf("  shows a message, in capitals when Shout is ticked.\n\n");

    rule();
    printf("  #include <gtk/gtk.h>\n");
    printf("  \n");
    printf("  struct Ui {\n");
    printf("      GtkWidget *entry;\n");
    printf("      GtkWidget *shout;\n");
    printf("      GtkWidget *result;\n");
    printf("  };\n");
    printf("  \n");
    printf("  static struct Ui ui;\n");
    printf("  \n");
    printf("  static void greet(gpointer source, gpointer user_data)\n");
    printf("  {\n");
    printf("      (void) source;\n");
    printf("      struct Ui *u = user_data;\n");
    printf("  \n");
    printf("      const char *name = gtk_editable_get_text(GTK_EDITABLE(u->entry));\n");
    printf("      if (*name == '\\0') {\n");
    printf("          gtk_label_set_text(GTK_LABEL(u->result), \"Type a name first\");\n");
    printf("          return;\n");
    printf("      }\n");
    printf("  \n");
    printf("      char *message;\n");
    printf("      if (gtk_check_button_get_active(GTK_CHECK_BUTTON(u->shout)))\n");
    printf("          message = g_strdup_printf(\"HELLO, %%s!\", name);\n");
    printf("      else\n");
    printf("          message = g_strdup_printf(\"Hello, %%s.\", name);\n");
    printf("  \n");
    printf("      gtk_label_set_text(GTK_LABEL(u->result), message);\n");
    printf("      g_free(message);\n");
    printf("  }\n");
    printf("  \n");
    printf("  static void activate(GtkApplication *app, gpointer user_data)\n");
    printf("  {\n");
    printf("      (void) user_data;\n");
    printf("      GtkWidget *window = gtk_application_window_new(app);\n");
    printf("      gtk_window_set_title(GTK_WINDOW(window), \"Greeter\");\n");
    printf("      gtk_window_set_default_size(GTK_WINDOW(window), 320, 160);\n");
    printf("  \n");
    printf("      GtkWidget *column = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);\n");
    printf("      gtk_widget_set_margin_top(column, 12);\n");
    printf("      gtk_widget_set_margin_bottom(column, 12);\n");
    printf("      gtk_widget_set_margin_start(column, 12);\n");
    printf("      gtk_widget_set_margin_end(column, 12);\n");
    printf("  \n");
    printf("      ui.entry = gtk_entry_new();\n");
    printf("      ui.shout = gtk_check_button_new_with_label(\"Shout\");\n");
    printf("      ui.result = gtk_label_new(\"\");\n");
    printf("      GtkWidget *button = gtk_button_new_with_label(\"Greet\");\n");
    printf("  \n");
    printf("      g_signal_connect(button, \"clicked\", G_CALLBACK(greet), &ui);\n");
    printf("      g_signal_connect(ui.entry, \"activate\", G_CALLBACK(greet), &ui);\n");
    printf("  \n");
    printf("      gtk_box_append(GTK_BOX(column), ui.entry);\n");
    printf("      gtk_box_append(GTK_BOX(column), ui.shout);\n");
    printf("      gtk_box_append(GTK_BOX(column), button);\n");
    printf("      gtk_box_append(GTK_BOX(column), ui.result);\n");
    printf("      gtk_window_set_child(GTK_WINDOW(window), column);\n");
    printf("      gtk_window_present(GTK_WINDOW(window));\n");
    printf("  }\n");
    printf("  \n");
    printf("  int main(int argc, char **argv)\n");
    printf("  {\n");
    printf("      GtkApplication *app = gtk_application_new(\n");
    printf("          \"com.example.greeter\", G_APPLICATION_DEFAULT_FLAGS);\n");
    printf("      g_signal_connect(app, \"activate\", G_CALLBACK(activate), NULL);\n");
    printf("      int status = g_application_run(G_APPLICATION(app), argc, argv);\n");
    printf("      g_object_unref(app);\n");
    printf("      return status;\n");
    printf("  }\n");
    rule();

    printf("\n  Checked by driving the real window from a test program:\n\n");
    printf("    click Greet, field empty         ->  Type a name first\n");
    printf("    type Ana, click Greet            ->  Hello, Ana.\n");
    printf("    tick Shout, press Enter          ->  HELLO, Ana!\n");

    wait_enter();
    clear_screen();
    exercise(3);

    question("The struct is declared inside activate() and its address is\n"
             "  passed to g_signal_connect. After activate() has returned,\n"
             "  does that struct still exist when the button is clicked?\n"
             "  (yes or no)",
             "no",
             "A local variable ends with its function. gcc -Wall -Wextra\n"
             "             printed nothing for this mistake, so the compiler will\n"
             "             not catch it for you. Make the struct static.");

    question("gtk_editable_get_text(...) returns a const char *. Do you\n"
             "  free it? (yes or no)",
             "no",
             "The text belongs to the entry. Copy it with g_strdup if you\n"
             "             need it after the entry changes, and free that copy.");

    question("Which function releases a string built by g_strdup_printf?\n"
             "  (the function name)",
             "g_free",
             "g_strdup_printf hands you ownership. GLib allocated it, so\n"
             "             GLib's g_free releases it, not free().");

    printf("\n  >> BUILD THIS YOURSELF, in a real file\n");
    rule();
    printf("  Starting from the greeter, add a Clear button that empties\n");
    printf("  the text field and the result label. It needs your own\n");
    printf("  handler, connected to the button's \"clicked\" signal, and it\n");
    printf("  must reach both widgets through the struct. Use\n");
    printf("  gtk_editable_set_text for the field and gtk_label_set_text\n");
    printf("  for the label.\n\n");

    printf("  There is no transcript to match -- the result is a window. You\n");
    printf("  have it right when, after greeting someone, clicking Clear\n");
    printf("  leaves an empty field and an empty label.\n\n");

    printf("  Compile with:\n");
    printf("    gcc -std=c11 -Wall -Wextra $(pkg-config --cflags gtk4) main.c -o app $(pkg-config --libs gtk4)\n");

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(3)) {
        printf("   - a callback gets the emitting object and user_data; a\n");
        printf("     struct of widgets passed as user_data is how it reaches\n");
        printf("     the rest\n");
        printf("   - that struct must outlive activate(): a local one compiles\n");
        printf("     silently and is a bug; a file-level static is the simple\n");
        printf("     fix\n");
        printf("   - gtk_editable_get_text is GTK's string, do not free it;\n");
        printf("     g_strdup_printf is your string, g_free it\n");
        printf("   - one function can handle several signals, for example a\n");
        printf("     button's \"clicked\" and an entry's \"activate\"\n");
    }
    wait_enter();
}

void lesson_04_dynamic(void)
{
    title("MODULE 4 - GTK: WIDGETS THAT COME AND GO");

    heading("A LIST THAT CHANGES WHILE THE PROGRAM RUNS");

    printf("  Everything so far built its widgets once, in activate().\n");
    printf("  A todo list cannot: rows appear when the user presses Add\n");
    printf("  and vanish when they press x. The calls are the same ones\n");
    printf("  from modules 1-3, only made from inside a handler:\n\n");

    printf("    GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);\n");
    printf("    GtkWidget *check = gtk_check_button_new_with_label(text);\n");
    printf("    GtkWidget *del = gtk_button_new_with_label(\"x\");\n");
    printf("    gtk_box_append(GTK_BOX(row), check);\n");
    printf("    gtk_box_append(GTK_BOX(row), del);\n");
    printf("    gtk_box_append(GTK_BOX(ui.list), row);\n\n");

    printf("  ui.list is a vertical box that already exists in the window.\n");
    printf("  Appending to a box that is on screen shows the new child\n");
    printf("  straight away; there is no \"redraw\" call to make. Read back\n");
    printf("  from the real window, the first row added was mapped on\n");
    printf("  screen and 34 pixels tall.\n");

    wait_enter();
    clear_screen();
    heading("PART 1: per-row data without a second list");

    printf("  The \"x\" button must remove its own row, and Clear done must\n");
    printf("  know which rows are ticked. Each row is a widget, so the\n");
    printf("  program can ask the row. g_object_set_data attaches a named\n");
    printf("  pointer to any widget:\n\n");

    printf("    g_object_set_data(G_OBJECT(row), \"check\", check);\n");
    printf("    ...\n");
    printf("    GtkWidget *check = g_object_get_data(G_OBJECT(row), \"check\");\n\n");

    printf("  The name is a string, so a typo gives NULL at run time, not an\n");
    printf("  error at compile time -- the same property \"clicked\" had in\n");
    printf("  module 1. The delete handler is told its row through\n");
    printf("  user_data, which is module 3's other use of that argument:\n\n");

    printf("    g_signal_connect(del, \"clicked\", G_CALLBACK(on_delete), row);\n");

    wait_enter();
    clear_screen();
    heading("PART 2: count by looking, not by keeping a number");

    printf("  \"N left\" could be an int incremented on Add and decremented on\n");
    printf("  tick, delete and clear -- four places that must all agree. The\n");
    printf("  example has no such int. refresh() walks the rows and counts\n");
    printf("  the unticked ones every time:\n\n");

    printf("    for (GtkWidget *row = gtk_widget_get_first_child(ui.list);\n");
    printf("         row != NULL;\n");
    printf("         row = gtk_widget_get_next_sibling(row)) { ... }\n\n");

    printf("  The widgets are the one source of truth, so the label cannot\n");
    printf("  drift away from what is on screen. This is a plain loop over\n");
    printf("  a linked chain of widgets, and for a list the size of a todo\n");
    printf("  list the cost does not matter.\n");

    wait_enter();
    clear_screen();
    heading("PART 3: removing while looping");

    printf("  Clear done removes every ticked row. The natural loop is:\n\n");
    printf("    for (row = first; row; row = gtk_widget_get_next_sibling(row))\n");
    printf("        if (ticked(row))\n");
    printf("            gtk_box_remove(GTK_BOX(ui.list), row);\n\n");

    printf("  gtk_box_remove drops the box's reference to the row. As module\n");
    printf("  1 showed, the container is what keeps its children alive, so\n");
    printf("  the row can be freed right there. The loop's next step then\n");
    printf("  calls gtk_widget_get_next_sibling on a row that may no longer\n");
    printf("  exist: undefined behaviour, so not run here. Read the next\n");
    printf("  sibling first, remove after:\n\n");

    printf("    GtkWidget *row = gtk_widget_get_first_child(ui.list);\n");
    printf("    while (row != NULL) {\n");
    printf("        GtkWidget *next = gtk_widget_get_next_sibling(row);\n");
    printf("        if (ticked(row))\n");
    printf("            gtk_box_remove(GTK_BOX(ui.list), row);\n");
    printf("        row = next;\n");
    printf("    }\n\n");

    printf("  Same rule as deleting from a linked list in plain C: save the\n");
    printf("  link before you free the node. refresh() may use a for loop\n");
    printf("  because it only reads.\n");

    wait_enter();
    clear_screen();
    heading("PART 4: a list that scrolls");

    printf("  Fifty rows would push the window taller than the screen. A\n");
    printf("  GtkScrolledWindow around the list adds a scrollbar instead:\n\n");

    printf("    GtkWidget *scroll = gtk_scrolled_window_new();\n");
    printf("    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), ui.list);\n");
    printf("    gtk_widget_set_vexpand(scroll, TRUE);\n\n");

    printf("  The vexpand line is module 2's expand idea, vertically. Read\n");
    printf("  back from the real window (420 pixels tall, empty list):\n");
    printf("  the scroller was 297 pixels tall with vexpand on, and 46\n");
    printf("  with it off, a thin strip under the buttons. Pixel counts\n");
    printf("  depend on your theme; the difference does not.\n");

    wait_enter();
    clear_screen();
    heading("THE FULL EXAMPLE");

    printf("  The todo list: type a task, press Enter or Add; tick it; x\n");
    printf("  deletes one row; Clear done deletes every ticked row.\n\n");

    rule();
    printf("  #include <gtk/gtk.h>\n");
    printf("  \n");
    printf("  struct Ui {\n");
    printf("      GtkWidget *entry;\n");
    printf("      GtkWidget *list;\n");
    printf("      GtkWidget *status;\n");
    printf("  };\n");
    printf("  \n");
    printf("  static struct Ui ui;\n");
    printf("  \n");
    printf("  static void refresh(void)\n");
    printf("  {\n");
    printf("      int left = 0;\n");
    printf("      for (GtkWidget *row = gtk_widget_get_first_child(ui.list); row != NULL;\n");
    printf("           row = gtk_widget_get_next_sibling(row)) {\n");
    printf("          GtkWidget *check = g_object_get_data(G_OBJECT(row), \"check\");\n");
    printf("          if (!gtk_check_button_get_active(GTK_CHECK_BUTTON(check)))\n");
    printf("              left++;\n");
    printf("      }\n");
    printf("      char *text = g_strdup_printf(\"%%d left\", left);\n");
    printf("      gtk_label_set_text(GTK_LABEL(ui.status), text);\n");
    printf("      g_free(text);\n");
    printf("  }\n");
    printf("  \n");
    printf("  static void on_toggled(gpointer source, gpointer user_data)\n");
    printf("  {\n");
    printf("      (void) source;\n");
    printf("      (void) user_data;\n");
    printf("      refresh();\n");
    printf("  }\n");
    printf("  \n");
    printf("  static void on_delete(gpointer source, gpointer row)\n");
    printf("  {\n");
    printf("      (void) source;\n");
    printf("      gtk_box_remove(GTK_BOX(ui.list), GTK_WIDGET(row));\n");
    printf("      refresh();\n");
    printf("  }\n");
    printf("  \n");
    printf("  static void on_add(gpointer source, gpointer user_data)\n");
    printf("  {\n");
    printf("      (void) source;\n");
    printf("      (void) user_data;\n");
    printf("      const char *text = gtk_editable_get_text(GTK_EDITABLE(ui.entry));\n");
    printf("      if (*text == '\\0')\n");
    printf("          return;\n");
    printf("  \n");
    printf("      GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);\n");
    printf("      GtkWidget *check = gtk_check_button_new_with_label(text);\n");
    printf("      GtkWidget *del = gtk_button_new_with_label(\"x\");\n");
    printf("      gtk_widget_set_hexpand(check, TRUE);\n");
    printf("      g_object_set_data(G_OBJECT(row), \"check\", check);\n");
    printf("      g_signal_connect(check, \"toggled\", G_CALLBACK(on_toggled), NULL);\n");
    printf("      g_signal_connect(del, \"clicked\", G_CALLBACK(on_delete), row);\n");
    printf("      gtk_box_append(GTK_BOX(row), check);\n");
    printf("      gtk_box_append(GTK_BOX(row), del);\n");
    printf("      gtk_box_append(GTK_BOX(ui.list), row);\n");
    printf("  \n");
    printf("      gtk_editable_set_text(GTK_EDITABLE(ui.entry), \"\");\n");
    printf("      refresh();\n");
    printf("  }\n");
    printf("  \n");
    printf("  static void on_clear_done(gpointer source, gpointer user_data)\n");
    printf("  {\n");
    printf("      (void) source;\n");
    printf("      (void) user_data;\n");
    printf("      GtkWidget *row = gtk_widget_get_first_child(ui.list);\n");
    printf("      while (row != NULL) {\n");
    printf("          GtkWidget *next = gtk_widget_get_next_sibling(row);\n");
    printf("          GtkWidget *check = g_object_get_data(G_OBJECT(row), \"check\");\n");
    printf("          if (gtk_check_button_get_active(GTK_CHECK_BUTTON(check)))\n");
    printf("              gtk_box_remove(GTK_BOX(ui.list), row);\n");
    printf("          row = next;\n");
    printf("      }\n");
    printf("      refresh();\n");
    printf("  }\n");
    printf("  \n");
    printf("  static void activate(GtkApplication *app, gpointer user_data)\n");
    printf("  {\n");
    printf("      (void) user_data;\n");
    printf("      GtkWidget *window = gtk_application_window_new(app);\n");
    printf("      gtk_window_set_title(GTK_WINDOW(window), \"Todo\");\n");
    printf("      gtk_window_set_default_size(GTK_WINDOW(window), 400, 420);\n");
    printf("  \n");
    printf("      GtkWidget *column = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);\n");
    printf("      gtk_widget_set_margin_top(column, 8);\n");
    printf("      gtk_widget_set_margin_bottom(column, 8);\n");
    printf("      gtk_widget_set_margin_start(column, 8);\n");
    printf("      gtk_widget_set_margin_end(column, 8);\n");
    printf("  \n");
    printf("      GtkWidget *top = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);\n");
    printf("      ui.entry = gtk_entry_new();\n");
    printf("      gtk_widget_set_hexpand(ui.entry, TRUE);\n");
    printf("      GtkWidget *add = gtk_button_new_with_label(\"Add\");\n");
    printf("      GtkWidget *clear = gtk_button_new_with_label(\"Clear done\");\n");
    printf("      gtk_box_append(GTK_BOX(top), ui.entry);\n");
    printf("      gtk_box_append(GTK_BOX(top), add);\n");
    printf("      gtk_box_append(GTK_BOX(top), clear);\n");
    printf("  \n");
    printf("      ui.list = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);\n");
    printf("      GtkWidget *scroll = gtk_scrolled_window_new();\n");
    printf("      gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), ui.list);\n");
    printf("      gtk_widget_set_vexpand(scroll, TRUE);\n");
    printf("  \n");
    printf("      ui.status = gtk_label_new(\"0 left\");\n");
    printf("      gtk_widget_set_halign(ui.status, GTK_ALIGN_START);\n");
    printf("  \n");
    printf("      g_signal_connect(ui.entry, \"activate\", G_CALLBACK(on_add), NULL);\n");
    printf("      g_signal_connect(add, \"clicked\", G_CALLBACK(on_add), NULL);\n");
    printf("      g_signal_connect(clear, \"clicked\", G_CALLBACK(on_clear_done), NULL);\n");
    printf("  \n");
    printf("      gtk_box_append(GTK_BOX(column), top);\n");
    printf("      gtk_box_append(GTK_BOX(column), scroll);\n");
    printf("      gtk_box_append(GTK_BOX(column), ui.status);\n");
    printf("      gtk_window_set_child(GTK_WINDOW(window), column);\n");
    printf("      gtk_window_present(GTK_WINDOW(window));\n");
    printf("  }\n");
    printf("  \n");
    printf("  int main(int argc, char **argv)\n");
    printf("  {\n");
    printf("      GtkApplication *app = gtk_application_new(\n");
    printf("          \"com.example.todo\", G_APPLICATION_DEFAULT_FLAGS);\n");
    printf("      g_signal_connect(app, \"activate\", G_CALLBACK(activate), NULL);\n");
    printf("      int status = g_application_run(G_APPLICATION(app), argc, argv);\n");
    printf("      g_object_unref(app);\n");
    printf("      return status;\n");
    printf("  }\n");
    rule();

    printf("\n  Checked by driving the real window from a test program:\n\n");
    printf("    add a, b, c, d                     ->  4 left, 4 rows\n");
    printf("    add an empty line                  ->  nothing added\n");
    printf("    tick two, delete one ticked row    ->  2 left, 3 rows\n");
    printf("    delete one unticked row            ->  1 left, 2 rows\n");
    printf("    add e, f; tick two; Clear done     ->  2 left, 2 rows\n");
    printf("    tick everything; Clear done        ->  0 left, 0 rows\n");

    wait_enter();
    clear_screen();
    exercise(4);

    question("After gtk_box_remove(list, row), may the next line call\n"
             "  gtk_widget_get_next_sibling(row)? (yes or no)",
             "no",
             "Removing drops the box's reference, and the box was what\n"
             "             kept the row alive, so row may already be freed. Read\n"
             "             the next sibling before you remove.");

    question("Which function attaches a named pointer, such as a row's\n"
             "  check button, to a widget? (the function name)",
             "g_object_set_data",
             "g_object_set_data(G_OBJECT(row), \"check\", check), read back\n"
             "             with g_object_get_data. The name is a string, so a\n"
             "             typo returns NULL instead of failing to compile.");

    question("The \"N left\" label has no counter variable behind it. What\n"
             "  does refresh() do instead? (one word: count, or keep)",
             "count",
             "It walks the rows and counts the unticked ones each time, so\n"
             "             the label always matches what is on screen.");

    printf("\n  >> BUILD THIS YOURSELF, in a real file\n");
    rule();
    printf("  Starting from the todo list, add an All done button that\n");
    printf("  ticks every row. Loop over the rows as refresh() does, and\n");
    printf("  tick each check button with gtk_check_button_set_active(\n");
    printf("  GTK_CHECK_BUTTON(check), TRUE). You do not need to call\n");
    printf("  refresh() yourself: ticking a button emits \"toggled\".\n\n");

    printf("  There is no transcript to match -- the result is a window. You\n");
    printf("  have it right when, after adding a few tasks, All done makes\n");
    printf("  every box ticked and the label reads 0 left.\n\n");

    printf("  Compile with:\n");
    printf("    gcc -std=c11 -Wall -Wextra $(pkg-config --cflags gtk4) main.c -o app $(pkg-config --libs gtk4)\n");

    if (ask_yes("Want to see example code?")) {
        printf("\n");
        rule();
        printf("  static void on_all_done(gpointer source, gpointer user_data)\n");
        printf("  {\n");
        printf("      (void) source;\n");
        printf("      (void) user_data;\n");
        printf("      for (GtkWidget *row = gtk_widget_get_first_child(ui.list);\n");
        printf("           row != NULL; row = gtk_widget_get_next_sibling(row)) {\n");
        printf("          GtkWidget *check = g_object_get_data(G_OBJECT(row), \"check\");\n");
        printf("          gtk_check_button_set_active(GTK_CHECK_BUTTON(check), TRUE);\n");
        printf("      }\n");
        printf("  }\n");
        rule();
        printf("  Connect it with g_signal_connect(all, \"clicked\",\n");
        printf("  G_CALLBACK(on_all_done), NULL) and append the button to the\n");
        printf("  top row. Compiled and tested: three added tasks, All done,\n");
        printf("  label reads 0 left, three rows still present.\n");
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    if (!print_summary_pt(4)) {
        printf("   - a handler can build and append widgets; a box that is on\n");
        printf("     screen shows them at once\n");
        printf("   - g_object_set_data hangs a named pointer on a widget; a\n");
        printf("     mistyped name returns NULL at run time\n");
        printf("   - derive a count from the widgets instead of keeping a\n");
        printf("     number that four handlers must update\n");
        printf("   - read gtk_widget_get_next_sibling before gtk_box_remove\n");
        printf("   - GtkScrolledWindow plus vexpand gives a list that scrolls\n\n");

        printf("  This is enough to build a simple GTK4 window with fields,\n");
        printf("  buttons, a list and behaviour. What it does not cover:\n");
        printf("  styling with CSS, windows described in .ui files (GtkBuilder),\n");
        printf("  and GTK's model-based list widgets for very large lists.\n");
        printf("  Those are in docs.gtk.org.\n");
    }
    wait_enter();
}
