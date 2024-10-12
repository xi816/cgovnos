import os;
import sys;

# Tokens:
T_INS = 0x00;
T_INT = 0x01;
T_BYT = 0x02;
T_WOR = 0x03;
T_LAB = 0x04;
T_0ID = 0x05;
T_EOL = 0x06;
T_EOF = 0xFF;
HUMAN = [
  "INST", "INT", "BYTE", "WORD", "LABEL",
  "ID", "EOL"
];
HUMAN += (["UNKNOWN" for i in range(255-len(HUMAN))]);
HUMAN.append("EOF");

# Symbols:
ba = bytearray;
CAP = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
SML = "abcdefghijklmnopqrstuvwxyz";
INT = "0123456789";
INTEXT = "0123456789ABCDEF";
WHI = " \t\r";
KEY = (
  "PUSH", "POP", "ADD", "SUB", "MUL", "DIV",
  "DUP", "INT", "JMP", "JMI", "CEQ", "CNEQ"
);
KSZ = {
  "PUSH": 3,
  "POP": 1,
  "ADD": 1,
  "SUB": 1,
  "MUL": 1,
  "DIV": 1,
  "DUP": 1,
  "INT": 1,
  "JMP": 3,
  "JMI": 4,
  "CEQ": 1,
  "CNEQ": 1
};

# Lexer:
def Lex(prog: str):
  toks = [];
  pos = 0;
  cpos = 0;
  proglen = len(prog);
  basemode = 10;
  buf = "";
  while (True):
    if (prog[pos] == "\0"):
      toks.append((T_EOF,));
      return toks, 0;
    elif (prog[pos] in WHI):
      pos += 1;
    elif (prog[pos] == "%"):
      basemode = 16;
      pos += 1;
    elif (prog[pos] == "\n"):
      toks.append((T_EOL,));
      pos += 1;
    elif (prog[pos] in INT):
      while (prog[pos] in INTEXT):
        buf += prog[pos];
        pos += 1;
      toks.append((T_INT, int(buf, base=basemode)));
      basemode = 10;
      buf = "";
    elif (prog[pos] in CAP):
      while (prog[pos] in CAP):
        buf += prog[pos];
        pos += 1;
      if (prog[pos] == ":"):
        toks.append((T_LAB, buf, cpos));
        pos += 1;
      else:
        if (buf in KEY):
          toks.append((T_INS, buf, cpos));
          cpos += KSZ[buf];
        else:
          toks.append((T_0ID, buf));
      buf = "";
    else:
      print(f"\033[31mUnknown\033[0m character {hex(ord(prog[pos]))[2:].upper():0>2}");
      return [], 1;

  return [], 1;

def FetchLabels(prog: list):
  labs = dict();
  for i in prog:
    if (i[0] == T_LAB):
      labs[i[1]] = i[2];
  return labs;

def RemEmpty(prog: str):
  return "\n".join([i for i in prog.split("\n") if i]);

# Tokens Printer:
def PrintTokens(prog: list):
  for i,j in enumerate(prog):
    if (j[0] not in [T_INS, T_LAB]):
      if (len(j) == 1):
        print(f"    [%{hex(i)[2:].upper():0>4}] {HUMAN[j[0]]}");
      elif (len(j) == 2):
        print(f"    [%{hex(i)[2:].upper():0>4}] {HUMAN[j[0]]}-{j[1]}");
    else:
      if (len(j) == 2):
        print(f"    [%{hex(i)[2:].upper():0>4}] [%{hex(j[1])[2:].upper():0>4}] {HUMAN[j[0]]}");
      elif (len(j) == 3):
        print(f"    [%{hex(i)[2:].upper():0>4}] [%{hex(j[2])[2:].upper():0>4}] {HUMAN[j[0]]}-{j[1]}");

# Compiler:
def Govnbin(prog: list, labs: dict):
  code = bytearray();
  pos = 0;
  while (prog[pos][0] != T_EOF):
    if (prog[pos][0] == T_LAB):
      pos += 1;
      if (prog[pos][0] == T_EOL):
        pos += 1;
    elif (prog[pos][0] == T_INS):
      if (prog[pos][1] == "PUSH"):
        code.append(0x19);
        code.append(prog[pos+1][1] >> 8);
        code.append(prog[pos+1][1] % 256);
        pos += 2;
        if (prog[pos][0] == T_EOL):
          pos += 1;
      elif (prog[pos][1] == "JMP"):
        code.append(0x07);
        code.append(labs[prog[pos+1][1]] >> 8);
        code.append(labs[prog[pos+1][1]] % 256);
        pos += 2;
        if (prog[pos][0] == T_EOL):
          pos += 1;
      elif (prog[pos][1] == "INT"):
        code.append(0x23);
        pos += 1;
        if (prog[pos][0] == T_EOL):
          pos += 1;
      else:
        print(f"\033[31mUnknown\033[0m instruction {prog[pos][1]}");
        return code, 1;
    else:
      print(f"\033[31mUnknown\033[0m token {prog[pos]}");
      return code, 1;

  return code, 0;

def Main(argc: int, argv: list):
  progname = argv[1];
  outname = argv[2];
  DBGMODE = 0;
  if (argc == 4):
    if (argv[3] == "-D"):
      DBGMODE = 1;
    if (argv[3] == "-F"):
      DBGMODE = 2;

  with open(progname, "r") as fl:
    src = fl.read();
  src = RemEmpty(src)+"\0";

  if (DBGMODE == 2):
    print(f"  Started lexing file {progname}...");

  tokens, exitcode = Lex(src);
  if (DBGMODE == 2):
    if (not exitcode):
      PrintTokens(tokens);
      print(tokens);
      print(f"  File {progname} lexed \033[32msucessfully\033[0m");
    else:
      print(f"  File {progname} lexed \033[31munsucessfully\033[0m");
      return 1;
  if (DBGMODE > 0):
    print(f"  Compiling {progname}...");

  labs = FetchLabels(tokens);
  code, exitcode00 = Govnbin(tokens, labs);
  if (DBGMODE > 0):
    if (not exitcode00):
      print(f"  File {progname} compiled \033[32msuccesfully\033[0m");
      print("   ", code);
    else:
      print(f"  File {progname} compiled \033[31munsuccesfully\033[0m");
      return 1;
  with open(outname, "wb") as fl:
    fl.write(code);

  return 0;

sys.exit(Main(len(sys.argv), sys.argv));

