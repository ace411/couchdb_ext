--TEST--
Extension prints all the databases currently available
--FILE--
<?php
require_once 'config.php';

echo gettype($couch->allDbs());
?>
--EXPECT--
string