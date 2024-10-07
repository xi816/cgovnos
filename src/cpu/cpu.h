// Govno Core 32 CPU
#include <curses.h>

#include "../../lib/namings.h"
#include "../../lib/gchar.h"
#include "../../lib/errormsg.h"
#include "../../lib/colors.h"

#include "instructions.h"

#define NCWN endwin()

// Registers structure
struct Registers {
  I16 A;
  I16 X;
  I16 Y;
  I32 CA;
  I32 CX;
  I32 CY;

  U16 SP;
  U16 CS;
  U16 PC;
  U8 FLAGS;
};

// CPU structure
typedef struct GC32 {
  struct Registers regs;
  U8* mem;
} GC32;

// CPU functions
// Reset the SP register
U0 ResetSP(GC32* gccpu) {
  gccpu->regs.SP = 0xFFFF;
}

// Reset the PC register
U0 ResetPC(GC32* gccpu) {
  gccpu->regs.PC = 0x0000;
}

// Reset the 16-bit general purpose registers (A, X, Y).
U0 ResetAXY(GC32* gccpu) {
  gccpu->regs.A = 0x0000;
  gccpu->regs.X = 0x0000;
  gccpu->regs.Y = 0x0000;
}

// Reset the 32-bit general purpose registers (CA, CX, CY).
U0 ResetCAXY(GC32* gccpu) {
  gccpu->regs.CA = 0x00000000;
  gccpu->regs.CX = 0x00000000;
  gccpu->regs.CY = 0x00000000;
}

// Reset flags
U0 ResetFLAGS(GC32* gccpu) {
  gccpu->regs.FLAGS = 0x00;
}

// Write a byte correctly to the screen
U0 PutByte(U8 a) {
  switch (a) {
    case 0x0A:
      puts("\r");
      break;
    default:
      putchar(a);
  }
}

// Fetch a byte from memory and return it
U8 FetchByte(GC32* gccpu, U16 addr) {
  return gccpu->mem[addr];
}

// Fetch a word (16 bits) from memory and return it
U16 FetchWord(GC32* gccpu, U16 addr) {
  return ((gccpu->mem[addr]) << 8) + (gccpu->mem[addr-1]);
}

// Fetch a word (16 bits) in reverse order from memory and return it
U16 FetchWordRev(GC32* gccpu, U16 addr) {
  return ((gccpu->mem[addr]) << 8) + (gccpu->mem[addr+1]);
}

// Put a byte into the memory
U8 WriteByte(GC32* gccpu, U16 addr, U8 val) {
  gccpu->mem[addr] = val;
}

// Put a word (16 bits) into the memory
U16 PutWord(GC32* gccpu, U16 addr, U16 val) {
  gccpu->mem[addr] = val >> 8;
  gccpu->mem[addr-1] = val % 256;
}

// Push a word (16 bits) into the stack
U0 StackPush(GC32* gccpu) {
  gccpu->mem[gccpu->regs.SP] = (gccpu->mem[gccpu->regs.PC+1]);
  gccpu->mem[gccpu->regs.SP-1] = (gccpu->mem[gccpu->regs.PC+2]);
  gccpu->regs.SP -= 2;
  gccpu->regs.PC += 2;
}

// Push a word (16 bits) into the stack
U0 StackPushInl(GC32* gccpu, I16 val) {
  gccpu->mem[gccpu->regs.SP] = (val >> 8);
  gccpu->mem[gccpu->regs.SP-1] = (val % 256);
  gccpu->regs.SP -= 2;
}

// Pop a word (16 bits) from the stack
U16 StackPop(GC32* gccpu) {
  gccpu->regs.SP += 2;
  return FetchWord(gccpu, gccpu->regs.SP);
}

// Duplicate a word (16 bits) in the stack
U0 StackDup(GC32* gccpu) {
  StackPushInl(gccpu, FetchWord(gccpu, gccpu->regs.SP+2));
}

// Add two word values (16 bits) in the stack
U0 StackAdd(GC32* gccpu) {
  U16 a = FetchWord(gccpu, gccpu->regs.SP+4);
  U16 b = FetchWord(gccpu, gccpu->regs.SP+2);
  PutWord(gccpu, gccpu->regs.SP+4, a+b);
  gccpu->regs.SP += 2;
}

// Subtract two word values (16 bits) in the stack
U0 StackSub(GC32* gccpu) {
  U16 a = FetchWord(gccpu, gccpu->regs.SP+4);
  U16 b = FetchWord(gccpu, gccpu->regs.SP+2);
  PutWord(gccpu, gccpu->regs.SP+4, a-b);
  gccpu->regs.SP += 2;
}

// Multiply two word values (16 bits) in the stack
U0 StackMul(GC32* gccpu) {
  U16 a = FetchWord(gccpu, gccpu->regs.SP+4);
  U16 b = FetchWord(gccpu, gccpu->regs.SP+2);
  PutWord(gccpu, gccpu->regs.SP+4, a*b);
  gccpu->regs.SP += 2;
}

// Divide two word values (16 bits) in the stack
U0 StackDiv(GC32* gccpu) {
  U16 a = FetchWord(gccpu, gccpu->regs.SP+4);
  U16 b = FetchWord(gccpu, gccpu->regs.SP+2);
  PutWord(gccpu, gccpu->regs.SP+4, a/b);
  gccpu->regs.SP += 2;
}

#include "../dumps.h"
U16 flen;
U8 Execute(GC32 GC) {
  U16 Arg1;

  while (true) {
    switch (GC.mem[GC.regs.PC]) {
      case I_CPUID:
        StackPushInl(&GC, MEMSIZE);
        break;
      case I_CSP:
        GC.regs.Y = getch();
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
      case I_DUP:
        StackDup(&GC);
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
            Arg1 = StackPop(&GC);
            NCWN;
            exit(Arg1);
            break;
          case C_WRITE:
            Arg1 = StackPop(&GC);
            while (GC.mem[Arg1] != 0) {
              PutByte(GC.mem[Arg1]);
              Arg1++;
            }
            break;
          case C_WRCHR:
            Arg1 = StackPop(&GC);
            PutByte(Arg1);
            break;
          default:
            fprintf(stderr, "%sUnknown interrupt %02Xh\n", ERROR, call);
        }
        break;
      default:
        fprintf(stderr, "%sUnknown instruction %02X\n  At position %04X\n", ERROR, GC.mem[GC.regs.PC], GC.regs.PC);
        return 1;
    }
    GC.regs.PC++;
  }
  return 0;
}

