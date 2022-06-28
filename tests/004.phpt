--TEST--
databases() returns list of all databases
--SKIPIF--
<?php
require_once 'auth.php';

if (!$session->available()) {
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