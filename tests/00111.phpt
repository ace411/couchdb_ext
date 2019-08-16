--TEST--
_delete throws CouchDbException when empty parameter list is supplied
--FILE--
<?php
require_once 'config.php';

try {
    echo $couch->_delete(COUCH_DEL_DB, []);
} catch (CouchDbException $exp) {
    echo $exp->getMessage();
}
?>
--EXPECT--
Options list cannot be empty