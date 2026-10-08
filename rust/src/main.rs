//! Rust course - main menu.
//!
//! Written in Rust on purpose, the way the other courses are written in the
//! language they teach: the module table below is an array of a struct that
//! holds a function pointer, which modules 4 and 7 explain.
//!
//! Build:  make
//! Run:    ./rust-course

mod lessons_advanced;
mod lessons_basics;
mod lessons_memory;
mod ui;

/// One menu entry: what is shown, the function that runs, and which of the
/// three menu bands ("BASIC", "INTERMEDIATE", "ADVANCED") it is filed under.
/// The tier is a display grouping only; the module functions never see it.
struct Module {
    title: &'static str,
    run: fn(),
    tier: &'static str,
}

const MODULES: [Module; 8] = [
    Module { title: "Compiling and println!", run: lessons_basics::lesson_01_println, tier: "BASIC" },
    Module { title: "Variables and types", run: lessons_basics::lesson_02_variables, tier: "BASIC" },
    Module { title: "Control flow", run: lessons_basics::lesson_03_control_flow, tier: "BASIC" },
    Module { title: "Functions", run: lessons_basics::lesson_04_functions, tier: "BASIC" },
    Module { title: "Ownership", run: lessons_memory::lesson_05_ownership, tier: "INTERMEDIATE" },
    Module { title: "References and borrowing", run: lessons_memory::lesson_06_borrowing, tier: "INTERMEDIATE" },
    Module { title: "Memory: stack, heap, raw pointers", run: lessons_memory::lesson_07_memory, tier: "INTERMEDIATE" },
    Module { title: "Inline assembly and NASM", run: lessons_advanced::lesson_08_assembly, tier: "ADVANCED" },
];

fn show_menu() {
    ui::clear_screen();
    ui::title("RUST COURSE - FROM println! TO MEMORY");

    let mut shown_tier = "";
    for (index, module) in MODULES.iter().enumerate() {
        if module.tier != shown_tier {
            shown_tier = module.tier;
            println!("\n  -- {shown_tier} --");
        }
        println!("   [{:>2}]  {}", index + 1, module.title);
    }

    println!("\n   [ 0]  Quit");
    ui::rule();
}

fn main() {
    loop {
        show_menu();
        print!("\n  Pick a module: ");

        // End of input (Ctrl-D, or a script feeding the program) counts as
        // quitting; without it this loop would spin forever on nothing.
        let Some(choice) = ui::read_line() else {
            break;
        };

        if choice.trim() == "0" {
            break;
        }

        match choice.trim().parse::<usize>() {
            Ok(number) if (1..=MODULES.len()).contains(&number) => {
                ui::clear_screen();
                (MODULES[number - 1].run)();
            }
            _ => {
                println!("\n  Not a valid option.");
                ui::wait_enter();
            }
        }
    }

    println!("\n  See you next time.\n");
}
