--TEST--
Extension outputs contents of view query
--FILE--
<?php
require_once 'config.php';

echo gettype($couch->queryView('testdb', 'userDoc', 'github-view', [
    'conflicts'     => 'false',
    'descending'    => 'true'
]));
?>
--EXPECT--
string