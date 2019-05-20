--TEST--
_delete method returns false when empty parameter list is supplied
--FILE--
<?php
require_once 'config.php';

var_dump($couch->_delete(COUCH_DEL_DB, []));
?>
--EXPECT--
bool(false)