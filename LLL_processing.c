#include "LLL_processing.h"
#include "LLL.h"

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
            break;
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
        tmpParam.carry = lll_get();
    }else{
        tmpParam.carry = lll_tmp;
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


/*
gets value of parameter or structure of range
*/
void getValueOrRange(token_s * token){
    switch(token->param.type){
        case mem:
            token->value = LLL_load_mem(token->param.value1);
            token->rov = 0;
            break;
        case cst:
            token->value = token->param.value1;
            token->rov = 0;
            break;
        case flag:
            token->value = ((status_register & (1<<token->param.value1))?1:0);
            token->rov = 0;
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
            token->rov = 1;
            break;
        default:
            lll_throw_error(1,"Interpreter error!",0);
            token->rov = 2;
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
    if(token->rov){
        if(iterator >= token->range.length) return 1;
        *actValue=LLL_load_mem(token->range.begin);
        token->range.begin += (token->range.increasing ? 1 : -1);
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
    if(token.rov){
        LLL_save_mem(token.range.begin + (token.range.increasing ? -1 : 1), value);
    }else{
        LLL_save_mem(token.param.value1, value);
    }
}