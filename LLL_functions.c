///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////                                                              LLL instructions - by KK                                                              //////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "LLL_functions.h"
#include "LLL_processing.h"

token_s tokens[3];
uint8_t actVal[3];

// flags
uint8_t s_opt=0;
uint8_t Overflow=0;
uint8_t Zero=0;
uint8_t Rest=0;
uint8_t Additional=0;

static uint8_t LLL_conditional(lll_command_list command,command_p command_f,char options){
    if(options & 0x80){
        uint8_t flags=lll_get();
        if(flags & 0x80){ //if should be set
            if((flags & 0x7F & status_register) == (flags & 0x7F)){
                return 2;
            }else{
                return 0;
            }
        }else{ // if cleared
            if((~(flags & 0x7F) | status_register) == ~(flags & 0x7F)){
                return 2;
            }else{
                return 0;
            }
        }
    }else{
        return 1;
    }
}

static void setupFlags(uint8_t flags){
    if(s_opt){
        if(flags & LLLF_O){
            if(Overflow){
                status_register |= LLLF_O;
            }else{
                status_register &=~ LLLF_O;
            }
        }
        if(flags & LLLF_A){
            if(Additional){
                status_register |= LLLF_A;
            }else{
                status_register &=~ LLLF_A;
            }
        }
        if(flags & LLLF_R){
            if(Rest){
                status_register |= LLLF_R;
            }else{
                status_register &=~ LLLF_R;
            }
        }
        if(flags & LLLF_Z){
            if(Zero){
                status_register |= LLLF_Z;
            }else{
                status_register &=~ LLLF_Z;
            }
        }
        #if LLL_DEBUG_MODE
            lll_send_info("sreg status: ",status_register);
        #endif
    }
}

void LLL_execute_command(lll_command_list command,command_p command_f,uint8_t paramNumber,char options){
    s_opt=0;
    Overflow = 0;
    Additional = 0;
    Rest = 0;
    Zero = 1;
    if(options & 0x40) s_opt=1; // check if s option set
    
    uint8_t condition=LLL_conditional(command,command_f, options);
    ra = lll_getPosition()-condition;

    if(LLL_doIfJump(command)){
        if(condition)
            setupFlags(command_f());
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
                
                if(condition)
                    setupFlags(command_f());                                                       // 4. do whatever you want
            }
        }else{  
            getValueInIteration(0,&tokens[0],&actVal[0]);                                                                        // if not range
            getValueInIteration(0,&tokens[1],&actVal[1]);                               // 3. get actual values
            getValueInIteration(0,&tokens[2],&actVal[2]);
            
            if(condition)
                setupFlags(command_f());                                                           // 4. do whatever you want
        }
    }else{
        if(condition)
            setupFlags(command_f());
        globalCarry = lll_get();
    }
}

uint8_t LLL_add(void){
    uint16_t sum;

    sum = actVal[1]+actVal[2]+Overflow;
    if(sum>255){
        Overflow=1;
        sum-=256;
    }else{
        Overflow=0;
    }

    LLL_save(tokens[0],sum);

    #if LLL_DEBUG_MODE
        lll_send_info("Command: add, sum: ",sum);
    #endif

    if(Zero){
        if(sum){
            Zero=0;
        }
    }

    return LLLF_O | LLLF_Z;
}

uint8_t LLL_and(void){
    uint8_t and_v = actVal[1] & actVal[2];

    if(Zero){
        if(and_v){
            Zero=0;
        }
    }

    #if LLL_DEBUG_MODE
        lll_send_info("Command: and, result: ",and_v);
    #endif

    return LLLF_Z;
}

uint8_t LLL_cmp(void){
    if(!Rest){
    if(actVal[0] > actVal[1]){
        Rest = 1;
        Additional = 1;
    }else if(actVal[0] < actVal[1]){
        Rest = 1;
    }
    }

    #if LLL_DEBUG_MODE
        lll_send_info("Command: cmp, r:",Rest);
    #endif
    s_opt=1;
    return LLLF_R | LLLF_A;
}

uint8_t LLL_dec(void){
    if(!actVal[0]){
        Overflow=1;
    }

    if(Zero){
        if(actVal[0] != 1){
            Zero = 0;
        }
    }

    LLL_save(tokens[0],(uint8_t)(actVal[0]-1));

    #if LLL_DEBUG_MODE
        lll_send_info("Command: dec",0);
    #endif
    return LLLF_O | LLLF_Z;
}

uint8_t LLL_div(void){
    lll_send_info("Command: div",0);
}

uint8_t LLL_frjmp(void){
    if(rv){
        pushStack8byte(lll_getPosition());
    }

    lll_goTo(ra+(int8_t)r_jmp);

    #if LLL_DEBUG_MODE
        lll_send_info("Command: frjmp",rv);
    #endif

    return 0;
}

uint8_t LLL_in(void){
    lll_send_info("Command: in",0);
}

uint8_t LLL_inc(void){
    if(!(actVal[0]+1)){
        Overflow=1;
    }

    if(Zero){
        if(actVal[0] != 255){
            Zero = 0;
        }
    }

    LLL_save(tokens[0],(uint8_t)(actVal[0]+1));

    #if LLL_DEBUG_MODE
        lll_send_info("Command: inc",0);
    #endif
    return LLLF_O | LLLF_Z;
}

uint8_t LLL_jmp(void){
    if(rv){
        pushStack8byte(lll_getPosition());
    }

    lll_goTo(d_jmp);

    #if LLL_DEBUG_MODE
        lll_send_info("Command: jmp",rv);
    #endif

    return 0;
}

uint8_t LLL_mov(void){

    LLL_save(tokens[0],actVal[1]);

    #if LLL_DEBUG_MODE
        lll_send_info("Command: mov",actVal[1]);
    #endif

    return 0;
}

uint8_t LLL_mul(void){
    lll_send_info("Command: mul",0);
}

uint8_t LLL_not(void){

    LLL_save(tokens[0],~actVal[1]);

    if(Zero){
        if(actVal[1] != 0xFF){
            Zero = 0;
        }
    }

    #if LLL_DEBUG_MODE
        lll_send_info("Command: not",(uint8_t)~actVal[1]);
    #endif

    return LLLF_Z;
}

uint8_t LLL_or(void){
    uint8_t or_v = actVal[1] | actVal[2];

    if(Zero){
        if(or_v){
            Zero=0;
        }
    }

    LLL_save(tokens[0],or_v);

    #if LLL_DEBUG_MODE
        lll_send_info("Command: or",or_v);
    #endif

    return LLLF_Z;
}

uint8_t LLL_out(void){
    lll_send_info("Command: out",0);
}

uint8_t LLL_pop(void){
    uint8_t p_v = lll_popStack();

    if(Zero){
        if(p_v){
            Zero=0;
        }
    }

    LLL_save(tokens[0],p_v);

    #if LLL_DEBUG_MODE
        lll_send_info("Command: pop",p_v);
    #endif

    return LLLF_Z;
}

uint8_t LLL_push(void){
    uint8_t p_v = actVal[0];

    if(Zero){
        if(p_v){
            Zero=0;
        }
    }

    lll_pushStack(p_v);

    #if LLL_DEBUG_MODE
        lll_send_info("Command: push",p_v);
    #endif

    return LLLF_Z;
}

uint8_t LLL_ret(void){
    uint64_t tmp=popStack8byte();
    lll_goTo(tmp);

    #if LLL_DEBUG_MODE
        lll_send_info("Command: ret",tmp);
    #endif

    return 0;
}

uint8_t LLL_rjmp(void){
    if(rv){
        pushStack8byte(lll_getPosition());
    }

    lll_goTo(ra+(int32_t)r_jmp);

    #if LLL_DEBUG_MODE
        lll_send_info("Command: rjmp",rv);
    #endif

    return 0;
}

uint8_t LLL_sub(void){
    lll_send_info("Command: sub",0);
}

uint8_t LLL_exit(void){
    lll_send_info("Exit",0);
}