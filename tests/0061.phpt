--TEST--
Extension throws CouchDbException whenever a parameter is empty
--FILE--
<?php
require_once 'config.php';

try {
    echo $couch->insertDocs('testdb', []);
} catch (CouchDbException $exp) {
    echo $exp->getMessage();
}
?>
--EXPECT--
Parameters cannot be empty