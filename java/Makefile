# Java course
#
#   make          compiles every .java file under src/, checking for errors
#                 (class files land next to the .java files, same as C's .o)
#   make run      runs the course -- see ./run, which the launcher also uses
#   make clean    removes the .class files
#
# -Xlint:all -Werror stay on always, same reason as every other course here:
# a compiler warning is teaching material more often than not.

JC      = javac
JFLAGS  = -Xlint:all -Werror
SOURCES = src/Ui.java src/LessonsBasics.java src/LessonsMore.java src/Main.java

all:
	$(JC) $(JFLAGS) $(SOURCES)

run:
	./run

clean:
	rm -f src/*.class

.PHONY: all run clean
