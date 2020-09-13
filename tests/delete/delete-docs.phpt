--TEST--
deleteDocs deletes documents
--FILE--
<?php
require_once __DIR__ . '/../../couch.php';

echo gettype($couch->deleteDocs(DATABASE, [
  'docs' => [
    [
      '_id'   => 'FishStew',
      '_rev'  => '1-41669894c7d25a634f5de4fef75fb982'
    ],
    [
      '_id'   => 'LambStew',
      '_rev'  => '1-599acfa0c7b36889599bde56276e444c'
    ]
  ]
]));
?>
--EXPECT--
boolean