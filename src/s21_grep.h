#ifndef SRC_CAT_S21_GREP_H_
#define SRC_CAT_S21_GREP_H_

#define _POSIX_C_SOURCE 200809L

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct s21GrepFlags {
  int eFlag, iFlag, vFlag, cFlag, lFlag, nFlag, hFlag, sFlag, oFlag;
};

void s21GrepUsage();
void s21GrepFlagsParsing(int argc, char *argv[],
                         struct s21GrepFlags *flagsValues);
void s21GrepFilesProcessing(int optind, int argc, char *argv[],
                            struct s21GrepFlags *flagsValues);
void s21GrepFlagsProcessingInOneFile(int argc, char *argv[],
                                     int curFileInArgList, FILE *currentFile,
                                     regex_t regex, int optind,
                                     struct s21GrepFlags *flagsValues);

#endif  // SRC_CAT_S21_GREP_H_