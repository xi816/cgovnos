#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#define FLSIZE 64

void old_st(struct termios oldt) {
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

int main() {
  struct termios oldt;
  struct termios newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  
  char x = ' ';
  unsigned char code[FLSIZE];
  short index = 0;
  printf("\033[H\033[2J");

  while (x != 'q') {
    printf("\033[H\033[2J");
    for (short i = 0; i < FLSIZE; i++) {
      if (!(i % 16) && (i > 0))
        putchar(10);
      printf("%02X ", code[i]);
    }
    printf("\033[%d;%dH", index/16+1, index%16*3+1);
    x = getchar();
    switch (x) {
      case 'a': case 'A':
        putchar('A');
        break;
      case 'h':
        if (index > 0)
          index--;
        break;
      case 'l':
        if (index < FLSIZE-1)
          index++;
        break;
      case 'q': case 'Q':
        goto exitProg;
        break;
    }
  }

  exitProg:
    old_st(oldt);
  return 0;
}
