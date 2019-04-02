--TEST--
Check if CouchDB is available
--FILE--
<?php
$couch = new CouchDb('localhost', '', '', 5984, 60);
echo gettype($couch->isAvailable());
?>
--EXPECT--
boolean