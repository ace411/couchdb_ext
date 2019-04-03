--TEST--
Extension allows one to search a CouchDB database
--FILE--
<?php
$couch = new CouchDb('localhost', '', '', 5984, 60);

echo gettype($couch->search('testdb', [
    'selector'          => [
        'name' => [
            '$regex' => '(?i)ike'
        ] 
    ],
    'fields'            => ['_id', '_rev', 'github'],
    'execution_stats'   => true
]));
?>
--EXPECT--
string