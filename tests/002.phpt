--TEST--
Check if CouchDB is available
--FILE--
<?php
require_once 'config.php';

echo gettype($couch->isAvailable());
?>
--EXPECT--
boolean
