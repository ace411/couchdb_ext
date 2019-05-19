--TEST--
Extension outputs error message whenever one attempts to insert empty list
--FILE--
<?php
require_once 'config.php';

$res = $couch->search('testdb', []);
var_dump(key_exists('error', $res));
?>
--EXPECT--
bool(true)