--TEST--
available() checks if CouchDB is available at a specified address
--FILE--
<?php
require_once 'config.php';

$basic = configure();

try {
  echo gettype($basic->available());
} catch (CouchDbException $_) {
  echo 'boolean';
}
?>
--EXPECT--
boolean