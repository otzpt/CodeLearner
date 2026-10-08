// use_asm.rs - calls the two NASM functions in add.asm.
//
//   nasm -f elf64 asm/add.asm -o asm/add.o
//   rustc --edition 2021 asm/use_asm.rs -C link-arg=asm/add.o -o asm/use_asm

extern "C" {
    fn add_numbers(a: u64, b: u64) -> u64;
    fn sum_bytes(data: *const u8, count: u64) -> u64;
}

fn main() {
    let bytes = [1u8, 2, 3, 4];
    // Calling a function the compiler cannot see into is unsafe: it trusts
    // that the declaration above matches what the assembly really does.
    unsafe {
        println!("add_numbers(40, 2)    = {}", add_numbers(40, 2));
        println!("sum_bytes([1,2,3,4]) = {}", sum_bytes(bytes.as_ptr(), bytes.len() as u64));
    }
}
