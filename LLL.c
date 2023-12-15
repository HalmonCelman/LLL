#include "LLL.h"
#include "LLL_functions.h"

uint8_t LLL_FAST_MEM[LLL_FAST_MEM_SIZE];
uint8_t LLL_STACK[LLL_STACK_SIZE];

uint32_t stack_pointer;
uint8_t status_register;
uint8_t globalCarry=0;

//the most important function - executes one lll function
lll_err LLL_exec(void){
    lll_err exec_err;
    exec_err.status=LLL_OK; 

    uint8_t lll_c; //command
    uint8_t lll_opt; //and options

    if(globalCarry){
        lll_c = globalCarry;
    }else{
        lll_c = lll_get();
    }
    
    lll_opt = lll_c & 0xC0;
    lll_c   = lll_c & 0x3F;

    #if LLL_DEBUG_MODE==1
        lll_send_info("MainExec command: ",lll_c); //send debug info
    #endif // LLL_DEBUG_MODE

    switch(lll_c){ //execute
        case LLL_ADD:   exec_err=LLL_add();     break;
        case LLL_AND:   exec_err=LLL_and();     break;
        case LLL_CMP:   exec_err=LLL_cmp();     break;
        case LLL_DEC:   exec_err=LLL_dec();     break;
        case LLL_DIV:   exec_err=LLL_div();     break;
        case LLL_FRJMP: exec_err=LLL_frjmp();   break;
        case LLL_IN:    exec_err=LLL_in();      break;
        case LLL_INC:   exec_err=LLL_inc();     break;
        case LLL_JMP:   exec_err=LLL_jmp();     break;
        case LLL_MOV:   exec_err=LLL_mov();     break;
        case LLL_MUL:   exec_err=LLL_mul();     break;
        case LLL_NOT:   exec_err=LLL_not();     break;
        case LLL_OR:    exec_err=LLL_or();      break;
        case LLL_OUT:   exec_err=LLL_out();     break;
        case LLL_RET:   exec_err=LLL_ret();     break;
        case LLL_RJMP:  exec_err=LLL_rjmp();    break;
        case LLL_SUB:   exec_err=LLL_sub();     break;
        case LLL_EXIT:  exec_err=LLL_exit();    break;
        default:
            ///if no command executed
            exec_err.status = LLL_NO_COMMAND; //command not found
            exec_err.additional = lll_c; //which command wasn't found
        break;
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
        LLL_FAST_MEM[adress]=value;
    #endif
}


// processing section
static uint32_t LLL_load_mem_32(uint32_t address){
    uint32_t tmpValue = LLL_load_mem(address);
    tmpValue = (tmpValue<<8)+LLL_load_mem(address+1);
    tmpValue = (tmpValue<<8)+LLL_load_mem(address+2);
    tmpValue = (tmpValue<<8)+LLL_load_mem(address+3);
    return tmpValue;
}

static lll_param LLL_getHalfParam(uint8_t firstChar){
    lll_param tmpParam;
    
    switch(firstChar){
        case 'r':
            tmpParam.type=mem;
            tmpParam.value1=lll_get();
            break;
        case '&':
            tmpParam.type=mem;
            tmpParam.value1=LLL_get32bit();
            break;
        case '%':
            tmpParam.type=flag;
            tmpParam.value1=lll_get();
            break;
        case '*':
            tmpParam.type=mem;
            tmpParam.value1=LLL_load_mem_32(LLL_get32bit());
            break;
        case '@':
            tmpParam.type=cst;
            tmpParam.value1=lll_get();
        default:
        lll_throw_error(1,"ERROR: Wrong parameter",firstChar);
    }

    tmpParam.value2=0;
    tmpParam.carry=0;

    return tmpParam;
}

lll_param LLL_getParam(uint8_t carry){
    lll_param tmpParam;

    uint8_t lll_tmp;
    if(carry){
        lll_tmp=carry;
    }else{
        lll_tmp=lll_get();
    }

    tmpParam = LLL_getHalfParam(lll_tmp);

    lll_tmp=lll_get();
    if(lll_tmp == '-'){
        tmpParam.type = range;
        tmpParam.value2 = LLL_getHalfParam(lll_get()).value1;
    }else{
        tmpParam.carry=lll_tmp;
    }

    return tmpParam;
}

uint32_t LLL_get32bit(void){
    uint32_t val32=0;
    for(int i=0;i<4;i++){ //read adress
        val32 = (val32<<8) + lll_get();
    }
    return val32;
}

uint64_t LLL_get64bit(void){
    uint64_t val64=0;
    for(int i=0;i<8;i++){ //read adress
        val64 = (val64<<8) + lll_get();
    }
    return val64;
}