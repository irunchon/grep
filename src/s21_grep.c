#include "s21_grep.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    s21GrepUsage();
    exit(EXIT_FAILURE);
  } else {
    struct s21GrepFlags flagsValues = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    s21GrepFlagsParsing(argc, argv, &flagsValues);
    s21GrepFilesProcessing(optind, argc, argv, &flagsValues);
  }
  return EXIT_SUCCESS;
}

void s21GrepUsage() {
  printf("Usage: s21_grep [OPTION]... PATTERNS [FILE]...\n");
}

void s21GrepFlagsParsing(int argc, char *argv[],
                         struct s21GrepFlags *flagsValues) {
  int currentOption = 0;

  while ((currentOption = getopt(argc, argv, "eivclnhso")) != -1) {
    switch (currentOption) {
      case 'e':
        flagsValues->eFlag = 1;
        break;
      case 'i':
        flagsValues->iFlag = 1;
        break;
      case 'v':
        flagsValues->vFlag = 1;
        break;
      case 'c':
        flagsValues->cFlag = 1;
        break;
      case 'l':
        flagsValues->lFlag = 1;
        break;
      case 'n':
        flagsValues->nFlag = 1;
        break;
      case 'h':
        flagsValues->hFlag = 1;
        break;
      case 's':
        flagsValues->sFlag = 1;
        break;
      case 'o':
        flagsValues->oFlag = 1;
        break;
      case '?':
        s21GrepUsage();
        exit(EXIT_FAILURE);
    }
  }
}

void s21GrepFilesProcessing(int optind, int argc, char *argv[],
                            struct s21GrepFlags *flagsValues) {
  for (int curFileInArgList = optind + 1; curFileInArgList < argc;
       curFileInArgList++) {
    FILE *currentFile = fopen(argv[curFileInArgList], "rb");

    if (currentFile == NULL) {
      if (!flagsValues->sFlag) {
        fprintf(stderr, "grep: %s: No such file or directory\n",
                argv[curFileInArgList]);
      }
      continue;
    }

    int regexFlags = REG_EXTENDED;
    if (flagsValues->iFlag) {
      regexFlags |= REG_ICASE;
    }
    if (flagsValues->oFlag) {
      regexFlags &= ~REG_NOSUB;
    }

    regex_t regex;
    int return_value;
    return_value = regcomp(&regex, argv[optind], regexFlags);

    if (return_value != 0) {
      char buffer[100];
      regerror(return_value, &regex, buffer, 100);
      fprintf(stderr, "s21_grep: regcomp failed with error: %s\n", buffer);
      regfree(&regex);
      fclose(currentFile);
      exit(EXIT_FAILURE);
    }
    s21GrepFlagsProcessingInOneFile(argc, argv, curFileInArgList, currentFile,
                                    regex, optind, flagsValues);
    regfree(&regex);
    fclose(currentFile);
  }
}

void s21GrepFlagsProcessingInOneFile(int argc, char *argv[],
                                     int curFileInArgList, FILE *currentFile,
                                     regex_t regex, int optind,
                                     struct s21GrepFlags *flagsValues) {
  int wasFilenameDisplayed = 0;
  int linesCounter = 0;
  int lineNumber = 0;

  char *lineToRead = NULL;
  size_t bufferSize = 0;

  while (getline(&lineToRead, &bufferSize, currentFile) != -1) {
    lineNumber++;
    size_t nmatch;

    if (flagsValues->oFlag) {
      nmatch = strlen(lineToRead) / strlen(argv[optind]);
    } else {
      nmatch = 0;
    }
    regmatch_t pmatch[nmatch];

    int return_value = regexec(&regex, lineToRead, nmatch, pmatch, 0);

    if (return_value != 0 && return_value != REG_NOMATCH) {
      char buffer[100];
      regerror(return_value, &regex, buffer, 100);
      fprintf(stderr, "s21_grep: regexec failed with error: %s\n", buffer);
      exit(EXIT_FAILURE);
    }
    int processingMode;
    if (flagsValues->vFlag) {
      if (flagsValues->oFlag) {
        exit(EXIT_FAILURE);
      }
      processingMode = REG_NOMATCH;
    } else {
      processingMode = 0;
    }
    if (return_value == processingMode) {
      if (flagsValues->lFlag) {
        if (!wasFilenameDisplayed) {
          printf("%s\n", argv[curFileInArgList]);
          wasFilenameDisplayed = 1;
        }
        continue;
      }
      if (flagsValues->cFlag) {
        linesCounter++;
        continue;
      }
      if ((argc - optind - 1) > 1 && !flagsValues->lFlag &&
          !flagsValues->hFlag && !flagsValues->oFlag) {
        printf("%s:", argv[curFileInArgList]);
      }
      if (flagsValues->nFlag && !flagsValues->oFlag) {
        printf("%d:", lineNumber);
      }
      if (flagsValues->oFlag) {
        char *subLine = lineToRead;
        while (!regexec(&regex, subLine, nmatch, pmatch, 0)) {
          if ((argc - optind - 1) > 1 && !flagsValues->hFlag) {
            printf("%s:", argv[curFileInArgList]);
          }
          if (flagsValues->nFlag) {
            printf("%d:", lineNumber);
          }
          printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
                 subLine + pmatch[0].rm_so);
          subLine += pmatch[0].rm_eo;
        }
      } else {
        printf("%s", lineToRead);
      }
      int lineLength = strlen(lineToRead);
      if (lineLength > 0 && lineToRead[lineLength - 1] != '\n' &&
          lineToRead[lineLength - 1] != '\r' && !flagsValues->oFlag) {
        printf("\n");
      }
    }
  }
  if (flagsValues->cFlag && !flagsValues->lFlag) {
    if ((argc - optind - 1) > 1 && !flagsValues->hFlag) {
      printf("%s:", argv[curFileInArgList]);
    }
    printf("%d\n", linesCounter);
  }
  if (lineToRead != NULL) {
    free(lineToRead);
  }
}