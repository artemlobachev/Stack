#include <stdio.h>
#include <assert.h>

#include "stack.h"   
#include "StackError.h"
#include "UsefulPrints.h"

void StackAssert(Stack *Stack, ErrorCode error, const char *FileName, const char *FuncName, const int LineCall)
{
    if (error)
    {
        PrintError(Stack, error, FileName, FuncName, LineCall);
        assert(0 && "tut krch code error");
    }
}
