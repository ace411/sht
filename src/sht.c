/* sht extension for PHP (c) 2024 Lochemem Bruno Michael */
#include "sht.h"

static sht_map *sht_map_init(size_t capacity)
{
#ifdef PHP_H
  sht_map *map = emalloc(sizeof(sht_map));
#else
  sht_map *map = malloc(sizeof(sht_map));
#endif
  if (map == NULL)
  {
    return NULL;
  }

  map->count = 0;
  map->capacity = capacity;

#ifdef PHP_H
  map->entries = ecalloc(capacity, sizeof(sht_map_entry));
#else
  map->entries = calloc(capacity, sizeof(sht_map_entry));
#endif

  if (map->entries == NULL)
  {
#ifdef PHP_H
    efree(map);
#else
    free(map);
#endif

    return NULL;
  }

  return map;
}
static void sht_map_free(sht_map *map)
{
  for (size_t i = 0; i < map->capacity; i++)
  {
#ifndef PHP_H
    free((void *)map->entries[i].key);
#endif
  }

#ifdef PHP_H
  efree(map->entries);
  efree(map);
#else
  free(map->entries);
  free(map);
#endif
}

static unsigned char *sht_map_insert(sht_map *map, const unsigned char *key, void *value)
{
  // ensure that capacity is not exceeded
  if (map->count > map->capacity)
  {
    return NULL;
  }

  if (map->entries)
  {
    return sht_map_entry_add(map->entries, map->capacity, key,
                             value, &map->count);
  }

  return NULL;
}
static unsigned char *sht_map_entry_add(sht_map_entry *entries, size_t capacity, const unsigned char *key,
                                        void *value, size_t *length)
{
  __uint64_t hash = xx64_hash((void *)key, strlen(key), SHT_DEFAULT_SEED);
  size_t idx = (size_t)(hash & (__uint64_t)(capacity - 1));

  size_t count = 0;

  if (entries[idx].key == NULL)
  {
    goto set;
  }
  else
  {
    // linear probe
    while (entries[idx].key)
    {
      if (strcmp(key, entries[idx].key) == 0)
      {
        entries[idx].key = value;

        return (unsigned char *)entries[idx].key;
      }

      idx++;

      if (idx >= capacity)
      {
        idx = 0;
      }
    }
  }

set:
  if (length)
  {
    key = strdup(key);

    if (key == NULL)
    {
      return NULL;
    }

    // free(key);

    (*length)++;
  }

  entries[idx].key = (unsigned char *)key;
  entries[idx].value = value;

  return (unsigned char *)key;
}
static void *sht_map_search(sht_map *map, const unsigned char *key)
{
  __uint64_t hash = xx64_hash((void *)key, strlen(key), SHT_DEFAULT_SEED);
  size_t idx = (size_t)(hash & (__uint64_t)(map->capacity - 1));
  size_t count = 0;

  if (map->entries[idx].key == NULL)
  {
    while (idx < map->capacity + idx)
    {
      if (strcmp(key, map->entries[idx].key) == 0)
      {
        return map->entries[idx].value;
      }

      idx++;

      if (idx >= map->capacity)
      {
        idx = 0;
      }
    }
  }
  else
  {
    while (map->entries[idx].key)
    {
      if (strcmp(key, map->entries[idx].key) == 0)
      {
        return map->entries[idx].value;
      }

      idx++;
      count++;

      if (idx >= map->capacity)
      {
        idx = 0;
      }

      if (count == map->capacity)
      {
        break;
      }
    }
  }

  return NULL;
}
