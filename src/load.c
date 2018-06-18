#include "main.h"
#include <ctype.h>

/* #################################################### */
/* lchars */

LCHAR * lchar_new(char * s,long line)
{
  LCHAR *c = (LCHAR*)calloc(1,sizeof(LCHAR));
  LCHAR *t;

       if(!strcasecmp(s,"and"))strcpy(s,"&");
  else if(!strcasecmp(s,"or"))strcpy(s,"|");
  else if(!strcasecmp(s,"not"))strcpy(s,"!");
  else if(!strcasecmp(s,"<>"))strcpy(s,"!=");
  else if(!strcasecmp(s,"eq"))strcpy(s,"==");
/*  else if(!strcasecmp(s,"wm"))strcpy(s,"==="); */
  else if(!strcasecmp(s,"ne"))strcpy(s,"!=");
  else if(!strcasecmp(s,"lt"))strcpy(s,"<");
  else if(!strcasecmp(s,"gt"))strcpy(s,">");
  else if(!strcasecmp(s,"le"))strcpy(s,"<=");
  else if(!strcasecmp(s,"ge"))strcpy(s,">=");

  if(!strcasecmp(s,"++"))
  {
    free(c);
    t=lchar_new(":=",line);
    if((!t)||(!t->prev))
    { 
       die(EFATAL,"Line %d: variable expected before '%s'.",line,s);
       return 0;
    }
    lchar_new(t->prev->s,line);
    lchar_new("+",line);
    lchar_new("1",line);
    return 0;
  }

  if(!strcasecmp(s,"--"))
  {
    free(c);
    t=lchar_new(":=",line);
    if((!t)||(!t->prev))
    { 
       die(EFATAL,"Line %d: variable expected before '%s'.",line,s);
       return 0;
    }
    lchar_new(t->prev->s,line);
    lchar_new("-",line);
    lchar_new("1",line);
    return 0;
  }

  if(!strcasecmp(s,"+="))
  {
    free(c);
    t=lchar_new(":=",line);
    if((!t)||(!t->prev))
    { 
       die(EFATAL,"Line %d: variable expected before '%s'.",line,s);
       return 0;
    }
    lchar_new(t->prev->s,line);
    lchar_new("+",line);
    return 0;
  }

  if(!strcasecmp(s,"-="))
  {
    free(c);
    t=lchar_new(":=",line);
    if((!t)||(!t->prev))
    { 
       die(EFATAL,"Line %d: variable expected before '%s'.",line,s);
       return 0;
    }
    lchar_new(t->prev->s,line);
    lchar_new("-",line);
    return 0;
  }

  if(!strcasecmp(s,"/="))
  {
    free(c);
    t=lchar_new(":=",line);
    if((!t)||(!t->prev))
    { 
       die(EFATAL,"Line %d: variable expected before '%s'.",line,s);
       return 0;
    }
    lchar_new(t->prev->s,line);
    lchar_new("/",line);
    return 0; 
  }

  if(!strcasecmp(s,"*="))
  {
    free(c);
    t=lchar_new(":=",line);
    if((!t)||(!t->prev))
    { 
       die(EFATAL,"Line %d: variable expected before '%s'.",line,s);
       return 0;
    }
    lchar_new(t->prev->s,line);
    lchar_new("*",line);
    return 0;
  }

#ifdef DEBUG_LOAD
  printf("load: >%s<\n",s);
#endif

  c->s=strdup(s);
  c->line=line;
  c->prev=lchar_last;
  if(lchar_last)lchar_last->next=c; else lchar_first=c;
  lchar_last=c;
  return c;
}

void lchar_clear()
{
  LCHAR * c=lchar_first;
  LCHAR * n;

  for(n=(c)?c->next:0; c ;c=n,n=(c)?c->next:0)
  {
    free(c->s);
    free(c);
  }

  lchar_first=0;
  lchar_last=0;
}

int load_toplevel()
{
  lchar_first=0;
  lchar_last=0;
  return load_file(program.script->filename);
}

#define LS_NORMAL	0
#define LS_STRING1	39
#define LS_STRING2	34
#define LS_NUMBER	2
#define LS_ALPHA	3
#define LS_COMMENT	4
#define LS_COMMAND	5
#define LS_VALUE	6
#define LS_INLINE	7

int load_command(char * command, char * value)
{
#ifdef DEBUG_LOAD
  printf("Compiler command: %s %s\n",command,value);
#endif

  if (!strcasecmp(command,"include"))
  {
    if (*value=='"')value++;
    if (*value) if (*(value+strlen(value)-1)=='"')*(value+strlen(value)-1)=0;
    return load_file(value);
  }

  die(EWARN,"'%s' is an unknown compiler command.",command);
  return 0;
}

int load_file(char * file)
{
  FILE * f;
  char b[1024];
  char l[2048];
  char *p;
  char *o=l;
  char inline_end=0;
  char lastwas=0;

  int state=LS_NORMAL;
  long line=0;
  long lastline=0;

  char command[255];
  char value[255];
  char *in=0;

  if(!(f=fopen(file,"r")))
  {
    die(EFATAL,"Couldn't open script file '%s'.",file);
    return -1;
  }
  while (!feof(f))
    if(fgets(b,1023,f))
    for (line++,p=b;*p;p++)
    {
      switch(state)
      {
        case LS_INLINE:
          if(isalnum(*p))
             *(o++)=*p;
          else
          {
             *o=0;
             lchar_new(l,line); if(duck_error) return -1;
             lchar_new("+",line); if(duck_error) return -1;
             o=l;*(o++)='"';
             state='"';
             if((!inline_end) && (*p!='}')) p--;
          }
          break;
        case LS_ALPHA:
        case LS_NUMBER:
        case LS_NORMAL:
          if (state==LS_NUMBER) 
          {
             lastwas=0;
             if( (*p=='.')  ||
             ((*p>='0') && (*p<='9')) )
             {
               *(o++)=*p;
               break;
             }
          } else 
          if (state==LS_ALPHA) 
          {
             lastwas=0;
             if( ((*p>='a') && (*p<='z')) ||
             ((*p>='A') && (*p<='Z')) || (*p=='_') ||
             ((*p>='0') && (*p<='9')) )
             {
               *(o++)=*p;
               break;
             }
          } else {

      /* Start of #! comment */
            if ((*p=='#') && (*(p+1)=='!'))
            {
              p=" ";
              break;
            }

             if( ((*p>='a') && (*p<='z')) ||
             ((*p>='A') && (*p<='Z')) ||
             ((*p>='0') && (*p<='9')) ||
             (*p=='$') || (*p=='@')||
             (*p=='?') || (*p=='#'))
             {
               if ( ((*p=='.') || ((*p>='0') && (*p<='9'))) && (o==l) )
                 state=LS_NUMBER;
               else
                 state=LS_ALPHA;
               *(o++)=*p;
               break;
             }
          }
/* Operator or symbol */
          if (o!=l)
          {
            /* write old token */
            *o=0; o=l;
            lchar_new(l,line); if(duck_error) return -1;
          }
          state=LS_NORMAL;

          if((*p==' ') || (*p=='\t') || (*p=='\n') || (*p=='\r'))
          { /* SKIP Whitespace */
            break;
          }

      /* Start of loader command */
          if ((*p=='!') && (*(p+1)=='!'))
          {
            lastwas=0;
            p++; o=l;
            lastline=line;
            state=LS_COMMAND;
            in=command;
            break;
          }

      /* Start of string */
          if((*p==(char)LS_STRING1)||(*p==(char)LS_STRING2))
          {
            lastwas=0;
            *(o++)='"';
            lastline=line;
            state=(int)*p;
            break;
          }

      /* Start of comment */
          if ((*p=='/') && (*(p+1)=='*'))
          {
            lastwas=0;
            p++;
            lastline=line;
            state=LS_COMMENT;
            break;
          }

      /* Start of line comment */
          if ((*p=='/') && (*(p+1)=='/'))
          {
            lastwas=0;
            p=" ";
            break;
          }

/* EXTENDED OPERATORS */

      *(o++)=*p;

      /* extended operators */
          if (
             ((*p=='=') && (*(p+1)=='=')) ||
             ((*p=='>') && (*(p+1)=='=')) ||
             ((*p=='<') && (*(p+1)=='=')) ||
/*             ((*p=='+') && (*(p+1)=='+')) || */
/*             ((*p=='-') && (*(p+1)=='-')) || */
             ((*p=='+') && (*(p+1)=='=')) ||
             ((*p=='-') && (*(p+1)=='=')) ||
             ((*p=='*') && (*(p+1)=='=')) ||
             ((*p=='/') && (*(p+1)=='=')) ||
             ((*p=='!') && (*(p+1)=='=')) ||
             ((*p==':') && (*(p+1)=='=')) ||
            0) *(o++)=*(++p);

      /* Reduced operators */
          else if (
             ((*p=='&') && (*(p+1)=='&')) ||
             ((*p=='|') && (*(p+1)=='|')) ||
            0) p++;

      /* Write Operator or symbol */

          if ( 
               ((*p==',')&&(lastwas==',')) ||
               ((*p==')')&&(lastwas==',')) ||
               ((*p==',')&&(lastwas=='(')) )
           {lchar_new("NULL",line); if(duck_error) return -1;}


	  *o=0;
          lchar_new(l,line); if(duck_error) return -1;
          o=l;

          if (*p==',') lastwas=',';
          else if (*p==')') lastwas=')';
          else lastwas=0;

        break;
        case LS_STRING1:
        case LS_STRING2: /* end of quote */
           if (((state==LS_STRING1)&&(*p==(char)LS_STRING1))||
               ((state==LS_STRING2)&&(*p==(char)LS_STRING2)))
           {
             *o=0; o=l;
             lchar_new(l,line); if(duck_error) return -1;
             state=LS_NORMAL;
             break;
           }
      /* inline variable */
           if (*p=='$')
           {
             *o=0; o=l;
             lchar_new(l,line); if(duck_error) return -1;
             lchar_new("+",line); if(duck_error) return -1;
             *(o++)='$';
             if (*(p+1)=='{') {p++; inline_end='}';} else inline_end=0;
             state=LS_INLINE;
             break;
           }
      /* escape code */
           if ( (*p=='\\') && (*(p+1)) )
           {
             p++;
             switch(*p)
             {
             case 'n':
               *(o++)='\n';break;
             case 't':
               *(o++)='\t';break;
             case 'r':
               *(o++)='\r';break;
             case '0':
               *(o++)='\0';break;
             default:
               *(o++)=*p;
             }
             break;
           }
           *(o++)=*p;
        break;

        case LS_COMMENT:
           if ((*p=='*') && (*(p+1)=='/')) /* end of comment */
           {
             p++;
             state=LS_NORMAL;
           }
        break;

        case LS_COMMAND:
             if( ((*p>='a') && (*p<='z')) ||
             ((*p>='A') && (*p<='Z')) || (*p=='_') ||
             ((*p>='0') && (*p<='9')) )
             {
               *(in++)=*p;
               break;
             }

             if((*p==' ') || (*p=='\t') || (*p=='\n') || (*p=='\r'))
             {
               *in=0;
               in=value;
               state=LS_VALUE;
               break;
             }
        break;

        case LS_VALUE:
             if( (*p=='\n') || (*p=='\r') ||
              ((*p=='/') && (*(p+1)=='/')) ||
              ((*p=='/') && (*(p+1)=='*')) )
             {
               p--;
               state=LS_NORMAL;

          /* clear whitespace at end of value */
               for(in--;(in>=value)&&((*in=='\t')||(*in==' '));in--);
               *(++in)=0;
               
          /* run command */
               if(load_command(command,value)) return -1;
               break;
             }
             if( (in!=value) || (!( 
                    (*p==' ') || (*p=='\t')
                                ))  )
               *(in++)=*p;
        break;
      }

    }

  if ((state==LS_STRING1)||(state==LS_STRING2))
  {
    die(EFATAL,"Line %d: Unterminated string constant.",lastline);
    return -1;
  }
  if (state==LS_COMMENT)
  {
    die(EFATAL,"Line %d: Unterminated comment.",lastline);
    return -1;
  }

  fclose(f);

  return 0;
}

