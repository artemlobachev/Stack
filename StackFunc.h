#ifndef STACK_FUNCTIONS_H
#define STACK_FUNCTIONS_H
#include "stack.h"

ErrorCode StackRealloc(Stack *SomeStack, int mode);
Stack_t *MyRealloc(Stack *SomeStack, size_t ReallocMemory);
#endif