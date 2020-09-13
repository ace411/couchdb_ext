--TEST--
createIndex creates CouchDB index
--FILE--
<?php
require_once __DIR__ . '/../../couch.php';

echo gettype(
  $couch->createIndex(DATABASE, [
    'index' => [
      'fields' => ['servings', '_id', 'subtitle']
    ],
    'ddoc'  => 'type-servings',
    'type'  => 'json'
  ])
);
?>
--EXPECT--
boolean