--TEST--
available() checks if CouchDB is available at a specified address
--FILE--
<?php
require_once 'config.php';

$basic = configure();

echo gettype($basic->available());
?>
--EXPECT--
boolean