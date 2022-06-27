--TEST--
connect() conditions methods to return JSON by default
--SKIPIF--
<?php
$basic = CouchDb::connect();
if (!$basic->available()) {
  echo 'skip';
}
?>
--FILE--
<?php
require_once 'config.php';
require_once 'helpers.php';

$basic = configure();

echo jsonType($basic->databases()) ? 'true' : 'false';
?>
--EXPECT--
true