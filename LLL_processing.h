#ifndef LLL_PROCESSING_H
#define LLL_PROCESSING_H

#include <stdint.h>

typedef struct{
    uint8_t  type;
    uint32_t value1;
    uint32_t value2;
    uint8_t carry;
} lll_param;

typedef enum{
    mem,        // Rx,&x,*x
    flag,       // %x
    cst,        // @x
    range,      // y-x
    sreg,       // $
    sp          // ^
} parameterType;


typedef struct{
    uint32_t begin;
    uint8_t increasing;
    uint32_t length;
} range_s;

typedef struct 
{
    lll_param param;
    uint8_t value;
    range_s range;
} token_s;

lll_param LLL_getParam(uint8_t carry);
uint32_t LLL_get32bit(void);
uint64_t LLL_get64bit(void);

void getValueOrRange(token_s * token);
uint8_t getValueInIteration(uint32_t iterator,token_s *token, uint8_t *actValue);
void LLL_save(token_s token, uint8_t value);

#endif