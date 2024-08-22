/* sht extension for PHP (c) 2024 Lochemem Bruno Michael */
#ifndef __PHP_SHT_H__
#define __PHP_SHT_H__

#include "php.h"
#include "ext/spl/spl_exceptions.h"
#include "ext/spl/spl_iterators.h"
#include "ext/standard/info.h"
#include "ext/standard/php_array.h"
#include "ext/standard/php_string.h"
#include "ext/standard/php_var.h"
#include "zend_exceptions.h"
#include "sht.c"

/* for compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
  ZEND_PARSE_PARAMETERS_START(0, 0)  \
  ZEND_PARSE_PARAMETERS_END()
#endif

zend_object_handlers sht_object_handlers;

struct php_sht_obj;
typedef struct php_sht_obj php_sht_obj;

/* definition of userspace-bound static hashtable */
struct php_sht_obj
{
  /* static hashtable */
  sht_map *map;
  /* PHP static hashtable object */
  zend_object std;
};

/* instantiate static hashtable object */
static inline php_sht_obj *php_sht_from_obj(zend_object *obj)
{
  return (php_sht_obj *)((char *)obj - XtOffsetOf(php_sht_obj, std));
}

#define SHT_OBJ(zv) php_sht_from_obj(Z_OBJ_P(zv))

/* creates userspace-destined static hashtable object */
static zend_object *php_sht_create_object(zend_class_entry *ce);
/* releases memory allotted to userspace-bound static hashtable object */
static void php_sht_free_object(zend_object *obj);
// static HashTable *php_sht_get_properties(zend_object *obj);

/* creates static hashtable from a PHP hashtable */
static void php_sht_create(INTERNAL_FUNCTION_PARAMETERS);
/* lists keys stored in static hashtable */
static void php_sht_keys(INTERNAL_FUNCTION_PARAMETERS);
/* retrieves value stored in static hashtable associated with specified key */
static void php_sht_get(INTERNAL_FUNCTION_PARAMETERS);

zend_class_entry *php_sht_ce, *php_sht_exception_ce;

#define SHT_THROW(message) zend_throw_exception(php_sht_exception_ce, message, 0);

#endif
