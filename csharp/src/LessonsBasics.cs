// LessonsBasics.cs - modules 1-5.

static class LessonsBasics
{
    public static void Lesson01Running()
    {
        Ui.Title("MODULE 1 - COMPILING AND PRINTING");

        Ui.Heading("PART 1: no class, no Main -- until you need one");

        Console.WriteLine("    Console.WriteLine(\"Hello, World!\");");
        Console.WriteLine();
        Console.WriteLine("  Running:");
        Console.WriteLine("    dotnet run");
        Console.WriteLine();
        Console.WriteLine("    Hello, World!");
        Console.WriteLine();
        Console.WriteLine("  That one line is the whole program. This is a top-level");
        Console.WriteLine("  statements file -- introduced in C# 9 -- and this course's");
        Console.WriteLine("  own Program.cs is written the same way, menu loop and all.");
        Console.WriteLine("  Older C# required the ceremony every method still secretly");
        Console.WriteLine("  runs inside:");
        Console.WriteLine();
        Console.WriteLine("    class Program");
        Console.WriteLine("    {");
        Console.WriteLine("        static void Main(string[] args)");
        Console.WriteLine("        {");
        Console.WriteLine("            Console.WriteLine(\"Hello, World!\");");
        Console.WriteLine("        }");
        Console.WriteLine("    }");
        Console.WriteLine();
        Console.WriteLine("  Both compile to the exact same thing. Top-level statements");
        Console.WriteLine("  removed the boilerplate, not the model -- there is still");
        Console.WriteLine("  exactly one entry point, and only one file per project may");
        Console.WriteLine("  use the short form.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 2: dotnet run is two steps wearing a trenchcoat");

        Console.WriteLine("  `dotnet run` looks like Python or Node running source");
        Console.WriteLine("  directly. It is not -- verified by building separately:");
        Console.WriteLine();
        Console.WriteLine("    dotnet build -c Release");
        Console.WriteLine();
        Console.WriteLine("  produces, in bin/Release/net8.0/:");
        Console.WriteLine();
        Console.WriteLine("    csharp.dll     <- the actual compiled output (IL, not");
        Console.WriteLine("                      machine code -- bytecode, same idea as");
        Console.WriteLine("                      Java's .class file)");
        Console.WriteLine("    csharp         <- a native launcher (\"apphost\") that");
        Console.WriteLine("                      starts the .NET runtime and hands it");
        Console.WriteLine("                      the .dll");
        Console.WriteLine();
        Console.WriteLine("  `dotnet run` builds the .dll, then launches it through the");
        Console.WriteLine("  runtime, in one command -- the same shortcut `java Hello.java`");
        Console.WriteLine("  is for javac+java, not a genuinely different execution model.");
        Console.WriteLine("  C compiles straight to a machine-code binary the OS runs");
        Console.WriteLine("  directly; neither C#'s .dll nor Java's .class is one of those.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 3: Write vs WriteLine");

        Console.WriteLine("    Console.Write(\"a\");");
        Console.WriteLine("    Console.Write(\"b\");");
        Console.WriteLine("    Console.WriteLine(\"c\");");
        Console.WriteLine();
        Console.Write("  Running: ");
        Console.Write("a");
        Console.Write("b");
        Console.WriteLine("c");
        Console.WriteLine();
        Console.WriteLine("  Write adds nothing extra; WriteLine adds the line break.");
        Console.WriteLine("  Same split as Java's print/println, Python's print(end=\"\")");
        Console.WriteLine("  vs the default, C's printf needing \\n written in by hand.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Exercise(1);

        Ui.Question(
            "Does `dotnet build` produce a single machine-code binary\n" +
            "  the way gcc does for C -- or something a runtime has to load?\n" +
            "  (answer: binary or runtime)",
            "runtime",
            "dotnet build produces IL in a .dll (bytecode); the .NET runtime loads and runs it -- two steps, one command with dotnet run.");
        Ui.Question(
            "Can a project have two files that both use top-level\n" +
            "  statements?  (answer: yes or no)",
            "no",
            "Only one file per project may use top-level statements -- there is still exactly one entry point underneath.");

        Ui.Challenge(
            task: new[] { "Write a program that prints exactly:", "Learning C#" },
            input: Array.Empty<string>(),
            expected: new[] { "Learning C#" },
            solution: new[] { "Console.WriteLine(\"Learning C#\");" });

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("SUMMARY");
        Console.WriteLine("  - Top-level statements are sugar over class Program { static");
        Console.WriteLine("    void Main(...) }, not a different execution model.");
        Console.WriteLine("  - dotnet run = dotnet build (source -> IL in a .dll) + launch");
        Console.WriteLine("    through the runtime, in one command.");
        Console.WriteLine("  - Write doesn't add a newline; WriteLine does.");
        Ui.WaitEnter();
    }

    public static void Lesson02Variables()
    {
        Ui.Title("MODULE 2 - VARIABLES AND TYPES");

        Ui.Heading("PART 1: var infers, it does not loosen");

        Console.WriteLine("    var name = \"Ada\";      // inferred: string");
        Console.WriteLine("    var count = 3;          // inferred: int");
        Console.WriteLine("    // name = 5;             <- compile error, still a string");
        Console.WriteLine();
        Console.WriteLine("  var asks the compiler to write the type down for you, once,");
        Console.WriteLine("  at the declaration -- it is not JavaScript's `var` (which");
        Console.WriteLine("  really can hold anything, and be reassigned across types) or");
        Console.WriteLine("  Python's rebinding (no declared type to begin with). Every");
        Console.WriteLine("  variable in C# has exactly one fixed type; var only spares");
        Console.WriteLine("  you from writing it out.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 2: int / int is still int");

        Console.WriteLine("    Console.WriteLine(5 / 2);");
        Console.WriteLine("    Console.WriteLine(5.0 / 2);");
        Console.WriteLine();
        Console.Write("  Running: ");
        Console.WriteLine(5 / 2);
        Console.Write("           ");
        Console.WriteLine(5.0 / 2);
        Console.WriteLine();
        Console.WriteLine("  Same trap as C, C++, Java: int / int truncates toward zero,");
        Console.WriteLine("  the fraction is gone before it could be assigned anywhere.");
        Console.WriteLine("  One side has to be a double before the division runs -- a");
        Console.WriteLine("  cast afterwards is too late, same as every course before");
        Console.WriteLine("  this one has already shown.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 3: value types vs reference types, the split that runs the course");

        Console.WriteLine("  int, double, bool, char, and struct are VALUE types: a");
        Console.WriteLine("  variable IS the data. string, and anything you declare with");
        Console.WriteLine("  class, is a REFERENCE type: the variable holds a pointer to");
        Console.WriteLine("  the data on the heap. Module 8 shows the actual consequence");
        Console.WriteLine("  -- copying a struct copies the value, copying a class copies");
        Console.WriteLine("  the reference -- with both run side by side. For now: this is");
        Console.WriteLine("  the split C++'s stack/heap distinction maps onto most closely,");
        Console.WriteLine("  and the split Python, JavaScript, and Java's non-primitives");
        Console.WriteLine("  don't make at all -- everything non-primitive in those three");
        Console.WriteLine("  is a reference.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Exercise(2);

        Ui.Question(
            "int total = 7 / 2; -- what does total hold?  (answer: a number)",
            "3",
            "int / int truncates toward zero before the result is ever assigned.");
        Ui.Question(
            "Is `int` a value type or a reference type?  (answer: value or reference)",
            "value",
            "int is a value type -- the variable IS the number, not a pointer to one.");

        Ui.Challenge(
            task: new[]
            {
                "Write a program that computes the average of 7, 8, and 10 as a",
                "double (not truncated) and prints exactly:",
                "Average: 8.333333333333334",
            },
            input: Array.Empty<string>(),
            expected: new[] { "Average: 8.333333333333334" },
            solution: new[]
            {
                "double average = (7 + 8 + 10) / 3.0;",
                "Console.WriteLine($\"Average: {average}\");",
            });

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("SUMMARY");
        Console.WriteLine("  - var infers a real, fixed type -- it is not dynamic typing.");
        Console.WriteLine("  - int / int truncates; make one side a double first.");
        Console.WriteLine("  - Value types (int, struct) ARE their data; reference types");
        Console.WriteLine("    (string, class) hold a pointer to it. Module 8 shows why");
        Console.WriteLine("    that difference matters.");
        Ui.WaitEnter();
    }

    public static void Lesson03Input()
    {
        Ui.Title("MODULE 3 - READING INPUT");

        Ui.Heading("PART 1: ReadLine's real return type is string?, not string");

        Console.WriteLine("    string line = Console.ReadLine();");
        Console.WriteLine("    Console.WriteLine(line.Length);");
        Console.WriteLine();
        Console.WriteLine("  Compiling that (verified, not assumed) produces two warnings:");
        Console.WriteLine();
        Console.WriteLine("    warning CS8600: Converting null literal or possible null");
        Console.WriteLine("                    value to non-nullable type.");
        Console.WriteLine("    warning CS8602: Dereference of a possibly null reference.");
        Console.WriteLine();
        Console.WriteLine("  Two warnings, one root cause: ReadLine() returns string?, not");
        Console.WriteLine("  string -- it returns null at end-of-input (piping a file in,");
        Console.WriteLine("  or Ctrl+D on a real terminal). Assigning that to a plain");
        Console.WriteLine("  `string` is flagged at the assignment (CS8600); using it");
        Console.WriteLine("  without checking is flagged again at the point of failure");
        Console.WriteLine("  (CS8602). Module 9 is this warning family's whole subject --");
        Console.WriteLine("  this module just needs one clean way to read a line.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 2: string? and a real check, once");

        Console.WriteLine("    string? raw = Console.ReadLine();");
        Console.WriteLine("    string name = raw ?? \"stranger\";");
        Console.WriteLine("    Console.WriteLine($\"Hi, {name}!\");");
        Console.WriteLine();
        Console.WriteLine("  Typing it string? tells the compiler you know it can be null");
        Console.WriteLine("  -- no warning. ?? (module 9 names it: the null-coalescing");
        Console.WriteLine("  operator) supplies a real string for the null case, and from");
        Console.WriteLine("  that line on, `name` is genuinely a non-nullable string.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 3: parsing what you read");

        Console.WriteLine("    string? raw = Console.ReadLine();");
        Console.WriteLine("    if (int.TryParse(raw, out int age))");
        Console.WriteLine("        Console.WriteLine($\"In 10 years: {age + 10}\");");
        Console.WriteLine("    else");
        Console.WriteLine("        Console.WriteLine(\"That wasn't a number.\");");
        Console.WriteLine();
        Console.WriteLine("  ReadLine always returns text (or null); a number has to be");
        Console.WriteLine("  parsed out of it explicitly, the same forgot-to-convert trap");
        Console.WriteLine("  Python's input() and JavaScript's prompt() both teach. C#'s");
        Console.WriteLine("  answer is TryParse: it returns a bool (did it work?) and hands");
        Console.WriteLine("  the parsed value back through an `out` parameter -- no");
        Console.WriteLine("  exception to catch for input that just wasn't a number.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Exercise(3);

        Ui.Question(
            "What does Console.ReadLine() return at end-of-input, instead\n" +
            "  of throwing?  (answer: one word)",
            "null",
            "ReadLine's real type is string? -- null signals end-of-input rather than an exception.");
        Ui.Question(
            "int.TryParse(\"abc\", out int n) -- does it throw, or return\n" +
            "  false?  (answer: throw or false)",
            "false",
            "TryParse reports success/failure through its bool return value, not an exception -- ordinary bad input isn't exceptional.");

        Ui.Challenge(
            task: new[]
            {
                "Write a program that reads one line and prints exactly:",
                "You typed: <what they typed>",
            },
            input: new[] { "hello" },
            expected: new[] { "You typed: hello" },
            solution: new[]
            {
                "string? line = Console.ReadLine();",
                "Console.WriteLine($\"You typed: {line}\");",
            });

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("SUMMARY");
        Console.WriteLine("  - Console.ReadLine() returns string?, not string -- it can");
        Console.WriteLine("    be null at end-of-input.");
        Console.WriteLine("  - ?? supplies a fallback for the null case.");
        Console.WriteLine("  - int.TryParse returns bool and an out parameter -- no");
        Console.WriteLine("    exception for input that's simply not a number.");
        Ui.WaitEnter();
    }

    public static void Lesson04Conditions()
    {
        Ui.Title("MODULE 4 - CONDITIONS AND PATTERN MATCHING");

        Ui.Heading("PART 1: bool is real, and if needs one");

        Console.WriteLine("    int x = 5;");
        Console.WriteLine("    if (x) { }   // does not compile");
        Console.WriteLine();
        Console.WriteLine("  error CS0029: Cannot implicitly convert type 'int' to 'bool'");
        Console.WriteLine();
        Console.WriteLine("  Same rule as Java and C++: a condition must be genuinely");
        Console.WriteLine("  bool. `if (x = 5)` -- the classic C typo, assignment where");
        Console.WriteLine("  == was meant -- is also a compile error here for the same");
        Console.WriteLine("  reason: an int assignment's result is an int, and an int is");
        Console.WriteLine("  not a bool. C accepts that line with at most a warning.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 2: switch expression -- the fresh idiom here");

        Console.WriteLine("    string Grade(int score) => score switch");
        Console.WriteLine("    {");
        Console.WriteLine("        < 0 or > 100 => \"invalid\",");
        Console.WriteLine("        >= 90 => \"A\",");
        Console.WriteLine("        >= 80 => \"B\",");
        Console.WriteLine("        _ => \"C or below\",");
        Console.WriteLine("    };");
        Console.WriteLine();
        Console.Write("  Running Grade(95), Grade(82), Grade(40): ");
        Console.WriteLine($"{Grade(95)}, {Grade(82)}, {Grade(40)}");
        Console.WriteLine();
        Console.WriteLine("  A switch EXPRESSION, not the older switch STATEMENT with");
        Console.WriteLine("  case/break -- it evaluates to a value directly, patterns are");
        Console.WriteLine("  checked top to bottom, and `_` is the catch-all (not");
        Console.WriteLine("  `default`). Relational patterns like `>= 90` and combinators");
        Console.WriteLine("  like `or` are pattern matching, the same family the `is`");
        Console.WriteLine("  operator belongs to.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 3: is, instead of a cast that might throw");

        Console.WriteLine("    object value = 42;");
        Console.WriteLine("    if (value is int n)");
        Console.WriteLine("        Console.WriteLine($\"It's an int: {n}\");");
        Console.WriteLine();
        object value = 42;
        Console.Write("  Running: ");
        if (value is int n)
        {
            Console.WriteLine($"It's an int: {n}");
        }
        Console.WriteLine();
        Console.WriteLine("  `is` checks the type AND, on success, binds it to a new");
        Console.WriteLine("  variable (n) in the same expression -- no separate cast that");
        Console.WriteLine("  could throw InvalidCastException if you guessed wrong.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Exercise(4);

        Ui.Question(
            "if (5) { ... } -- does this compile in C#?  (answer: yes or no)",
            "no",
            "A condition must genuinely be bool -- an int, even 5, does not implicitly convert.");
        Ui.Question(
            "In a switch expression, what pattern means \"anything else\"?\n" +
            "  (answer: one symbol)",
            "_",
            "_ is the discard pattern -- the catch-all arm, not the word default used in a switch statement.");

        Ui.Challenge(
            task: new[]
            {
                "Write a program that reads an integer and prints exactly one of:",
                "negative",
                "zero",
                "positive",
                "depending on the number typed.",
            },
            input: new[] { "-3" },
            expected: new[] { "negative" },
            solution: new[]
            {
                "int n = int.Parse(Console.ReadLine()!);",
                "string result = n switch { < 0 => \"negative\", 0 => \"zero\", _ => \"positive\" };",
                "Console.WriteLine(result);",
            });

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("SUMMARY");
        Console.WriteLine("  - A condition must be bool -- if (x = 5) does not compile.");
        Console.WriteLine("  - switch expressions evaluate to a value; _ is the catch-all.");
        Console.WriteLine("  - `is` checks a type and binds a variable in one step, no");
        Console.WriteLine("    throwing cast required.");
        Ui.WaitEnter();

        static string Grade(int score) => score switch
        {
            < 0 or > 100 => "invalid",
            >= 90 => "A",
            >= 80 => "B",
            _ => "C or below",
        };
    }

    public static void Lesson05Loops()
    {
        Ui.Title("MODULE 5 - LOOPS");

        Ui.Heading("PART 1: for, while -- and the trap that survives every language");

        Console.WriteLine("    for (int i = 0; i <= 5; i++) Console.Write(i);");
        Console.WriteLine();
        Console.Write("  Running: ");
        for (int i = 0; i <= 5; i++) Console.Write(i);
        Console.WriteLine();
        Console.WriteLine();
        Console.WriteLine("  <= runs one extra time compared to <. Six digits printed for");
        Console.WriteLine("  a range that reads like five. Same off-by-one every C-family");
        Console.WriteLine("  loop can produce -- the fix is reading the condition as \"keep");
        Console.WriteLine("  going while this holds,\" not counting iterations by eye.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 2: foreach and the range operator");

        Console.WriteLine("    int[] scores = { 10, 20, 30 };");
        Console.WriteLine("    foreach (int s in scores) Console.Write(s + \" \");");
        Console.WriteLine();
        int[] scores = { 10, 20, 30 };
        Console.Write("  Running: ");
        foreach (int s in scores) Console.Write(s + " ");
        Console.WriteLine();
        Console.WriteLine();
        Console.WriteLine("    foreach (int i in 1..4) ...   // does NOT compile");
        Console.WriteLine();
        Console.WriteLine("  error CS1579: foreach statement cannot operate on variables");
        Console.WriteLine("  of type 'Range' because 'Range' does not contain a public");
        Console.WriteLine("  instance or extension definition for 'GetEnumerator'");
        Console.WriteLine();
        Console.WriteLine("  1..4 (the range operator) is real C#, but it is not a");
        Console.WriteLine("  foreach-able sequence by itself the way Python's range(1, 4)");
        Console.WriteLine("  is -- its actual job is slicing (arr[1..4]). To loop over a");
        Console.WriteLine("  numeric range in C#, the ordinary for loop above is the tool.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 3: foreach cannot modify what it's iterating");

        Console.WriteLine("    foreach (int s in scores) s = s * 2;   // does NOT compile");
        Console.WriteLine();
        Console.WriteLine("  error CS1656: Cannot assign to 's' because it is a 'foreach");
        Console.WriteLine("  iteration variable'");
        Console.WriteLine();
        Console.WriteLine("  The loop variable is read-only by design -- foreach hands you");
        Console.WriteLine("  each value, it does not hand you a writable slot back into");
        Console.WriteLine("  the array. To change elements in place, index with an");
        Console.WriteLine("  ordinary for loop instead: for (int i = 0; i < scores.Length;");
        Console.WriteLine("  i++) scores[i] *= 2;");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Exercise(5);

        Ui.Question(
            "for (int i = 0; i <= 5; i++) -- how many times does the body\n" +
            "  run?  (answer: a number)",
            "6",
            "0,1,2,3,4,5 -- <= includes the endpoint, one more iteration than < would give.");
        Ui.Question(
            "Can a foreach loop assign a new value to its own iteration\n" +
            "  variable?  (answer: yes or no)",
            "no",
            "The iteration variable is read-only (CS1656) -- use an indexed for loop to mutate elements in place.");

        Ui.Challenge(
            task: new[]
            {
                "Write a program that prints the numbers 1 to 5, each on its own",
                "line, using a for loop.",
            },
            input: Array.Empty<string>(),
            expected: new[] { "1", "2", "3", "4", "5" },
            solution: new[]
            {
                "for (int i = 1; i <= 5; i++)",
                "    Console.WriteLine(i);",
            });

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("SUMMARY");
        Console.WriteLine("  - <= runs one more iteration than < -- read the condition,");
        Console.WriteLine("    don't count by eye.");
        Console.WriteLine("  - foreach reads a sequence; the range operator (1..4) slices,");
        Console.WriteLine("    it isn't itself foreach-able.");
        Console.WriteLine("  - foreach's iteration variable is read-only; use an indexed");
        Console.WriteLine("    for loop to mutate elements.");
        Ui.WaitEnter();
    }
}
