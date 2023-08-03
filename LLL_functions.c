///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////                                                              LLL instructions - by KK                                                              //////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "LLL_functions.h"

lll_err LLL_add(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    uint32_t lll_reg;
    uint8_t lll_ovf=0;
    uint16_t lll_sum=0;

    #if LLL_DEBUG_MODE
        lll_number=lll_get();
        lll_send_info("add number: ",lll_number);
        LLL_CHECK_REG(inst_err);
        lll_send_info("add reg mode: ",LLL_REG_MODE);
        lll_reg = LLL_load_reg_addr(LLL_REG_MODE);
    #else
        lll_number=lll_get();
        lll_h8=lll_get();
        lll_reg1=LLL_load_reg_addr(LLL_REG_MODE);
    #endif

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

    return inst_err;
}

lll_err LLL_addi(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    uint32_t lll_reg;
    uint16_t lll_sum=0;

    #if LLL_DEBUG_MODE
        LLL_CHECK_REG(inst_err);
        lll_send_info("addi reg mode: ",LLL_REG_MODE);
        lll_reg=LLL_load_reg_addr(LLL_REG_MODE);
        lll_sum=LLL_load_mem(lll_reg)+lll_get();
        lll_send_info("addi sum: ",((lll_sum>256) ? (lll_sum-255) : lll_sum));
        lll_send_info("add ovf: ",((lll_sum>256) ? 255 : 0));
    #else
        lll_h8=lll_get();
        lll_reg=LLL_load_reg_addr(LLL_REG_MODE);
        lll_sum=LLL_load_mem(lll_reg)+lll_get();
    #endif

    if(lll_sum>255){
        lll_sum-=255;
        LLL_save_mem(LLL_getFlagNumber('O'), 255);
    }else{
        LLL_save_mem(LLL_getFlagNumber('O'), 0);
    }
    LLL_save_mem(lll_reg, (uint8_t)lll_sum);

    return inst_err;
}

lll_err LLL_sub(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    uint8_t lll_reg;
    uint8_t lll_ovf=0;
    int16_t lll_sum;

    lll_number=lll_get();

    #if LLL_DEBUG_MODE
    lll_send_info("sub number: ",lll_number);
    LLL_CHECK_REG(inst_err);
    lll_send_info("sub reg mode: ",LLL_REG_MODE);
    lll_reg = LLL_load_reg_addr(LLL_REG_MODE);
    #else
    lll_h8 = lll_get();
    lll_reg = LLL_load_reg_addr(LLL_REG_MODE);
    #endif

    lll_sum = LLL_load_mem(lll_reg);

    for(int i=1;i<lll_number;i++){
        lll_sum -= LLL_load_mem(lll_reg + i);
        if(lll_sum < -255){
            lll_sum += 255;
            lll_ovf = 1;
        }
    }


    #if LLL_DEBUG_MODE
        lll_send_info("sub reg value: ",(lll_ovf ? lll_sum : ((lll_sum < 0)? 0 : lll_sum)));
        lll_send_info("sub ovf: ",(lll_ovf ? 255 : ((lll_sum < 0)? (-lll_sum) : 0)));
    #endif

    LLL_save_mem(lll_reg,(lll_ovf ? lll_sum : ((lll_sum < 0)? 0 : lll_sum)));
    LLL_save_mem(LLL_getFlagNumber('O'),(lll_ovf ? 255 : ((lll_sum < 0)? (-lll_sum) : 0)));

    return inst_err;
}


lll_err LLL_subi(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;

    uint8_t lll_reg;
    int16_t lll_sum;

    #if LLL_DEBUG_MODE
    LLL_CHECK_REG(inst_err);
    lll_send_info("subi reg mode: ",LLL_REG_MODE);
    lll_reg = LLL_load_reg_addr(LLL_REG_MODE);
    #else
    lll_h8 = lll_get();
    lll_reg = LLL_load_reg_addr(LLL_REG_MODE);
    #endif

    lll_sum = LLL_load_mem(lll_reg) - lll_get();

    #if LLL_DEBUG_MODE
        lll_send_info("subi reg value: ",((lll_sum < 0) ? 0 : lll_sum));
        lll_send_info("subi ovf: ",((lll_sum < 0) ? (-lll_sum) : 0));
    #endif

    LLL_save_mem(lll_reg,((lll_sum < 0) ? 0 : lll_sum));
    LLL_save_mem(LLL_getFlagNumber('O'),((lll_sum < 0) ? (-lll_sum) : 0));

    return inst_err;
}

lll_err LLL_seri(void){
lll_err inst_err;

uint32_t lll_reg; //adress of register

inst_err.status=LLL_OK;

//main instruction code

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
for(int i=0;i<lll_number;i++){
    lll_h8=lll_get();
    #if LLL_DEBUG_MODE
        lll_send_info("seri value: ",lll_h8);
    #endif
    LLL_save_mem(lll_reg+i,lll_h8);
}
/////////////////////////////////////////////
return inst_err;
}

lll_err LLL_out(void){
    lll_err inst_err;
    inst_err.status=LLL_OK;
    
    inst_err = lll_stream_out(LLL_get32bit(),LLL_load_mem(LLL_getFlagNumber('A')));

    return inst_err;
}