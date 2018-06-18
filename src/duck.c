#include "main.h"

/* #################################################### */
/* main */


int main(int argc,char *argv[])
{
  int r;

  if (argc<2) die(EFATAL,"No filename given.");

/* load scripts */
  duck_init();

/* set custom functions and constants here */
/* custom_constants = ... */
/* custom_functions = ... */

/* load scripts */
  for (r=1;r<argc;r++)
    load_script(argv[r]);

/* run init scripts */
  run_init();


/* Rest of program here */


/* run done scripts */
  run_done();

/* unload scripts */
  for (r=1;r<argc;r++)
    unload_script(argv[r]);

  return 0;
}
