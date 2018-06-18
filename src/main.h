

#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "load.h"
#include "data.h"
#include "token.h"
#include "stack.h"
#include "keywords.h"
#include "pass.h"
#include "expr.h"
#include "run.h"
#include "std.h"


/* #################################################### */
/* #################################################### */

/* #define LIMIT_NO_FILE   */
/* #define LIMIT_NO_NET    */
/* #define LIMIT_NO_ENV    */
/* #define LIMIT_NO_STDIN  */
/* #define LIMIT_NO_STDOUT */
/* #define LIMIT_NO_STDERR */

/* #define LIMIT_SHOW_NULL */

/* Floating point number precision */
#define PREC	5

/* #################################################### */
/* #################################################### */

#define DEBUG

#ifdef DEBUG
/*  #define DEBUG_LOAD  */
/*  #define DEBUG_VARS  */
/*  #define DEBUG_FUNC  */
/*  #define DEBUG_BLOCK */
/*  #define DEBUG_RUN_E */
/*  #define DEBUG_RUN   */
#endif

/* #################################################### */
/* #################################################### */

#define ARG_OFFSET	1

typedef struct _SCRIPT
{
  char * filename;

  struct _FUNCTION * functions;

  struct _VARIABLE *vars;       /* script variables descripts */
  struct _FUNCTION * init;      /* init function */
  struct _FUNCTION * done;      /* done function */

  DATA * var_area;              /* script variable area */
  long varcount;                /* number of variables */

  struct _SCRIPT *next;
  struct _SCRIPT *prev;
} SCRIPT;

struct
{
  struct _FUNCTION * func;      /* current loading function */

  struct _STACK * stack;        /* top stack */

  struct _SCRIPT * script;      /* current script */

  SCRIPT *first;  
  SCRIPT *last;
} program;

LCHAR * lchar_first;
LCHAR * lchar_last;

#define EFATAL	1
#define EWARN	2
#define EDROP	3

extern void die(int t,char * error, ... );

extern int duck_error;

extern void duck_init();
extern int load_script(char *filename);
extern int unload_script(char *filename);

#endif

