/* sht extension for PHP (c) 2024 Lochemem Bruno Michael */
#include "xxhash.h"

static xx_hash_t *xx_hash_init(__uint64_t seed)
{
  xx_hash_t hash;
  hash.length = 0;
  hash.size = 0;
  hash.state[0] = seed + XX_PRIME_I + XX_PRIME_II;
  hash.state[1] = seed + XX_PRIME_II;
  hash.state[2] = seed;
  hash.state[3] = seed - XX_PRIME_I;

  xx_hash_t *result = &hash;

  return result;
}
static void xx_hash_free(xx_hash_t *hash)
{
  free(hash);
}
static __uint64_t rotate_left(__uint64_t x, unsigned char bits)
{
  // bitwise OR of x shifted left by n bits and x shifted right by 64 - n bits
  return (x << bits) | (x >> (64 - bits));
}
static __uint64_t process_single(__uint64_t prev, __uint64_t input)
{
  // ensure that any bit from input lane impacts several bits in accumulator
  return rotate_left(prev + input * XX_PRIME_II, 31) * XX_PRIME_I;
}
static void process(void *data, __uint64_t *s1, __uint64_t *s2, __uint64_t *s3, __uint64_t *s4)
{
  const __uint64_t *block = (const __uint64_t *)data;
  __uint64_t r1 = process_single(*s1, block[0]),
             r2 = process_single(*s2, block[1]),
             r3 = process_single(*s3, block[2]),
             r4 = process_single(*s4, block[3]);

  *s1 = r1;
  *s2 = r2;
  *s3 = r3;
  *s4 = r4;
}

static bool xx_hash_add(xx_hash_t *hash, void *input, __uint64_t length)
{
  if (input == NULL || length == 0)
  {
    return false;
  }

  hash->length += length;
  const unsigned char *data = (const unsigned char *)input;

  if (hash->size + length < XX_BUFFER_SIZE)
  {
    while (length-- > 0)
    {
      hash->buffer[hash->size++] = *data++;
    }

    return true;
  }

  const unsigned char *stop = data + length;
  const unsigned char *stopBlock = stop - XX_BUFFER_SIZE;

  if (hash->size > 0)
  {
    while (hash->size < XX_BUFFER_SIZE)
    {
      hash->buffer[hash->size++] = *data++;
    }

    process(hash->buffer, &hash->state[0], &hash->state[1], &hash->state[2], &hash->state[3]);
  }

  __uint64_t s1 = hash->state[0],
             s2 = hash->state[1],
             s3 = hash->state[2],
             s4 = hash->state[3];

  while (data <= stopBlock)
  {
    process((void *)data, &s1, &s2, &s3, &s4);
    data += 32;
  }

  hash->state[0] = s1;
  hash->state[1] = s2;
  hash->state[2] = s3;
  hash->state[3] = s4;

  hash->size = stop - data;

  for (__uint64_t idx = 0; idx < hash->size; idx++)
  {
    hash->buffer[idx] = data[idx];
  }

  return true;
}
static __uint64_t xx_hash_final(xx_hash_t *hash)
{
  __uint64_t result, xsum = XX_PRIME_I + XX_PRIME_IV;

  // converge accumulators into 64 bit value
  if (hash->length >= XX_BUFFER_SIZE)
  {
    result = rotate_left(hash->state[0], 1) +
             rotate_left(hash->state[1], 7) +
             rotate_left(hash->state[2], 12) +
             rotate_left(hash->state[3], 18);

    result ^= process_single(0, hash->state[0]);
    result *= xsum;
    result ^= process_single(0, hash->state[1]);
    result *= xsum;
    result ^= process_single(0, hash->state[2]);
    result *= xsum;
    result ^= process_single(0, hash->state[3]);
    result *= xsum;
  }
  else
  {
    // if the input is too small, use a single accumulator (no stripes)
    result = hash->state[2] + XX_PRIME_V;
  }

  // add input length
  result += hash->length;

  const unsigned char *data = hash->buffer;
  const unsigned char *stop = data + hash->size;

  // consume remaining input (up to 31 bytes remaining)
  // ensure that all the buffer input is included in final mix

  // process residual 8-byte chunks
  for (; data + 8 <= stop; data += 8)
  {
    result = rotate_left(result ^ process_single(0, *(__uint64_t *)data), 27) * xsum;
  }

  // process residual 4-byte chunks
  if (data + 4 <= stop)
  {
    result = rotate_left(result ^ *(__uint64_t *)data * XX_PRIME_I, 23) * (XX_PRIME_II + XX_PRIME_III);
  }

  // process any residual data smaller than 4 bytes
  while (data != stop)
  {
    result = rotate_left(result ^ (*data++) * XX_PRIME_V, 11) * XX_PRIME_I;
  }

  // execute final mix (avalanche)
  result ^= result >> 33;
  result *= XX_PRIME_II;
  result ^= result >> 29;
  result *= XX_PRIME_III;
  result ^= result >> 32;

  // return non-endian-aware result

  return result;
}

static __uint64_t xx64_hash(void *input, __uint64_t length, __uint64_t seed)
{
  xx_hash_t *hash = xx_hash_init(seed);
  xx_hash_add(hash, input, length);

  __uint64_t result = xx_hash_final(hash);

  return result;
}
