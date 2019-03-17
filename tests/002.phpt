--TEST--
Check if CouchDB is available
--SKIPIF--
<?php
$couch = new CouchDb();
if (!$couch->isAvailable()) {
    echo 'skip';
}
?>
--FILE--
<?php
$couch = new CouchDb();
echo $couch->isAvailable();
?>
--EXPECT--
1