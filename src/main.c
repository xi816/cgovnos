#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>

#include "../lib/namings.h"
// #include "../lib/bool.h"
#include "../lib/gchar.h"
#include "../lib/errormsg.h"
#include "../lib/colors.h"

#include "cpu/cpu.h"
#include "cpu/instructions.h"

#define NCWN endwin()

U0 StackDump(GC32* GC, I16 size) {
  printf("POS: %04X\r\n", GC->regs.PC);
  for (I16 i = 0; i < size; i++) {
    if (0xFFFF-i == GC->regs.SP) {
      printf("Stack value (\x1B[32m%04X\x1B[0m): %X\r\n", 0xFFFF-i, GC->mem[0xFFFF-i]);
    }
    else {
      printf("Stack value (%04X): %02X\r\n", 0xFFFF-i, GC->mem[0xFFFF-i]);
    }
  }
  // for (I16 i = 0; i < size+1; i++) {
  //   printf("\x1B[A");
  // }
}

U0 RegDump(GC32* gccpu) {
  printf("Registers:\r\n");
  printf("CA A: %s%08X\033[0m %s%04X\033[0m\r\n", BgB_FgW, gccpu->regs.CA, BgB_FgW, gccpu->regs.A);
  printf("CX X: %s%08X\033[0m %s%04X\033[0m\r\n", BgB_FgW, gccpu->regs.CX, BgB_FgW, gccpu->regs.X);
  printf("CY Y: %s%08X\033[0m %s%04X\033[0m\r\n", BgB_FgW, gccpu->regs.CY, BgB_FgW, gccpu->regs.Y);
  printf("SP: %s%04X\033[0m\r\n", BgB_FgW, gccpu->regs.SP);
  printf("CS: %s%04X\033[0m\r\n", BgB_FgW, gccpu->regs.CS);
  printf("PC: %s%04X\033[0m\r\n", BgB_FgW, gccpu->regs.PC);
  printf("\r\n");
}

I32 main(I32 argc, U8** argv) {
  GC32 GC;

  FILE* fl = fopen(argv[1], "rb");
  if (!fl) {
    fprintf(stderr, "%sFile %s not found\n", ERROR, argv[1]);
    goto FAILED;
  }
  fseek(fl, 0, SEEK_END);
  U16 flen = ftell(fl);
  fseek(fl, 0, SEEK_SET);
  fread(GC.mem, 1, flen, fl);
  fclose(fl);

  initscr();
  cbreak();
  noecho();

  printf("\n");

  U16 Arg1;

  initCPU:
  ResetSP(&GC);
  ResetCAXY(&GC);

  runProgram:
  while (true) {
    switch (GC.mem[GC.regs.PC]) {
      case I_CPUID:
        GC.regs.A = 0x03; // Placeholder for CPUID instruction
        break;
      case I_CSP:
        GC.regs.Y = getch();
        RegDump(&GC);
        break;
      case I_FLF:
        GC.regs.FLAGS = 0x00;
        break;
      case I_FLRA:
        ResetAXY(&GC);
        break;
      case I_FLRB:
        ResetCAXY(&GC);
        break;
      case I_JMP:
        GC.regs.PC = FetchWordRev(&GC, GC.regs.PC+1)-1;
        break;
      case I_PUSH:
        StackPush(&GC);
        break;
      case I_POP:
        StackPop(&GC);
        break;
      case I_ADD:
        StackAdd(&GC);
        break;
      case I_SUB:
        StackSub(&GC);
        break;
      case I_MUL:
        StackMul(&GC);
        break;
      case I_DIV:
        StackDiv(&GC);
        break;
      case I_INT:
        U16 call = StackPop(&GC);
        switch (call) {
          case C_EXIT:
            U16 Arg1 = StackPop(&GC);
            NCWN;
            exit(Arg1);
            break;
          default:
            fprintf(stderr, "%sUnknown interrupt %02Xh\n", ERROR, call);
        }
        break;
      default:
        fprintf(stderr, "%sUnknown instruction %02X\n  At position %04X\n", ERROR, GC.mem[GC.regs.PC], GC.regs.PC);
        goto FAILED;
      // StackDump(&GC, 5);
    }
    GC.regs.PC++;
  }
  NCWN;

  SUCESS:
    NCWN;
    return 0;
  FAILED:
    NCWN;
    return 1;
}

