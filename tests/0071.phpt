--TEST--
Extension outputs CouchDB error whenever search array is empty
--FILE--
<?php
require_once 'config.php';

$res = json_decode($couch->search('testdb', []), true);
var_dump(key_exists('error', $res));
?>
--EXPECT--
bool(true)