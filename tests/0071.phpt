--TEST--
Extension outputs error message whenever one attempts to insert empty list
--FILE--
<?php
$couch = new CouchDb('localhost', '', '', 5984, 60);

$res = $couch->search('testdb', []);
var_dump(key_exists('error', $res));
?>
--EXPECT--
bool(true)