/* sht extension for PHP (c) 2024 Lochemem Bruno Michael */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Sht_create, 0, 0, 1)
ZEND_ARG_TYPE_INFO(0, entries, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Sht_keys, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Sht_get, 0, 0, 1)
ZEND_ARG_TYPE_INFO(0, key, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_METHOD(Sht, create);
ZEND_METHOD(Sht, keys);
ZEND_METHOD(Sht, get);

static const zend_function_entry class_Sht_methods[] = {
    PHP_ME(Sht, create, arginfo_class_Sht_create, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
        PHP_ME(Sht, keys, arginfo_class_Sht_keys, ZEND_ACC_PUBLIC)
            PHP_ME(Sht, get, arginfo_class_Sht_get, ZEND_ACC_PUBLIC)
                PHP_FE_END};
