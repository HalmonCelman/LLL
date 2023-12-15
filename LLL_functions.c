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

typedef struct{
    uint32_t begin;
    uint8_t increasing;
    uint32_t length;
} range_s;


/*
gets value of parameter or structure of range
*/
static uint8_t getValueOrRange(lll_param parameter,range_s *out_range,uint8_t *value){
    switch(parameter.type){
        case mem:
            *value = LLL_load_mem(parameter.value1);
            return 0;
        case cst:
            *value = parameter.value1;
            return 0;
        case flag:
            *value = ((status_register & (1<<parameter.value1))?1:0);
            return 0;
        case range:
            if(parameter.value1 > parameter.value2){
                out_range->increasing = 1;
                out_range->length = parameter.value1 - parameter.value2+1;
            }else{
                out_range->increasing = 0;
                out_range->length = parameter.value2 - parameter.value1+1;
            }
            out_range->begin = parameter.value2;
            
            return 1;
        default:
            lll_throw_error(1,"Interpreter error!",0);
            return 2;
    }
}

/*
gets actual value of range or item in single iteration - if it's range incerases/decerases also begin of range
iterator    - which iteration
*range      - address to range structure
value       - value to write(at iterator=0) if it's not range
*actValue   - where to write resulting value
rov         - if it's range or value - range:1 - value:0

return value - if length of range has been exceed - loop should be broken
*/
static uint8_t getValueInIteration(uint32_t iterator,range_s * range, uint8_t value, uint8_t *actValue, uint8_t rov){
    if(rov){
        if(iterator>=range->length) return 1;
        *actValue=LLL_load_mem(range->begin);
        range->begin += (range->increasing ? 1 : -1);
    }else{
        if(!iterator){
            *actValue = value;
        }else{
            *actValue = 0;
        }
    }
    return 0;
}

lll_err LLL_add(void){
    lll_err tmpErr;
    tmpErr.status=LLL_OK;

    lll_param param1 = LLL_getParam(0);
    lll_param param2 = LLL_getParam(param1.carry);
    lll_param param3 = LLL_getParam(param2.carry);
    globalCarry = param3.carry;

    uint8_t value1,value2,value3;
    range_s range1,range2,range3;

    uint8_t carry=0;
    uint8_t actVal1, actVal2;
    uint16_t sum;

    // 1. check if range or value
    uint8_t rov1=getValueOrRange(param1,&range1,&value1);
    uint8_t rov2=getValueOrRange(param2,&range2,&value2);
    uint8_t rov3=getValueOrRange(param3,&range3,&value3);

    for(int i=0;i<(rov1 ? range1.length : 1);i++){  //2. if range do multiple times, if value - once

        if(getValueInIteration(i,&range2,value2,&actVal1,rov2)) break;  //3. get values
        if(getValueInIteration(i,&range3,value3,&actVal2,rov3)) break;

        sum = actVal1+actVal2+carry;                                    //4. do whatever you want
        if(sum>255){
            carry=1;
            sum-=256;
        }

        if(rov1){                                                       //5. save result
            LLL_save_mem(range1.begin,sum);
            range1.begin += (range1.increasing ? 1 : -1);
        }else{
            LLL_save_mem(param1.value1,sum);
        }
    }

    #if LLL_DEBUG_MODE
        lll_send_info("Command: add, sum: ",sum);
    #endif
    
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
