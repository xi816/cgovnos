#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

// Instruction set
#define I_NOP        0x00
#define I_CPUID      0x01
#define I_CSP        0x02
#define I_FLF        0x03
#define I_FLRA       0x04
#define I_FLRB       0x05
#define I_FLRC       0x06
#define I_JMP        0x07
#define I_JMI        0x08
#define I_RAD        0x09
#define I_RADI       0x0A
#define I_RSU        0x0B
#define I_RSUI       0x0C
#define I_RML        0x0D
#define I_RMLI       0x0E
#define I_RDV        0x0F
#define I_DRVI       0x10
#define I_CMP        0x11
#define I_CMIM       0x12
#define I_AND        0x13
#define I_OR         0x14
#define I_XOR        0x15
#define I_NAND       0x16
#define I_NOR        0x17
#define I_NOT        0x18
#define I_PUSH       0x19
#define I_POP        0x1A
#define I_DUP        0x1B
#define I_SWAP       0x1C
#define I_ADD        0x1D
#define I_SUB        0x1E
#define I_MUL        0x1F

#define I_RDD        0x20
#define I_LOD16      0x21
#define I_LOD32      0x22
#define I_INT        0x23
#define I_DIV        0x24

#define I_LAB16      0x40
#define I_LAB32      0x41
#define I_LAR16      0x42
#define I_LAR32      0x43

// Interrupts
#define C_EXIT       0x00
#define C_WRITE      0x01
#define C_WRCHR      0x02
#define C_RCHAR      0x03
#define C_RCHAE      0x04
#define C_RIBTE      0x05
#define C_RIWRD      0x06
#define C_VIDEO      0x10

// VIDEO interrupts
#define INT_VIDEO_CLEAR            0x00
#define INT_VIDEO_SLEEP            0x0A

// JMI condition table
#define B_JMI_EQ     0x00
#define B_JMI_NEQ    0x01

#endif

