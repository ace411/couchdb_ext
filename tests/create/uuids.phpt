--TEST--
uuids outputs a list of uuids
--FILE--
<?php
require_once __DIR__ . '/../config.php';

$uuids = $couch->uuids(2);

echo gettype($uuids);
?>
--EXPECT--
string