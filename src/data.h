
#ifndef _DATA_H_
#define _DATA_H_

/* #################################################### */
/* data types */

#define DT_NONE		0
#define DT_STRING	1 /* string */
#define DT_NUMBER	2 /* floating point number */
#define DT_LONG		3 /* longint */
#define DT_FILE		4 /* file */
#define DT_ARRAY	5 /* array */
#define DT_POINTER	6 /* pointer */

/* ###################################################### */

typedef struct _ARRAY
{
  long length;
  struct _DATA ** array;
} ARRAY;

typedef struct _POINTREG
{
  struct _DATA * pointer;

  struct _POINTREG * next;
} POINTREG;

typedef struct _STRING
{
  long length;
  char * text;
} STRING;

typedef struct _DATA
{
  int t;    /* type */
  union     /* data */
  {
    struct _DATA * p;  /* pointer */
    struct _STRING s;  /* string */
    double n;          /* floating point number */
    long i;            /* long number */
    struct _ARRAY a;   /* array */
  } d;
  struct _POINTREG * pointers;
} DATA;


/* ###################################################### */
/* Array */

void array_make(DATA * dest);
void array_resize(DATA * dest,long space);
DATA * array_get_index(DATA * a, DATA * subs, long index);
DATA * array_cat(DATA * dest, DATA * a1, DATA * a2);

void array_set(DATA * a, DATA * data, DATA * subs);
DATA * array_get(DATA * a, DATA * data, DATA * subs);
void array_dup(ARRAY * dest, ARRAY * src);
void array_free(ARRAY * a);

/* ###################################################### */
/* Pointer */

void pointer_reg(DATA * dest);
void pointer_unreg(DATA * pointer);
void pointer_check(DATA * data);
void pointer_make(DATA * dest, DATA * data);

/* ###################################################### */
/* String */

void string_make(DATA * dest,char * s);
void string_dup(STRING * dest, STRING * src);

/* ###################################################### */
/* Numbers */

void number_make(DATA * dest, char*n, double number, long integer);

/* ###################################################### */
/* FILE */

void file_make(DATA * dest, long fd);

/* ###################################################### */
/* Data */

DATA * data_new();
void data_clear(DATA * data);
DATA * data_dup(DATA * dest, DATA * src);
long data2int(DATA * data);
void data_free(DATA * data);

/* ###################################################### */

char * mystrdup(char*s, long len);

DATA * array_format(DATA * dest, char * format, ... );

#endif
