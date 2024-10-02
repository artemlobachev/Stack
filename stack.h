#ifndef STACK_H
#define STACK_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef double Stack_t;
typedef int ErrorCode;

const static int POISON = 0xB00B5;
const static int STDCAPACITY = sizeof(Stack_t) * 64;
const static int ALLOCATION_COEF = 2;

struct Stack
{
    Stack_t *StackElements;
    size_t capacity;
    size_t size;
};

enum HysteresisMode {decrease, increase};

ErrorCode StackCtor(Stack *SomeStack, size_t capacity);

ErrorCode StackPush(Stack *SomeStack, Stack_t element); //push elem into stack

ErrorCode StackDump(Stack *SomeStack, const char *FileName, const char *FromFunc, const int LineCall); //all info

ErrorCode StackPop(Stack *SomeStack);
ErrorCode StackDtor(Stack *SomeStack);

#endif