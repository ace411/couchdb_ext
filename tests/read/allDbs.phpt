--TEST--
allDbs outputs list of all available databases
--FILE--
<?php
require_once __DIR__ . '/../../couch.php';

echo gettype($couch->allDbs());
?>
--EXPECT--
string