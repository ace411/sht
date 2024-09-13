/* sht extension for PHP (c) 2024 Lochemem Bruno Michael */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "src/php_sht.c"
#include "php_sht.h"
#include "sht_arginfo.h"

/* {{{ proto Sht Sht::create( array entries ) */
PHP_METHOD(Sht, create)
{
  php_sht_create(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto array Sht::keys() */
PHP_METHOD(Sht, keys)
{
  php_sht_keys(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto mixed Sht::get( string key ) */
PHP_METHOD(Sht, get)
{
  php_sht_get(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(sht)
{
  zend_class_entry sht_ce, sht_exception_ce;

  INIT_CLASS_ENTRY(sht_ce, "Sht", class_Sht_methods);
  INIT_CLASS_ENTRY(sht_exception_ce, "ShtException", NULL);

  php_sht_ce = zend_register_internal_class(&sht_ce);
  php_sht_ce->create_object = php_sht_create_object;

  memcpy(&sht_object_handlers, zend_get_std_object_handlers(), sizeof(sht_object_handlers));
  sht_object_handlers.free_obj = php_sht_free_object;

#ifdef HAVE_SPL
  php_sht_exception_ce = zend_register_internal_class_ex(&sht_exception_ce, spl_ce_RuntimeException);
#else
  php_sht_exception_ce = zend_register_internal_class_ex(&sht_exception_ce, zend_exception_get_default());
#endif

  return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(sht)
{
#if defined(ZTS) && defined(COMPILE_DL_SHT)
  ZEND_TSRMLS_CACHE_UPDATE();
#endif

  return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(sht)
{
  php_info_print_table_start();
  php_info_print_table_header(2, "sht support", "enabled");
  php_info_print_table_header(2, "sht version", SHT_VERSION);
  php_info_print_table_header(2, "sht author", SHT_AUTHOR);
  php_info_print_table_end();
}
/* }}} */

/* {{{ sht_module_entry */
zend_module_entry sht_module_entry = {
    STANDARD_MODULE_HEADER,
    "sht",
    class_Sht_methods,
    PHP_MINIT(sht),
    NULL,
    PHP_RINIT(sht),
    NULL,
    PHP_MINFO(sht),
    SHT_VERSION,
    STANDARD_MODULE_PROPERTIES};
/* }}} */

#ifdef COMPILE_DL_SHT
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(sht)
#endif
