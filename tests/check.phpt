--TEST--
couchdb_ext works
--SKIPIF--
<?php 
if (!extension_loaded('couchdb_ext')) 
    echo 'skip';
?>
--FILE--
<?php
require_once 'config.php';

echo $couch instanceof CouchDb ? 'true' : 'false';
?>
--EXPECT--
true
