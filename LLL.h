#ifndef LLL_TINY_H_INCLUDED
#define LLL_TINY_H_INCLUDED

///include
#include <stdint.h>
#include "LLL_conf.h"

///Avialibe errors

typedef enum{
    LLL_OK = 0,
    LLL_NO_COMMAND,
    LLL_UNKNOWN_TYPE,
    LLL_EXPECTED_SYMBOL,
    LLL_WRONG_STREAM,
    LLL_EOP //end of program
}  lll_err_status ;

typedef enum{
    LLL_ADD = 0,
    LLL_AND,
    LLL_CMP,
    LLL_DEC,
    LLL_DIV,
    LLL_FRJMP,
    LLL_IN,
    LLL_INC,
    LLL_JMP,
    LLL_MOV,
    LLL_MUL,
    LLL_NOT,
    LLL_OR,
    LLL_OUT,
    LLL_RET,
    LLL_RJMP,
    LLL_SUB,
    LLL_EXIT=63
} lll_command_list;

///error stream
typedef struct{
    lll_err_status status; //actual status returned
    uint8_t additional; //additional information about error
} lll_err;

typedef struct{
    uint8_t  type;
    uint64_t value1;
    uint64_t value2;
    uint8_t carry;
} lll_param;

typedef enum{
    mem,        // Rx,&x,*x
    flag,       // %x
    cst,        // @x
    range,      // y-x
} parameterType;

///memory
extern uint8_t LLL_FAST_MEM[LLL_FAST_MEM_SIZE];
extern uint8_t LLL_STACK[LLL_STACK_SIZE];

///global varialibes
extern uint32_t stack_pointer;
extern uint8_t status_register;
extern uint8_t globalCarry;

///functions

void LLL_init(void);
void LLL_end(void);
void LLL_run(char * name);
lll_err LLL_exec(void); //execute command
uint32_t LLL_load_reg_addr(uint8_t); //load adress of register - MODE: 0 - normal register adress &, 1 - flag %, 2 - indirect *, returns adress
uint8_t LLL_load_mem(uint32_t);
void LLL_save_mem(uint32_t,uint8_t);

//processing
lll_param LLL_getParam(uint8_t carry);
uint32_t LLL_get32bit(void);
uint64_t LLL_get64bit(void);

//driver functions - this functions should be specified in driver for device
#if LLL_USE_EXTERNAL_MEMORY
    extern void lll_init_external_memory(void);
    extern void lll_external_mem_write(uint32_t,uint8_t);
    extern uint8_t lll_external_mem_read(uint32_t);
    extern void lll_close_external_memory(void);
    extern void lll_init_cache(void);
    extern void lll_remove_cache(void);
#endif

extern uint8_t lll_init_program(char*,uint32_t);
extern uint8_t lll_end_program(void);
extern uint8_t lll_get(void);
extern void lll_send_info(char*, uint32_t);
extern void lll_throw_error(uint8_t,char *,uint8_t);
extern void lll_goTo(uint64_t);
extern uint64_t lll_getPosition(void);

//streams for device
extern lll_err lll_stream_out(uint32_t,uint8_t);
extern lll_err lll_stream_in(uint32_t,uint8_t);

#endif // LLL_TINY_H_INCLUDED
