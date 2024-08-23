/* sht extension for PHP (c) 2024 Lochemem Bruno Michael */
#ifndef __SHT_H__
#define __SHT_H__

/* default seed for extension */
#define SHT_DEFAULT_SEED 256UL

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "xxhash.c"

struct sht_map;
struct sht_map_entry;

typedef struct sht_map sht_map;
typedef struct sht_map_entry sht_map_entry;

/* static hashtable  */
struct sht_map
{
  /* static hashtable entries */
  sht_map_entry *entries;
  /* static hashtable capacity (maximum size) */
  size_t capacity;
  /* static hashtable element count (at max. capacity, capacity = count) */
  size_t count;
};

/* static hashtable entry (key-value pair) */
struct sht_map_entry
{
  /* static hashtable entry key */
  const unsigned char *key;
  /* static hashtable key-assigned value */
  void *value;
};

/* initializes static hashtable with arbitrary capacity */
static sht_map *sht_map_init(size_t capacity);
/* inserts arbitrarily defined key-value pair into hashtable */
static unsigned char *sht_map_insert(sht_map *map, const unsigned char *key, void *value);
/* searches static hashtable for value that corresponds to specified key */
static void *sht_map_search(sht_map *map, const unsigned char *key);
/* inserts value into static hashtable without incrementing its size */
static unsigned char *sht_map_entry_add(sht_map_entry *entries, size_t capacity, const unsigned char *key,
                                        void *value, size_t *length);
/* releases memory allotted to static hashtable */
static void sht_map_free(sht_map *map);

#endif
