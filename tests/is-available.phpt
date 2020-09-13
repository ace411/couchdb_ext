--TEST--
isAvailable method checks if CouchDb instance is running
--FILE--
<?php
require_once __DIR__ . '/../couch.php';

echo gettype($couch->isAvailable());
?>
--EXPECT--
boolean