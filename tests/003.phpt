--TEST--
keys() lists keys stored in static hashtable
--FILE--
<?php

$data  = \range(1, 10);
$table = Sht::create($data);

$keys = \array_map(
  fn (int $key) => \sprintf('%d', $key),
  $data,
);

var_dump(
  \array_reduce(
    $table->keys(),
    function (bool $exists, string $key) use ($keys) {
      if (!\in_array($key, $keys)) {
        $exists = false;
      }

      return $exists;
    },
    true,
  ),
);
?>
--EXPECT--
bool(true)
