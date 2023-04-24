TARGET1=build/s21_grep

CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11

SOURCES1 = src/s21_grep.c

.PHONY: tests

all: s21_grep

s21_grep: $(SOURCES1)
	$(CC) $(CFLAGS) $(SOURCES1) -o $(TARGET1)

clean:
	rm -rf *.o *.out build/s21_grep tmp*

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem $(SOURCES1)

formatcheck:
	clang-format -style=google -n src/*.c
	clang-format -style=google -n src/*.h

formatcorrect:
	clang-format -style=google -i src/*.c
	clang-format -style=google -i src/*.h

tests:
	chmod +x tests/tests_grep.sh
	./tests/tests_grep.sh

leakscheck:
	chmod +x tests/test_leaks_grep.sh
	./tests/test_leaks_grep.sh

rebuild: clean all
