--TEST--
update method allows one to change multiple documents
--FILE--
<?php
require_once 'config.php';

echo gettype($couch->update('testdb', COUCH_UPDATE_MULTIPLE, [
    [
        '_id'       => '@MH',
        '_rev'      => '5-914786f9b9324e88fca3e81aabca3b45',
        'team'      => 'Miami Heat',
        'arena'     => 'AA Arena'
    ],
    [
        '_id'       => '@SAS',
        '_rev'      => '4-1aae64f22d7b47cba3eef60c2a99343a',
        'team'      => 'San Antonio Spurs',
        'arena'     => 'AT&T Center'
    ]
]));
?>
--EXPECT--
boolean