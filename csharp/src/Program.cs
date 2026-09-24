// C# course - main menu.
//
// Written in C#, the same way every other course here is written in the
// language it teaches. Top-level statements (no explicit class/Main
// wrapper) on purpose, not just for brevity -- this file demonstrates
// module 1's own subject. MODULES is an array of a small record holding a
// title and an Action, C#'s answer to Java's Runnable and C's function
// pointer table.

// Tier files a module under one of the three menu bands. Display grouping
// only -- the lesson methods never see it.
var modules = new (string Title, Action Run, string Tier)[]
{
    ("Compiling and printing",              LessonsBasics.Lesson01Running,      "BASIC"),
    ("Variables and types",                 LessonsBasics.Lesson02Variables,    "BASIC"),
    ("Reading input",                       LessonsBasics.Lesson03Input,        "BASIC"),
    ("Conditions and pattern matching",     LessonsBasics.Lesson04Conditions,   "BASIC"),
    ("Loops",                               LessonsBasics.Lesson05Loops,        "BASIC"),
    ("Arrays and List<T>",                  LessonsMore.Lesson06Collections,    "INTERMEDIATE"),
    ("Methods and default parameters",      LessonsMore.Lesson07Methods,        "INTERMEDIATE"),
    ("Classes, structs, and records",       LessonsMore.Lesson08ClassesStructs, "ADVANCED"),
    ("Nullable reference types",            LessonsMore.Lesson09Nullable,       "ADVANCED"),
    ("Exceptions and IDisposable",          LessonsMore.Lesson10Exceptions,     "ADVANCED"),
    ("LINQ",                                LessonsAdvanced.Lesson11Linq,       "ADVANCED"),
    ("Async/await",                         () => LessonsAdvanced.Lesson12Async().GetAwaiter().GetResult(), "ADVANCED"),
    ("Generics and interfaces in depth",    LessonsAdvanced.Lesson13GenericsInterfaces, "ADVANCED"),
    ("Delegates and events",                LessonsAdvanced.Lesson14DelegatesEvents,    "ADVANCED"),
};

ShowMenu();

while (true)
{
    Console.Write("\n  Pick a module: ");
    string? choice = Console.ReadLine();

    if (choice is null || choice.Trim() == "0")
    {
        break;
    }

    if (int.TryParse(choice.Trim(), out int n) && n >= 1 && n <= modules.Length)
    {
        Ui.ClearScreen();
        // A module that lets an exception escape should not end the
        // session with a raw stack trace -- every other exit path here
        // prints something and returns to this menu.
        try
        {
            modules[n - 1].Run();
        }
        catch (Exception e)
        {
            Console.WriteLine($"\n  Something went wrong: {e.Message}");
            Ui.WaitEnter();
        }
    }
    else
    {
        Console.WriteLine("\n  Not a valid option.");
        Ui.WaitEnter();
    }

    ShowMenu();
}

Console.WriteLine("\n  See you next time.\n");

void ShowMenu()
{
    Ui.ClearScreen();
    Ui.Title("C# COURSE - FROM TOP-LEVEL STATEMENTS TO IDISPOSABLE");

    string shownTier = "";
    for (int i = 0; i < modules.Length; i++)
    {
        if (modules[i].Tier != shownTier)
        {
            shownTier = modules[i].Tier;
            Console.WriteLine($"\n  -- {shownTier} --");
        }
        Console.WriteLine($"   [{i + 1,2}]  {modules[i].Title}");
    }
    Console.WriteLine("\n   [ 0]  Quit");
    Ui.Rule();
}
