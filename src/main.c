#include "main.h"

int duck_error;

/* #################################################### */
/* main */

void die(int t, char *error, ... )
{
  char temp[512];

  va_list ap;
  va_start(ap,error);
   vsnprintf(temp,511,error,ap);
  va_end(ap);

  switch(t)
  {
   case EFATAL:
     fprintf(stderr,"Script Error: %s\n",temp);
     duck_error=1;
     break;
   case EDROP:
     fprintf(stderr,"Duck Error: %s\n",temp);
     break;
   case EWARN:
    fprintf(stderr,"Warning: %s\n",temp);
     break;
  }
}


void duck_init()
{
/* init */
  std_init();
  program.first=0;
  program.last=0;
  duck_error=0;
}

void reset_duck_error()
{
  duck_error=0;
}

int load_script(char *filename)
{
  SCRIPT * script;
  FUNCTION * func;
  FUNCTION * next;

  duck_error=0;

  if (!filename) { die(EDROP,"No filename given.");return -1; }

/* insert script into program list */
  script=calloc(1,sizeof(SCRIPT));

/* load program */
  script->filename=filename;
  program.script=script;

  if(load_toplevel())
  {
    lchar_clear();
    return -1;
  }

/* first pass */
  if(first_pass())
  {
    lchar_clear();
    variable_clear();
    for (func=script->functions;func;func=next)
    {
      next=func->next;
      free(func->name);
      state_free(func->block);
      free(func);
    }
    return -1;
  }

/* clear up input data */
  lchar_clear();

/* allocate scipt variable data */
  script->var_area=data_new();
  array_resize(script->var_area,script->varcount);

/* clear variable data */
  variable_clear();

  script->filename=strdup(filename);
  if(program.last)
    program.last->next=script;
  else
    program.first=script;
  program.last=script;

  return 0;
}


int unload_script(char *filename)
{
  SCRIPT * script;
  FUNCTION * func;
  FUNCTION * next;

  if (!filename) { die(EDROP,"No filename given."); return -1; }

/* insert script into program list */
  for (script=program.first;script;script=script->next)
  if(!strcmp(script->filename,filename))
  {
    free(script->filename);
    if(script->prev)script->prev->next=script->next;
    else program.first=script->next;

    if(script->next)script->next->prev=script->prev;
    else program.last=script->prev;

    data_clear(script->var_area);
    for (func=script->functions;func;func=next)
    {
      next=func->next;
      free(func->name);
      state_free(func->block);
      free(func);
    }

    return 0;
  }
  die(EFATAL,"Script not found with that name.");
  return -1;
}
