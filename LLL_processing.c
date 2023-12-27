#include "LLL_processing.h"
#include "LLL.h"

uint64_t d_jmp;
int32_t r_jmp;
uint64_t ra;
uint8_t rv;  // if you should return

// processing section
static uint32_t LLL_load_mem_32(uint32_t address){
    uint32_t tmpValue = LLL_load_mem(address);
    tmpValue = (tmpValue<<8)+LLL_load_mem(address+1);
    tmpValue = (tmpValue<<8)+LLL_load_mem(address+2);
    tmpValue = (tmpValue<<8)+LLL_load_mem(address+3);
    return tmpValue;
}

static uint32_t LLL_get32bit(void){
    uint32_t val32=0;
    for(int i=0;i<4;i++){ //read adress
        val32 = (val32<<8) + lll_get();
    }
    return val32;
}

static uint64_t LLL_get64bit(void){
    uint64_t val64=0;
    for(int i=0;i<8;i++){ //read adress
        val64 = (val64<<8) + lll_get();
    }
    return val64;
}

static int32_t LLL_getS32bit(void){
    int32_t val32=0;
    for(int i=0;i<4;i++){ //read adress
        val32 = (val32<<8) + lll_get();
    }
    return val32;
}

static lll_param LLL_getHalfParam(uint8_t firstChar){
    lll_param tmpParam;
    
    tmpParam.value2=0;
    tmpParam.carry=0;

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
            break;
        case '$':
            tmpParam.type=sreg;
            break;
        case '^':
            tmpParam.type=sp;
            break;
        default:
        lll_throw_error(1,"Wrong parameter",firstChar);
    }
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
        tmpParam.carry = lll_get();
    }else{
        tmpParam.carry = lll_tmp;
    }

    return tmpParam;
}

uint8_t LLL_doIfJump(lll_command_list command){
    if(LLL_FRJMP == command){
        r_jmp=lll_get();
    }else if(LLL_RJMP == command){
        r_jmp = LLL_getS32bit();
    }else if(LLL_JMP == command){
        d_jmp = LLL_get64bit();
    }else{
        return 0;
    }
    
    rv=lll_get();
    return 1;
}

/*
gets value of parameter or structure of range
*/
void getValueOrRange(token_s * token){

    switch(token->param.type){
        case mem:
            token->value = LLL_load_mem(token->param.value1);
            break;
        case cst:
            token->value = token->param.value1;
            break;
        case flag:
            token->value = ((status_register & (1<<token->param.value1))?1:0);
            break;
        case range:
            if(token->param.value1 > token->param.value2){
                token->range.increasing = 1;
                token->range.length = token->param.value1 - token->param.value2+1;
            }else{
                token->range.increasing = 0;
                token->range.length = token->param.value2 - token->param.value1+1;
            }
            token->range.begin = token->param.value2;
            break;
        case sreg:
            token->value = status_register;
            break;
        case sp:
            stack_pointer.counter=0;
            break;
        default:
            lll_throw_error(1,"Interpreter error!",0);
            break;
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
uint8_t getValueInIteration(uint32_t iterator,token_s *token, uint8_t *actValue){
    if(token->param.type == range){
        if(iterator >= token->range.length) return 1;
        *actValue=LLL_load_mem(token->range.begin);
        token->range.begin += (token->range.increasing ? 1 : -1);
    }else if(token->param.type == sp){
        *actValue = ((stack_pointer.value>>(3<<stack_pointer.counter)) & 0xFF);
        if(stack_pointer.counter>3)   return 1;
        stack_pointer.counter++;
    }else{
        if(!iterator){
            *actValue = token->value;
        }else{
            *actValue = 0;
        }
    }
    return 0;
}


void LLL_save(token_s token, uint8_t value){
    switch(token.param.type){
        case range:
            LLL_save_mem(token.range.begin + (token.range.increasing ? -1 : 1), value);
            break;
        case mem:
            LLL_save_mem(token.param.value1, value);
            break;
        case flag:
            if(value){
                status_register |= (1<<token.param.value1);
            }else{
                status_register &=~ (1<<token.param.value1);
            }
            break;
        case sreg:
            status_register = value;
            break;
        case sp:
            if(!(stack_pointer.counter-1)){
                stack_pointer.value=value;
            }else{
                stack_pointer.value+=(value<<(3<<(stack_pointer.counter-1)));
            }
            break;
        default:
            lll_throw_error(1,"Wrong output parameter",token.param.type);
            break;
    }
        
}