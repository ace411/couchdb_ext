--TEST--
update method allows one to change multiple documents
--FILE--
<?php
require_once 'config.php';

echo gettype($couch->update('testdb', COUCH_UPDATE_MULTIPLE, [
    [
        '_id'       => '@MH',
        'arena'     => 'AA Arena',
        'team'      => 'Miami Heat'
    ],
    [
        '_id'       => '@SAS',
        'arena'     => 'AT&T Center',
        'team'      => 'San Antonio Spurs'
    ]
]));
?>
--EXPECT--
boolean