#include "LLL.h"
#include "LLL_functions.h"

///declarations of buffers due to gcc 12 
uint8_t LLL_FAST_MEM[LLL_FAST_MEM_SIZE+LLL_FLAG_NUMBER];
uint8_t LLL_COMM_BUFF[LLL_COMM_BUFF_SIZE];
uint64_t LLL_LABEL[LLL_LABEL_NUMBER];
const uint8_t LLL_FLAG_MAP[LLL_FLAG_NUMBER]={'O','A','R'};

volatile uint8_t lll_c;
volatile uint8_t lll_h8;
volatile uint8_t lll_number;

//the most important function - executes one lll function
lll_err LLL_exec(void){
    lll_err exec_err; //error status

    exec_err.status=LLL_OK; //default - OK

    lll_c=lll_get(); //read command
    if(LLL_CHECK_LABEL(lll_c)){
        lll_set_label(LLL_get32bit());
        lll_c=lll_get(); // read once more
    }
    #if LLL_DEBUG_MODE==1
        lll_send_info("MainExec command: ",lll_c); //send debug info
    #endif // LLL_DEBUG_MODE

    switch(lll_c){ //execute
        case LLL_ADD:  exec_err=LLL_add();  break;
        case LLL_ADDI: exec_err=LLL_addi(); break;
        case LLL_SUB:  exec_err=LLL_sub();  break;
        case LLL_SUBI: exec_err=LLL_subi(); break;
        case LLL_MUL:  exec_err=LLL_mul();  break;
        case LLL_MULI: exec_err=LLL_muli(); break;
        case LLL_DIV:  exec_err=LLL_div();  break;
        case LLL_DIVI: exec_err=LLL_divi(); break;
        case LLL_AND:  exec_err=LLL_and();  break;
        case LLL_ANDI: exec_err=LLL_andi(); break;
        case LLL_OR:   exec_err=LLL_or();   break;
        case LLL_ORI:  exec_err=LLL_ori();  break;
        case LLL_NOT:  exec_err=LLL_not();  break;
        case LLL_INC:  exec_err=LLL_inc();  break;
        case LLL_DEC:  exec_err=LLL_dec();  break;
        case LLL_SER:  exec_err=LLL_ser();  break;
        case LLL_SERI: exec_err=LLL_seri(); break;
        case LLL_LJMP: exec_err=LLL_ljmp(); break;
        case LLL_JMP:  exec_err=LLL_jmp();  break;
        case LLL_RET:  exec_err=LLL_ret();  break;
        case LLL_CMP:  exec_err=LLL_cmp();  break;
        case LLL_CMPI: exec_err=LLL_cmpi(); break;
        case LLL_SEQ:  exec_err=LLL_seq();  break;
        case LLL_DEQ:  exec_err=LLL_deq();  break;
        case LLL_SLO:  exec_err=LLL_slo();  break;
        case LLL_DLO:  exec_err=LLL_dlo();  break;
        case LLL_SGR:  exec_err=LLL_sgr();  break;
        case LLL_DGR:  exec_err=LLL_dgr();  break;
        case LLL_IN:   exec_err=LLL_in();   break;
        case LLL_OUT:  exec_err=LLL_out();  break;
        case LLL_EXIT: exec_err=LLL_exit(); break;
        default:
            ///if no command executed
            exec_err.status = LLL_NO_COMMAND; //command not found
            exec_err.additional = lll_c; //which command wasn't found
        break;
    }

    return exec_err;
}





//returns flag number(which is also it's adress)
uint8_t LLL_getFlagNumber(char flag){
    for(int i=0;i<LLL_FLAG_NUMBER;i++){ //find flag adress
        if(LLL_FLAG_MAP[i]==flag){
            return i;
        }
    }
    #if LLL_DEBUG_MODE
        lll_throw_error(1,"INVALID FLAG ",0);
    #endif
    return 0;
}

/*
LLL_get32bit: gets 32bit value from program
returns this value
*/
uint32_t LLL_get32bit(void){
    uint32_t val32=0;
    for(int i=0;i<4;i++){ //read adress
        val32 = (val32<<8) + lll_get();
    }
    return val32;
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
    res=lll_init_main_program(name,0);
    lll_throw_error(res,"Failed to run program!",0);

    if(!res){ //if initialized properly
    while(!res){
        res=LLL_exec().status;
    }
    if(res != LLL_EOP){
        lll_send_info("ERROR EXEC: ",res);
        lll_throw_error(res,"ERROR :",0);
    }
    lll_end_main_program();
    }
}




uint32_t LLL_load_reg_addr(uint8_t mode){
uint32_t lll_reg=0;
    if(mode==0){ //if normal mode
        lll_reg=LLL_get32bit();
        lll_reg+=LLL_FLAG_NUMBER;
    }
    if(mode==1){ //if flag mode
        return LLL_getFlagNumber(lll_get());
    }
    if(mode==2){ //if indirect mode
        uint32_t lll_pom=LLL_get32bit()+LLL_FLAG_NUMBER;
        for(int i=0;i<4;i++){ //get direct adress
            lll_reg = (lll_reg<<8) + LLL_load_mem(lll_pom+i);
        }
        lll_reg+=LLL_FLAG_NUMBER;
    }
    return lll_reg;
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