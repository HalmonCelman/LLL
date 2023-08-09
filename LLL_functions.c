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
    lll_err inst_err;
    inst_err.status=LLL_OK;

    uint32_t lll_reg;
    uint8_t lll_ovf=0;
    uint16_t lll_sum=0;

    lll_number=lll_get();
    #if LLL_DEBUG_MODE
        LLL_CHECK_REG(inst_err);
    #else
        lll_h8=lll_get();
    #endif
    lll_reg=LLL_load_reg_addr(LLL_REG_MODE);

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("add skipped ",0);
        #endif
        lll_skip=0;
    }else{
        for(int i=0;i<lll_number;i++){
            lll_sum += LLL_load_mem(lll_reg+i);
            if(lll_sum > 255){
                lll_sum -= 255;
                lll_ovf=1;
            }    
        }

        #if LLL_DEBUG_MODE
            lll_send_info("add sum: ",lll_sum);
            lll_send_info("add ovf: ",(lll_ovf ? 255 : 0));
        #endif

        LLL_save_mem(lll_reg, (uint8_t)lll_sum);
        LLL_save_mem(LLL_getFlagNumber('O'), (lll_ovf ? 255 : 0));
    }
    return inst_err;
}

lll_err LLL_addi(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    uint32_t lll_reg;
    uint16_t lll_sum=0;

    #if LLL_DEBUG_MODE
        LLL_CHECK_REG(inst_err);        
    #else
        lll_h8=lll_get();
    #endif
    lll_reg=LLL_load_reg_addr(LLL_REG_MODE);
    lll_h8 = lll_get();

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("addi skipped ",0);
        #endif
        lll_skip=0;
    }else{
        lll_sum=LLL_load_mem(lll_reg)+lll_h8;

        if(lll_sum>255){
            lll_sum-=255;
            LLL_save_mem(LLL_getFlagNumber('O'), 255);
        }else{
            LLL_save_mem(LLL_getFlagNumber('O'), 0);
        }

        #if LLL_DEBUG_MODE
            lll_send_info("addi sum: ",lll_sum);
            lll_send_info("add ovf: ",((lll_sum>255) ? 255 : 0));
        #endif

        LLL_save_mem(lll_reg, (uint8_t)lll_sum);
    }
    return inst_err;
}

lll_err LLL_sub(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    uint32_t lll_reg;
    uint8_t lll_ovf=0;
    int16_t lll_sum;

    lll_number=lll_get();

    #if LLL_DEBUG_MODE
    LLL_CHECK_REG(inst_err);
    #else
    lll_h8 = lll_get();
    #endif
    lll_reg = LLL_load_reg_addr(LLL_REG_MODE);

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("sub skipped ",0);
        #endif
        lll_skip=0;
    }else{
        lll_sum = LLL_load_mem(lll_reg);

        for(int i=1;i<lll_number;i++){
            lll_sum -= LLL_load_mem(lll_reg + i);
            if(lll_sum < -255){
                lll_sum += 255;
                lll_ovf = 1;
            }
        }


        #if LLL_DEBUG_MODE
            lll_send_info("sub reg value: ",(lll_ovf ? (-lll_sum) : ((lll_sum < 0)? 0 : lll_sum)));
            lll_send_info("sub ovf: ",(lll_ovf ? 255 : ((lll_sum < 0)? (-lll_sum) : 0)));
        #endif

        LLL_save_mem(lll_reg,(lll_ovf ? (-lll_sum) : ((lll_sum < 0)? 0 : lll_sum)));
        LLL_save_mem(LLL_getFlagNumber('O'),(lll_ovf ? 255 : ((lll_sum < 0)? (-lll_sum) : 0)));
    }
    return inst_err;
}


lll_err LLL_subi(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    uint32_t lll_reg;
    int16_t lll_sum;

    #if LLL_DEBUG_MODE
    LLL_CHECK_REG(inst_err);
    #else
    lll_h8 = lll_get();
    #endif
    lll_reg = LLL_load_reg_addr(LLL_REG_MODE);
    lll_h8 = lll_get();

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("subi skipped ",0);
        #endif
        lll_skip=0;
    }else{
        lll_sum = LLL_load_mem(lll_reg) - lll_h8;

        #if LLL_DEBUG_MODE
            lll_send_info("subi reg value: ",((lll_sum < 0) ? 0 : lll_sum));
            lll_send_info("subi ovf: ",((lll_sum < 0) ? (-lll_sum) : 0));
        #endif

        LLL_save_mem(lll_reg,((lll_sum < 0) ? 0 : lll_sum));
        LLL_save_mem(LLL_getFlagNumber('O'),((lll_sum < 0) ? (-lll_sum) : 0));
    }
    return inst_err;
}

lll_err LLL_mul(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    uint32_t lll_reg;
    int32_t lll_res=1;

    lll_number=lll_get();

    #if LLL_DEBUG_MODE
    LLL_CHECK_REG(inst_err);
    #else
    lll_h8 = lll_get();
    #endif
    lll_reg = LLL_load_reg_addr(LLL_REG_MODE);

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("mul skipped ",0);
        #endif
        lll_skip=0;
    }else{
        for(int i=0;i<lll_number;i++){
            lll_res *= LLL_load_mem(lll_reg + i);
            if(lll_res > 65535){
                lll_res &= 0xFFFF; //cut to 16 bits
            }
        }

        #if LLL_DEBUG_MODE
            lll_send_info("mul reg value: ",(lll_res & 0xFF));
            lll_send_info("mul R: ",((lll_res & 0xFF00)>>8));
        #endif

        LLL_save_mem(lll_reg,(lll_res & 0xFF));
        LLL_save_mem(LLL_getFlagNumber('R'),((lll_res & 0xFF00)>>8));
    }
    return inst_err;
}


lll_err LLL_muli(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    uint32_t lll_reg;
    int16_t lll_res;

    #if LLL_DEBUG_MODE
    LLL_CHECK_REG(inst_err);
    #else
    lll_h8 = lll_get();
    #endif
    lll_reg = LLL_load_reg_addr(LLL_REG_MODE);
    lll_h8 = lll_get();

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("muli skipped ",0);
        #endif
        lll_skip=0;
    }else{
        lll_res = LLL_load_mem(lll_reg) * lll_h8;

        #if LLL_DEBUG_MODE
            lll_send_info("muli reg value: ",(lll_res & 0xFF));
            lll_send_info("muli R: ",((lll_res & 0xFF00)>>8));
        #endif

        LLL_save_mem(lll_reg,(lll_res & 0xFF));
        LLL_save_mem(LLL_getFlagNumber('R'),((lll_res & 0xFF00)>>8));
    }
    return inst_err;
}


lll_err LLL_div(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    uint32_t lll_reg;
    uint8_t lll_res;
    uint16_t lll_div=1;

    lll_number = lll_get();
    #if LLL_DEBUG_MODE
    LLL_CHECK_REG(inst_err);
    #else
    lll_h8 = lll_get();
    #endif
    lll_reg = LLL_load_reg_addr(LLL_REG_MODE);
    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("div skipped ",0);
        #endif
        lll_skip=0;
    }else{
        lll_res = LLL_load_mem(lll_reg);

        for(int i=1;i<lll_number-1;i++){
            lll_div *= LLL_load_mem(lll_reg+i);
            if(lll_div > lll_res){
                #if LLL_DEBUG_MODE
                    lll_send_info("div reg value: ",0);
                    lll_send_info("div R: ",lll_res);
                #endif

                LLL_save_mem(lll_reg,0);
                LLL_save_mem(LLL_getFlagNumber('R'),lll_res);
                
                return inst_err;
            }
        }

        lll_res /= lll_div;
        lll_div = LLL_load_mem(lll_reg+lll_number-1);

        #if LLL_DEBUG_MODE
            lll_send_info("div reg value: ",lll_res / lll_div);
            lll_send_info("div R: ",lll_res % lll_div);
        #endif

        LLL_save_mem(lll_reg,lll_res / lll_div);
        LLL_save_mem(LLL_getFlagNumber('R'),lll_res % lll_div);
    }
    return inst_err;
}


lll_err LLL_divi(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    uint32_t lll_reg;
    uint8_t lll_res;
    uint16_t lll_div;

    #if LLL_DEBUG_MODE
    LLL_CHECK_REG(inst_err);
    #else
    lll_h8 = lll_get();
    #endif
    lll_reg = LLL_load_reg_addr(LLL_REG_MODE);
    lll_div = lll_get();

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("divi skipped ",0);
        #endif
        lll_skip=0;
    }else{
        lll_res = LLL_load_mem(lll_reg);

        #if LLL_DEBUG_MODE
            lll_send_info("divi reg value: ",lll_res / lll_div);
            lll_send_info("divi R: ",lll_res % lll_div);
        #endif

        LLL_save_mem(lll_reg,lll_res / lll_div);
        LLL_save_mem(LLL_getFlagNumber('R'),lll_res % lll_div);
    }
    return inst_err;
}

lll_err LLL_and(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    uint8_t lll_res=0xFF;
    uint32_t lll_reg;

    lll_number=lll_get();
    #if LLL_DEBUG_MODE
    LLL_CHECK_REG(inst_err);
    #else
    lll_h8 = lll_get();
    #endif
    lll_reg = LLL_load_reg_addr(LLL_REG_MODE);

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("and skipped ",0);
        #endif
        lll_skip=0;
    }else{
        for(int i=0;i<lll_number;i++){
            lll_res &= LLL_load_mem(lll_reg + i);
        }

        #if LLL_DEBUG_MODE
            lll_send_info("and reg value: ",lll_res);
        #endif

        LLL_save_mem(lll_reg,lll_res);
    }

    return inst_err;
}


lll_err LLL_andi(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    uint8_t lll_res;
    uint32_t lll_reg;

    #if LLL_DEBUG_MODE
    LLL_CHECK_REG(inst_err);
    #else
    lll_h8 = lll_get();
    #endif
    lll_reg = LLL_load_reg_addr(LLL_REG_MODE);
    lll_h8 = lll_get();

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("andi skipped ",0);
        #endif
        lll_skip=0;
    }else{
        lll_res= LLL_load_mem(lll_reg) & lll_h8;

        #if LLL_DEBUG_MODE
            lll_send_info("andi reg value: ",lll_res);
        #endif

        LLL_save_mem(lll_reg,lll_res);
    }
    return inst_err;
}


lll_err LLL_or(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    uint8_t lll_res=0;
    uint32_t lll_reg;

    lll_number=lll_get();

    #if LLL_DEBUG_MODE
    LLL_CHECK_REG(inst_err);
    #else
    lll_h8 = lll_get();
    #endif
    lll_reg = LLL_load_reg_addr(LLL_REG_MODE);

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("or skipped ",0);
        #endif
        lll_skip=0;
    }else{
        for(int i=0;i<lll_number;i++){
            lll_res |= LLL_load_mem(lll_reg + i);
        }

        #if LLL_DEBUG_MODE
            lll_send_info("or reg value: ",lll_res);
        #endif

        LLL_save_mem(lll_reg,lll_res);
    }
    return inst_err;
}


lll_err LLL_ori(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    uint8_t lll_res;
    uint32_t lll_reg;

    #if LLL_DEBUG_MODE
    LLL_CHECK_REG(inst_err);
    #else
    lll_h8 = lll_get();
    #endif
    lll_reg = LLL_load_reg_addr(LLL_REG_MODE);
    lll_h8 = lll_get();

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("ori skipped ",0);
        #endif
        lll_skip=0;
    }else{
        lll_res = LLL_load_mem(lll_reg) | lll_h8;

        #if LLL_DEBUG_MODE
            lll_send_info("ori reg value: ",lll_res);
        #endif

        LLL_save_mem(lll_reg,lll_res);
    }
    return inst_err;
}


lll_err LLL_not(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    uint32_t lll_reg;
    uint8_t lll_res;

    #if LLL_DEBUG_MODE
    LLL_CHECK_REG(inst_err);
    #else
    lll_h8 = lll_get();
    #endif
    lll_reg = LLL_load_reg_addr(LLL_REG_MODE);

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("not skipped ",0);
        #endif
        lll_skip=0;
    }else{
        lll_res = ~LLL_load_mem(lll_reg);

        #if LLL_DEBUG_MODE
            lll_send_info("not reg value: ",lll_res);
        #endif

        LLL_save_mem(lll_reg,lll_res);
    }
    return inst_err;
}


lll_err LLL_inc(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    uint32_t lll_reg;
    uint8_t lll_res;
    uint8_t lll_ovf=0;

    #if LLL_DEBUG_MODE
    LLL_CHECK_REG(inst_err);
    #else
    lll_h8 = lll_get();
    #endif
    lll_reg = LLL_load_reg_addr(LLL_REG_MODE);

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("inc skipped ",0);
        #endif
        lll_skip=0;
    }else{
        lll_res = LLL_load_mem(lll_reg);
        if(lll_res == 255 ){
            lll_ovf = 1;
            lll_res = 0;
        }else{
            lll_res++;
        }
        #if LLL_DEBUG_MODE
            lll_send_info("inc reg value: ",lll_res);
            lll_send_info("inc ovf value: ",lll_ovf);
        #endif

        LLL_save_mem(lll_reg,lll_res);
        LLL_save_mem(LLL_getFlagNumber('O'),lll_ovf);
    }
    return inst_err;
}


lll_err LLL_dec(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    uint32_t lll_reg;
    uint8_t lll_res;
    uint8_t lll_ovf=0;

    #if LLL_DEBUG_MODE
    LLL_CHECK_REG(inst_err);
    #else
    lll_h8 = lll_get();
    #endif
    lll_reg = LLL_load_reg_addr(LLL_REG_MODE);
    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("dec skipped ",0);
        #endif
        lll_skip=0;
    }else{
        lll_res = LLL_load_mem(lll_reg);
        if(lll_res == 0 ){
            lll_ovf = 1;
            lll_res = 0;
        }else{
            lll_res--;
        }
        #if LLL_DEBUG_MODE
            lll_send_info("dec reg value: ",lll_res);
            lll_send_info("dec ovf value: ",lll_ovf);
        #endif

        LLL_save_mem(lll_reg,lll_res);
        LLL_save_mem(LLL_getFlagNumber('O'),lll_ovf);
    }
    return inst_err;
}


lll_err LLL_ser(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;
    uint8_t lll_value;
    uint32_t lll_reg1,lll_reg2;

    lll_number=lll_get();

    #if LLL_DEBUG_MODE
    LLL_CHECK_REG(inst_err);
    #else
    lll_h8 = lll_get();
    #endif

    lll_reg1 = LLL_load_reg_addr(LLL_REG_MODE);

    #if LLL_DEBUG_MODE
    LLL_CHECK_REG(inst_err);
    #else
    lll_h8 = lll_get();
    #endif
    lll_reg2 = LLL_load_reg_addr(LLL_REG_MODE);

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("ser skipped ",0);
        #endif
        lll_skip=0;
    }else{
        for(int i=0;i<lll_number;i++){
            lll_value=LLL_load_mem(lll_reg2+i);
            LLL_save_mem(lll_reg1+i,lll_value);
            #if LLL_DEBUG_MODE
                char tmpbuff[10]={"          "};
                copyString(tmpbuff,"ser ");
                copyString(&tmpbuff[7],": ");
                tmpbuff[9]=0;
                itoa(lll_reg1+i,&tmpbuff[4]);
                lll_send_info(tmpbuff,lll_value);
            #endif
        }
    }

    return inst_err;
}


lll_err LLL_seri(void){
lll_err inst_err;

uint32_t lll_reg; //adress of register

inst_err.status=LLL_OK;

#if LLL_DEBUG_MODE==1
    lll_number=lll_get(); //load number
    lll_send_info("seri number: ",lll_number); //send debug info
    LLL_CHECK_REG(inst_err); //check if properly written
    lll_send_info("seri reg mode: ",LLL_REG_MODE); //send debug info
    lll_reg=LLL_load_reg_addr(LLL_REG_MODE); //load register adress
    lll_send_info("seri adress: ",lll_reg); //send debug info
#else
    lll_number=lll_get(); //load number
    lll_h8=lll_get(); //load mode of register
    lll_reg=LLL_load_reg_addr(LLL_REG_MODE); //load adress
#endif // LLL_DEBUG_MODE
    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("seri skipped ",0);
        #endif
        for(int i=0;i<lll_number;i++) lll_get();
        lll_skip=0;
    }else{
        for(int i=0;i<lll_number;i++){
            lll_h8=lll_get();
            #if LLL_DEBUG_MODE
                lll_send_info("seri value: ",lll_h8);
            #endif
            LLL_save_mem(lll_reg+i,lll_h8);
        }
    }
return inst_err;
}

lll_err LLL_ljmp(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    uint32_t lll_label_number=LLL_get32bit();

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("ljmp skipped ",0);
        #endif
        lll_skip=0;
    }else{
    uint64_t lll_label_adress=lll_get_label(lll_label_number);

    lll_last_jump=lll_getPosition();
    lll_goTo(lll_label_adress);

    #if LLL_DEBUG_MODE
        lll_send_info("ljmp label number: ",lll_label_number);
        lll_send_info("ljmp jump position: ",lll_label_adress);
        lll_send_info("ljmp last position: ",lll_last_jump);
    #endif
    }
    return inst_err;
}


lll_err LLL_jmp(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    uint64_t lll_jump=LLL_get64bit();

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("jmp skipped ",0);
        #endif
        lll_skip=0;
    }else{
        lll_last_jump=lll_getPosition();
        lll_goTo(lll_jump);

        #if LLL_DEBUG_MODE
            lll_send_info("jmp jump position: ",lll_jump);
            lll_send_info("jmp last jump position: ",lll_last_jump);
        #endif
    }
    return inst_err;
}


lll_err LLL_ret(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("ret skipped ",0);
        #endif
        lll_skip=0;
    }else{
    if(lll_last_jump != 0){
        lll_goTo(lll_last_jump);
    }

    #if LLL_DEBUG_MODE
        lll_send_info(lll_last_jump ? "ret to: " : "ret ignored ",lll_last_jump);
    #endif
    }
    return inst_err;
}


lll_err LLL_cmp(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;
    
    uint32_t lll_reg1,lll_reg2;
    uint8_t val1,val2;

    #if LLL_DEBUG_MODE
        LLL_CHECK_REG(inst_err);
        lll_reg1=LLL_load_reg_addr(LLL_REG_MODE);
        LLL_CHECK_REG(inst_err);
        lll_reg2=LLL_load_reg_addr(LLL_REG_MODE);
    #else
        lll_h8=lll_get();
        lll_reg1=LLL_load_reg_addr(LLL_REG_MODE);
        lll_h8=lll_get();
        lll_reg2=LLL_load_reg_addr(LLL_REG_MODE);
    #endif
    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("cmp skipped ",0);
        #endif
        lll_skip=0;
    }else{
        val1=LLL_load_mem(lll_reg1);
        val2=LLL_load_mem(lll_reg2);
        
        LLL_save_mem(LLL_getFlagNumber('R'),((val1>val2)?COMP_GR:((val1<val2)?COMP_LO:COMP_EQ))); //if > then 2, < then 0, == then 1

        #if LLL_DEBUG_MODE
            lll_send_info("cmp val1: ",val1);
            lll_send_info("cmp val2: ",val2);
            lll_send_info("cmp R: ",((val1>val2)?COMP_GR:((val1<val2)?COMP_LO:COMP_EQ)));
        #endif
    }
    return inst_err;
}


lll_err LLL_cmpi(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    uint32_t lll_reg;
    uint8_t val1,val2;

    #if LLL_DEBUG_MODE
        LLL_CHECK_REG(inst_err);
    #else
        lll_h8=lll_get();
    #endif
    lll_reg=LLL_load_reg_addr(LLL_REG_MODE);
    val2=lll_get();

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("cmpi skipped ",0);
        #endif
        lll_skip=0;
    }else{
        val1=LLL_load_mem(lll_reg);
        
        LLL_save_mem(LLL_getFlagNumber('R'),((val1>val2)?COMP_GR:((val1<val2)?COMP_LO:COMP_EQ))); //if > then 2, < then 0, == then 1

        #if LLL_DEBUG_MODE
            lll_send_info("cmpi val1: ",val1);
            lll_send_info("cmpi val2: ",val2);
            lll_send_info("cmpi R: ",((val1>val2)?COMP_GR:((val1<val2)?COMP_LO:COMP_EQ)));
        #endif
    }
    return inst_err;
}


lll_err LLL_seq(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("seq skipped ",0);
        #endif
        lll_skip=0;
    }else{
        if(LLL_load_mem(LLL_getFlagNumber('R'))==COMP_EQ){
            lll_skip=1;
        }
    }
    return inst_err;
}


lll_err LLL_deq(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("deq skipped ",0);
        #endif
        lll_skip=0;
    }else{
        if(LLL_load_mem(LLL_getFlagNumber('R'))!=COMP_EQ){
            lll_skip=1;
        }
    }
    return inst_err;
}


lll_err LLL_slo(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("slo skipped ",0);
        #endif
        lll_skip=0;
    }else{
        if(LLL_load_mem(LLL_getFlagNumber('R'))==COMP_LO){
            lll_skip=1;
        }
    }
    return inst_err;
}


lll_err LLL_dlo(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("dlo skipped ",0);
        #endif
        lll_skip=0;
    }else{
        if(LLL_load_mem(LLL_getFlagNumber('R'))!=COMP_LO){
            lll_skip=1;
        }
    }
    return inst_err;
}


lll_err LLL_sgr(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("sgr skipped ",0);
        #endif
        lll_skip=0;
    }else{
        if(LLL_load_mem(LLL_getFlagNumber('R'))==COMP_GR){
            lll_skip=1;
        }
    }
    return inst_err;
}


lll_err LLL_dgr(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("dgr skipped ",0);
        #endif
        lll_skip=0;
    }else{
        if(LLL_load_mem(LLL_getFlagNumber('R'))!=COMP_GR){
            lll_skip=1;
        }
    }
    return inst_err;
}


lll_err LLL_in(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;
    
    uint32_t lll_reg;

    #if LLL_DEBUG_MODE
        LLL_CHECK_REG(inst_err);
        lll_send_info("in command ",LLL_load_mem(LLL_getFlagNumber('A')));
    #else
        lll_h8=lll_get();
    #endif
    
    lll_reg=LLL_load_reg_addr(LLL_REG_MODE);

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("in skipped ",0);
        #endif
        lll_skip=0;
    }else{
        inst_err = lll_stream_in(lll_reg,LLL_load_mem(LLL_getFlagNumber('A')));
    }

    return inst_err;
}



lll_err LLL_out(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;
    
    uint32_t lll_reg;

    #if LLL_DEBUG_MODE
        LLL_CHECK_REG(inst_err);
        lll_send_info("out command ",LLL_load_mem(LLL_getFlagNumber('A')));
    #else
        lll_h8=lll_get();
    #endif
    
    lll_reg=LLL_load_reg_addr(LLL_REG_MODE);

    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("out skipped ",0);
        #endif
        lll_skip=0;
    }else{
        inst_err = lll_stream_out(lll_reg,LLL_load_mem(LLL_getFlagNumber('A')));
    }

    return inst_err;
}

lll_err LLL_exit(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;
    
    if(lll_skip){
        #if LLL_DEBUG_MODE
            lll_send_info("in skipped ",0);
        #endif
        lll_skip=0;
    }else{
        inst_err.status=LLL_EOP;
        #if LLL_DEBUG_MODE
            lll_send_info("exit: ",0);
        #endif
    }
    return inst_err;
}