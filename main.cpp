#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "stack.h"


int main()
{
    Stack Stack  = {0};
    size_t capacity = 64;

    StackCtor(&Stack, capacity);
    StackPush(&Stack, 10.0);
    printf("Stack[0] = %lf\n", Stack.StackElements[0]);
    StackPop(&Stack);
    printf("Stack[0] = %lf\n", Stack.StackElements[0]);

    StackDtor(&Stack);
}

