--TEST--
createDb creates new database
--FILE--
<?php
require_once __DIR__ . '/../../couch.php';

$create = $couch->createDb('testdb');

echo gettype($create);
?>
--EXPECT--
boolean