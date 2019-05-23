--TEST--
Extensions generates arbitrary number of unique identifiers.
--FILE--
<?php
require_once 'config.php';

echo gettype($couch->uuids(5));
?>
--EXPECT--
string
