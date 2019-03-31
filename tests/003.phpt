--TEST--
Extensions generates arbitrary number of unique identifiers.
--SKIPIF--
<?php
$couch = new CouchDb('localhost', '', '', 5984, 60);
if (!$couch->isAvailable())
    echo 'skip';
?>
--FILE--
<?php
$couch = new CouchDb('localhost', '', '', 5984, 60);

echo gettype($couch->uuids(5));
?>
--EXPECT--
string