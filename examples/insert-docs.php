<?php

/**
 * bulk document insertion
 *
 * @author Lochemem Bruno Michael
 * @license PHP
 * @package couchdb-ext
 */

declare(strict_types=1);

require_once 'config.php';

// docs key is mandatory
echo $couch->insertDocs(DATABASE, [
  'docs' => [
    [
      [
        '_id'       => 'FishStew',
        'servings'  => 4,
        'subtitle'  => 'Delicious with freshly baked bread',
      ],
      [
        '_id'       => 'LambStew',
        'servings'  => 6,
        'subtitle'  => 'Serve with a whole meal scone topping',
      ],
      [
        '_id'       => 'BeefStew',
        'servings'  => 8,
        'subtitle'  => 'Hand-made dumplings make a great accompaniment',
      ],
    ]
  ]
]);
