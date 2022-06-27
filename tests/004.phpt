--TEST--
databases() returns list of all databases
--SKIPIF--
<?php
$basic = CouchDb::connect();
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