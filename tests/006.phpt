--TEST--
Extension allows for storage of data in CouchDB database
--FILE--
<?php
$couch = new CouchDb('localhost', '', '', 5984, 60);

echo gettype($couch->insertDocs('testdb', [
    'docs' => [
        [
            'name'      => 'Michael',
            'github'    => '@ace411',
            'twitter'   => '@agiroLoki'
        ]
    ]
]));
?>
--EXPECT-- 
boolean