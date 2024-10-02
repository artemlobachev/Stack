#include <stdlib.h>

#include "stack.h"
#include "StackError.h"
#include "StackFunc.h"

void StackRealloc(Stack *SomeStack, int mode)
{
    STACK_ASSERT(SomeStack);

    size_t ReallocMemory = 0;

    switch(mode)
    {
        case low:
            ReallocMemory = (SomeStack->capacity / 2) * sizeof(Stack_t);
            SomeStack->StackElements = MyRealloc(SomeStack, ReallocMemory);
            break;
        case up:
            ReallocMemory = (SomeStack->capacity * 2) * sizeof(Stack_t);
            SomeStack->StackElements = MyRealloc(SomeStack, ReallocMemory);
            break;
        default:
            assert(0);
    };
    
    STACK_ASSERT(SomeStack);
}

Stack_t *MyRealloc(Stack *SomeStack, size_t ReallocMemory)
{
    while (ReallocMemory != 0)
    {
        Stack_t *AllocPtr = (Stack_t *) realloc(SomeStack, ReallocMemory);

        if (AllocPtr != nullptr)
        {
            size_t AllocCells = ReallocMemory / sizeof(Stack_t);            //TODO РАЗОБРАТЬСЯ С РАЗНЫМИ СЛУЧАЯМИ
            for (int i = 0; i < AllocCells; i++)
                AllocPtr[i] = POISON;
            
            return AllocPtr;
        }

        ReallocMemory = ReallocMemory / 2;
    }

    return SomeStack->StackElements;
}