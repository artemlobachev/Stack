#ifndef STACK_H
#define STACK_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#include "HashFunction.h"
#include "StackError.h"

#define PRINT_ERROR(STACK, ERROR) PrintError(STACK, ERROR, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#define CHECK_STACK(STACK) PRINT_ERROR(STACK, StackVerify(STACK))

#ifdef CANARY1
    typedef uint64_t CanaryType;
    #define ON_CANARY(...) __VA_ARGS__
#else
    #define ON_CANARY(...)
#endif

#ifdef HASH
    typedef int64_t HashType;
    #define ON_HASH(...) __VA_ARGS__
#else
    #define ON_HASH(...)
#endif

typedef double Stack_t;
typedef int ErrorCode;
 
enum mode
{
    increase,
    decrease
};

struct Stack
{  
    ON_CANARY
    (

    CanaryType TOP_CANARY;
    
    )

    ON_HASH
    (

    HashType DataHash;
    HashType StackHash;
    
    )

    Stack_t *StackElements;
    size_t capacity;
    size_t size;
    
    ON_CANARY
    (

    CanaryType BOTTOM_CANARY;

    )
};

void PrintError(Stack *stk, ErrorCode error, const char *FileName, const char *FuncName, const int LineCall);
ErrorCode StackVerify(Stack *stk);
ErrorCode StackCtor(Stack *stk, size_t capacity);
ErrorCode StackPush(Stack *stk, Stack_t element);
static ErrorCode StackRealloc(Stack *stk, size_t AllocatedMemory);
static size_t GetReallocMemory(Stack *stk, int mode);
ErrorCode StackDump(Stack *stk, const char *FileName, const char *FromFunc, const int LineCall);
ErrorCode StackPop(Stack *stk);
ErrorCode StackDtor(Stack *stk);
void GetStackElements(Stack *stk);

#endif