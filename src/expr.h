
#ifndef _EXPR_H_
#define _EXPR_H_

/* #################################################### */
/* expression stack */

typedef struct _EXSITEM
{
  DATA d;

  struct _EXSITEM * prev;
} EXSITEM;

typedef struct _EXS
{
  struct _EXSITEM * top;
  long count;

  struct _EXS * prev;
} EXS;

/* #################################################### */

EXSITEM * exs_push(EXS * s); /* callocs mem and inserts */
EXSITEM * exs_pop(EXS * s);  /* removes from stack */
void exsitem_free(EXSITEM * e);   /* frees mem and data */

void exs_clear(EXS * s);          /* clears entire stack */

/* #################################################### */
/* #################################################### */

DATA * run_expr(DATA * retval, TOKEN * token,int leave_as_array);

#endif
