--TEST--
createDdoc throws CouchDbException whenever parameter is missing
--FILE--
<?php
require_once 'config.php';

try {
    echo $couch->createDdoc('', 'githubDoc', []);
} catch (CouchDbException $exp) {
    echo $exp->getMessage();
}
?>
--EXPECT--
Parameters cannot be empty