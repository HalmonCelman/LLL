///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////                                                              LLL instructions - by KK                                                              //////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "LLL_functions.h"
#include "LLL_processing.h"

token_s tokens[3];
uint8_t actVal[3];
uint8_t carry=0;

void LLL_execute_command(lll_command_list command,command_p command_f,uint8_t paramNumber,char options){
    carry = 0;

    if(LLL_doIfJump(command)){
        command_f(options);
        globalCarry = lll_get();
        return;
    }
    
    if(paramNumber){
        for(uint8_t i=0;i<paramNumber;i++){                                             // 1. get parameters and values
            tokens[i].param = LLL_getParam((i ? tokens[i-1].param.carry : 0));
            getValueOrRange(&tokens[i]);
        }
        globalCarry = tokens[paramNumber-1].param.carry;                                // 2. setup carry

        if(tokens[0].param.type == range || tokens[0].param.type == sp){                // if range - token 0 is always the most important
            for(uint32_t j=0;1;j++){
                for(uint8_t i=0;i<paramNumber;i++){
                    if(getValueInIteration(j,&tokens[i],&actVal[i])) return;     // 3. get actual values 
                }
                command_f(options);                                                       // 4. do whatever you want
            }
        }else{                                                                          // if not range
            getValueInIteration(0,&tokens[1],&actVal[1]);                               // 3. get actual values
            getValueInIteration(0,&tokens[2],&actVal[2]);
            command_f(options);                                                           // 4. do whatever you want
        }
    }else{
        command_f(options);
    }
}

void LLL_add(char s){
    uint16_t sum;

    sum = actVal[1]+actVal[2]+carry;
    if(sum>255){
        carry=1;
        sum-=256;
    }else{
        carry=0;
    }

    LLL_save(tokens[0],sum);

    #if LLL_DEBUG_MODE
        lll_send_info("Command: add, sum: ",sum);
    #endif
}

void LLL_and(char s){
    lll_send_info("Command: and",0);
}

void LLL_cmp(char s){
    lll_send_info("Command: cmp",0);
}

void LLL_dec(char s){
    lll_send_info("Command: dec",0);
}

void LLL_div(char s){
    lll_send_info("Command: div",0);
}

void LLL_frjmp(char s){
    lll_send_info("Command: frjmp",0);
}

void LLL_in(char s){
    lll_send_info("Command: in",0);
}

void LLL_inc(char s){
    lll_send_info("Command: inc",0);
}

void LLL_jmp(char s){
    lll_send_info("Command: jmp",0);
}

void LLL_mov(char s){
    lll_send_info("Command: mov",0);
}

void LLL_mul(char s){
    lll_send_info("Command: mul",0);
}

void LLL_not(char s){
    lll_send_info("Command: not",0);
}

void LLL_or(char s){
    lll_send_info("Command: or",0);
}

void LLL_out(char s){
    lll_send_info("Command: out",0);
}

void LLL_pop(char s){
    lll_send_info("Command: pop",0);
}

void LLL_push(char s){
    lll_send_info("Command: push",0);
}

void LLL_ret(char s){
    lll_send_info("Command: ret",0);
}

void LLL_rjmp(char s){
    lll_send_info("Command: rjmp",0);
}

void LLL_sub(char s){
    lll_send_info("Command: sub",0);
}