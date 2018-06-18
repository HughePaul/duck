#define _NEED_OPERS_
#include "main.h"

/* #################################################### */
/* #################################################### */

void run_operator(EXS * exs,long op)
{
  EXSITEM * r=0;
  EXSITEM * v1=0;
  char * temp;
  long lv1=0;
  long lv2=0;
  double nv1=0;
  double nv2=0;
  EXSITEM * v2=0;

  if((op & LEV_MASK)<LEV_6)
  {


      v1=exs_pop(exs);
      
      switch(v1->d.t)
      {
        case DT_LONG:   lv1=v1->d.d.i; break;
        case DT_NUMBER: nv1=v1->d.d.n; break;
        case DT_STRING: nv1=atof(v1->d.d.s.text); break;
        case DT_FILE:   lv1=v1->d.d.i; break;
      }


     if( (op & LEV_MASK) > LEV_1)
     {
       v2=exs_pop(exs);
      
       switch(v2->d.t)
       {
         case DT_LONG:   lv2=v2->d.d.i; break;
         case DT_NUMBER: nv2=v2->d.d.n; break;
         case DT_STRING: nv2=atof(v2->d.d.s.text); break;
         case DT_FILE:   lv2=v2->d.d.i; break;
       }

     }

     r=exs_push(exs);

    switch (op)
    {
    case SB_POW:
      if(lv1 && lv2) lv1=pow(lv2,lv1); else nv1=pow((lv2)?lv2:nv2,(lv1)?lv1:nv1);
    break;

    case SB_MULTIPLY:
      if(lv1 && lv2) lv1=lv1*lv2; else nv1=((lv1)?lv1:nv1) * ((lv2)?lv2:nv2);
    break;
    case SB_DIVIDE:
      if(!lv1 && !nv1) break;
      if(lv1 && lv2) lv1=lv2/lv1; else nv1=((lv2)?lv2:nv2) / ((lv1)?lv1:nv1);
    break;
    case SB_ADD:
/* one of them is an array */
     if( (v1->d.t==DT_ARRAY) || (v2->d.t==DT_ARRAY) )
     {
       array_cat(&r->d,&v2->d,&v1->d);
     }
/* one of them is a string */
     else if( (v1->d.t==DT_STRING) || (v2->d.t==DT_STRING) )
     {
       char tchar[255];tchar[0]=0;

       switch(v2->d.t)
       {
         case DT_NUMBER: snprintf(tchar,sizeof(tchar)-1,"%g",v2->d.d.n);break;
         case DT_FILE:
         case DT_LONG: snprintf(tchar,sizeof(tchar)-1,"%ld",v2->d.d.i);break;
         case DT_STRING: break;
         default:        data_clear(&v2->d);
       }

       if(tchar[0])string_make(&v2->d,tchar);
       tchar[0]=0;

       switch(v1->d.t)
       {
         case DT_NUMBER: snprintf(tchar,sizeof(tchar)-1,"%g",v1->d.d.n);break;
         case DT_FILE:
         case DT_LONG: snprintf(tchar,sizeof(tchar)-1,"%ld",v1->d.d.i);break;
         case DT_STRING: break;
         default:        data_clear(&v1->d);
       }

       if(tchar[0])string_make(&v1->d,tchar);

       temp=(char*)calloc(1,v1->d.d.s.length+v2->d.d.s.length+1);
       if(v2->d.t==DT_STRING) strcat(temp,v2->d.d.s.text);
       if(v1->d.t==DT_STRING) strcat(temp,v1->d.d.s.text);
       string_make(&r->d,temp);
       free(temp);
/* otherwise they are numbers */
     } else {
      if(lv1 && lv2) lv1=lv1+lv2; else nv1=((lv1)?lv1:nv1) + ((lv2)?lv2:nv2);
     }
    break;
    case SB_SUBTRACT:
      if(lv1 && lv2) lv1=lv2-lv1; else nv1=((lv2)?lv2:nv2) - ((lv1)?lv1:nv1);
    break;

    case SB_AND:
      if(lv1 && lv2) lv1= (lv2!=0) && (lv1!=0);
      else lv1=((lv2)?lv2:nv2!=0) && ((lv1)?lv1:nv1!=0);
      nv1=0;
    break;
    case SB_OR:
      if(lv1 && lv2) lv1=(lv2!=0) || (lv1!=0);
      else lv1=((lv2)?lv2:nv2!=0) || ((lv1)?lv1:nv1!=0);
      nv1=0;
    break;

    case SB_NOT:
      lv1 = (lv1==0); nv1 = (nv1==0);
    break;
    case SB_NEGATE:
      lv1 = -lv1; nv1 = -nv1;
    break;

    case SB_REMAIN:
      if(!lv1 && !nv1) break;
      if(!(lv1 && lv2)) {lv2=(long)((lv2)?lv2:nv2);lv1=(long)((lv1)?lv1:nv1); }
       lv1=lv2%lv1;nv1=0;
    break;
    case SB_INTDIV:
      if(!lv1 && !nv1) break;
      if(lv1 && lv2) lv1=lv2/lv1; else nv1=((lv2)?lv2:nv2) / ((lv1)?lv1:nv1);
      if(!lv1)lv1=(long)nv1;
      nv1=0;
    break;

    case SB_NE:
     if((v1->d.t==DT_NONE) || (v2->d.t==DT_NONE))
     {
       lv1=(!((v1->d.t==DT_NONE) && (v2->d.t==DT_NONE)));
     }
     else if((v1->d.t==DT_STRING) && (v2->d.t==DT_STRING))
     {
       lv1=strcmp(v1->d.d.s.text,v1->d.d.s.text);
     } else {
       lv1=(!( ((lv2)?lv2:nv2) == ((lv1)?lv1:nv1) ));
     }
     nv1=0;
    break;
    case SB_EQ:
     if((v1->d.t==DT_NONE) || (v2->d.t==DT_NONE))
     {
       lv1=((v1->d.t==DT_NONE) && (v2->d.t==DT_NONE));
     }
     else if((v1->d.t==DT_STRING) && (v2->d.t==DT_STRING))
     {
       lv1=!strcasecmp(v2->d.d.s.text,v1->d.d.s.text);
     } else {
       lv1=(((lv2)?lv2:nv2) == ((lv1)?lv1:nv1));
     }
     nv1=0;
    break;
    case SB_EQ_C:
     if((v1->d.t==DT_NONE) || (v2->d.t==DT_NONE))
     {
       lv1=((v1->d.t==DT_NONE) && (v2->d.t==DT_NONE));
     }
     else if((v1->d.t==DT_STRING) && (v2->d.t==DT_STRING))
     {
       lv1=!strcmp(v2->d.d.s.text,v1->d.d.s.text);
     } else {
       lv1=(((lv2)?lv2:nv2) == ((lv1)?lv1:nv1));
     }
     nv1=0;
    break;

    case SB_LT:
     if((v1->d.t==DT_STRING) && (v2->d.t==DT_STRING))
     {
       lv1=(strcmp(v2->d.d.s.text,v1->d.d.s.text) < 0);
     } else {
       lv1=(((lv2)?lv2:nv2) < ((lv1)?lv1:nv1));
     }
     nv1=0;
    break;
    case SB_GT:
     if((v1->d.t==DT_STRING) && (v2->d.t==DT_STRING))
     {
       lv1=(strcmp(v2->d.d.s.text,v1->d.d.s.text) > 0);
     } else {
       lv1=(((lv2)?lv2:nv2) > ((lv1)?lv1:nv1));
     }
     nv1=0;
    break;
    case SB_LE:
     if((v1->d.t==DT_STRING) && (v2->d.t==DT_STRING))
     {
       lv1=(strcmp(v2->d.d.s.text,v1->d.d.s.text) <= 0);
     } else {
       lv1=(((lv2)?lv2:nv2) <= ((lv1)?lv1:nv1));
     }
     nv1=0;
    break;
    case SB_GE:
     if((v1->d.t==DT_STRING) && (v2->d.t==DT_STRING))
     {
       lv1=(strcmp(v2->d.d.s.text,v1->d.d.s.text) >= 0);
     } else {
       lv1=(((lv2)?lv2:nv2) >= ((lv1)?lv1:nv1));
     }
     nv1=0;
    break;



    }

    if (!r->d.t) number_make(&r->d,0,nv1,lv1);

    exsitem_free(v1);
    exsitem_free(v2);
  }
}

/* #################################################### */
/* #################################################### */

DATA * run_expr(DATA * retval, TOKEN * t, int leave_as_array)
{
  DATA data;
  DATA * dd;

  int i;

  EXS exs;
  EXSITEM * dest=0;

  DATA * myvar=0;
  DATA mysubs;

  mysubs.t=DT_NONE;

  data_clear(retval);

/* blank static data records */
  memset(&mysubs,0,sizeof(DATA));
  mysubs.t=DT_NONE;
  memset(&data,0,sizeof(DATA));
  data.t=DT_NONE;

  if (!t)return 0;

  exs.top=0;
  exs.count=0;

  for(;t;t=t->next)
  {

    if(t->type!=TT_OPERATOR)
    {
     dest=exs_push(&exs);

     switch (t->type)
     {
      case TT_VALUE:
        data_dup(&dest->d,&t->d.v);
      break;
      case TT_FUNCTION:
        if(!t->d.f) t->d.f=function_get(t->name);
        if(!t->d.f) die(EWARN,"Unknown function '%s' in function '%s'",
          t->name, program.stack->func->name);
        else
        {
          run_expr(&data,t->e,1);
          run_func(t->d.f,&dest->d,&data);
        }
        t->d.f=0; /* safe for unloads */
      break;
      case TT_INTFUNC:
         run_expr(&data,t->e,0);
         if(t->d.s) (*(void(*)(DATA*,DATA*)) t->d.s ) (&dest->d,&data);
      break;
      case TT_EXPRESSION:
        run_expr(&dest->d,t->e,0);
      break;
      case TT_VARIABLE:
        if(!t->noeval)
          array_get(&dest->d,stack_var(t->d.s),run_expr(&data,t->e,0));
        else
        {
          exs_pop(&exs);
          exsitem_free(dest);
          myvar=stack_var(t->d.s);
          run_expr(&mysubs,t->e,0);
        }
      break;
      case TT_VARIABLE_P:
        { 
          DATA sub; sub.t=DT_NONE;
          array_get(&data,stack_var(t->d.s),run_expr(&sub,t->e,0));
          data_clear(&sub);
        }
        if(data.t!=DT_POINTER)
        {
          die(EWARN,"Variable is not a pointer in function '%s'.",program.stack->func->name);
          break;
        }
        if(!t->noeval)
          data_dup(&dest->d,data.d.p);
        else
        {
          exs_pop(&exs);
          exsitem_free(dest);
          myvar=data.d.p;
        }
      break;
      case TT_VARIABLE_A:
        if(t->e) pointer_make(&dest->d,array_get_index(stack_var(t->d.s),
          run_expr(&data,t->e,0),0));
        else pointer_make(&dest->d,stack_var(t->d.s));
      break;
      case TT_VARIABLE_L:
        if(t->e)
          dd=array_get_index(stack_var(t->d.s),run_expr(&data,t->e,0),0);
        else
          dd=stack_var(t->d.s);
        switch(dd->t)
        {
          case DT_ARRAY:
            number_make(&dest->d,0,0,dd->d.a.length);
          break;
          case DT_STRING:
            number_make(&dest->d,0,0,dd->d.s.length);
          break;
          case DT_NONE:
            number_make(&dest->d,0,0,0);
          break;
          default:
            number_make(&dest->d,0,0,1);
        }
      break;
      default:
       printf("\nUnknown type in expression: %ld\n",t->type);
       exsitem_free(exs_pop(&exs));
      break;
     }
    } else run_operator(&exs,t->d.s);

    data_clear(&data);
  }

/* work out return data */
  if((myvar) || (retval))
  {
    if((exs.count==1)&&(!leave_as_array))
 /* make single value from stack top or we want an array */
    {
      data_dup(&data,&exs.top->d);
    }
    if((exs.count>1)||(leave_as_array)) /* make array out of stack */
    {
      array_make(&data);
      data.d.a.length=exs.count;
      data.d.a.array=(DATA**)calloc(exs.count,sizeof(DATA*));
      for (i=exs.count-1,dest=exs.top;(i>=0)&&(dest);i--,dest=dest->prev)
      {
        data.d.a.array[i]=data_new();
        data_dup(data.d.a.array[i],&dest->d);
      }
    }

    if(retval)data_dup(retval,&data);
    if(myvar)array_set(myvar,&data,(mysubs.t!=DT_NONE)?&mysubs:0);

    data_clear(&data);
  }

  exs_clear(&exs);
  return retval;
}

/* #################################################### */
/* #################################################### */
/* expression stack */


EXSITEM * exs_push(EXS * s)
{
  EXSITEM * e;
  if(!s) die(EFATAL,"BUG: Tried to push onto non-existant stack.");
  e=(EXSITEM*)calloc(1,sizeof(EXSITEM));
  e->prev=s->top;
  s->top=e;
  s->count++;
  return e;
}

EXSITEM * exs_pop(EXS * s)
{
  EXSITEM * e;
  if(!s) die(EFATAL,"BUG: Tried to pop from non-existant stack.");
  e=s->top;
  if(!e) return e;
  s->top=e->prev;
  s->count--;
  return e;
}

void exsitem_free(EXSITEM * e)
{
  if(!e)return;
  data_clear(&e->d);
  free (e);
}

void exs_clear(EXS * s)
{
  EXSITEM * e;
  EXSITEM * p;
  if(!s)return;
  for(e=s->top;e;e=p)
  {
    p=e->prev;
    exsitem_free(e);
  }
  s->top=0;
  s->count=0;
}

/* #################################################### */
/* #################################################### */

