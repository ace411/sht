--TEST--
create() creates static hashtable from a PHP array
--FILE--
<?php

$table = Sht::create(
  \range(1, 500),
);

var_dump($table instanceof Sht);
?>
--EXPECT--
bool(true)