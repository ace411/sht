/* sht extension for PHP (c) 2024 Lochemem Bruno Michael */
#ifndef __XXHASH_H__
#define __XXHASH_H__

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

/* maximum stripe size */
#define XX_BUFFER_SIZE 32
/* non-endian-aware primes for xxhash64 algorithm */
#define XX_PRIME_I 11400714785074694791ULL
#define XX_PRIME_II 14029467366897019727ULL
#define XX_PRIME_III 1609587929392839161ULL
#define XX_PRIME_IV 9650029242287828579ULL
#define XX_PRIME_V 2870177450012600261ULL

struct xx_hash_t;
typedef struct xx_hash_t xx_hash_t;

/* quintessential xxhash64 computation structure */
struct xx_hash_t
{
  /* xxhash lane accumulators (each accumulator stores an unsigned 64 bit value) */
  __uint64_t state[4];
  /* xxhash input buffer size */
  __uint64_t size;
  /* xxhash input buffer length */
  __uint64_t length;
  /* xxhash stripe (divided into 4 lanes of 8 bytes each) */
  unsigned char buffer[XX_BUFFER_SIZE];
};

/* initializes xxhash64 structure with arbitrary seed */
static xx_hash_t *xx_hash_init(__uint64_t seed);
/* performs relevant multi-byte left rotation */
static __uint64_t rotate_left(__uint64_t x, unsigned char bits);
/* updates a single accumulator */
static __uint64_t process_single(__uint64_t prev, __uint64_t input);
/* processes stripe; updates relevant accumulators */
static void process(void *data, __uint64_t *s1, __uint64_t *s2, __uint64_t *s3, __uint64_t *s4);

/* increments size of input buffer (increases hash entropy) */
static bool xx_hash_add(xx_hash_t *hash, void *input, __uint64_t length);
/* converges xxhash64 structure into a unique numeric value (generates hash) */
static __uint64_t xx_hash_final(xx_hash_t *hash);
/* creates unique hash from arbitrary string input and seed */
static __uint64_t xx64_hash(void *input, __uint64_t length, __uint64_t seed);

#endif
