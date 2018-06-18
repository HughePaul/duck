
#ifndef _LOAD_H_
#define _LOAD_H_

/* #################################################### */
/* LCHAR */

typedef struct _LCHAR
{
  char * s;
  long line;

  struct _LCHAR *prev;
  struct _LCHAR *next;
} LCHAR;

LCHAR * lchar_first;

LCHAR * lchar_new();
void lchar_clear();


int load_file(char * file);
int load_toplevel();

#endif
