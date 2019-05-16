--TEST--
Extension returns false whenever one attempts to insert empty list
--FILE--
<?php
$couch = new CouchDb('localhost', '', '', 5984, 60);

$insert = $couch->insertDocs('testdb', []);
var_dump($insert);
?>
--EXPECT--
bool(false)