--TEST--
Check if couchdb_ext works
--SKIPIF--
<?php 
if (!extension_loaded('couchdb_ext')) 
    echo 'skip';
?>
--FILE--
<?php
$couch = new CouchDb('localhost', '', '', 5984, 60);
echo gettype($couch);
?>
--EXPECT--
object
