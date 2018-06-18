
#ifndef _STD_H_
#define _STD_H_

#define STD_INT_MAX_FDS	32
#define STD_INT_MAX_BUF	1023
#define PRINT_BUF_SIZE	255

#ifdef _NEED_STD_

static char ** custom_constants;
static char ** custom_functions;

static char * std_constants[] = {

(char*)"NIL",	(char*)DT_NONE,		(char*)0,
(char*)"NULL",	(char*)DT_NONE,		(char*)0,

/* math */
(char*)"PI",	(char*)DT_NUMBER,	"3.14159265358979323846",
(char*)"E",	(char*)DT_NUMBER,	"2.7182818284590452354",
/* bool */
(char*)"TRUE",	(char*)DT_LONG,		(char*)1,
(char*)"FALSE",	(char*)DT_LONG,		(char*)0,
(char*)"YES",	(char*)DT_LONG,		(char*)1,
(char*)"NO",	(char*)DT_LONG,		(char*)0,
/* files */
(char*)"STDIN",	(char*)DT_FILE,		(char*)1,
(char*)"STDOUT",(char*)DT_FILE,		(char*)2,
(char*)"STDERR",(char*)DT_FILE,		(char*)3,
/* types */
(char*)"ARRAY",(char*)DT_LONG,		(char*)DT_ARRAY,
(char*)"INT",(char*)DT_LONG,		(char*)DT_LONG,
(char*)"INTEGER",(char*)DT_LONG,	(char*)DT_LONG,
(char*)"FLOAT",(char*)DT_LONG,		(char*)DT_NUMBER,
(char*)"FILE",(char*)DT_LONG,		(char*)DT_FILE,
(char*)"STRING",(char*)DT_LONG,		(char*)DT_STRING,

(char*)0,(char*)0,(char*)0
};


/* math */
void std_func_log(DATA * dest, DATA * data);
void std_func_log10(DATA * dest, DATA * data);
void std_func_sin(DATA * dest, DATA * data);
void std_func_cos(DATA * dest, DATA * data);
void std_func_tan(DATA * dest, DATA * data);
void std_func_asin(DATA * dest, DATA * data);
void std_func_acos(DATA * dest, DATA * data);
void std_func_atan(DATA * dest, DATA * data);
void std_func_exp(DATA * dest, DATA * data);
void std_func_sqrt(DATA * dest, DATA * data);
void std_func_floor(DATA * dest, DATA * data);
void std_func_ceil(DATA * dest, DATA * data);
void std_func_abs(DATA * dest, DATA * data);

/* utilitys */
void std_func_type(DATA * dest, DATA * data);
void std_func_getenv(DATA * dest, DATA * data);

/* cast */
void std_func_int(DATA * dest, DATA * data);
void std_func_val(DATA * dest, DATA * data);
void std_func_str(DATA * dest, DATA * data);
void std_func_array(DATA * dest, DATA * data);

/* arrays and strings */
void std_func_split(DATA * dest, DATA * data);
void std_func_join(DATA * dest, DATA * data);
void std_func_strpos(DATA * dest, DATA * data);
void std_func_ucase(DATA * dest, DATA * data);
void std_func_lcase(DATA * dest, DATA * data);
void std_func_chomp(DATA * dest, DATA * data);


/* files and net connections */
void std_func_open(DATA * dest, DATA * data);
void std_func_connect(DATA * dest, DATA * data);
void std_func_read(DATA * dest, DATA * data);
void std_func_write(DATA * dest, DATA * data);
void std_func_close(DATA * dest, DATA * data);



static char * std_functions[] = {

/* cast */
(char*)"int",(char*) std_func_int,
(char*)"val",(char*) std_func_val,
(char*)"str",(char*) std_func_str,
(char*)"array",(char*) std_func_array,
(char*)"list",(char*) std_func_array,

/* math */
(char*)"ln",(char*) std_func_log,
(char*)"log",(char*) std_func_log,
(char*)"log10",(char*) std_func_log10,
(char*)"sin",(char*) std_func_sin,
(char*)"cos",(char*) std_func_cos,
(char*)"tan",(char*) std_func_tan,
(char*)"asin",(char*) std_func_asin,
(char*)"acos",(char*) std_func_acos,
(char*)"atan",(char*) std_func_atan,
(char*)"exp",(char*) std_func_exp,
(char*)"sqrt",(char*) std_func_sqrt,
(char*)"floor",(char*) std_func_floor,
(char*)"ceil",(char*) std_func_ceil,
(char*)"round",(char*) std_func_int,
(char*)"abs",(char*) std_func_abs,

/* utilitys */
(char*)"type",(char*) std_func_type,
#ifndef LIMIT_NO_ENV
(char*)"getenv",(char*) std_func_getenv,
#endif

/* arrays and strings */
(char*)"split",(char*) std_func_split,
(char*)"join",(char*) std_func_join,
(char*)"strpos",(char*) std_func_strpos,
(char*)"strstr",(char*) std_func_strpos,
(char*)"strchr",(char*) std_func_strpos,
(char*)"ucase",(char*) std_func_ucase,
(char*)"lcase",(char*) std_func_lcase,
(char*)"chomp",(char*) std_func_chomp,
(char*)"chop",(char*) std_func_chomp,

/* files and net connections */
#ifndef LIMIT_NO_FILE
(char*)"open",(char*) std_func_open,
#endif

#ifndef LIMIT_NO_NET
(char*)"connect",(char*) std_func_connect,
#endif

(char*)"read",(char*) std_func_read,
(char*)"readln",(char*) std_func_read,
(char*)"write",(char*) std_func_write,
(char*)"print",(char*) std_func_write,
(char*)"close",(char*) std_func_close,


(char*)0,(char*)0
};
#endif


/* ############################################### */

extern int std_int_write(int f, DATA * data, long * size);
extern int std_int_get_fd(long fakefd);
extern long std_int_set_fd(int realfd);
extern int std_int_free_fd(long fakefd);

/* ############################################### */


extern void set_custom(char ** constants,char ** functions);
extern void std_init();

#endif

