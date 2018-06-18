
#ifndef _STACK_H_
#define _STACK_H_

/* #################################################### */
/* Function stack space (Used ONLY when running program) */

typedef struct _STACK
{
  DATA * retval;      /* return value WARNING: THIS DOESN'T FREE WITH
                                               STACK_FREE */
  DATA * args;        /* this doesn't either. it should be set up and
                           freed by the calling function */

  int ret;

  DATA * var_area;   /* variable area */
  long varcount;

  FUNCTION * func;  /* current function */

  struct _STACK *prev;
} STACK;

STACK * stack_new(long vars);
void stack_free();
DATA * stack_var(long index);


#endif
