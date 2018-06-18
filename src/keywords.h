
#ifndef _KEYWORDS_H_
#define _KEYWORDS_H_

#define KEY_RETURN	0
#define KEY_IF		1
#define KEY_FOR		2
#define KEY_WHILE	3
#define KEY_FOREACH	4
#define KEY_DO		5

#ifdef _NEED_KEYWORDS_
static char * keywords[] =
{
"return", (char*) KEY_RETURN,
"if", (char*) KEY_IF,
"for", (char*) KEY_FOR,
"while", (char*) KEY_WHILE,
"do", (char*) KEY_DO,
"foreach", (char*) KEY_FOREACH,

(char*)0,(char*)0
};
#endif

/* ################################################ */

void run_keyword(KEYWORD * key);

KEYWORD * pass_keyword(LCHAR ** c,long key);

#endif

