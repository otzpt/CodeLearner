// LessonsMore.cs - modules 6-10.

static class LessonsMore
{
    public static void Lesson06Collections()
    {
        Ui.Title("MODULE 6 - ARRAYS AND LIST<T>");

        Ui.Heading("PART 1: an array is a fixed size, forever");

        Console.WriteLine("    int[] scores = { 10, 20, 30 };");
        Console.WriteLine("    Console.WriteLine(scores.Length);");
        Console.WriteLine();
        int[] scores = { 10, 20, 30 };
        Console.WriteLine($"  Running: {scores.Length}");
        Console.WriteLine();
        Console.WriteLine("  .Length is a property (no parentheses) -- unlike Java's");
        Console.WriteLine("  arr.length (a field, also no parens) and String.length()");
        Console.WriteLine("  (a method, needs them), C# is consistent: .Length is always");
        Console.WriteLine("  a property here, on arrays and on string alike. There is no");
        Console.WriteLine("  way to grow this array -- scores[3] = 40 throws, it does not");
        Console.WriteLine("  extend it.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 2: List<T> is the one that grows");

        Console.WriteLine("    var list = new List<int> { 1, 2, 3 };");
        Console.WriteLine("    list.Add(4);");
        Console.WriteLine("    Console.WriteLine(string.Join(\",\", list));");
        Console.WriteLine();
        var list = new List<int> { 1, 2, 3 };
        list.Add(4);
        Console.WriteLine($"  Running: {string.Join(",", list)}");
        Console.WriteLine();
        Console.WriteLine("  <int> is a generic type parameter -- this List holds only");
        Console.WriteLine("  ints, checked at compile time, not \"a list, figure out what's");
        Console.WriteLine("  in it at runtime\" the way Python's list or JavaScript's Array");
        Console.WriteLine("  work. Reach for List<T> by default; use a plain array only");
        Console.WriteLine("  when the size is genuinely fixed and known up front.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 3: out of bounds, and == on arrays vs strings");

        Console.WriteLine("    Console.WriteLine(scores[5]);");
        Console.WriteLine();
        Console.WriteLine("  Unhandled exception. System.IndexOutOfRangeException: Index");
        Console.WriteLine("  was outside the bounds of the array.");
        Console.WriteLine();
        Console.WriteLine("    int[] a = { 1, 2, 3 };");
        Console.WriteLine("    int[] b = { 1, 2, 3 };");
        Console.WriteLine("    Console.WriteLine(a == b);        // false");
        Console.WriteLine("    Console.WriteLine(\"hi\" == \"h\" + \"i\"); // true");
        Console.WriteLine();
        int[] a = { 1, 2, 3 };
        int[] b = { 1, 2, 3 };
        Console.WriteLine($"  Running: {a == b}, {"hi" == "h" + "i"}");
        Console.WriteLine();
        Console.WriteLine("  Arrays are reference types with no overloaded ==, so a == b");
        Console.WriteLine("  compares identity: two different arrays, false, even with");
        Console.WriteLine("  identical contents -- the same trap Java's array == is. But");
        Console.WriteLine("  string is a reference type that DOES overload == to compare");
        Console.WriteLine("  content, which is the opposite of Java's string ==, where");
        Console.WriteLine("  you need .equals() for the same job.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Exercise(6);

        Ui.Question(
            "int[] fixedArr = new int[3]; -- can you make it hold a 4th\n" +
            "  element later?  (answer: yes or no)",
            "no",
            "Array size is fixed at creation. Use List<T> for a collection that grows.");
        Ui.Question(
            "Two int[] arrays hold the same values. Does == compare them\n" +
            "  as equal?  (answer: yes or no)",
            "no",
            "Arrays don't overload == -- it compares reference identity, same as Java's array ==.");

        Ui.Challenge(
            task: new[]
            {
                "Write a program that builds a List<int> from 1 to 5, then prints",
                "the sum of its elements as exactly:",
                "Sum: 15",
            },
            input: Array.Empty<string>(),
            expected: new[] { "Sum: 15" },
            solution: new[]
            {
                "var nums = new List<int> { 1, 2, 3, 4, 5 };",
                "int sum = 0;",
                "foreach (int n in nums) sum += n;",
                "Console.WriteLine($\"Sum: {sum}\");",
            });

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("SUMMARY");
        Console.WriteLine("  - Arrays are fixed-size; List<T> grows. Reach for List<T> by");
        Console.WriteLine("    default.");
        Console.WriteLine("  - .Length is always a property here, on arrays and string");
        Console.WriteLine("    alike -- no parentheses, unlike Java's String.length().");
        Console.WriteLine("  - Arrays compare by reference (== is false for equal");
        Console.WriteLine("    contents); string overloads == to compare content instead");
        Console.WriteLine("    -- opposite defaults from the same == operator.");
        Ui.WaitEnter();
    }

    public static void Lesson07Methods()
    {
        Ui.Title("MODULE 7 - METHODS AND DEFAULT PARAMETERS");

        Ui.Heading("PART 1: C# has default parameter values -- Java does not");

        Console.WriteLine("    void Greet(string name, string greeting = \"Hello\")");
        Console.WriteLine("    {");
        Console.WriteLine("        Console.WriteLine($\"{greeting}, {name}!\");");
        Console.WriteLine("    }");
        Console.WriteLine("    Greet(\"Ada\");");
        Console.WriteLine("    Greet(\"Ada\", \"Hi\");");
        Console.WriteLine();
        Console.WriteLine("  Running:");
        Greet("Ada");
        Greet("Ada", "Hi");
        Console.WriteLine();
        Console.WriteLine("  A parameter with = value becomes optional, filled in at the");
        Console.WriteLine("  call site when omitted. This is the direct opposite of the");
        Console.WriteLine("  Java course's own module 7: Java has no default arguments at");
        Console.WriteLine("  all and uses overloading to fill that gap instead. C# has");
        Console.WriteLine("  both tools -- default parameters for \"usually this value,");
        Console.WriteLine("  sometimes different,\" overloading for genuinely different");
        Console.WriteLine("  parameter shapes.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 2: overloading still exists too");

        Console.WriteLine("    int Add(int a, int b) => a + b;");
        Console.WriteLine("    double Add(double a, double b) => a + b;");
        Console.WriteLine();
        Console.WriteLine($"  Running Add(2, 3), Add(2.5, 3.5): {AddOverload(2, 3)}, {AddOverload(2.5, 3.5)}");
        Console.WriteLine();
        Console.WriteLine("  Resolved at compile time by parameter types, same as C++ and");
        Console.WriteLine("  Java's overloading. A method can have default parameters,");
        Console.WriteLine("  overloads, or both -- they solve different problems and");
        Console.WriteLine("  aren't a replacement for each other.");
        Console.WriteLine();
        Console.WriteLine("  (These two overloads are declared as private static methods on");
        Console.WriteLine("  this class rather than local functions -- verified while");
        Console.WriteLine("  writing this module: local functions cannot be overloaded by");
        Console.WriteLine("  signature the way class members can, CS0128 either way.)");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 3: named arguments skip the position rule");

        Console.WriteLine("    void Describe(string name, int age, string city = \"?\")");
        Console.WriteLine("    Describe(\"Ada\", city: \"London\", age: 30);");
        Console.WriteLine();
        Describe("Ada", city: "London", age: 30);
        Console.WriteLine();
        Console.WriteLine("  name:value at a call site names which parameter it fills,");
        Console.WriteLine("  so arguments can arrive out of declared order -- useful");
        Console.WriteLine("  specifically for skipping earlier optional parameters to");
        Console.WriteLine("  reach a later one, the way city was reached here without");
        Console.WriteLine("  needing an age value in between.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Exercise(7);

        Ui.Question(
            "Does Java support default parameter values the way C# does?\n" +
            "  (answer: yes or no)",
            "no",
            "Java uses overloading to fill that gap; C# has both default parameters and overloading.");
        Ui.Question(
            "void F(int a, int b = 10) -- what does F(5) pass for b?\n" +
            "  (answer: a number)",
            "10",
            "An omitted parameter with a default value is filled in with that default at the call site.");

        Ui.Challenge(
            task: new[]
            {
                "Write a method Repeat(string text, int times = 2) that returns",
                "text repeated `times` times with no separator, and a program",
                "that calls Repeat(\"ab\") and prints the result exactly:",
                "abab",
            },
            input: Array.Empty<string>(),
            expected: new[] { "abab" },
            solution: new[]
            {
                "string Repeat(string text, int times = 2) => string.Concat(Enumerable.Repeat(text, times));",
                "Console.WriteLine(Repeat(\"ab\"));",
            });

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("SUMMARY");
        Console.WriteLine("  - C# has default parameter values; Java's module 7 uses");
        Console.WriteLine("    overloading for the same job because it has to.");
        Console.WriteLine("  - Overloading is resolved by parameter types at compile time.");
        Console.WriteLine("  - name: at a call site lets arguments arrive out of order.");
        Ui.WaitEnter();

        static void Greet(string name, string greeting = "Hello") =>
            Console.WriteLine($"{greeting}, {name}!");

        static void Describe(string name, int age, string city = "?") =>
            Console.WriteLine($"{name}, {age}, {city}");
    }

    static int AddOverload(int a, int b) => a + b;
    static double AddOverload(double a, double b) => a + b;

    public static void Lesson08ClassesStructs()
    {
        Ui.Title("MODULE 8 - CLASSES, STRUCTS, AND RECORDS");

        Ui.Heading("PART 1: the trap module 2 promised -- copying a struct vs a class");

        Console.WriteLine("    struct PointStruct { public int X; }");
        Console.WriteLine("    class PointClass { public int X; }");
        Console.WriteLine();
        Console.WriteLine("    var s1 = new PointStruct { X = 1 };");
        Console.WriteLine("    var s2 = s1;         // COPIES the value");
        Console.WriteLine("    s2.X = 99;");
        Console.WriteLine();
        Console.WriteLine("    var c1 = new PointClass { X = 1 };");
        Console.WriteLine("    var c2 = c1;         // COPIES the reference");
        Console.WriteLine("    c2.X = 99;");
        Console.WriteLine();
        var s1 = new PointStruct { X = 1 };
        var s2 = s1;
        s2.X = 99;
        var c1 = new PointClass { X = 1 };
        var c2 = c1;
        c2.X = 99;
        Console.WriteLine($"  Running: struct s1.X={s1.X}, s2.X={s2.X}");
        Console.WriteLine($"           class  c1.X={c1.X}, c2.X={c2.X}");
        Console.WriteLine();
        Console.WriteLine("  s2 = s1 copies the actual value -- s1 is untouched. c2 = c1");
        Console.WriteLine("  copies the reference -- c1 and c2 now point at the same");
        Console.WriteLine("  object, so mutating through either one is visible through");
        Console.WriteLine("  both. This is module 2's value-type/reference-type split,");
        Console.WriteLine("  made concrete: it is the single biggest reason to know which");
        Console.WriteLine("  kind a type is before assigning it anywhere.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 2: record -- a class with value equality built in");

        Console.WriteLine("    record PointRecord(int X, int Y);");
        Console.WriteLine("    class  PointClass2 { public int X, Y; ... }");
        Console.WriteLine();
        Console.WriteLine("    var r1 = new PointRecord(1, 2);");
        Console.WriteLine("    var r2 = new PointRecord(1, 2);");
        Console.WriteLine("    Console.WriteLine(r1 == r2);   // true");
        Console.WriteLine();
        var r1 = new PointRecord(1, 2);
        var r2 = new PointRecord(1, 2);
        var c3 = new PointClass2(1, 2);
        var c4 = new PointClass2(1, 2);
        Console.WriteLine($"  Running: records == {r1 == r2}, plain classes == {c3 == c4}");
        Console.WriteLine();
        Console.WriteLine("  A record IS a reference type -- r1 and r2 are two distinct");
        Console.WriteLine("  objects on the heap, same as any class. What's different is");
        Console.WriteLine("  ==: the compiler generates it (and GetHashCode, and ToString)");
        Console.WriteLine("  to compare every property's value, not object identity. An");
        Console.WriteLine("  ordinary class needs that written by hand to get the same");
        Console.WriteLine("  behaviour; a record gets it from the declaration alone.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 3: private is enforced, same as Java, unlike Python");

        Console.WriteLine("    class Account");
        Console.WriteLine("    {");
        Console.WriteLine("        private decimal balance;");
        Console.WriteLine("        public decimal Balance => balance;");
        Console.WriteLine("        public void Deposit(decimal amount) => balance += amount;");
        Console.WriteLine("    }");
        Console.WriteLine("    var acc = new Account();");
        Console.WriteLine("    acc.balance = 1000;   // does NOT compile");
        Console.WriteLine();
        Console.WriteLine("  error CS0122: 'Account.balance' is inaccessible due to its");
        Console.WriteLine("  protection level");
        Console.WriteLine();
        Console.WriteLine("  private is a compiler-enforced access modifier here, the");
        Console.WriteLine("  same as Java's private and unlike Python's leading-underscore");
        Console.WriteLine("  convention (which that course's own module 9 shows you can");
        Console.WriteLine("  simply read past from outside). => on Balance is an");
        Console.WriteLine("  expression-bodied property -- a read-only, computed property");
        Console.WriteLine("  with no separate get { } block needed for a one-line body.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Exercise(8);

        Ui.Question(
            "var s2 = s1; where s1 is a struct -- does mutating s2\n" +
            "  affect s1?  (answer: yes or no)",
            "no",
            "Structs are value types -- assignment copies the value, so s1 and s2 are independent.");
        Ui.Question(
            "Two records with identical property values -- does == treat\n" +
            "  them as equal?  (answer: yes or no)",
            "yes",
            "record generates value-based equality automatically; two ordinary classes with the same values would not be == unless you wrote that comparison yourself.");

        Ui.Challenge(
            task: new[]
            {
                "Declare a record Point(int X, int Y), create two Points with the",
                "same X and Y, and print whether they are equal using == as",
                "exactly:",
                "Equal: True",
            },
            input: Array.Empty<string>(),
            expected: new[] { "Equal: True" },
            solution: new[]
            {
                "record Point(int X, int Y);",
                "var p1 = new Point(3, 4);",
                "var p2 = new Point(3, 4);",
                "Console.WriteLine($\"Equal: {p1 == p2}\");",
            });

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("SUMMARY");
        Console.WriteLine("  - Assigning a struct copies the value; assigning a class");
        Console.WriteLine("    copies the reference. This is module 2's split made real.");
        Console.WriteLine("  - record is a reference type with compiler-generated value");
        Console.WriteLine("    equality; a plain class needs that written by hand.");
        Console.WriteLine("  - private is compiler-enforced, same as Java, unlike");
        Console.WriteLine("    Python's naming convention.");
        Ui.WaitEnter();
    }

    struct PointStruct { public int X; }
    class PointClass { public int X; }
    record PointRecord(int X, int Y);
    class PointClass2
    {
        public int X, Y;
        public PointClass2(int x, int y) { X = x; Y = y; }
    }

    public static void Lesson09Nullable()
    {
        Ui.Title("MODULE 9 - NULLABLE REFERENCE TYPES");

        Ui.Heading("PART 1: the warning from module 3, explained properly");

        Console.WriteLine("    string name = null;   // string, not string?");
        Console.WriteLine();
        Console.WriteLine("  warning CS8600: Converting null literal or possible null");
        Console.WriteLine("                  value to non-nullable type.");
        Console.WriteLine();
        Console.WriteLine("  With <Nullable>enable</Nullable> in the project file (on by");
        Console.WriteLine("  default for new projects, including this course's), every");
        Console.WriteLine("  reference type you write is read as non-nullable UNLESS you");
        Console.WriteLine("  write the ?: string means \"never null, or the compiler warns");
        Console.WriteLine("  you\"; string? means \"can genuinely be null, check before use.\"");
        Console.WriteLine("  This is a compile-time convention, not a runtime guarantee --");
        Console.WriteLine("  the warnings can be silenced, and null can still reach a");
        Console.WriteLine("  string at runtime through code that ignores them.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 2: the cascade -- one bug, flagged twice");

        Console.WriteLine("    string line = Console.ReadLine();   // CS8600 here");
        Console.WriteLine("    Console.WriteLine(line.Length);     // CS8602 here");
        Console.WriteLine();
        Console.WriteLine("  CS8600 fires where a possibly-null value meets a non-nullable");
        Console.WriteLine("  variable. CS8602 fires separately, later, wherever that");
        Console.WriteLine("  variable is actually dereferenced without a null check first.");
        Console.WriteLine("  Same root cause, two warnings, because the compiler flags");
        Console.WriteLine("  both the mistake AND the place it would actually crash --");
        Console.WriteLine("  a NullReferenceException, at .Length, if this ran on null.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 3: ?., ??, and the operator that overrides the compiler");

        Console.WriteLine("    string? maybe = null;");
        Console.WriteLine("    Console.WriteLine(maybe?.Length ?? -1);");
        Console.WriteLine("    maybe = \"hi\";");
        Console.WriteLine("    Console.WriteLine(maybe?.Length ?? -1);");
        Console.WriteLine();
        string? maybe = null;
        Console.WriteLine($"  Running: {maybe?.Length ?? -1}");
        maybe = "hi";
        Console.WriteLine($"           {maybe?.Length ?? -1}");
        Console.WriteLine();
        Console.WriteLine("  ?. (null-conditional) short-circuits to null instead of");
        Console.WriteLine("  throwing when the left side is null; ?? (null-coalescing)");
        Console.WriteLine("  supplies a real value for that null case. Together they read");
        Console.WriteLine("  as \"length if there is one, else -1,\" no if statement needed.");
        Console.WriteLine();
        Console.WriteLine("  One more operator exists -- the null-forgiving !, as in");
        Console.WriteLine("  value!.Length -- which does NOT check anything at runtime. It");
        Console.WriteLine("  only silences the compiler warning, telling it \"trust me, this");
        Console.WriteLine("  isn't null here.\" Wrong, and it still throws");
        Console.WriteLine("  NullReferenceException at runtime -- ! is a promise to the");
        Console.WriteLine("  compiler, not a safety check.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Exercise(9);

        Ui.Question(
            "Does string? being null actually PREVENT a\n" +
            "  NullReferenceException at runtime, or just warn about it at\n" +
            "  compile time?  (answer: prevent or warn)",
            "warn",
            "Nullable reference types are a compile-time convention (warnings) -- they don't change what happens at runtime if you ignore them.");
        Ui.Question(
            "value!.Length -- does the ! operator check whether value is\n" +
            "  actually null?  (answer: yes or no)",
            "no",
            "! (null-forgiving) only silences the compiler warning -- it performs no runtime check, and still throws on an actually-null value.");

        Ui.Challenge(
            task: new[]
            {
                "Write a program that reads a line and prints its length, or -1",
                "if end-of-input was reached instead, using ?. and ??, as exactly:",
                "Length: <n or -1>",
            },
            input: new[] { "hello" },
            expected: new[] { "Length: 5" },
            solution: new[]
            {
                "string? line = Console.ReadLine();",
                "Console.WriteLine($\"Length: {line?.Length ?? -1}\");",
            });

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("SUMMARY");
        Console.WriteLine("  - string means non-nullable by convention; string? means it");
        Console.WriteLine("    can genuinely be null -- enforced at compile time only.");
        Console.WriteLine("  - ?. short-circuits to null instead of throwing; ?? supplies");
        Console.WriteLine("    a fallback for that null case.");
        Console.WriteLine("  - ! silences the warning without checking anything -- it can");
        Console.WriteLine("    still throw at runtime if you were wrong.");
        Ui.WaitEnter();
    }

    public static void Lesson10Exceptions()
    {
        Ui.Title("MODULE 10 - EXCEPTIONS AND IDISPOSABLE");

        Ui.Heading("PART 1: no checked exceptions -- none, unlike Java's module 10");

        Console.WriteLine("    void ReadConfig()");
        Console.WriteLine("    {");
        Console.WriteLine("        File.ReadAllText(\"config.json\");  // can throw");
        Console.WriteLine("    }                                      // no catch, no");
        Console.WriteLine("                                           // declaration --");
        Console.WriteLine("                                           // compiles fine");
        Console.WriteLine();
        Console.WriteLine("  This compiles with zero warnings and zero errors. Java's own");
        Console.WriteLine("  module 10 shows the opposite: an uncaught checked exception");
        Console.WriteLine("  there is a compile error unless the method declares `throws`.");
        Console.WriteLine("  C# has exactly one category of exception -- all of them behave");
        Console.WriteLine("  like Java's unchecked RuntimeException. Nothing forces you to");
        Console.WriteLine("  catch or declare anything, which is closer to how Python,");
        Console.WriteLine("  JavaScript, and C++ all already handle exceptions too.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 2: try/catch/finally works the way you'd expect");

        Console.WriteLine("    try");
        Console.WriteLine("    {");
        Console.WriteLine("        int n = int.Parse(\"abc\");");
        Console.WriteLine("    }");
        Console.WriteLine("    catch (FormatException e)");
        Console.WriteLine("    {");
        Console.WriteLine("        Console.WriteLine($\"Bad input: {e.Message}\");");
        Console.WriteLine("    }");
        Console.WriteLine();
        try
        {
            int n = int.Parse("abc");
        }
        catch (FormatException e)
        {
            Console.WriteLine($"  Running: Bad input: {e.Message}");
        }
        Console.WriteLine();
        Console.WriteLine("  Unlike TryParse from module 3 (a bool, no exception, for");
        Console.WriteLine("  input that's simply not a number), Parse throws on the exact");
        Console.WriteLine("  same bad input. Two tools for the same underlying job: Parse");
        Console.WriteLine("  when bad input really is exceptional and should stop the");
        Console.WriteLine("  method; TryParse when it's routine and expected.");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("PART 3: using -- deterministic cleanup, no garbage-collector timing involved");

        Console.WriteLine("    using (var r = new Resource(\"A\"))");
        Console.WriteLine("    {");
        Console.WriteLine("        Console.WriteLine(\"inside block\");");
        Console.WriteLine("    }");
        Console.WriteLine("    Console.WriteLine(\"after block\");");
        Console.WriteLine();
        Console.WriteLine("  Running:");
        using (var r = new Resource("A"))
        {
            Console.WriteLine("    inside block");
        }
        Console.WriteLine("    after block");
        Console.WriteLine();
        Console.WriteLine("  \"close A\" printed BEFORE \"after block\" -- Dispose() runs the");
        Console.WriteLine("  instant control leaves the using block, exception or not, not");
        Console.WriteLine("  whenever the garbage collector eventually gets to it. Same");
        Console.WriteLine("  deterministic guarantee as C++'s RAII destructors and Python's");
        Console.WriteLine("  with statement; the opposite of Python's own module 9, where");
        Console.WriteLine("  CPython's collection timing is explicitly NOT a language");
        Console.WriteLine("  guarantee. A type earns the right to sit in a using block by");
        Console.WriteLine("  implementing IDisposable -- one method, Dispose().");

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Exercise(10);

        Ui.Question(
            "Does an uncaught exception in C# ever cause a compile error\n" +
            "  the way Java's checked exceptions can?  (answer: yes or no)",
            "no",
            "C# has one exception category, all behaving like Java's unchecked RuntimeException -- nothing is checked at compile time.");
        Ui.Question(
            "In a using block, does Dispose() run immediately when the\n" +
            "  block ends, or whenever the garbage collector gets to it?\n" +
            "  (answer: immediately or gc)",
            "immediately",
            "using guarantees Dispose() runs the instant the block exits, exception or not -- deterministic, not GC-timed.");

        Ui.Challenge(
            task: new[]
            {
                "Write a program that tries to parse \"xyz\" as an int with",
                "int.Parse inside a try/catch, and on failure prints exactly:",
                "Could not parse input",
            },
            input: Array.Empty<string>(),
            expected: new[] { "Could not parse input" },
            solution: new[]
            {
                "try { int.Parse(\"xyz\"); }",
                "catch (FormatException) { Console.WriteLine(\"Could not parse input\"); }",
            });

        Ui.WaitEnter();
        Ui.ClearScreen();
        Ui.Heading("SUMMARY");
        Console.WriteLine("  - C# has no checked exceptions -- every exception behaves");
        Console.WriteLine("    like Java's unchecked RuntimeException.");
        Console.WriteLine("  - Parse throws on bad input; TryParse reports it as a bool.");
        Console.WriteLine("    Pick based on whether that input is exceptional or routine.");
        Console.WriteLine("  - using guarantees Dispose() runs deterministically when the");
        Console.WriteLine("    block exits -- not whenever the GC happens to run.");
        Ui.WaitEnter();

        Console.WriteLine();
        Console.WriteLine("  You've reached the end of the current C# course. There's more");
        Console.WriteLine("  of the language past here -- LINQ, async/await, generics in");
        Console.WriteLine("  depth, interfaces beyond IDisposable -- tracked as this");
        Console.WriteLine("  course's own target arc, the same way every other language");
        Console.WriteLine("  here states where it's headed next.");
        Ui.WaitEnter();
    }

    class Resource : IDisposable
    {
        readonly string name;
        public Resource(string n) { name = n; Console.WriteLine($"    open {name}"); }
        public void Dispose() => Console.WriteLine($"    close {name}");
    }
}
