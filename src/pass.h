
#ifndef _PASS_H_
#define _PASS_H_

/* #################################################### */


int pass_my(LCHAR ** c);
TOKEN * pass_expr(LCHAR ** c, char limit);
STATE * pass_state(LCHAR ** c);
STATE * pass_block(LCHAR ** c);
FUNCTION * pass_sub(LCHAR ** c);

int first_pass();
#define nextc(x)	if(_nextc(x))return 0;
int _nextc(LCHAR ** c);

#endif
