/* sht extension for PHP (c) 2024 Lochemem Bruno Michael */
#include "php_sht.h"

static zend_object *php_sht_create_object(zend_class_entry *ce)
{
  php_sht_obj *obj = zend_object_alloc(sizeof(php_sht_obj), php_sht_ce);
  zend_object_std_init(&obj->std, php_sht_ce);

  obj->std.handlers = &sht_object_handlers;
  obj->map = NULL;

  return &obj->std;
}
static void php_sht_free_object(zend_object *obj)
{
  php_sht_obj *intern = php_sht_from_obj(obj);

  if (intern->map)
  {
    sht_map_free(intern->map);
  }

  zend_object_std_dtor(obj);
  efree(intern);
}
static HashTable *php_sht_get_properties(zend_object *obj)
{
  php_sht_obj *intern = php_sht_from_obj(obj);
  zval config;
  HashTable *props;

  array_init(&config);

  if (intern->map == NULL || intern->map->capacity == 0)
  {
    goto define;
  }

  for (size_t idx = 0; idx < intern->map->capacity; idx++)
  {
    add_assoc_zval(&config, intern->map->entries[idx].key,
                   (zval *)intern->map->entries[idx].value);
  }

define:
  props = Z_ARRVAL(config);

  return props;
}

static void php_sht_create(INTERNAL_FUNCTION_PARAMETERS)
{
  zval *entries;
  php_sht_obj *sht;

  ZEND_PARSE_PARAMETERS_START(1, 1)
  Z_PARAM_ARRAY(entries)
  ZEND_PARSE_PARAMETERS_END();

  __uint64_t count = zend_hash_num_elements(Z_ARRVAL_P(entries));

  if (count < 1)
  {
    SHT_THROW("Cannot create static hashtable from map with no entries");
    RETURN_NULL();
  }

  object_init_ex(return_value, php_sht_ce);
  sht = SHT_OBJ(return_value);

  sht_map *map = sht_map_init(count);

  if (map == NULL)
  {
    SHT_THROW("Cannot allocate memory for static hashtable creation");
    RETURN_NULL();
  }

  zend_string *key = NULL;
  zend_ulong idx;
  zval *value = NULL;

  // populate hashtable
  ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(entries), idx, key, value)
  {
    ZVAL_DEREF(value);

    if (key)
    {
      // handle string indexes
      char skey[ZSTR_LEN(key) + 1];
      sprintf(skey, "%s", ZSTR_VAL(key));

      sht_map_insert(map, skey, value);
    }
    else
    {
      // handle numeric keys
      char skey[sizeof(zend_ulong)];
      sprintf(skey, "%lu", idx);

      sht_map_insert(map, skey, value);
    }
  }
  ZEND_HASH_FOREACH_END();

  sht->map = map;

  zval_ptr_dtor(entries);
}
static void php_sht_keys(INTERNAL_FUNCTION_PARAMETERS)
{
  zval retval;
  zval *obj = getThis();
  php_sht_obj *this;

  ZEND_PARSE_PARAMETERS_NONE();

  this = SHT_OBJ(obj);
  array_init(&retval);

  for (size_t idx = 0; idx < this->map->capacity; idx++)
  {
    if (this->map->entries[idx].key)
    {
      add_index_string(&retval, idx, this->map->entries[idx].key);
    }
  }

  RETURN_ZVAL(&retval, 1, 0);
}
static void php_sht_get(INTERNAL_FUNCTION_PARAMETERS)
{
  zend_string *key;
  zval *obj = getThis();
  php_sht_obj *this;

  ZEND_PARSE_PARAMETERS_START(1, 1)
  Z_PARAM_STR(key)
  ZEND_PARSE_PARAMETERS_END();

  this = SHT_OBJ(obj);
  void *value = sht_map_search(this->map, ZSTR_VAL(key));

  if (value == NULL)
  {
    RETURN_NULL();
  }
  else
  {
    zval *retval = (zval *)value;

    RETURN_ZVAL(retval, 1, 0);
  }

  zend_string_release(key);
}
