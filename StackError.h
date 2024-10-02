#ifndef STACK_ERROR_H
#define STACK_ERROR_H

#include "stack.h"


#ifndef NDEBUG
    #define STACK_DUMP(SOMESTACK) StackDump(SOMESTACK, __FILE__, __PRETTY_FUNCTION__, __LINE__)
    #define ON_DEBUG(...)  __VA_ARGS__                                                     
#else
    #define ON_DEBUG(code);
    #define STACK_DUMP(SOMESTACK);
#endif

enum Errors
{
    ERROR_NOT_FOUND,
    STACK_ELEMENTS_ALLOCATION_ERROR,
    STACK_NULL_ADRESS,
    CAPACITY_BELOW_ZERO,
    POP_EMPTY_STACK,
    REALLOC_ERROR,
};

void StackAssert(Stack *SomeStack, const char* FileName, const char *FuncName, const int ErrorLine);
int StackVerificator(Stack *SomeStack);
void PrintError(Stack *SomeStack, int ErrorCode);

#endif