--TEST--
dbExists checks if a database exists
--FILE--
<?php
require_once __DIR__ . '/../../couch.php';

echo gettype($couch->dbExists(DATABASE)) . PHP_EOL;
echo gettype($couch->dbExists('recipes'));
?>
--EXPECT--
boolean
boolean