--TEST--
Extension returns false whenever one attempts to insert empty list
--FILE--
<?php
require_once 'config.php';

$insert = $couch->insertDocs('testdb', []);
var_dump($insert);
?>
--EXPECT--
bool(false)