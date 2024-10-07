#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "stack.h"


int main()
{
    Stack stk = {}; 
    size_t capacity = 3;

    StackCtor(&stk, capacity);
    StackPush(&stk, 10.0);
    StackPush(&stk, 10.0);
    StackPush(&stk, 10.0);
    StackPush(&stk, 10.0);
    StackPush(&stk, 10.0);
    StackPush(&stk, 10.0);
    printf("Stack[0] = %lf\n", stk.StackElements[0]);
    StackPop(&stk); 
    printf("Stack[0] = %lf\n", stk.StackElements[6]);

    StackDtor(&stk);
}

