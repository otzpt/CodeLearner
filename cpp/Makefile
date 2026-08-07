# C++ course
#
#   make          builds ./cpp-course
#   make run      builds and runs
#   make clean    removes build output
#
# -Wall -Wextra stay on always, same reason as the C course: a warning here
# is teaching material more often than not.

CXX      = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g
TARGET   = cpp-course
SOURCES  = src/main.cpp src/ui.cpp src/lessons_basics.cpp src/lessons_modern.cpp
OBJECTS  = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

%.o: %.cpp src/ui.h src/lessons.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: run clean
