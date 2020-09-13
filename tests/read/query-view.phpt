--TEST--
queryView returns view contents
--FILE--
<?php
require __DIR__ . '/../../couch.php';

echo gettype($couch->queryView(DATABASE, 'recipe-doc', 'recipe-view', [
  'descending'  => 'true',
  'conflicts'   => 'false',
  'update'      => 'true'
]));
?>
--EXPECT--
string