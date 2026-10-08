# C++ course
#
#   make          builds ./cpp-course
#   make run      builds and runs
#   make asm-demo builds and runs the NASM example in asm/ (needs nasm)
#   make clean    removes build output
#
# -Wall -Wextra stay on always, same reason as the C course: a warning here
# is teaching material more often than not.

CXX      = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g
TARGET   = cpp-course
SOURCES  = src/main.cpp src/ui.cpp src/lessons_basics.cpp src/lessons_modern.cpp src/lessons_assembly.cpp
OBJECTS  = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

%.o: %.cpp src/ui.h src/lessons.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

# Module 13's NASM example. Separate from the course on purpose: the course
# builds with a C++ compiler alone, and only this target needs nasm. The
# course runs asm/use_asm itself, when it exists, to show its real output.
asm-demo:
	nasm -f elf64 asm/add.asm -o asm/add.o
	$(CXX) -std=c++20 -Wall -Wextra asm/use_asm.cpp asm/add.o -o asm/use_asm
	./asm/use_asm

clean:
	rm -f $(OBJECTS) $(TARGET) asm/add.o asm/use_asm

.PHONY: run asm-demo clean
