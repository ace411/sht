dnl sht extension for PHP (c) 2024 Lochemem Bruno Michael
PHP_ARG_ENABLE(sht, whether or not to enable sht support,
[ --enable-sht   Enable sht support], no)

if test "$PHP_SHT" != "no"; then
  AC_DEFINE(HAVE_SHT, 1, [ Have sht support ])

  PHP_NEW_EXTENSION(sht, sht.c, $ext_shared)
fi
