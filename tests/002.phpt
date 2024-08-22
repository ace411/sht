--TEST--
get() retrieves value stored in static hashtable associated with specified key
--FILE--
<?php

$range = function (int $min, int $max) {
  $idx = $min - 1;
  $acc = [];

  while (++$idx <= $max) {
    $acc[\sprintf('key-%d', $idx)] = $idx;
  }

  return $acc;
};

$table = Sht::create($range(1, 300));

var_dump(
  $table->get('key-10'),
  $table->get('key-288'),
  $table->get('key-5000'),
);
?>
--EXPECT--
int(10)
int(288)
NULL