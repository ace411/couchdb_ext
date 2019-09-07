--TEST--
_delete throws CouchDbException when invalid delete option is detected
--FILE--
<?php
require_once 'config.php';

try {
    echo $couch->_delete(55, [
        'database'  => 'testdb',
        'docs'      => [
            [
                'id'    => 'Fish Fillet',
                'rev'   => '1-6b90b755d8a95b72a9fcc327a4a5e114'
            ]
        ]
    ]);
} catch (CouchDbException $exp) {
    echo $exp->getMessage();
}
?>
--EXPECT--
Invalid option