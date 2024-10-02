#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "stack.h"


int main()
{
    Stack MyStack  = {0};
    size_t capacity = 64;

    StackCtor(&MyStack, capacity);
    StackPush(&MyStack, 10.0);
    printf("MyStack[0] = %lf\n", MyStack.StackElements[0]);
    StackPop(&MyStack);
    printf("MyStack[0] = %lf\n", MyStack.StackElements[0]);

    StackDtor(&MyStack);
}

