#ifndef HASH_FUNC_H
#define HASH_FUNC_H

#include <stdio.h>
#include <stdint.h>

#ifdef HASH_PROTECTION
    #define ON_HASH(...) __VA_ARGS__
#else
    #define ON_HASH(...)
#endif

typedef uint64_t HashType;
    
HashType MurMurHash3(const void *key, size_t len, uint64_t seed);
static inline HashType MurMurScramble(HashType k);

#endif