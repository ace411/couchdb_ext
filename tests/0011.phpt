--TEST--
_delete method allows for one to delete a database
--FILE--
<?php
require_once 'config.php';

echo gettype($couch->_delete(COUCH_DEL_DB, [
    'database'  => 'testdb'
]));
?>
--EXPECT--
boolean