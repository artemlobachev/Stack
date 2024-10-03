#ifndef STACK_H
#define STACK_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "StackError.h"

#define PRINT_ERROR(STACK, ERROR) PrintError(STACK, ERROR, __FILE__, __PRETTY_FUNCTION__, __LINE__)

typedef double Stack_t;
typedef int ErrorCode;

struct Stack
{
    Stack_t *StackElements;
    size_t capacity;
    size_t size;
};

enum HysteresisMode {decrease, increase};
void PrintError(ON_DEBUG(Stack *Stack,) ErrorCode error, const char *FileName, const char *FuncName, const int LineCall);
ErrorCode StackVerif(Stack *Stack);
ErrorCode StackCtor(Stack *SomeStack, size_t capacity);
ErrorCode StackPush(Stack *SomeStack, Stack_t element);
static ErrorCode StackRealloc(Stack *SomeStack, int mode);
static void FillPoison(Stack *SomeStack, size_t ElementsAfterAlloc, int mode);
ErrorCode StackDump(Stack *SomeStack, const char *FileName, const char *FromFunc, const int LineCall);
ErrorCode StackPop(Stack *SomeStack);
ErrorCode StackDtor(Stack *SomeStack);

#endif