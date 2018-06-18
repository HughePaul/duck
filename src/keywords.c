#include "main.h"

/* #################################################### */
/* #################################################### */

void run_keyword(KEYWORD * key)
{
  DATA data;
  DATA *dp;
  long i;

  if (!key)return;

  memset(&data,0,sizeof(DATA));
  data.t=DT_NONE;
  
  switch(key->keyword)
  {
    case KEY_IF:
      if(data2int(run_expr(&data,key->expr[0],0)))
        run_block(key->block[0]);
      else
        run_block(key->block[1]);
    break;


    case KEY_FOR:
      for(run_expr(0,key->expr[0],0);
          data2int(run_expr(&data,key->expr[1],0));
          run_expr(0,key->expr[2],0))
            run_block(key->block[0]);
    break;

    case KEY_FOREACH:
      run_expr(&data,key->expr[0],0);
      for(i=0;(dp=array_get_index(&data,0,i));i++)
      {
       data_dup(stack_var(key->var),dp);
        run_block(key->block[0]);
      }
    break;


    case KEY_RETURN:
      run_expr(program.stack->retval,key->expr[0],0);
      program.stack->ret=1;
    break;

    case KEY_WHILE:
      while(data2int(run_expr(&data,key->expr[0],0)))
        run_block(key->block[0]);
    break;

    case KEY_DO:
      do { run_block(key->block[0]); }
        while(data2int(run_expr(&data,key->expr[0],0)));
    break;

  }

  data_clear(&data);
}

/* #################################################### */
/* #################################################### */


KEYWORD * pass_keyword(LCHAR ** c, long key)
{
  KEYWORD * keyword=keyword_new();

  long line;
  char *s;

  keyword->keyword=key;

#ifdef DEBUG_BLOCK
  printf("Start Keyword\n");
#endif

  switch(key)
  {

    case KEY_IF:
      if(_nextc(c)) return keyword;

      line=(*c)->line;
      if (*(*c)->s!='(')
      {
        die(EFATAL,"Line %d: '(' expected but '%s' found.",line,(*c)->s);
        return keyword;
      }
      keyword->expr[0]=pass_expr(c,')');
      if(duck_error) return keyword;
      if(_nextc(c)) return keyword;
      keyword->block[0]=pass_state(c);
      if(duck_error) return keyword;

      line=(*c)->line;
      if (!(*c)->next)
      {
        die(EFATAL,"Line %d: Unexpected end of file.",line);
        return keyword;
      }

      if(!strcasecmp((*c)->next->s,"else"))
      {
        if(_nextc(c)) return keyword;
        if(_nextc(c)) return keyword;
        keyword->block[1]=pass_state(c);
        if(duck_error) return keyword;
      }
    break;


    case KEY_FOR:
      if(_nextc(c)) return keyword;

      line=(*c)->line;
      if (*(*c)->s!='(')
      {
        die(EFATAL,"Line %d: '(' expected but '%s' found.",line,(*c)->s);
        return keyword;
      }
      keyword->expr[0]=pass_expr(c,';');
      if(duck_error) return keyword;
      keyword->expr[1]=pass_expr(c,';');
      if(duck_error) return keyword;
      keyword->expr[2]=pass_expr(c,')');
      if(duck_error) return keyword;
      if(_nextc(c)) return keyword;
      keyword->block[0]=pass_state(c);
      if(duck_error) return keyword;
    break;


    case KEY_FOREACH:
      if(_nextc(c)) return keyword;

      line=(*c)->line;

/* get target variable */
      s=(*c)->s;
      if(*s=='$') s++;
      if(!(keyword->var=variable_get(s))) keyword->var=variable_new(s);
      if(!(keyword->var))
      {
        die(EFATAL,"Line %d: '%s' is not a valid variable for 'foreach' assignment.",line,s);
        return keyword;
      }
      if(_nextc(c)) return keyword;

      line=(*c)->line;

/* get array */
      if (*(*c)->s!='(')
      {
        die(EFATAL,"Line %d: '(' expected but '%s' found.",line,(*c)->s);
        return keyword;
      }
      keyword->expr[0]=pass_expr(c,')');
      if(duck_error) return keyword;

      if(_nextc(c)) return keyword;

      keyword->block[0]=pass_state(c);
      if(duck_error) return keyword;
    break;


    case KEY_RETURN:
      keyword->expr[0]=pass_expr(c,';');
      if(duck_error) return keyword;
    break;


    case KEY_WHILE:

      if(_nextc(c)) return keyword;

      line=(*c)->line;
      if (*(*c)->s!='(')
      {
        die(EFATAL,"Line %d: '(' expected but '%s' found.",line,(*c)->s);
        return keyword;
      }

      keyword->expr[0]=pass_expr(c,')');
      if(duck_error) return keyword;

      if(_nextc(c)) return keyword;

      keyword->block[0]=pass_state(c);
      if(duck_error) return keyword;
    break;

    case KEY_DO:

      if(_nextc(c)) return keyword;

      line=(*c)->line;

      keyword->block[0]=pass_state(c);
      if(duck_error) return keyword;

      if(_nextc(c)) return keyword;

      if (strcasecmp((*c)->s,"while"))
      {
/*        die(EFATAL,"Line %d: '(' expected but '%s' found.",line,(*c)->s); */
        return keyword;
      }

      keyword->expr[0]=pass_expr(c,';');
      if(duck_error) return keyword;


    break;

  }

  return keyword; 
}
