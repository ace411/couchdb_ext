--TEST--
allDocs method throws CouchDbException if parameter(s) is|are empty
--FILE--
<?php
require_once 'config.php';

try {
    echo $couch->allDocs('testdb', []);
} catch (CouchDbException $exp) {
    echo $exp->getMessage();
}
?>
--EXPECT--
Parameters cannot be empty