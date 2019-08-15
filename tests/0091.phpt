--TEST--
queryView throws CouchDbException when parameters are empty
--FILE--
<?php
require_once 'config.php';

try {
    echo $couch->queryView('', '', '', [
        'include_docs'  => 'true',
        'group'         => 'false'
    ]);
} catch (CouchDbException $exp) {
    echo $exp->getMessage();
}
?>
--EXPECT--
Parameters cannot be empty