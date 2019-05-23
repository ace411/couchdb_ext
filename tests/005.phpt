--TEST--
Extension prints all the documents in a specific database
--FILE--
<?php
require_once 'config.php';

echo gettype($couch->allDocs('testdb', [
    'include_docs' => 'true',
    'descending' => 'true'
]));
?>
--EXPECT--
string