--TEST--
Extension allows one to create design document
--FILE--
<?php
require_once 'config.php';

echo gettype($couch->createDdoc('testdb', 'githubDoc', [
    'views' => [
        'github' => [
            'map' => 'function (doc) { if (doc.name && doc.github) { emit(doc._id, { name: doc.name, acc: doc.github }) } }'
        ]
    ]
]));
?>
--EXPECT--
boolean