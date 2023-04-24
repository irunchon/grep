# grep
My implementation of the standard grep utility (C language)

### grep Usage

Usage of the utility:

`grep [options] template [file_name]`

### grep Options

| No. | Options | Description |
| ------ | ------ | ------ |
| 1 | -e | pattern |
| 2 | -i | Ignore uppercase vs. lowercase.  |
| 3 | -v | Invert match. |
| 4 | -c | Output count of matching lines only. |
| 5 | -l | Output matching files only.  |
| 6 | -n | Precede each matching line with a line number. |
| 7 | -h | Output matching lines without preceding them by file names. |
| 8 | -s | Suppress error messages about nonexistent or unreadable files. |
| 9 | -o | Output the matched parts of a matching line. |

### grep Tests
There are two types of tests available:
* Comparison of the results of my grep and standard grep utility. For the reference was taken work of grep utility in OS Ubuntu ver 22.04 and MAC OS 13.3.1.
* Tests for memory leaks check made via checking valgrind utility's results. 

Tests can be launched via command `make tests` and `make leakscheck` respectively.

