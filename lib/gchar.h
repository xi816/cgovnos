#include "../lib/namings.h"

void SSFlush(FILE *in) {
  U8 ch;
  do
    ch = fgetc(in);
  while (ch != EOF && ch != '\n');
  clearerr(in);
}

void SSPause(void) {
  fflush(stdin);
  getchar();
}

