#ifndef HASH_H_
#define HASH_H_

#include <stdint.h>

int      string_cmp(void *key1, void *key2);
uint64_t string_hash(void *key);

#endif
