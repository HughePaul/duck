#include "main.h"


/* #################################################### */
/* Tokens */

TOKEN * token_new(){return (TOKEN*)calloc(1,sizeof(TOKEN));}

void token_free(TOKEN*token)
{
  TOKEN * next;

  for(;token;token=next)
  {
    next=token->next;
    if(token->type==TT_VALUE)data_clear(&token->d.v);

    if(token->name)free(token->name);
    if(token->e)token_free(token->e);

    free(token);
  }
}


/* #################################################### */
/* Variables - used for loading */

long variable_new(char *name)
{
  VARIABLE * var=(VARIABLE*)calloc(1,sizeof(VARIABLE));

#ifdef DEBUG_VARS
  printf("New variable %s in function: %s\n",name,(program.func)?program.func->name:"GLOBAL");
#endif

  var->name=strdup(name);
  if (program.func)
  {
    var->varpos= (++(program.func->varcount))+ARG_OFFSET;
    var->next=program.func->vars;
    program.func->vars=var;
  } else {
    var->varpos= -( ++(program.script->varcount) );
    var->next=program.script->vars;
    program.script->vars=var;
  }
  return var->varpos;
}


long variable_get(char *name)
{
  VARIABLE * var;

  if(!strcasecmp(name,"arg"))
    return ARG_OFFSET;

/* search global variables */

  if(program.func) var=program.func->vars; else var=program.script->vars;
  for (;var;var=var->next)
    if(!strcmp(var->name,name))
      return var->varpos;

  if (!program.func) return 0; /* variable not found! */

/* search global variables too */

  var=program.script->vars;
  for (;var;var=var->next)
    if(!strcmp(var->name,name))
      return var->varpos;
  return 0;
}

void variable_clear()
{
  VARIABLE * var;
  VARIABLE * next;
  FUNCTION * func;

/* clear global variables */

  var=program.script->vars;
  next=(var)?var->next:0;
  for (;var;var=next,next=(var)?var->next:0)
  {
    free(var->name);
    free(var);
  }

/* clear function variables */

  for (func=program.script->functions;func;func=func->next)
  {
    var=func->vars;
    next=(var)?var->next:0;
    for (;var;var=next,next=(var)?var->next:0)
    {
      free(var->name);
      free(var);
    }
  }

#ifdef DEBUG_VARS
  printf("All variables cleared\n");
#endif

}


/* #################################################### */
/* Functions */

FUNCTION * function_new(char * name)
{
  FUNCTION * func = (FUNCTION*)calloc(1,sizeof(FUNCTION));

#ifdef DEBUG_FUNC
  printf("New function: %s\n",name);
#endif

  func->name=strdup(name);
  func->next=program.script->functions;
  program.script->functions=func;
  func->varcount=0;

  return func;
}


FUNCTION * function_get(char *name)
{
  FUNCTION * func;
  SCRIPT * script;

/* optimise this to look at local script first? */

  for (script=program.first;script;script=script->next)
   for (func=script->functions;func;func=func->next)
    if(!strcmp(func->name,name))
      return func;

  return 0;
}



/* #################################################### */
/* Keywords */

KEYWORD * keyword_new(){return (KEYWORD*)calloc(1,sizeof(KEYWORD));}

void keyword_free(KEYWORD*keyword)
{
  int i;

  for(i=0;i<3;i++)
    if(keyword->expr[i])
      token_free(keyword->expr[i]);

  for(i=0;i<2;i++)
    if(keyword->block[i])
      state_free(keyword->block[i]);

  free(keyword);
}

/* #################################################### */
/* Statements */

STATE * state_new(){return (STATE*)calloc(1,sizeof(STATE));}

void state_free(STATE*state)
{
  STATE* next;

  for(;state;state=next)
  {
    next=state->next;
    switch(state->type)
    {
      case ST_EXPR:
        token_free(state->d.expr);
        break;
      case ST_BLOCK:
        state_free(state->d.block);
        break;
      case ST_KEYWORD:
        keyword_free(state->d.keyword);
        break;
    } 
    free(state);
  }
}

/* #################################################### */
/* #################################################### */


