/*
 * Modules 2-4 - GTK: layout and widgets, signals and state, and widgets
 * that come and go.
 *
 * Same rule as module 1 (lessons_gtk.c): the lesson program itself is plain
 * C, and the GTK code it teaches is compiled separately by the student.
 * gcc 14.2.1, GTK 4.22.5.
 *
 * Every example printed here is a file that was compiled with
 * -std=c11 -Wall -Wextra and no warnings, then driven by a test program
 * that included it, opened the real window, set entry text, ticked check
 * buttons, emitted "clicked" and "activate", and asserted on the labels and
 * the number of rows. The examples are printed with their comments exactly
 * as in those files.
 *
 * Numbers read back from the real widgets on the author's machine (they
 * change with theme and font, and the lessons say so): the label widths
 * (336 and 79), the button widths (22/49, 117/143, 130/130), the scroller
 * heights (297 and 46) and the first row's height (34).
 *
 * Run, and quoted as real output next to the BAD examples:
 *   gtk_box_append of a widget that already has a parent -> CRITICAL
 *   gtk_box_remove of a widget that is not a child       -> CRITICAL
 *   GTK_EDITABLE(label) / a NULL widget                   -> CRITICAL
 *   g_object_get_data with a mistyped name                -> NULL
 *   GTK3's gtk_box_pack_start, gtk_container_add,
 *   gtk_widget_show_all                                   -> compile errors
 *
 * Explained and not run, because they are undefined behaviour: a callback
 * reading a dead stack struct, free() on GTK's text, and reading a
 * widget's sibling after gtk_box_remove.
 */

#include <stdio.h>

#include "lessons.h"
#include "strings_pt.h"
#include "ui.h"

static const char *const EX2_P0[] = {
    "// layout.c : a form built from boxes, a label, a text field, buttons.",
    "",
    "#include <gtk/gtk.h>",
    "",
    "static void activate(GtkApplication *app, gpointer user_data)",
    "{",
    "    (void) user_data;",
    "",
    "    GtkWidget *window = gtk_application_window_new(app);",
    "    gtk_window_set_title(GTK_WINDOW(window), \"Layout\");",
    "    gtk_window_set_default_size(GTK_WINDOW(window), 360, 160);",
    "",
    "    // A window holds ONE child, so that child is a vertical box.",
    "    // The box stacks its own children top to bottom, 8 pixels apart.",
    "    GtkWidget *column = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);",
    "",
    "    // Space between the box and the window edge, one call per side.",
    "    gtk_widget_set_margin_top(column, 12);",
    "    gtk_widget_set_margin_bottom(column, 12);",
    "    gtk_widget_set_margin_start(column, 12);",
    "    gtk_widget_set_margin_end(column, 12);",
};
static const char *const EX2_P1[] = {
    "    // A label: text the user reads but cannot edit.",
    "    GtkWidget *prompt = gtk_label_new(\"Your name:\");",
    "",
    "    // A label stretches across the whole box by default. START makes",
    "    // it take only the room its text needs, at the left edge.",
    "    gtk_widget_set_halign(prompt, GTK_ALIGN_START);",
    "",
    "    // A one-line text field the user can type in.",
    "    GtkWidget *entry = gtk_entry_new();",
    "",
    "    // A tick box with its label beside it.",
    "    GtkWidget *check = gtk_check_button_new_with_label(\"Subscribe\");",
};
static const char *const EX2_P2[] = {
    "    // A second box, horizontal, to put two buttons side by side.",
    "    // Boxes nest inside boxes: that is how layouts are built.",
    "    GtkWidget *buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);",
    "    GtkWidget *ok = gtk_button_new_with_label(\"OK\");",
    "    GtkWidget *cancel = gtk_button_new_with_label(\"Cancel\");",
    "",
    "    // A child normally takes only the width its content needs.",
    "    // hexpand lets it grow into the spare room.",
    "    gtk_widget_set_hexpand(ok, TRUE);",
    "    gtk_widget_set_hexpand(cancel, TRUE);",
    "",
    "    // append() adds a child at the END of the box, so the order of",
    "    // the calls is the order on screen.",
    "    gtk_box_append(GTK_BOX(buttons), ok);",
    "    gtk_box_append(GTK_BOX(buttons), cancel);",
};
static const char *const EX2_P3[] = {
    "    // Fill the vertical box, top to bottom.",
    "    gtk_box_append(GTK_BOX(column), prompt);",
    "    gtk_box_append(GTK_BOX(column), entry);",
    "    gtk_box_append(GTK_BOX(column), check);",
    "    gtk_box_append(GTK_BOX(column), buttons);",
    "",
    "    // The vertical box is the window's one child.",
    "    gtk_window_set_child(GTK_WINDOW(window), column);",
    "    gtk_window_present(GTK_WINDOW(window));",
    "}",
    "",
    "int main(int argc, char **argv)",
    "{",
    "    // Same four lines as module 1: create, connect, run, release.",
    "    GtkApplication *app = gtk_application_new(",
    "        \"com.example.layout\", G_APPLICATION_DEFAULT_FLAGS);",
    "    g_signal_connect(app, \"activate\", G_CALLBACK(activate), NULL);",
    "    int status = g_application_run(G_APPLICATION(app), argc, argv);",
    "    g_object_unref(app);",
    "    return status;",
    "}",
};
static const char *const GOOD_APPEND[] = {
    "gtk_box_append(GTK_BOX(column), prompt);",
};
static const char *const BAD_APPEND2[] = {
    "gtk_box_append(GTK_BOX(box1), button);",
    "gtk_box_append(GTK_BOX(box2), button);   // already in box1",
};
static const char *const GOOD_HALIGN[] = {
    "gtk_widget_set_halign(prompt, GTK_ALIGN_START);",
};
static const char *const BAD_EXPAND[] = {
    "gtk_widget_set_hexpand(ok, TRUE);       // expecting equal buttons",
    "gtk_widget_set_hexpand(cancel, TRUE);",
};
static const char *const GOOD_EQUAL[] = {
    "gtk_box_set_homogeneous(GTK_BOX(buttons), TRUE);",
    "gtk_widget_set_hexpand(ok, TRUE);",
    "gtk_widget_set_hexpand(cancel, TRUE);",
};
static const char *const EX3_P0[] = {
    "// greeter.c : read a text field and a tick box when a button is used.",
    "",
    "#include <gtk/gtk.h>",
    "",
    "// The widgets that more than one function needs, kept together.",
    "struct Ui {",
    "    GtkWidget *entry;    // where the user types a name",
    "    GtkWidget *shout;    // the \"Shout\" tick box",
    "    GtkWidget *result;   // the label that shows the greeting",
    "};",
    "",
    "// One file-level variable. \"static\" at file level means: visible only",
    "// in this file, and it lives for the whole run of the program.",
    "static struct Ui ui;",
};
static const char *const EX3_P1[] = {
    "// The callback. GTK passes the object that emitted the signal first,",
    "// then user_data last. Here the first one is ignored, and user_data is",
    "// the &ui we hand over in g_signal_connect below.",
    "static void greet(gpointer source, gpointer user_data)",
    "{",
    "    (void) source;",
    "    struct Ui *u = user_data;   // plain pointer back into a Ui *",
    "",
    "    // The text belongs to the entry: read it, never free it.",
    "    const char *name = gtk_editable_get_text(GTK_EDITABLE(u->entry));",
    "    if (*name == '\\0') {   // nothing typed: say so and stop",
    "        gtk_label_set_text(GTK_LABEL(u->result), \"Type a name first\");",
    "        return;",
    "    }",
    "",
    "    // g_strdup_printf builds a NEW string that we own and must free.",
    "    char *message;",
    "    if (gtk_check_button_get_active(GTK_CHECK_BUTTON(u->shout)))",
    "        message = g_strdup_printf(\"HELLO, %s!\", name);",
    "    else",
    "        message = g_strdup_printf(\"Hello, %s.\", name);",
    "",
    "    // set_text copies the text, so we can free our copy straight away.",
    "    gtk_label_set_text(GTK_LABEL(u->result), message);",
    "    g_free(message);",
    "}",
};
static const char *const EX3_P2[] = {
    "static void activate(GtkApplication *app, gpointer user_data)",
    "{",
    "    (void) user_data;",
    "    GtkWidget *window = gtk_application_window_new(app);",
    "    gtk_window_set_title(GTK_WINDOW(window), \"Greeter\");",
    "    gtk_window_set_default_size(GTK_WINDOW(window), 320, 160);",
    "",
    "    GtkWidget *column = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);",
    "    gtk_widget_set_margin_top(column, 12);",
    "    gtk_widget_set_margin_bottom(column, 12);",
    "    gtk_widget_set_margin_start(column, 12);",
    "    gtk_widget_set_margin_end(column, 12);",
    "",
    "    // Fill in the shared struct as the widgets are created.",
    "    ui.entry = gtk_entry_new();",
    "    ui.shout = gtk_check_button_new_with_label(\"Shout\");",
    "    ui.result = gtk_label_new(\"\");",
    "    GtkWidget *button = gtk_button_new_with_label(\"Greet\");",
    "",
    "    // Two different signals, one handler. The last argument, &ui,",
    "    // arrives in greet() as user_data.",
    "    g_signal_connect(button, \"clicked\", G_CALLBACK(greet), &ui);",
    "    g_signal_connect(ui.entry, \"activate\", G_CALLBACK(greet), &ui);",
    "",
    "    gtk_box_append(GTK_BOX(column), ui.entry);",
    "    gtk_box_append(GTK_BOX(column), ui.shout);",
    "    gtk_box_append(GTK_BOX(column), button);",
    "    gtk_box_append(GTK_BOX(column), ui.result);",
    "    gtk_window_set_child(GTK_WINDOW(window), column);",
    "    gtk_window_present(GTK_WINDOW(window));",
    "}",
    "",
    "int main(int argc, char **argv)",
    "{",
    "    GtkApplication *app = gtk_application_new(",
    "        \"com.example.greeter\", G_APPLICATION_DEFAULT_FLAGS);",
    "    g_signal_connect(app, \"activate\", G_CALLBACK(activate), NULL);",
    "    int status = g_application_run(G_APPLICATION(app), argc, argv);",
    "    g_object_unref(app);",
    "    return status;",
    "}",
};
static const char *const GOOD_STATIC[] = {
    "static struct Ui ui;   // file level: lives for the whole run",
};
static const char *const BAD_LOCAL[] = {
    "static void activate(GtkApplication *app, gpointer user_data)",
    "{",
    "    struct Ui ui;   // local: ends when activate() returns",
    "    g_signal_connect(button, \"clicked\", G_CALLBACK(greet), &ui);",
    "}",
};
static const char *const BAD_NULL_STATE[] = {
    "// the handler runs, but ui.entry was never assigned (NULL)",
    "gtk_editable_get_text(GTK_EDITABLE(u->entry));",
};
static const char *const GOOD_GET[] = {
    "const char *name = gtk_editable_get_text(GTK_EDITABLE(u->entry));",
};
static const char *const BAD_GET_LABEL[] = {
    "gtk_editable_get_text(GTK_EDITABLE(u->result));   // a label",
};
static const char *const GOOD_FMT[] = {
    "char *message = g_strdup_printf(\"Hello, %s.\", name);",
    "gtk_label_set_text(GTK_LABEL(u->result), message);",
    "g_free(message);   // the label copied it, so ours can go",
};
static const char *const EX4_P0[] = {
    "// todo.c : a list that grows and shrinks while the program runs.",
    "",
    "#include <gtk/gtk.h>",
    "",
    "struct Ui {",
    "    GtkWidget *entry;    // where a new task is typed",
    "    GtkWidget *list;     // a vertical box holding one row per task",
    "    GtkWidget *status;   // the \"N left\" label",
    "};",
    "",
    "static struct Ui ui;",
    "",
    "// Recount and redraw the \"N left\" label. There is no counter variable:",
    "// we ask the rows themselves, so the label cannot disagree with them.",
    "static void refresh(void)",
    "{",
    "    int left = 0;",
    "",
    "    // Walk the list's children: first child, then each next sibling.",
    "    for (GtkWidget *row = gtk_widget_get_first_child(ui.list);",
    "         row != NULL; row = gtk_widget_get_next_sibling(row)) {",
    "        // Each row has its check button stored under the name \"check\".",
    "        GtkWidget *check = g_object_get_data(G_OBJECT(row), \"check\");",
    "        if (!gtk_check_button_get_active(GTK_CHECK_BUTTON(check)))",
    "            left++;",
    "    }",
    "",
    "    char *text = g_strdup_printf(\"%d left\", left);   // we own this",
    "    gtk_label_set_text(GTK_LABEL(ui.status), text);  // label copies it",
    "    g_free(text);                                    // so we free ours",
    "}",
};
static const char *const EX4_P1[] = {
    "// Ticking a box emits \"toggled\": just recount.",
    "static void on_toggled(gpointer source, gpointer user_data)",
    "{",
    "    (void) source;",
    "    (void) user_data;",
    "    refresh();",
    "}",
    "",
    "// The \"x\" button. user_data is the row it belongs to.",
    "static void on_delete(gpointer source, gpointer row)",
    "{",
    "    (void) source;",
    "    // Remove the row from the list. The list lets go of it, which",
    "    // frees it. We do not touch `row` again after this line.",
    "    gtk_box_remove(GTK_BOX(ui.list), GTK_WIDGET(row));",
    "    refresh();",
    "}",
};
static const char *const EX4_P2[] = {
    "// Add button, or Enter in the text field.",
    "static void on_add(gpointer source, gpointer user_data)",
    "{",
    "    (void) source;",
    "    (void) user_data;",
    "    const char *text = gtk_editable_get_text(GTK_EDITABLE(ui.entry));",
    "    if (*text == '\\0')   // ignore an empty entry",
    "        return;",
    "",
    "    // Build one row: a horizontal box with a check button and an \"x\".",
    "    GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);",
    "    GtkWidget *check = gtk_check_button_new_with_label(text);",
    "    GtkWidget *del = gtk_button_new_with_label(\"x\");",
    "    gtk_widget_set_hexpand(check, TRUE);   // push the \"x\" to the right",
    "",
    "    // Attach the check button to the row under the name \"check\".",
    "    g_object_set_data(G_OBJECT(row), \"check\", check);",
    "",
    "    g_signal_connect(check, \"toggled\", G_CALLBACK(on_toggled), NULL);",
    "    g_signal_connect(del, \"clicked\", G_CALLBACK(on_delete), row);",
    "    gtk_box_append(GTK_BOX(row), check);",
    "    gtk_box_append(GTK_BOX(row), del);",
    "    gtk_box_append(GTK_BOX(ui.list), row);   // now it is on screen",
    "",
    "    gtk_editable_set_text(GTK_EDITABLE(ui.entry), \"\");   // clear field",
    "    refresh();",
    "}",
};
static const char *const EX4_P3[] = {
    "// Remove every ticked row.",
    "static void on_clear_done(gpointer source, gpointer user_data)",
    "{",
    "    (void) source;",
    "    (void) user_data;",
    "    GtkWidget *row = gtk_widget_get_first_child(ui.list);",
    "    while (row != NULL) {",
    "        // Read the next row BEFORE a possible remove: afterwards",
    "        // `row` may be freed and cannot be asked for its sibling.",
    "        GtkWidget *next = gtk_widget_get_next_sibling(row);",
    "        GtkWidget *check = g_object_get_data(G_OBJECT(row), \"check\");",
    "        if (gtk_check_button_get_active(GTK_CHECK_BUTTON(check)))",
    "            gtk_box_remove(GTK_BOX(ui.list), row);",
    "        row = next;",
    "    }",
    "    refresh();",
    "}",
};
static const char *const EX4_P4[] = {
    "static void activate(GtkApplication *app, gpointer user_data)",
    "{",
    "    (void) user_data;",
    "    GtkWidget *window = gtk_application_window_new(app);",
    "    gtk_window_set_title(GTK_WINDOW(window), \"Todo\");",
    "    gtk_window_set_default_size(GTK_WINDOW(window), 400, 420);",
    "",
    "    GtkWidget *column = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);",
    "    gtk_widget_set_margin_top(column, 8);",
    "    gtk_widget_set_margin_bottom(column, 8);",
    "    gtk_widget_set_margin_start(column, 8);",
    "    gtk_widget_set_margin_end(column, 8);",
    "",
    "    // Top row: text field, Add, Clear done.",
    "    GtkWidget *top = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);",
    "    ui.entry = gtk_entry_new();",
    "    gtk_widget_set_hexpand(ui.entry, TRUE);   // field takes spare width",
    "    GtkWidget *add = gtk_button_new_with_label(\"Add\");",
    "    GtkWidget *clear = gtk_button_new_with_label(\"Clear done\");",
    "    gtk_box_append(GTK_BOX(top), ui.entry);",
    "    gtk_box_append(GTK_BOX(top), add);",
    "    gtk_box_append(GTK_BOX(top), clear);",
    "",
    "    // The list of rows, inside a scrolling area so it can grow.",
    "    ui.list = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);",
    "    GtkWidget *scroll = gtk_scrolled_window_new();",
    "    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), ui.list);",
    "    gtk_widget_set_vexpand(scroll, TRUE);   // take the spare height",
    "",
    "    ui.status = gtk_label_new(\"0 left\");",
    "    gtk_widget_set_halign(ui.status, GTK_ALIGN_START);",
    "",
    "    g_signal_connect(ui.entry, \"activate\", G_CALLBACK(on_add), NULL);",
    "    g_signal_connect(add, \"clicked\", G_CALLBACK(on_add), NULL);",
    "    g_signal_connect(clear, \"clicked\", G_CALLBACK(on_clear_done), NULL);",
    "",
    "    gtk_box_append(GTK_BOX(column), top);",
    "    gtk_box_append(GTK_BOX(column), scroll);",
    "    gtk_box_append(GTK_BOX(column), ui.status);",
    "    gtk_window_set_child(GTK_WINDOW(window), column);",
    "    gtk_window_present(GTK_WINDOW(window));",
    "}",
    "",
    "int main(int argc, char **argv)",
    "{",
    "    GtkApplication *app = gtk_application_new(",
    "        \"com.example.todo\", G_APPLICATION_DEFAULT_FLAGS);",
    "    g_signal_connect(app, \"activate\", G_CALLBACK(activate), NULL);",
    "    int status = g_application_run(G_APPLICATION(app), argc, argv);",
    "    g_object_unref(app);",
    "    return status;",
    "}",
};
static const char *const BAD_REMOVE_STRANGER[] = {
    "gtk_box_remove(GTK_BOX(box), other);   // other is not in box",
};
static const char *const BAD_LOOP_REMOVE[] = {
    "for (row = first; row; row = gtk_widget_get_next_sibling(row))",
    "    if (ticked(row))",
    "        gtk_box_remove(GTK_BOX(ui.list), row);",
};
static const char *const GOOD_LOOP_REMOVE[] = {
    "GtkWidget *next = gtk_widget_get_next_sibling(row);   // read FIRST",
    "if (ticked(row))",
    "    gtk_box_remove(GTK_BOX(ui.list), row);",
    "row = next;",
};
static const char *const GOOD_DATA[] = {
    "g_object_set_data(G_OBJECT(row), \"check\", check);",
    "GtkWidget *c = g_object_get_data(G_OBJECT(row), \"check\");",
};
static const char *const BAD_DATA[] = {
    "GtkWidget *c = g_object_get_data(G_OBJECT(row), \"chek\");   // typo",
};
static const char *const GOOD_SCROLL[] = {
    "gtk_widget_set_vexpand(scroll, TRUE);   // take the spare height",
};

void lesson_02_layout(void)
{
    title("MODULE 2 - GTK: LAYOUT AND THE COMMON WIDGETS");

    heading("WHAT YOU WILL HAVE AT THE END");

    printf("  A form: a label, a text field, a tick box and two buttons\n");
    printf("  side by side, laid out the way you want. It does nothing yet.\n");
    printf("  Making it do something is module 3.\n");

    heading("THE IDEA: WIDGETS FORM A TREE");

    printf("  Module 1 ended on a limit: a window holds ONE child. A real\n");
    printf("  window has many widgets, so the one child is a container: a\n");
    printf("  widget whose job is to hold and arrange other widgets.\n\n");

    printf("  The simplest container is GtkBox. It lays its children out\n");
    printf("  in a line, vertical or horizontal. A box can hold another\n");
    printf("  box, so a column of rows is just a vertical box whose\n");
    printf("  children are horizontal boxes. Every layout in this course\n");
    printf("  is built that way.\n\n");

    printf("  Every widget has at most one parent. The window is the root\n");
    printf("  of the tree, and the widgets you can see are its leaves.\n\n");

    printf("  Size works like a negotiation. Each child asks for the room\n");
    printf("  its content needs, and the box gives it that. Spare room\n");
    printf("  goes only to children that asked to expand. Part 3 shows\n");
    printf("  measured numbers.\n");

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 1 OF 4: THE COLUMN");

    printf("  Save this as layout.c. All four parts are one file.\n\n");
    code(LINES(EX2_P0));

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 2 OF 4: THE WIDGETS");

    code(LINES(EX2_P1));

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 3 OF 4: THE ROW OF BUTTONS");

    code(LINES(EX2_P2));

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 4 OF 4: ASSEMBLE AND RUN");

    code(LINES(EX2_P3));

    printf("\n  Compile and run it as in module 1. You should see a\n");
    printf("  window with a name field, a tick box and two buttons.\n");

    wait_enter();
    clear_screen();
    heading("REFERENCE 1/3: BOXES");

    ref_head("GtkWidget *gtk_box_new(orientation, spacing)");
    ref_line("Does:", "creates a box that lays its children out in a line.");
    ref_line("Takes:", "orientation  GTK_ORIENTATION_VERTICAL or");
    ref_line("", "             GTK_ORIENTATION_HORIZONTAL.");
    ref_line("", "spacing      the gap between children, in pixels.");

    ref_head("void gtk_box_append(box, child)");
    ref_line("Does:", "adds child at the end of the box. Call order is the");
    ref_line("", "order on screen.");
    ref_line("Takes:", "box  a GtkBox * (use GTK_BOX), child  any widget.");
    printf("\n");
    code_good(LINES(GOOD_APPEND));
    code_bad(LINES(BAD_APPEND2));
    bad("Gtk-CRITICAL: gtk_box_append: assertion");
    more("'gtk_widget_get_parent (child) == NULL' failed");
    more("The second call is ignored: read back, the button stayed in box1.");
    more("A widget has one parent. To move it, remove it first.");

    ref_head("gtk_box_pack_start(...), gtk_container_add(...)  [GTK3]");
    ref_line("Status:", "do not exist in GTK4. Tutorials written for GTK3");
    ref_line("", "use them, which is the usual way to get stuck.");
    bad("error: implicit declaration of function 'gtk_box_pack_start'");
    more("error: implicit declaration of function 'gtk_container_add'");
    more("error: implicit declaration of function 'gtk_widget_show_all'");
    more("(gcc 14.2.1, GTK 4.22.5. \"Implicit declaration\" means the header");
    more("you included does not declare it: it is not a typo.)");
    printf("\n    GTK3                            GTK4\n");
    printf("    gtk_box_pack_start(box, w,...)  gtk_box_append(box, w)\n");
    printf("    gtk_container_add(window, w)    gtk_window_set_child(window, w)\n");
    printf("    gtk_widget_show_all(window)     gtk_window_present(window)\n");

    wait_enter();
    clear_screen();
    heading("REFERENCE 2/3: MARGINS AND THE FOUR WIDGETS");

    ref_head("void gtk_widget_set_margin_top(widget, pixels)");
    ref_line("Also:", "gtk_widget_set_margin_bottom, _start, _end.");
    ref_line("Does:", "adds empty space around a widget, one side per call.");
    ref_line("", "(start and end are left and right in left-to-right text.)");
    ref_line("Note:", "the window has no padding setting of its own. Space");
    ref_line("", "around the edge is a margin on its outer box.");

    ref_head("GtkWidget *gtk_label_new(text)");
    ref_line("Does:", "text the user reads but cannot edit.");

    ref_head("GtkWidget *gtk_entry_new()");
    ref_line("Does:", "a one-line text field the user can type in.");

    ref_head("GtkWidget *gtk_check_button_new_with_label(text)");
    ref_line("Does:", "a tick box with text beside it.");

    ref_head("void gtk_widget_set_halign(widget, align)");
    ref_line("Does:", "where a widget sits across the space it is given.");
    ref_line("Takes:", "GTK_ALIGN_START (left), GTK_ALIGN_CENTER, GTK_ALIGN_END");
    ref_line("", "(right), or GTK_ALIGN_FILL (stretch, the default).");
    printf("\n");
    code_good(LINES(GOOD_HALIGN));
    tip("Measured on the example: the \"Your name:\" label was 336 pixels");
    more("wide (its whole column) with the default, and 79 with START.");
    more("A label you want at the left edge needs START.");

    wait_enter();
    clear_screen();
    heading("REFERENCE 3/3: SIZE, EXPAND AND EQUAL WIDTHS");

    ref_head("void gtk_widget_set_hexpand(widget, TRUE)");
    ref_line("Does:", "lets the widget grow into spare room in a horizontal");
    ref_line("", "box. Its vertical twin is gtk_widget_set_vexpand.");

    ref_head("void gtk_box_set_homogeneous(box, TRUE)");
    ref_line("Does:", "makes every child of the box the same size.");

    printf("\n  Measured on this machine (window 360 px wide; your theme and\n");
    printf("  font change the numbers, not which row is equal):\n\n");

    printf("    no expand                       OK  22 px   Cancel  49 px\n");
    printf("    hexpand on both                 OK 117 px   Cancel 143 px\n");
    printf("    hexpand + homogeneous           OK 130 px   Cancel 130 px\n\n");

    code_bad(LINES(BAD_EXPAND));
    bad("Both buttons grew, but they are not equal. Each starts from its");
    more("own text width: the 27 px gap between OK and Cancel (22 and 49)");
    more("comes back as 26 px (117 and 143). No error is printed.");
    printf("\n");
    code_good(LINES(GOOD_EQUAL));

    wait_enter();
    clear_screen();
    heading("GOOD PRACTICE");

    tip("Name each widget for what it is: prompt, entry, ok, cancel. A");
    more("layout with names like w1, w2, w3 cannot be read back later.");
    tip("Build a row as its own box, and a column as a box of rows. One");
    more("box does one job.");
    tip("Put the margin on the outer box once, and the gap between");
    more("children in the box's spacing. Do not add margins to each child");
    more("to fake spacing.");
    tip("Ask for expansion on exactly the widgets that should grow: the");
    more("text field, not every widget \"just in case\".");
    tip("Group the calls for one widget together, with a comment per");
    more("group, so activate() reads top to bottom like the screen.");
    tip("Run from a terminal. Every CRITICAL line in this module was a");
    more("call that was ignored.");

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
    printf("    gcc -Wall -Wextra main.c -o app $(pkg-config --cflags --libs gtk4)\n");

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

    heading("WHAT YOU WILL HAVE AT THE END");

    printf("  A greeter. You type a name, tick Shout, and press Greet or\n");
    printf("  Enter. The window shows a greeting built from what you\n");
    printf("  typed. This is the first window that reads your input.\n");

    heading("THE IDEA: A CALLBACK SEES ONLY WHAT IT IS HANDED");

    printf("  A callback is an ordinary C function. When GTK calls it, the\n");
    printf("  only things it has are its parameters: the object that\n");
    printf("  emitted the signal, and one extra pointer, user_data, that\n");
    printf("  you chose when you connected it. Nothing else is in scope.\n\n");

    printf("  So a handler that must read the text field when a button is\n");
    printf("  clicked needs some way to reach that field. There are three:\n\n");

    printf("    1. a global variable for each widget;\n");
    printf("    2. user_data carrying one widget (module 1's label);\n");
    printf("    3. user_data carrying a struct that holds all the widgets\n");
    printf("       the handlers share.\n\n");

    printf("  This course uses the third. It scales: adding a widget adds\n");
    printf("  one field, not one more parameter on every function.\n");

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 1 OF 3: THE SHARED STATE");

    printf("  Save this as greeter.c. All three parts are one file.\n\n");
    code(LINES(EX3_P0));

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 2 OF 3: THE HANDLER");

    code(LINES(EX3_P1));

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 3 OF 3: BUILD THE WINDOW");

    code(LINES(EX3_P2));

    printf("\n  Checked by driving the real window from a test program:\n\n");
    printf("    click Greet, field empty         ->  Type a name first\n");
    printf("    type Ana, click Greet            ->  Hello, Ana.\n");
    printf("    tick Shout, press Enter          ->  HELLO, Ana!\n");

    wait_enter();
    clear_screen();
    heading("REFERENCE 1/3: user_data AND WHERE THE STRUCT LIVES");

    ref_head("g_signal_connect(instance, \"signal\", G_CALLBACK(fn), &ui)");
    ref_line("Does:", "the fourth argument arrives in fn as its last");
    ref_line("", "parameter, user_data. It is a plain pointer: you turn");
    ref_line("", "it back into your type with a line like");
    ref_line("", "struct Ui *u = user_data;");
    printf("\n");
    code_good(LINES(GOOD_STATIC));
    code_bad(LINES(BAD_LOCAL));
    bad("gcc -std=c11 -Wall -Wextra printed nothing for this file. It is");
    more("still a bug. activate() builds the window and returns at once,");
    more("and a local variable ends with its function. When the button is");
    more("clicked later, &ui points at stack space something else is using:");
    more("undefined behaviour, so this course does not run it.");
    printf("\n");
    code_bad(LINES(BAD_NULL_STATE));
    bad("Gtk-CRITICAL: gtk_editable_get_text: assertion");
    more("'GTK_IS_EDITABLE (editable)' failed");
    more("Fill every field of the struct before the window is shown.");

    wait_enter();
    clear_screen();
    heading("REFERENCE 2/3: TEXT IN AND OUT");

    ref_head("const char *gtk_editable_get_text(editable)");
    ref_line("Does:", "returns what is typed in a text field.");
    ref_line("Takes:", "an entry (use GTK_EDITABLE(entry)).");
    ref_line("Owner:", "GTK. It is good until the text next changes. You do");
    ref_line("", "not free it. To keep it longer, copy it with g_strdup.");
    printf("\n");
    code_good(LINES(GOOD_GET));
    code_bad(LINES(BAD_GET_LABEL));
    bad("GLib-GObject-CRITICAL: invalid cast from 'GtkLabel' to 'GtkEditable'");
    more("Gtk-CRITICAL: gtk_editable_get_text: assertion");
    more("'GTK_IS_EDITABLE (editable)' failed");
    more("It returned NULL. Passing that to printf or strlen would crash.");
    bad("free(name) on the returned text is undefined behaviour (the text");
    more("belongs to the entry), so it is explained here and not run.");

    ref_head("void gtk_editable_set_text(editable, text)");
    ref_line("Does:", "replaces the text in the field; \"\" empties it.");

    ref_head("void gtk_label_set_text(label, text)");
    ref_line("Does:", "replaces a label's text. The label keeps its own copy,");
    ref_line("", "so your string may be freed straight afterwards.");

    ref_head("gboolean gtk_check_button_get_active(check)");
    ref_line("Does:", "TRUE when the box is ticked, FALSE when not.");

    wait_enter();
    clear_screen();
    heading("REFERENCE 3/3: BUILDING A STRING, AND WHO FREES IT");

    ref_head("char *g_strdup_printf(format, ...)");
    ref_line("Does:", "like printf, but builds and returns a new string.");
    ref_line("Owner:", "YOU. The memory is yours to release with g_free.");
    ref_head("void g_free(pointer)");
    ref_line("Does:", "releases memory from g_strdup_printf and the other");
    ref_line("", "g_ allocators. Use g_free for these, not free().");
    printf("\n");
    code_good(LINES(GOOD_FMT));
    bad("Forgetting g_free leaks one string per click. Nothing is printed,");
    more("which is why it needs a habit: every g_strdup_printf gets a");
    more("g_free on every path out of the function.");

    ref_head("signal \"activate\" on a GtkEntry");
    ref_line("Does:", "emitted when the user presses Enter in the field.");
    ref_line("", "Connecting it to the same handler as the button's \"clicked\"");
    ref_line("", "makes Enter and Greet do the same thing.");
    ref_line("Note:", "greet is written (gpointer source, gpointer user_data):");
    ref_line("", "both signals pass an object pointer first and user_data");
    ref_line("", "last. A signal with other parameters needs another");
    ref_line("", "signature. Check docs.gtk.org for each new signal.");

    wait_enter();
    clear_screen();
    heading("GOOD PRACTICE");

    tip("Keep shared widgets in one struct, at file level, and fill every");
    more("field before the window is shown.");
    tip("Return early on bad input (an empty name) instead of nesting the");
    more("whole handler inside an if.");
    tip("Every string you own gets exactly one g_free. Every string GTK");
    more("owns gets none. When unsure, the function's documentation says");
    more("who owns the result.");
    tip("One handler per action, connected to every signal that means that");
    more("action, instead of copying the code.");
    tip("Do not trust a clean compile for pointer lifetimes: the local");
    more("struct above compiled with no warnings at all.");

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
    printf("    gcc -Wall -Wextra main.c -o app $(pkg-config --cflags --libs gtk4)\n");

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

    heading("WHAT YOU WILL HAVE AT THE END");

    printf("  A todo list. You type a task and press Add or Enter, and a\n");
    printf("  row appears with a tick box and an x. Tick tasks, delete\n");
    printf("  them, clear the finished ones, and a label tells you how\n");
    printf("  many are left.\n");

    heading("THE IDEA: THE WIDGET TREE CAN CHANGE WHILE IT RUNS");

    printf("  Everything so far built its widgets once, in activate(). A\n");
    printf("  todo list cannot: rows appear when the user presses Add and\n");
    printf("  vanish when they press x. The calls are the same ones from\n");
    printf("  modules 1 to 3, made from inside a handler, appended to a box\n");
    printf("  that is already on screen. Three ideas make that manageable:\n\n");

    printf("    1. Each row is a widget, so keep a row's data ON the row.\n");
    printf("    2. Do not keep a copy of a fact the widgets already hold.\n");
    printf("       The \"N left\" count is counted from the rows each time.\n");
    printf("    3. A removed widget may be freed, so never touch it after\n");
    printf("       the remove call.\n\n");

    printf("  Read back from the real window: after adding the first row,\n");
    printf("  it was on screen (mapped) and 34 pixels tall, with no\n");
    printf("  redraw call anywhere in the program.\n");

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 1 OF 5: THE STATE AND THE COUNT");

    printf("  Save this as todo.c. All five parts are one file.\n\n");
    code(LINES(EX4_P0));

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 2 OF 5: TICKING AND DELETING");

    code(LINES(EX4_P1));

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 3 OF 5: ADDING A ROW");

    code(LINES(EX4_P2));

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 4 OF 5: CLEARING DONE ROWS");

    code(LINES(EX4_P3));

    wait_enter();
    clear_screen();
    heading("THE PROGRAM, PART 5 OF 5: BUILD THE WINDOW");

    code(LINES(EX4_P4));

    printf("\n  Checked by driving the real window from a test program:\n\n");
    printf("    add a, b, c, d                     ->  4 left, 4 rows\n");
    printf("    add an empty line                  ->  nothing added\n");
    printf("    tick two, delete one ticked row    ->  2 left, 3 rows\n");
    printf("    delete one unticked row            ->  1 left, 2 rows\n");
    printf("    add e, f; tick two; Clear done     ->  2 left, 2 rows\n");
    printf("    tick everything; Clear done        ->  0 left, 0 rows\n");

    wait_enter();
    clear_screen();
    heading("REFERENCE 1/3: WALKING AND CHANGING THE TREE");

    ref_head("GtkWidget *gtk_widget_get_first_child(widget)");
    ref_head("GtkWidget *gtk_widget_get_next_sibling(widget)");
    ref_line("Does:", "walk a container's children: first, then each next one.");
    ref_line("Returns:", "NULL when there is no more. That ends the loop.");

    ref_head("void gtk_box_remove(box, child)");
    ref_line("Does:", "takes child out of the box. The box lets go of it, and");
    ref_line("", "if nothing else holds it, it is freed.");
    printf("\n");
    code_bad(LINES(BAD_REMOVE_STRANGER));
    bad("Gtk-CRITICAL: gtk_box_remove: assertion");
    more("'gtk_widget_get_parent (child) == (GtkWidget *)box' failed");
    more("Nothing was removed. Only remove children of that exact box.");
    printf("\n");
    code_bad(LINES(BAD_LOOP_REMOVE));
    bad("After the remove, row may be freed, and the loop then asks it for");
    more("its next sibling. Undefined behaviour, so explained and not run.");
    code_good(LINES(GOOD_LOOP_REMOVE));
    more("Same rule as deleting from a linked list in plain C: save the");
    more("link before you free the node.");

    wait_enter();
    clear_screen();
    heading("REFERENCE 2/3: DATA ON A WIDGET");

    ref_head("void g_object_set_data(object, \"name\", pointer)");
    ref_head("gpointer g_object_get_data(object, \"name\")");
    ref_line("Does:", "attach a pointer to any widget under a text name, and");
    ref_line("", "read it back later. Here each row stores its check button.");
    ref_line("Returns:", "NULL from get_data when the name was never set.");
    printf("\n");
    code_good(LINES(GOOD_DATA));
    code_bad(LINES(BAD_DATA));
    bad("The name is only text, so a typo compiles and returns NULL (read");
    more("back: (nil)). Using that NULL then prints:");
    more("Gtk-CRITICAL: gtk_check_button_get_active: assertion");
    more("'GTK_IS_CHECK_BUTTON (self)' failed");
    more("and the call returns 0 as if the box were not ticked.");

    ref_head("void gtk_check_button_set_active(check, TRUE)");
    ref_line("Does:", "ticks or unticks a box from code. It emits \"toggled\", so");
    ref_line("", "handlers connected to it run, as if the user had clicked.");

    wait_enter();
    clear_screen();
    heading("REFERENCE 3/3: COUNT, DO NOT KEEP A COUNTER");

    printf("  \"N left\" could be an int that Add increments and tick,\n");
    printf("  delete and Clear change: four places that must all agree.\n");
    printf("  The program has no such int. refresh() walks the rows and\n");
    printf("  counts the unticked ones every time. The widgets are the one\n");
    printf("  source of truth, so the label cannot drift away from what is\n");
    printf("  on screen. For a list this size the loop costs nothing.\n\n");

    ref_head("GtkWidget *gtk_scrolled_window_new()");
    ref_head("void gtk_scrolled_window_set_child(scrolled, child)");
    ref_line("Does:", "a viewport with scrollbars around its child, so a");
    ref_line("", "long list scrolls instead of growing the window.");
    printf("\n");
    code_good(LINES(GOOD_SCROLL));
    tip("Read back from the real window (420 px tall, empty list): the");
    more("scroller was 297 px tall with vexpand on and 46 px with it off,");
    more("a thin strip under the buttons.");

    wait_enter();
    clear_screen();
    heading("GOOD PRACTICE");

    tip("Keep a row's data on the row (g_object_set_data) or in a struct");
    more("the row owns. Do not keep a second list that must match.");
    tip("Derive values from the widgets (count, ticked state) instead of");
    more("mirroring them in variables.");
    tip("In a loop that removes, read the next pointer first.");
    tip("After gtk_box_remove, treat the removed widget as gone. Do not");
    more("read it, and do not store it.");
    tip("Reject empty input at the top of the handler, once.");
    tip("Wrap a list in a GtkScrolledWindow from the start. Adding it");
    more("later changes the layout.");

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
    printf("    gcc -Wall -Wextra main.c -o app $(pkg-config --cflags --libs gtk4)\n");

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

