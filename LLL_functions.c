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

    lll_param param1 = LLL_getParam(0);
    lll_send_info("param1, value1",param1.value1);
    lll_send_info("param1, value2",param1.value2);
    lll_send_info("param1, type",param1.type);
    lll_send_info("param1, carry",param1.carry);

    lll_param param2 = LLL_getParam(param1.carry);
    lll_send_info("param2, value1",param2.value1);
    lll_send_info("param2, value2",param2.value2);
    lll_send_info("param2, type",param2.type);
    lll_send_info("param2, carry",param2.carry);

    lll_param param3 = LLL_getParam(param2.carry);
    lll_send_info("param3, value1",param3.value1);
    lll_send_info("param3, value2",param3.value2);
    lll_send_info("param3, type",param3.type);
    lll_send_info("param3, carry",param3.carry);

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
