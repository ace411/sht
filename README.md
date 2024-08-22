# SHT

A simple static hashtable for PHP.

## Rationale

There already exists a [static hashtable](https://github.com/sevenval/SHMT) implementation in PHP—and a rather good one at that. The goal of the project documented in this text is to provide a single-write, multiple-read structure that lives in memory—and not exclusively in adjacent specialized files—that computes indexes in accordance with the conventions of the performant [xxhash64 algorithm](https://xxhash.com/doc/v0.8.2/index.html).

## Requirements

- PHP 8 or newer

## Installation

Before attempting to install ext-sht, ascertain that you have the aforelisted requirement(s) installed on your machine. Proceed to type the following in a console of your choosing upon ascertainment of the existence of the project dependencies.

```sh
$ git clone https://github.com/ace411/sht.git <dir>
$ cd <dir>
$ phpize
$ ./configure --enable-sht
$ make & sudo make install
```

> Remember to configure the extension in your `php.ini` file by appending the entry `extension=sht` to it.

## API Synopsis

```php

class Sht
{

  /* public methods */
  public static create(array $entries): Sht
  public get(string $key): mixed
  public keys(): array
}
```

### `Sht::create`

```php
public static create(array $entries): Sht
```

Creates a static hashtable from a PHP array.

**Parameters**

- **entries** (array) - a PHP array the contents of which are to be inserted in a static hashtable

```php
$table = Sht::create(
  \range(1, 1_000),
);

var_dump($table);
```

The example above will produce output similar to that in the snippet to follow.

```
object(Sht)#2 (0) {
}
```

### `Sht::get`

```php
public get(string $key): mixed
```

Retrieves value stored in static hashtable associated with specified key.

> The function returns `null` if a specified key does not exist in a static hashtable.

**Parameters**

- **key** (string) - The key the data associated with which is to be retrieved

```php
$range = function (int $min, int $max) {
  $idx = $min - 1;
  $acc = [];

  while (++$idx <= $max) {
    $acc[\sprintf('key-%d', $idx)] = $idx;
  }

  return $acc;
};

$table = Sht::create(
  $range(1, 500),
);

var_dump(
  $table->get('key-50'),    // 50
  $table->get('key-488'),   // 488
  $table->get('key-2000'),  // null
);
```

The example above will produce output similar to that in the snippet to follow.

```
int(50)
int(488)
NULL
```

### `Sht::keys`

```php
public keys(): array
```

Lists keys stored in a static hashtable.

**Parameters**

None.

```php
$table = Sht::create(
  \range(1, 5),
);

var_dump(
  $table->keys(),
);
```

The example above will produce output similar to that in the snippet to follow.

```
array(5) {
  [0]=>
  string(1) "0"
  [1]=>
  string(1) "1"
  [2]=>
  string(1) "2"
  [3]=>
  string(1) "3"
  [4]=>
  string(1) "4"
}
```
