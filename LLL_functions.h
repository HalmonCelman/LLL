#ifndef LLL_FUNCTIONS_H
#define LLL_FUNCTIONS_H

#include "LLL.h"

#define COMP_EQ 1  //equal
#define COMP_LO 0  //lower
#define COMP_GR 2  //greater


lll_err LLL_add(void);
lll_err LLL_addi(void);
lll_err LLL_sub(void);
lll_err LLL_subi(void);
lll_err LLL_mul(void);
lll_err LLL_muli(void);
lll_err LLL_div(void);
lll_err LLL_divi(void);
lll_err LLL_and(void);
lll_err LLL_andi(void);
lll_err LLL_or(void);
lll_err LLL_ori(void);
lll_err LLL_not(void);
lll_err LLL_inc(void);
lll_err LLL_dec(void);
lll_err LLL_ser(void);
lll_err LLL_seri(void);
lll_err LLL_ljmp(void);
lll_err LLL_jmp(void);
lll_err LLL_ret(void);
lll_err LLL_cmp(void);
lll_err LLL_cmpi(void);
lll_err LLL_seq(void);
lll_err LLL_deq(void);
lll_err LLL_slo(void);
lll_err LLL_dlo(void);
lll_err LLL_sgr(void);
lll_err LLL_dgr(void);
lll_err LLL_in(void);
lll_err LLL_out(void);
lll_err LLL_exit(void);

#endif