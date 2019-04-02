--TEST--
Extension prints all the documents in a specific database
--FILE--
<?php
$couch = new CouchDb('localhost', '', '', 5984, 60);

echo gettype($couch->allDocs('testdb', [
    'include_docs' => 'true',
    'descending' => 'true'
]));
?>
--EXPECT--
string