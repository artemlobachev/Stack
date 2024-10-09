#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "HashProtection.h"


static inline HashType MurMurScramble(HashType k)
{
    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;

    return k;
}

HashType MurMurHash3(const void *key, size_t len, HashType seed)
{
    assert(key && len > 0);
    
    HashType h = seed;
    HashType k;

    for (size_t i = len >> 2; i; i--) 
    {
        memcpy(&k, key, sizeof(HashType));
        key += sizeof(HashType);
        h ^= MurMurScramble(k);
        h = (h << 13) | (h >> 19);
        h = h * 5 + 0xe6546b64;
    }
 
    k = 0;
    for (size_t i = len & 3; i; i--)
    {
        k <<= 8;
        k |=  *(HashType *)(key + (i - 1));
    }   

    h ^= MurMurScramble(k);
    h ^= len;
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16; 

    return h; 
} 

HashType IsDataHashChange(Stack *stk, HashType (*HashFunction) (const void *StackElements, size_t len, HashType seed))
{
    printf("%#x %#x ", stk->DataHash, HashFunction((const void*)stk->StackElements, stk->capacity, 0));
    return (stk->DataHash != HashFunction((const void*)stk->StackElements, stk->capacity, 0));
}  

HashType IsStackHashChange(Stack *stk, HashType (*HashFunction) (const void *Stack, size_t len, HashType seed))
{
    return (stk->StackHash != HashFunction((const void*)stk, 1, 0));
}

void GetDataHash(Stack *stk, HashType (*HashFunction) (const void *StackElements, size_t len, HashType seed))
{
    stk->DataHash = HashFunction((const void*)stk->StackElements, stk->capacity, 0);
}

void GetStackHash(Stack *stk, HashType (*HashFunction) (const void *Stack, size_t len, HashType seed))
{
    stk->StackHash = HashFunction((const void*)stk, 1, 0);
}