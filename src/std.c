#define _NEED_STD_
#include "main.h"
#include <unistd.h>
#include <math.h>
#include <ctype.h>
#include <fcntl.h>

void set_custom(char ** constants, char ** functions)
{
  if (constants)custom_constants=constants;
  if (functions)custom_functions=functions;
}

/* ###################################################### */
/* ###################################################### */


/* math */
void std_func_log(DATA * dest, DATA * data)
{
  DATA num;
  num.t=DT_NONE;
  if(!data) return;
  std_func_val(&num,data);
  switch(num.t)
  {
  case DT_NUMBER:
    number_make(dest,0,log(num.d.n),0);
  break;
  case DT_LONG:
    number_make(dest,0,log(num.d.i),0);
  break;
  }
  data_clear(&num);
}

void std_func_log10(DATA * dest, DATA * data)
{
  DATA num;
  num.t=DT_NONE;
  if(!data) return;
  std_func_val(&num,data);
  switch(num.t)
  {
  case DT_NUMBER:
    number_make(dest,0,log10(num.d.n),0);
  break;
  case DT_LONG:
    number_make(dest,0,log10(num.d.i),0);
  break;
  }
  data_clear(&num);
}

void std_func_sin(DATA * dest, DATA * data)
{
  DATA num;
  num.t=DT_NONE;
  if(!data) return;
  std_func_val(&num,data);
  switch(num.t)
  {
  case DT_NUMBER:
    number_make(dest,0,sin(num.d.n),0);
  break;
  case DT_LONG:
    number_make(dest,0,sin(num.d.i),0);
  break;
  }
  data_clear(&num);
}

void std_func_cos(DATA * dest, DATA * data)
{
  DATA num;
  num.t=DT_NONE;
  if(!data) return;
  std_func_val(&num,data);
  switch(num.t)
  {
  case DT_NUMBER:
    number_make(dest,0,cos(num.d.n),0);
  break;
  case DT_LONG:
    number_make(dest,0,cos(num.d.i),0);
  break;
  }
  data_clear(&num);
}

void std_func_tan(DATA * dest, DATA * data)
{
  DATA num;
  num.t=DT_NONE;
  if(!data) return;
  std_func_val(&num,data);
  switch(num.t)
  {
  case DT_NUMBER:
    number_make(dest,0,tan(num.d.n),0);
  break;
  case DT_LONG:
    number_make(dest,0,tan(num.d.i),0);
  break;
  }
  data_clear(&num);
}

void std_func_asin(DATA * dest, DATA * data)
{
  DATA num;
  num.t=DT_NONE;
  if(!data) return;
  std_func_val(&num,data);
  switch(num.t)
  {
  case DT_NUMBER:
    number_make(dest,0,asin(num.d.n),0);
  break;
  case DT_LONG:
    number_make(dest,0,asin(num.d.i),0);
  break;
  }
  data_clear(&num);
}

void std_func_acos(DATA * dest, DATA * data)
{
  DATA num;
  num.t=DT_NONE;
  if(!data) return;
  std_func_val(&num,data);
  switch(num.t)
  {
  case DT_NUMBER:
    number_make(dest,0,acos(num.d.n),0);
  break;
  case DT_LONG:
    number_make(dest,0,acos(num.d.i),0);
  break;
  }
  data_clear(&num);
}

void std_func_atan(DATA * dest, DATA * data)
{
  DATA num;
  num.t=DT_NONE;
  if(!data) return;
  std_func_val(&num,data);
  switch(num.t)
  {
  case DT_NUMBER:
    number_make(dest,0,atan(num.d.n),0);
  break;
  case DT_LONG:
    number_make(dest,0,atan(num.d.i),0);
  break;
  }
  data_clear(&num);
}

void std_func_exp(DATA * dest, DATA * data)
{
  DATA num;
  num.t=DT_NONE;
  if(!data) return;
  std_func_val(&num,data);
  switch(num.t)
  {
  case DT_NUMBER:
    number_make(dest,0,exp(num.d.n),0);
  break;
  case DT_LONG:
    number_make(dest,0,exp(num.d.i),0);
  break;
  }
  data_clear(&num);
}

void std_func_sqrt(DATA * dest, DATA * data)
{
  DATA num;
  num.t=DT_NONE;
  if(!data) return;
  std_func_val(&num,data);
  switch(num.t)
  {
  case DT_NUMBER:
    number_make(dest,0,sqrt(num.d.n),0);
  break;
  case DT_LONG:
    number_make(dest,0,sqrt(num.d.i),0);
  break;
  }
  data_clear(&num);
}

void std_func_floor(DATA * dest, DATA * data)
{
  DATA num;
  num.t=DT_NONE;
  if(!data) return;
  std_func_val(&num,data);
  switch(num.t)
  {
  case DT_NUMBER:
    number_make(dest,0,0,floor(num.d.n));
  break;
  case DT_LONG:
    number_make(dest,0,0,floor(num.d.i));
  break;
  }
  data_clear(&num);
}

void std_func_ceil(DATA * dest, DATA * data)
{
  DATA num;
  num.t=DT_NONE;
  if(!data) return;
  std_func_val(&num,data);
  switch(num.t)
  {
  case DT_NUMBER:
    number_make(dest,0,0,ceil(num.d.n));
  break;
  case DT_LONG:
    number_make(dest,0,0,ceil(num.d.i));
  break;
  }
  data_clear(&num);
}

void std_func_abs(DATA * dest, DATA * data)
{
  DATA num;
  num.t=DT_NONE;
  if(!data) return;
  std_func_val(&num,data);
  switch(num.t)
  {
  case DT_NUMBER:
    number_make(dest,0,fabs(num.d.n),0);
  break;
  case DT_LONG:
    number_make(dest,0,0,labs(num.d.i));
  break;
  }
  data_clear(&num);
}


/* utilitys */
void std_func_type(DATA * dest, DATA * data)
{
  if (!data) return;
  if(data->t)number_make(dest,0,0,data->t);
}

void std_func_getenv(DATA * dest, DATA * data)
{
  if (!data) return;
  if (data->t==DT_STRING)
  {
    string_make(dest,getenv(data->d.s.text));
  }
}


/* ###################################################### */
/* ###################################################### */

/* cast */
void std_func_int(DATA * dest, DATA * data)
{
  double d;
  long l;

  if(!data) return;
  switch(data->t)
  {
  case DT_STRING:
    data->d.n=atof(data->d.s.text);
  case DT_NUMBER:
    d=data->d.n;
    l=ceil(data->d.n);
    if((l-d)>0.5)l=floor(d);
    number_make(dest,0,0,l); break;
  case DT_ARRAY:
  case DT_NONE:
    number_make(dest,0,0,0); break;
  default:
    data_dup(dest,data);
    dest->t=DT_LONG;
    break;
  }
  
}

void std_func_val(DATA * dest, DATA * data)
{
  if (!data) return;
  switch(data->t)
  {
  case DT_STRING:
    number_make(dest,data->d.s.text,0,0); break;
  case DT_ARRAY:
  case DT_NONE:
    number_make(dest,0,0,0); break;
  case DT_FILE:
  case DT_POINTER:
    dest->t=DT_LONG;
  default:
    data_dup(dest,data); break;
  }
}

void std_func_str(DATA * dest, DATA * data)
{
  static char buf[PRINT_BUF_SIZE];
  if(!data)return;

  buf[0]=0;

  switch(data->t)
  {
    case DT_NONE: break;
    case DT_STRING:
      string_make(dest,data->d.s.text);
      return; break;
    case DT_NUMBER:
      snprintf(buf,PRINT_BUF_SIZE,"%g",data->d.n); break;
    case DT_LONG:
    case DT_FILE:
      snprintf(buf,PRINT_BUF_SIZE,"%ld",data->d.i); break;
    case DT_POINTER:
      snprintf(buf,PRINT_BUF_SIZE,"%ld",(long)data->d.p); break;
    case DT_ARRAY:
      if(data->d.a.length)
        { std_func_str(dest,data->d.a.array[0]); return; }
    default:
      snprintf(buf,PRINT_BUF_SIZE,">>UNKNOWN<<");
  }

  string_make(dest,buf);

}

void std_func_array(DATA * dest, DATA * data)
{

  if(!data)
  {
    array_resize(dest,0);
    return;
  }

  if(data->t!=DT_ARRAY)
  {
    array_resize(dest,1);
    data_dup(array_get_index(dest,0,0),data);
  }
  
}


/* ###################################################### */
/* ###################################################### */

/* arrays and strings */
void std_func_split(DATA * dest, DATA * data)
{
  DATA split;
  DATA string;
  char * p;
  char * e;
  long splitlen=0;
  long n=0;

  split.t=DT_NONE;
  string.t=DT_NONE;

  if (!data) return;
  if (data->t!=DT_ARRAY) return;
  if (data->d.a.length<2) return;

  std_func_str(&split,array_get_index(data,0,0));
  std_func_str(&string,array_get_index(data,0,1));

  array_resize(dest,0);

  if(split.t==DT_STRING)
  {
    splitlen=strlen(split.d.s.text);
    p=string.d.s.text;
    if (splitlen)
    {
      while((e=strstr(p,split.d.s.text)))
      {
        array_resize(dest,n+1);
        *e=0;
        string_make(array_get_index(dest,0,n++),p);
        p=e+splitlen;
      }
      array_resize(dest,n+1);
      string_make(array_get_index(dest,0,n),p);
    }
    else
    {
      char t[2];
      t[1]=0;
      while(*p)
      {
        array_resize(dest,n+1);
        t[0]=*(p++);
        string_make(array_get_index(dest,0,n++),t);
      }
    }
  }

  data_clear(&split);
  data_clear(&string);
}

void std_func_join(DATA * dest, DATA * data)
{
  DATA join;
  DATA string;
  DATA * array;
  char * temp;
  char * p;
  long len=0;
  long i;

  join.t=DT_NONE;
  string.t=DT_NONE;

  if(!data)return;

  if (data->t!=DT_ARRAY) return;
  if (data->d.a.length<2) return;

  if(!(array=array_get_index(data,0,1))) return;
  std_func_str(&join,array_get_index(data,0,0));

  switch(array->t)
  {
  case DT_STRING:
    len=(join.d.s.length*(array->d.s.length-1))+array->d.s.length;
    p=temp=(char*)calloc(1,len+1);
    *(p++)=array->d.s.text[0];
    for (i=1;i<array->d.s.length;i++)
    {
      strcpy(p,join.d.s.text);
      p+=join.d.s.length;
      *(p++)=array->d.s.text[i];
    }
    *p=0;
    string_make(dest,temp);
    free(temp);
    break;
  case DT_ARRAY:
    len=1024;
    p=temp=(char*)calloc(1,len);
    std_func_str(&string,array_get_index(array,0,0));
    strcpy(p,string.d.s.text);
    p+=string.d.s.length;
    for (i=1;i<array->d.a.length;i++)
    {
      strcpy(p,join.d.s.text);
      p+=join.d.s.length;
      data_clear(&string);
      std_func_str(&string,array_get_index(array,0,i));
      strcpy(p,string.d.s.text);
      p+=string.d.s.length;
    }
    *p=0;
    string_make(dest,temp);
    free(temp);
    break;
  default:
    data_dup(dest,array); break;
  }

  data_clear(&string);
  data_clear(&join);
}

void std_func_strpos(DATA * dest, DATA * data)
{
  DATA haystack;
  DATA needle;
  char * p;

  number_make(dest,0,0,-1);

  if(!data)return;

  haystack.t=DT_NONE;
  needle.t=DT_NONE;

  std_func_str(&haystack,array_get_index(data,0,0));
  std_func_str(&needle,array_get_index(data,0,1));

  p=strstr(haystack.d.s.text,needle.d.s.text);

  number_make(dest,0,0,(p)?p - haystack.d.s.text:-1);

  data_clear(&needle);
  data_clear(&haystack);
}

void std_func_ucase(DATA * dest, DATA * data)
{
  DATA str;
  long i;

  str.t=DT_NONE;
  if(!data)return;

  std_func_str(&str,data);
  for(i=0;i<str.d.s.length;i++)
    str.d.s.text[i]=toupper(str.d.s.text[i]);

  string_make(dest,str.d.s.text);
  data_clear(&str);
}

void std_func_lcase(DATA * dest, DATA * data)
{
  DATA str;
  long i;

  str.t=DT_NONE;
  if(!data)return;

  std_func_str(&str,data);
  for(i=0;i<str.d.s.length;i++)
    str.d.s.text[i]=tolower(str.d.s.text[i]);

  string_make(dest,str.d.s.text);
  data_clear(&str);

}

void std_func_chomp(DATA * dest, DATA * data)
{
  DATA str;
  char* p;

  str.t=DT_NONE;
  if(!data)return;

  std_func_str(&str,data);
  for(p=str.d.s.text+str.d.s.length-1;
   (p>=str.d.s.text)&&( (*p=='\n') || (*p=='\r') );p--)
    {*p=0; str.d.s.length--; }

  data_dup(dest,&str);

  data_clear(&str);
}

/* ###################################################### */
/* ###################################################### */

/* files and net connections */

int std_int_fds[STD_INT_MAX_FDS];
int std_int_fd_flags[STD_INT_MAX_FDS];
long std_int_fd_count;

int std_int_fd_allowed(long realfd)
{
#ifdef LIMIT_NO_STDIN
  if (realfd==0) return 0;
#endif
#ifdef LIMIT_NO_STDOUT
  if (realfd==1) return 0;
#endif
#ifdef LIMIT_NO_STDERR
  if (realfd==2) return 0;
#endif
  if (realfd==-1) return 0;
  return 1;
}

int std_int_get_fd(long fakefd)
{
  if(fakefd>=std_int_fd_count) return -1;
  if(fakefd<1) return -1;
  return std_int_fds[fakefd];
}

long std_int_set_fd(int realfd)
{
  long i;

  if (realfd==-1)return -1;

  for (i=4;i<std_int_fd_count;i++)
    if (std_int_fds[i]==-1) break; /* found a free slot */

  if(i>=std_int_fd_count)
  {
    if (std_int_fd_count >= STD_INT_MAX_FDS) return 0;
    i=std_int_fd_count;
    std_int_fd_count++;
  }

  std_int_fds[i]=realfd;
  std_int_fd_flags[i]=0;

  return i;
}

int std_int_free_fd(long fakefd)
{
  if(fakefd>=std_int_fd_count) return -1;
  if(fakefd<1) return -1;
  if(fakefd<4) return 0;

  std_int_fds[fakefd]=-1;

  if (fakefd == (std_int_fd_count-1)) std_int_fd_count--;

  return 0;
}

long std_int_readln(int fd, char * buf)
{
  long length=0;
  char c;

  if(!buf) return -1;
  
  while(1)
  {
    if(!read(fd,&c,1))
    { /* eof */
      buf=0;
      return length;
    } else
    switch(c)
    {
    case '\r': break;
    case '\n': *(buf++)=c; buf=0; return ++length;
    default:
     length++;
     *(buf++)=c;
    }
    if(length>=STD_INT_MAX_BUF) return length;

  }
}

int std_int_write(int f, DATA * d, long * size)
{
  static char buf[PRINT_BUF_SIZE+1];
  int len=0;
  int i;
  if(!d)return f;

  if(!d)return 0;

  len=0;

  switch(d->t)
  {
    case DT_NONE:
#ifdef LIMIT_SHOW_NULL
      len=snprintf(buf,PRINT_BUF_SIZE,"(NULL)");
#endif
      break;
    case DT_STRING:
      if(std_int_fd_allowed(f))
        len=write(f,d->d.s.text,d->d.s.length);
      if(size)*size=(*size)+len;
      return f;
    case DT_NUMBER:
      {
        static char mybuf[PRINT_BUF_SIZE+1];
        char * p;
        char * v=buf;
        int exp;
        snprintf(mybuf,PRINT_BUF_SIZE,"%+.*e",PREC,d->d.n);
        mybuf[PREC+3]=0;
        exp=atol(mybuf+PREC+4);
        p=mybuf+2;
        mybuf[2]=mybuf[1];
        if(mybuf[0]=='-')*(v++)='-';

/* before '.' */
        if(exp<0)*(v++)='0';
        else
        {
          while((exp>=0)&&(*p)){ *(v++)=*(p++); exp--;}
          while(exp>=0){ *(v++)='0'; exp--;}
        }
/* the (in)famous '.' */
        *(v++)='.';
/* after '.' */
        while(exp<-1){ *(v++)='0'; exp++;}
        while(*p) *(v++)=*(p++);
        *v=0;
/* take off '0's */
        for(v--;(v>buf)&&(*v=='0');v--)*v=0;
        if(*v=='.')*(v--)=0;
        len=v-buf+1;
      }
      break;
    case DT_LONG:
      len=snprintf(buf,PRINT_BUF_SIZE,"%ld",d->d.i); break;
    case DT_POINTER:
      len=snprintf(buf,PRINT_BUF_SIZE,"%ld",(long)d->d.p); break;
    case DT_ARRAY:
      for(i=0;i<d->d.a.length;i++)
         f=std_int_write(f,d->d.a.array[i], size);
      return f;
    case DT_FILE:
      return std_int_get_fd(d->d.i);
      break;
    default:
      len=snprintf(buf,PRINT_BUF_SIZE,">>UNKNOWN<<");
  }

  if(std_int_fd_allowed(f)) len=write(f,buf,len);
  if(size)*size=(*size)+len;
  return f;
}


/* ######### */

#define STD_INT_DEFAULT_FILE_PERMS	0664
#define STD_INT_DEFAULT_FILE_FLAGS	O_RDWR

void std_func_open(DATA * dest, DATA * data)
{
  DATA fname;
  int fd=-1;
  long fakefd;
  char * fn;

  if(!data)return;

  fname.t=DT_NONE;
  std_func_str(&fname,data);
  fn=fname.d.s.text;
  if(*fn=='>')
  {
    fn++;
    if(*fn=='>') /* append */
    {
      fn++;
      while((*fn=='\n')||(*fn==' ')||(*fn=='\t'))fn++;
      if((fd=open(fn,O_RDWR,STD_INT_DEFAULT_FILE_PERMS))==-1)
      {
        if((fd=creat(fn,STD_INT_DEFAULT_FILE_PERMS))==-1) return;
      } else { /* set position to end */
        lseek(fd,0L,2);
      }
    }else{       /* overwrite */
      while((*fn=='\n')||(*fn==' ')||(*fn=='\t'))fn++;
      if((fd=creat(fn,STD_INT_DEFAULT_FILE_PERMS))==-1) return;
    }
  } else
  {
    if(*fn=='<')
    {
      fn++;
      if(*fn=='<')fn++;
      while((*fn=='\n')||(*fn==' ')||(*fn=='\t'))fn++;
      if((fd=open(fn,O_RDONLY,STD_INT_DEFAULT_FILE_PERMS))==-1) return;
    } else {
      if((fd=open(fn,O_RDWR,STD_INT_DEFAULT_FILE_PERMS))==-1) return;
    }
  }

  if(fd==-1) return;

  fakefd=std_int_set_fd(fd);
  if(!fakefd) return;

  file_make(dest,fakefd);

  data_clear(&fname);
}

void std_func_connect(DATA * dest, DATA * data)
{
  if(!data)return;

}

void std_func_read(DATA * dest, DATA * data)
{
  char buf[STD_INT_MAX_BUF+1];
  DATA * file=array_get_index(data,0,0);
  DATA * len=array_get_index(data,0,1);
  long length;
  int fd;
  long fakefd;
/*  long b,c; */

  if (!file) file=data;
  if ((!file) || (file->t != DT_FILE))
  {
    fakefd=1; /* stdin */
    len=file;
  } else
    fakefd=file->d.i;

  if ((fd=std_int_get_fd(fakefd))<0) return;

  if(!std_int_fd_allowed(fd)) return;

  if((!len)||(len->t==DT_NONE)) length=-1;
  else length=data2int(len);

  if (length>STD_INT_MAX_BUF) length=STD_INT_MAX_BUF;


  if(length==-1)
    length=std_int_readln(fd,buf);
  else
    length=read(fd,buf,length);
/*
  {
    if(fd==0)
    {
      b=length;
      length=0;
      while(b)
      {
        sleep(1);
        if((c=read(0,buf+length,b))>0)
        {
          b=b-c;
          length=length+c;
        }
      }
    }
    else
      length=read(fd,buf,length);
  }
*/

  if(length>=0)
  {
    dest->t=DT_STRING;
    dest->d.s.length=length;
    dest->d.s.text=mystrdup(buf,length);
  }

  return;

}

void std_func_write(DATA * dest, DATA * data)
{
  long size=0;
  if(!data)return;

  std_int_write(1,data,&size);
  number_make(dest,0,0,size);
}

void std_func_writeln(DATA * dest, DATA * data)
{
  long size=0;
  char c='\n';
  int fd=std_int_write(2,data,&size);

  size=size+write(fd,&c,1);
  number_make(dest,0,0,size);
}

void std_func_close(DATA * dest, DATA * data)
{
  int fd;

  if(!data)return;

  if (data->t != DT_FILE) return;

  if(!std_int_fd_allowed(fd=std_int_get_fd(data->d.i))) return;

  std_int_free_fd(data->d.i);

  close(fd);
}



/* ###################################################### */
/* ###################################################### */


void std_init()
{
  custom_constants=std_constants;
  custom_functions=std_functions;
  custom_constants=0;
  custom_functions=0;

  std_int_fd_count=4;
  std_int_fds[0]=0;
  std_int_fds[1]=0; /* stdin  */
  std_int_fds[2]=1; /* stdout */
  std_int_fds[3]=2; /* stderr */

}
