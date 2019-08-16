--TEST--
_delete throws CouchDbException when either _rev or _id keys are missing
--FILE--
<?php
require_once 'config.php';

try {
    echo $couch->_delete(COUCH_DEL_DOC, [
        'database'  => 'testdb'
    ]);
} catch (CouchDbException $exp) {
    echo $exp->getMessage();
}
?>
--EXPECT--
'_id' or '_rev' key is missing