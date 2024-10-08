#include <stdio.h>
#include <stdlib.h>

#include "../lib/namings.h"
#include "../lib/errormsg.h"

I32 main(I32 argc, U8** argv) {
  printf("Test\n");
  
  FILE* fl = fopen(argv[1], "r");
  if (!fl) {
    fprintf(stderr, "%sFile %s not found\n", ERROR, argv[1]);
    return 2;
  }
  return 0;
}

