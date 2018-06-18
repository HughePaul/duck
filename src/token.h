
#ifndef _TOKEN_H_
#define _TOKEN_H_

/* OPERATORS */

#define LEV_LIFT  256

#define LEV_1		(LEV_LIFT*1)
#define LEV_2		(LEV_LIFT*2)
#define LEV_3		(LEV_LIFT*3)
#define LEV_4		(LEV_LIFT*4)
#define LEV_5		(LEV_LIFT*5)
#define LEV_6		(LEV_LIFT*6)
#define LEV_7		(LEV_LIFT*7)

#define LEV_MASK	(LEV_1|LEV_2|LEV_3|LEV_4|LEV_5|LEV_6|LEV_7)

#define LEV_2ARG	(LEV_2|LEV_3|LEV_4|LEV_5)
#define LEV_1ARG	(LEV_1)

#ifdef _NEED_OPERS_
static char * opers[]={

#define SB_NOT		(1 | LEV_1)
"!",(char*) SB_NOT,
#define SB_NEGATE	(2 | LEV_1)
"--",(char*) SB_NEGATE,

#define SB_AND		(1 | LEV_2)
"&",(char*) SB_AND,
#define SB_OR		(2 | LEV_2)
"|",(char*) SB_OR,
#define SB_POW		(3 | LEV_2)
"^",(char*) SB_POW,

#define SB_MULTIPLY	(1 | LEV_3)
"*",(char*) SB_MULTIPLY,
#define SB_DIVIDE	(2 | LEV_3)
"/",(char*) SB_DIVIDE,
#define SB_REMAIN	(3 | LEV_3)
"%",(char*) SB_REMAIN,
#define SB_INTDIV	(4 | LEV_3)
"\\",(char*) SB_INTDIV,

#define SB_ADD		(1 | LEV_4)
"+",(char*) SB_ADD,
#define SB_SUBTRACT	(2 | LEV_4)
"-",(char*) SB_SUBTRACT,

#define SB_EQ		(1 | LEV_5)
"=",(char*) SB_EQ,
#define SB_EQ_C		(2 | LEV_5)
"==",(char*) SB_EQ_C,
#define SB_EQ_W		(3 | LEV_5)
"===",(char*) SB_EQ_W,
#define SB_GT		(4 | LEV_5)
">",(char*) SB_GT,
#define SB_LT		(5 | LEV_5)
"<",(char*) SB_LT,
#define SB_GE		(6 | LEV_5)
">=",(char*) SB_GE,
#define SB_LE		(7 | LEV_5)
"<=",(char*) SB_LE,
#define SB_NE		(8 | LEV_5)
"!=",(char*) SB_NE,

#define SB_ASSIGN	(1 | LEV_6)
":=",(char*) SB_ASSIGN,

#define SB_COMMA	(1 | LEV_7)
",",(char*) SB_COMMA,
#define SB_TILDE	(2 | LEV_7)
"~",(char*) SB_TILDE,



(char*)0,(char*)0
};
#endif

/* #################################################### */
/* Tokens */

#define TT_VALUE	1 /* value */
#define TT_FUNCTION	2 /* external function */
#define TT_OPERATOR	3 /* operator */
#define TT_EXPRESSION	4 /* ( ) */
#define TT_SUBSCRIPT	5 /* [ ] */
#define TT_INTFUNC	6 /* internal function */

#define TT_VARIABLE	'$' /* variable */
#define TT_VARIABLE_P	'@' /* variable pointer */

#define TT_VARIABLE_L	'#' /* variable length */
#define TT_VARIABLE_A	'?' /* variable address */

typedef struct _TOKEN
{
  long type;
  union
  {
    DATA v;               /* value */
    long s;               /* symbol / operator / variable */
    struct _FUNCTION * f; /* function */
  } d;
  struct _TOKEN * e;    /* expression - used in variable and function too*/

/* set to 1 if its an lvalue variable */
  int noeval;

  char * name;  /* used for function or variable name when loading */

  struct _TOKEN *prev;
  struct _TOKEN *next;
} TOKEN;

TOKEN * token_new();
void token_free(TOKEN * token);

/* #################################################### */
/* Variables - used for loading */

typedef struct _VARIABLE
{
  char * name;
  long varpos;

  struct _VARIABLE * next;
} VARIABLE;

long variable_new(char * name);
long variable_get(char * name);
void variable_clear();


/* #################################################### */
/* Functions */

typedef struct _FUNCTION
{
  char * name;
  struct _STATE * block;

  struct _SCRIPT * script;

  struct _VARIABLE * vars;  /* used for loading only */
  long varcount;

  struct _FUNCTION * next;
} FUNCTION;

FUNCTION * function_new(char * name);
FUNCTION * function_get(char * name);

/* #################################################### */
/* Keywords */

typedef struct _KEYWORD
{
  long keyword; /* see keywords.h */

  struct _TOKEN * expr[3];
  struct _STATE * block[2];

  long var;

} KEYWORD;

KEYWORD * keyword_new();
void keyword_free(KEYWORD*keyword);


/* #################################################### */
/* Statements */

#define ST_EXPR		1
#define ST_KEYWORD	2
#define ST_BLOCK	3


typedef struct _STATE
{
  int type;
  union
  {
    struct _TOKEN * expr;
    struct _STATE * block;
    struct _KEYWORD * keyword;
  } d;

  struct _STATE * next;
  struct _STATE * prev;
} STATE;

STATE * state_new();
void state_free(STATE*state);


#endif
