--TEST--
_delete allows for deletion of single document
--FILE--
<?php
require_once 'config.php';

echo gettype($couch->_delete(COUCH_DEL_DOC, [
    'database'      => 'testdb',
    '_id'           => '@agiroLoki',
    '_rev'          => '1-4739ee06c8d09dbf11f1a82b0a87ad1b'
]));
?>
--EXPECT--
boolean
