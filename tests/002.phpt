--TEST--
available() checks if CouchDB is available at a specified address
--FILE--
<?php
$basic = CouchDb::connect();

echo gettype($basic->available());
?>
--EXPECT--
boolean