--TEST--
keys() lists keys stored in static hashtable
--FILE--
<?php

$table = Sht::create(
  \range(1, 10),
);

$keys = $table->keys();
\asort($keys);

var_dump($keys, \count($keys));
?>
--EXPECT--
array(10) {
  [8]=>
  string(1) "0"
  [0]=>
  string(1) "1"
  [1]=>
  string(1) "2"
  [9]=>
  string(1) "3"
  [2]=>
  string(1) "4"
  [3]=>
  string(1) "5"
  [4]=>
  string(1) "6"
  [5]=>
  string(1) "7"
  [6]=>
  string(1) "8"
  [7]=>
  string(1) "9"
}
int(10)
