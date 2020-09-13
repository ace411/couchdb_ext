--TEST--
couchdb_ext works
--SKIPIF--
<?php 
if (!extension_loaded('couchdb')) 
    echo 'skip';
?>
--FILE--
<?php
require_once __DIR__ . '/../couch.php';

echo $couch instanceof CouchDb ? 'true' : 'false';
?>
--EXPECT--
true
