#define _NEED_STD_
#define _NEED_KEYWORDS_
#define _NEED_OPERS_
#include "main.h"

/* #################################################### */
/* #################################################### */

int _nextc(LCHAR ** c)
{
  LCHAR *b=(*c)->next;
  if (!b)
  {
    die(EFATAL,"Line %d: Unexpected end of file.",(*c)->line);
    return -1;
  }
  *c=b;
  return 0;
}

int pass_my(LCHAR ** c)
{
  char *s=0;
  long line=0;

  nextc(c);

  for (;*c;*c=(*c)->next)
  {
    s=(*c)->s;
    line=(*c)->line;
    if (!strcmp(";",s)) return 1;
    if (*s=='$')
    {
      s++;
      if( ( (*s>='a') && (*s<='z') ) || 
          ( (*s>='A') && (*s<='Z') ) ) 
      {
        if (!strcasecmp(s,"arg"))
        { 
          die(EFATAL,"Line %d: Reserved variable $ARG declared in 'my' statement",line);
          return 0;
        }
        variable_new(s);
      }
      else
      {
         die(EFATAL,"Line %d: Invalid variable in 'my' statement: '%s'",
              line,--s);
         return 0;
      }
    }
    else if ( (*s==',') || (*s=='(')|| (*s==')') );
    else
    {
       die(EFATAL,"Line %d: Invalid variable in 'my' statement: '%s'",
             line,s);
       return 0;
    }
  }
  die(EFATAL,"Line %d: Unexpected end of file.",line);
  return 0;
}

/* #################################################### */
/* #################################################### */
/* pass_expr */

TOKEN * token_grab(LCHAR ** c)
{
  TOKEN * token=token_new();
  char * s;
  long line;
  int i,e;
  char ** chrarr;

    s=(*c)->s;
    line=(*c)->line;


#ifdef DEBUG_BLOCK
  printf(" MINE: %s\n",s);
#endif


/* Variable */

      if ( (*s=='$') || (*s=='#') || (*s=='@') || (*s=='?') )
      {
        if(!(token->d.s=variable_get(s+1)))
          token->d.s=variable_new(s+1);
        if(!token->d.s)
          {die(EFATAL,"Line %d: Unknown variable '%s' in function '%s'.",line,s,program.func->name);
          return 0;}
        token->type=*s;
        token->name=strdup(s);

      }else


/* Number */

      if( ((*s>='0') && (*s<='9')) || (*s=='.') )
      {
        token->type=TT_VALUE;
        number_make(&token->d.v,s,0,0);
      }else


/* String */

      if(*s=='"')
      {
        token->type=TT_VALUE;
        string_make(&token->d.v,s+1);
      }else


/* Expression */

      if(*s=='(')
      {
        token->type=TT_EXPRESSION;
        token->e=pass_expr(c,')');
        if(duck_error) return 0;
      }else



/* Functions and Constants */

      if( ((*s>='a') && (*s<='z')) || ((*s>='A') && (*s<='Z')) )
      {

/* language internals */

  /* constants */

for(chrarr=std_constants,e=2;(e)&&(chrarr);chrarr=custom_constants,e--)
        for(i=0;chrarr[i];i+=3)  /* see std.h */
          if(!strcmp(s,chrarr[i]))
          {
            token->type=TT_VALUE;
            token->d.v.t=(long)chrarr[i+1];
            switch(token->d.v.t)
            {
              case DT_STRING:
                string_make(&token->d.v,chrarr[i+2]);
              break;
              case DT_NUMBER:
                token->d.v.d.n=atof(chrarr[i+2]);
              break;
              case DT_LONG:
              case DT_FILE:
                token->d.v.d.i=(long)chrarr[i+2];
              break;
            }
          }

  /* functions */


for(chrarr=std_functions,e=2;(e)&&(chrarr);chrarr=custom_functions,e--)
        for(i=0;chrarr[i];i+=2)  /* see std.h */
          if(!strcmp(s,chrarr[i]))
          {
            token->type=TT_INTFUNC;
            token->name=strdup(s);
            token->d.s=(long)chrarr[i+1];
            break;
          }

/* all other functions */

        if(!token->type)
        {
          token->type=TT_FUNCTION;
          token->d.f=0;
          token->name=strdup(s);
        }

      } else


/* Operator */

      {
        token->type=TT_OPERATOR;
        for(i=0;opers[i];i+=2)
          if(!strcmp(s,opers[i]))
          {
            token->d.s=(long)opers[i+1];
            return token;
          }
        die(EFATAL,"Line %d: Unknown operator '%s' in function '%s'.",line,s,program.func->name);
        return 0;
      }



 /* Variable subscripts */

      if(((*c)->next) && (*(*c)->next->s=='['))
      {
        if((token->type==TT_VARIABLE)||
           (token->type==TT_VARIABLE_P)||
           (token->type==TT_VARIABLE_L)||
           (token->type==TT_VARIABLE_A))
        {
          nextc(c);
          token->e=pass_expr(c,']');
          if(duck_error) return 0;
        } else {
          die(EFATAL,"Line %d: Subscript used on non-variable in function '%s'.",line,program.func->name);
          return 0;
        }
      }




 /* Function Args */

      if((token->type==TT_INTFUNC)||
         (token->type==TT_FUNCTION))
      {
        if(((*c)->next) && (*(*c)->next->s=='('))
        {
          nextc(c);
          token->e=pass_expr(c,')');
          if(duck_error) return 0;
        }
      }

  return token;
}

#define WHAT_AFTER 	1
#define WHAT_BEFORE	2

TOKEN * pass_expr(LCHAR ** c,char limit)
{
  TOKEN * first=0;
  TOKEN * prev=0;

  TOKEN * a=0;
  TOKEN * b=0;

  TOKEN * token1=0;
  TOKEN * token2=0;
  TOKEN * token3=0;
  TOKEN * token4=0;

  TOKEN * old=0;

  char *s;
  long line=0;

  int what=0;

#ifdef DEBUG_BLOCK
  printf("pass_expr\n");
#endif

  if(_nextc(c)) return first;

  while(*c)
  {
    s=(*c)->s;
    line=(*c)->line;

/* check for end of expression */

    if( (*s==';') || (*s==')') || (*s==']')
     || (*s=='{') || (*s=='}'))
    {
      if (limit==*s) return first;
      { die(EFATAL,"Line %d: '%c' expected but '%c' found.",
         line,limit,*s);
        return first;}
    } else {

      if(!first)
      {
        if(!(token1=token_grab(c))){return first;}

        old=token1;
        prev=token1;

        if(token1->type==TT_OPERATOR)
        {
          if((token1->d.s!=SB_SUBTRACT)&&(token1->d.s!=SB_ADD))
            {die(EFATAL,"Line %d: Expression expected but operator found in function '%s'.",line,program.func->name);
             return first;}

          if(_nextc(c)) return first;

          token2=token1;
          if(!(token1=token_grab(c))){return first;}

          if(token2->d.s==SB_SUBTRACT)
          {
            token2->d.s=SB_NEGATE;
            token1->next=token2;
            token2->prev=token1;
            old=token2;
            prev=token2;
            token2=0;
          } else {
            old=token1;
            free(token2);
          }
        }
        first=token1;
        token1=0;
      } else {
        if(!token1) if(!(token1=token_grab(c))){return first;}

        if(token1->type!=TT_OPERATOR){
          die(EFATAL,"Line %d: Operator expected near '%s' in function '%s'.",line,s,program.func->name);
          return first;}

        if(token1->d.s==SB_ASSIGN)
        {
          if((old->type!=TT_VARIABLE)&&(old->type!=TT_VARIABLE_P))
            { die(EFATAL,"Line %d: Non-variable as lvalue in function '%s'.",line,program.func->name);
            return first;}
          old->noeval=1;
        }

        if(_nextc(c)) return first;

        if(!token2)if(!(token2=token_grab(c))){return first;}

        if((token2->type==TT_OPERATOR)&&(token2->d.s!=SB_NOT)&&
           (token1->d.s!=SB_SUBTRACT)&&(token1->d.s!=SB_ADD))
          {die(EFATAL,"Line %d: Expression expected but operator found near '%s' in function '%s'.",line,s,program.func->name);
          return first;}
        old=token2;

        if(token2->type==TT_OPERATOR)
        {
          if(_nextc(c)) return first;

          if(token2->d.s==SB_NOT)
          {
            if(!token3)if(!(token3=token_grab(c))){return first;}
            old=token3;
            if((token3->type==TT_OPERATOR)&&
               (token3->d.s!=SB_SUBTRACT)&&(token3->d.s!=SB_ADD))
              { die(EFATAL,"Line %d: Expression expected but operator found near '%s' in function '%s'. (missing ';' off previous line?)",
                line,s,program.func->name); return first;}

            if(token3->type==TT_OPERATOR)
            {
              if(_nextc(c)) return first;

              if((token3->type==TT_OPERATOR)&&(token3->d.s!=SB_SUBTRACT))
              {
                token4=token3;
                if(!(token3=token_grab(c))){return first;}
                token4->d.s=SB_NEGATE;
                old=token4;
              } else
              if((token3->type==TT_OPERATOR)&&(token3->d.s!=SB_ADD))
              {
                free(token3);
                if(!(token3=token_grab(c))){return first;}
                old=token3;
              }
            }

          } else /* plus or minus */

          if((token2->type==TT_OPERATOR)&&(token2->d.s!=SB_SUBTRACT))
          {
            token3=token2;
            if(!(token2=token_grab(c))){return first;}
            token3->d.s=SB_NEGATE;
            old=token3;
          } else
          if((token2->type==TT_OPERATOR)&&(token2->d.s!=SB_ADD))
          {
            free(token2);
            if(!(token2=token_grab(c))){return first;}
            old=token2;
          }

        }

        what=WHAT_AFTER;
        if((prev->type==TT_OPERATOR)&&(token1->type==TT_OPERATOR))
          if( (prev->d.s & LEV_MASK) > (token1->d.s & LEV_MASK) )
            what=WHAT_BEFORE;

        if(what==WHAT_BEFORE)
        {
          a=prev->prev;
          b=prev;
        } else {
          a=prev;
          b=0;
        }

        if(token4) {
          token3->next=b;
          a->next=token4;
          token4->prev=a;
          a=token4;
        }

        if(token3) {
          token3->next=b;
          a->next=token3;
          token3->prev=a;
          a=token3;
        }

        if(token3) {
          token3->next=b;
          a->next=token3;
          token3->prev=a;
          a=token3;
        }

        if(token2) {
          token2->next=b;
          a->next=token2;
          token2->prev=a;
          a=token2;
        }

        if(token1) {
          token1->next=b;
          a->next=token1;
          token1->prev=a;
        }

        if(what==WHAT_AFTER)prev=token1;

        token1=0;
        token2=0;
        token3=0;
        token4=0;
      }

      if(!*c){die(EFATAL,"Line %d: Unexpected end of file.",line);
        return first;}
    }
    if(_nextc(c)) return first;
  }
  die(EFATAL,"Line %d: Unexpected end of file.",line);
  return first;
}

/* #################################################### */
/* #################################################### */

STATE * pass_state(LCHAR ** c)
{
  STATE * state=state_new();
  char *s;
  int i;

  s=(*c)->s;

#ifdef DEBUG_BLOCK
  printf("pass_state: %s %d\n",s,state);
#endif

  if(*s=='{')
  {
    state->type=ST_BLOCK;
    state->d.block=pass_block(c);
    if(duck_error){state_free(state); return 0;}
    return state;
  }

  for (i=0;keywords[i];i+=2)
  if(!strcasecmp(s,keywords[i]))
    {
      state->type=ST_KEYWORD;
      state->d.keyword=pass_keyword(c,(long)keywords[i+1]);
      if(duck_error){state_free(state); return 0;}
      return state;
    }
  
  state->type=ST_EXPR;
  *c=(*c)->prev;
  state->d.expr=pass_expr(c,';');
  if(duck_error) {state_free(state);return 0;}
  return state;
}

/* #################################################### */
/* #################################################### */

STATE * pass_block(LCHAR ** c)
{
  STATE * first=0;
  STATE * prev=0;
  STATE * cur=0;
  long line=0;

  if(_nextc(c)) return first;

  while(*c)
  {
    line=(*c)->line;

    if (!strcmp((*c)->s,"my")) pass_my(c);
    else if(*((*c)->s)=='}')return first;
    else cur=pass_state(c);

    if(duck_error) return 0;

    if(cur)
    { 
      cur->prev=prev;
      if(prev)prev->next=cur;
      cur->next=0;
      prev=cur;
      if (!first)first=cur;
      cur=0;
    }
    if(_nextc(c)) return first;
  }
  die(EFATAL,"Line %d: Unexpected end of file.",line);
  return 0;
}

/* #################################################### */
/* #################################################### */

FUNCTION * pass_sub(LCHAR ** c)
{
  FUNCTION *func;
  long line;

  nextc(c);
  line=(*c)->line;

  func=function_new((*c)->s);
  program.func=func;
  func->script=program.script;

  if (!strcmp(func->name,"init")) program.script->init=func;
  if (!strcmp(func->name,"done")) program.script->done=func;

  nextc(c);
  line=(*c)->line;

  if(strcmp((*c)->s,"{"))
  {
     die(EFATAL,"Line %d: Block expected for function '%s'.",line,func->name);
     return 0;
  }

  func->block=pass_block(c);
    if(duck_error) return 0;

#ifdef DEBUG_FUNC
  printf("Function: %s @ %d\n",func->name,func->block);
#endif

  program.func=0;

  return func;
}

/* #################################################### */
/* #################################################### */

int first_pass()
{
  LCHAR * c;
  long line;

  program.func=0;

  for (c=lchar_first;c;c=c->next)
  {
    line=c->line;
    if (!strcmp(c->s,"my")){ if(!pass_my(&c)) return -1; }
    else if (!strcmp(c->s,"sub")){ if (!pass_sub(&c)) return -1; }
    else
    {
      die(EFATAL,"Line %d: Unknown global keyword: '%s'",line,c->s);
      return -1;
    }
    if(duck_error) return -1;
  }
  return 0;
}

/* #################################################### */
/* #################################################### */


