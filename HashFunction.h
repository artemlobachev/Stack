#ifndef HASH_FUNC_H
#define HASH_FUNC_H

#include <stdio.h>
#include <stdint.h>

typedef uint64_t HashType;
   
HashType MurMurHash3(const void *key, size_t len, uint64_t seed);
static inline HashType MurMurScramble(HashType k);

#endif