/* sht extension for PHP (c) 2024 Lochemem Bruno Michael */
#ifndef PHP_SHT_H
#define PHP_SHT_H

extern zend_module_entry sht_module_entry;
#define phpext_sht_ptr &sht_module_entry;

#define SHT_VERSION "0.1.0"
#define SHT_AUTHOR "Lochemem Bruno Michael <lochbm@live.com>"

#if defined(ZTS) && defined(COMPILE_DL_SHT)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif
