--TEST--
createIndex throws exception when partial_filter_selector key is absent
--FILE--
<?php
require_once __DIR__ . '/../../couch.php';

echo gettype($couch->createIndex(DATABASE, [
  'index' => ['foo'],
  'type'  => 'json'
]));
?>
--EXPECT--
boolean