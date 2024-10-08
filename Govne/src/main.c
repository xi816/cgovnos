#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>

#include "../lib/namings.h"

U8 main(U16 argc, U8** argv) {
  initscr();
  cbreak();
  noecho();

  U8 a[50] = {};
  for (I16 i = 0; i < 50; i++) {
    a[i] = i;
  }
  U8 ast[16];
  U8 MSIZE = 3;
  sprintf(ast, "%c", a);

  while (true) {
    erase();
    addstr("| ");
    sprintf(ast, "%f", ceil((sizeof(a)/MSIZE)*MSIZE));
    addstr(ast);
    // for (I16 i = 0; i < (ceil(sizeof(a)/MSIZE)*MSIZE); i++) {
    for (I16 i = 0; i < sizeof(a); i++) {
      if ((i % MSIZE == 0) && (i != 0))
        addstr("|\n| ");
      if (i <= sizeof(a))
        sprintf(ast, "%02X ", a[i]);
      else
        sprintf(ast, "XX ", a[i]);
      addstr(ast);
    }
    addstr("|\n");

    refresh();

    U8 key = getch();
    switch (key) {
      case 4:
        endwin();
        exit(0);
        break;
      case 45:
        if (MSIZE < 17) MSIZE++;
        break;
      case 43:
        if (MSIZE > 2) MSIZE--;
        break;
        // sprintf(ast, "%c", a);
    }
  }

  endwin();
}

