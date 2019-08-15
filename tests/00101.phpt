--TEST--
createDb throws CouchDbException when parameter is empty
--FILE--
<?php
require_once 'config.php';

try {
    echo $couch->createDb('');
} catch (CouchDbException $exp) {
    echo $exp->getMessage();
}
?>
--EXPECT--
Parameter cannot be empty