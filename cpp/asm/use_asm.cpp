// use_asm.cpp - calls the two NASM functions in add.asm.
//
//   nasm -f elf64 asm/add.asm -o asm/add.o
//   g++ -std=c++20 -Wall -Wextra asm/use_asm.cpp asm/add.o -o asm/use_asm

#include <cstdint>
#include <iostream>

// extern "C" is not optional. Without it the C++ compiler looks for a
// mangled name that encodes the parameter types, such as
//   add_numbers(unsigned long, unsigned long)
// and the linker, which finds only the plain label add_numbers in the
// assembly, reports it as an undefined reference.
extern "C" {
std::uint64_t add_numbers(std::uint64_t a, std::uint64_t b);
std::uint64_t sum_bytes(const std::uint8_t *data, std::uint64_t count);
}

int main() {
    const std::uint8_t bytes[] = {1, 2, 3, 4};

    std::cout << "add_numbers(40, 2)    = " << add_numbers(40, 2) << "\n";
    std::cout << "sum_bytes([1,2,3,4]) = " << sum_bytes(bytes, sizeof bytes) << "\n";
    return 0;
}
