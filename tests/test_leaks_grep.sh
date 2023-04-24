#!/bin/bash

test_number=1

flags=(
    "-e"
    "-i"
    "-v"
    "-c"
    "-l"
    "-n"
    "-h"
    "-s"
    "-o"
)

test_files=(
    "tests/testfiles/testfiles_1.txt"
    "tests/testfiles/testfiles_2.txt"
    "tests/testfiles/testfiles_3.txt"
    "tests/testfiles/testfiles_4.txt"
    "tests/testfiles/testfiles_5.txt"
    "tests/testfiles/testfiles_1.txt tests/testfiles/testfiles_2.txt"
    "tests/testfiles/testfiles_1.txt tests/testfiles/testfiles_4.txt"
    "tests/testfiles/testfiles_2.txt tests/testfiles/testfiles_3.txt"
    "tests/testfiles/testfiles_4.txt tests/testfiles/testfiles_5.txt"
    "tests/testfiles/testfiles_2.txt tests/testfiles/testfiles_3.txt tests/testfiles/testfiles_5.txt"
    "tests/testfiles/testfiles_1.txt tests/testfiles/testfiles_4.txt tests/testfiles/testfiles_5.txt"
)

patterns=(
  "source"
  "de*f"
)

for pattern in "${patterns[@]}"
do
  for flag in "${flags[@]}"
  do
    for test_file in "${test_files[@]}"
    do
      echo "#$test_number: $flag $pattern $test_file"
      valgrind --show-leak-kinds=all ./build/s21_grep $flag $pattern $test_file > tmp_leaks.txt 2>&1
      tail -n4 ./tmp_leaks.txt
      echo " "
      rm tmp_leaks.txt
      test_number=$(( $test_number + 1 ))
    done
  done
done
