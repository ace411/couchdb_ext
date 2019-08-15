--TEST--
_delete throws CouchDbException when database key is not provided
--FILE--
<?php
require_once 'config.php';

try {
    echo $couch->_delete(COUCH_DEL_DB, [
        '_rev'  => 'abc123',
        '_id'   => 'def456'
    ]);
} catch (CouchDbException $exp) {
    echo $exp->getMessage();
}
?>
--EXPECT--
'database' key is missing