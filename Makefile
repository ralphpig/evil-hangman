CC = gcc
FLAGS = -Wall -std=c99 -g
DRIVER = build/main.o build/string.o build/vector.o build/tree.o
TEST = test_def.o string.o unit_test.o

hangman: $(DRIVER)
	$(CC) $(FLAGS) -o hangman $(DRIVER)
clean:
	rm -f hangman
	rm -rf *.dSYM
	rm -rf build/*

build/main.o: main.c
	$(CC) $(FLAGS) -c main.c -o build/main.o
build/tree.o: lib/tree/tree.c lib/tree/tree.h lib/string/string.h lib/vector/vector.h
	$(CC) $(FLAGS) -c lib/tree/tree.c -o build/tree.o
build/string.o: lib/string/string.c lib/string/string.h lib/generic.h
	$(CC) $(FLAGS) -c lib/string/string.c -o build/string.o
build/vector.o: lib/vector/vector.c lib/vector/vector.h lib/generic.h
	$(CC) $(FLAGS) -c lib/vector/vector.c -o build/vector.o


build/test_def.o: tests/test_def.c
	$(CC) $(FLAGS) -c tests/test_def.c
build/unit_test.o: tests/unit_test.c tests/unit_test.h
	$(CC) $(FLAGS) -c tests/unit_test.c