#include "main.h"

/* #################################################### */
/* #################################################### */

void run_block(STATE * state)
{
  if (!state)return;

#ifdef DEBUG_RUN
  printf("Running block in %s = %s\n",program.stack->func->name,
      (state->type==1)?"Expression":
      (state->type==2)?"Keyword":"Block");
#endif

  for(;state;state=state->next)
  {
    switch(state->type)
    {
     case ST_EXPR:
       run_expr(0,state->d.expr,0);
     break;
     case ST_BLOCK:
       run_block(state->d.block);
     break;
     case ST_KEYWORD:
       run_keyword(state->d.keyword);
     break;
    }

    if(program.stack->ret)return;
  }

  return;
}

/* #################################################### */
/* #################################################### */

void run_func(FUNCTION * func, DATA * retval, DATA * args)
{
  if (!func)return;

#ifdef DEBUG_RUN
  printf("Running function: %s @ %d returning to %d\n",
  func->name,(int)func->block,(int)retval);
#endif

  stack_new(func->varcount);
  program.stack->func=func;
  program.stack->retval=retval;
  program.stack->args=args;
  data_clear(retval);

  run_block(func->block);

  stack_free(); 
}


/* #################################################### */
/* #################################################### */


void run_init()
{
  DATA retval;
  SCRIPT * c;

  memset(&retval,0,sizeof(DATA));
  retval.t=DT_NONE;

  for(c=program.first;c;c=c->next)
    if(c->init)
      run_func(c->init,&retval,0);

  data_clear(&retval);
}


void run_done()
{
  DATA retval;
  SCRIPT * c;

  memset(&retval,0,sizeof(DATA));
  retval.t=DT_NONE;

  for(c=program.first;c;c=c->next)
    if(c->done)
      run_func(c->done,&retval,0);

  data_clear(&retval);
}

