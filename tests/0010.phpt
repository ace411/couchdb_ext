--TEST--
Creating a database yields a boolean value
--FILE--
<?php
require_once 'config.php';

echo gettype($couch->createDb('testdb'));
?>
--EXPECT--
boolean