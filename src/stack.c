#include "main.h"

/* #################################################### */
/* Function stack space (Used ONLY when running program) */

STACK * stack_new(long vars)
{
  STACK*s=(STACK*)calloc(1,sizeof(STACK));
  s->var_area=data_new();
  array_resize(s->var_area,vars);
  s->varcount=vars;
  s->prev=program.stack;
  s->ret=0;
  program.stack=s;
  return s;
}

void stack_free()
{
  STACK * s=program.stack;
    /* clear out data record */
  data_free(s->var_area);
  program.stack=s->prev;
  free(s);
}

DATA * stack_var(long index)
{
  DATA * dd=0;

  if (index==ARG_OFFSET) return program.stack->args;

  if (index<0)
  {
    index=-index;
    dd=array_get_index(program.script->var_area,0,index-1);
  } else {
    if(program.stack)
    dd=array_get_index(program.stack->var_area,0,index-ARG_OFFSET-1);
  }
  return dd;
}
