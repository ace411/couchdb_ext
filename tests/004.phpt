--TEST--
Extension prints all the databases currently available
--FILE--
<?php
$couch = new CouchDb('localhost', '', '', 5984, 60);

echo gettype($couch->allDbs());
?>
--EXPECT--
string