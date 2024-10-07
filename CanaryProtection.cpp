#include "UsefulPrints.h"
#include "CanaryProtection.h"
#include "StackError.h"

void CanaryDump(Stack *stk, const char *FileName, const char *FromFunc, const int  LineCall)
{
    assert(FileName && FromFunc && LineCall > 0);
    
    CanaryType *TopDataCanary = (CanaryType *) ((char *) stk->StackElements - sizeof(CanaryType));
    CanaryType *BottomDataCanary = (CanaryType *) (stk->StackElements + stk->capacity); 

    PRINT_STARS;

    COLOR_PRINTF(GREEN, "\t\tCANARY DUMP\n");

    printf("%s:  in function: %s line: %d\n", FileName, FromFunc, LineCall);

/*                    CANARIES FOR STACK                                           */
    printf("Bottom canary for stack stored at this address: %#p\n", stk->BOTTOM_CANARY);
    if (&stk->BOTTOM_CANARY) printf("Value at this address: %llu\n", stk->BOTTOM_CANARY);
    
    printf("Top canary for stack stored at this address: %#p\n", &stk->TOP_CANARY);
    if (&stk->TOP_CANARY) printf("Value at this address: %llu\n\n", stk->TOP_CANARY);

/*                    CANARIES FOR DATA IS STACK                                   */
    printf("Bottom canary for data in stack stored at this address: %#p\n", BottomDataCanary);
    if (BottomDataCanary) printf("Value at this address: %llu\n", *BottomDataCanary);

    printf("Top canary for data in stack stored at this address: %#p\n", TopDataCanary);
    if (TopDataCanary) printf("Value at thus address: %llu\n", *TopDataCanary);

    PRINT_STARS;
}

void CanaryAbort(Stack *stk, int CanaryError, const char *FileName, const char *FromFunc, const int LineCall)
{ 
    CanaryDump(stk, FileName, FromFunc, LineCall);
    STACK_DUMP(stk);
    
    switch(CanaryError)
    { 
        case CANARY_IN_NULL:
            COLOR_PRINTF(RED, "CANARY POINTED IN NULL ADRESS\n");
            break;

        case CHANGE_CANARY:                          
            COLOR_PRINTF(RED, "TRY OVERFLOW BUFFER!\n");                                                                         
            break;

        case SAME_ADRESS:                                                
            COLOR_PRINTF(RED, "CANARY HAVE SAME ADRESS ");                     
            COLOR_PRINTF(RED, "MAYBE CANARY-ADRESS WAS CHANGED\n");             
            break;
    };

    COLOR_PRINTF(RED, "ABORTED!");                                 
    abort(); 
}

void CheckStackCanary(Stack *stk, const char *FileName, const char *FromFunc, int LineCall)
{
    assert(stk && FileName && FromFunc && LineCall > 0);
    
    if (&stk->TOP_CANARY == NULL || &stk->BOTTOM_CANARY == NULL)
        CanaryAbort(stk, CANARY_IN_NULL, FileName, FromFunc, LineCall);
    
    if (stk->TOP_CANARY != CANARY || stk->BOTTOM_CANARY != CANARY)       
        CanaryAbort(stk, CHANGE_CANARY, FileName, FromFunc, LineCall);

    if (&stk->TOP_CANARY == &stk->BOTTOM_CANARY)
        CanaryAbort(stk, SAME_ADRESS, FileName, FromFunc, LineCall);

}  

void CheckDataCanary(Stack *stk, const char *FileName, const char *FromFunc, int LineCall)
{
    assert(stk && FileName && FromFunc && LineCall > 0);

    CanaryType *TopCanaryAdress = (CanaryType *) ((char *) stk->StackElements - sizeof(CanaryType));
    CanaryType *BottomCanaryAdress = (CanaryType *) (stk->StackElements + stk->capacity);

    if (TopCanaryAdress == NULL || BottomCanaryAdress == NULL)
        CanaryAbort(stk, CANARY_IN_NULL, FileName, FromFunc, LineCall);

    if (*TopCanaryAdress != CANARY ||  *BottomCanaryAdress != CANARY)
        CanaryAbort(stk, CHANGE_CANARY, FileName, FromFunc, LineCall);

    if (TopCanaryAdress == BottomCanaryAdress)
        CanaryAbort(stk, SAME_ADRESS, FileName, FromFunc, LineCall);
    
} 

void SetDataCanary(Stack_t *StackElements, size_t capacity)
{
    assert(StackElements);
    
    CanaryType BottomDataCanary = CANARY;
    CanaryType TopDataCanary = CANARY;         


    *(CanaryType *)((char *) StackElements - sizeof(CanaryType)) = TopDataCanary; 
    *(CanaryType *)(StackElements + capacity) = BottomDataCanary;
}