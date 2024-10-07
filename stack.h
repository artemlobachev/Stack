#ifndef STACK_H
#define STACK_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#define PRINT_ERROR(STACK, ERROR) PrintError(STACK, ERROR, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#define CHECK_STACK(STACK) PRINT_ERROR(STACK, StackVerify(STACK))

typedef double Stack_t;
typedef int ErrorCode;
typedef uint64_t CanaryType;
 
enum mode
{
    increase,
    decrease
};

struct Stack
{  
#ifndef CANARY_PROTECTION   
    CanaryType TOP_CANARY;
#endif

    Stack_t *StackElements;
    size_t capacity;
    size_t size;

#ifndef CANARY_PROTECTION
    CanaryType BOTTOM_CANARY;
#endif
};

void PrintError(Stack *stk, ErrorCode error, const char *FileName, const char *FuncName, const int LineCall);
ErrorCode StackVerify(Stack *stk);
ErrorCode StackCtor(Stack *stk, size_t capacity);
ErrorCode StackPush(Stack *stk, Stack_t element);
static ErrorCode StackRealloc(Stack *stk, size_t AllocatedMemory);
static size_t GetReallocMemory(Stack *stk, int mode);
//static void FillPoison(Stack *stk, size_t ElementsAfterAlloc, int mode);
ErrorCode StackDump(Stack *stk, const char *FileName, const char *FromFunc, const int LineCall);
ErrorCode StackPop(Stack *stk);
ErrorCode StackDtor(Stack *stk);

#endif