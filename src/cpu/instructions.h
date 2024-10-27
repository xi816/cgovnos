#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H 1

// Instruction set
#define I_NOP        0x00
#define I_CPUID      0x01
#define I_CSP        0x02
#define I_SHL        0x03
#define I_SHR        0x04
#define I_MOD        0x05
#define I_CALL       0x06
#define I_JMP        0x07
#define I_JMI        0x08
#define I_RET        0x09
#define I_CALI       0x0A

#define I_RDDW       0x10
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

#define I_RDDB       0x20
#define I_REAB       0x21
#define I_REAW       0x22
#define I_INT        0x23
#define I_DIV        0x24
#define I_RDDBB      0x25
#define I_RDDWR      0x26

#define I_LODB       0x35
#define I_LODW       0x36

#define I_LSP        0x4A
#define I_LCS        0x4B
#define I_SSP        0x4C
#define I_SCS        0x4D
#define I_SPC        0x4E

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
