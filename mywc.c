#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>
#include <wctype.h>

typedef struct COUNT {
  long lines;
  long words;
  long bytes;
  long chars;
} cnt;

typedef struct OPTIONS {
  bool lflag;
  bool wflag;
  bool cflag;
  bool mflag;
} opts;

// returns true if all options are false
bool noOptions(opts* ops) {
  return (!ops->lflag && !ops->wflag && !ops->cflag && !ops->mflag);
}

// c1 = c1 + c2
void addCount(cnt* c1, cnt* c2) {
  c1->lines += c2->lines;
  c1->words += c2->words;
  c1->bytes += c2->bytes;
  c1->chars += c2->chars;
}

// getCount
cnt getCount(FILE* file_d) {
  // Initialize with 0s
  cnt fcnt = {.lines = 0, .words = 0, .bytes = 0, .chars = 0};

  // Prev Char and Curr Char
  wint_t prev_ch = ' ', curr_ch;

  char s[4];

  while ((curr_ch = fgetwc(file_d)) != EOF) {
    fcnt.chars += 1;

    // The wctomb() function converts a wide character, 'curr_ch', into a
    // multibyte character and stores the result in 's'
    fcnt.bytes += wctomb(s, curr_ch);

    // Count '\n' (endline characters) in file for lines
    if (curr_ch == '\n') {
      fcnt.lines += 1;
    }

    // if prev char is space and curr char is not space
    // Then it is start of new word
    if (iswspace(prev_ch) && !iswspace(curr_ch)) {
      fcnt.words += 1;
    }

    // set prev char to curr char
    prev_ch = curr_ch;
  }

  // We have reached the end of file

  // I tried this way to count bytes but it fails if the file_d is 'stdin'
  // fseek(file_d, 0, SEEK_END);
  // fcnt.bytes = ftell(file_d);

  return fcnt;
}

// pretty printCount
void printCount(FILE* file_d, cnt* fcnt, opts* ops) {
  if (ops->lflag) fprintf(file_d, "%8ld", fcnt->lines);
  if (ops->wflag) fprintf(file_d, "%8ld", fcnt->words);
  if (ops->cflag) fprintf(file_d, "%8ld", fcnt->bytes);
  if (ops->mflag) fprintf(file_d, "%8ld", fcnt->chars);
}

int main(int argc, char** argv) {
  // handle counting wide characters
  setlocale(LC_ALL, "");

  // Parse Options

  // Initially set to false
  opts ops = {.lflag = 0, .wflag = 0, .cflag = 0, .mflag = 0};

  int opt;

  // -l The number of lines in each input file
  // -w The number of words in each input file
  // -c The number of bytes in each input file.
  // -m The number of characters in each input file

  /*
    https://opensource.com/article/21/8/short-option-parsing-c

    At each getopt call, the function returns the next short option it finds on
    the command line or the value '?' for any unrecognized short options. When
    getopt can't find any more short options, it returns -1 and sets the global
    variable optind to the next element in **argv after all the short options.
  */

  while ((opt = getopt(argc, argv, "lcmw")) != -1) {
    switch (opt) {
      case 'l':
        ops.lflag = 1;
        break;
      case 'w':
        ops.wflag = 1;
        break;
      case 'c':
        ops.cflag = 1;
        ops.mflag = 0;
        break;
      case 'm':
        ops.mflag = 1;
        ops.cflag = 0;
        break;
      case '?':
        // Invalid Option
        fprintf(stdout, "usage: mywc [-lcmw] [file ...]\n");
        exit(EXIT_FAILURE);
    }
  }

  // If no flags provided
  if (noOptions(&ops)) {
    // default options -l -w -c
    ops.lflag = 1;
    ops.wflag = 1;
    ops.cflag = 1;
  }

  // Count of files provided
  int files = 0;

  // File Descriptor
  FILE* file_d;

  // File Count
  cnt fcnt;

  // Total Count
  cnt tcnt = {.lines = 0, .words = 0, .bytes = 0, .chars = 0};

  for (int idx = optind; idx < argc; ++idx) {
    // increment files
    files += 1;

    // open file in read mode
    file_d = fopen(argv[idx], "r");
    if (file_d == NULL) {
      fprintf(stderr, "[ERROR] Cannot open file: %s\n", argv[idx]);
      // exit(EXIT_FAILURE);
    } else {
      fcnt = getCount(file_d);

      fclose(file_d);

      printCount(stdout, &fcnt, &ops);
      fprintf(stdout, " %s\n", argv[idx]);

      addCount(&tcnt, &fcnt);
    }
  }

  // Case 1: files == 0
  if (files == 0) {
    file_d = stdin;
    fcnt = getCount(file_d);

    fclose(file_d);
    printCount(stdout, &fcnt, &ops);
    fprintf(stdout, "\n");
  }

  // Case 2: files > 1
  // Print the total
  if (files > 1) {
    printCount(stdout, &tcnt, &ops);
    fprintf(stdout, " %s\n", "total");
  }

  exit(EXIT_SUCCESS);
}
