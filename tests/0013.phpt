--TEST--
_delete method deletes database
--FILE--
<?php
require_once 'config.php';

echo gettype($couch->_delete(COUCH_DEL_DB, ['database' => '_testdb']));
?>
--EXPECT--
boolean