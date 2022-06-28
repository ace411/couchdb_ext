--TEST--
databases() returns list of all databases
--SKIPIF--
<?php
require_once 'config.php';

$basic = configure();
if (!$basic->available()) {
  echo 'skip';
}
?>
--FILE--
<?php
require 'config.php';

$basic = configure(
  [
    'type' => CouchDb::RETURN_ARRAY,
  ],
);

echo gettype($basic->databases());
?>
--EXPECT--
array