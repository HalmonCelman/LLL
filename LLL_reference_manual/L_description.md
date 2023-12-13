# L description

L is machine language for lll - lll gets compiled to L

## Shape of command
```
[6 bits - command][1 bit - s option][1 bit - condition option][1 bit set/clear][7 bits - flags][? bits - param1][? bits - param2][? bits - param3]
```
? means command dependent number of bits

**If condition option is set to 0 than next 8 bits( sat/clear and flags ) don't exist so frame look like this:**
```
[6 bits - command][1 bit - s option][1 bit - condition option][? bits - param1][? bits - param2][? bits - param3]
```

## Numeration

Automatic numeration by markdown
### Commands
0. add
0. and       
0. cmp
0. dec
0. div
0. frjmp
0. in
0. inc
0. jmp
0. mov
0. mul
0. not
0. or
0. out
0. ret
0. rjmp
0. sub
<!--> <!-->

63. exit

## Parameters

### Registers

> [**R**][8-bits]

### Memory Matrix

> Direct: [**&**][32-bits]
>
> Indirect: [**\***][32-bits]

### Flags

> SREG: [**$**]
> 
> Flags: [**%**][8-bits]

### Constants

> [**@**][8-bits]

### Range

> [Register or adress][**-**][Register or adress]

### Bool value

> [8-bits]

### 8 bit value

> [8-bits]

### 32 bits value

> [32-bits]

### 64 bits value

> [64-bits]