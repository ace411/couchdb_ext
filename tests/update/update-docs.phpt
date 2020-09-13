--TEST--
updateDocs updates documents in CouchDb database
--FILE--
<?php
require_once __DIR__ . '/../../couch.php';

echo gettype($couch->updateDocs(DATABASE, [
  'docs' => [
    [
      '_id'       => 'FishStew',
      '_rev'      => '1-41669894c7d25a634f5de4fef75fb982',
      'servings'  => 4,
      'subtitle'  => 'Delicious with freshly baked bread'
    ],
    [
      '_id'       => 'LambStew',
      '_rev'      => '1-599acfa0c7b36889599bde56276e444c',
      'servings'  => 6,
      'subtitle'  => 'Serve with a whole meal scone topping'
    ]
  ]
]));
?>
--EXPECT--
boolean