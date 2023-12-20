#ifndef LLL_FUNCTIONS_H
#define LLL_FUNCTIONS_H

#include "LLL.h"

typedef void (* command_p)(char);
void LLL_execute_command(lll_command_list command,command_p command_f,uint8_t paramNumber,char options);

void LLL_add(char);
void LLL_and(char);
void LLL_cmp(char);
void LLL_dec(char);
void LLL_div(char);
void LLL_frjmp(char);
void LLL_in(char);
void LLL_inc(char);
void LLL_jmp(char);
void LLL_mov(char);
void LLL_mul(char);
void LLL_not(char);
void LLL_or(char);
void LLL_out(char);
void LLL_pop(char);
void LLL_push(char);
void LLL_ret(char);
void LLL_rjmp(char);
void LLL_sub(char);

void LLL_exit(char);
#endif