// Govno Core 32 CPU
#include "../../lib/namings.h"

#define MEMSIZE 65536

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
  U8 mem[MEMSIZE];
} GC32;

// CPU functions
// Reset the SP register
U0 ResetSP(GC32* gccpu) {
  gccpu->regs.SP = 0xFFFF;
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
U8 PutByte(GC32* gccpu, U16 addr, U8 val) {
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

// Pop a word (16 bits) from the stack
U16 StackPop(GC32* gccpu) {
  gccpu->regs.SP += 2;
  return FetchWord(gccpu, gccpu->regs.SP);
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

