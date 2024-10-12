#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>

#include "../lib/namings.h"
#include "../lib/flsize.h"

#define MEMSIZE 4096
#include "cpu/cpu.h"
#include "cpu/instructions.h"

I32 main(I32 argc, U8** argv) {
  GC32 GC;

  FILE* fl = fopen(argv[1], "rb");
  if (!fl) {
    fprintf(stderr, "%sFile %s not found\n", ERROR, argv[1]);
    return 2;
  }
  U16 flen = FileSize(fl);
  GC.mem = malloc(flen);
  fseek(fl, 0, SEEK_SET);
  fread(GC.mem, 1, flen, fl);
  fclose(fl);

  initscr();
  cbreak();
  raw();
  noecho();

  initCPU:
  ResetSP(&GC);
  ResetPC(&GC);

  runProgram:
  U8 exitcode = Execute(GC);

  NCWN;
  return exitcode;
}

