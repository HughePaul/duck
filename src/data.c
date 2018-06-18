#include "main.h"

/* ###################################################### */
/* #################################################### */
/* Data types */

/* With these free commands the actual pointer isn't freed,
** just its contents
*/

void data_clear(DATA * data)
{
  if (!data) return;
  if (!data->t) return;
  switch(data->t)
  {
    case DT_STRING:
      free(data->d.s.text);
    break;
    case DT_ARRAY:
     array_free(&data->d.a);
    break;
    case DT_POINTER:
      pointer_unreg(data);
    break;
  }
  memset(data,0,sizeof(DATA));
  data->t=DT_NONE;
}

void data_free(DATA * data)
{
  if (!data) return;
  pointer_check(data); /* need to do this for non freed mem too */
  data_clear(data);
  free(data);
}

DATA * data_new()
{  return (DATA*) calloc(1,sizeof(DATA));  }

DATA * data_dup(DATA * dest,DATA * src)
{

  if(!dest) return 0;

  data_clear(dest);

  if(!src) return dest;

  dest->t=src->t;

  switch(src->t)
  {
    case DT_STRING:
      string_dup(&dest->d.s,&src->d.s);
    break;
    case DT_ARRAY:
      array_dup(&dest->d.a,&src->d.a);
    break;
    case DT_NUMBER:
      dest->d.n=src->d.n;
    break;
    case DT_LONG:
    case DT_FILE:
      dest->d.i=src->d.i;
    break;
    case DT_POINTER:
      dest->d.p=src->d.p;
      pointer_reg(dest);
    break;
  }
  return dest;
}

/* ###################################################### */
/* ###################################################### */
/* File */

void file_make(DATA * dest, long fd)
{
  if (!dest)return;
  data_clear(dest);
  dest->t=DT_FILE;
  dest->d.i=fd;
}


/* ###################################################### */
/* ###################################################### */
/* Number */


void number_make(DATA * dest, char*n, double number, long integer)
{
  if (!dest)return;
  data_clear(dest);

  dest->t=DT_NUMBER;
  if(n)
  {
    if (strchr(n,'.')) dest->d.n=atof(n);
    else
    {
      dest->t=DT_LONG;
      dest->d.i=atol(n);
    }
  }
  else if(number) dest->d.n=number;
  else
  {
    dest->t=DT_LONG;
    dest->d.i=integer;
  }
}



/* ###################################################### */
/* ###################################################### */
/* Pointer */

void pointer_make(DATA * dest, DATA * data)
{
  if (!dest)return;
  data_clear(dest);
  dest->t=DT_POINTER;
  if(data)
  {
    dest->d.p=data;
    pointer_reg(dest);
  } else dest->d.p=0;
}

void pointer_reg(DATA * dest)
{
  POINTREG * p;

/* register pointer */
  if (!dest)return;
  if (!dest->d.p)return;
  if (dest->d.p==(DATA*)-1)return;

  p=calloc(1,sizeof(POINTREG));
  p->pointer=dest;
  p->next=dest->d.p->pointers;
  dest->d.p->pointers=p;
}

DATA * pointer_get(DATA * data)
{
  if(!data) return 0;
  if(data->t!=DT_POINTER)
  {
    die(EWARN,"Variable is not a pointer.");
    return 0;
  }
  if (data->d.p==(DATA*)-1)
  {
    die(EWARN,"Pointer points to an expired variable.");
    return 0;
  }
  return data->d.p;
}

void pointer_unreg(DATA * pointer)
{
  POINTREG * p;
  POINTREG * last;

  if (!pointer)return;
  if (!pointer->d.p)return;
  if (pointer->d.p==(DATA*)-1)return;

  for(p=pointer->d.p->pointers,last=0;p;last=p,p=p->next)
    if (pointer==p->pointer)
    {
      if(last)last->next=p->next;
         else pointer->d.p->pointers=p->next;
      free(p);
      return;
    }
  die(EWARN,"BUG: Registered pointer not in pointer list");
}

void pointer_check(DATA * data)
{
  POINTREG * p;
  POINTREG * next;

  if (!data)return;

  for(p=data->pointers;p;p=next)
  {
    next=p->next;
    if(p->pointer)
      p->pointer->d.p=(DATA*)-1;
    else
      die(EWARN,"BUG: Pointer in list is not registered.");
    free(p);
  }
  data->pointers=0;
}

/* ###################################################### */
/* ###################################################### */
/* String */

char*mystrdup(char*s,long len)
{
  char * d;
  char *p;
  int i;

  if (!s) return 0;
  d=calloc(1,len+1);
  for(i=0,p=d;i<len;p++,s++,i++)*p=*s;
  *p=0;
  return d;
}

void string_dup(STRING * dest,STRING * src)
{
  if (!dest)return;
  free(dest->text);
  dest->text=0;
  if (!src)
  {
    dest->length=0;
    dest->text=0;
  } else {
    dest->text=mystrdup(src->text,src->length);
    dest->length=src->length;
  }
}

void string_make(DATA * dest, char * s)
{
  if (!dest)return;
  data_clear(dest);
  dest->t=DT_STRING;
  if (!s)
  {
    dest->d.s.length=0;
    dest->d.s.text=0;
  } else {
    dest->d.s.text=strdup(s);
    dest->d.s.length=strlen(s);
  }
}

/* ###################################################### */
/* ###################################################### */
/* Array */

void array_make(DATA * dest)
{
  if (!dest)return;
  data_clear(dest);
  dest->t=DT_ARRAY;
  dest->d.a.array=0;
  dest->d.a.length=0;
}

void array_resize(DATA * dest, long space)
{
  long d;

  if (!dest)return;

/* if array doesn't exist create one */
  if (dest->t!=DT_ARRAY) array_make(dest);

/* if array size doesn't change then return */
  if (space==dest->d.a.length) return;

/* clear up data records if space is less than before */
  if(space < dest->d.a.length)
  {
    for(d=space;d<dest->d.a.length;d++)
      data_free(dest->d.a.array[d]);
  }

/* resize array data */
  dest->d.a.array=realloc(dest->d.a.array,space * sizeof(DATA*));

/* if space is more than make data */
  if(space > dest->d.a.length)
  {
    for(d=dest->d.a.length;d<space;d++) /* wastes memory for emptys? */
      dest->d.a.array[d]=data_new();
  }

/* record new length */
  dest->d.a.length=space;
}

DATA * array_cat(DATA * dest, DATA * a1, DATA * a2)
{
  long length=0;
  long i=0;
  if (!dest) return 0;
  data_clear(dest);
  if (!a1) return data_dup(dest,a2);  
  if (!a2) return data_dup(dest,a1);
  if (a1->t==DT_ARRAY) length+=a1->d.a.length; else length++;
  if (a2->t==DT_ARRAY) length+=a2->d.a.length; else length++;
  array_resize(dest,length);
  length=0;

  if (a1->t!=DT_ARRAY)
    data_dup(array_get_index(dest,0,length++),a1);
  else
    for(i=0;i<a1->d.a.length;i++,length++)
      data_dup(array_get_index(dest,0,length), array_get_index(a1,0,i));

  if (a2->t!=DT_ARRAY)
    data_dup(array_get_index(dest,0,length++),a2);
  else
    for(i=0;i<a2->d.a.length;i++,length++)
      data_dup(array_get_index(dest,0,length), array_get_index(a2,0,i));

  return dest;
}

void array_set(DATA * a, DATA * data, DATA * subs)
{
  long d,s;
  long start=0;
  long end=0;

  if(!a) return;
  if(subs)
  {
    start=data2int(array_get_index(subs,0,0));
    end=data2int(array_get_index(subs,0,1));
  }else{
    data_dup(a,data);
    return;
  }

  if(a->t!=DT_ARRAY) array_make(a);

  if (start<0) 
    {die(EWARN,"Array start subscript less than 0."); start=0;}
  if (end<start) end=start;

/* make more memory */
  if (end >= a->d.a.length)
    array_resize(a,end+1);

/* set data */

/* copy a section of an array starting at the begining and ending after
** ( <start> - <end> ) to a section of an array from <start> to <end> */

  if (data->t == DT_ARRAY)
  {
    for(d=start,s=0;d<=end;d++,s++)
      data_dup(a->d.a.array[d], array_get_index(data,0,s));
  }

/* Fill an array from <start> to <end> with the data given */

  else {
    for(d=start;d<=end;d++)
      data_dup(a->d.a.array[d],data);
  }
}

DATA * array_get(DATA * a, DATA * data, DATA * subs)
{
  long d,s;
  long start=0;
  long end=0;
  char * temp;
  char * ps;
  long len;
  long p;

  if(!a) return 0;

  data_clear(a);

  if(!data) return a;

/* Set <a> with a section of <data> given, the range of which is given by
** <subs>. */

  if(subs)
  {
    start=data2int(array_get_index(subs,0,0));
    end=data2int(array_get_index(subs,0,1));
  }

/* copy <data> to <a> as no <subs> is given. */

  else
  {
    data_dup(a,data);
    return a;
  }

  if (end<start) end=start;

      if (start<0) 
        {die(EWARN,"Array start subscript less than 0."); start=0;}

  switch(data->t)
  {

  case DT_ARRAY:
    if (end-start)
    {
      array_make(a);
      array_resize(a,end-start+1);
      for(d=start,s=0;d<=end;d++,s++)
      {
        a->d.a.array[s]=data_new();
        data_dup(a->d.a.array[s],
          array_get_index(data,0,d));
      }
    } else
        data_dup(a,array_get_index(data,0,start));
    break;
  case DT_STRING:
    if(end >= data->d.s.length) end=data->d.s.length-1;
    if(start >= data->d.s.length) start=data->d.s.length-1;

    if (start<0)start=0;
    if (end<0)end=0;

    len=end-start+1;

    temp=(char*)calloc(1,len+10);
    for(p=0,ps=data->d.s.text+start; p<len; p++,ps++)
      temp[p]=*ps;

    string_make(a,temp);
    free(temp);

    break;
  default:
    if(start==0)data_dup(a,data);
    else data_clear(a);
    break;
  }
  return a;
}

DATA * array_get_index(DATA * a, DATA * subs, long index)
{
  if(!a)return 0;
  if(subs)
    index=data2int(subs);
  if (a->t!=DT_ARRAY)
  {
    if(index==0)return a; else return 0;
  }
  if ( (index<0) || (index>=a->d.a.length) ) return 0;
  return a->d.a.array[index];
}

void array_free(ARRAY * a)
{
  long i=0;
  if (!a)return;
  for(i=0;i<a->length;i++)
    data_free(a->array[i]);
  a->length=0;
  free(a->array);
  a->array=0;
}

void array_dup(ARRAY * dest,ARRAY * src)
{
  long i=0;

  if (!dest)return;
  array_free(dest);
  if (!src)return;
  dest->length=src->length;
  dest->array=calloc(src->length,sizeof(DATA*));
  for(i=0;i<src->length;i++)
  {
    dest->array[i]=data_new();
    data_dup(dest->array[i],src->array[i]);
  }
}


/* ###################################################### */
/* ###################################################### */

long data2int(DATA * data)
{
  if (!data) return 0;
  switch(data->t)
  {
    case DT_STRING:
      return data->d.s.length;
    break;
    case DT_NUMBER:
      return ceil(data->d.n);
    break;
    case DT_FILE:
    case DT_LONG:
      return data->d.i;
    break;
    case DT_ARRAY:
      if (!data->d.a.length) return 0;
      return data2int(data->d.a.array[0]);
    break;
    case DT_POINTER:
      return (long)data->d.p;
    break;
  }
  return 0;
}

#define MAX_NESTED_ARRAY_FORMATS	10

DATA * array_format(DATA * dest, char * format, ... )
{
  va_list va;
  long i[MAX_NESTED_ARRAY_FORMATS];
  DATA * d[MAX_NESTED_ARRAY_FORMATS];
  long b=0;

  if(!dest) return 0;
  if(!format) return 0;

  d[0]=dest;
  i[0]=0;

  data_clear(dest);
  array_resize(dest,0);

  va_start(va,format);

  while (*format)
  {
    switch (*(format++))
    {
      case ')':
        b--;
        if(b<0)
        {
          die(EDROP,"Mismatched brackets in array_format.");
          va_end(va); return dest;
        }
      case ',':
        break;
      case '0':
        array_resize(d[b],i[b]+1);
        data_clear(d[b]->d.a.array[i[b]]);
        break;
      case 's':
        array_resize(d[b],i[b]+1);
        string_make(d[b]->d.a.array[i[b]],va_arg(va, char *));
        i[b]++;
        break;
      case 'l':
        array_resize(d[b],i[b]+1);
        number_make(d[b]->d.a.array[i[b]],0,0,va_arg(va, long));
        i[b]++;
        break;
      case 'n':
        array_resize(d[b],i[b]+1);
        number_make(d[b]->d.a.array[i[b]],0,va_arg(va, double),0);
        i[b]++;
        break;
      case 'f':
        array_resize(d[b],i[b]+1);
        file_make(d[b]->d.a.array[i[b]],va_arg(va, long));
        i[b]++;
        break;
      case 'd':
        array_resize(d[b],i[b]+1);
        data_dup(d[b]->d.a.array[i[b]],va_arg(va, DATA*));
        i[b]++;
       break;
      case '(':
        array_resize(d[b],i[b]+1);
        b++;
        if (b>=MAX_NESTED_ARRAY_FORMATS)
        {
          die(EDROP,"Too many nested brackets in array_format.");
          va_end(va); return dest;
        }
        d[b]=d[b-1]->d.a.array[i[b-1]];
        i[b-1]++;
        i[b]=0;
        break;
      default: die(EDROP,"Invalid char in array format."); break;
    }
  }
  va_end(va);
  return dest;
}
