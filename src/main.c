#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>

#include "main.h"
#include "../lib/namings.h"
#include "../lib/flsize.h"

U16 memsize;
#include "cpu/cpu.h"
#include "cpu/instructions.h"
#include "doc/usage.h"

I32 main(I32 argc, I8** argv) {
  GC32 GC;

  U8 argp = 1;
  U16 romsize = 4096;

  U8 romfn[64];
  U8 memfn[64];

  while (argp < argc) {
    if (!strcmp(argv[argp], "-M")) {
      memsize = atoi(argv[argp+1]);
      argv++;
    }
    else if (!strcmp(argv[argp], "-ROM")) {
      romsize = atoi(argv[argp+1]);
      argv++;
    }
    else if (!strcmp(argv[argp], "-H")) {
      Usage();
      return 100;
    }
    else {
      memcpy(romfn, argv[argp], 64);
      memcpy(memfn, argv[argp+1], 64);
      break;
    }
    argv++;
  }
  FILE* romfile = fopen(romfn, "rb");
  FILE* memfile = fopen(memfn, "rb");
  if (!romfile) {
    fprintf(stderr, "%sROM file %s not found\n", ERROR, romfn);
    return 2;
  }
  if (!memfile) {
    fprintf(stderr, "%sMemory file %s not found\n", ERROR, memfn);
    return 2;
  }
  memsize = FileSize(memfile);

  GC.rom = malloc(romsize);
  GC.mem = malloc(memsize);

  // Read the ROM file
  fread(GC.rom, 1, romsize, romfile);
  fclose(romfile);

  // Read the memory
  fread(GC.mem, 1, memsize, memfile);
  fclose(memfile);

  struct termios oldt;
  struct termios newt;

  tcgetattr(STDIN_FILENO, &oldt);
  memcpy(&newt, &oldt, sizeof(oldt));
  newt.c_iflag &= ~(IXON);
  newt.c_lflag &= ~(ICANON | ECHO | ISIG | IEXTEN);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  initCPU:
  ResetSP(&GC);
  ResetPC(&GC);

  runProgram:
  U8 exitcode = Execute(GC);

  old_st(oldt);
  return exitcode;
}

