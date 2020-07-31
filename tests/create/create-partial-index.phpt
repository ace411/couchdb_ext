--TEST--
createIndex creates a CouchDB partial index
--FILE--
<?php
require_once __DIR__ . '/../config.php';

echo gettype(
  $couch->createIndex(DATABASE, [
    'index' => [
      'partial_filter_selector' => [
        'servings' => ['$gt' => 6]
      ],
      'fields'                  => ['servings', 'subtitle', '_id']
    ],
    'ddoc'  => 'type-servings-greater-than-six',
    'type'  => 'json'
  ])
);
?>
--EXPECT--
string