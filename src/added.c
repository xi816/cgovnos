/*
 * main.c - CGovnOS virtual machine
 *
 * Copyright 2024 Xi816 <xi816/cgovnos>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
*/

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
      argp++;
    }
    else if (!strcmp(argv[argp], "-ROM")) {
      romsize = atoi(argv[argp+1]);
      argp++;
    }
    else if (!strcmp(argv[argp], "-H")) {
      Usage();
      return 100;
    }
    else {
      if (argp == argc-1) {
        fprintf(stderr, "%sNo memory file provided\n", ERROR);
        return 1;
      }
      memcpy(romfn, argv[argp], 64);
      memcpy(memfn, argv[argp+1], 64);
      break;
    }
    argp++;
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

  new_st;

  initCPU:
  ResetSP(&GC);
  ResetPC(&GC);

  runProgram:
  U8 exitcode = Execute(GC);

  old_st;
  return exitcode;
}
