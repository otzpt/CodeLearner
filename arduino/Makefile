# Arduino course
#
#   make                 builds ./arduino-course
#   make run             builds and runs
#   make try FILE=x.ino  builds and runs one of your own sketches
#                        (setup() once, then loop() LOOPS times, default 10)
#   make clean           removes build output
#
# A sketch folder is built the way the Arduino IDE builds it: every .ino
# tab is joined into one C++ file, then compiled with the Arduino core. Here
# the "core" is shim/Arduino.h, which runs the sketch on this PC. The IDE
# writes the function prototypes for you; g++ does not, so the tabs are
# joined in an order where every function is defined before it is used
# (ui, then the lessons, then the main menu).
#
# -Wall -Wextra stay on always: a warning here is teaching material more
# often than not.

CXX      = g++
CXXFLAGS = -std=gnu++17 -Wall -Wextra -g -Ishim
TARGET   = arduino-course
LOOPS   ?= 10
TABS     = course/ui.ino course/lessons_basics.ino course/lessons_more.ino \
           course/lessons_advanced.ino course/course.ino

$(TARGET): build/sketch.cpp shim/main.cpp shim/Arduino.h
	$(CXX) $(CXXFLAGS) -o $@ build/sketch.cpp shim/main.cpp

build/sketch.cpp: $(TABS)
	mkdir -p build
	( echo '#include <Arduino.h>'; \
	  for tab in $(TABS); do echo "#line 1 \"$$tab\""; cat $$tab; echo; done ) > $@

run: $(TARGET)
	./$(TARGET)

# Your own sketch: one .ino file, built against the same shim. A real board
# would call loop() until the power goes; here it stops after LOOPS calls.
try:
	@test -n "$(FILE)" || { echo "usage: make try FILE=your_sketch.ino"; exit 1; }
	mkdir -p build
	( echo '#include <Arduino.h>'; echo '#line 1 "$(FILE)"'; cat $(FILE) ) > build/try.cpp
	$(CXX) $(CXXFLAGS) -o build/try build/try.cpp shim/main.cpp
	SIM_LOOPS=$(LOOPS) ./build/try

clean:
	rm -rf build $(TARGET)

.PHONY: run try clean
