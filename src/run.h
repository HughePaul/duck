
#ifndef _RUN_H_
#define _RUN_H_

/* #################################################### */

void run_block(STATE * state);
void run_func(FUNCTION * func, DATA * retval, DATA * args);

/* #################################################### */

void run_init();
int run_hook(char * hook, DATA * data);
void run_done();

/* #################################################### */

#endif
