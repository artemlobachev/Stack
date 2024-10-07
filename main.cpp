#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "stack.h"


int main()
{
    Stack stk = {}; 
    size_t capacity = 1;

    StackCtor(&stk, capacity);
    StackPush(&stk, 10.0);
    StackPush(&stk, 10.0);
    StackPush(&stk, 10.0);
    StackPush(&stk, 10.0);
    StackPush(&stk, 20.0);
    StackPush(&stk, 20.0);
    StackPush(&stk, 10.0);
    StackPush(&stk, 10.0);
    StackPop(&stk);


    GetStackElements(stk);

    StackDtor(&stk);
} 

