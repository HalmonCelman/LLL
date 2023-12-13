///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////                                                              LLL instructions - by KK                                                              //////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "LLL_functions.h"

#if LLL_DEBUG_MODE

static void copyString(char* dest, char* source){
    while(*source){
        (*dest++)=*source++;
    }
}


char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

#endif


lll_err LLL_add(void){
    lll_err tmpErr;
    tmpErr.status=LLL_OK;

    lll_send_info("Command: add",0);

    return tmpErr;
}

lll_err LLL_and(void){
    lll_err tmpErr;
    tmpErr.status=LLL_OK;

    lll_send_info("Command: and",0);

    return tmpErr;
}

lll_err LLL_cmp(void){
    lll_err tmpErr;
    tmpErr.status=LLL_OK;

    lll_send_info("Command: cmp",0);

    return tmpErr;
}

lll_err LLL_dec(void){
    lll_err tmpErr;
    tmpErr.status=LLL_OK;

    lll_send_info("Command: dec",0);

    return tmpErr;
}

lll_err LLL_div(void){
    lll_err tmpErr;
    tmpErr.status=LLL_OK;

    lll_send_info("Command: div",0);

    return tmpErr;
}

lll_err LLL_frjmp(void){
    lll_err tmpErr;
    tmpErr.status=LLL_OK;

    lll_send_info("Command: frjmp",0);

    return tmpErr;
}

lll_err LLL_in(void){
    lll_err tmpErr;
    tmpErr.status=LLL_OK;

    lll_send_info("Command: in",0);

    return tmpErr;
}

lll_err LLL_inc(void){
    lll_err tmpErr;
    tmpErr.status=LLL_OK;

    lll_send_info("Command: inc",0);

    return tmpErr;
}

lll_err LLL_jmp(void){
    lll_err tmpErr;
    tmpErr.status=LLL_OK;

    lll_send_info("Command: jmp",0);

    return tmpErr;
}

lll_err LLL_mov(void){
    lll_err tmpErr;
    tmpErr.status=LLL_OK;

    lll_send_info("Command: mov",0);

    return tmpErr;
}

lll_err LLL_mul(void){
    lll_err tmpErr;
    tmpErr.status=LLL_OK;

    lll_send_info("Command: mul",0);

    return tmpErr;
}

lll_err LLL_not(void){
    lll_err tmpErr;
    tmpErr.status=LLL_OK;

    lll_send_info("Command: not",0);

    return tmpErr;
}

lll_err LLL_or(void){
    lll_err tmpErr;
    tmpErr.status=LLL_OK;

    lll_send_info("Command: or",0);

    return tmpErr;
}

lll_err LLL_out(void){
    lll_err tmpErr;
    tmpErr.status=LLL_OK;

    lll_send_info("Command: out",0);

    return tmpErr;
}

lll_err LLL_ret(void){
    lll_err tmpErr;
    tmpErr.status=LLL_OK;

    lll_send_info("Command: ret",0);

    return tmpErr;
}

lll_err LLL_rjmp(void){
    lll_err tmpErr;
    tmpErr.status=LLL_OK;

    lll_send_info("Command: rjmp",0);

    return tmpErr;
}

lll_err LLL_sub(void){
    lll_err tmpErr;
    tmpErr.status=LLL_OK;

    lll_send_info("Command: sub",0);

    return tmpErr;
}

lll_err LLL_exit(void){
    lll_err tmpErr;
    tmpErr.status=LLL_EOP;

    lll_send_info("Command: exit",0);

    return tmpErr;
}
