--TEST--
Check if CouchDB is available
--SKIPIF--
<?php
$couch = new CouchDb('localhost', '', '', 5984, 60);
if (!$couch->isAvailable())
    echo 'skip';
?>
--FILE--
<?php
$couch = new CouchDb('localhost', '', '', 5984, 60);
echo $couch->isAvailable();
?>
--EXPECT--
1