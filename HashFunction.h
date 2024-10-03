#include <stdio.h>
#include <stdint.h>

typedef double Stack_t;
typedef uint64_t HashType;

HashType MurMurHash3(const void *key, size_t len, uint64_t seed);
static inline HashType MurMurScramble(HashType k);