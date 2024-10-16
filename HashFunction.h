#ifndef HASH_FUNC_H
#define HASH_FUNC_H

#include <stdio.h>
#include <stdint.h>

typedef int64_t HashType;
    
HashType MurMurHash3(const void *key, size_t len, HashType seed);
static inline HashType MurMurScramble(HashType k);

#endif