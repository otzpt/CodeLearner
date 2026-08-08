/*
 * Module 1 - GTK: your first window.
 *
 * This course, unlike c/, cpp/, python/ and javascript/, is not a
 * zero-dependency single binary all the way down: the lesson program
 * itself (this file, compiled into gui-course) needs nothing beyond a C
 * compiler, but the code it teaches needs GTK4's development files
 * installed to actually compile and run -- see docs/writing-a-course.md's
 * rule that a course needs nothing beyond its language's toolchain; GTK is
 * not part of C's toolchain, it is a library, the same way it would be a
 * dependency for any other language calling it.
 *
 * Every example in this module was actually compiled and run before being
 * written down, same as every other course:
 *
 *   gcc -Wall -Wextra $(pkg-config --cflags gtk4) main.c -o app $(pkg-config --libs gtk4)
 *
 * compiled warning-free, and running it opened a real window titled
 * "VUXIOCODE GTK Test" (confirmed via the window manager's own client list,
 * not just "the process didn't crash") with a button that changed its own
 * label on click, then closed cleanly.
 */

#include <stdio.h>

#include "lessons.h"
#include "ui.h"

void lesson_01_gtk(void)
{
    title("MODULE 1 - GTK: YOUR FIRST WINDOW");

    heading("GTK IS A C LIBRARY");

    printf("  GTK is a C library for building real, native graphical\n");
    printf("  applications -- windows, buttons, text fields, the works.\n");
    printf("  It is the toolkit behind GNOME and a large share of native\n");
    printf("  Linux desktop apps. \"Written in C\" here does not mean a\n");
    printf("  C program calling into someone else's black box the way,\n");
    printf("  say, a Python program calls into NumPy's C internals --\n");
    printf("  GTK's own public API is C: the functions you call, the\n");
    printf("  structs you pass around, are C, and everything a C program\n");
    printf("  already knows about pointers, structs and function calls\n");
    printf("  carries straight over.\n\n");

    printf("  This course needs something none of CodeLearner's other\n");
    printf("  courses do: GTK4's development files, installed separately\n");
    printf("  from your C compiler.\n\n");

    printf("    Arch:           sudo pacman -S gtk4\n");
    printf("    Debian/Ubuntu:  sudo apt install libgtk-4-dev\n\n");

    printf("  This program itself -- the menu, this text -- needed none of\n");
    printf("  that to build; it is plain C, same as every other course.\n");
    printf("  Only the GTK code shown below, which you compile and run\n");
    printf("  yourself, actually touches GTK.\n");

    wait_enter();
    clear_screen();
    heading("PART 1: pkg-config -- the compile line changes");

    printf("  A plain C program compiles with just:\n\n");
    printf("    gcc -Wall -Wextra test.c -o test\n\n");

    printf("  Try that on a file that #includes <gtk/gtk.h> and it fails\n");
    printf("  before it gets anywhere near your code:\n\n");
    printf("    fatal error: gtk/gtk.h: No such file or directory\n\n");

    printf("  gcc does not know where GTK's headers live, or what to link\n");
    printf("  against -- there could be several versions installed side by\n");
    printf("  side. pkg-config knows, because GTK's install registered\n");
    printf("  itself with it:\n\n");

    printf("    gcc -Wall -Wextra $(pkg-config --cflags gtk4) main.c -o app $(pkg-config --libs gtk4)\n\n");

    printf("  The two $(...) parts are the shell running pkg-config first\n");
    printf("  and substituting its output into the gcc command line --\n");
    printf("  --cflags for the include paths, --libs for what to link.\n");
    printf("  Nothing here is GTK-specific machinery; any library that\n");
    printf("  registers with pkg-config is compiled this same way.\n");

    wait_enter();
    clear_screen();
    heading("PART 2: GtkApplication, not gtk_init");

    printf("  Most GTK tutorials you will find online are GTK3, and GTK3's\n");
    printf("  pattern -- gtk_init() then gtk_main() -- is not how a GTK4\n");
    printf("  app is actually structured. The current, correct shape is\n");
    printf("  GtkApplication:\n\n");

    printf("    static void activate(GtkApplication *app, gpointer user_data)\n");
    printf("    {\n");
    printf("        GtkWidget *window = gtk_application_window_new(app);\n");
    printf("        gtk_window_set_title(GTK_WINDOW(window), \"My App\");\n");
    printf("        gtk_window_present(GTK_WINDOW(window));\n");
    printf("    }\n\n");

    printf("    int main(int argc, char **argv)\n");
    printf("    {\n");
    printf("        GtkApplication *app = gtk_application_new(\n");
    printf("            \"com.example.myapp\", G_APPLICATION_DEFAULT_FLAGS);\n");
    printf("        g_signal_connect(app, \"activate\", G_CALLBACK(activate), NULL);\n");
    printf("        int status = g_application_run(G_APPLICATION(app), argc, argv);\n");
    printf("        g_object_unref(app);\n");
    printf("        return status;\n");
    printf("    }\n\n");

    printf("  main() does not build the window. It builds an application\n");
    printf("  object and hands control to g_application_run(), which\n");
    printf("  drives GTK's own event loop -- reacting to clicks, redraws\n");
    printf("  and window-close, on its own schedule -- and calls activate()\n");
    printf("  once that loop is ready. Building the window inside main(),\n");
    printf("  before that loop exists, is the mistake this structure exists\n");
    printf("  to prevent.\n");

    wait_enter();
    clear_screen();
    heading("PART 3: signals, and what G_CALLBACK does not check");

    printf("  A button does nothing by itself. g_signal_connect wires a\n");
    printf("  function to an event:\n\n");

    printf("    static void on_click(GtkButton *button, gpointer user_data)\n");
    printf("    {\n");
    printf("        gtk_button_set_label(button, \"Clicked!\");\n");
    printf("    }\n\n");
    printf("    GtkWidget *button = gtk_button_new_with_label(\"Click me\");\n");
    printf("    g_signal_connect(button, \"clicked\", G_CALLBACK(on_click), NULL);\n\n");

    printf("  \"clicked\" is a string GTK matches against internally --\n");
    printf("  nothing checks it at compile time, so a typo (\"click\") does\n");
    printf("  not error, it just silently never fires.\n\n");

    printf("  The other half is sharper: G_CALLBACK(on_click) casts your\n");
    printf("  function to a generic function pointer type. It does not\n");
    printf("  check that on_click's parameters match what the \"clicked\"\n");
    printf("  signal actually passes. Get the signature wrong and this\n");
    printf("  still compiles -- GTK calls your function with arguments it\n");
    printf("  was never written for. That is undefined behaviour, not a\n");
    printf("  caught error: ordinary C already treats calling a function\n");
    printf("  pointer through the wrong signature as undefined behaviour,\n");
    printf("  G_CALLBACK's cast just makes it easy to do by accident,\n");
    printf("  since the compiler no longer has the real signature to check\n");
    printf("  it against.\n\n");

    printf("  Correct signatures for the signals you are most likely to use\n");
    printf("  first are documented per-widget in GTK's own reference\n");
    printf("  (docs.gtk.org) -- worth checking any time a callback runs but\n");
    printf("  behaves strangely, rather than assuming your logic is wrong.\n");

    wait_enter();
    clear_screen();
    heading("PART 4: who frees the button?");

    printf("  Nowhere above is there a free(button) or free(window). If\n");
    printf("  you already have the malloc/free habit from plain C -- every\n");
    printf("  malloc owned by exactly one free -- GTK widgets are a\n");
    printf("  different memory model on purpose. Every GtkWidget is a\n");
    printf("  GObject, reference-counted, and gtk_window_set_child() makes\n");
    printf("  the window hold a reference to the button. Destroying the\n");
    printf("  window drops that reference and frees the button along with\n");
    printf("  it. You free the one thing you created yourself with a plain\n");
    printf("  reference -- g_object_unref(app) above -- and let the widget\n");
    printf("  tree own everything under it.\n");

    wait_enter();
    clear_screen();
    heading("THE FULL EXAMPLE");

    printf("  Everything above, as one file. This is exactly what was\n");
    printf("  compiled and run to verify this module -- a window titled\n");
    printf("  \"VUXIOCODE GTK Test\" with a button that relabels itself to\n");
    printf("  \"Clicked!\" on click:\n\n");

    rule();
    printf("  #include <gtk/gtk.h>\n");
    printf("  \n");
    printf("  static void on_click(GtkButton *button, gpointer user_data)\n");
    printf("  {\n");
    printf("      (void) user_data;\n");
    printf("      gtk_button_set_label(button, \"Clicked!\");\n");
    printf("  }\n");
    printf("  \n");
    printf("  static void activate(GtkApplication *app, gpointer user_data)\n");
    printf("  {\n");
    printf("      (void) user_data;\n");
    printf("      GtkWidget *window = gtk_application_window_new(app);\n");
    printf("      gtk_window_set_title(GTK_WINDOW(window), \"VUXIOCODE GTK Test\");\n");
    printf("      gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);\n");
    printf("  \n");
    printf("      GtkWidget *button = gtk_button_new_with_label(\"Click me\");\n");
    printf("      g_signal_connect(button, \"clicked\", G_CALLBACK(on_click), NULL);\n");
    printf("      gtk_window_set_child(GTK_WINDOW(window), button);\n");
    printf("  \n");
    printf("      gtk_window_present(GTK_WINDOW(window));\n");
    printf("  }\n");
    printf("  \n");
    printf("  int main(int argc, char **argv)\n");
    printf("  {\n");
    printf("      GtkApplication *app = gtk_application_new(\n");
    printf("          \"com.vuxiocode.test\", G_APPLICATION_DEFAULT_FLAGS);\n");
    printf("      g_signal_connect(app, \"activate\", G_CALLBACK(activate), NULL);\n");
    printf("      int status = g_application_run(G_APPLICATION(app), argc, argv);\n");
    printf("      g_object_unref(app);\n");
    printf("      return status;\n");
    printf("  }\n");
    rule();

    printf("\n  Build and run it yourself:\n\n");
    printf("    gcc -Wall -Wextra $(pkg-config --cflags gtk4) main.c -o app $(pkg-config --libs gtk4)\n");
    printf("    ./app\n");

    wait_enter();
    clear_screen();
    exercise(1);

    question("gcc file.c -o app fails with \"gtk/gtk.h: No such file or\n"
             "  directory\" even though GTK is installed. What is missing\n"
             "  from the compile line? (one word)",
             "pkg-config",
             "gcc has no built-in idea where GTK's headers or libraries\n"
             "             live; $(pkg-config --cflags gtk4) and $(pkg-config\n"
             "             --libs gtk4) are what supply that.");

    question("Where does a GTK4 app build its window: inside main(), or\n"
             "  inside the function connected to the \"activate\" signal?",
             "activate",
             "main() only builds the GtkApplication and starts\n"
             "             g_application_run()'s event loop. The loop calls\n"
             "             activate() once it is actually ready to show a window.");

    question("g_signal_connect(button, \"clicked\", G_CALLBACK(wrong_signature), NULL)\n"
             "  compiles even if wrong_signature's parameters do not match\n"
             "  what \"clicked\" passes. Why does the compiler allow this?\n"
             "  (one word: what kind of cast is G_CALLBACK doing?)",
             "generic",
             "G_CALLBACK casts to a generic function pointer type, which\n"
             "             erases the real parameter types the compiler could\n"
             "             otherwise have checked -- the mismatch becomes\n"
             "             undefined behaviour at the call, not a compile error.");

    question("Who frees a GtkButton that was added to a window with\n"
             "  gtk_window_set_child()? (one word: you, or GTK)",
             "gtk",
             "GtkWidgets are reference-counted GObjects. The window holds\n"
             "             a reference to its child; destroying the window\n"
             "             drops it, which is what actually frees the button.\n"
             "             You never call free() on it directly.");

    printf("\n  >> BUILD THIS YOURSELF, in a real file\n");
    rule();
    printf("  Starting from the full example above, add a GtkLabel above\n");
    printf("  the button (gtk_label_new(\"...\")) and a container to hold\n");
    printf("  both widgets, since gtk_window_set_child() only takes one\n");
    printf("  child -- GtkBox is the simplest (gtk_box_new, then\n");
    printf("  gtk_box_append for each widget, then set the box as the\n");
    printf("  window's child instead of the button directly). Make the\n");
    printf("  click handler update the label's text instead of the\n");
    printf("  button's own label.\n\n");

    printf("  There is no fixed transcript to match here -- the result is a\n");
    printf("  window, not text. You have it right when a window opens with\n");
    printf("  a label and a button stacked vertically, and clicking the\n");
    printf("  button changes the label's text.\n\n");

    printf("  Compile with:\n");
    printf("    gcc -Wall -Wextra $(pkg-config --cflags gtk4) main.c -o app $(pkg-config --libs gtk4)\n");

    if (ask_yes("Want to see example code?")) {
        printf("\n");
        rule();
        printf("  #include <gtk/gtk.h>\n");
        printf("  \n");
        printf("  static void on_click(GtkButton *button, gpointer user_data)\n");
        printf("  {\n");
        printf("      (void) button;\n");
        printf("      gtk_label_set_text(GTK_LABEL(user_data), \"Clicked!\");\n");
        printf("  }\n");
        printf("  \n");
        printf("  static void activate(GtkApplication *app, gpointer user_data)\n");
        printf("  {\n");
        printf("      (void) user_data;\n");
        printf("      GtkWidget *window = gtk_application_window_new(app);\n");
        printf("      gtk_window_set_title(GTK_WINDOW(window), \"Label Demo\");\n");
        printf("      gtk_window_set_default_size(GTK_WINDOW(window), 300, 120);\n");
        printf("  \n");
        printf("      GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);\n");
        printf("      GtkWidget *label = gtk_label_new(\"Not clicked yet\");\n");
        printf("      GtkWidget *button = gtk_button_new_with_label(\"Click me\");\n");
        printf("  \n");
        printf("      g_signal_connect(button, \"clicked\", G_CALLBACK(on_click), label);\n");
        printf("  \n");
        printf("      gtk_box_append(GTK_BOX(box), label);\n");
        printf("      gtk_box_append(GTK_BOX(box), button);\n");
        printf("      gtk_window_set_child(GTK_WINDOW(window), box);\n");
        printf("  \n");
        printf("      gtk_window_present(GTK_WINDOW(window));\n");
        printf("  }\n");
        printf("  \n");
        printf("  int main(int argc, char **argv)\n");
        printf("  {\n");
        printf("      GtkApplication *app = gtk_application_new(\n");
        printf("          \"com.vuxiocode.labeldemo\", G_APPLICATION_DEFAULT_FLAGS);\n");
        printf("      g_signal_connect(app, \"activate\", G_CALLBACK(activate), NULL);\n");
        printf("      int status = g_application_run(G_APPLICATION(app), argc, argv);\n");
        printf("      g_object_unref(app);\n");
        printf("      return status;\n");
        printf("  }\n");
        rule();
        printf("  user_data is how on_click reaches the label without a\n");
        printf("  global variable: g_signal_connect's fourth argument is\n");
        printf("  handed straight to the callback's user_data parameter,\n");
        printf("  so passing `label` there makes it available inside\n");
        printf("  on_click without either widget needing to know about the\n");
        printf("  other ahead of time.\n");
    }

    wait_enter();
    clear_screen();
    heading("SUMMARY");

    printf("   - GTK needs pkg-config in the compile line; gcc alone\n");
    printf("     cannot find its headers or libraries\n");
    printf("   - a GTK4 app builds its window inside the function connected\n");
    printf("     to \"activate\", not inside main() -- main() only starts\n");
    printf("     the event loop\n");
    printf("   - G_CALLBACK casts away the real function signature, so a\n");
    printf("     mismatched callback compiles and misbehaves instead of\n");
    printf("     failing to build\n");
    printf("   - widgets are reference-counted GObjects, not malloc'd\n");
    printf("     blocks -- the window frees its children when it is freed,\n");
    printf("     you do not free() them yourself\n\n");

    printf("  This is one window with one button. GTK's own reference at\n");
    printf("  docs.gtk.org is where the rest of it -- layout containers,\n");
    printf("  dozens of widgets, CSS styling -- actually lives; this module\n");
    printf("  is the trap-filled first step onto that ground, not the\n");
    printf("  whole of it.\n");
    wait_enter();
}
