U0 StackDump(GC32* GC, I16 size) {
  printf("POS: %04X\r\n", GC->regs.PC);
  for (I16 i = 0; i < size; i++) {
    if (0xFFFF-i == GC->regs.SP) {
      printf("Stack value (\x1B[32m%04X\x1B[0m): %02X\r\n", 0xFFFF-i, GC->mem[0xFFFF-i]);
    }
    else {
      printf("Stack value (%04X): %02X\r\n", 0xFFFF-i, GC->mem[0xFFFF-i]);
    }
  }
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

