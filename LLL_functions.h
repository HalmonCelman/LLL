#ifndef LLL_FUNCTIONS_H
#define LLL_FUNCTIONS_H

#include "LLL.h"

typedef uint8_t (* command_p)(void);
uint8_t LLL_execute_command(lll_command_list command,command_p command_f,uint8_t paramNumber,char options);

uint8_t LLL_add(void);
uint8_t LLL_and(void);
uint8_t LLL_cmp(void);
uint8_t LLL_dec(void);
uint8_t LLL_frjmp(void);
uint8_t LLL_in(void);
uint8_t LLL_inc(void);
uint8_t LLL_jmp(void);
uint8_t LLL_mov(void);
uint8_t LLL_mul(void);
uint8_t LLL_not(void);
uint8_t LLL_or(void);
uint8_t LLL_out(void);
uint8_t LLL_pop(void);
uint8_t LLL_push(void);
uint8_t LLL_ret(void);
uint8_t LLL_rjmp(void);
uint8_t LLL_sub(void);

uint8_t LLL_exit(void);
#endif