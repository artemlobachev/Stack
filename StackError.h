#ifndef STACK_ERROR_H
#define STACK_ERROR_H

#include "stack.h"

#ifndef NDEBUG
    #define STACK_ASSERT(SOMESTACK) StackAssert(SOMESTACK, StackVerif(SOMESTACK), __FILE__, __PRETTY_FUNCTION__, __LINE__)
    #define STACK_DUMP(SOMESTACK) StackDump(SOMESTACK, __FILE__, __PRETTY_FUNCTION__, __LINE__)
    #define PRINT_ERROR(SOMESTACK, ERROR) PrintError(SOMESTACK, ERROR, __FILE__, __PRETTY_FUNCTION__, __LINE__)
    #define ON_DEBUG(...) __VA_ARGS__                                                    
#else
    #define ON_DEBUG(...)
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
    STACK_ELEMENT_NULL_ADDRESS,
    SIZE_MORE_CAPACITY
};

void StackAssert(Stack *SomeStack, ErrorCode error, const char* FileName, const char *FuncName, const int ErrorLine);
void PrintError(ON_DEBUG(Stack *SomeStack,) ErrorCode error, const char *FileName, const char *FuncName, const int LineCall);
ErrorCode StackVerif(Stack *SomeStack);

#endif