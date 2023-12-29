#include "LLL.h"
#include "LLL_functions.h"

uint8_t LLL_FAST_MEM[LLL_FAST_MEM_SIZE];
uint8_t LLL_STACK[LLL_STACK_SIZE];

stackp_s stack_pointer;
uint8_t status_register;
uint8_t globalCarry=0;
uint8_t firstTime=1;

// command section
#define COMMAND_MAP_LENGTH 18
const command_p lll_command_map[COMMAND_MAP_LENGTH]={
    LLL_add,
    LLL_and,
    LLL_cmp,
    LLL_dec,
    LLL_frjmp,
    LLL_in,
    LLL_inc,
    LLL_jmp,
    LLL_mov,
    LLL_mul,
    LLL_not,
    LLL_or,
    LLL_out,
    LLL_pop,
    LLL_push,
    LLL_ret,
    LLL_rjmp,
    LLL_sub
};

// number of tokens WITHOUT command itself
const uint8_t lll_param_num[COMMAND_MAP_LENGTH]={
    3,      // ADD
    3,      // AND
    2,      // CMP
    1,      // DEC
    2,      // FRJMP
    2,      // IN
    1,      // INC
    2,      // JMP
    2,      // MOV
    3,      // MUL
    2,      // NOT
    3,      // OR
    2,      // OUT
    1,      // POP
    1,      // PUSH
    0,      // RET
    2,      // RJMP
    3       // SUB
};
// end of section


//the most important function - executes one lll function
lll_err LLL_exec(void){
    lll_err exec_err;
    exec_err.status=LLL_OK; 

    uint8_t lll_c; //command
    uint8_t lll_opt; //and options

    if(firstTime){
        lll_c = lll_get();
        firstTime=0;
    }else{
        lll_c = globalCarry;
    }

    lll_opt = lll_c & 0xC0;
    lll_c   = lll_c & 0x3F;

    if(lll_c == LLL_EXIT){
        exec_err.status = LLL_EOP;
        LLL_execute_command(lll_c,LLL_exit,0,lll_opt);
    }else{
        if(lll_c < COMMAND_MAP_LENGTH){
            LLL_execute_command(lll_c,lll_command_map[lll_c],lll_param_num[lll_c],lll_opt);
        }else{
            exec_err.status = LLL_NO_COMMAND;
            exec_err.additional = lll_c;
        }
    }
    return exec_err;
}

void LLL_init(void){
    #if LLL_USE_EXTERNAL_MEMORY
        lll_init_cache();
        lll_init_external_memory();
    #endif
}

void LLL_end(void){
    #if LLL_USE_EXTERNAL_MEMORY
        lll_close_external_memory();
        lll_remove_cache();
    #endif
}

void LLL_run(char * name){
    uint8_t res=0;
    #if LLL_DEBUG_MODE
        lll_send_info(name,0);
    #endif
    res=lll_init_program(name,0);
    lll_throw_error(res,"Failed to run program!",0);

    if(!res){ //if initialized properly
    while(!res){
        res=LLL_exec().status;
    }
    if(res != LLL_EOP){
        lll_send_info("ERROR EXEC: ",res);
        lll_throw_error(res,"ERROR :",0);
    }
    lll_end_program();
    }
}

uint8_t LLL_load_mem(uint32_t adress){
     #if LLL_USE_EXTERNAL_MEMORY    
        if(adress<LLL_FAST_MEM_SIZE+LLL_FLAG_NUMBER){
            return LLL_FAST_MEM[adress];
        }else{
            return lll_external_mem_read(adress);
        }
    #else
        return LLL_FAST_MEM[adress];
    #endif
}

void LLL_save_mem(uint32_t adress, uint8_t value){ 
    #if LLL_USE_EXTERNAL_MEMORY    
        if(adress<LLL_FAST_MEM_SIZE+LLL_FLAG_NUMBER){
            LLL_FAST_MEM[adress]=value;
        }else{
        lll_external_mem_write(adress,value);
        }
    #else
        lll_send_info("SA: ",adress);
        lll_send_info("SV: ",value);
        
        LLL_FAST_MEM[adress]=value;
    #endif
}


