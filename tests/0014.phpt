--TEST--
update method allows one to change single document
--FILE--
<?php
require_once 'config.php';

echo gettype($couch->update('testdb', COUCH_UPDATE_SINGLE, [
    '_id'       => '@ace411',
    '_rev'      => '1-4739ee06c8d09dbf11f1a82b0a87ad1b',
    'doc'       => [
        'names'     => 'Michael Bruno',
        'twitter'   => '@agiroLoki',
        'medium'    => '@agiroLoki'
    ] 
]));
?>
--EXPECT--
boolean