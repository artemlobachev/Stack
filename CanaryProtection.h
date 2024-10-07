#ifndef CANPR_H
#define CANPR_H

#include <stdint.h>
#include "stack.h"

const CanaryType CANARY = 0xCAEFF64;

#ifndef CANARY_PROTECTION             
    #define ON_CANARY(...) __VA_ARGS__
#else 
    #define ON_CANARY(...) 
#endif 

enum CanaryError
{  
    CHANGE_CANARY,
    SAME_ADRESS,
    CANARY_IN_NULL
};

#define CHECK_CANARY(STACK) CheckStackCanary(STACK, __FILE__, __PRETTY_FUNCTION__, __LINE__); \
                            CheckDataCanary(STACK, __FILE__, __PRETTY_FUNCTION__, __LINE__)

#define CANARY_DUMP(STACK) CanaryDump(STACK, __FILE__, __PRETTY_FUNCTION__, __LINE__)

void CheckStackCanary(Stack *stk, const char *FileName, const char *FromFunc, int LineCall);
void CheckDataCanary(Stack *stk, const char *FileName, const char *FromFunc, int LineCall);
void SetDataCanary(Stack_t *StackElements, size_t capacity);
void CanaryDump(Stack *stk, const char *FileName, const char *FromFunc, const int  LineCall);
void CanaryAbort(Stack *stk, int CanaryError, const char *FileName, const char *FuncName, const int LineCall);

#endif