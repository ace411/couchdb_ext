<?php

/**
 * design document functions
 *
 * @author Lochemem Bruno Michael
 * @license PHP
 * @package couchdb-ext
 */

declare(strict_types=1);

require_once 'config.php';

// create a view design document
$couch->createDdoc(DATABASE, 'meal-doc', [
  'language'  => 'javascript',
  'views'     => [
    'recipe-view' => [
      'map' => 'function (doc) { emit(doc._id, doc.subtitle) }'
    ]
  ],
]);

print_r([
  'metadata'  => $couch->getDoc(DATABASE, '_design/meal-doc'),
  'query'     => $couch->queryView(DATABASE, 'meal-doc', 'recipe-view', [
    'descending'  => true,
    'conflicts'   => false,
    'update'      => true,
  ]),
]);
