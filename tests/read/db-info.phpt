--TEST--
dbInfo outputs database metadata
--FILE--
<?php
require_once __DIR__ . '/../../couch.php';

echo gettype($couch->dbInfo(DATABASE));
?>
--EXPECT--
string