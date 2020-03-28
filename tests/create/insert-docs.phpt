--TEST--
insertDocs creates database documents
--FILE--
<?php
require_once __DIR__ . '/../config.php';

echo gettype(
  $couch->insertDocs(DATABASE, [
    'docs' => [
      [
        '_id'       => 'FishStew',
        'servings'  => 4,
        'subtitle'  => 'Delicious with freshly baked bread'
      ],
      [
        '_id'       => 'LambStew',
        'servings'  => 6,
        'subtitle'  => 'Serve with a whole meal scone topping'
      ],
      [
        '_id'       => 'BeefStew',
        'servings'  => 8,
        'subtitle'  => 'Hand-made dumplings make a great accompaniment'
      ]
    ]
  ])
);
?>
--EXPECT--
boolean