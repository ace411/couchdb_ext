--TEST--
isAvailable method checks if CouchDb instance is running
--FILE--
<?php
require_once 'config.php';

echo gettype($couch->isAvailable());
?>
--EXPECT--
boolean