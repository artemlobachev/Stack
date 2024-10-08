#ifndef HASH_FUNC_H
#define HASH_FUNC_H

#include <stdio.h>
#include <stdint.h>

#include "stack.h"

#ifndef HASH_PROTECTION
    #define ON_HASH(...) __VA_ARGS__
    #define IS_HASH_CHANGED(STACK, HASH_FUNC) (IsDataHashChange(STACK, HASH_FUNC) || IsStackHashChange(STACK, HASH_FUNC))
#else 
    #define ON_HASH(...)
#endif 
 
typedef uint64_t HashType;
    
HashType MurMurHash3(const void *key, size_t len, HashType seed);
static inline HashType MurMurScramble(HashType k);

void GetDataHash(Stack *stk, HashType (*HashFunction) (const void *StackElements, size_t len, HashType seed));
void GetStackHash(Stack *stk, HashType (*HashFunction) (const void *Stack, size_t len, HashType seed));

HashType IsDataHashChange(Stack *stk, HashType (*HashFunction) (const void *StackElements, size_t len, HashType seed));
HashType IsStackHashChange(Stack *stk, HashType (*HashFunction) (const void *Stack, size_t len, HashType seed));

#endif