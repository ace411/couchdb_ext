--TEST--
getDoc outputs document info
--FILE--
<?php
require_once __DIR__ . '/../config.php';

echo gettype($couch->getDoc(DATABASE, 'FishStew'));
?>
--EXPECT--
string