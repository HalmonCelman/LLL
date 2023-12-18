# LLL v 2.0 instruction set manual and description
by KK

## Contents

- [Description](#description)
    - [Possibilities](#possibilities)
    - [Memory](#memory)
        - [Stack](#stack)
        - [Stack Pointer](#stack-pointer)
        - [Memory Matrix](#memory-matrix-description)
        - [Registers](#registers-description)
        - [Status Register](#sreg-description)
        - [Memory Map](#memory-map)
    - [Command Shape](#command-shape)
        - [Command](#command)
        - [s option](#s-option)
        - [conditon](#condition)
        - [parameters](#parameters)
        - [*Delimiters*](#delimiters)
    - [Syntax Description](#syntax-description)
        - [Constants](#constants)
        - [Flags](#flags)
        - [Memory Matrix](#memory-matrix)
        - [Registers](#registers)
        - [Ranges](#ranges)
    - [Directives](#directives)
        - [Labels](#labels)
        - [Comments](#comments)
        - [.def directive](#def-directive)
- [Instruction Set Manual](#instruction-set-manual)
    - [Symbols used in Instruction Set Manual](#symbols-used-in-instruction-set-manual)
    - [Commands](#commands)
        - [ADD](#add)
        - [AND](#and)
        - [CMP](#cmp)
        - [DEC](#dec)
        - [DIV](#mul)
        - [EXIT](#exit)
        - [FJMP](#fjmp)
        - [IN](#in)
        - [INC](#inc)
        - [JMP](#jmp)
        - [MOV](#mov)
        - [MUL](#mul)
        - [NOT](#not)
        - [OR](#or)
        - [OUT](#out)
        - [POP](#pop)
        - [PUSH](#push)
        - [RET](#ret)
        - [RJMP](#rjmp)
        - [SUB](#sub)
        
- [Examples of usage](#examples-of-usage)

## Description
### Possibilities
- LLL have memory adressed by **32 bits** - up to 4GB
- S option is one bit
- set/clear option is one bit
- Number of command is adressed by **6 bits**
- All memory units( registers also ) are 8-bit
- All memory units starts with the value of 0 or readed from [Memory map](#memory-map)
- **LLL isn't case-sensitive** so:
> ```
> ADD r0,r1,r2
> ```
> is the same as 
> ```
> add R0,R1,R2
> ```
> or even
> ```
> aDd R0,r1,R2
> ```

---

### Memory:
####  **Stack**
>   Stack intentionally is located in RAM, but when
>   device have to low RAM memory to have all things
>   inside RAM it creates file with stack data
>
>    ***! WARNING: remember to pop data after pushing it
>    especially in loops!***
>
>    If you don't pop it you can obtain very large file and in the best case error and cancelled program execution

#### Stack Pointer
> it 32-bit register and can be read using **^**
> it's technically trated as [**range**](#ranges) and can be used wherever **range** can be used.
> it points how many memory stack uses at the moment - it's **increasing** as stack is **increasing** (not like in most of microcontrollers)
> 
> Example of reading Stack Pointer:
> ```
> mov R3-R0,^   # now we have it's value in R3-R0
> ```
> - with this register you can detect if your stact isn't too high
> - when stack pointer overflows ***program will be killed*** so on stack can be up to 4 gigabytes of memory

#### **Memory Matrix description** 
> this memory takes place in RAM or in external memory if it's needed, adressing memory matrix is **32-bit**.

#### **Registers description**
> in fact they are part on the start of `Memory Matrix` they are adressed in **8 bit** value.

#### **SREG description**  
> **Status Register** - contains inside flags - each flag is 1 bit, each flag can be modified by some instructions reffering to [Instruction Set Manual](#instruction-set-manual)
>
> There are only few flags:
>
> - **O** Flag - Overflow flag
> - **A** Flag - Additional flag
> - **R** Flag - Rest and Result flag
> - **Z** Flag - Zero flag
>
> Construction of SREG:
> ```
> [x,x,x,x,Z,R,A,O]
> MSB            LSB
> x - unused
> ```
>
> so at bit 0 is **O** flag
> 
> at bit 1 - **A** etc.
>
> *SREG behaves like register and can be used wherever registers can be used*

#### **Memory Map** 
> it is **FILE** and it's very important file, if you want in your application to load some data at the start I recommend to place it in memory map file
>
> - this file is lll.mmp and this name is strictly reserved
> - this file should be in the same folder as your program 
>
> - Too see how to construct Memory Map file see:
>
> [**Constructing Memory Map**](Construct_Memory_Map.md)

---

### Command Shape

```
{command}[s][condition] {param1} {param2} {param3} ...
```
#### {command}

> All comand are described in [Instruction Set Manual](#instruction-set-manual)

#### [s] option

> it's optional
> when you don't write **s** after command name flags wouldn't change - **except [cmp command](#cmp)**
> 
> Example:
> ```
> add R0,R1,@5  # without flag changing
> adds R0,R1,@5 # with flag changing
> ```
>
> In first case even if overflow occures **O** flag wouldn't change
>
> In second case all flags will be cleared, unchanged or set according to the [Instruction Set Manual](#instruction-set-manual)

#### [condition]

> also optional
> you can construct with these conditional instructions
> **condition** is build by **flag names*** and either **s**(yes, once again) or **c** which means accordingly that flag is **set** or **cleared**
>
> *flags are set when 1 is on flag, cleared if 0*
> 
> Example:
> ```
> adds R0,@123,R1   # add with flag change
> movos R2,R0       # if overflow occures do mov
> cmp R0,R1         # compare values
> movars R1,R0      # if A flag AND R flag are set
> ```
>
> *clearification: if command have no condition it will be executed **always***

#### Parameters
> - What could be a parameter?
> 
> It is dependent on command you are using 
> 
> Parameter can be(for sytax see [Syntax description](#syntax-description)):
> - a constant value
> - a register, memory adress, flag or label
> - a range of registers or memory adresses

#### Delimiters

>*instead of spaces as a delimiter can be , or ; or even end of line character*
>
>> Example:
>> ```
>> add R0 R0 R1
>> add,R0,R0,R1
>> add;R0;R0;R1
>> ```
>
>*delimiters can be also mixed in one command*
>
>> Example:
>> ```
>> add,R0;R0 R1
>> ```
>
>*and can be joined together*
>
>> Example:
>> ```
>> add, R0 ,;R0   R1
>> ```

---

### Syntax Description

#### Constants

> What syntax is for constant parameter?
>> Just place **@** before your constant value, but remember in this language there are no minus values do doing something like **@-5** is forbidden and will cause error
>
> Example:
> ```
> add R0,@5,@9 # write 14 to R0
> ```
>
> *also remember that constants are **8bit** so you cant write more than 255*

#### Flags
> - How to access SREG?
>> write **SREG** as a register, or just **$**:
>> ```
>> add R0,SREG,R1
>> add R0,$,R1
>> ``````
> - How to access particular flag? 
>> Just write their name after **%**
>> Then this flag will return 1 or 0 when it is set or cleared  
>>
>> Examples:
>> ```
>> add R0 %M %A # flags A and M are stored in R0
>> add %R &0 &5
>> ```

#### Memory Matrix

> - How to access memory?
>> Just write **&** before adress of memory
> - Is there any other way?
>> ***Yes*** you can give indirect adress, if you pass  __\*__ before adress instead of **&** machine will read adress of memory you want to access from 4 bytes beginning of given adress after __\*__
>
> - I don't understand, any examples?
>> Here you go:
>> - Add value of **0** adress and **1** adress and store it into adress **275** 
>> ```
>> add &275, &0, &1
>> ```
>> - lets assume that our memory at **15** adress looks like these(in hex):
>>
>> `0 0 0 1`
>>
>> - then do the same as before but *indirect*
>> ```
>> add &275, &0, *15
>> ```
>> 
> - What's happenig at the end?
>> by passing `\*15` we are reading from adress **15** new adress which is **0x0001** - so **&1** - it makes this command equivalent to the first one

#### Registers

- Registers take place in first 256 bytes of **Memory Matrix**

- How to access registers?
> Just write **R** before it's number *OR* write **&** before it's number
- Wait wait wait, isn't **&** reserved for **Memory Matrix**?
> It is, but registers ***are*** in the **Memory Matrix** as was said at the beginning of *Registers section*
- So what's the difference?
> If you have good compiler there is no difference ;) *BUT* if you write **R**, not **&** you can be sure it will take less space and be faster than **&** if your compiler isn't good enough


#### Ranges

> - parameters can also be a **range**
> - range subparameters are delimited by **-**
> - range is a group of two registers, adresses or register and adress
> 
> Example ranges:
> ```
> R24-R15
> &237-&112
> &220-R255
> *12-R15
> ```
---

### Directives

- Directives in fact **depends on the compiler**, but all compilers of LLL **should have at least** directives described below.

- Directives are "commands" that are used during compilation process and **don't exist** in executable file

#### Labels

>- Labels ***dont't exist*** in execution code, it means that if you compile and decompile program their names propably ***wouldn't be the same*** as before, their names would depend only on decompiler you use.
>
>- How to write labels?
>> put **:** before label name
>> Example:
>> ```
>> :label_name
>> ```
> - Labels? For what?
>> For simplicity, when you are jumping somewhere you **don't want** to calculate where you should jump, let's pass it to compiler
> - Example?
>> Here you go:
>> - you can do that
>> ```
>> add R0,R1,R2
>> rjmp -4, 0
>> ```
>> so you calculated that add command in this case will take 4 bytes so you are jumping to start
>> - or you can do that
>> ```
>> add R0,R1,R2
>> jmp 0, 0
>> ```
>> here we are just jumping to the start of the programs
>> but in these 2 cases when we add some line of code we must change values in jumps ://
>> - so the better option is to use labels
>> ```
>> :inf_loop
>> add R0,R1,R2
>> jmp :inf_loop, 0
>> ```
>> also you can use **frjmp** or **rjmp** instead of **jmp** and it will have in this case same effect.
>
> - At the end you said that it will have the same effect so what's the diffrence beetween these 3 commands?
>> It is better explained in [Instruction Set Manual](#instruction-set-manual) but simplifying:
>> - frjmp is faster and takes less space than rjmp but can jump only by 255 bytes relatively to place where you are
>> - rjmp is faster and shorter than jmp but it can jump up to 2^32-1 bytes relatively to place where you are
>> - jmp is the slowest one but jumps directly so you just say where in the program you want to jump
>> 
>> ***In fact** when you have good compiler it will optimize it - you would then use **jmp** or **rjmp** and compiler would replace it with **frjmp** if it would be better*

#### Comments

> You can comment rest of line in your code using **#**
> You can comment any part of code with **/ \*/** 
> Example:
> ```
> add R0,R1,R2 # this is one line comment
> # add R0,R1,R2 <- this also
> / anything there
> mov R0,R5
> won't be executed */
> ``` 
> **Normally comments will be deleted from code in compilation**
> but if you **REALLY** want to put comment in executable file you can do this - it will make your program larger and a bit slower(compiler will make jump just after comment so it will not be executed), *how to do this?*
>> instead of just **#** or **/ \*/** place **#!** or **/! \*/**

#### .def directive

> - defines some \<string\> as something another
> 
> Example:
> ```
> .def Register_0 R0
> .def,const_value;@5
> add Register_0, Register_0, const_value
> ```
> - compiler will just replace \<string\> with the second parameter
> - delimiters are the same as in commands
> - **it will be unrolled**
> 
> Example:
> ```
> .def To_unroll R0
> .def Unroll To_unroll
> add Unroll, R1, R2
> ```
> *Unroll will be replaced with To_unroll and then with R0*

## Instruction Set Manual

### Symbols used in Instruction Set Manual

- **8v** means 8-bit value **signed**  ( without any prefix except minus if it's under 0)
- **32v** means 32-bit value **signed** ( without any prefix except minus if it's under 0)
- **64v** means 64-bit value ( without any prefix )
- **bv** means bool value - **1** or **0** ( without any prefix )
- **Rx** means any [register](#registers)
- **&x** means any [memory adress](#memory-matrix)
- **%x** means any [flag](#flags)
- **\*x** means any [indirect memory adress](#memory-matrix)
- **@x** means any [constant value](#constants)
- **y-x** means any [range](#ranges) (also [stack pointer](#stack-pointer))
- **:x** means any [label](#labels)
- **|** char means that either some thing on left or thing of the right can be used
> Syntax
> ```
> @x | Rx
> ```
> means that you can use here either any constant value or register 

---

### Commands

#### ADD
> sums second and third parameter and gives the result to the first parameter
> 
> Syntax:
> ```
> add {param1} {param2} {param3}
> ```
>
> - {param 1} can be: **`Rx | &x | %x | *x | y-x`**
> - {param 2} can be: **`Rx | &x | %x | *x | @x | y-x`**
> - {param 3} can be: **`Rx | &x | %x | *x | @x | y-x`**
>
> **Functionality:**
> 
>> **Basics:**
>>
>> *param1 = param2 + param3*
>>
>> [Basic examples](#example-add1---basics)
>
>> **Ranges**
>>
>> Ranges have their arguments added one by one
>> 
>> [Ranges examples](#example-add2---ranges)
>>
>> And can be reversed, then added
>> 
>> [Reversing range example](#example-add3---more-ranges)
>
>> **Overflows:**
>>
>> when adding two values, if result >= 256 then local overflow occures
>> if output parameter( parameter1 ) is **range** then this local overflow is stored into the byte which is next to the left: [overflow example](#example-add4---overflows) if there is no place to store the byte or parameter1 isn't range then overflow (**O**) is set
>
>> **Mixing types of parameters**
>>
>> You can also mix the type of parameters, then arguments will be added as long as parameter1 have place to store it or as long as one of parameter don't have values
>>
>> [Mixing example](#example-add5---mixed-types-and-ranges-of-different-length)
>
> If used with s option:
> - overflow: **%O** = 1
> - no overflow: **%O** = 0
> - if result = 0: **%Z** = 1
> - if result != 0: **%Z** = 0
> - all other flags are unchanged
>
> [***Examples***](#add-examples)


#### AND
> makes **binary and** of second and third parameter and gives the result to the first parameter
> 
> Syntax:
> ```
> and {param1} {param2} {param3}
> ```
>
> - {param 1} can be: **`Rx | &x | %x | *x | y-x`**
> - {param 2} can be: **`Rx | &x | %x | *x | @x | y-x`**
> - {param 3} can be: **`Rx | &x | %x | *x | @x | y-x`**
>
> If used with s option:
> - if result = 0: **%Z** = 1
> - if result != 0: **%Z** = 0
> - all other flags are unchanged
>
> [***Examples***](#and-examples)


#### CMP
> compares first and second parameter
> 
> ***! This function is exception, it always sets flags !*** 
>
> Syntax:
> ```
> cmp {param1} {param2}
> ```
>
> - {param1} can be: **`Rx | &x | %x | *x | @x | y-x`**
> - {param2} can be: **`Rx | &x | %x | *x | @x | y-x`**
>
> **ALWAYS**:
> - values are equal: **%R** = 1
> - values are diffrent: **%R** = 0
> - if param1 > param2: **%A** = 1
> - if param1 <= param2: **%A** = 0
> - all other flags are unchanged
>
> [***Examples***](#cmp-examples)


#### DEC
> decreases parameter by 1
> 
> Syntax:
> ```
> dec {param}
> ```
>
> - {param} can be: **`Rx | &x | %x | *x | y-x`**
>
> If used with s option:
> - underflow: **%O** = 1
> - no underflow: **%O** = 0
> - if result = 0: **%Z** = 1
> - if result != 0: **%Z** = 0
> - all other flags are unchanged
>
> [***Examples***](#dec-examples)


#### DIV
> divides third parameter by second and gives the result to the first parameter
> 
> Syntax:
> ```
> div {param1} {param2} {param3}
> ```
>
> - {param 1} can be: **`Rx | &x | %x | *x | y-x`**
> - {param 2} can be: **`Rx | &x | %x | *x | @x | y-x`**
> - {param 3} can be: **`Rx | &x | %x | *x | @x | y-x`**
>
> If used with s option:
> - overflow: **%R** = 1
> - if remainder = 0: **%Z** = 1
> - if remainder != 0: **%Z** = 0
> - all other flags are unchanged
>
> [***Examples***](#div-examples)


#### EXIT
> this function just exit program 
>
> Syntax:
> ```
> exit
> ```
>
> *This function don't have any parameters*
> 
> *This function don't modify flags even with s option*
>
> [***Examples***](#exit-examples)


#### FRJMP
> Fast jumps relatively to adress or to label with or without saving return adress on stack
> 
> Syntax:
> ```
> frjmp {param1} {param2}
> ```
>
> - {param1} can be: **`8v | :x`**
> - {param2} can be: **`bv`**
>
> *This function doesn't set any flags even with s option*
>
> *if {param2} = 1 Stack Pointer increases by 4*
>
> [***Examples***](#frjmp-examples)


#### IN
> pulls values from IN stream, selecting from which stream
> 
> Syntax:
> ```
> in {param1} {param2}
> ```
>
> - {param1} can be: **`Rx | &x | %x | *x | y-x`**
> - {param2} can be: **`8v`**
>
> *This function don't modify flags even with s option*
>
> [***Examples***](#in-examples)


#### INC
> increases parameter by 1
> 
> Syntax:
> ```
> inc {param}
> ```
>
> - {param} can be: **`Rx | &x | %x | *x | y-x`**
>
> If used with s option:
> - overflow: **%O** = 1
> - no overflow: **%O** = 0
> - if result = 0: **%Z** = 1
> - if result != 0: **%Z** = 0
> - all other flags are unchanged
>
> [***Examples***](#inc-examples)


#### JMP
> jumps directly to adress or to label with or without saving return adress on stack
>
> Syntax:
> ```
> jmp {param1} {param2}
> ```
>
> - {param1} can be: **`64v | :x`**
> - {param2} can be: **`bv`**
>
> *This function doesn't set any flags even with s option*
>
> *if {param2} = 1 Stack Pointer increases by 4*
>
> [***Examples***](#jmp-examples)


#### MOV
> moves value of second parameter to the first one
> 
> Syntax:
> ```
> mov {param1} {param2}
> ```
>
> - {param1} can be: **`Rx | &x | %x | *x | @x | y-x`**
> - {param2} can be: **`Rx | &x | %x | *x | y-x`**
>
> *This function doesn't set any flags even with s option*
>
> [***Examples***](#mov-examples)


#### MUL
> multiplicates second and third parameter and gives the result to the first parameter
> 
> Syntax:
> ```
> mul {param1} {param2} {param3}
> ```
>
> - {param 1} can be: **`Rx | &x | %x | *x | y-x`**
> - {param 2} can be: **`Rx | &x | %x | *x | @x | y-x`**
> - {param 3} can be: **`Rx | &x | %x | *x | @x | y-x`**
>
> If used with s option:
> - overflow: **%O** = 1
> - no overflow: **%O** = 0
> - if result = 0: **%Z** = 1
> - if result != 0: **%Z** = 0
> - all other flags are unchanged
>
> [***Examples***](#mul-examples)


#### NOT
> makes **binary not** of second parameter - result is in first parameter
> 
> Syntax:
> ```
> or {param1} {param2}
> ```
>
> - {param 1} can be: **`Rx | &x | %x | *x | y-x`**
> - {param 2} can be: **`Rx | &x | %x | *x | @x | y-x`**
>
> If used with s option:
> - if result = 0: **%Z** = 1
> - if result != 0: **%Z** = 0
> - all other flags are unchanged
>
> [***Examples***](#not-examples)


#### OR
> makes **binary or** of second and third parameter and gives the result to the first parameter
> 
> Syntax:
> ```
> or {param1} {param2} {param3}
> ```
>
> - {param 1} can be: **`Rx | &x | %x | *x | y-x`**
> - {param 2} can be: **`Rx | &x | %x | *x | @x | y-x`**
> - {param 3} can be: **`Rx | &x | %x | *x | @x | y-x`**
>
> If used with s option:
> - if result = 0: **%Z** = 1
> - if result != 0: **%Z** = 0
> - all other flags are unchanged
>
> [***Examples***](#or-examples)


#### OUT
> pushes values into OUT stream, selecting for which stream
> 
> Syntax:
> ```
> out {param1} {param2}
> ```
>
> - {param1} can be: **`Rx | &x | %x | *x | y-x`**
> - {param2} can be: **`8v`**
>
> *This function don't modify flags even with s option*
>
> [***Examples***](#out-examples)

#### POP
> pops value from stack
>
> Syntax:
> ```
> pop {param}
> ```
>
> - {param} can be **`Rx | &x | %x | *x | y-x`**
>
> If used with s option:
> - if poped value = 0: **%Z** = 1
> - if poped value != 0: **%Z** = 0
> - all other flags are unchanged
>
> *this function decreases stack size by length of range or by 1*
>
> [***Examples***](#pop-examples)

#### PUSH
> pushes value into stack
>
> Syntax:
> ```
> push {param}
> ```
>
> - {param} can be **`Rx | &x | %x | *x | @x | y-x`**
>
> If used with s option:
> - if pushed value = 0: **%Z** = 1
> - if pushed value != 0: **%Z** = 0
> - all other flags are unchanged
>
> *this function incerases stack size by length of range or by 1*
>
> [***Examples***](#push-examples)

#### RET
> Returns to the last-saved return adress
> 
> Syntax:
> ```
> ret
> ```
>
> **When there is nothing on stack this function is ignored**
> 
> *This function don't have any parameters*
>
> *This function doesn't set any flags even with s option*
>
> *This function decreases Stack Pointer by 4*
>
> [***Examples***](#ret-examples)


#### RJMP
> jumps relatively to adress or to label with or without saving return adress on stack
>
> Syntax:
> ```
> rjmp {param1} {param2}
> ```
>
> - {param1} can be: **`32v | :x`**
> - {param2} can be: **`bv`**
>
> *This function doesn't set any flags even with s option*
>
> *if {param2} = 1 Stack Pointer increases by 4*
>
> [***Examples***](#rjmp-examples)


#### SUB
> substracts third parameter from second and gives the result to the first parameter
> 
> Syntax:
> ```
> sub {param1} {param2} {param3}
> ```
>
> - {param 1} can be: **`Rx | &x | %x | *x | y-x`**
> - {param 2} can be: **`Rx | &x | %x | *x | @x | y-x`**
> - {param 3} can be: **`Rx | &x | %x | *x | @x | y-x`**
>
> If used with s option:
> - underflow: **%O** = 1
> - no underflow: **%O** = 0
> - if result = 0: **%Z** = 1
> - if result != 0: **%Z** = 0
> - all other flags are unchanged
>
> [***Examples***](#sub-examples)


---


### Examples of usage
#### ADD Examples
>> #### Example add.1 - Basics
>> Adding **constants** and/or **single memory units** and storing in **single memory unit**
>> ```
>> add R0, @1, @2    # 3 is written to 0 address
>> add R1, &0, @2    # R1 = &0 + 2 -> in this case &0=3 so R1=5
>> add $,  R1, *5    # SREG = R1 + indirect address stored beggining from 5
>> add %A, R0, R1    # if R0+R1 != 0 then A flag is set otherwise A is cleared 
>> ```
> 
>> #### Example add.2 - Ranges
>> Adding **ranges** and storing into **range**
>> ```
>> add R5-R4, R3-R2, R1-R0 # R0+R2 stored into R4, R1+R3 stored into R5
>> add R3-R0, ^, R7-R4     # R3-R0 now have value of stack pointer increased by R7-R4
>> ```
>
>> #### Example add.3 - More ranges
>> You can do also:
>> ```
>> add R5-R4,R2-R3,R1-R0 # R4=R0+R3, R5=R1+R2
>> ```
>> And a bit tricky:
>> ```
>> assuming:
>> R0 = 1
>> 
>> add R2-R1, R1-R0, R1-R0
>>
>> What is the value of R1 and R2?
>> R1 = R0+R0 = 2
>> and THEN
>> R2 = R1+R1 = 2+2 = 4
>> ```
>
>> #### Example add.4 - Overflows
>> ```
>> lets assume that:
>> R0 = 255
>> R1 = 5
>> R2 = 1
>> R3 = 1
>> 
>> then:
>> add R5-R4,R3-R2,R1-R0
>> 
>> results in:
>> R4 = R0+R2 = 0 -> 256 - local overflow
>> R5 = R1+R3+1 -> because there was overflow
>> so R5 = 7
>> and overflow flag is CLEARED( if s option was set ) because overflow was stored into R5
>> ```
>
>> #### Example add.5 - Mixed types and ranges of different length
>> ***if sth is unchanged it doesn't mean it's 0***
>> ```
>> add R1-R0, @255, @1  # R1=1 R2=0 (from local overflow)
>> add R1-R0, @12,  @0  # R1 is unchanged R2=12
>> add R0, R1-R0, R3-R2 # R0 = R2+R0
>> ```
>>
#### SUB Examples
>
#### MUL Examples
>
#### DIV Examples
>
#### AND Examples
>
#### OR Examples
>
#### NOT Examples
>
#### INC Examples
>
#### DEC Examples
>
#### MOV Examples
>
#### JMP Examples
>
#### RJMP Examples
>
#### FRJMP Examples
>
#### RET Examples
>
#### CMP Examples
>
#### PUSH Examples
>
#### POP Examples
>
#### IN Examples
>
#### OUT Examples
>
#### EXIT Examples
>