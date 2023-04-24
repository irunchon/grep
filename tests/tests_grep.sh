#!/bin/bash

test_number=1
success=0
fail=0

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
  "int"
  "de*f"
)

for pattern in "${patterns[@]}"
do
  for flag in "${flags[@]}"
  do
    for test_file in "${test_files[@]}"
    do
      ./build/s21_grep $flag $pattern $test_file > tmp_1.txt
      grep $flag $pattern $test_file > tmp_2.txt
      cmp_result="$(diff -s tmp_1.txt tmp_2.txt)"
      if [ "$cmp_result" == "Files tmp_1.txt and tmp_2.txt are identical" ]
      then
        result="Success"
        success=$(( $success + 1 ))
      else
        result="Fail"
        fail=$(( $fail + 1 ))
      fi
      rm tmp_*.txt
      echo "#$test_number: $result: $flag $pattern $test_file"
      test_number=$(( $test_number + 1 ))
    done
  done
done

for pattern in "${patterns[@]}"
do
  for flag1 in "${flags[@]}"
  do
    for flag2 in "${flags[@]}"
    do
      if [ $flag1 != $flag2 ]
      then
        for test_file in "${test_files[@]}"
        do
          ./build/s21_grep $flag $pattern $test_file > tmp_1.txt
          grep $flag $pattern $test_file > tmp_2.txt
          cmp_result="$(diff -s tmp_1.txt tmp_2.txt)"
          if [ "$cmp_result" == "Files tmp_1.txt and tmp_2.txt are identical" ]
          then
            result="Success"
            success=$(( $success + 1 ))
          else
            result="Fail"
            fail=$(( $fail + 1 ))
          fi
          rm tmp_*.txt
          echo "#$test_number: $result: $flag1 $flag2 $pattern $test_file"
          test_number=$(( $test_number + 1 ))
        done
      fi
    done
  done
done

test_number=$(( $test_number - 1 ))
echo " "
echo "Total: $test_number"
echo "Successful: $success"
echo "Failed: $fail"