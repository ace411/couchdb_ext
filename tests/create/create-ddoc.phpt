--TEST--
createDdoc creates design document
--FILE--
<?php
require_once __DIR__ . '/../../couch.php';

echo gettype($couch->createDdoc(DATABASE, 'recipe-doc', [
  'language'  => 'javascript',
  'views'     => [
    'recipe-view' => [
      'map' => 'function (doc) { emit(doc._id, doc.subtitle) }'
    ]
  ]
]));
?>
--EXPECT--
boolean