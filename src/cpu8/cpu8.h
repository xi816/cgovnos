// Govno Core 32 CPU
#include <termios.h>

#include "../../lib/bool.h"
#include "../../lib/namings.h"
#include "../../lib/errormsg.h"
#include "../../lib/colors.h"

#include "instructions.h"

// Registers structure
struct Registers {
  U16 SP;   // Stack pointer
  U16 CS;   // Common stack
  U16 PC;   // Program counter
  U16 A;    // Accumulator
  U16 B;    // Base
  U16 C;    // Counter
  U16 D;    // Data
  U8 FLAGS; // Flags
};

// CPU structure
typedef struct GC16 {
  struct Registers regs;
  U8* mem;  // Memory
  U8* rom;  // ROM disk
} GC16;

// CPU functions
// Reset the SP register
U0 ResetSP(GC16* gccpu) {
  gccpu->regs.SP = 0xFFFF;
}

// Reset the PC register
U0 ResetPC(GC16* gccpu) {
  gccpu->regs.PC = 0x0000;
}

// Reset flags
U0 ResetFLAGS(GC16* gccpu) {
  gccpu->regs.FLAGS = 0x00;
}

// Write a byte to the screen
U0 PutByte(U8 a) {
  switch (a) {
    case 0x7F:
      printf("\x08 \x08");
      break;
    default:
      putchar(a);
  }
}

// Fetch a byte from memory and return it
U8 FetchByte(GC16* gccpu, U16 addr) {
  return gccpu->mem[addr];
}

// Fetch a word (16 bits) from memory and return it
U16 FetchWord(GC16* gccpu, U16 addr) {
  return ((gccpu->mem[addr]) << 8) + (gccpu->mem[addr-1]);
}

// Fetch a word (16 bits) in reverse order from memory and return it
U16 FetchWordRev(GC16* gccpu, U16 addr) {
  return ((gccpu->mem[addr]) << 8) + (gccpu->mem[addr+1]);
}

// Put a byte into the memory
U8 WriteByte(GC16* gccpu, U16 addr, U8 val) {
  gccpu->mem[addr] = val;
}

// Put a word (16 bits) into the memory
U16 PutWord(GC16* gccpu, U16 addr, U16 val) {
  gccpu->mem[addr] = val >> 8;
  gccpu->mem[addr-1] = val % 256;
}

// Push a word (16 bits) into the stack
U0 StackPush(GC16* gccpu) {
  gccpu->mem[gccpu->regs.SP] = (gccpu->mem[gccpu->regs.PC+1]);
  gccpu->mem[gccpu->regs.SP-1] = (gccpu->mem[gccpu->regs.PC+2]);
  gccpu->regs.SP -= 2;
  gccpu->regs.PC += 2;
}

// Push a word (16 bits) into the stack
U0 StackPushInl(GC16* gccpu, I16 val) {
  gccpu->mem[gccpu->regs.SP] = (val >> 8);
  gccpu->mem[gccpu->regs.SP-1] = (val % 256);
  gccpu->regs.SP -= 2;
}

// Pop a word (16 bits) from the stack
U16 StackPop(GC16* gccpu) {
  gccpu->regs.SP += 2;
  return FetchWord(gccpu, gccpu->regs.SP);
}

// Duplicate a word (16 bits) in the stack
U0 StackDup(GC16* gccpu) {
  StackPushInl(gccpu, FetchWord(gccpu, gccpu->regs.SP+2));
}

// Swap two words (16 bits) in the stack
U0 StackSwap(GC16* gccpu) {
  I16 a = StackPop(gccpu);
  I16 b = StackPop(gccpu);
  StackPushInl(gccpu, a);
  StackPushInl(gccpu, b);
}

// Add two word values (16 bits) in the stack
U0 StackAdd(GC16* gccpu) {
  U16 a = FetchWord(gccpu, gccpu->regs.SP+4);
  U16 b = FetchWord(gccpu, gccpu->regs.SP+2);
  PutWord(gccpu, gccpu->regs.SP+4, a+b);
  gccpu->regs.SP += 2;
}

// Subtract two word values (16 bits) in the stack
U0 StackSub(GC16* gccpu) {
  U16 a = FetchWord(gccpu, gccpu->regs.SP+4);
  U16 b = FetchWord(gccpu, gccpu->regs.SP+2);
  PutWord(gccpu, gccpu->regs.SP+4, a-b);
  gccpu->regs.SP += 2;
}

// Multiply two word values (16 bits) in the stack
U0 StackMul(GC16* gccpu) {
  U16 a = FetchWord(gccpu, gccpu->regs.SP+4);
  U16 b = FetchWord(gccpu, gccpu->regs.SP+2);
  PutWord(gccpu, gccpu->regs.SP+4, a*b);
  gccpu->regs.SP += 2;
}

// Divide two word values (16 bits) in the stack
U0 StackDiv(GC16* gccpu) {
  U16 a = FetchWord(gccpu, gccpu->regs.SP+4);
  U16 b = FetchWord(gccpu, gccpu->regs.SP+2);
  PutWord(gccpu, gccpu->regs.SP+4, a/b);
  gccpu->regs.SP += 2;
}

U0 LoadBootableDrive(GC16* gccpu) {
  U16 ind = 0x8000;
  while (gccpu->rom[ind] != 0xFF) {
    gccpu->mem[ind-0x8000] = gccpu->rom[ind];
    ind++;
  }
}

#include "../dumps.h"
U8 Execute(GC16 GC) {
  U16 Arg1;
  U16 Arg2;

  while (true) {
    switch (GC.mem[GC.regs.PC]) {
      case I_NOP:
        break;
      case I_CPUID:
        StackPushInl(&GC, memsize);
        break;
      case I_CSP:
        StackPushInl(&GC, getchar());
        break;
      case I_SHL: {
          Arg1 = StackPop(&GC);
          I16 a = FetchWordRev(&GC, GC.regs.PC+1);
          StackPushInl(&GC, Arg1<<a);
          GC.regs.PC += 2;
        } break;
      case I_SHR: {
          Arg1 = StackPop(&GC);
          I16 a = FetchWordRev(&GC, GC.regs.PC+1);
          StackPushInl(&GC, Arg1>>a);
          GC.regs.PC += 2;
        } break;
      case I_JMP:
        GC.regs.PC = FetchWordRev(&GC, GC.regs.PC+1)-1;
        break;
      case I_CALL:
        StackPushInl(&GC, GC.regs.PC+3);
        GC.regs.PC = FetchWordRev(&GC, GC.regs.PC+1)-1;
        break;
      case I_RET:
        Arg1 = StackPop(&GC)-1;
        GC.regs.PC = Arg1;
        break;
      case I_JMI: {
        U8 condition = FetchByte(&GC, GC.regs.PC+1);
        switch (condition) {
          case B_JMI_EQ:
            if (StackPop(&GC) != 0) {
              GC.regs.PC = FetchWordRev(&GC, GC.regs.PC+2)-1;
            }
            else {
              GC.regs.PC += 3;
            }
            break;
          case B_JMI_NEQ:
            if (StackPop(&GC) == 0) {
              GC.regs.PC = FetchWordRev(&GC, GC.regs.PC+2)-1;
            }
            else {
              GC.regs.PC += 3;
            }
            break;
          default:
            fprintf(stderr, "%sUnknown JMI operand: %02X\n", ERROR, condition);
            exit(1);
        }
      } break;
      case I_CALI: {
        U8 condition = FetchByte(&GC, GC.regs.PC+1);
        switch (condition) {
          case B_JMI_EQ: // JMI substructions are the same for CALI
            if (StackPop(&GC) != 0) {
              StackPushInl(&GC, GC.regs.PC+4);
              GC.regs.PC = FetchWordRev(&GC, GC.regs.PC+2)-1;
            }
            else {
              GC.regs.PC += 3;
            }
            break;
          case B_JMI_NEQ:
            if (StackPop(&GC) == 0) {
              StackPushInl(&GC, GC.regs.PC+4);
              GC.regs.PC = FetchWordRev(&GC, GC.regs.PC+2)-1;
            }
            else {
              GC.regs.PC += 3;
            }
            break;
          default:
            fprintf(stderr, "%sUnknown CALI operand: %02X\n", ERROR, condition);
            exit(1);
        }
      } break;
      case I_CMP:
        Arg1 = StackPop(&GC);
        Arg2 = StackPop(&GC);
        StackPushInl(&GC, (Arg1 == Arg2));
        break;
      case I_CMIM:
        Arg1 = StackPop(&GC);
        StackPushInl(&GC, (Arg1 != 0));
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
      case I_SWAP:
        StackSwap(&GC);
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
      case I_RDDB:
        StackPushInl(&GC, GC.rom[FetchWordRev(&GC, GC.regs.PC+1)]%256);
        GC.regs.PC += 2;
        break;
      case I_REAB:
        StackPushInl(&GC, GC.mem[FetchWordRev(&GC, GC.regs.PC+1)]%256);
        GC.regs.PC += 2;
        break;
      case I_RDDW:
        StackPushInl(&GC, GC.rom[FetchWordRev(&GC, GC.regs.PC+1)]);
        GC.regs.PC += 2;
        break;
      case I_RDDWR:
        StackPushInl(&GC, GC.rom[FetchWord(&GC, GC.regs.PC+1)]);
        GC.regs.PC += 2;
        break;
      case I_REAW:
        StackPushInl(&GC, GC.mem[FetchWordRev(&GC, GC.regs.PC+1)]);
        GC.regs.PC += 2;
        break;
      case I_INT:
        U16 call = StackPop(&GC);
        switch (call) {
          case C_EXIT:
            Arg1 = StackPop(&GC);
            return 3;
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
          case C_VIDEO:
            Arg1 = StackPop(&GC);
            switch (Arg1) {
              case INT_VIDEO_SLEEP:
                Arg2 = StackPop(&GC);
                sleep(Arg2);
                break;
             case INT_VIDEO_CLEAR:
                printf("\033[H\033[2J");
                break;
              default:
                fprintf(stderr, "%sUnknown video interrupt %02Xh\n", ERROR, Arg1);
                return 1;
            }
            break;
          default:
            fprintf(stderr, "%sUnknown interrupt %02Xh\n", ERROR, call);
            fprintf(stderr, "  At position %04Xh\n", GC.regs.PC);
            return 1;
        }
        break;
      case I_LODB:
        Arg1 = StackPop(&GC);
        GC.mem[FetchWordRev(&GC, GC.regs.PC+1)] = Arg1;
        GC.regs.PC += 2;
        break;
      case I_LSP:
        GC.regs.SP = FetchWordRev(&GC, GC.regs.PC+1);
        GC.regs.PC += 2;
        break;
      case I_LCS:
        GC.regs.CS = FetchWordRev(&GC, GC.regs.PC+1);
        GC.regs.PC += 2;
        break;
      case I_SSP:
        StackPushInl(&GC, GC.regs.SP);
        break;
      case I_SCS:
        StackPushInl(&GC, GC.regs.CS);
        break;
      case I_SPC:
        StackPushInl(&GC, GC.regs.PC);
        break;
      case 0xA9: // Special VM codes
        switch (GC.mem[GC.regs.PC+1]) {
          case 0x55:
            while (1) {}
          case 0x56:
            GC.regs.PC = 0x7FFF;
          default:
            fprintf(stderr, "%sUnknown VM code %02X\n  At position %04X\n", ERROR, GC.mem[GC.regs.PC+1], GC.regs.PC+1);
            return 1;
        }
        break;
      default:
        fprintf(stderr, "%sUnknown instruction %02X\n  At position %04X\n", ERROR, GC.mem[GC.regs.PC], GC.regs.PC);
        return 1;
    }
    GC.regs.PC++;
    // printf("\033[32mPC[%04X] = %02X\033[0m\n", GC.regs.PC, GC.mem[GC.regs.PC]);
    // StackDump(&GC, 10);
  }
  return 0;
}
